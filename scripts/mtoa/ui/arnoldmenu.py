import mtoa.core as core
from mtoa.core import createStandIn, createVolume
from mtoa.ui.ae.aiStandInTemplate import LoadStandInButtonPush
import mtoa.utils as mutils
import maya.cmds as cmds
import maya.mel
import mtoa.txManager
import mtoa.lightManager
import mtoa.renderToTexture
import mtoa.denoise
import mtoa.licensing
import arnold as ai
import mtoa.convertShaders

from uuid import getnode as get_mac
import os
import sys

defaultFolder = ""
defaultOperatorsFolder = ""

_maya_version = mutils.getMayaVersion()

def doCreateStandInFile():
    node = createStandIn()
    LoadStandInButtonPush('{}.dso'.format(node))


def doExportStandIn():
    #Save the defaultType
    core.createOptions()
    default = cmds.optionVar(q='defaultFileExportActiveType')
    defaultBounds = cmds.getAttr('defaultArnoldRenderOptions.outputAssBoundingBox')
    cmds.setAttr('defaultArnoldRenderOptions.outputAssBoundingBox', 1)
    try:
        #Change it to ASS
        cmds.optionVar(sv=('defaultFileExportActiveType', "ASS Export"))
        maya.mel.eval('ExportSelection')
    finally:
        cmds.optionVar(sv=('defaultFileExportActiveType', default))

    cmds.setAttr('defaultArnoldRenderOptions.outputAssBoundingBox', defaultBounds)

def doExportOptionsStandIn():
    core.createOptions()
    defaultBounds = cmds.getAttr('defaultArnoldRenderOptions.outputAssBoundingBox')
    cmds.setAttr('defaultArnoldRenderOptions.outputAssBoundingBox', 1)

    maya.mel.eval('ExportSelectionOptions')
    maya.mel.eval('setCurrentFileTypeOption ExportActive "" "ASS Export"')

    cmds.setAttr('defaultArnoldRenderOptions.outputAssBoundingBox', defaultBounds)

def doCreateCurveCollector():
    # Get selection and group the curves ?
    sls = cmds.ls(sl=True, et='transform')
    curveNode = mutils.createLocator('aiCurveCollector')

    if len(sls) > 0:
        for slsElem in sls:
            print slsElem
            shs = cmds.listRelatives(slsElem, fullPath=True, type='nurbsCurve', allDescendents=True)
            if shs is None:
                continue
            if len(shs):
                for shsElem in shs:
                    sts = cmds.listRelatives(shsElem, fullPath=True, parent=True)
                    if len(sts) > 0:
                        cmds.parent(sts, curveNode[1])
                
    cmds.select(curveNode, replace=True)

def doCreateCustomShape(shapeName):
    shapeNode = mutils.createLocator(shapeName)
    cmds.select(shapeNode, replace=True)

def doCreateOperator(opName):
    opNode = cmds.createNode(opName)
    cmds.select(opNode, replace=True)

def doCreateOldMeshLight():
    sls = cmds.ls(sl=True, fullPath=True, et='transform')
    if len(sls) == 0:
        cmds.confirmDialog(title='Error', message='No transform is selected!', button='Ok')
        return
    shs = cmds.listRelatives(sls[0], fullPath=True, type='mesh')
    if shs is None:
        cmds.confirmDialog(title='Error', message='The selected transform has no meshes', button='Ok')
        return
    elif len(shs) == 0:
        cmds.confirmDialog(title='Error', message='The selected transform has no meshes', button='Ok')
        return
    cmds.setAttr('%s.aiTranslator' % shs[0], 'mesh_light', type='string')

def doCreatePhysicalSky(): 
    skydome = mutils.createLocator('aiSkyDomeLight', asLight=True)
    physicalSkyShader = cmds.createNode('aiPhysicalSky')
    cmds.connectAttr(physicalSkyShader+".outColor", skydome[0] + ".color")

def doCreateLightPortal():

    sls = cmds.ls(sl=False, et='aiSkyDomeLight')
    if len(sls) == 0:
        msg = "Light Portals need a "
        msg += "SkyDome light in the scene"

        result = cmds.confirmDialog(
            title='No SkyDome Light found',
            message=msg
            #button=['OK', 'Cancel'],
            #defaultButton='OK',
            #cancelButton='Cancel',
            #dismissString='Cancel'
            )

        return

    mutils.createLocator('aiLightPortal', asLight=True)



