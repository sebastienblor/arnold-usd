import os 
from SCons.Script import *
import system
import utils
import glob
import testsuite

Import('env')
Import('env BUILD_BASE_DIR MTOA MTOA_SHADERS ')


class MayaTestRunner():
    
    def __init__(self, maya_version = '2018'):
        print "Inited"
        script = 'Render -proj "%proj%" -rd "%dir%" -r arnold -x 160 -y 120 %options% "%file%"'
        if system.is_windows:
            self.script = os.path.normpath(script)
        else:
            self.script = script
        self.plugin_sources = '*.c*'
        self.program_sources = ''
        self.program_name = 'MayaTestRunner'
        self.plugin_dependencies = ''
        self.output_image = 'testrender'
        self.reference_image = os.path.join('ref', 'reference')
        self.force_result = 'OK'
        self.tests = []
        self.test_results = []
        self.total_running_time = 0
        self.black_list = []
        self.gui_list = []
        self.install_files = []
        self.maya_version = maya_version
        # test source directory, before it has been copied to build dir
        self.source_dir  = os.path.abspath(os.path.join('testsuite')) 
        self.build_dir = None
        self.fill_blacklist(maya_version)        
        self.gui_list = utils.find_test_group('gui')

    def fill_blacklist(self, maya_version):
        # blacklist
        self.black_list = utils.find_test_group('ignore')

        # Add tests intended for other platforms to the black list
        for o in system.get_valid_oses():
            if o != system.os():
                self.black_list.extend(utils.find_test_group(o))

        ignore_os = 'ignore_' + system.os()

        self.black_list.extend(utils.find_test_group(ignore_os))

        ignore_version = 'ignore_' + maya_version
        self.black_list.extend(utils.find_test_group(ignore_version))

    def run(self, target, source, env):
        self.results_dir = os.getcwd()
        self.build_dir = os.path.dirname(self.results_dir)
        print "RUnning:"
        test_env = env.Clone()
    
        # add libai to library search path, and Arnold bin dir to path
        if system.is_darwin:
            #test_env.AppendENVPath('DYLD_LIBRARY_PATH', test_env['EXTERNAL_PATH'], envname='ENV', sep=':', delete_existing=1)  
            test_env.AppendENVPath('DYLD_LIBRARY_PATH', test_env['ARNOLD_BINARIES'], envname='ENV', sep=':', delete_existing=1)
            test_env.AppendENVPath('DYLD_LIBRARY_PATH', os.path.join(env['MAYA_ROOT'], 'lib'), sep=':', delete_existing=1)
            previous_lib_path = os.environ.get('DYLD_LIBRARY_PATH')
            os.environ['DYLD_LIBRARY_PATH'] = test_env['ENV']['DYLD_LIBRARY_PATH']
        elif system.is_linux:
            #test_env.AppendENVPath('LD_LIBRARY_PATH', test_env['EXTERNAL_PATH'], envname='ENV', sep=':', delete_existing=1)  
            test_env.AppendENVPath('LD_LIBRARY_PATH', test_env['ARNOLD_BINARIES'], envname='ENV', sep=':', delete_existing=1)
            test_env.AppendENVPath('LD_LIBRARY_PATH', os.path.join(env['MAYA_ROOT'], 'lib'), sep=':', delete_existing=1)
            previous_lib_path = os.environ.get('LD_LIBRARY_PATH')     
            os.environ['LD_LIBRARY_PATH'] = test_env['ENV']['LD_LIBRARY_PATH'] 
        else:
            previous_lib_path = None
           
        ## setup program path   
        previous_path = os.environ.get('PATH')

        path_separator = ':'
        if system.is_windows:
            path_separator = ';'

        #make sure mtoa's kick path is set *before* the arnold one
        test_env['ENV']['PATH'] = os.path.join(test_env['TARGET_MODULE_PATH'], 'bin') + ";" + test_env['ENV']['PATH']
        os.environ['PATH'] = test_env['ENV']['PATH']

        # make sure we'll load the .mod file in the target mtoa foler, and thus load the right version of mtoa
        os.environ['MAYA_MODULE_PATH'] = test_env['TARGET_MODULE_PATH']
       
        ## why this being in Maya.env doesn't work?
        previous_render_desc = os.environ.get('MAYA_RENDER_DESC_PATH')
        os.environ['MAYA_RENDER_DESC_PATH'] = test_env.subst(test_env['TARGET_DESCR_PATH'])
        os.environ['MAYA_VERSION'] = test_env['MAYA_VERSION']
    
        os.environ['PYMEL_SKIP_MEL_INIT'] = "1"
        show_test_output = (test_env['SHOW_TEST_OUTPUT'] == 'always') \
            or (test_env['SHOW_TEST_OUTPUT'] == 'single' and (len(self.tests) == 1))

        # temporarily removing multi-threads testsuite
        oiiotool_path = os.path.join(test_env['ROOT_DIR'], 'external', 'OpenImageIO', 'bin', system.os(), 'oiiotool')
        self.tests.sort()
        print "Running"

    def prepare_test(self, test_name, env):
        print "Prepare Test"
        # Avoid having duplicate test cases
        test_env = env.Clone()
        if test_name in self.tests:
            return False

        test_dir       = os.path.join(self.source_dir, test_name)
        test_data_dir  = os.path.join(test_dir, 'data')
        test_build_dir = os.path.abspath(os.path.join(test_env['BUILD_BASE_DIR'], 'testsuite', test_name))

        test_env.VariantDir(test_build_dir, test_data_dir)
        if test_name in self.black_list:
            print 'skipping test %s -- found in black list' % (test_name)
            return False
        if not (os.path.exists(os.path.join(test_dir, 'README'))):
            print 'skipping test %s -- missing README' % (test_name)
            return False
        

        ## process the current test directory
        ## Step 1: build any shaders/procedurals that might exist
        SHADERS = []
        if self.plugin_sources.find('*') == -1:
            ## just a list of regular file names
            shader_files = Split(self.plugin_sources)
        else:
            ## use recursive glob pattern
            shader_files = []
            for root, dirs, files in os.walk(test_data_dir):
                if '.svn' in dirs:
                    dirs.remove('.svn')
                shader_files += glob.glob(os.path.join(root, self.plugin_sources))
        for c_file in shader_files:
            BUILD_C_FILE = c_file.replace(test_data_dir, test_build_dir)
            SHADERS += test_env.SharedLibrary(os.path.splitext(BUILD_C_FILE)[0], BUILD_C_FILE)
        if not self.program_sources == '':
            ## we need to build a program
            SHADERS += test_env.Program(os.path.join(test_build_dir, self.program_name), [os.path.join(test_build_dir, f) for f in Split(self.program_sources)], LIBS=Split('ai'))

        self.install_files += test_env.Install(test_build_dir, os.path.join(test_dir, 'README'))
        refimg = os.path.join(test_dir, self.reference_image)
        if os.path.exists(refimg):
            self.install_files += test_env.Install(test_build_dir, os.path.join(test_dir, refimg))
   
        for root, dirs, files in os.walk(test_data_dir):
            if '.svn' in dirs:
                dirs.remove('.svn')
            for f in files:
                if os.path.basename(f) == 'Makefile':
                    continue
                if os.path.splitext(f)[1] == 'c':
                    continue
                if os.path.splitext(f)[1] == 'cpp':
                    continue
                d = root
                d = d.replace(test_data_dir, test_build_dir)
                self.install_files += test_env.Install(d, os.path.join(root, f))  
        return True

    def add_test(self, test_dir, env):
        test_name = os.path.basename(test_dir)

        if self.prepare_test(test_name, env):
            self.tests.append(test_name)

    def create_job(self, env):
        # we used to have index.html instead of dummy.html. But we don't actually want previous index.html 
        # files to be removed (which happens when you fix independent tests after running a whole testsuite)
        test_env = env.Clone()
        job = test_env.Testsuite(os.path.abspath(os.path.join(test_env['BUILD_BASE_DIR'], 'testsuite', 'results', 'dummy.html')),
                                self.install_files, # MTOA + MTOA_SHADERS  were removed, I no longer want to trigger a rebuild during the testsuite
                                PRINT_CMD_LINE_FUNC = lambda a, b, c, d : None, ## silence the builder
                                chdir = 1
                                )
        
        test_env.AlwaysBuild(job)
    
        # MTOA + MTOA_SHADERS  were removed, I no longer want to trigger a rebuild during the testsuite
        # This way we can run the tests on an installed package
        #test_env.Depends(job, MTOA)
        #test_env.Depends(job, MTOA_SHADERS)
        return job
        #return True