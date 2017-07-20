"""
This module implements the function for performing an indivudal test of a Maya batch render.

It must be in a separate module in order to work properly with the python multiprocessing module
"""
import re
import time
import os
import shutil
import string
import system
import glob
import colorama
import subprocess
import platform
from colorama import Fore, Style
import shlex

from build_tools import process_return_code, saferemove

colorama.init(autoreset=True)

# Obtain information about the system only once, when loaded
os_name = platform.system().lower()

_linux   = 'linux'
_darwin  = 'darwin'
_windows = 'windows'


# These data avoid writing error prone checks like "os.system.os == 'linux'"
is_linux   = os_name == _linux
is_darwin  = os_name == _darwin
is_windows = os_name == _windows

# startupinfo to prevent Windows processes to display a console window
if is_windows:
    _no_window = subprocess.STARTUPINFO()
    _no_window.dwFlags |= subprocess.STARTF_USESHOWWINDOW
else:
    _no_window = None




def cmd_execute(cmd, env=None, cwd=None, verbose=False, shell=False, callback=lambda line: None, timeout=0):
   '''
   Executes a command and returns a tuple with the exit code and the output
   '''
   # Things to do before executing the command:
   # - Split cmd into a list if it is a string
   # - Initialize the output and return codes
   # - Normalize environment to strings
   c = shlex.split(cmd, posix=(not is_windows)) if (type(cmd) == str) and not shell else cmd
   r, o = 0, []
   e = None
   # Create a dictionary with the arguments for subprocess.Popen()
   popen_args = {
      'args'    : c,
      'stdout'  : subprocess.PIPE,
      'stderr'  : subprocess.STDOUT,
      'cwd'     : cwd,
      'env'     : e,
      'shell'   : shell,
      'bufsize' : 1,
      'universal_newlines': True,
   }
   try:
      t = time.time()
      p = subprocess.Popen(**popen_args)
      with p.stdout:
         for line in iter(p.stdout.readline, b''):
            if not line:
               break
            elif timeout and (time.time() - t) > timeout:
               p.kill()
               break
            line = line.rstrip('\n')
            o.append(line)
            callback(line)
            if verbose:
               print(line)
      r = p.wait()
   except OSError as e:
      o = [e.strerror]
      r = e.errno
   return (r, o)


# for windows we use a fake lock to keep the code cleaner
class DummyLock(object):
    def acquire(self):
        pass
    def release(self):
        pass

def do_run_test(args):
    return run_test(*args)

def print_banner(test_name, color_cmds=False):
    f = open('README', 'r')
    summary = f.readline().strip('\n')
    f.close()
    if color_cmds:
        print Fore.MAGENTA + Style.BRIGHT + '='*80
        print Fore.MAGENTA + Style.BRIGHT +  test_name.ljust(15) + Style.RESET_ALL + Fore.MAGENTA + summary
        print Fore.MAGENTA + Style.BRIGHT + '-'*80
    else:
        print '='*80
        print test_name.ljust(15) + summary
        print '-'*80

def run_test(test_name, lock, test_dir, cmd, output_basename, reference_basename, expected_result, oiiotool_path, update_reference=False, show_test_output=True, color_cmds=False):
    os.chdir(test_dir)

    # make a glob out of the output image
    output_image = output_basename + "*.tif"

    fore_magenta = ''
    fore_cyan = ''
    fore_green = ''
    fore_red = ''
    style_bright = ''
    if color_cmds:
        fore_magenta = Fore.MAGENTA
        fore_cyan = Fore.CYAN
        fore_green = Fore.GREEN
        fore_red = Fore.RED
        style_bright = Style.BRIGHT