def arnoldAboutDialog():
    legaltext = "Arnold\n\
\n\
© 2018 Autodesk, Inc.  All rights reserved.\n\
\n\
All use of this Software is subject to the terms and conditions of the software license agreement accepted upon installation of this Software and/or packaged with the Software.\n\
\n\
Third-Party Software Credits and Attributions\n\
\n\
Portions relate to The OpenGL Extension Wrangler Library (GLEW) v. 1.10.0:\n\
\n\
Copyright (C) 2008-2016, Nigel Stewart <nigels[]users sourceforge net>.  Copyright (C) 2002-2008, Milan Ikits <milan ikits[]ieee org>.  Copyright (C) 2002-2008, Marcelo E. Magallon <mmagallo[]debian org>.  Copyright (C) 2002, Lev Povalahev.  All rights reserved.  Redistribution and use in source and binary forms, with or without modification, are permitted provided that the following conditions are met: * Redistributions of source code must retain the above copyright notice, this list of conditions and the following disclaimer.  * Redistributions in binary form must reproduce the above copyright notice, this list of conditions and the following disclaimer in the documentation and/or other materials provided with the distribution. * The name of the author may be sed to endorse or promote products derived from this software without specific prior written permission.  THIS SOFTWARE IS PROVIDED BY THE COPYRIGHT HOLDERS AND CONTRIBUTORS \"AS IS\” AND ANY EXPRESS OR IMPLIED WARRANTIES, INCLUDING, BUT NOT LIMITED TO, THE IMPLIED WARRANTIES OF MERCHANTABILITY AND FITNESS FOR A PARTICULAR PURPOSE ARE DISCLAIMED. IN NO EVENT SHALL THE COPYRIGHT OWNER OR CONTRIBUTORS BE LIABLE FOR ANY DIRECT, INDIRECT, INCIDENTAL, SPECIAL, EXEMPLARY, OR CONSEQUENTIAL DAMAGES (INCLUDING, BUT NOT LIMITED TO, PROCUREMENT OF SUBSTITUTE GOODS OR SERVICES; LOSS OF USE, DATA, OR PROFITS; OR BUSINESS INTERRUPTION) HOWEVER CAUSED AND ON ANY THEORY OF LIABILITY, WHETHER IN CONTRACT, STRICT LIABILITY, OR TORT (INCLUDING NEGLIGENCE OR OTHERWISE) ARISING IN ANY WAY OUT OF THE USE OF THIS SOFTWARE, EVEN IF ADVISED OF THE POSSIBILITY OF SUCH DAMAGE.\n\
\n\
Certain portions Copyright (c) Autodesk, Inc. 2016. All rights reserved.\n\
Portions related to OpenImageIO 1.7.17 Copyright 2008-2015 Larry Gritz et al. All Rights Reserved.  Redistribution and use in source and binary forms, with or without modification, are permitted provided that the following conditions are met: * Redistributions of source code must retain the above copyright notice, this list of conditions and the following disclaimer.  * Redistributions in binary form must reproduce the above copyright notice, this list of conditions and the following disclaimer in the documentation and/or other materials provided with the distribution.  * Neither the name of the software's owners nor the names of its contributors may be used to endorse or promote products derived from this software without specific prior written permission.  THIS SOFTWARE IS PROVIDED BY THE COPYRIGHT HOLDERS AND CONTRIBUTORS \"AS IS\" AND ANY EXPRESS OR IMPLIED WARRANTIES, INCLUDING, BUT NOT LIMITED TO, THE IMPLIED WARRANTIES OF MERCHANTABILITY AND FITNESS FOR A PARTICULAR PURPOSE ARE DISCLAIMED. IN NO EVENT SHALL THE COPYRIGHT OWNER OR CONTRIBUTORS BE LIABLE FOR ANY DIRECT, INDIRECT, INCIDENTAL, SPECIAL, EXEMPLARY, OR CONSEQUENTIAL DAMAGES (INCLUDING, BUT NOT LIMITED TO, PROCUREMENT OF SUBSTITUTE GOODS OR SERVICES; LOSS OF USE, DATA, OR PROFITS; OR BUSINESS INTERRUPTION) HOWEVER CAUSED AND ON ANY THEORY OF LIABILITY, WHETHER IN CONTRACT, STRICT LIABILITY, OR TORT (INCLUDING NEGLIGENCE OR OTHERWISE) ARISING IN ANY WAY OUT OF THE USE OF THIS SOFTWARE, EVEN IF ADVISED OF THE POSSIBILITY OF SUCH DAMAGE.\n\
\n\
Portions related to IlmBase 2.2 Copyright (c) 2002-2011, Industrial Light & Magic, a division of Lucasfilm Entertainment Company Ltd. All rights reserved. Redistribution and use in source and binary forms, with or without modification, are permitted provided that the following conditions are met: Redistributions of source code must retain the above copyright notice, this list of conditions and the following disclaimer.  Redistributions in binary form must reproduce the above copyright notice, this list of conditions and the following disclaimer in the documentation and/or other materials provided with the distribution.  Neither the name of Industrial Light & Magic nor the names of its contributors may be used to endorse or promote products derived from this software without specific prior written permission.  THIS SOFTWARE IS PROVIDED BY THE COPYRIGHT HOLDERS AND CONTRIBUTORS \"AS IS\" AND ANY EXPRESS OR IMPLIED WARRANTIES, INCLUDING, BUT NOT LIMITED TO, THE IMPLIED WARRANTIES OF MERCHANTABILITY AND FITNESS FOR A PARTICULAR PURPOSE ARE DISCLAIMED. IN NO EVENT SHALL THE COPYRIGHT OWNER OR CONTRIBUTORS BE LIABLE FOR ANY DIRECT, INDIRECT, INCIDENTAL, SPECIAL, EXEMPLARY, OR CONSEQUENTIAL DAMAGES (INCLUDING, BUT NOT LIMITED TO, PROCUREMENT OF SUBSTITUTE GOODS OR SERVICES; LOSS OF USE, DATA, OR PROFITS; OR BUSINESS INTERRUPTION) HOWEVER CAUSED AND ON ANY THEORY OF LIABILITY, WHETHER IN CONTRACT, STRICT LIABILITY, OR TORT (INCLUDING NEGLIGENCE OR OTHERWISE) ARISING IN ANY WAY OUT OF THE USE OF THIS SOFTWARE, EVEN IF ADVISED OF THE POSSIBILITY OF SUCH DAMAGE.\n\
\n\
Portions related to libjpeg v9b (C) 1991-1998, Thomas G. Lane. All Rights Reserved except as specified below. The authors make NO WARRANTY or representation, either express or implied, with respect to this software, its quality, accuracy, merchantability, or fitness for a particular purpose. This software is provided “AS IS”, and you, its user, assume the entire risk as to its quality and accuracy. Permission is hereby granted to use, copy, modify, and distribute this software (or portions thereof) for any purpose, without fee, subject to these conditions:  (1) If any part of the source code for this software is distributed, then this README file must be included, with this copyright and no-warranty notice unaltered; and any additions, deletions, or changes to the original files must be clearly indicated in accompanying documentation. (2) If only executable code is distributed, then the accompanying documentation must state that \"this software is based in part on the work of the Independent JPEG Group\". (3) Permission for use of this software is granted only if the user accepts full responsibility for any undesirable consequences; the authors accept NO LIABILITY for damages of any kind. These conditions apply to any software derived from or based on the IJG code, not just to the unmodified library. If you use our work, you ought to acknowledge us. Permission is NOT granted for the use of any IJG author's name or company name in advertising or publicity relating to this software or products derived from it. This software may be referred to only as \"the Independent JPEG Group's software\". We specifically permit and encourage the use of this software as the basis of commercial products, provided that all warranty or liability claims are assumed by the product vendor. ansi2knr.c is included in this distribution by permission of L. Peter Deutsch, sole proprietor of its copyright holder, Aladdin Enterprises of Menlo Park, CA. ansi2knr.c is NOT covered by the above copyright and conditions, but instead by the usual distribution terms of the Free Software Foundation; principally, that you must include source code if you redistribute it. (See the file ansi2knr.c for full details). However, since ansi2knr.c is not needed as part of any program generated from the IJG code, this does not limit you more than the foregoing paragraphs do. We are required to state that \"The Graphics Interchange Format(c) is the Copyright property of CompuServe Incorporated. GIF(sm) is a Service Mark property of CompuServe Incorporated.\"\n\
\n\
Portions related to libtiff 4.0.8 Copyright (c) 1988-1997 Sam Leffler.  Copyright (c) 1991-1997 Silicon Graphics, Inc.  Permission to use, copy, modify, distribute, and sell this software and its documentation for any purpose is hereby granted without fee, provided that (i) the above copyright notices and this permission notice appear in all copies of the software and related documentation, and (ii) the names of Sam Leffler and Silicon Graphics may not be used in any advertising or publicity relating to the software without the specific, prior written permission of Sam Leffler and Silicon Graphics.  THE SOFTWARE IS PROVIDED \"AS-IS\" AND WITHOUT WARRANTY OF ANY KIND, EXPRESS, IMPLIED OR OTHERWISE, INCLUDING WITHOUT LIMITATION, ANY WARRANTY OF MERCHANTABILITY OR FITNESS FOR A PARTICULAR PURPOSE.  IN NO EVENT SHALL SAM LEFFLER OR SILICON GRAPHICS BE LIABLE FOR ANY SPECIAL, INCIDENTAL, INDIRECT OR CONSEQUENTIAL DAMAGES OF ANY KIND, OR ANY DAMAGES WHATSOEVER RESULTING FROM LOSS OF USE, DATA OR PROFITS, WHETHER OR NOT ADVISED OF THE POSSIBILITY OF DAMAGE, AND ON ANY THEORY OF LIABILITY, ARISING OUT OF OR IN CONNECTION WITH THE USE OR PERFORMANCE OF THIS SOFTWARE.\n\
\n\
\n\
Portions related to libpng versions 1.0.7, July 1, 2000 through 1.6.32, August 24, 2017 are Copyright (c) 2000-2002, 2004, 2006-2017 Glenn Randers-Pehrson, are\n\
derived from libpng-1.0.6, and are distributed according to the same\n\
disclaimer and license as libpng-1.0.6 with the following individuals\n\
added to the list of Contributing Authors: Simon-Pierre Cadieux, Eric S. Raymond,    Mans Rullgard, Cosmin Truta, Gilles Vollant, James Yu, Mandar Sahastrabuddhe,    Google Inc, Vadim Barkov and with the following additions to the disclaimer:\n\
There is no warranty against interference with your enjoyment of the library or against infringement.  There is no warranty that our efforts or the library will fulfill any of your particular purposes or needs.  This library is provided with all faults, and the entire risk of satisfactory quality, performance, accuracy, and effort is with   the user.\n\
\n\
Some files in the \"contrib\" directory and some configure-generated files that are distributed with libpng have other copyright owners and are released under other open source licenses.\n\
\n\
Portions related to OpenEXR 2.2 Copyright (c) 2002-2011, Industrial Light & Magic, a division of Lucasfilm Entertainment Company Ltd. All rights reserved.  Redistribution and use in source and binary forms, with or without modification, are permitted provided that the following conditions are met: Redistributions of source code must retain the above copyright notice, this list of conditions and the following disclaimer.  Redistributions in binary form must reproduce the above copyright notice, this list of conditions and the following disclaimer in the documentation and/or other materials provided with the distribution.  Neither the name of Industrial Light & Magic nor the names of its contributors may be used to endorse or promote products derived from this software without specific prior written permission.  THIS SOFTWARE IS PROVIDED BY THE COPYRIGHT HOLDERS AND CONTRIBUTORS \"AS IS\" AND ANY EXPRESS OR IMPLIED WARRANTIES, INCLUDING, BUT NOT LIMITED TO, THE IMPLIED WARRANTIES OF MERCHANTABILITY AND FITNESS FOR A PARTICULAR PURPOSE ARE DISCLAIMED. IN NO EVENT SHALL THE COPYRIGHT OWNER OR CONTRIBUTORS BE LIABLE FOR ANY DIRECT, INDIRECT, INCIDENTAL, SPECIAL, EXEMPLARY, OR CONSEQUENTIAL DAMAGES (INCLUDING, BUT NOT LIMITED TO, PROCUREMENT OF SUBSTITUTE GOODS OR SERVICES; LOSS OF USE, DATA, OR PROFITS; OR BUSINESS INTERRUPTION) HOWEVER CAUSED AND ON ANY THEORY OF LIABILITY, WHETHER IN CONTRACT, STRICT LIABILITY, OR TORT (INCLUDING NEGLIGENCE OR OTHERWISE) ARISING IN ANY WAY OUT OF THE USE OF THIS SOFTWARE, EVEN IF ADVISED OF THE POSSIBILITY OF SUCH DAMAGE.\n\
\n\
Portions related to OpenSubdiv 3.1 Copyright (C) 2013 Pixar.  Licensed under the Apache License, Version 2.0 (the \"Apache License\") with the following modification; you may not use this file except in compliance with the Apache License and the following modification to it: Section 6. Trademarks. is deleted and replaced with: 6. Trademarks. This License does not grant permission to use the trade names, trademarks, service marks, or product names of the Licensor and its affiliates, except as required to comply with Section 4(c) of the License and to reproduce the content of the NOTICE file.  You may obtain a copy of the Apache License at http://www.apache.org/licenses/LICENSE-2.0.  Unless required by applicable law or agreed to in writing, software distributed under the Apache License with the above modification is distributed on an \"AS IS\" BASIS, WITHOUT WARRANTIES OR CONDITIONS OF ANY KIND, either express or implied. See the Apache License for the specific language governing permissions and limitations under the Apache License.  \n\
\n\
Portions related to gperftools 2.2.1 Copyright (C) 2005 Google Inc.  All rights reserved.  Redistribution and use in source and binary forms, with or without modification, are permitted provided that the following conditions are met: * Redistributions of source code must retain the above copyright notice, this list of conditions and the following disclaimer.  * Redistributions in binary form must reproduce the above copyright notice, this list of conditions and the following disclaimer in the documentation and/or other materials provided with the distribution.  * Neither the name of Google Inc. nor the names of its contributors may be used to endorse or promote products derived from this software without specific prior written permission.  THIS SOFTWARE IS PROVIDED BY THE COPYRIGHT HOLDERS AND CONTRIBUTORS \"AS IS\" AND ANY EXPRESS OR IMPLIED WARRANTIES, INCLUDING, BUT NOT LIMITED TO, THE IMPLIED WARRANTIES OF MERCHANTABILITY AND FITNESS FOR A PARTICULAR PURPOSE ARE DISCLAIMED. IN NO EVENT SHALL THE COPYRIGHT OWNER OR CONTRIBUTORS BE LIABLE FOR ANY DIRECT, INDIRECT, INCIDENTAL, SPECIAL, EXEMPLARY, OR CONSEQUENTIAL DAMAGES (INCLUDING, BUT NOT LIMITED TO, PROCUREMENT OF SUBSTITUTE GOODS OR SERVICES; LOSS OF USE, DATA, OR PROFITS; OR BUSINESS INTERRUPTION) HOWEVER CAUSED AND ON ANY THEORY OF LIABILITY, WHETHER IN CONTRACT, STRICT LIABILITY, OR TORT (INCLUDING NEGLIGENCE OR OTHERWISE) ARISING IN ANY WAY OUT OF THE USE OF THIS SOFTWARE, EVEN IF ADVISED OF THE POSSIBILITY OF SUCH DAMAGE.\n\
\n\
Portions related to OpenVDB 4.0.0 Copyright (c) 2012-2013 DreamWorks Animation LLC.  All rights reserved. OpenVDB is licensed under the Mozilla Public License v.2.0, which can be found at http://www.mozilla.org/MPL/2.0/.  A text copy of this license and the source code for OpenVDB (and modifications made by Autodesk, if any) can be found at the Autodesk website www.autodesk.com/lgplsource.\n\
\n\
Portions related to SCons Copyright (c) 2001, 2002, 2003, 2004, 2005, 2006, 2007, 2008, 2009, 2010, 2011, 2012, 2013 The SCons Foundation.  Permission is hereby granted, free of charge, to any person obtaining a copy of this software and associated documentation files (the \"Software\"), to deal in the Software without restriction, including without limitation the rights to use, copy, modify, merge, publish, distribute, sublicense, and/or sell copies of the Software, and to permit persons to whom the Software is furnished to do so, subject to the following conditions:  The above copyright notice and this permission notice shall be included in all copies or substantial portions of the Software.  THE SOFTWARE IS PROVIDED \"AS IS\", WITHOUT WARRANTY OF ANY KIND, EXPRESS OR IMPLIED, INCLUDING BUT NOT LIMITED TO THE WARRANTIES OF MERCHANTABILITY, FITNESS FOR A PARTICULAR PURPOSE AND NONINFRINGEMENT. IN NO EVENT SHALL THE AUTHORS OR COPYRIGHT HOLDERS BE LIABLE FOR ANY CLAIM, DAMAGES OR OTHER LIABILITY, WHETHER IN AN ACTION OF CONTRACT, TORT OR OTHERWISE, ARISING FROM, OUT OF OR IN CONNECTION WITH THE SOFTWARE OR THE USE OR OTHER DEALINGS IN THE SOFTWARE.\n\
\n\
Portions related to Boost 1.59 Permission is hereby granted, free of charge, to any person or organization obtaining a copy of the software and accompanying documentation covered by this license (the \"Software\") to use, reproduce, display, distribute, execute, and transmit the Software, and to prepare derivative works of the Software, and to permit third-parties to whom the Software is furnished to do so, all subject to the following:  The copyright notices in the Software and this entire statement, including the above license grant, this restriction and the following disclaimer, must be included in all copies of the Software, in whole or in part, and all derivative works of the Software, unless such copies or derivative works are solely in the form of machine-executable object code generated by a source language processor.  THE SOFTWARE IS PROVIDED \"AS IS\", WITHOUT WARRANTY OF ANY KIND, EXPRESS OR IMPLIED, INCLUDING BUT NOT LIMITED TO THE WARRANTIES OF MERCHANTABILITY, FITNESS FOR A PARTICULAR PURPOSE, TITLE AND NON-INFRINGEMENT. IN NO EVENT SHALL THE COPYRIGHT HOLDERS OR ANYONE DISTRIBUTING THE SOFTWARE BE LIABLE FOR ANY DAMAGES OR OTHER LIABILITY, WHETHER IN CONTRACT, TORT OR OTHERWISE, ARISING FROM, OUT OF OR IN CONNECTION WITH THE SOFTWARE OR THE USE OR OTHER DEALINGS IN THE SOFTWARE.\n\
\n\
Portions related to pystring 1.13 Copyright (c) 2008-2010, Sony Pictures Imageworks Inc.  All rights reserved.  Redistribution and use in source and binary forms, with or without modification, are permitted provided that the following conditions are met:  Redistributions of source code must retain the above copyright notice, this list of conditions and the following disclaimer.  Redistributions in binary form must reproduce the above copyright notice, this list of conditions and the following disclaimer in the documentation and/or other materials provided with the distribution.  Neither the name of the organization Sony Pictures Imageworks nor the names of its contributors may be used to endorse or promote products derived from this software without specific prior written permission.  THIS SOFTWARE IS PROVIDED BY THE COPYRIGHT HOLDERS AND CONTRIBUTORS \"AS IS\" AND ANY EXPRESS OR IMPLIED WARRANTIES, INCLUDING, BUT NOT LIMITED TO, THE IMPLIED WARRANTIES OF MERCHANTABILITY AND FITNESS FOR A PARTICULAR PURPOSE ARE DISCLAIMED. IN NO EVENT SHALL THE COPYRIGHT OWNER OR CONTRIBUTORS BE LIABLE FOR ANY DIRECT, INDIRECT, INCIDENTAL, SPECIAL, EXEMPLARY, OR CONSEQUENTIAL DAMAGES (INCLUDING, BUT NOT LIMITED TO, PROCUREMENT OF SUBSTITUTE GOODS OR SERVICES; LOSS OF USE, DATA, OR PROFITS; OR BUSINESS INTERRUPTION) HOWEVER CAUSED AND ON ANY THEORY OF LIABILITY, WHETHER IN CONTRACT, STRICT LIABILITY, OR TORT (INCLUDING NEGLIGENCE OR OTHERWISE) ARISING IN ANY WAY OUT OF THE USE OF THIS SOFTWARE, EVEN IF ADVISED OF THE POSSIBILITY OF SUCH DAMAGE.\n\
\n\
Portions related to dirent 2050.18.24 Copyright (c) 1999-2003 Apple Computer, Inc.  All Rights Reserved.  This file contains Original Code and/or Modifications of Original Code as defined in and that are subject to the Apple Public Source License Version 2.0 (the \"License\"). You may not use this file except in compliance with the License. Please obtain a copy of the License at http://www.opensource.apple.com/apsl/ and read it before using this file.  The Original Code and all software distributed under the License are distributed on an 'AS IS' basis, WITHOUT WARRANTY OF ANY KIND, EITHER EXPRESS OR IMPLIED, AND APPLE HEREBY DISCLAIMS ALL SUCH WARRANTIES, INCLUDING WITHOUT LIMITATION, ANY WARRANTIES OF MERCHANTABILITY, FITNESS FOR A PARTICULAR PURPOSE, QUIET ENJOYMENT OR NON-INFRINGEMENT.  Please see the License for the specific language governing rights and limitations under the License.\n\
\n\
Portions related to dirent Copyright (c) 1989, 1993 The Regents of the University of California.  All rights reserved.  Redistribution and use in source and binary forms, with or without modification, are permitted provided that the following conditions are met: 1. Redistributions of source code must retain the above copyright notice, this list of conditions and the following disclaimer. 2. Redistributions in binary form must reproduce the above copyright notice, this list of conditions and the following disclaimer in the documentation and/or other materials provided with the distribution. 3. All advertising materials mentioning features or use of this software must display the following acknowledgement:  This product includes software developed by the University of California, Berkeley and its contributors. 4. Neither the name of the University nor the names of its contributors may be used to endorse or promote products derived from this software without specific prior written permission.  THIS SOFTWARE IS PROVIDED BY THE REGENTS AND CONTRIBUTORS ``AS IS'' AND ANY EXPRESS OR IMPLIED WARRANTIES, INCLUDING, BUT NOT LIMITED TO, THE IMPLIED WARRANTIES OF MERCHANTABILITY AND FITNESS FOR A PARTICULAR PURPOSE ARE DISCLAIMED.  IN NO EVENT SHALL THE REGENTS OR CONTRIBUTORS BE LIABLE FOR ANY DIRECT, INDIRECT, INCIDENTAL, SPECIAL, EXEMPLARY, OR CONSEQUENTIAL DAMAGES (INCLUDING, BUT NOT LIMITED TO, PROCUREMENT OF SUBSTITUTE GOODS OR SERVICES; LOSS OF USE, DATA, OR PROFITS; OR BUSINESS INTERRUPTION).  HOWEVER CAUSED AND ON ANY THEORY OF LIABILITY, WHETHER IN CONTRACT, STRICT LIABILITY, OR TORT (INCLUDING NEGLIGENCE OR OTHERWISE) ARISING IN ANY WAY OUT OF THE USE OF THIS SOFTWARE, EVEN IF ADVISED OF THE POSSIBILITY OF SUCH DAMAGE.\n\
\n\
Portions related to OpenColorIO 1.0.9 Copyright (c) 2003-2010 Sony Pictures Imageworks Inc., et al. All Rights Reserved.  Redistribution and use in source and binary forms, with or without modification, are permitted provided that the following conditions are met: Redistributions of source code must retain the above copyright notice, this list of conditions and the following disclaimer. Redistributions in binary form must reproduce the above copyright notice, this list of conditions and the following disclaimer in the documentation and/or other materials provided with the distribution. Neither the name of Sony Pictures Imageworks nor the names of its contributors may be used to endorse or promote products derived from this software without specific prior written permission. THIS SOFTWARE IS PROVIDED BY THE COPYRIGHT HOLDERS AND CONTRIBUTORS “AS IS” AND ANY EXPRESS OR IMPLIED WARRANTIES, INCLUDING, BUT NOT LIMITED TO, THE IMPLIED WARRANTIES OF MERCHANTABILITY AND FITNESS FOR A PARTICULAR PURPOSE ARE DISCLAIMED. IN NO EVENT SHALL THE COPYRIGHT OWNER OR CONTRIBUTORS BE LIABLE FOR ANY DIRECT, INDIRECT, INCIDENTAL, SPECIAL, EXEMPLARY, OR CONSEQUENTIAL DAMAGES (INCLUDING, BUT NOT LIMITED TO, PROCUREMENT OF SUBSTITUTE GOODS OR SERVICES; LOSS OF USE, DATA, OR PROFITS; OR BUSINESS INTERRUPTION) HOWEVER CAUSED AND ON ANY THEORY OF LIABILITY, WHETHER IN CONTRACT, STRICT LIABILITY, OR TORT (INCLUDING NEGLIGENCE OR OTHERWISE) ARISING IN ANY WAY OUT OF THE USE OF THIS SOFTWARE, EVEN IF ADVISED OF THE POSSIBILITY OF SUCH DAMAGE.\n\
\n\
Portions related to Pystring is part of OpenColorIO 1.0.9 Copyright (c) 2008-2010, Sony Pictures Imageworks Inc All rights reserved.  Redistribution and use in source and binary forms, with or without modification, are permitted provided that the following conditions are met:  Redistributions of source code must retain the above copyright notice, this list of conditions and the following disclaimer. Redistributions in binary form must reproduce the above copyright notice, this list of conditions and the following disclaimer in the documentation and/or other materials provided with the distribution. Neither the name of the organization Sony Pictures Imageworks nor the names of its contributors may be used to endorse or promote products derived from this software without specific prior written permission. THIS SOFTWARE IS PROVIDED BY THE COPYRIGHT HOLDERS AND CONTRIBUTORS “AS IS” AND ANY EXPRESS OR IMPLIED WARRANTIES, INCLUDING, BUT NOT LIMITED TO, THE IMPLIED WARRANTIES OF MERCHANTABILITY AND FITNESS FOR A PARTICULAR PURPOSE ARE DISCLAIMED. IN NO EVENT SHALL THE COPYRIGHT OWNER OR CONTRIBUTORS BE LIABLE FOR ANY DIRECT, INDIRECT, INCIDENTAL, SPECIAL, EXEMPLARY, OR CONSEQUENTIAL DAMAGES (INCLUDING, BUT NOT LIMITED TO, PROCUREMENT OF SUBSTITUTE GOODS OR SERVICES; LOSS OF USE, DATA, OR PROFITS; OR BUSINESS INTERRUPTION) HOWEVER CAUSED AND ON ANY THEORY OF LIABILITY, WHETHER IN CONTRACT, STRICT LIABILITY, OR TORT (INCLUDING NEGLIGENCE OR OTHERWISE) ARISING IN ANY WAY OUT OF THE USE OF THIS SOFTWARE, EVEN IF ADVISED OF THE POSSIBILITY OF SUCH DAMAGE.\n\
\n\
Portions related to yaml-cpp that is part of OpenColorIO 1.0.9 Copyright (c) 2008 Jesse Beder.  Permission is hereby granted, free of charge, to any person obtaining a copy of this software and associated documentation files (the “Software”), to deal in the Software without restriction, including without limitation the rights to use, copy, modify, merge, publish, distribute, sublicense, and/or sell copies of the Software, and to permit persons to whom the Software is furnished to do so, subject to the following conditions:  The above copyright notice and this permission notice shall be included in all copies or substantial portions of the Software.  THE SOFTWARE IS PROVIDED “AS IS”, WITHOUT WARRANTY OF ANY KIND, EXPRESS OR IMPLIED, INCLUDING BUT NOT LIMITED TO THE WARRANTIES OF MERCHANTABILITY, FITNESS FOR A PARTICULAR PURPOSE AND NONINFRINGEMENT. IN NO EVENT SHALL THE AUTHORS OR COPYRIGHT HOLDERS BE LIABLE FOR ANY CLAIM, DAMAGES OR OTHER LIABILITY, WHETHER IN AN ACTION OF CONTRACT, TORT OR OTHERWISE, ARISING FROM, OUT OF OR IN CONNECTION WITH THE SOFTWARE OR THE USE OR OTHER DEALINGS IN THE SOFTWARE.\n\
\n\
Portions related to PTEX software that is part of OpenColorIO 1.0.9 2009 Disney Enterprises, Inc. All rights reserved.  Redistribution and use in source and binary forms, with or without modification, are permitted provided that the following conditions are met: Redistributions of source code must retain the above copyright notice, this list of conditions and the following disclaimer.  Redistributions in binary form must reproduce the above copyright notice, this list of conditions and the following disclaimer in the documentation and/or other materials provided with the distribution.  The names “Disney”, “Walt Disney Pictures”, “Walt Disney Animation Studios” or the names of its contributors may NOT be used to endorse or promote products derived from this software without specific prior written permission from Walt Disney Pictures. Disclaimer: THIS SOFTWARE IS PROVIDED BY WALT DISNEY PICTURES AND CONTRIBUTORS “AS IS” AND ANY EXPRESS OR IMPLIED WARRANTIES, INCLUDING, BUT NOT LIMITED TO, THE IMPLIED WARRANTIES OF MERCHANTABILITY, FITNESS FOR A PARTICULAR PURPOSE, NONINFRINGEMENT AND TITLE ARE DISCLAIMED. IN NO EVENT SHALL WALT DISNEY PICTURES, THE COPYRIGHT HOLDER OR CONTRIBUTORS BE LIABLE FOR ANY DIRECT, INDIRECT, INCIDENTAL, SPECIAL, EXEMPLARY, OR CONSEQUENTIAL DAMAGES (INCLUDING, BUT NOT LIMITED TO, PROCUREMENT OF SUBSTITUTE GOODS OR SERVICES; LOSS OF USE, DATA, OR PROFITS; OR BUSINESS INTERRUPTION) HOWEVER CAUSED AND BASED ON ANY THEORY OF LIABILITY, WHETHER IN CONTRACT, STRICT LIABILITY, OR TORT (INCLUDING NEGLIGENCE OR OTHERWISE) ARISING IN ANY WAY OUT OF THE USE OF THIS SOFTWARE, EVEN IF ADVISED OF THE POSSIBILITY OF SUCH DAMAGES.\n\
\n\
Portions related to Little CMS that is part of OpenColorIO 1.0.9 Copyright (c) 1998-2010 Marti Maria Saguer.  Permission is hereby granted, free of charge, to any person obtaining a copy of this software and associated documentation files (the “Software”), to deal in the Software without restriction, including without limitation the rights to use, copy, modify, merge, publish, distribute, sublicense, and/or sell copies of the Software, and to permit persons to whom the Software is furnished to do so, subject to the following conditions: The above copyright notice and this permission notice shall be included in all copies or substantial portions of the Software.  THE SOFTWARE IS PROVIDED “AS IS”, WITHOUT WARRANTY OF ANY KIND, EXPRESS OR IMPLIED, INCLUDING BUT NOT LIMITED TO THE WARRANTIES OF MERCHANTABILITY, FITNESS FOR A PARTICULAR PURPOSE AND NONINFRINGEMENT. IN NO EVENT SHALL THE AUTHORS OR COPYRIGHT HOLDERS BE LIABLE FOR ANY CLAIM, DAMAGES OR OTHER LIABILITY, WHETHER IN AN ACTION OF CONTRACT, TORT OR OTHERWISE, ARISING FROM, OUT OF OR IN CONNECTION WITH THE SOFTWARE OR THE USE OR OTHER DEALINGS IN THE SOFTWARE.\n\
\n\
Portions related to argparse that is part of OpenColorIO 1.0.9 Copyright 2008 Larry Gritz and the other authors and contributors. All Rights Reserved. Based on BSD-licensed software Copyright 2004 NVIDIA Corp.  Redistribution and use in source and binary forms, with or without modification, are permitted provided that the following conditions are met: * Redistributions of source code must retain the above copyright notice, this list of conditions and the following disclaimer. * Redistributions in binary form must reproduce the above copyright notice, this list of conditions and the following disclaimer in the documentation and/or other materials provided with the distribution. * Neither the name of the software’s owners nor the names of its contributors may be used to endorse or promote products derived from this software without specific prior written permission. THIS SOFTWARE IS PROVIDED BY THE COPYRIGHT HOLDERS AND CONTRIBUTORS “AS IS” AND ANY EXPRESS OR IMPLIED WARRANTIES, INCLUDING, BUT NOT LIMITED TO, THE IMPLIED WARRANTIES OF MERCHANTABILITY AND FITNESS FOR A PARTICULAR PURPOSE ARE DISCLAIMED. IN NO EVENT SHALL THE COPYRIGHT OWNER OR CONTRIBUTORS BE LIABLE FOR ANY DIRECT, INDIRECT, INCIDENTAL, SPECIAL, EXEMPLARY, OR CONSEQUENTIAL DAMAGES (INCLUDING, BUT NOT LIMITED TO, PROCUREMENT OF SUBSTITUTE GOODS OR SERVICES; LOSS OF USE, DATA, OR PROFITS; OR BUSINESS INTERRUPTION) HOWEVER CAUSED AND ON ANY THEORY OF LIABILITY, WHETHER IN CONTRACT, STRICT LIABILITY, OR TORT (INCLUDING NEGLIGENCE OR OTHERWISE) ARISING IN ANY WAY OUT OF THE USE OF THIS SOFTWARE, EVEN IF ADVISED OF THE POSSIBILITY OF SUCH DAMAGE.\n\
\n\
Portions related to Python v. 2.5, 2.6, 2.7 Copyright (c) 2001, 2002, 2003, 2004, 2005, 2006 Python Software Foundation; All Rights Reserved.\n\
\n\
Portions related to JsonCpp 1.8.0 Copyright (c) 2007-2010 Baptiste Lepilleur.  Permission is hereby granted, free of charge, to any person obtaining a copy of this software and associated documentation files (the \"Software\"), to deal in the Software without restriction, including without limitation the rights to use, copy, modify, merge, publish, distribute, sublicense, and/or sell copies of the Software, and to permit persons to whom the Software is furnished to do so, subject to the following conditions: The above copyright notice and this permission notice shall be included in all copies or substantial portions of the Software.  THE SOFTWARE IS PROVIDED \"AS IS\", WITHOUT WARRANTY OF ANY KIND, EXPRESS OR IMPLIED, INCLUDING BUT NOT LIMITED TO THE WARRANTIES OF MERCHANTABILITY, FITNESS FOR A PARTICULAR PURPOSE AND NONINFRINGEMENT. IN NO EVENT SHALL THE AUTHORS OR COPYRIGHT HOLDERS BE LIABLE FOR ANY CLAIM, DAMAGES OR OTHER LIABILITY, WHETHER IN AN ACTION OF CONTRACT, TORT OR OTHERWISE, ARISING FROM, OUT OF OR IN CONNECTION WITH THE SOFTWARE OR THE USE OR OTHER DEALINGS IN THE SOFTWARE.\n\
\n\
Portions related to psutil 5.1.1 Copyright (c) 2009, Jay Loden, Dave Daeschler, Giampaolo Rodola'.  All rights reserved.  Redistribution and use in source and binary forms, with or without modification, are permitted provided that the following conditions are met: * Redistributions of source code must retain the above copyright notice, this list of conditions and the following disclaimer. * Redistributions in binary form must reproduce the above copyright notice, this list of conditions and the following disclaimer in the documentation and/or other materials provided with the distribution. * Neither the name of the psutil authors nor the names of its contributors may be used to endorse or promote products derived from this software without specific prior written permission. THIS SOFTWARE IS PROVIDED BY THE COPYRIGHT HOLDERS AND CONTRIBUTORS \"AS IS\" AND ANY EXPRESS OR IMPLIED WARRANTIES, INCLUDING, BUT NOT LIMITED TO, THE IMPLIED WARRANTIES OF MERCHANTABILITY AND FITNESS FOR A PARTICULAR PURPOSE ARE DISCLAIMED. IN NO EVENT SHALL THE COPYRIGHT OWNER OR CONTRIBUTORS BE LIABLE FOR ANY DIRECT, INDIRECT, INCIDENTAL, SPECIAL, EXEMPLARY, OR CONSEQUENTIAL DAMAGES (INCLUDING, BUT NOT LIMITED TO, PROCUREMENT OF SUBSTITUTE GOODS OR SERVICES;\n\
LOSS OF USE, DATA, OR PROFITS; OR BUSINESS INTERRUPTION) HOWEVER CAUSED AND ON ANY THEORY OF LIABILITY, WHETHER IN CONTRACT, STRICT LIABILITY, OR TORT (INCLUDING NEGLIGENCE OR OTHERWISE) ARISING IN ANY WAY OUT OF THE USE OF THIS SOFTWARE, EVEN IF ADVISED OF THE POSSIBILITY OF SUCH DAMAGE.\n\
\n\
Portions related to Open Shading Language 1.9.0 Copyright © 2016 Chaos Software.  Permission is hereby granted, free of charge, to any person obtaining a copy of this software and associated documentation files (the \"Software\"), to deal in the Software without restriction, including without limitation the rights to use, copy, modify, merge, publish, distribute, sublicense, and/or sell copies of the Software, and to permit persons to whom the Software is furnished to do so, subject to the following conditions: The above copyright notice and this permission notice shall be included in all copies or substantial portions of the Software. THE SOFTWARE IS PROVIDED \"AS IS\", WITHOUT WARRANTY OF ANY KIND, EXPRESS OR IMPLIED, INCLUDING BUT NOT LIMITED TO THE WARRANTIES OF MERCHANTABILITY, FITNESS FOR A PARTICULAR PURPOSE AND NONINFRINGEMENT. IN NO EVENT SHALL THE AUTHORS OR COPYRIGHT HOLDERS BE LIABLE FOR ANY CLAIM, DAMAGES OR OTHER LIABILITY, WHETHER IN AN ACTION OF CONTRACT, TORT OR OTHERWISE, ARISING FROM, OUT OF OR IN CONNECTION WITH THE SOFTWARE OR THE USE OR OTHER DEALINGS IN THE SOFTWARE.\n\
\n\
Portions Copyright (C) 2005-2015 Intel Corporation. Intel Threading Building Blocks v. 4.2 is distributed with this Autodesk software as a separate work. Intel Threading Building Blocks is licensed under the GNU General Public License v.2 with the Runtime Exception, which can be found at http://www.threadingbuildingblocks.org/. A text copy of this license and the source code for Intel Threading Building Blocks v. 4.2 (and modifications made by Autodesk, if any) can be found at the Autodesk website www.autodesk.com/lgplsource.\n\
\n\
Copyright (C) 1989, 1991 Free Software Foundation, Inc.,\n\
 51 Franklin Street, Fifth Floor, Boston, MA 02110-1301 USA\n\
 Everyone is permitted to copy and distribute verbatim copies\n\
 of this license document, but changing it is not allowed.\n\
\n\
Portions Copyright (c) 2007-2014 iMatix Corporation and Contributors. This Autodesk software contains ZeroMQ 3.2.2.  ZeroMQ is licensed under the GNU Lesser General Public License v.2.1, which can be found at http://www.gnu.org/licenses/old-licenses/lgpl-2.1.txt.  A text copy of this license and the source code for ZeroMQ v.3.2.2 can be found at the Autodesk website www.autodesk.com/lgplsource or by sending a written request to:\n\
\n\
Autodesk, Inc.\n\
Attention:  General Counsel\n\
Legal Department\n\
111 McInnis Parkway\n\
San Rafael, CA 94903\n\
\n\
Your written request must:\n\
\n\
1.  Contain a self-addressed CD/DVD mailer (or envelope sufficiently large to hold a DVD) with postage sufficient to cover the amount of the current U.S. Post Office First Class postage rate for CD/DVD mailers (or the envelope you have chosen) weighing  5 ounces from San Rafael, California USA to your indicated address; and\n\
\n\
\n\
2.  Identify:\n\
    a.  This Autodesk software name and release number;\n\
    b.  That you are requesting the source code for ZeroMQ v. 3.2.2; and\n\
    c.  The above URL (www.autodesk.com/lgplsource)\n\
\n\
so that Autodesk may properly respond to your request.  The offer to receive this ZeroMQ source code via the above URL (www.autodesk.com/lgplsource) or by written request to Autodesk is valid for a period of three (3) years from the date you purchased your license to this Autodesk software.\n\
\n\
You may modify, debug and relink ZeroMQ to this Autodesk software as provided under the terms of the GNU Lesser General Public License v.2.1.\n\
\n\
Portions Copyright (C) 1992-2006 Trolltech ASA. All rights reserved.  This Autodesk software contains Qt v.4.2.  Qt is licensed under the GNU Lesser General Public License v.2.1, which can be found at http://www.gnu.org/licenses/old-licenses/lgpl-2.1.txt.  A text copy of this license is included on the media provided by Autodesk or with the download of this Autodesk software.  A text copy of this license and the source code for Qt v.4.2 can be found at the Autodesk website www.autodesk.com/lgplsource or by sending a written request to:\n\
\n\
Autodesk, Inc.\n\
Attention:  General Counsel\n\
Legal Department\n\
111 McInnis Parkway\n\
San Rafael, CA 94903\n\
\n\
Your written request must:\n\
\n\
1.  Contain a self-addressed CD/DVD mailer (or envelope sufficiently large to hold a DVD) with postage sufficient to cover the amount of the current U.S. Post Office First Class postage rate for CD/DVD mailers (or the envelope you have chosen) weighing  5 ounces from San Rafael, California USA to your indicated address; and\n\
\n\
\n\
2.  Identify:\n\
    a.  This Autodesk software name and release number;\n\
    b.  That you are requesting the source code for Qt v. 4.2; and\n\
    c.  The above URL (www.autodesk.com/lgplsource)\n\
\n\
so that Autodesk may properly respond to your request.  The offer to receive this Qt source code via the above URL (www.autodesk.com/lgplsource) or by written request to Autodesk is valid for a period of three (3) years from the date you purchased your license to this Autodesk software.\n\
\n\
You may modify, debug and relink Qt to this Autodesk software as provided under the terms of the GNU Lesser General Public License v.2.1.\n\
\n\
\n\
Autodesk Color Management  \n\
\n\
Portions related to GLEE v. 5.4.0 Copyright © 2009 Ben Woodhouse.  All rights reserved. Redistribution and use in source and binary forms, with or without modification, are permitted provided that the following conditions are met: 1. Redistributions of source code must retain the above copyright notice, this list of conditions and the following disclaimer as the first lines of this file unmodified. 2. Redistributions in binary form must reproduce the above copyright notice, this list of conditions and the following disclaimer in the documentation and/or other materials provided with the distribution. THIS SOFTWARE IS PROVIDED BY BEN WOODHOUSE “AS IS” AND ANY EXPRESS OR IMPLIED WARRANTIES, INCLUDING, BUT NOT LIMITED TO, THE IMPLIED WARRANTIES OF MERCHANTABILITY AND FITNESS FOR A PARTICULAR PURPOSE ARE DISCLAIMED. IN NO EVENT SHALL BEN WOODHOUSE BE LIABLE FOR ANY DIRECT, INDIRECT, INCIDENTAL, SPECIAL, EXEMPLARY, OR CONSEQUENTIAL DAMAGES (INCLUDING, BUT NOT LIMITED TO, PROCUREMENT OF SUBSTITUTE GOODS OR SERVICES; LOSS OF USE, DATA, OR PROFITS; OR BUSINESS INTERRUPTION) HOWEVER CAUSED AND ON ANY THEORY OF LIABILITY, WHETHER IN CONTRACT, STRICT LIABILITY, OR TORT (INCLUDING NEGLIGENCE OR OTHERWISE) ARISING IN ANY WAY OUT OF THE USE OF THIS SOFTWARE, EVEN IF ADVISED OF THE POSSIBILITY OF SUCH DAMAGE.\n\
\n\
Portions related to OpenColorIO v 1.0.9 Copyright (c) 2003-2010 Sony Pictures Imageworks Inc., et al.  All Rights Reserved.  Redistribution and use in source and binary forms, with or without modification, are permitted provided that the following conditions are met: * Redistributions of source code must retain the above copyright notice, this list of conditions and the following disclaimer.  * Redistributions in binary form must reproduce the above copyright notice, this list of conditions and the following disclaimer in the documentation and/or other materials provided with the distribution.  * Neither the name of Sony Pictures Imageworks nor the names of its contributors may be used to endorse or promote products derived from this software without specific prior written permission. THIS SOFTWARE IS PROVIDED BY THE COPYRIGHT HOLDERS AND CONTRIBUTORS \"AS IS\" AND ANY EXPRESS OR IMPLIED WARRANTIES, INCLUDING, BUT NOT LIMITED TO, THE IMPLIED WARRANTIES OF MERCHANTABILITY AND FITNESS FOR A PARTICULAR PURPOSE ARE DISCLAIMED. IN NO EVENT SHALL THE COPYRIGHT OWNER OR CONTRIBUTORS BE LIABLE FOR ANY DIRECT, INDIRECT, INCIDENTAL, SPECIAL, EXEMPLARY, OR CONSEQUENTIAL DAMAGES (INCLUDING, BUT NOT LIMITED TO, PROCUREMENT OF SUBSTITUTE GOODS OR SERVICES; LOSS OF USE, DATA, OR PROFITS; OR BUSINESS INTERRUPTION) HOWEVER CAUSED AND ON ANY THEORY OF LIABILITY, WHETHER IN CONTRACT, STRICT LIABILITY, OR TORT (INCLUDING NEGLIGENCE OR OTHERWISE) ARISING IN ANY WAY OUT OF THE USE OF THIS SOFTWARE, EVEN IF ADVISED OF THE POSSIBILITY OF SUCH DAMAGE.\n\
\n\
Portions related to SampleICC v 1.6.6 Copyright (c) 2003-2010 The International Color Consortium. All rights reserved.  Redistribution and use in source and binary forms, with or without modification, are permitted provided that the following conditions are met: 1. Redistributions of source code must retain the above copyright notice, this list of conditions and the following disclaimer.  2. Redistributions in binary form must reproduce the above copyright notice, this list of conditions and the following disclaimer in the documentation and/or other materials provided with the distribution.  3. In the absence of prior written permission, the names \"ICC\" and \"The International Color Consortium\" must not be used to imply that the ICC organization endorses or promotes products derived from this software.  THIS SOFTWARE IS PROVIDED ''AS IS'' AND ANY EXPRESSED OR IMPLIED WARRANTIES, INCLUDING, BUT NOT LIMITED TO, THE IMPLIED WARRANTIES OF MERCHANTABILITY AND FITNESS FOR A PARTICULAR PURPOSE ARE DISCLAIMED.  IN NO EVENT SHALL THE INTERNATIONAL COLOR CONSORTIUM OR ITS CONTRIBUTING MEMBERS BE LIABLE FOR ANY DIRECT, INDIRECT, INCIDENTAL, SPECIAL, EXEMPLARY, OR CONSEQUENTIAL DAMAGES (INCLUDING, BUT NOT LIMITED TO, PROCUREMENT OF SUBSTITUTE GOODS OR SERVICES; LOSS OF USE, DATA, OR PROFITS; OR BUSINESS INTERRUPTION) HOWEVER CAUSED AND ON ANY THEORY OF LIABILITY, WHETHER IN CONTRACT, STRICT LIABILITY, OR TORT (INCLUDING NEGLIGENCE OR OTHERWISE) ARISING IN ANY WAY OUT OF THE USE OF THIS SOFTWARE, EVEN IF ADVISED OF THE POSSIBILITY OF SUCH DAMAGE.  This software consists of voluntary contributions made by many individuals on behalf of the The International Color Consortium.  Membership in the ICC is encouraged when this software is used for commercial purposes.  For more information on The International Color Consortium, please see <http://www.color.org/>.\n\
\n\
Portions related to SampleICC v 1.6.6 Copyright 2001-2004 Unicode, Inc.  \n\
\n\
Disclaimer\n\
This source code is provided as is by Unicode, Inc. No claims are made as to fitness for any particular purpose. No warranties of any kind are expressed or implied. The recipient agrees to determine applicability of information provided. If this file has been purchased on magnetic or optical media from Unicode, Inc., the sole remedy for any claim will be exchange of defective media within 90 days of receipt.\n\
\n\
Limitations on Rights to Redistribute This Code\n\
Unicode, Inc. hereby grants the right to freely use the information supplied in this file in the creation of products supporting the Unicode Standard, and to make copies of this file in any form for internal or external distribution as long as this notice remains attached.\n\
\n\
Portions related to SampleICC v 1.6.6 Copyright (c) 1994 SunSoft, Inc.  Rights Reserved.  Permission is hereby granted, free of charge, to any person obtaining a copy of this software and associated documentation files (the \"Software\"), to deal in the Software without restriction, including without limitation the rights to use, copy, modify,  merge, publish distribute, sublicense, and/or sell copies of the Software, and to permit persons to whom the Software is furnished to do so, subject to the following conditions: The above copyright notice and this permission notice shall be included in all copies or substantial portions of the Software.  THE SOFTWARE IS PROVIDED \"AS IS\", WITHOUT WARRANTY OF ANY KIND, EXPRESS OR IMPLIED, INCLUDING BUT NOT LIMITED TO THE WARRANTIES \n\
OF MERCHANTABILITY, FITNESS FOR A PARTICULAR PURPOSE AND NON-INFRINGEMENT.  IN NO EVENT SHALL SUNSOFT, INC. OR ITS PARENT COMPANY BE LIABLE FOR ANY CLAIM, DAMAGES OR OTHER LIABILITY, WHETHER IN AN ACTION OF CONTRACT, TORT OR OTHERWISE, ARISING FROM, OUT OF OR IN CONNECTION WITH THE SOFTWARE OR THE USE OR OTHER DEALINGS IN THE SOFTWARE.  Except as contained in this notice, the name of SunSoft, Inc. shall not be used in advertising or otherwise to promote the sale, use or other dealings in this Software without written authorization from SunSoft Inc.   \n\
\n\
Portions related to eXpat v 1.2 Copyright (c) 1998, 1999 Thai Open Source Software Center Ltd.  Copyright (c) 1998, 1999, 2000 Thai Open Source Software Center Ltd.  Copyright 2000, Clark Cooper.  Copyright (c) 1998, 1999, 2000 Thai Open Source Software Center Ltd and Clark Cooper .  Copyright (c) 2001, 2002, 2003, 2004, 2005, 2006 Expat maintainers. Permission is hereby granted, free of charge, to any person obtaining a copy of this software and associated documentation files (the \"Software\"), to deal in the Software without restriction, including without limitation the rights to use, copy, modify, merge, publish, distribute, sublicense, and/or sell copies of the Software, and to permit persons to whom the Software is furnished to do so, subject to the following conditions: The above copyright notice and this permission notice shall be included in all copies or substantial portions of the Software.  THE SOFTWARE IS PROVIDED \"AS IS\", WITHOUT WARRANTY OF ANY KIND, EXPRESS OR IMPLIED, INCLUDING BUT NOT LIMITED TO THE WARRANTIES OF MERCHANTABILITY, FITNESS FOR A PARTICULAR PURPOSE AND NONINFRINGEMENT.  IN NO EVENT SHALL THE AUTHORS OR COPYRIGHT HOLDERS BE LIABLE FOR ANY CLAIM, DAMAGES OR OTHER LIABILITY, WHETHER IN AN ACTION OF CONTRACT, TORT OR OTHERWISE, ARISING FROM, OUT OF OR IN CONNECTION WITH THE SOFTWARE OR THE USE OR OTHER DEALINGS IN THE SOFTWARE.\n\
\n\
Portions related to Boost Version 1.59 Copyright © - August 17th, 2003.  Permission is hereby granted, free of charge, to any person or organization obtaining a copy of the software and accompanying documentation covered by this license (the \"Software\") to use, reproduce, display, distribute, execute, and transmit the Software, and to prepare derivative works of the Software, and to permit third-parties to whom the Software is furnished to do so, all subject to the following:  The copyright notices in the Software and this entire statement, including the above license grant, this restriction and the following disclaimer, must be included in all copies of the Software, in whole or in part, and all derivative works of the Software, unless such copies or derivative works are solely in the form of machine-executable object code generated by a source language processor. THE SOFTWARE IS PROVIDED \"AS IS\", WITHOUT WARRANTY OF ANY KIND, EXPRESS OR IMPLIED, INCLUDING BUT NOT LIMITED TO THE WARRANTIES OF MERCHANTABILITY, FITNESS FOR A PARTICULAR PURPOSE, TITLE AND NON-INFRINGEMENT. IN NO EVENT SHALL THE COPYRIGHT HOLDERS OR ANYONE DISTRIBUTING THE SOFTWARE BE LIABLE FOR ANY DAMAGES OR OTHER LIABILITY, WHETHER IN CONTRACT, TORT OR OTHERWISE, ARISING FROM, OUT OF OR IN CONNECTION WITH THE SOFTWARE OR THE USE OR OTHER DEALINGS IN THE SOFTWARE.\n\
\n\
\n"
    # Arnold core additional text

    legaltext += "Portions related to Halide 2018_02_15 46d8e9e Copyright (c) 2012-2014 MIT CSAIL, Google Inc., and other contributors Developed by: The Halide team http://halide-lang.org   Permission is hereby granted, free of charge, to any person obtaining a copy of this software and associated documentation files (the \"Software\"), to deal in the Software without restriction, including without limitation the rights to use, copy, modify, merge, publish, distribute, sublicense, and/or sell copies of the Software, and to permit persons to whom the Software is furnished to do so, subject to the following conditions:   The above copyright notice and this permission notice shall be included in all copies or substantial portions of the Software.   THE SOFTWARE IS PROVIDED \"AS IS\", WITHOUT WARRANTY OF ANY KIND, EXPRESS OR IMPLIED, INCLUDING BUT NOT LIMITED TO THE WARRANTIES OF MERCHANTABILITY, FITNESS FOR A PARTICULAR PURPOSE AND NONINFRINGEMENT. IN NO EVENT SHALL THE AUTHORS OR COPYRIGHT HOLDERS BE LIABLE FOR ANY CLAIM, DAMAGES OR OTHER LIABILITY, WHETHER IN AN ACTION OF CONTRACT, TORT OR OTHERWISE, ARISING FROM, OUT OF OR IN CONNECTION WITH THE SOFTWARE OR THE USE OR OTHER DEALINGS IN THE SOFTWARE.\n\
\n\
\n\
Portions related to Alembic 1.7.1 TM & © 2009-2015 Lucasfilm Entertainment Company Ltd. or Lucasfilm Ltd.  All rights reserved.  Industrial Light & Magic, ILM and the Bulb and Gear design logo are all registered trademarks or service marks of Lucasfilm Ltd.  © 2009-2015 Sony Pictures Imageworks Inc.  All rights reserved. Redistribution and use in source and binary forms, with or without modification, are permitted provided that the following conditions are met: * Redistributions of source code must retain the above copyright notice, this list of conditions and the following disclaimer. * Redistributions in binary form must reproduce the above copyright notice, this list of conditions and the following disclaimer in the documentation and/or other materials provided with the distribution. * Neither the name of Industrial Light & Magic nor the names of its contributors may be used to endorse or promote products derived from this software without specific prior written permission.  THIS SOFTWARE IS PROVIDED BY THE COPYRIGHT HOLDERS AND CONTRIBUTORS \"AS IS\" AND ANY EXPRESS OR IMPLIED WARRANTIES, INCLUDING, BUT NOT LIMITED TO, THE IMPLIED WARRANTIES OF MERCHANTABILITY AND FITNESS FOR A PARTICULAR PURPOSE ARE DISCLAIMED. IN NO EVENT SHALL THE COPYRIGHT OWNER OR CONTRIBUTORS BE LIABLE FOR ANY DIRECT, INDIRECT, INCIDENTAL, SPECIAL, EXEMPLARY, OR CONSEQUENTIAL DAMAGES (INCLUDING, BUT NOT LIMITED TO, PROCUREMENT OF SUBSTITUTE GOODS OR SERVICES; LOSS OF USE, DATA, OR PROFITS; OR BUSINESS INTERRUPTION) HOWEVER CAUSED AND ON ANY THEORY OF LIABILITY, WHETHER IN CONTRACT, STRICT LIABILITY, OR TORT (INCLUDING NEGLIGENCE OR OTHERWISE) ARISING IN ANY WAY OUT OF THE USE OF THIS SOFTWARE, EVEN IF ADVISED OF THE POSSIBILITY OF SUCH DAMAGE.\n\
\n\
The Alembic Software is distributed along with certain third party components licensed under various open source software licenses (\"Open Source Components\"). In addition to the warranty disclaimers contained in the open source licenses found below, Industrial Light & Magic, a division of Lucasfilm Entertainment Company Ltd. (\"ILM\") makes the following disclaimers regarding the Open Source Components on behalf of itself, the copyright holders, contributors, and licensors of such Open \n\
Source Components: TO THE FULLEST EXTENT PERMITTED UNDER APPLICABLE LAW, THE OPEN SOURCE COMPONENTS ARE PROVIDED BY THE COPYRIGHT HOLDERS, CONTRIBUTORS, LICENSORS, AND ILM \"AS IS\" AND ANY REPRESENTATIONS OR WARRANTIES OF ANY KIND, WHETHER ORAL OR WRITTEN, WHETHER EXPRESS, IMPLIED, OR ARISING BY STATUTE, CUSTOM, COURSE OF DEALING, OR TRADE USAGE, INCLUDING WITHOUT LIMITATION THE IMPLIED WARRANTIES OF TITLE, MERCHANTABILITY, FITNESS FOR A PARTICULAR PURPOSE, AND NON-INFRINGEMENT, ARE DISCLAIMED. IN NO EVENT WILL THE COPYRIGHT OWNER, CONTRIBUTORS, LICENSORS, OR ILM AND/OR ITS \n\
AFFILIATES BE LIABLE FOR ANY DIRECT, INDIRECT, INCIDENTAL, SPECIAL, \n\
EXEMPLARY, OR CONSEQUENTIAL DAMAGES (INCLUDING, BUT NOT LIMITED TO,  PROCUREMENT OF SUBSTITUTE GOODS OR SERVICES; LOSS OF USE, DATA, OR \n\
PROFITS; OR BUSINESS INTERRUPTION), HOWEVER CAUSED AND ON ANY THEORY OF LIABILITY, WHETHER IN CONTRACT, STRICT LIABILITY, OR TORT (INCLUDING \n\
NEGLIGENCE OR OTHERWISE) ARISING IN ANY WAY OUT OF THE USE OF THE OPEN SOURCE COMPONENTS, EVEN IF ADVISED OF THE POSSIBILITY OF SUCH DAMAGE. \n\
\n\
\n\
Copyright © 2014 Inigo Quilez, Voronoise Version 1/15/2014.  Permission is hereby granted, free of charge, to any person obtaining a copy of this software and associated documentation files (the \"Software\"), to deal in the Software without restriction, including without limitation the rights to use, copy, modify, merge, publish, distribute, sublicense, and/or sell copies of the Software, and to permit persons to whom the Software is furnished to do so, subject to the following conditions: The above copyright notice and this permission notice shall be included in all copies or substantial portions of the Software. THE SOFTWARE IS PROVIDED \"AS IS\", WITHOUT WARRANTY OF ANY KIND, EXPRESS OR IMPLIED, INCLUDING BUT NOT LIMITED TO THE WARRANTIES OF MERCHANTABILITY, FITNESS FOR A PARTICULAR PURPOSE AND NONINFRINGEMENT. IN NO EVENT SHALL THE AUTHORS OR COPYRIGHT HOLDERS BE LIABLE FOR ANY CLAIM, DAMAGES OR OTHER LIABILITY, WHETHER IN AN ACTION OF CONTRACT, TORT OR OTHERWISE, ARISING FROM, OUT OF OR IN CONNECTION WITH THE SOFTWARE OR THE USE OR OTHER DEALINGS IN THE SOFTWARE.\n\
\n\
Copyright (c) 2014, 2015, 2016 Cryptomatte Version 1.0.0 Psyop Media Company, LLC All rights reserved.  Redistribution and use in source and binary forms, with or without modification, are permitted provided that the following conditions are met: Redistributions of source code must retain the above copyright notice, this list of conditions and the following disclaimer. Redistributions in binary form must reproduce the above copyright notice, this list of conditions and the following disclaimer in the documentation and/or other materials provided with the distribution.  Neither the name of the <organization> nor the names of its contributors may be used to endorse or promote products derived from this software without specific prior written permission.  THIS SOFTWARE IS PROVIDED BY THE COPYRIGHT HOLDERS AND CONTRIBUTORS \"AS IS\" AND ANY EXPRESS OR IMPLIED WARRANTIES, INCLUDING, BUT NOT LIMITED TO, THE IMPLIED WARRANTIES OF MERCHANTABILITY AND FITNESS FOR A PARTICULAR PURPOSE ARE DISCLAIMED. IN NO EVENT SHALL <COPYRIGHT HOLDER> BE LIABLE FOR ANY DIRECT, INDIRECT, INCIDENTAL, SPECIAL, EXEMPLARY, OR CONSEQUENTIAL DAMAGES (INCLUDING, BUT NOT LIMITED TO, PROCUREMENT OF SUBSTITUTE GOODS OR SERVICES; LOSS OF USE, DATA, OR PROFITS; OR BUSINESS INTERRUPTION) HOWEVER CAUSED AND ON ANY THEORY OF LIABILITY, WHETHER IN CONTRACT, STRICT LIABILITY, OR TORT (INCLUDING NEGLIGENCE OR OTHERWISE) ARISING IN ANY WAY OUT OF THE USE OF THIS SOFTWARE, EVEN IF ADVISED OF THE POSSIBILITY OF SUCH DAMAGE.\n\
\n\
Alligator Noise is provided by Side Effects Software Inc. and is licensed under a Creative Commons Attribution-ShareAlike 4.0 International License.\n\
\n\
Boost C++ Libraries \n\
------------------------------------------------------------------------ \n\
Boost Software License – Version 1.0 August 17th, 2003 Permission is hereby granted, free of charge, to any person or organization obtaining a copy of the software and accompanying documentation \n\
covered by this license (the \"Software\") to use, reproduce, display, distribute, execute, and transmit the Software, and to prepare derivative works of the Software, and to permit third-parties to whom the Software is furnished to do so, all subject to the following: \n\
\n\
The copyright notices in the Software and this entire statement, including the above license grant, this restriction and the following disclaimer, must be included in all copies of the Software, in whole or in part, and all derivative works of the Software, unless such copies or derivative works are solely in the form of machine-executable object code generated by a source language processor. \n\
\n\
THE SOFTWARE IS PROVIDED \"AS IS\", WITHOUT WARRANTY OF ANY KIND EXPRESS OR IMPLIED, INCLUDING BUT NOT LIMITED TO THE WARRANTIES OF \n\
MERCHANTABILITY, FITNESS FOR A PARTICULAR PURPOSE, TITLE AND NON-INFRINGEMENT. IN NO EVENT SHALL THE COPYRIGHT HOLDERS OR ANYONE DISTRIBUTING THE SOFTWARE BE LIABLE FOR ANY DAMAGES OR OTHER LIABILITY, WHETHER IN CONTRACT, TORT OR OTHERWISE, ARISING FROM, OUT OF OR IN CONNECTION WITH THE SOFTWARE OR THE USE OR OTHER DEALINGS IN THE SOFTWARE. \n\
\n\
\n"
    
    arnoldAboutText =  u"Arnold for Maya\n\n"
    arnoldAboutText += "MtoA " + cmds.pluginInfo( 'mtoa', query=True, version=True)
    arnoldBuildID = cmds.arnoldPlugins(getBuildID=True)
    if not '(Master)' in arnoldBuildID:
        arnoldAboutText += " - " + arnoldBuildID
    arnoldAboutText += "\nArnold Core "+".".join(ai.AiGetVersion())
    arnoldAboutText += "\nCLM V"+ cmds.arnoldPlugins(getClmVersion=True) + "\n\n"
    arnoldAboutText += u"Copyright (c) 2001-2009 Marcos Fajardo and\nCopyright (c) 2009-2016 Solid Angle  S.L.\nAll rights reserved\n\n"
    arnoldAboutText += u"Developed by: Ángel Jimenez, Olivier Renouard, Yannick Puech,\nBorja Morales, Nicolas Dumay, Pedro Fernando Gomez,\nPál Mezei, Sebastien Blaineau-Ortega, Ashley Handscomb Retallack\n\n"
    arnoldAboutText += u"Acknowledgements: Javier González, Miguel González, Lee Griggs,\nChad Dombrova, Gaetan Guidet, Gaël Honorez, Diego Garcés,\nKevin Tureski, Frédéric Servant, Darin Grant"

    if (cmds.window("AboutArnold", ex=True)):
        cmds.deleteUI("AboutArnold")
    w = cmds.window("AboutArnold", title="About")
    cmds.window("AboutArnold", edit=True, width=520, height=550)
    cmds.rowColumnLayout( numberOfColumns=4, columnWidth=[(1,20), (2, 52), (3, 50), (4, 380)] )

    cmds.text(label="");cmds.text(label="");cmds.text(label="");cmds.text(label="")

    cmds.text(label="")
    cmds.image(image="MtoA_Logo.png")
    cmds.text(label="")
    cmds.text(align="left",label=arnoldAboutText)

    cmds.text(label="");cmds.text(label="");cmds.text(label="");cmds.text(label="")
    
    cmds.text(label="");cmds.text(label="");cmds.text(label="");
    
    cmds.scrollField(editable=False, wordWrap=True, font="plainLabelFont", height=200, text=legaltext)
    
    cmds.text(label="");cmds.text(label="");cmds.text(label="");cmds.text(label="")

    cmds.text(label="");cmds.text(label="\n");cmds.text(label="");cmds.text(label="")

    cmds.text(label="")
    cmds.text(label="")
    cmds.button( width=150,label='OK', command=('import maya.cmds as cmds;cmds.deleteUI(\"' + w + '\", window=True)') )
    cmds.setParent( '..' )
    
    cmds.showWindow(w)
   
