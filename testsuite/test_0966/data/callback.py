import os
import maya.cmds as cmds
from  arnold import *

def callback(universeId):
   print(__file__)
   print("Universe ID : '{}'".format(universeId))
   universe = AiUniverseGetFromId(universeId)
   node_it = AiUniverseGetNodeIterator(universe, AI_NODE_SHADER)

   while not AiNodeIteratorFinished(node_it):
      node = AiNodeIteratorGetNext(node_it)
      nodename = AiNodeGetName(node)
      print(nodename)
      if nodename == "lambert1":
         AiNodeSetRGB(node, "Kd_color", 0.2,0.8,0.2)
   
   AiNodeIteratorDestroy(node_it)

