
#pragma once

#if defined(_DARWIN)
#include <OpenGL/gl.h>
#elif defined(_WIN32)
#define NOMINMAX
#define WIN32_LEAN_AND_MEAN
#include <windows.h>
#include <GL/gl.h>
// #include <GL/glew.h>
#else
#include <GL/gl.h>
#include <GL/glx.h>
#endif


/* Print OpenGL error messages */
void GL_print_error(const char *operation);

/* Dynamic loading of OpenGL symbols */
int GL_dynamic_load_init();

void printGPUState();

/* No dynamic loading on Mac / Windows */
#if defined(_DARWIN) || defined(_WIN32)

#define kglAccum glAccum
#define kglAlphaFunc glAlphaFunc
#define kglAreTexturesResident glAreTexturesResident
#define kglArrayElement glArrayElement
#define kglBegin glBegin
#define kglBindTexture glBindTexture
#define kglBitmap glBitmap
#define kglBlendFunc glBlendFunc
#define kglCallList glCallList
#define kglCallLists glCallLists
#define kglClear glClear
#define kglClearAccum glClearAccum
#define kglClearColor glClearColor
#define kglClearDepth glClearDepth
#define kglClearIndex glClearIndex
#define kglClearStencil glClearStencil
#define kglClipPlane glClipPlane
#define kglColor3b glColor3b
#define kglColor3bv glColor3bv
#define kglColor3d glColor3d
#define kglColor3dv glColor3dv
#define kglColor3f glColor3f
#define kglColor3fv glColor3fv
#define kglColor3i glColor3i
#define kglColor3iv glColor3iv
#define kglColor3s glColor3s
#define kglColor3sv glColor3sv
#define kglColor3ub glColor3ub
#define kglColor3ubv glColor3ubv
#define kglColor3ui glColor3ui
#define kglColor3uiv glColor3uiv
#define kglColor3us glColor3us
#define kglColor3usv glColor3usv
#define kglColor4b glColor4b
#define kglColor4bv glColor4bv
#define kglColor4d glColor4d
#define kglColor4dv glColor4dv
#define kglColor4f glColor4f
#define kglColor4fv glColor4fv
#define kglColor4i glColor4i
#define kglColor4iv glColor4iv
#define kglColor4s glColor4s
#define kglColor4sv glColor4sv
#define kglColor4ub glColor4ub
#define kglColor4ubv glColor4ubv
#define kglColor4ui glColor4ui
#define kglColor4uiv glColor4uiv
#define kglColor4us glColor4us
#define kglColor4usv glColor4usv
#define kglColorMask glColorMask
#define kglColorMaterial glColorMaterial
#define kglColorPointer glColorPointer
#define kglCopyPixels glCopyPixels
#define kglCopyTexImage1D glCopyTexImage1D
#define kglCopyTexImage2D glCopyTexImage2D
#define kglCopyTexSubImage1D glCopyTexSubImage1D
#define kglCopyTexSubImage2D glCopyTexSubImage2D
#define kglCullFace glCullFace
#define kglDeleteLists glDeleteLists
#define kglDeleteTextures glDeleteTextures
#define kglDepthFunc glDepthFunc
#define kglDepthMask glDepthMask
#define kglDepthRange glDepthRange
#define kglDisable glDisable
#define kglDisableClientState glDisableClientState
#define kglDrawArrays glDrawArrays
#define kglDrawBuffer glDrawBuffer
#define kglDrawElements glDrawElements
#define kglDrawPixels glDrawPixels
#define kglEdgeFlag glEdgeFlag
#define kglEdgeFlagPointer glEdgeFlagPointer
#define kglEdgeFlagv glEdgeFlagv
#define kglEnable glEnable
#define kglEnableClientState glEnableClientState
#define kglEnd glEnd
#define kglEndList glEndList
#define kglEvalCoord1d glEvalCoord1d
#define kglEvalCoord1dv glEvalCoord1dv
#define kglEvalCoord1f glEvalCoord1f
#define kglEvalCoord1fv glEvalCoord1fv
#define kglEvalCoord2d glEvalCoord2d
#define kglEvalCoord2dv glEvalCoord2dv
#define kglEvalCoord2f glEvalCoord2f
#define kglEvalCoord2fv glEvalCoord2fv
#define kglEvalMesh1 glEvalMesh1
#define kglEvalMesh2 glEvalMesh2
#define kglEvalPoint1 glEvalPoint1
#define kglEvalPoint2 glEvalPoint2
#define kglFeedbackBuffer glFeedbackBuffer
#define kglFinish glFinish
#define kglFlush glFlush
#define kglFogf glFogf
#define kglFogfv glFogfv
#define kglFogi glFogi
#define kglFogiv glFogiv
#define kglFrontFace glFrontFace
#define kglFrustum glFrustum
#define kglGenLists glGenLists
#define kglGenTextures glGenTextures
#define kglGetBooleanv glGetBooleanv
#define kglGetClipPlane glGetClipPlane
#define kglGetDoublev glGetDoublev
#define kglGetError glGetError
#define kglGetFloatv glGetFloatv
#define kglGetIntegerv glGetIntegerv
#define kglGetLightfv glGetLightfv
#define kglGetLightiv glGetLightiv
#define kglGetMapdv glGetMapdv
#define kglGetMapfv glGetMapfv
#define kglGetMapiv glGetMapiv
#define kglGetMaterialfv glGetMaterialfv
#define kglGetMaterialiv glGetMaterialiv
#define kglGetPixelMapfv glGetPixelMapfv
#define kglGetPixelMapuiv glGetPixelMapuiv
#define kglGetPixelMapusv glGetPixelMapusv
#define kglGetPointerv glGetPointerv
#define kglGetPolygonStipple glGetPolygonStipple
#define kglGetString glGetString
#define kglGetTexEnvfv glGetTexEnvfv
#define kglGetTexEnviv glGetTexEnviv
#define kglGetTexGendv glGetTexGendv
#define kglGetTexGenfv glGetTexGenfv
#define kglGetTexGeniv glGetTexGeniv
#define kglGetTexImage glGetTexImage
#define kglGetTexLevelParameterfv glGetTexLevelParameterfv
#define kglGetTexLevelParameteriv glGetTexLevelParameteriv
#define kglGetTexParameterfv glGetTexParameterfv
#define kglGetTexParameteriv glGetTexParameteriv
#define kglHint glHint
#define kglIndexMask glIndexMask
#define kglIndexPointer glIndexPointer
#define kglIndexd glIndexd
#define kglIndexdv glIndexdv
#define kglIndexf glIndexf
#define kglIndexfv glIndexfv
#define kglIndexi glIndexi
#define kglIndexiv glIndexiv
#define kglIndexs glIndexs
#define kglIndexsv glIndexsv
#define kglIndexub glIndexub
#define kglIndexubv glIndexubv
#define kglInitNames glInitNames
#define kglInterleavedArrays glInterleavedArrays
#define kglIsEnabled glIsEnabled
#define kglIsList glIsList
#define kglIsTexture glIsTexture
#define kglLightModelf glLightModelf
#define kglLightModelfv glLightModelfv
#define kglLightModeli glLightModeli
#define kglLightModeliv glLightModeliv
#define kglLightf glLightf
#define kglLightfv glLightfv
#define kglLighti glLighti
#define kglLightiv glLightiv
#define kglLineStipple glLineStipple
#define kglLineWidth glLineWidth
#define kglListBase glListBase
#define kglLoadIdentity glLoadIdentity
#define kglLoadMatrixd glLoadMatrixd
#define kglLoadMatrixf glLoadMatrixf
#define kglLoadName glLoadName
#define kglLogicOp glLogicOp
#define kglMap1d glMap1d
#define kglMap1f glMap1f
#define kglMap2d glMap2d
#define kglMap2f glMap2f
#define kglMapGrid1d glMapGrid1d
#define kglMapGrid1f glMapGrid1f
#define kglMapGrid2d glMapGrid2d
#define kglMapGrid2f glMapGrid2f
#define kglMaterialf glMaterialf
#define kglMaterialfv glMaterialfv
#define kglMateriali glMateriali
#define kglMaterialiv glMaterialiv
#define kglMatrixMode glMatrixMode
#define kglMultMatrixd glMultMatrixd
#define kglMultMatrixf glMultMatrixf
#define kglNewList glNewList
#define kglNormal3b glNormal3b
#define kglNormal3bv glNormal3bv
#define kglNormal3d glNormal3d
#define kglNormal3dv glNormal3dv
#define kglNormal3f glNormal3f
#define kglNormal3fv glNormal3fv
#define kglNormal3i glNormal3i
#define kglNormal3iv glNormal3iv
#define kglNormal3s glNormal3s
#define kglNormal3sv glNormal3sv
#define kglNormalPointer glNormalPointer
#define kglOrtho glOrtho
#define kglPassThrough glPassThrough
#define kglPixelMapfv glPixelMapfv
#define kglPixelMapuiv glPixelMapuiv
#define kglPixelMapusv glPixelMapusv
#define kglPixelStoref glPixelStoref
#define kglPixelStorei glPixelStorei
#define kglPixelTransferf glPixelTransferf
#define kglPixelTransferi glPixelTransferi
#define kglPixelZoom glPixelZoom
#define kglPointSize glPointSize
#define kglPolygonMode glPolygonMode
#define kglPolygonOffset glPolygonOffset
#define kglPolygonStipple glPolygonStipple
#define kglPopAttrib glPopAttrib
#define kglPopClientAttrib glPopClientAttrib
#define kglPopMatrix glPopMatrix
#define kglPopName glPopName
#define kglPrioritizeTextures glPrioritizeTextures
#define kglPushAttrib glPushAttrib
#define kglPushClientAttrib glPushClientAttrib
#define kglPushMatrix glPushMatrix
#define kglPushName glPushName
#define kglRasterPos2d glRasterPos2d
#define kglRasterPos2dv glRasterPos2dv
#define kglRasterPos2f glRasterPos2f
#define kglRasterPos2fv glRasterPos2fv
#define kglRasterPos2i glRasterPos2i
#define kglRasterPos2iv glRasterPos2iv
#define kglRasterPos2s glRasterPos2s
#define kglRasterPos2sv glRasterPos2sv
#define kglRasterPos3d glRasterPos3d
#define kglRasterPos3dv glRasterPos3dv
#define kglRasterPos3f glRasterPos3f
#define kglRasterPos3fv glRasterPos3fv
#define kglRasterPos3i glRasterPos3i
#define kglRasterPos3iv glRasterPos3iv
#define kglRasterPos3s glRasterPos3s
#define kglRasterPos3sv glRasterPos3sv
#define kglRasterPos4d glRasterPos4d
#define kglRasterPos4dv glRasterPos4dv
#define kglRasterPos4f glRasterPos4f
#define kglRasterPos4fv glRasterPos4fv
#define kglRasterPos4i glRasterPos4i
#define kglRasterPos4iv glRasterPos4iv
#define kglRasterPos4s glRasterPos4s
#define kglRasterPos4sv glRasterPos4sv
#define kglReadBuffer glReadBuffer
#define kglReadPixels glReadPixels
#define kglRectd glRectd
#define kglRectdv glRectdv
#define kglRectf glRectf
#define kglRectfv glRectfv
#define kglRecti glRecti
#define kglRectiv glRectiv
#define kglRects glRects
#define kglRectsv glRectsv
#define kglRenderMode glRenderMode
#define kglRotated glRotated
#define kglRotatef glRotatef
#define kglScaled glScaled
#define kglScalef glScalef
#define kglScissor glScissor
#define kglSelectBuffer glSelectBuffer
#define kglShadeModel glShadeModel
#define kglStencilFunc glStencilFunc
#define kglStencilMask glStencilMask
#define kglStencilOp glStencilOp
#define kglTexCoord1d glTexCoord1d
#define kglTexCoord1dv glTexCoord1dv
#define kglTexCoord1f glTexCoord1f
#define kglTexCoord1fv glTexCoord1fv
#define kglTexCoord1i glTexCoord1i
#define kglTexCoord1iv glTexCoord1iv
#define kglTexCoord1s glTexCoord1s
#define kglTexCoord1sv glTexCoord1sv
#define kglTexCoord2d glTexCoord2d
#define kglTexCoord2dv glTexCoord2dv
#define kglTexCoord2f glTexCoord2f
#define kglTexCoord2fv glTexCoord2fv
#define kglTexCoord2i glTexCoord2i
#define kglTexCoord2iv glTexCoord2iv
#define kglTexCoord2s glTexCoord2s
#define kglTexCoord2sv glTexCoord2sv
#define kglTexCoord3d glTexCoord3d
#define kglTexCoord3dv glTexCoord3dv
#define kglTexCoord3f glTexCoord3f
#define kglTexCoord3fv glTexCoord3fv
#define kglTexCoord3i glTexCoord3i
#define kglTexCoord3iv glTexCoord3iv
#define kglTexCoord3s glTexCoord3s
#define kglTexCoord3sv glTexCoord3sv
#define kglTexCoord4d glTexCoord4d
#define kglTexCoord4dv glTexCoord4dv
#define kglTexCoord4f glTexCoord4f
#define kglTexCoord4fv glTexCoord4fv
#define kglTexCoord4i glTexCoord4i
#define kglTexCoord4iv glTexCoord4iv
#define kglTexCoord4s glTexCoord4s
#define kglTexCoord4sv glTexCoord4sv
#define kglTexCoordPointer glTexCoordPointer
#define kglTexEnvf glTexEnvf
#define kglTexEnvfv glTexEnvfv
#define kglTexEnvi glTexEnvi
#define kglTexEnviv glTexEnviv
#define kglTexGend glTexGend
#define kglTexGendv glTexGendv
#define kglTexGenf glTexGenf
#define kglTexGenfv glTexGenfv
#define kglTexGeni glTexGeni
#define kglTexGeniv glTexGeniv
#define kglTexImage1D glTexImage1D
#define kglTexImage2D glTexImage2D
#define kglTexParameterf glTexParameterf
#define kglTexParameterfv glTexParameterfv
#define kglTexParameteri glTexParameteri
#define kglTexParameteriv glTexParameteriv
#define kglTexSubImage1D glTexSubImage1D
#define kglTexSubImage2D glTexSubImage2D
#define kglTranslated glTranslated
#define kglTranslatef glTranslatef
#define kglVertex2d glVertex2d
#define kglVertex2dv glVertex2dv
#define kglVertex2f glVertex2f
#define kglVertex2fv glVertex2fv
#define kglVertex2i glVertex2i
#define kglVertex2iv glVertex2iv
#define kglVertex2s glVertex2s
#define kglVertex2sv glVertex2sv
#define kglVertex3d glVertex3d
#define kglVertex3dv glVertex3dv
#define kglVertex3f glVertex3f
#define kglVertex3fv glVertex3fv
#define kglVertex3i glVertex3i
#define kglVertex3iv glVertex3iv
#define kglVertex3s glVertex3s
#define kglVertex3sv glVertex3sv
#define kglVertex4d glVertex4d
#define kglVertex4dv glVertex4dv
#define kglVertex4f glVertex4f
#define kglVertex4fv glVertex4fv
#define kglVertex4i glVertex4i
#define kglVertex4iv glVertex4iv
#define kglVertex4s glVertex4s
#define kglVertex4sv glVertex4sv
#define kglVertexPointer glVertexPointer
#define kglViewport glViewport