def arnoldLicensingSignIn():
    ai.AiLicenseClmSignIn()

def arnoldLicensingSignOut():
    ai.AiLicenseClmSignOut()

def arnoldLicensingLicenseManager():
    ai.AiLicenseClmLicenseManager()  
        
def arnoldLicensingGetMacAddress():
    if (cmds.window("ArnoldLicenseGetMacAddress", ex=True)):
        cmds.deleteUI("ArnoldLicenseGetMacAddress")
    w = cmds.window("ArnoldLicenseGetMacAddress", sizeable=False, title="Get MAC Address")
    cmds.window("ArnoldLicenseGetMacAddress", edit=True, width=240, height=60)
    
    cmds.columnLayout()

    cmds.rowColumnLayout( numberOfColumns=3, columnWidth=[(1,10),(2,90),(3,140)] )
    cmds.text(align="left", label="")
    cmds.text(align="left", label="MAC Address")
    name = cmds.textField()
    mac = get_mac()
    mactext = ("%012X" % mac)
    cmds.textField(name,  edit=True, text=mactext, editable=False )
    cmds.setParent( '..' )
    cmds.rowColumnLayout( numberOfColumns=5, columnWidth=[(1,10),(2,70),(3,80), (4, 70), (5,10)])
    cmds.text(align="left",label="")
    commandStr = 'import maya.cmds as cmds;cmds.arnoldLicense(copyToClipboard=\"' + mactext+'\")'
    cmds.button( align="left", label='Copy', command=(commandStr))
    cmds.text(label="")
    cmds.button(align="right", label='Close', command=('import maya.cmds as cmds;cmds.deleteUI(\"' + w + '\", window=True)'))
    cmds.text(align="right", label="")
    cmds.setParent( '..' )
    cmds.showWindow(w)

