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
from colorama import Fore, Style

from build_tools import process_return_code, saferemove

colorama.init(autoreset=True)

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

def run_test(test_name, lock, test_dir, cmd, output_basename, reference_basename, expected_result, update_reference=False, show_test_output=True, color_cmds=False):
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
    
    # this is a little hack to keep non-AOV tests working. a proper fix would involve
    # opening each file and ensuring that it has "testrender" as its output prefix.
    options = ''
    if len(references) == 1:
        options = '-im %s' % output_basename

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
            
            results.append((new, ref, dif))
            if True : #status =='OK': 
                if os.path.exists(output) and os.path.exists(reference):

                    ## if the test passed - compare the generated image to the reference
                    difftiff_cmd = 'difftiff -f compare_%s -a 0.5 -m 27 %s %s' % (output, output, reference)
                    if show_test_output:
                        print difftiff_cmd
                    else:
                        if system.os() == 'windows':
                            difftiff_cmd = '%s 1> %s.diff.log 2>&1' % (difftiff_cmd, test_name)
                        else:
                            difftiff_cmd = '%s > %s.diff.log 2>>%s.diff.log' % (difftiff_cmd, test_name, test_name)
                    diff_retcode = os.system(difftiff_cmd)
                    if diff_retcode != 0:
                        status = 'FAILED'
                        current_status = 'FAILED'
                        cause = 'images differ'
                        ## run difftiff again (!) to make one with a solid alpha...
                        if system.os() == 'windows':
                            os.system('difftiff -s -f dif.tif %s %s > nul 2> nul' % (output, reference))
                        else:
                            os.system('difftiff -s -f dif.tif %s %s > /dev/null 2>> /dev/null' % (output, reference))
                        os.system('tiff2jpeg dif.tif %s' % dif)
                        saferemove('dif.tif')
        
                ## convert these to jpg form for makeweb
                if os.path.exists(output):
                    os.system('tiff2jpeg %s %s' % (output, new))
                else:
                    status = 'FAILED'
                    current_status = 'FAILED'
                    if cause:
                        cause += ", '%s'" % output
                    else:
                        cause = "output image does not exist: '%s'" % output
            if os.path.exists(reference):
                os.system('tiff2jpeg %s %s' % (reference, ref))
                
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
        f.write('''
            <tr>
            <td bgcolor="#ececec">
                <center>
                <font face="Arial">
                &nbsp;%s&nbsp;
                </font>
                </center>
            </td>
            <td bgcolor="#ececec">
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
                  test_name,
                  os.path.exists(dif) and ''' FAILED ''' or '''OK''',
                  len(references) < 2 and
                    ( '''<td bgcolor="#ececec">
                         &nbsp;
                         <pre>
                         %s
                         </pre>
                         &nbsp;
                         </td>''' % readme)
                    or ( '''<td bgcolor="#ececec">
                        &nbsp;
                        %s
                        &nbsp;
                        </td>''' % ref),
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

