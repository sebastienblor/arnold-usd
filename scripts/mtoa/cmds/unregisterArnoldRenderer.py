from __future__ import absolute_import
import maya.cmds as cmds
from . import arnoldShelf
from . import rendererCallbacks

def unregisterArnoldRenderer():

   try:
      cmds.deleteUI('ArnoldMenu', menu=True)
   except RuntimeError as err:
      pass

   try:
      cmds.renderer('arnold', unregisterRenderer=True)
   except RuntimeError as err:
      pass
   try:
      arnoldShelf.removeArnoldShelf()
   except RuntimeError as err:
      pass
   try:
      rendererCallbacks.clearCallbacks()
   except RuntimeError as err:
      pass
      