def arnoldLicensingConnectLicenseServer():
    win = mtoa.licensing.ConnectToLicenseServer()
    win.create()
def arnoldLicensingGetDiagnostics():
    win = mtoa.licensing.GetDiagnostics()
    win.create()

def arnoldLicensingNodeLocked():
    win = mtoa.licensing.NodeLocked()
    win.create()
    
def arnoldTxManager():
    core.createOptions()
    win = mtoa.txManager.MtoATxManager()
    win.create()
    win.refreshList()

def arnoldUpdateTx():
    core.createOptions()
    cmds.arnoldUpdateTx()
    
def arnoldConvertDeprecated():
    mtoa.convertShaders.convertArnoldShaders()

    
def arnoldLightManager():
    win = mtoa.lightManager.MtoALightManager()
    win.create()

def arnoldBakeGeo():
    objFilter = "Obj File (*.obj)"
    global defaultFolder
    if defaultFolder == "":
        defaultFolder = cmds.workspace(q=True,rd=True, fn=True)

    ret = cmds.fileDialog2(cap='Bake Selection as OBJ', fm=0, ff=objFilter, dir=defaultFolder)
    if ret is not None and len(ret):
        defaultFolder = ret[0]
        cmds.arnoldBakeGeo(f=defaultFolder)

def arnoldDenoise():
    win = mtoa.denoise.MtoANoice()
    win.create()    


