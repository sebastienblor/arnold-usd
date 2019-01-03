# vim: filetype=python

from .contrib import colorama
from . import system

class _ANSI_Codes(object):
   def __init__(self, codes):
      for name in dir(codes):
         if not name.startswith('_'):
            setattr(self, name.lower(), '')
   def init_attributes(self, codes):
      for name in dir(codes):
         if not name.startswith('_'):
            setattr(self, name.lower(), getattr(codes, name))

fg = _ANSI_Codes(colorama.Fore)
bg = _ANSI_Codes(colorama.Back)
st = _ANSI_Codes(colorama.Style)

def init():
   global fg, bg, st
   colorama.init(convert=system.is_windows)
   fg.init_attributes(colorama.Fore)
   bg.init_attributes(colorama.Back)
   st.init_attributes(colorama.Style)

def fmt(s, fore=None, back=None, style=None):
   tmp = s
   if style: tmp = style + tmp + st.reset_all
   if fore : tmp = fore  + tmp + fg.reset
   if back : tmp = back  + tmp + bg.reset
   return tmp
