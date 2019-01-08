
import os, glob

from SCons.Script import *

import system
import testsuite
import string

# Set NOCRASH string for tests which we don't want to debug on crash (Windows only)
NOCRASH = ''
if system.is_windows:
   NOCRASH = '-nocrashpopup'

testsuite_common = os.path.abspath(os.path.join('testsuite', 'common'))
#Import('env')
class Test:
   def __init__(self,
                script              = 'Render -proj "%proj%" -rd "%dir%" -r arnold -x 160 -y 120 %options% "%file%"',  # To be set later in prepare_test
                plugin_sources      = '*.c*',
                program_sources     = '',
                program_name        = 'test',
                plugin_dependencies = '',
                output_image        = 'testrender.tif',     ## can be '' if the test does not generate an image
                reference_image     = '',
                progressive         = False,
                kick_params         = '',
                resaved             = False,
                forceexpand         = False,
                scene               = 'test.ma',
                diff_hardfail       = 0.0157,  ## (4/256) oiiotool option --hardfail
                diff_fail           = 0.00001, ## (<1/256) oiiotool option --fail
                diff_failpercent    = 33.334,  ## oiiotool option --failpercent
                diff_warnpercent    = 0.0,     ## oiiottol option --warnpercent
                make_thumbnails     = True,    ## generate images for html display (needs to be disabled for formats with no conversion to 2D image, like deepexr)
                environment         = [],      ## environment variables set before the script is run
                continue_on_failure = False,   ## if this is a multi-command test, keep running even if one of the early stages fails (crashes)
                force_result        = 'OK'):
      ## save the params
      self.script = script
      self.plugin_sources = plugin_sources
      self.program_sources = program_sources
      self.program_name = program_name
      self.plugin_dependencies = plugin_dependencies
      self.output_image = output_image
      self.reference_image = reference_image
      self.progressive = progressive
      self.kick_params = kick_params
      self.resaved = resaved
      self.forceexpand = forceexpand
      self.scene = scene
      self.diff_hardfail = diff_hardfail
      self.diff_fail = diff_fail
      self.diff_failpercent = diff_failpercent
      self.diff_warnpercent = diff_warnpercent
      self.make_thumbnails = make_thumbnails
      self.environment = environment
      self.continue_on_failure = continue_on_failure
      self.force_result = force_result

   @staticmethod
   def CreateTest(test, locals):
      params = dict()
      with open(os.path.join('testsuite', test, 'README'), 'r') as f:
         readme = f.read()
         index = readme.find('PARAMS:')
         if index != -1:
            params = eval(readme[index + 8:], globals(), locals)
      return Test(**params)


   # This function is called when the script was not especified (self.script is null)
   def generate_command_line(self, test_dir, env):
      
      # the test scenes are in the folder 'data'
      test_dir = os.path.join(test_dir, 'data')

      maya_root = env['MAYA_ROOT']
      test_script = self.script 


      if not ' ' in env['MAYA_ROOT']:
         test_script = os.path.join(env['MAYA_ROOT'], 'bin', test_script)

      # if a file test.mel is found, we want to run mayabatch with this script
      # Note that we won't be loading the scene because it seems that the script is called before scene load.
      # So it's the script itself that loads whatever scene it wants
      if os.path.exists(os.path.join(test_dir, 'test.mel')):
         if system.is_windows:
            mayabatch_cmd = 'mayabatch'
         else:
            mayabatch_cmd = 'maya -batch'
         if not ' ' in env['MAYA_ROOT']:
            mayabatch_cmd = os.path.join(env['MAYA_ROOT'], 'bin', mayabatch_cmd)

         test_script = mayabatch_cmd + ' -file %file % -script "' + os.path.join(test_dir, 'test.mel')+ '" -proj "%proj%"'
      # If a file gui_test.mel is found, we want to run maya with this script
      # As above, it's up to the script to load a maya scene
      elif os.path.exists(os.path.join(test_dir, 'gui_test.mel')):

         maya_cmd = 'maya'
         if not ' ' in env['MAYA_ROOT']:
            maya_cmd = os.path.join(env['MAYA_ROOT'], 'bin', maya_cmd)

         test_script = maya_cmd + ' -script "' + os.path.join(test_dir, 'gui_test.mel')+ '" -proj "%proj%"'
      elif os.path.exists(os.path.join(test_dir, 'test.py')):

         maya_root = env['MAYA_ROOT']
         test_script = ''
         if system.is_darwin:   
            test_script = 'python '
            maya_root = maya_root.replace('/Maya.app/Contents', '')

         test_script += os.path.join(test_dir, 'test.py "%s" "%s"' % (maya_root, env['TARGET_MODULE_PATH']) )

      elif os.path.exists(os.path.join(test_dir, 'mayapy_test.py')):
      # If the test.py is found, we want to run mayapy with this script
         maya_root = env['MAYA_ROOT']
         mayapy_cmd = 'mayapy '
         if system.is_darwin:   
            maya_root = maya_root.replace('/Maya.app/Contents', '')
         if not ' ' in env['MAYA_ROOT']:
            mayapy_cmd = os.path.join(env['MAYA_ROOT'], 'bin', mayapy_cmd)
         test_script = mayapy_cmd + os.path.join(test_dir, 'mayapy_test.py "%s" "%s" "%s"' \
            % (maya_root, env['TARGET_MODULE_PATH'], test_dir) )
      # else:
      #    test_script = os.path.join(env['MAYA_ROOT'],'bin',self.script)
      
      

      self.script = test_script

   def prepare_test(self, test_name, env):
      # Silence test preparation by globally overriding the paramater PRINT_CMD_LINE_FUNC
      # in this current SCons sub-environment, used in target generation (Program(),
      # SharedLibrary(), Install(), etc.).
      #print env.Dump('ENV')
      env['PRINT_CMD_LINE_FUNC'] = lambda s, target, src, env : None


      test_dir       = os.path.join(env.Dir('.').srcnode().abspath, test_name)
      test_data_dir  = os.path.join(test_dir, 'data')
      test_build_dir = os.path.join(env.Dir('.').abspath, test_name)

      

      env.VariantDir(test_build_dir, test_data_dir)

      # if os.path.exists(os.path.join(test_data_dir, 'test.cpp')):
      #    if not self.script:
      #       self.script = os.path.join(test_build_dir, 'test')
      #    self.program_name    = 'test'
      #    self.program_sources = 'test.cpp'
      
      # if os.path.exists(os.path.join(test_data_dir, 'test.py')):
      #    if not self.script:
      #       self.script = './test.py'

      # If reference_image was not specified, try to guess from existing files
      if not self.reference_image:
         if os.path.exists(os.path.join(test_dir, 'ref', 'reference.exr')):
            self.reference_image = os.path.join('ref', 'reference.exr')
            self.output_image    = 'testrender.exr'
         elif os.path.exists(os.path.join(test_dir, 'ref', 'reference.tif')):
            self.reference_image = os.path.join('ref', 'reference.tif')

      # If an execution command line was not specified or generated, set the default one
      #if not self.script:
      self.generate_command_line(test_dir, env)

      ## process the current test directory
      ## Step 1: build any shaders/procedurals that might exist
      SHADERS = []
      if self.plugin_sources.find('*') == -1:
         ## just a list of regular file names
         shader_files = [os.path.join(test_data_dir, shader) for shader in Split(self.plugin_sources) if shader != 'test.cpp']
      else:
         ## use recursive glob pattern
         shader_files = []
         for root, dirs, files in os.walk(test_data_dir):
            if '.svn' in dirs:
               dirs.remove('.svn')
            for pattern in Split(self.plugin_sources):
               shader_files += [f for f in glob.glob(os.path.join(root, pattern)) if not os.path.basename(f) == 'test.cpp']
      for shader_file in shader_files:
         BUILD_SHADER_FILE = shader_file.replace(test_data_dir, test_build_dir)
         t = env.SharedLibrary(os.path.splitext(BUILD_SHADER_FILE)[0], BUILD_SHADER_FILE)
         env.Depends(t, LIBAI)
         SHADERS += t
      if self.program_sources != '':
         ## we need to build a program
         t = env.Program(os.path.join(test_build_dir, self.program_name), [os.path.join(test_build_dir, f) for f in Split(self.program_sources)])
         env.Depends(t, LIBAI)
         SHADERS += t
      FILES = []
      FILES += env.Install(test_build_dir, os.path.join(test_dir, 'README'))

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
            FILES += env.Install(d, os.path.join(root, f))
      ## generate the build action that will run the test and produce the html output
      test_target = env.RunTest(os.path.join(test_build_dir, test_name + '.html'), FILES + SHADERS,
         TEST_SCRIPT = self.script,
         REFERENCE_IMAGE = self.reference_image != '' and os.path.join(test_dir, self.reference_image) or '',
         OUTPUT_IMAGE = self.output_image,
         MAKE_THUMBNAILS = self.make_thumbnails,
         DIFF_HARDFAIL = self.diff_hardfail,
         DIFF_FAIL = self.diff_fail,
         DIFF_FAILPERCENT = self.diff_failpercent,
         DIFF_WARNPERCENT = self.diff_warnpercent,
         FORCE_RESULT = self.force_result,
         CONTINUE_ON_FAILURE = self.continue_on_failure,
         TEST_NAME = test_name,
         ENVIRONMENT = self.environment,
         PRINT_CMD_LINE_FUNC = lambda a, b, c, d : None, ## silence the builder
         chdir = 0)

      env.AlwaysBuild(test_target)
      env.Alias(test_name, test_target)

      return test_target  # The test has been prepared and can be run