def arnoldExportOperators():
    global defaultOperatorsFolder
    if defaultOperatorsFolder == "":
        defaultOperatorsFolder = cmds.workspace(q=True,rd=True, fn=True)
    
    objFilter = "ASS File (*.ass)"        
    ret = cmds.fileDialog2(cap='Export Operator Graph',okc='Select',ff=objFilter,fm=0,dir=defaultOperatorsFolder) or []
    if len(ret):
        defaultOperatorsFolder = ret[0]
        cmds.arnoldExportAss('defaultArnoldRenderOptions', f=ret[0],s=True,mask=4097)

def arnoldImportOperators():
    global defaultOperatorsFolder
    if defaultOperatorsFolder == "":
        defaultOperatorsFolder = cmds.workspace(q=True,rd=True, fn=True)

    objFilter = "ASS File (*.ass)"
    ret = cmds.fileDialog2(cap='Import Operator Graph',okc='Select',fm=1,ff=objFilter,dir=defaultOperatorsFolder) or []
    if len(ret):
        defaultOperatorsFolder = ret[0]
        cmds.arnoldImportAss(f=ret[0])



def arnoldRenderToTexture():
    selList = cmds.ls(sl=1)
    if (len(selList) == 0):
        cmds.confirmDialog( title='Render To Texture', message='No Geometry Selected', button=['Ok'], defaultButton='Ok', cancelButton='Ok', dismissString='Ok' )
        return False

    win = mtoa.renderToTexture.MtoARenderToTexture()
    win.create()