#define kglXQueryExtension glXQueryExtension
#define kglXQueryVersion glXQueryVersion
#define kglXGetConfig glXGetConfig
#define kglXChooseVisual glXChooseVisual
#define kglXCreateContext glXCreateContext
#define kglXDestroyContext glXDestroyContext
#define kglXMakeCurrent glXMakeCurrent
#define kglXSwapBuffers glXSwapBuffers
#define kglXGetProcAddressARB glXGetProcAddressARB















#else

typedef void (*KGLACCUMPROC)(GLenum op, GLfloat value);
typedef void (*KGLALPHAFUNCPROC)(GLenum func, GLclampf ref);
typedef GLboolean (*KGLARETEXTURESRESIDENTPROC)(GLsizei n, const GLuint *textures, GLboolean *residences);
typedef void (*KGLARRAYELEMENTPROC)(GLint i);
typedef void (*KGLBEGINPROC)(GLenum mode);
typedef void (*KGLBINDTEXTUREPROC)(GLenum target, GLuint texture);
typedef void (*KGLBITMAPPROC)(GLsizei width, GLsizei height, GLfloat xorig, GLfloat yorig, GLfloat xmove, GLfloat ymove, const GLubyte *bitmap);
typedef void (*KGLBLENDFUNCPROC)(GLenum sfactor, GLenum dfactor);
typedef void (*KGLCALLLISTPROC)(GLuint list);
typedef void (*KGLCALLLISTSPROC)(GLsizei n, GLenum type, const void *lists);
typedef void (*KGLCLEARPROC)(GLbitfield mask);
typedef void (*KGLCLEARACCUMPROC)(GLfloat red, GLfloat green, GLfloat blue, GLfloat alpha);
typedef void (*KGLCLEARCOLORPROC)(GLclampf red, GLclampf green, GLclampf blue, GLclampf alpha);
typedef void (*KGLCLEARDEPTHPROC)(GLclampd depth);
typedef void (*KGLCLEARINDEXPROC)(GLfloat c);
typedef void (*KGLCLEARSTENCILPROC)(GLint s);
typedef void (*KGLCLIPPLANEPROC)(GLenum plane, const GLdouble *equation);
typedef void (*KGLCOLOR3BPROC)(GLbyte red, GLbyte green, GLbyte blue);
typedef void (*KGLCOLOR3BVPROC)(const GLbyte *v);
typedef void (*KGLCOLOR3DPROC)(GLdouble red, GLdouble green, GLdouble blue);
typedef void (*KGLCOLOR3DVPROC)(const GLdouble *v);
typedef void (*KGLCOLOR3FPROC)(GLfloat red, GLfloat green, GLfloat blue);
typedef void (*KGLCOLOR3FVPROC)(const GLfloat *v);
typedef void (*KGLCOLOR3IPROC)(GLint red, GLint green, GLint blue);
typedef void (*KGLCOLOR3IVPROC)(const GLint *v);
typedef void (*KGLCOLOR3SPROC)(GLshort red, GLshort green, GLshort blue);
typedef void (*KGLCOLOR3SVPROC)(const GLshort *v);
typedef void (*KGLCOLOR3UBPROC)(GLubyte red, GLubyte green, GLubyte blue);
typedef void (*KGLCOLOR3UBVPROC)(const GLubyte *v);
typedef void (*KGLCOLOR3UIPROC)(GLuint red, GLuint green, GLuint blue);
typedef void (*KGLCOLOR3UIVPROC)(const GLuint *v);
typedef void (*KGLCOLOR3USPROC)(GLushort red, GLushort green, GLushort blue);
typedef void (*KGLCOLOR3USVPROC)(const GLushort *v);
typedef void (*KGLCOLOR4BPROC)(GLbyte red, GLbyte green, GLbyte blue, GLbyte alpha);
typedef void (*KGLCOLOR4BVPROC)(const GLbyte *v);
typedef void (*KGLCOLOR4DPROC)(GLdouble red, GLdouble green, GLdouble blue, GLdouble alpha);
typedef void (*KGLCOLOR4DVPROC)(const GLdouble *v);
typedef void (*KGLCOLOR4FPROC)(GLfloat red, GLfloat green, GLfloat blue, GLfloat alpha);
typedef void (*KGLCOLOR4FVPROC)(const GLfloat *v);
typedef void (*KGLCOLOR4IPROC)(GLint red, GLint green, GLint blue, GLint alpha);
typedef void (*KGLCOLOR4IVPROC)(const GLint *v);
typedef void (*KGLCOLOR4SPROC)(GLshort red, GLshort green, GLshort blue, GLshort alpha);
typedef void (*KGLCOLOR4SVPROC)(const GLshort *v);
typedef void (*KGLCOLOR4UBPROC)(GLubyte red, GLubyte green, GLubyte blue, GLubyte alpha);
typedef void (*KGLCOLOR4UBVPROC)(const GLubyte *v);
typedef void (*KGLCOLOR4UIPROC)(GLuint red, GLuint green, GLuint blue, GLuint alpha);
typedef void (*KGLCOLOR4UIVPROC)(const GLuint *v);
typedef void (*KGLCOLOR4USPROC)(GLushort red, GLushort green, GLushort blue, GLushort alpha);
typedef void (*KGLCOLOR4USVPROC)(const GLushort *v);
typedef void (*KGLCOLORMASKPROC)(GLboolean red, GLboolean green, GLboolean blue, GLboolean alpha);
typedef void (*KGLCOLORMATERIALPROC)(GLenum face, GLenum mode);
typedef void (*KGLCOLORPOINTERPROC)(GLint size, GLenum type, GLsizei stride, const void *pointer);
typedef void (*KGLCOPYPIXELSPROC)(GLint x, GLint y, GLsizei width, GLsizei height, GLenum type);
typedef void (*KGLCOPYTEXIMAGE1DPROC)(GLenum target, GLint level, GLenum internalFormat, GLint x, GLint y, GLsizei width, GLint border);
typedef void (*KGLCOPYTEXIMAGE2DPROC)(GLenum target, GLint level, GLenum internalFormat, GLint x, GLint y, GLsizei width, GLsizei height, GLint border);
typedef void (*KGLCOPYTEXSUBIMAGE1DPROC)(GLenum target, GLint level, GLint xoffset, GLint x, GLint y, GLsizei width);
typedef void (*KGLCOPYTEXSUBIMAGE2DPROC)(GLenum target, GLint level, GLint xoffset, GLint yoffset, GLint x, GLint y, GLsizei width, GLsizei height);
typedef void (*KGLCULLFACEPROC)(GLenum mode);
typedef void (*KGLDELETELISTSPROC)(GLuint list, GLsizei range);
typedef void (*KGLDELETETEXTURESPROC)(GLsizei n, const GLuint *textures);
typedef void (*KGLDEPTHFUNCPROC)(GLenum func);
typedef void (*KGLDEPTHMASKPROC)(GLboolean flag);
typedef void (*KGLDEPTHRANGEPROC)(GLclampd zNear, GLclampd zFar);
typedef void (*KGLDISABLEPROC)(GLenum cap);
typedef void (*KGLDISABLECLIENTSTATEPROC)(GLenum array);
typedef void (*KGLDRAWARRAYSPROC)(GLenum mode, GLint first, GLsizei count);
typedef void (*KGLDRAWBUFFERPROC)(GLenum mode);
typedef void (*KGLDRAWELEMENTSPROC)(GLenum mode, GLsizei count, GLenum type, const void *indices);
typedef void (*KGLDRAWPIXELSPROC)(GLsizei width, GLsizei height, GLenum format, GLenum type, const void *pixels);
typedef void (*KGLEDGEFLAGPROC)(GLboolean flag);
typedef void (*KGLEDGEFLAGPOINTERPROC)(GLsizei stride, const void *pointer);
typedef void (*KGLEDGEFLAGVPROC)(const GLboolean *flag);
typedef void (*KGLENABLEPROC)(GLenum cap);
typedef void (*KGLENABLECLIENTSTATEPROC)(GLenum array);
typedef void (*KGLENDPROC)(void);
typedef void (*KGLENDLISTPROC)(void);
typedef void (*KGLEVALCOORD1DPROC)(GLdouble u);
typedef void (*KGLEVALCOORD1DVPROC)(const GLdouble *u);
typedef void (*KGLEVALCOORD1FPROC)(GLfloat u);
typedef void (*KGLEVALCOORD1FVPROC)(const GLfloat *u);
typedef void (*KGLEVALCOORD2DPROC)(GLdouble u, GLdouble v);
typedef void (*KGLEVALCOORD2DVPROC)(const GLdouble *u);
typedef void (*KGLEVALCOORD2FPROC)(GLfloat u, GLfloat v);
typedef void (*KGLEVALCOORD2FVPROC)(const GLfloat *u);
typedef void (*KGLEVALMESH1PROC)(GLenum mode, GLint i1, GLint i2);
typedef void (*KGLEVALMESH2PROC)(GLenum mode, GLint i1, GLint i2, GLint j1, GLint j2);
typedef void (*KGLEVALPOINT1PROC)(GLint i);
typedef void (*KGLEVALPOINT2PROC)(GLint i, GLint j);
typedef void (*KGLFEEDBACKBUFFERPROC)(GLsizei size, GLenum type, GLfloat *buffer);
typedef void (*KGLFINISHPROC)(void);
typedef void (*KGLFLUSHPROC)(void);
typedef void (*KGLFOGFPROC)(GLenum pname, GLfloat param);
typedef void (*KGLFOGFVPROC)(GLenum pname, const GLfloat *params);
typedef void (*KGLFOGIPROC)(GLenum pname, GLint param);
typedef void (*KGLFOGIVPROC)(GLenum pname, const GLint *params);
typedef void (*KGLFRONTFACEPROC)(GLenum mode);
typedef void (*KGLFRUSTUMPROC)(GLdouble left, GLdouble right, GLdouble bottom, GLdouble top, GLdouble zNear, GLdouble zFar);
typedef GLuint (*KGLGENLISTSPROC)(GLsizei range);
typedef void (*KGLGENTEXTURESPROC)(GLsizei n, GLuint *textures);
typedef void (*KGLGETBOOLEANVPROC)(GLenum pname, GLboolean params);
typedef void (*KGLGETCLIPPLANEPROC)(GLenum plane, GLdouble *equation);
typedef void (*KGLGETDOUBLEVPROC)(GLenum pname, GLdouble *params);
typedef GLenum (*KGLGETERRORPROC)(void);
typedef void (*KGLGETFLOATVPROC)(GLenum pname, GLfloat *params);
typedef void (*KGLGETINTEGERVPROC)(GLenum pname, GLint *params);
typedef void (*KGLGETLIGHTFVPROC)(GLenum light, GLenum pname, GLfloat *params);
typedef void (*KGLGETLIGHTIVPROC)(GLenum light, GLenum pname, GLint *params);
typedef void (*KGLGETMAPDVPROC)(GLenum target, GLenum query, GLdouble *v);
typedef void (*KGLGETMAPFVPROC)(GLenum target, GLenum query, GLfloat *v);
typedef void (*KGLGETMAPIVPROC)(GLenum target, GLenum query, GLint *v);
typedef void (*KGLGETMATERIALFVPROC)(GLenum face, GLenum pname, GLfloat *params);
typedef void (*KGLGETMATERIALIVPROC)(GLenum face, GLenum pname, GLint *params);
typedef void (*KGLGETPIXELMAPFVPROC)(GLenum map, GLfloat *values);
typedef void (*KGLGETPIXELMAPUIVPROC)(GLenum map, GLuint *values);
typedef void (*KGLGETPIXELMAPUSVPROC)(GLenum map, GLushort *values);
typedef void (*KGLGETPOINTERVPROC)(GLenum pname, void* *params);
typedef void (*KGLGETPOLYGONSTIPPLEPROC)(GLubyte *mask);
typedef const GLubyte *(*KGLGETSTRINGPROC)(GLenum name);
typedef void (*KGLGETTEXENVFVPROC)(GLenum target, GLenum pname, GLfloat *params);
typedef void (*KGLGETTEXENVIVPROC)(GLenum target, GLenum pname, GLint *params);
typedef void (*KGLGETTEXGENDVPROC)(GLenum coord, GLenum pname, GLdouble *params);
typedef void (*KGLGETTEXGENFVPROC)(GLenum coord, GLenum pname, GLfloat *params);
typedef void (*KGLGETTEXGENIVPROC)(GLenum coord, GLenum pname, GLint *params);
typedef void (*KGLGETTEXIMAGEPROC)(GLenum target, GLint level, GLenum format, GLenum type, void *pixels);
typedef void (*KGLGETTEXLEVELPARAMETERFVPROC)(GLenum target, GLint level, GLenum pname, GLfloat *params);
typedef void (*KGLGETTEXLEVELPARAMETERIVPROC)(GLenum target, GLint level, GLenum pname, GLint *params);
typedef void (*KGLGETTEXPARAMETERFVPROC)(GLenum target, GLenum pname, GLfloat *params);
typedef void (*KGLGETTEXPARAMETERIVPROC)(GLenum target, GLenum pname, GLint *params);
typedef void (*KGLHINTPROC)(GLenum target, GLenum mode);
typedef void (*KGLINDEXMASKPROC)(GLuint mask);
typedef void (*KGLINDEXPOINTERPROC)(GLenum type, GLsizei stride, const void *pointer);
typedef void (*KGLINDEXDPROC)(GLdouble c);
typedef void (*KGLINDEXDVPROC)(const GLdouble *c);
typedef void (*KGLINDEXFPROC)(GLfloat c);
typedef void (*KGLINDEXFVPROC)(const GLfloat *c);
typedef void (*KGLINDEXIPROC)(GLint c);
typedef void (*KGLINDEXIVPROC)(const GLint *c);
typedef void (*KGLINDEXSPROC)(GLshort c);
typedef void (*KGLINDEXSVPROC)(const GLshort *c);
typedef void (*KGLINDEXUBPROC)(GLubyte c);
typedef void (*KGLINDEXUBVPROC)(const GLubyte *c);
typedef void (*KGLINITNAMESPROC)(void);
typedef void (*KGLINTERLEAVEDARRAYSPROC)(GLenum format, GLsizei stride, const void *pointer);
typedef GLboolean (*KGLISENABLEDPROC)(GLenum cap);
typedef GLboolean (*KGLISLISTPROC)(GLuint list);
typedef GLboolean (*KGLISTEXTUREPROC)(GLuint texture);
typedef void (*KGLLIGHTMODELFPROC)(GLenum pname, GLfloat param);
typedef void (*KGLLIGHTMODELFVPROC)(GLenum pname, const GLfloat *params);
typedef void (*KGLLIGHTMODELIPROC)(GLenum pname, GLint param);
typedef void (*KGLLIGHTMODELIVPROC)(GLenum pname, const GLint *params);
typedef void (*KGLLIGHTFPROC)(GLenum light, GLenum pname, GLfloat param);
typedef void (*KGLLIGHTFVPROC)(GLenum light, GLenum pname, const GLfloat *params);
typedef void (*KGLLIGHTIPROC)(GLenum light, GLenum pname, GLint param);
typedef void (*KGLLIGHTIVPROC)(GLenum light, GLenum pname, const GLint *params);
typedef void (*KGLLINESTIPPLEPROC)(GLint factor, GLushort pattern);
typedef void (*KGLLINEWIDTHPROC)(GLfloat width);
typedef void (*KGLLISTBASEPROC)(GLuint base);
typedef void (*KGLLOADIDENTITYPROC)(void);
typedef void (*KGLLOADMATRIXDPROC)(const GLdouble *m);
typedef void (*KGLLOADMATRIXFPROC)(const GLfloat *m);
typedef void (*KGLLOADNAMEPROC)(GLuint name);
typedef void (*KGLLOGICOPPROC)(GLenum opcode);
typedef void (*KGLMAP1DPROC)(GLenum target, GLdouble u1, GLdouble u2, GLint stride, GLint order, const GLdouble *points);
typedef void (*KGLMAP1FPROC)(GLenum target, GLfloat u1, GLfloat u2, GLint stride, GLint order, const GLfloat *points);
typedef void (*KGLMAP2DPROC)(GLenum target, GLdouble u1, GLdouble u2, GLint ustride, GLint uorder, GLdouble v1, GLdouble v2, GLint vstride, GLint vorder, const GLdouble *points);
typedef void (*KGLMAP2FPROC)(GLenum target, GLfloat u1, GLfloat u2, GLint ustride, GLint uorder, GLfloat v1, GLfloat v2, GLint vstride, GLint vorder, const GLfloat *points);
typedef void (*KGLMAPGRID1DPROC)(GLint un, GLdouble u1, GLdouble u2);
typedef void (*KGLMAPGRID1FPROC)(GLint un, GLfloat u1, GLfloat u2);
typedef void (*KGLMAPGRID2DPROC)(GLint un, GLdouble u1, GLdouble u2, GLint vn, GLdouble v1, GLdouble v2);
typedef void (*KGLMAPGRID2FPROC)(GLint un, GLfloat u1, GLfloat u2, GLint vn, GLfloat v1, GLfloat v2);
typedef void (*KGLMATERIALFPROC)(GLenum face, GLenum pname, GLfloat param);
typedef void (*KGLMATERIALFVPROC)(GLenum face, GLenum pname, const GLfloat *params);
typedef void (*KGLMATERIALIPROC)(GLenum face, GLenum pname, GLint param);
typedef void (*KGLMATERIALIVPROC)(GLenum face, GLenum pname, const GLint *params);
typedef void (*KGLMATRIXMODEPROC)(GLenum mode);
typedef void (*KGLMULTMATRIXDPROC)(const GLdouble *m);
typedef void (*KGLMULTMATRIXFPROC)(const GLfloat *m);
typedef void (*KGLNEWLISTPROC)(GLuint list, GLenum mode);
typedef void (*KGLNORMAL3BPROC)(GLbyte nx, GLbyte ny, GLbyte nz);
typedef void (*KGLNORMAL3BVPROC)(const GLbyte *v);
typedef void (*KGLNORMAL3DPROC)(GLdouble nx, GLdouble ny, GLdouble nz);
typedef void (*KGLNORMAL3DVPROC)(const GLdouble *v);
typedef void (*KGLNORMAL3FPROC)(GLfloat nx, GLfloat ny, GLfloat nz);
typedef void (*KGLNORMAL3FVPROC)(const GLfloat *v);
typedef void (*KGLNORMAL3IPROC)(GLint nx, GLint ny, GLint nz);
typedef void (*KGLNORMAL3IVPROC)(const GLint *v);
typedef void (*KGLNORMAL3SPROC)(GLshort nx, GLshort ny, GLshort nz);
typedef void (*KGLNORMAL3SVPROC)(const GLshort *v);
typedef void (*KGLNORMALPOINTERPROC)(GLenum type, GLsizei stride, const void *pointer);
typedef void (*KGLORTHOPROC)(GLdouble left, GLdouble right, GLdouble bottom, GLdouble top, GLdouble zNear, GLdouble zFar);
typedef void (*KGLPASSTHROUGHPROC)(GLfloat token);
typedef void (*KGLPIXELMAPFVPROC)(GLenum map, GLsizei mapsize, const GLfloat *values);
typedef void (*KGLPIXELMAPUIVPROC)(GLenum map, GLsizei mapsize, const GLuint *values);
typedef void (*KGLPIXELMAPUSVPROC)(GLenum map, GLsizei mapsize, const GLushort *values);
typedef void (*KGLPIXELSTOREFPROC)(GLenum pname, GLfloat param);
typedef void (*KGLPIXELSTOREIPROC)(GLenum pname, GLint param);
typedef void (*KGLPIXELTRANSFERFPROC)(GLenum pname, GLfloat param);
typedef void (*KGLPIXELTRANSFERIPROC)(GLenum pname, GLint param);
typedef void (*KGLPIXELZOOMPROC)(GLfloat xfactor, GLfloat yfactor);
typedef void (*KGLPOINTSIZEPROC)(GLfloat size);
typedef void (*KGLPOLYGONMODEPROC)(GLenum face, GLenum mode);
typedef void (*KGLPOLYGONOFFSETPROC)(GLfloat factor, GLfloat units);
typedef void (*KGLPOLYGONSTIPPLEPROC)(const GLubyte *mask);
typedef void (*KGLPOPATTRIBPROC)(void);
typedef void (*KGLPOPCLIENTATTRIBPROC)(void);
typedef void (*KGLPOPMATRIXPROC)(void);
typedef void (*KGLPOPNAMEPROC)(void);
typedef void (*KGLPRIORITIZETEXTURESPROC)(GLsizei n, const GLuint *textures, const GLclampf *priorities);
typedef void (*KGLPUSHATTRIBPROC)(GLbitfield mask);
typedef void (*KGLPUSHCLIENTATTRIBPROC)(GLbitfield mask);
typedef void (*KGLPUSHMATRIXPROC)(void);
typedef void (*KGLPUSHNAMEPROC)(GLuint name);
typedef void (*KGLRASTERPOS2DPROC)(GLdouble x, GLdouble y);
typedef void (*KGLRASTERPOS2DVPROC)(const GLdouble *v);
typedef void (*KGLRASTERPOS2FPROC)(GLfloat x, GLfloat y);
typedef void (*KGLRASTERPOS2FVPROC)(const GLfloat *v);
typedef void (*KGLRASTERPOS2IPROC)(GLint x, GLint y);
typedef void (*KGLRASTERPOS2IVPROC)(const GLint *v);
typedef void (*KGLRASTERPOS2SPROC)(GLshort x, GLshort y);
typedef void (*KGLRASTERPOS2SVPROC)(const GLshort *v);
typedef void (*KGLRASTERPOS3DPROC)(GLdouble x, GLdouble y, GLdouble z);
typedef void (*KGLRASTERPOS3DVPROC)(const GLdouble *v);
typedef void (*KGLRASTERPOS3FPROC)(GLfloat x, GLfloat y, GLfloat z);
typedef void (*KGLRASTERPOS3FVPROC)(const GLfloat *v);
typedef void (*KGLRASTERPOS3IPROC)(GLint x, GLint y, GLint z);
typedef void (*KGLRASTERPOS3IVPROC)(const GLint *v);
typedef void (*KGLRASTERPOS3SPROC)(GLshort x, GLshort y, GLshort z);
typedef void (*KGLRASTERPOS3SVPROC)(const GLshort *v);
typedef void (*KGLRASTERPOS4DPROC)(GLdouble x, GLdouble y, GLdouble z, GLdouble w);
typedef void (*KGLRASTERPOS4DVPROC)(const GLdouble *v);
typedef void (*KGLRASTERPOS4FPROC)(GLfloat x, GLfloat y, GLfloat z, GLfloat w);
typedef void (*KGLRASTERPOS4FVPROC)(const GLfloat *v);
typedef void (*KGLRASTERPOS4IPROC)(GLint x, GLint y, GLint z, GLint w);
typedef void (*KGLRASTERPOS4IVPROC)(const GLint *v);
typedef void (*KGLRASTERPOS4SPROC)(GLshort x, GLshort y, GLshort z, GLshort w);
typedef void (*KGLRASTERPOS4SVPROC)(const GLshort *v);
typedef void (*KGLREADBUFFERPROC)(GLenum mode);
typedef void (*KGLREADPIXELSPROC)(GLint x, GLint y, GLsizei width, GLsizei height, GLenum format, GLenum type, void *pixels);
typedef void (*KGLRECTDPROC)(GLdouble x1, GLdouble y1, GLdouble x2, GLdouble y2);
typedef void (*KGLRECTDVPROC)(const GLdouble *v1, const GLdouble *v2);
typedef void (*KGLRECTFPROC)(GLfloat x1, GLfloat y1, GLfloat x2, GLfloat y2);
typedef void (*KGLRECTFVPROC)(const GLfloat *v1, const GLfloat *v2);
typedef void (*KGLRECTIPROC)(GLint x1, GLint y1, GLint x2, GLint y2);
typedef void (*KGLRECTIVPROC)(const GLint *v1, const GLint *v2);
typedef void (*KGLRECTSPROC)(GLshort x1, GLshort y1, GLshort x2, GLshort y2);
typedef void (*KGLRECTSVPROC)(const GLshort *v1, const GLshort *v2);
typedef GLint (*KGLRENDERMODEPROC)(GLenum mode);
typedef void (*KGLROTATEDPROC)(GLdouble angle, GLdouble x, GLdouble y, GLdouble z);
typedef void (*KGLROTATEFPROC)(GLfloat angle, GLfloat x, GLfloat y, GLfloat z);
typedef void (*KGLSCALEDPROC)(GLdouble x, GLdouble y, GLdouble z);
typedef void (*KGLSCALEFPROC)(GLfloat x, GLfloat y, GLfloat z);
typedef void (*KGLSCISSORPROC)(GLint x, GLint y, GLsizei width, GLsizei height);
typedef void (*KGLSELECTBUFFERPROC)(GLsizei size, GLuint *buffer);
typedef void (*KGLSHADEMODELPROC)(GLenum mode);
typedef void (*KGLSTENCILFUNCPROC)(GLenum func, GLint ref, GLuint mask);
typedef void (*KGLSTENCILMASKPROC)(GLuint mask);
typedef void (*KGLSTENCILOPPROC)(GLenum fail, GLenum zfail, GLenum zpass);
typedef void (*KGLTEXCOORD1DPROC)(GLdouble s);
typedef void (*KGLTEXCOORD1DVPROC)(const GLdouble *v);
typedef void (*KGLTEXCOORD1FPROC)(GLfloat s);
typedef void (*KGLTEXCOORD1FVPROC)(const GLfloat *v);
typedef void (*KGLTEXCOORD1IPROC)(GLint s);
typedef void (*KGLTEXCOORD1IVPROC)(const GLint *v);
typedef void (*KGLTEXCOORD1SPROC)(GLshort s);
typedef void (*KGLTEXCOORD1SVPROC)(const GLshort *v);
typedef void (*KGLTEXCOORD2DPROC)(GLdouble s, GLdouble t);
typedef void (*KGLTEXCOORD2DVPROC)(const GLdouble *v);
typedef void (*KGLTEXCOORD2FPROC)(GLfloat s, GLfloat t);
typedef void (*KGLTEXCOORD2FVPROC)(const GLfloat *v);
typedef void (*KGLTEXCOORD2IPROC)(GLint s, GLint t);
typedef void (*KGLTEXCOORD2IVPROC)(const GLint *v);
typedef void (*KGLTEXCOORD2SPROC)(GLshort s, GLshort t);
typedef void (*KGLTEXCOORD2SVPROC)(const GLshort *v);
typedef void (*KGLTEXCOORD3DPROC)(GLdouble s, GLdouble t, GLdouble r);
typedef void (*KGLTEXCOORD3DVPROC)(const GLdouble *v);
typedef void (*KGLTEXCOORD3FPROC)(GLfloat s, GLfloat t, GLfloat r);
typedef void (*KGLTEXCOORD3FVPROC)(const GLfloat *v);
typedef void (*KGLTEXCOORD3IPROC)(GLint s, GLint t, GLint r);
typedef void (*KGLTEXCOORD3IVPROC)(const GLint *v);
typedef void (*KGLTEXCOORD3SPROC)(GLshort s, GLshort t, GLshort r);
typedef void (*KGLTEXCOORD3SVPROC)(const GLshort *v);
typedef void (*KGLTEXCOORD4DPROC)(GLdouble s, GLdouble t, GLdouble r, GLdouble q);
typedef void (*KGLTEXCOORD4DVPROC)(const GLdouble *v);
typedef void (*KGLTEXCOORD4FPROC)(GLfloat s, GLfloat t, GLfloat r, GLfloat q);
typedef void (*KGLTEXCOORD4FVPROC)(const GLfloat *v);
typedef void (*KGLTEXCOORD4IPROC)(GLint s, GLint t, GLint r, GLint q);
typedef void (*KGLTEXCOORD4IVPROC)(const GLint *v);
typedef void (*KGLTEXCOORD4SPROC)(GLshort s, GLshort t, GLshort r, GLshort q);
typedef void (*KGLTEXCOORD4SVPROC)(const GLshort *v);
typedef void (*KGLTEXCOORDPOINTERPROC)(GLint size, GLenum type, GLsizei stride, const void *pointer);
typedef void (*KGLTEXENVFPROC)(GLenum target, GLenum pname, GLfloat param);
typedef void (*KGLTEXENVFVPROC)(GLenum target, GLenum pname, const GLfloat *params);
typedef void (*KGLTEXENVIPROC)(GLenum target, GLenum pname, GLint param);
typedef void (*KGLTEXENVIVPROC)(GLenum target, GLenum pname, const GLint *params);
typedef void (*KGLTEXGENDPROC)(GLenum coord, GLenum pname, GLdouble param);
typedef void (*KGLTEXGENDVPROC)(GLenum coord, GLenum pname, const GLdouble *params);
typedef void (*KGLTEXGENFPROC)(GLenum coord, GLenum pname, GLfloat param);
typedef void (*KGLTEXGENFVPROC)(GLenum coord, GLenum pname, const GLfloat *params);
typedef void (*KGLTEXGENIPROC)(GLenum coord, GLenum pname, GLint param);
typedef void (*KGLTEXGENIVPROC)(GLenum coord, GLenum pname, const GLint *params);
typedef void (*KGLTEXIMAGE1DPROC)(GLenum target, GLint level, GLint internalformat, GLsizei width, GLint border, GLenum format, GLenum type, const void *pixels);
typedef void (*KGLTEXIMAGE2DPROC)(GLenum target, GLint level, GLint internalformat, GLsizei width, GLsizei height, GLint border, GLenum format, GLenum type, const void *pixels);
typedef void (*KGLTEXPARAMETERFPROC)(GLenum target, GLenum pname, GLfloat param);
typedef void (*KGLTEXPARAMETERFVPROC)(GLenum target, GLenum pname, const GLfloat *params);
typedef void (*KGLTEXPARAMETERIPROC)(GLenum target, GLenum pname, GLint param);
typedef void (*KGLTEXPARAMETERIVPROC)(GLenum target, GLenum pname, const GLint *params);
typedef void (*KGLTEXSUBIMAGE1DPROC)(GLenum target, GLint level, GLint xoffset, GLsizei width, GLenum format, GLenum type, const void *pixels);
typedef void (*KGLTEXSUBIMAGE2DPROC)(GLenum target, GLint level, GLint xoffset, GLint yoffset, GLsizei width, GLsizei height, GLenum format, GLenum type, const void *pixels);
typedef void (*KGLTRANSLATEDPROC)(GLdouble x, GLdouble y, GLdouble z);
typedef void (*KGLTRANSLATEFPROC)(GLfloat x, GLfloat y, GLfloat z);
typedef void (*KGLVERTEX2DPROC)(GLdouble x, GLdouble y);
typedef void (*KGLVERTEX2DVPROC)(const GLdouble *v);
typedef void (*KGLVERTEX2FPROC)(GLfloat x, GLfloat y);
typedef void (*KGLVERTEX2FVPROC)(const GLfloat *v);
typedef void (*KGLVERTEX2IPROC)(GLint x, GLint y);
typedef void (*KGLVERTEX2IVPROC)(const GLint *v);
typedef void (*KGLVERTEX2SPROC)(GLshort x, GLshort y);
typedef void (*KGLVERTEX2SVPROC)(const GLshort *v);
typedef void (*KGLVERTEX3DPROC)(GLdouble x, GLdouble y, GLdouble z);
typedef void (*KGLVERTEX3DVPROC)(const GLdouble *v);
typedef void (*KGLVERTEX3FPROC)(GLfloat x, GLfloat y, GLfloat z);
typedef void (*KGLVERTEX3FVPROC)(const GLfloat *v);
typedef void (*KGLVERTEX3IPROC)(GLint x, GLint y, GLint z);
typedef void (*KGLVERTEX3IVPROC)(const GLint *v);
typedef void (*KGLVERTEX3SPROC)(GLshort x, GLshort y, GLshort z);
typedef void (*KGLVERTEX3SVPROC)(const GLshort *v);
typedef void (*KGLVERTEX4DPROC)(GLdouble x, GLdouble y, GLdouble z, GLdouble w);
typedef void (*KGLVERTEX4DVPROC)(const GLdouble *v);
typedef void (*KGLVERTEX4FPROC)(GLfloat x, GLfloat y, GLfloat z, GLfloat w);
typedef void (*KGLVERTEX4FVPROC)(const GLfloat *v);
typedef void (*KGLVERTEX4IPROC)(GLint x, GLint y, GLint z, GLint w);
typedef void (*KGLVERTEX4IVPROC)(const GLint *v);
typedef void (*KGLVERTEX4SPROC)(GLshort x, GLshort y, GLshort z, GLshort w);
typedef void (*KGLVERTEX4SVPROC)(const GLshort *v);
typedef void (*KGLVERTEXPOINTERPROC)(GLint size, GLenum type, GLsizei stride, const void *pointer);
typedef void (*KGLVIEWPORTPROC)(GLint x, GLint y, GLsizei width, GLsizei height);

