#!/usr/bin/python

import os
import sys
import subprocess
import time
import psutil
import system
# Kill all the jobs named as hick or husk

exe_extension = '.exe' if system.is_windows else ''
jobNames = ['mayabatch{}'.format(exe_extension)]
if system.is_windows:
  jobNames += ['Render{}'.format(exe_extension)]

# First kill the testsuite job
for proc in psutil.process_iter():
    procName = proc.name()
    if procName.startswith('python'):
        cmdLine = proc.cmdline()
        for cmd in cmdLine:
            if cmd == 'testsuite':
                proc.kill()
                print('---- Killed parent testsuite job')


for proc in psutil.process_iter():
    procName = proc.name()
    for n in jobNames:
       if procName.startswith(n):
          # found a potential job, let's check the command line now
          try:
              print('Killing job {} : {}'.format(proc.pid, proc.cmdline()))
              proc.kill()
          except OSError as e:
              print(e.strerror)

          