def arnoldOpenMtoARenderView():
    cmds.arnoldRenderView(mode ="open")

def arnoldMtoARenderView():
    # core.ACTIVE_CAMERA is not set, anything we could do here ?
    #if core.ACTIVE_CAMERA != None:
    #    cmds.arnoldRenderView(cam=core.ACTIVE_CAMERA)
    # so instead we're calling it without any argument
    cmds.arnoldRenderView()

def addRuntimeMenuItem(name, parent, command, label = '', rtcLabel = '', tearOff=False, optionBox=False, image='', annotation='', category='', keywords ='', tags=''):
    
    tags = 'Render;Arnold' if (tags == '') else 'Render;Arnold;{}'.format(tags)
    category = 'Menu items.Mainframe.Arnold' if (category == '') else 'Menu items.Mainframe.Arnold.{}'.format(category)
    rtcLabel = 'Arnold: {}'.format(label) if (rtcLabel == '') else rtcLabel

    if _maya_version < 2019:
        cmds.menuItem(name, label=label, parent=parent, c=command, tearOff=tearOff, optionBox=optionBox, image=image)
    else:
        cmds.runTimeCommand('cmd{}'.format(name), d=True, label=rtcLabel, annotation=annotation, category=category, keywords=keywords, tags=tags, command=command, image=image )
        cmds.menuItem(name, parent=parent, rtc='cmd{}'.format(name), label = label, sourceType= 'mel', optionBox=optionBox, tearOff=tearOff)