#	## remove any leftovers
    saferemove(output_image)
    saferemove('new*.jpg')
    saferemove('ref*.jpg')
    saferemove('dif*.jpg')
    saferemove('STATUS*')

    test_dir = os.path.abspath(test_dir)

    # if a plugin folder is found, it's because it contains custom shaders/procedurals
    plugin_dir = os.path.join(test_dir, 'plugin')
    if os.path.exists(plugin_dir):
        os.environ['ARNOLD_PLUGIN_PATH'] = plugin_dir
    else:
        os.unsetenv('ARNOLD_PLUGIN_PATH')

    ## TODO: attach valgrind to each command
    if update_reference:
        show_test_output = False
    output_to_file = not show_test_output

    if show_test_output:
        # if we are showing output we need to lock around the entire thing, which negates all the benefits of multi-threading
        lock.acquire()
        print_banner(test_name, color_cmds)
    else:
        # otherwise, we print a little something so that we know when the process is hung, and print the rest at the end
        lock.acquire()
        print '...starting %s...' % test_name
        lock.release()
    
    references = sorted(glob.glob(reference_basename + '*.tif'))

    # replace test render dir with this test dir
    # image filename "testrender" and format "Tiff" are set in the test scene render settings
    # cmd = string.replace(cmd, "test.log", os.path.join(os.path.abspath(os.path.dirname(test_env['OUTPUT_IMAGE'])), (test_env['test_name']+'.log')))
    cmd = string.replace(cmd, "%proj%", test_dir)
    cmd = string.replace(cmd, "%dir%", test_dir)
    cmd = string.replace(cmd, "%file%", os.path.join(test_dir, 'test.ma'))
    
    print cmd
    # this is a little hack to keep non-AOV tests working. a proper fix would involve
    # opening each file and ensuring that it has "testrender" as its output prefix.
    options = ''
    #if len(references) == 1:
        #options = '-im %s' % output_basename

    # verbose and log options for Render cmd : -verb -log "test.log" 
    if show_test_output:
        cmd = string.replace(cmd, "%options%", options)
        print fore_cyan + cmd
        print fore_magenta + style_bright + '-'*80
    else:
        logfile = "%s.log" % (test_name)
        logfile = os.path.join(test_dir, logfile)
        cmd = string.replace(cmd, "%options%", '-log %s %s' % (logfile, options))
        if system.os() == 'windows':
            cmd = '%s  1> "%s" 2>&1' % (cmd, logfile)
        else:
            cmd = '%s  > "%s" 2>> "%s"' % (cmd, logfile, logfile)
    
    before_time = time.time()
    try:
        retcode = os.system(cmd)
    except KeyboardInterrupt:
        print "child keyboard interrupt"
        return

    running_time = time.time() - before_time
               
    # check if a post_script.py file exists
    post_script = os.path.join(test_dir, 'post_script.py')

    if os.path.exists(post_script):
        print 'Executing post-script'
        import sys; sys.argv = ['post_script.py', test_dir, oiiotool_path]
        execfile(post_script)


    ## redirect test output (if not using os.system
    # if output_to_file:
        # file = open("%s.log" % (test_name), 'w')
        # p = subprocess.Popen(cmd, stdout = file, stderr = subprocess.STDOUT)
        # retcode = p.wait()
        # file.close()
    # else:
        # p = subprocess.Popen(cmd)
        # retcode = p.wait()

    status = process_return_code(retcode)
    if expected_result == 'FAILED':
        # In this case, retcode interpretation is fliped (except for crashed tests)
        if status == 'FAILED':
            status = 'OK'
        elif status == 'OK':
            status = 'FAILED'
    elif expected_result == 'CRASHED':
        if status == 'CRASHED':
            status = 'OK'
        elif status == 'OK':
            status = 'FAILED'
    if status != 'OK':
        cause = 'non-zero return code'

    use_shell = (system.os() != 'windows')
    has_diff = False
    cause = None
    results = []
    if expected_result == 'OK' and reference_basename:
        if status =='OK' and update_reference:
            reference_log = reference_basename + '.log'
            print 'Updating %s ...' % (reference_log)
            shutil.copy('%s.log' % test_name, reference_log)

            full_output_base = os.path.join(test_dir, output_basename)
            for output in glob.glob(full_output_base + '*.tif'):
                reference = output.replace(full_output_base, reference_basename)
                ## the user wants to update the reference image and log
                ## NOTE(boulos): For some reason reference.tif might be read
                ## only, so just remove it first.
                saferemove(reference)
                shutil.copy(output, reference)
                print 'Updating %s ...' % (reference)
            
            references = sorted(glob.glob(reference_basename + '*.tif'))

        for reference in references:
            current_status = 'OK'
            output = reference.replace(reference_basename, output_basename)
            output_stripped = os.path.splitext(output)[0]
            reference_stripped = os.path.split(os.path.splitext(reference)[0])[1]
            new = 'new_%s.jpg' % output_stripped
            dif = 'dif_%s.jpg' % output_stripped
            ref = 'ref_%s.jpg' % reference_stripped
            
            diff_hardfail = 0.01
            diff_fail=0.01
            diff_failpercent = 1
            diff_warnpercent = 1
            channels = 'R,G,B'
            alpha = 'A'

            results.append((new, ref, dif))
            if True : #status =='OK': 
                if os.path.exists(output) and os.path.exists(reference):

                    img_diff_opt = '--threads 1 --hardfail %f --fail %f --failpercent %f --warnpercent %f' % (diff_hardfail, diff_fail, diff_failpercent, diff_warnpercent)
                    img_diff_cmd = ('%s ' + img_diff_opt + ' --diff %s %s') % (oiiotool_path, output, reference)
            
                    img_diff_cmd += ' --sub --abs --cmul 8 -ch "%s,%s" --dup --ch "%s,%s,%s,0" --add -ch "0,1,2" -o dif_testrender.jpg ' % tuple([channels] + [alpha] * 4)
            
                    f = open(os.path.join(test_dir, "%s.diff.log") % test_name, 'w')

                    retcode, out = cmd_execute(img_diff_cmd, cwd=test_dir, shell=use_shell)
                    f.close()

                    if retcode != 0:
                        status = 'FAILED'
                        cause = 'images differ'
                        current_status = 'FAILED'
                    else:
                        #success !
                        # remove the diff file so that we know there is no difference
                        if os.path.exists('dif_testrender.jpg'):
                            os.remove('dif_testrender.jpg')

        
                ## convert these to jpg form for makeweb
                if os.path.exists(output):
                    os.system('%s -i %s -ch "R","G","B" -resize 160x120 -o %s' % (oiiotool_path, output, new))
                else:
                    status = 'FAILED'
                    current_status = 'FAILED'
                    if cause:
                        cause += ", '%s'" % output
                    else:
                        cause = "output image does not exist: '%s'" % output
            if os.path.exists(reference):
                os.system('%s -i %s -ch "R","G","B" -resize 160x120 -o %s' % (oiiotool_path, reference, ref))
                
                
            ## create the html file with the results
            f = open('STATUS_'+dif+'.txt', 'w')
            f.write(current_status) ## so we can get the status of this test later
            f.close()

    if show_test_output:
        print fore_magenta + style_bright + '-'*80
    else:
        lock.acquire()
        print_banner(test_name, color_cmds)
        print "logged to", logfile

    print '%s %s' % ('time'.ljust(15), running_time)
        
    ## progress text (scream if the test didn't pass)
    
    if status == 'OK':
        print fore_green + '%s %s' % ('status'.ljust(15), status)
    else:
        print fore_red + '%s %s' % ('status'.ljust(15), status)
        print fore_red + '%s %s' % ('cause'.ljust(15), cause)
        

    html_file = test_name + '.html'
    print os.path.abspath(html_file)
    
    lock.release()

    ## get README so that we can stick it inside the html file
    f = open('README', 'r')
    readme = ''
    for line in f:
        readme += line
    f.close()

    ## create the html file with the results
    f = open('STATUS', 'w')
    f.write(status) ## so we can get the status of this test later
    f.close()
    
    saferemove(html_file)
    f = open(html_file, 'w')
    f.write('''
<!DOCTYPE html PUBLIC "-//W3C//DTD XHTML 1.0 Transitional//EN" "http://www.w3.org/TR/xhtml1/DTD/xhtml1-transitional.dtd">
<html xmlns="http://www.w3.org/1999/xhtml" lang="en" xml:lang="en">
<head>
<title>
Arnold testsuite - %s
</title>
</head>
<body>
%s
    <table border="0" cellpadding="0">
        <tr>
            <th><font face="Arial">test</font></th>
            <th><font face="Arial">status</font></th>
            <th><font face="Arial"> %s </font></th>
            <th><font face="Arial">new</font></th>
            <th><font face="Arial">ref</font></th>
            <th><font face="Arial">diff</font></th>
        </tr>''' % (test_name,
        len(references) < 2 and ''' '''
        or
        ('''
        <table border="0" cellpadding="0">
        <tr>
        <th><font face="Arial">description</font></th>
        </tr>
        
        <tr>
        <td bgcolor="#ececec">
                         &nbsp;
                         <pre>
                         %s
                         </pre>
                         &nbsp;
        </td>
        </tr>
        </table>
        
        ''' % readme)
        ,
        len(references) < 2 and
            '''description'''
            or '''file'''))
    
    for new, ref, dif in results:

        bgcolor = os.path.exists(dif) and '#ffa0a0' or '#ececec'
        out_img = new.replace('.jpg', '.tif')
        out_img = out_img[4:]
        
        if  os.path.exists(dif) and os.path.exists(out_img):
            test_log = test_name + '<br><br><a href="%s">Log</a><br><br><a href="%s">Download New</a>' % (test_name + '.log', out_img)
        else:
            test_log = test_name + '<br><br><a href="%s">Log</a>' % (test_name + '.log')
        f.write('''
            <tr>
            <td bgcolor="%s">
                <center>
                <font face="Arial">
                &nbsp;%s&nbsp;
                </font>
                </center>
            </td>
            <td bgcolor="%s">
                <center>
                <font face="Arial">
                &nbsp;%s&nbsp;
                </font>
                </center>
            </td>
            %s
            <td bgcolor="#ececec">
                <font face="Arial">
                  %s
                </font>
            </td>
            <td bgcolor="#ececec">
                <font face="Arial">
                  %s
                </font>
            </td>
                <td bgcolor="#ececec">
                <font face="Arial">
                  %s
                </font>
            </td>
            </tr>''' % (
                  bgcolor,
                  test_log,
                  bgcolor,
                  os.path.exists(dif) and ''' FAILED ''' or '''OK''',
                  len(references) < 2 and
                    ( '''<td bgcolor="%s">
                         &nbsp;
                         <pre>
                         %s
                         </pre>
                         &nbsp;
                         </td>''' % (bgcolor, readme))
                    or ( '''<td bgcolor="%s">
                        &nbsp;
                        %s
                        &nbsp;
                        </td>''' % (bgcolor, ref)),
                  os.path.exists(new) and ('''<img src="%s" border="0" hspace="1" width="160" height="120" alt="new image" />''' % new)
                                                      or '''&nbsp;''',
                  os.path.exists(ref) and ('''<img src="%s" border="0" hspace="1" width="160" height="120" alt="ref image" />''' % ref)
                                                      or '''&nbsp;''',
                  os.path.exists(dif) and ('''<img src="%s" border="0" hspace="1" width="160" height="120" alt="difference image" />''' % dif)
                                                      or '''&nbsp;<b>no difference</b>&nbsp;'''
                  )
            )

    f.write('''
    </table>
    <font face="Arial">
    <a href=".">link to files</a>
    </font>
</body>
</html>''')
    f.close()

    return running_time, results

