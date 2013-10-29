import subprocess, zipfile, shutil
import os, sys

subprocess.call(['less', '-e', os.path.abspath('MtoAEULA.txt')])

inp = raw_input('Please type in Accept to accept the EULA, or press enter to cancel the installer : ')

if inp.replace(' ', '').lower() != 'accept':
    sys.exit(0)