def createArnoldMenu():
    # Add an Arnold menu in Maya main window
    if not cmds.about(b=1):
        
        if _maya_version < 2017:
            cmds.menu('ArnoldMenu', label='Arnold', parent='MayaWindow', tearOff=True )
        else:
            cmds.menu('ArnoldMenu', label='Arnold', parent='MayaWindow', tearOff=True, version="2017" )

        addRuntimeMenuItem('ArnoldRender', label='Render', parent='ArnoldMenu', rtcLabel = 'Arnold: Render', image='RenderShelf.png', 
                    command='import mtoa.ui.arnoldmenu;mtoa.ui.arnoldmenu.arnoldMtoARenderView()', keywords='arv;ipr', annotation ='Render with the Arnold RenderView')
        
        addRuntimeMenuItem('ArnoldMtoARenderView', label='Open Arnold RenderView', rtcLabel = 'Arnold: Open RenderView', parent='ArnoldMenu',  image='RenderViewShelf.png',
                    command='import mtoa.ui.arnoldmenu;mtoa.ui.arnoldmenu.arnoldOpenMtoARenderView()', keywords='arv;ipr', annotation='Open the Arnold RenderView window')
        cmds.menuItem(parent='ArnoldMenu', divider=True)

        cmds.menuItem('ArnoldStandIn', label='StandIn', parent='ArnoldMenu', subMenu=True, tearOff=True)

        addRuntimeMenuItem('ArnoldCreateStandIn', parent='ArnoldStandIn', label="Create StandIn", image='StandinShelf.png',
                    command='import mtoa.ui.arnoldmenu;mtoa.ui.arnoldmenu.createStandIn()', category='StandIn', keywords='procedural;proxy;archive;import', annotation='Create a StandIn to load a .ass file')
        addRuntimeMenuItem('ArnoldCreateStandInFile', parent='ArnoldStandIn', optionBox=True,  
                    command='import mtoa.ui.arnoldmenu;mtoa.ui.arnoldmenu.doCreateStandInFile()', rtcLabel='Arnold: Create StandIn from File', category='StandIn', keywords='procedural;proxy;archive;import', annotation='Create a StandIn to load a .ass file')
        addRuntimeMenuItem('ArnoldExportStandIn', parent='ArnoldStandIn', label='Export StandIn', image='ExportStandinShelf.png',
                    command='import mtoa.ui.arnoldmenu;mtoa.ui.arnoldmenu.doExportStandIn()', category='StandIn', keywords='procedural;proxy;archive', annotation='Export the selection as a Standin .ass file')
        cmds.menuItem('ArnoldExportOptionsStandIn', parent='ArnoldStandIn', optionBox=True,
                    command='import mtoa.ui.arnoldmenu;mtoa.ui.arnoldmenu.doExportOptionsStandIn()')

        cmds.menuItem('ArnoldLights', label='Lights', parent='ArnoldMenu', subMenu=True, tearOff=True)
        
        addRuntimeMenuItem('ArnoldAreaLights', parent='ArnoldLights', label="Area Light",rtcLabel = 'Arnold: Create Area Light', image='AreaLightShelf.png',
                    command='import mtoa.utils;mtoa.utils.createLocator("aiAreaLight", asLight=True)', category='Lights', keywords='quad;softbox', annotation='Create an Arnold Area Light (Quad/Cylinder/Disk)')
        addRuntimeMenuItem('SkydomeLight', parent='ArnoldLights', label="Skydome Light", rtcLabel='Arnold: Create Skydome Light', image='SkydomeLightShelf.png',
                    command='import mtoa.utils;mtoa.utils.createLocator("aiSkyDomeLight", asLight=True)', category='Lights', keywords='ibl;hdr;dome;env', annotation='Create an Arnold Skydome Light for Environment/IBL lighting')
        addRuntimeMenuItem('ArnoldMeshLight', parent='ArnoldLights', label='Mesh Light', rtcLabel='Arnold: Create Mesh Light', image='MeshLightShelf.png',
                    command='import mtoa.utils;mtoa.utils.createMeshLight()', category='Lights', keywords='geo light;object light', annotation='Convert the selected Mesh to an Arnold Mesh Light')
        addRuntimeMenuItem('PhotometricLights', parent='ArnoldLights', label="Photometric Light", rtcLabel="Arnold: Create Photometric Light", image='PhotometricLightShelf.png', keywords='ies;physical light;light map;practical light',
                    command='import mtoa.utils;mtoa.utils.createLocator("aiPhotometricLight", asLight=True)', category='Lights', annotation='Create an Arnold Photometric Light to load IES files')
        addRuntimeMenuItem('LightPortal', parent='ArnoldLights', label="Light Portal", rtcLabel="Arnold: Create Light Portal", image='LightPortalShelf.png',
                    command='import mtoa.ui.arnoldmenu;mtoa.ui.arnoldmenu.doCreateLightPortal()', keywords='hdr;ibl;env;dome', annotation='Portals can optimize Skydomes interior lighting', category='Lights')
        addRuntimeMenuItem('PhysicalSky', parent='ArnoldLights', label="Physical Sky", rtcLabel="Arnold: Create Physical Sky", image='PhysicalSkyShelf.png', keywords='sun;sky;env;dome;ibl',
                    command='import mtoa.ui.arnoldmenu;mtoa.ui.arnoldmenu.doCreatePhysicalSky()', category='Lights', annotation='Create an Arnold Skydome w/ Physical Sky illumination')
        
        customShapes = cmds.arnoldPlugins(listCustomShapes=True)
        if customShapes and len(customShapes) > 0:
            cmds.menuItem('ArnoldCustomShapes', label='Custom Shapes', parent='ArnoldMenu', subMenu=True, tearOff=True)
            for customShape in customShapes:
                addRuntimeMenuItem(customShape, parent='ArnoldCustomShapes', label=customShape, rtcLabel='Arnold: Create {} Shape'.format(customShape),
                    command='import mtoa.ui.arnoldmenu;mtoa.ui.arnoldmenu.doCreateCustomShape("{}")'.format(customShape), category='Custom Shapes')

        operators = cmds.arnoldPlugins(listOperators=True)
        if operators and len(operators) > 0:
            cmds.menuItem('ArnoldOperators', label='Operators', parent='ArnoldMenu', subMenu=True, tearOff=True)
            for operator in operators:
                addRuntimeMenuItem(operator, parent='ArnoldOperators', label=operator, rtcLabel='Arnold: Create {} Operator'.format(operator),
                    command='import mtoa.ui.arnoldmenu;mtoa.ui.arnoldmenu.doCreateOperator("{}")'.format(operator), category = 'Operators')

        addRuntimeMenuItem('CurveCollector', label='Curve Collector', rtcLabel='Arnold: Create Curve Collector', parent='ArnoldMenu', image='CurveCollectorShelf.png',
                    command='import mtoa.ui.arnoldmenu;mtoa.ui.arnoldmenu.doCreateCurveCollector()', annotation="Group the selected curves in an Arnold Curve Collector")
        addRuntimeMenuItem('ArnoldVolume', label='Volume', rtcLabel='Arnold: Create Volume', parent='ArnoldMenu', image='VolumeShelf.png', 
                    command='import mtoa.ui.arnoldmenu;mtoa.ui.arnoldmenu.createVolume()', keywords='vdb;openvdb', annotation="Create an Arnold Volume node to load a VDB file")
                    
        cmds.menuItem('ArnoldFlush', label='Flush Caches', parent='ArnoldMenu', subMenu=True, tearOff=True)
        addRuntimeMenuItem('ArnoldFlushTexture', parent='ArnoldFlush', label="Textures", rtcLabel='Arnold: Flush Texture Cache', image='FlushTextureShelf.png', 
                    command='import maya.cmds;maya.cmds.arnoldFlushCache(textures=True)', category="Flush Caches")
        addRuntimeMenuItem('ArnoldFlushSelectedTextures', parent='ArnoldFlush', label="Selected Textures", rtcLabel = 'Arnold: Flush selected textures', image='FlushTextureShelf.png', 
                    command='import maya.cmds;maya.cmds.arnoldFlushCache(selected_textures=True)', category="Flush Caches")
        addRuntimeMenuItem('ArnoldFlushBackground', parent='ArnoldFlush', label="Skydome Lights", rtcLabel = 'Arnold: Flush Skydome Cache', image='FlushBackgroundShelf.png',
                    command='import maya.cmds;maya.cmds.arnoldFlushCache(skydome=True)', category="Flush Caches")
        addRuntimeMenuItem('ArnoldFlushQuads', parent='ArnoldFlush', label="Quad Lights", rtcLabel='Arnold: Flush Area Lights Cache', image='FlushQuadLightShelf.png',
                    command='import maya.cmds; maya.cmds.arnoldFlushCache(quads=True)', category="Flush Caches")
        addRuntimeMenuItem('ArnoldFlushAll', parent='ArnoldFlush', label="All", rtcLabel='Arnold: Flush All Caches', image='FlushAllCachesShelf.png',
                    command='import maya.cmds; maya.cmds.arnoldFlushCache(flushall=True)', category="Flush Caches")
                    
        cmds.menuItem('ArnoldUtilities', label='Utilities', parent='ArnoldMenu', subMenu=True, tearOff=True)
        addRuntimeMenuItem('ArnoldDenoise', label='Arnold Denoiser (noice)', parent='ArnoldUtilities', 
                    command='import mtoa.ui.arnoldmenu;mtoa.ui.arnoldmenu.arnoldDenoise()', category="Utilities", keywords='noice;denoising', annotation="Denoise an image or a sequence using the Arnold denoise \"noice\"")
        addRuntimeMenuItem('ArnoldBakeGeo', label='Bake Selected Geometry', parent='ArnoldUtilities', image='BakeGeometryShelf.png', 
                    command='import mtoa.ui.arnoldmenu;mtoa.ui.arnoldmenu.arnoldBakeGeo()', category="Utilities", keywords='baking', annotation='Bake the selected geometry to OBJ (w/ subdivision and displacement)')
        addRuntimeMenuItem('ArnoldRenderToTexture', label='Render Selection To Texture', parent='ArnoldUtilities', image='RenderToTextureShelf.png', 
                    command='import mtoa.ui.arnoldmenu;mtoa.ui.arnoldmenu.arnoldRenderToTexture()', category="Utilities", keywords='bake;baking', annotation="Shade the selected shape and bake it on a UV texture")
        addRuntimeMenuItem('ArnoldExportOperators', label='Export Operator Graph', parent='ArnoldUtilities', keywords='operator',
                    command='import mtoa.ui.arnoldmenu;mtoa.ui.arnoldmenu.arnoldExportOperators()', category="Utilities", annotation='Export the current operator graph to .ass')
        addRuntimeMenuItem('ArnoldImportOperators', label='Import Operator Graph', parent='ArnoldUtilities', keywords='operator',
                    command='import mtoa.ui.arnoldmenu;mtoa.ui.arnoldmenu.arnoldImportOperators()', category="Utilities", annotation='Import an operator graph from a .ass file')
        addRuntimeMenuItem('ArnoldTxManager', label='TX Manager', parent='ArnoldUtilities', image='TXManagerShelf.png', keywords='textures;convert;optimize',
                    command='import mtoa.ui.arnoldmenu;mtoa.ui.arnoldmenu.arnoldTxManager()', category="Utilities", annotation='Open the Arnold TX Manager')
        addRuntimeMenuItem('ArnoldUpdateTx', label='Update TX Files', parent='ArnoldUtilities',  image='UpdateTxShelf.png', keywords='textures',
                    command='import mtoa.ui.arnoldmenu;mtoa.ui.arnoldmenu.arnoldUpdateTx()', category="Utilities", annotation="Convert / Updates all textures to TX for Arnold rendering")
        cmds.menuItem('ArnoldLightManager', label='Light Manager', parent='ArnoldUtilities', image='LightManagerShelf.png', 
                    command='import mtoa.ui.arnoldmenu;mtoa.ui.arnoldmenu.arnoldLightManager()')
        cmds.menuItem('ArnoldConvertShaders', label='Convert Shaders to Arnold', parent='ArnoldUtilities',
                    command='import mtoa.ui.arnoldmenu;mtoa.ui.arnoldmenu.arnoldConvertDeprecated()')

        cmds.menuItem('ArnoldLicensingMenu', label='RLM Licensing', parent='ArnoldMenu',
                    subMenu=True, tearOff=True)
        cmds.menuItem('ArnoldConnectLicenseServer', label='Connect to License Server', parent='ArnoldLicensingMenu',
                    c=lambda *args: arnoldLicensingConnectLicenseServer())
        cmds.menuItem('ArnoldGetDiagnostics', label='Diagnostics', parent='ArnoldLicensingMenu',
                    c=lambda *args: arnoldLicensingGetDiagnostics())
        cmds.menuItem('ArnoldTroubleshootWatermarks', label='Troubleshoot Watermarks', parent='ArnoldLicensingMenu', 
                    c=lambda *args: cmds.launch(webPage='https://support.solidangle.com/x/LAAzAg'))
        
        cmds.menuItem('ArnoldSuscribe',  label='Buy Arnold', parent='ArnoldLicensingMenu', 
                    c=lambda *args: cmds.launch(webPage='https://www.solidangle.com/arnold/buy/'))
        cmds.menuItem('ArnoldGetMacAddress', label='Get MAC Address', parent='ArnoldLicensingMenu',
                    c=lambda *args: arnoldLicensingGetMacAddress())

        setupServerLink = 'https://support.solidangle.com/display/A5AILIC/Setting+up+a+License+Server'
        platformName = sys.platform

        if platformName.startswith('win'):
            setupServerLink = 'https://support.solidangle.com/x/IQAzAg'
        elif platformName.startswith('linux'):
            setupServerLink = 'https://support.solidangle.com/x/JwAzAg'
        elif platformName.startswith('darwin'):
            setupServerLink = 'https://support.solidangle.com/x/EQAzAg'

        cmds.menuItem('ArnoldSetupLicenseServer', label='Setup License Server', parent='ArnoldLicensingMenu', 
                    c=lambda *args: cmds.launch(webPage=setupServerLink))

        cmds.menuItem('ArnoldInstallTrialLicense', label='Install Trial License', parent='ArnoldLicensingMenu',
                    c=lambda *args: arnoldLicensingNodeLocked())

        cmds.menuItem('ArnoldHelpMenu', label='Help', parent='ArnoldMenu', 
                    subMenu=True, tearOff=True)

        cmds.menuItem('ArnoldUserGuide', label='User Guide', parent='ArnoldHelpMenu',
                    c=lambda *args: cmds.launch(webPage='https://support.solidangle.com/display/a5AFMUG/Arnold+for+Maya+User+Guide'))

        cmds.menuItem('ArnoldTutorials', label='Tutorials', parent='ArnoldHelpMenu',
                    c=lambda *args: cmds.launch(webPage='https://support.solidangle.com/display/A5AFMUG/Tutorials'))

