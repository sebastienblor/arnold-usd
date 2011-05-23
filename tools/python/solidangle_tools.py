import os.path

from build_tools import *

import os, sys, urllib2, getpass, glob, shutil, subprocess
from urlparse import urlparse, urlunparse

TRAC_SERVER = "https://trac.solidangle.com/mtoa"

## Used for web scraping (HTML parsing/modifying)
from BeautifulSoup import BeautifulSoup, Tag, NavigableString

## Generate release notes for current Arnold version, including bugfixes
def get_release_notes(env):

   # Replaces all links (<a ...options...>text</a>) with the text
   def remove_links(node):
      if node != None:
         [a.replaceWith(a.string) for a in node.findAll('a')]

   username = raw_input('Enter your Trac username: ')
   
   # Enter empty user name for early exit, avoiding generating the release notes
   if not username:
      return "Not available"
       
   password = getpass.getpass('Enter Trac password: ')
   
   # this creates a password manager
   passman = urllib2.HTTPPasswordMgrWithDefaultRealm()
   passman.add_password(None, TRAC_SERVER, username, password)
   authhandler = urllib2.HTTPBasicAuthHandler(passman)
   opener = urllib2.build_opener(authhandler)
   urllib2.install_opener(opener)
   # All calls to urllib2.urlopen will now use our handler

   mainurl = TRAC_SERVER + "/milestone/" + get_mtoa_version(3)
   try:
      page = urllib2.urlopen(mainurl)
   except urllib2.URLError, e:
      # Try again with a shorter version string
      mainurl = TRAC_SERVER + "/milestone/" + get_mtoa_version(2)
      try:
         page = urllib2.urlopen(mainurl)
      except:
         print "ERROR: Could not open URL: %s" % e
         return None
   except:
      print "ERROR: Unknown problem accessing URL"
      return None
      
   soup = BeautifulSoup(page)

   # First clean the <HEAD> section
   [elem.extract() for elem in soup.head.findAll('link')]
   [elem.extract() for elem in soup.head.findAll('style')]
   [elem.extract() for elem in soup.head.findAll('script')]

   # Now we go for the <BODY> section
   [elem.extract() for elem in soup.body.findAll('div', id='banner')]
   [elem.extract() for elem in soup.body.findAll('div', id='mainnav')]
   [elem.extract() for elem in soup.body.findAll('div', id='ctxtnav')]
   [elem.extract() for elem in soup.body.findAll('div', id='footer')]
   [elem.extract() for elem in soup.body.findAll('div', id='help')]
   [elem.extract() for elem in soup.body.findAll('div', 'buttons')]
   [elem.extract() for elem in soup.body.findAll('div', 'info')]
   [elem.extract() for elem in soup.body.findAll('form', id='stats')]
   [elem.extract() for elem in soup.body.findAll('script')]
   [elem.extract() for elem in soup.body.findAll('table', 'progress')]
   [elem.extract() for elem in soup.body.findAll('p', 'percent')]

   div = soup.body.find('div', 'description')
   
   # Remove some links to Trac
   dl = soup.body.find('dl')
   remove_links(dl)

   ul = div.find('h2', id='Enhancements')
   if ul != None:
      ul = ul.nextSibling.nextSibling
   while ul != None and ul.name == 'ul':
      remove_links(ul)
      ul = ul.nextSibling
   ul = div.find('h2', id='Plugins')
   if ul != None:
      ul = ul.nextSibling.nextSibling
   while ul != None and ul.name == 'ul':
      remove_links(ul)
      ul = ul.nextSibling
   ul = div.find('h2', id='APIadditions')
   if ul != None:
      ul = ul.nextSibling.nextSibling
   while ul != None and ul.name == 'ul':
      remove_links(ul)
      ul = ul.nextSibling
   ul = div.find('h2', id='Incompatiblechanges')
   if ul != None:
      ul = ul.nextSibling.nextSibling
   while ul != None and ul.name == 'ul':
      remove_links(ul)
      ul = ul.nextSibling
   ul = div.find('h2', id='Deprecated')
   if ul != None:
      ul = ul.nextSibling.nextSibling
   while ul != None and ul.name == 'ul':
      remove_links(ul)
      ul = ul.nextSibling
   
   # Find the top level tag for bug fixes
   tab = div.find('table')

   remove_links(tab)
   
   return soup
   # was return soup.prettify(), but that messes up the formatted output (unwanted spaces)

## Creates a release package for Arnold
def make_package(target, source, env):
   package_name = str(target[0])
   base_pkg_dir = 'package_temp'
   
   # First we make sure the temp directory doesn't exist
   if os.path.exists(base_pkg_dir):
      shutil.rmtree(base_pkg_dir)

   # Copy all package files into the temporary directory   
   for t in env['PACKAGE_FILES']:
      target_dir = os.path.join(base_pkg_dir, str(t[1]))
      file_spec = str(t[0])
      if os.path.isdir(file_spec):
         # This is needed for Python 2.4, as copytree didn't create intermediate directories
         os.makedirs(os.path.dirname(target_dir))
         # If the first element is a directory, use copytree to make a recursive copy
         shutil.copytree(file_spec, target_dir, True)
      else:
         # If the first element is a file or wildcard, create the target dir and copy files separately
         if not os.path.exists(target_dir):
            os.makedirs(target_dir)
         file_list = glob.glob(file_spec)
      
         # Optionally rename the destination file (only for single files)
         if (len(file_list) == 1) and (len(t) == 3):
            copy_file_or_link(file_list[0], os.path.join(target_dir, str(t[2])))
         else:
            for f in file_list:
               copy_file_or_link(f, target_dir)
      
   # Now we have to generate the release notes for this Arnold version. Only
   # perform this step if there is an interactive shell so the user
   # can enter a trac username/password.
   if system.is_terminal_interactive():
      release_notes = get_release_notes(env)
   else:
      release_notes = "no release notes available"
        
   # Write release notes into the 'Changes.html' file
   f = open(os.path.join(base_pkg_dir, 'Changes.html'), 'w')
   f.write(str(release_notes))
   f.close()
   
   saferemove(package_name)

   # For now, we default to .rar for windows and .tar.gz for Linux/Mac
   command_string = ''   
   if system.os() == 'windows':
      if find_in_path('rar.exe') != []:
         command_string = "rar a -ep1 -inul -r %s %s" % (package_name, os.path.join(base_pkg_dir, '*'))
   else:
      # this gives us an explicit list of all the files/directories in 'package_temp/'
      file_list = " ".join(os.listdir('package_temp'))
      command_string = "tar -czf %s -C ./package_temp %s" % (package_name, file_list)

   if command_string != '':
      p = subprocess.Popen(command_string, shell=True, stdout = None)
      retcode = p.wait()
      if retcode != 0:
         print "ERROR: Could not create package '%s'" % package_name

   # Clean temporary directory
   shutil.rmtree(base_pkg_dir)
         
   return None