extern KGLACCUMPROC kglAccum;
extern KGLALPHAFUNCPROC kglAlphaFunc;
extern KGLARETEXTURESRESIDENTPROC kglAreTexturesResident;
extern KGLARRAYELEMENTPROC kglArrayElement;
extern KGLBEGINPROC kglBegin;
extern KGLBINDTEXTUREPROC kglBindTexture;
extern KGLBITMAPPROC kglBitmap;
extern KGLBLENDFUNCPROC kglBlendFunc;
extern KGLCALLLISTPROC kglCallList;
extern KGLCALLLISTSPROC kglCallLists;
extern KGLCLEARPROC kglClear;
extern KGLCLEARACCUMPROC kglClearAccum;
extern KGLCLEARCOLORPROC kglClearColor;
extern KGLCLEARDEPTHPROC kglClearDepth;
extern KGLCLEARINDEXPROC kglClearIndex;
extern KGLCLEARSTENCILPROC kglClearStencil;
extern KGLCLIPPLANEPROC kglClipPlane;
extern KGLCOLOR3BPROC kglColor3b;
extern KGLCOLOR3BVPROC kglColor3bv;
extern KGLCOLOR3DPROC kglColor3d;
extern KGLCOLOR3DVPROC kglColor3dv;
extern KGLCOLOR3FPROC kglColor3f;
extern KGLCOLOR3FVPROC kglColor3fv;
extern KGLCOLOR3IPROC kglColor3i;
extern KGLCOLOR3IVPROC kglColor3iv;
extern KGLCOLOR3SPROC kglColor3s;
extern KGLCOLOR3SVPROC kglColor3sv;
extern KGLCOLOR3UBPROC kglColor3ub;
extern KGLCOLOR3UBVPROC kglColor3ubv;
extern KGLCOLOR3UIPROC kglColor3ui;
extern KGLCOLOR3UIVPROC kglColor3uiv;
extern KGLCOLOR3USPROC kglColor3us;
extern KGLCOLOR3USVPROC kglColor3usv;
extern KGLCOLOR4BPROC kglColor4b;
extern KGLCOLOR4BVPROC kglColor4bv;
extern KGLCOLOR4DPROC kglColor4d;
extern KGLCOLOR4DVPROC kglColor4dv;
extern KGLCOLOR4FPROC kglColor4f;
extern KGLCOLOR4FVPROC kglColor4fv;
extern KGLCOLOR4IPROC kglColor4i;
extern KGLCOLOR4IVPROC kglColor4iv;
extern KGLCOLOR4SPROC kglColor4s;
extern KGLCOLOR4SVPROC kglColor4sv;
extern KGLCOLOR4UBPROC kglColor4ub;
extern KGLCOLOR4UBVPROC kglColor4ubv;
extern KGLCOLOR4UIPROC kglColor4ui;
extern KGLCOLOR4UIVPROC kglColor4uiv;
extern KGLCOLOR4USPROC kglColor4us;
extern KGLCOLOR4USVPROC kglColor4usv;
extern KGLCOLORMASKPROC kglColorMask;
extern KGLCOLORMATERIALPROC kglColorMaterial;
extern KGLCOLORPOINTERPROC kglColorPointer;
extern KGLCOPYPIXELSPROC kglCopyPixels;
extern KGLCOPYTEXIMAGE1DPROC kglCopyTexImage1D;
extern KGLCOPYTEXIMAGE2DPROC kglCopyTexImage2D;
extern KGLCOPYTEXSUBIMAGE1DPROC kglCopyTexSubImage1D;
extern KGLCOPYTEXSUBIMAGE2DPROC kglCopyTexSubImage2D;
extern KGLCULLFACEPROC kglCullFace;
extern KGLDELETELISTSPROC kglDeleteLists;
extern KGLDELETETEXTURESPROC kglDeleteTextures;
extern KGLDEPTHFUNCPROC kglDepthFunc;
extern KGLDEPTHMASKPROC kglDepthMask;
extern KGLDEPTHRANGEPROC kglDepthRange;
extern KGLDISABLEPROC kglDisable;
extern KGLDISABLECLIENTSTATEPROC kglDisableClientState;
extern KGLDRAWARRAYSPROC kglDrawArrays;
extern KGLDRAWBUFFERPROC kglDrawBuffer;
extern KGLDRAWELEMENTSPROC kglDrawElements;
extern KGLDRAWPIXELSPROC kglDrawPixels;
extern KGLEDGEFLAGPROC kglEdgeFlag;
extern KGLEDGEFLAGPOINTERPROC kglEdgeFlagPointer;
extern KGLEDGEFLAGVPROC kglEdgeFlagv;
extern KGLENABLEPROC kglEnable;
extern KGLENABLECLIENTSTATEPROC kglEnableClientState;
extern KGLENDPROC kglEnd;
extern KGLENDLISTPROC kglEndList;
extern KGLEVALCOORD1DPROC kglEvalCoord1d;
extern KGLEVALCOORD1DVPROC kglEvalCoord1dv;
extern KGLEVALCOORD1FPROC kglEvalCoord1f;
extern KGLEVALCOORD1FVPROC kglEvalCoord1fv;
extern KGLEVALCOORD2DPROC kglEvalCoord2d;
extern KGLEVALCOORD2DVPROC kglEvalCoord2dv;
extern KGLEVALCOORD2FPROC kglEvalCoord2f;
extern KGLEVALCOORD2FVPROC kglEvalCoord2fv;
extern KGLEVALMESH1PROC kglEvalMesh1;
extern KGLEVALMESH2PROC kglEvalMesh2;
extern KGLEVALPOINT1PROC kglEvalPoint1;
extern KGLEVALPOINT2PROC kglEvalPoint2;
extern KGLFEEDBACKBUFFERPROC kglFeedbackBuffer;
extern KGLFINISHPROC kglFinish;
extern KGLFLUSHPROC kglFlush;
extern KGLFOGFPROC kglFogf;
extern KGLFOGFVPROC kglFogfv;
extern KGLFOGIPROC kglFogi;
extern KGLFOGIVPROC kglFogiv;
extern KGLFRONTFACEPROC kglFrontFace;
extern KGLFRUSTUMPROC kglFrustum;
extern KGLGENLISTSPROC kglGenLists;
extern KGLGENTEXTURESPROC kglGenTextures;
extern KGLGETBOOLEANVPROC kglGetBooleanv;
extern KGLGETCLIPPLANEPROC kglGetClipPlane;
extern KGLGETDOUBLEVPROC kglGetDoublev;
extern KGLGETERRORPROC kglGetError;
extern KGLGETFLOATVPROC kglGetFloatv;
extern KGLGETINTEGERVPROC kglGetIntegerv;
extern KGLGETLIGHTFVPROC kglGetLightfv;
extern KGLGETLIGHTIVPROC kglGetLightiv;
extern KGLGETMAPDVPROC kglGetMapdv;
extern KGLGETMAPFVPROC kglGetMapfv;
extern KGLGETMAPIVPROC kglGetMapiv;
extern KGLGETMATERIALFVPROC kglGetMaterialfv;
extern KGLGETMATERIALIVPROC kglGetMaterialiv;
extern KGLGETPIXELMAPFVPROC kglGetPixelMapfv;
extern KGLGETPIXELMAPUIVPROC kglGetPixelMapuiv;
extern KGLGETPIXELMAPUSVPROC kglGetPixelMapusv;
extern KGLGETPOINTERVPROC kglGetPointerv;
extern KGLGETPOLYGONSTIPPLEPROC kglGetPolygonStipple;
extern KGLGETSTRINGPROC kglGetString;
extern KGLGETTEXENVFVPROC kglGetTexEnvfv;
extern KGLGETTEXENVIVPROC kglGetTexEnviv;
extern KGLGETTEXGENDVPROC kglGetTexGendv;
extern KGLGETTEXGENFVPROC kglGetTexGenfv;
extern KGLGETTEXGENIVPROC kglGetTexGeniv;
extern KGLGETTEXIMAGEPROC kglGetTexImage;
extern KGLGETTEXLEVELPARAMETERFVPROC kglGetTexLevelParameterfv;
extern KGLGETTEXLEVELPARAMETERIVPROC kglGetTexLevelParameteriv;
extern KGLGETTEXPARAMETERFVPROC kglGetTexParameterfv;
extern KGLGETTEXPARAMETERIVPROC kglGetTexParameteriv;
extern KGLHINTPROC kglHint;
extern KGLINDEXMASKPROC kglIndexMask;
extern KGLINDEXPOINTERPROC kglIndexPointer;
extern KGLINDEXDPROC kglIndexd;
extern KGLINDEXDVPROC kglIndexdv;
extern KGLINDEXFPROC kglIndexf;
extern KGLINDEXFVPROC kglIndexfv;
extern KGLINDEXIPROC kglIndexi;
extern KGLINDEXIVPROC kglIndexiv;
extern KGLINDEXSPROC kglIndexs;
extern KGLINDEXSVPROC kglIndexsv;
extern KGLINDEXUBPROC kglIndexub;
extern KGLINDEXUBVPROC kglIndexubv;
extern KGLINITNAMESPROC kglInitNames;
extern KGLINTERLEAVEDARRAYSPROC kglInterleavedArrays;
extern KGLISENABLEDPROC kglIsEnabled;
extern KGLISLISTPROC kglIsList;
extern KGLISTEXTUREPROC kglIsTexture;
extern KGLLIGHTMODELFPROC kglLightModelf;
extern KGLLIGHTMODELFVPROC kglLightModelfv;
extern KGLLIGHTMODELIPROC kglLightModeli;
extern KGLLIGHTMODELIVPROC kglLightModeliv;
extern KGLLIGHTFPROC kglLightf;
extern KGLLIGHTFVPROC kglLightfv;
extern KGLLIGHTIPROC kglLighti;
extern KGLLIGHTIVPROC kglLightiv;
extern KGLLINESTIPPLEPROC kglLineStipple;
extern KGLLINEWIDTHPROC kglLineWidth;
extern KGLLISTBASEPROC kglListBase;
extern KGLLOADIDENTITYPROC kglLoadIdentity;
extern KGLLOADMATRIXDPROC kglLoadMatrixd;
extern KGLLOADMATRIXFPROC kglLoadMatrixf;
extern KGLLOADNAMEPROC kglLoadName;
extern KGLLOGICOPPROC kglLogicOp;
extern KGLMAP1DPROC kglMap1d;
extern KGLMAP1FPROC kglMap1f;
extern KGLMAP2DPROC kglMap2d;
extern KGLMAP2FPROC kglMap2f;
extern KGLMAPGRID1DPROC kglMapGrid1d;
extern KGLMAPGRID1FPROC kglMapGrid1f;
extern KGLMAPGRID2DPROC kglMapGrid2d;
extern KGLMAPGRID2FPROC kglMapGrid2f;
extern KGLMATERIALFPROC kglMaterialf;
extern KGLMATERIALFVPROC kglMaterialfv;
extern KGLMATERIALIPROC kglMateriali;
extern KGLMATERIALIVPROC kglMaterialiv;
extern KGLMATRIXMODEPROC kglMatrixMode;
extern KGLMULTMATRIXDPROC kglMultMatrixd;
extern KGLMULTMATRIXFPROC kglMultMatrixf;
extern KGLNEWLISTPROC kglNewList;
extern KGLNORMAL3BPROC kglNormal3b;
extern KGLNORMAL3BVPROC kglNormal3bv;
extern KGLNORMAL3DPROC kglNormal3d;
extern KGLNORMAL3DVPROC kglNormal3dv;
extern KGLNORMAL3FPROC kglNormal3f;
extern KGLNORMAL3FVPROC kglNormal3fv;
extern KGLNORMAL3IPROC kglNormal3i;
extern KGLNORMAL3IVPROC kglNormal3iv;
extern KGLNORMAL3SPROC kglNormal3s;
extern KGLNORMAL3SVPROC kglNormal3sv;
extern KGLNORMALPOINTERPROC kglNormalPointer;
extern KGLORTHOPROC kglOrtho;
extern KGLPASSTHROUGHPROC kglPassThrough;
extern KGLPIXELMAPFVPROC kglPixelMapfv;
extern KGLPIXELMAPUIVPROC kglPixelMapuiv;
extern KGLPIXELMAPUSVPROC kglPixelMapusv;
extern KGLPIXELSTOREFPROC kglPixelStoref;
extern KGLPIXELSTOREIPROC kglPixelStorei;
extern KGLPIXELTRANSFERFPROC kglPixelTransferf;
extern KGLPIXELTRANSFERIPROC kglPixelTransferi;
extern KGLPIXELZOOMPROC kglPixelZoom;
extern KGLPOINTSIZEPROC kglPointSize;
extern KGLPOLYGONMODEPROC kglPolygonMode;
extern KGLPOLYGONOFFSETPROC kglPolygonOffset;
extern KGLPOLYGONSTIPPLEPROC kglPolygonStipple;
extern KGLPOPATTRIBPROC kglPopAttrib;
extern KGLPOPCLIENTATTRIBPROC kglPopClientAttrib;
extern KGLPOPMATRIXPROC kglPopMatrix;
extern KGLPOPNAMEPROC kglPopName;
extern KGLPRIORITIZETEXTURESPROC kglPrioritizeTextures;
extern KGLPUSHATTRIBPROC kglPushAttrib;
extern KGLPUSHCLIENTATTRIBPROC kglPushClientAttrib;
extern KGLPUSHMATRIXPROC kglPushMatrix;
extern KGLPUSHNAMEPROC kglPushName;
extern KGLRASTERPOS2DPROC kglRasterPos2d;
extern KGLRASTERPOS2DVPROC kglRasterPos2dv;
extern KGLRASTERPOS2FPROC kglRasterPos2f;
extern KGLRASTERPOS2FVPROC kglRasterPos2fv;
extern KGLRASTERPOS2IPROC kglRasterPos2i;
extern KGLRASTERPOS2IVPROC kglRasterPos2iv;
extern KGLRASTERPOS2SPROC kglRasterPos2s;
extern KGLRASTERPOS2SVPROC kglRasterPos2sv;
extern KGLRASTERPOS3DPROC kglRasterPos3d;
extern KGLRASTERPOS3DVPROC kglRasterPos3dv;
extern KGLRASTERPOS3FPROC kglRasterPos3f;
extern KGLRASTERPOS3FVPROC kglRasterPos3fv;
extern KGLRASTERPOS3IPROC kglRasterPos3i;
extern KGLRASTERPOS3IVPROC kglRasterPos3iv;
extern KGLRASTERPOS3SPROC kglRasterPos3s;
extern KGLRASTERPOS3SVPROC kglRasterPos3sv;
extern KGLRASTERPOS4DPROC kglRasterPos4d;
extern KGLRASTERPOS4DVPROC kglRasterPos4dv;
extern KGLRASTERPOS4FPROC kglRasterPos4f;
extern KGLRASTERPOS4FVPROC kglRasterPos4fv;
extern KGLRASTERPOS4IPROC kglRasterPos4i;
extern KGLRASTERPOS4IVPROC kglRasterPos4iv;
extern KGLRASTERPOS4SPROC kglRasterPos4s;
extern KGLRASTERPOS4SVPROC kglRasterPos4sv;
extern KGLREADBUFFERPROC kglReadBuffer;
extern KGLREADPIXELSPROC kglReadPixels;
extern KGLRECTDPROC kglRectd;
extern KGLRECTDVPROC kglRectdv;
extern KGLRECTFPROC kglRectf;
extern KGLRECTFVPROC kglRectfv;
extern KGLRECTIPROC kglRecti;
extern KGLRECTIVPROC kglRectiv;
extern KGLRECTSPROC kglRects;
extern KGLRECTSVPROC kglRectsv;
extern KGLRENDERMODEPROC kglRenderMode;
extern KGLROTATEDPROC kglRotated;
extern KGLROTATEFPROC kglRotatef;
extern KGLSCALEDPROC kglScaled;
extern KGLSCALEFPROC kglScalef;
extern KGLSCISSORPROC kglScissor;
extern KGLSELECTBUFFERPROC kglSelectBuffer;
extern KGLSHADEMODELPROC kglShadeModel;
extern KGLSTENCILFUNCPROC kglStencilFunc;
extern KGLSTENCILMASKPROC kglStencilMask;
extern KGLSTENCILOPPROC kglStencilOp;
extern KGLTEXCOORD1DPROC kglTexCoord1d;
extern KGLTEXCOORD1DVPROC kglTexCoord1dv;
extern KGLTEXCOORD1FPROC kglTexCoord1f;
extern KGLTEXCOORD1FVPROC kglTexCoord1fv;
extern KGLTEXCOORD1IPROC kglTexCoord1i;
extern KGLTEXCOORD1IVPROC kglTexCoord1iv;
extern KGLTEXCOORD1SPROC kglTexCoord1s;
extern KGLTEXCOORD1SVPROC kglTexCoord1sv;
extern KGLTEXCOORD2DPROC kglTexCoord2d;
extern KGLTEXCOORD2DVPROC kglTexCoord2dv;
extern KGLTEXCOORD2FPROC kglTexCoord2f;
extern KGLTEXCOORD2FVPROC kglTexCoord2fv;
extern KGLTEXCOORD2IPROC kglTexCoord2i;
extern KGLTEXCOORD2IVPROC kglTexCoord2iv;
extern KGLTEXCOORD2SPROC kglTexCoord2s;
extern KGLTEXCOORD2SVPROC kglTexCoord2sv;
extern KGLTEXCOORD3DPROC kglTexCoord3d;
extern KGLTEXCOORD3DVPROC kglTexCoord3dv;
extern KGLTEXCOORD3FPROC kglTexCoord3f;
extern KGLTEXCOORD3FVPROC kglTexCoord3fv;
extern KGLTEXCOORD3IPROC kglTexCoord3i;
extern KGLTEXCOORD3IVPROC kglTexCoord3iv;
extern KGLTEXCOORD3SPROC kglTexCoord3s;
extern KGLTEXCOORD3SVPROC kglTexCoord3sv;
extern KGLTEXCOORD4DPROC kglTexCoord4d;
extern KGLTEXCOORD4DVPROC kglTexCoord4dv;
extern KGLTEXCOORD4FPROC kglTexCoord4f;
extern KGLTEXCOORD4FVPROC kglTexCoord4fv;
extern KGLTEXCOORD4IPROC kglTexCoord4i;
extern KGLTEXCOORD4IVPROC kglTexCoord4iv;
extern KGLTEXCOORD4SPROC kglTexCoord4s;
extern KGLTEXCOORD4SVPROC kglTexCoord4sv;
extern KGLTEXCOORDPOINTERPROC kglTexCoordPointer;
extern KGLTEXENVFPROC kglTexEnvf;
extern KGLTEXENVFVPROC kglTexEnvfv;
extern KGLTEXENVIPROC kglTexEnvi;
extern KGLTEXENVIVPROC kglTexEnviv;
extern KGLTEXGENDPROC kglTexGend;
extern KGLTEXGENDVPROC kglTexGendv;
extern KGLTEXGENFPROC kglTexGenf;
extern KGLTEXGENFVPROC kglTexGenfv;
extern KGLTEXGENIPROC kglTexGeni;
extern KGLTEXGENIVPROC kglTexGeniv;
extern KGLTEXIMAGE1DPROC kglTexImage1D;
extern KGLTEXIMAGE2DPROC kglTexImage2D;
extern KGLTEXPARAMETERFPROC kglTexParameterf;
extern KGLTEXPARAMETERFVPROC kglTexParameterfv;
extern KGLTEXPARAMETERIPROC kglTexParameteri;
extern KGLTEXPARAMETERIVPROC kglTexParameteriv;
extern KGLTEXSUBIMAGE1DPROC kglTexSubImage1D;
extern KGLTEXSUBIMAGE2DPROC kglTexSubImage2D;
extern KGLTRANSLATEDPROC kglTranslated;
extern KGLTRANSLATEFPROC kglTranslatef;
extern KGLVERTEX2DPROC kglVertex2d;
extern KGLVERTEX2DVPROC kglVertex2dv;
extern KGLVERTEX2FPROC kglVertex2f;
extern KGLVERTEX2FVPROC kglVertex2fv;
extern KGLVERTEX2IPROC kglVertex2i;
extern KGLVERTEX2IVPROC kglVertex2iv;
extern KGLVERTEX2SPROC kglVertex2s;
extern KGLVERTEX2SVPROC kglVertex2sv;
extern KGLVERTEX3DPROC kglVertex3d;
extern KGLVERTEX3DVPROC kglVertex3dv;
extern KGLVERTEX3FPROC kglVertex3f;
extern KGLVERTEX3FVPROC kglVertex3fv;
extern KGLVERTEX3IPROC kglVertex3i;
extern KGLVERTEX3IVPROC kglVertex3iv;
extern KGLVERTEX3SPROC kglVertex3s;
extern KGLVERTEX3SVPROC kglVertex3sv;
extern KGLVERTEX4DPROC kglVertex4d;
extern KGLVERTEX4DVPROC kglVertex4dv;
extern KGLVERTEX4FPROC kglVertex4f;
extern KGLVERTEX4FVPROC kglVertex4fv;
extern KGLVERTEX4IPROC kglVertex4i;
extern KGLVERTEX4IVPROC kglVertex4iv;
extern KGLVERTEX4SPROC kglVertex4s;
extern KGLVERTEX4SVPROC kglVertex4sv;
extern KGLVERTEXPOINTERPROC kglVertexPointer;
extern KGLVIEWPORTPROC kglViewport;