#        cmds.menuItem('ArnoldVideos', label='Videos', parent='ArnoldHelpMenu',
#                    c=lambda *args: cmds.launch(webPage='https://support.solidangle.com/display/AFMUG/Video+Tutorials'))

        cmds.menuItem('ArnoldAnswers', label='Arnold Answers', parent='ArnoldHelpMenu',
                    c=lambda *args: cmds.launch(webPage='https://answers.arnoldrenderer.com/index.html'))

        cmds.menuItem('ArnoldLearningScenes', label='Learning Scenes', parent='ArnoldHelpMenu',
                    c=lambda *args: cmds.launch(webPage='https://support.solidangle.com/display/A5AFMUG/Learning+Scenes'))

        cmds.menuItem(divider=1, parent='ArnoldHelpMenu')

        cmds.menuItem('ArnoldSolidAngle', label='Solid Angle', parent='ArnoldHelpMenu',
                    c=lambda *args: cmds.launch(webPage='https://www.solidangle.com'))

        cmds.menuItem('ArnoldMailingLists', label='Mailing Lists', parent='ArnoldHelpMenu',
                    c=lambda *args: cmds.launch(webPage='https://subscribe.solidangle.com'))
        
#        cmds.menuItem('ArnoldAsk', label='Knowledge Base', parent='ArnoldHelpMenu',
#                    c=lambda *args: cmds.launch(webPage='https://ask.solidangle.com'))

        cmds.menuItem('ArnoldSupportBlog', label='Support Blog', parent='ArnoldHelpMenu',
                    c=lambda *args: cmds.launch(webPage='https://support.solidangle.com/blog/arnsupp'))

#        cmds.menuItem('ArnoldLicensing', label='Licensing', parent='ArnoldHelpMenu',
#                    c=lambda *args: arnoldLicenseDialog())

        cmds.menuItem(divider=1, parent='ArnoldHelpMenu')

        cmds.menuItem('ArnoldDeveloperGuide', label='Developer Guide', parent='ArnoldHelpMenu',
                    c=lambda *args: cmds.launch(webPage='https://support.solidangle.com/display/A5ARP/Arnoldpedia'))
                    
        cmds.menuItem('ArnoldAbout', label='About', parent='ArnoldMenu', image ='menuIconHelp.png',
                    c=lambda *args: arnoldAboutDialog())
        cmds.menuItem(divider=1, parent='ArnoldMenu')

        clmVersion = cmds.arnoldPlugins(getClmVersion=True)
        if int(clmVersion) > 1:
            cmds.menuItem('ArnoldLicensingLicenseManager', label='Manage License...', parent='ArnoldMenu',
                        c=lambda *args: arnoldLicensingLicenseManager())
            cmds.menuItem('ArnoldLicensingSignIn', label='Sign-In', parent='ArnoldMenu',
                        c=lambda *args: arnoldLicensingSignIn())
            cmds.menuItem('ArnoldLicensingSignOut', label='Sign-Out', parent='ArnoldMenu',
                        c=lambda *args: arnoldLicensingSignOut())
            