typedef Bool (*KGLXQUERYEXTENSION)(Display *dpy, int *errorBase, int *eventBase);
typedef Bool (*KGLXQUERYVERSION)(Display *dpy, int *major, int *minor);
typedef int (*KGLXGETCONFIG)(Display *dpy, XVisualInfo *vis, int attrib, int *value);
typedef XVisualInfo* (*KGLXCHOOSEVISUAL)(Display *dpy, int screen, int *attribList);
typedef GLXContext (*KGLXCREATECONTEXT)(Display *dpy, XVisualInfo *vis, GLXContext shareList, Bool direct);
typedef void (*KGLXDESTROYCONTEXT)(Display *dpy, GLXContext ctx);
typedef Bool (*KGLXMAKECURRENT)(Display *dpy, GLXDrawable drawable, GLXContext ctx);
typedef void (*KGLXSWAPBUFFERS)(Display *dpy, GLXDrawable drawable);
typedef void* (*KGLXGETPROCADDRESSARB)(const GLubyte *procName);

extern KGLXQUERYEXTENSION kglXQueryExtension;
extern KGLXQUERYVERSION kglXQueryVersion;
extern KGLXGETCONFIG kglXGetConfig;
extern KGLXCHOOSEVISUAL kglXChooseVisual;
extern KGLXCREATECONTEXT kglXCreateContext;
extern KGLXDESTROYCONTEXT kglXDestroyContext;
extern KGLXMAKECURRENT kglXMakeCurrent;
extern KGLXSWAPBUFFERS kglXSwapBuffers;
extern KGLXGETPROCADDRESSARB kglXGetProcAddressARB;

#endif

