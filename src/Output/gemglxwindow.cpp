///////////////////////////////////////////////////////
//
// GEM - Graphics Environment for Multimedia
//
// zmoelnig@iem.kug.ac.at
//
// Implementation file
//
//    Copyright (c) 1997-2000 Mark Danks.
//    Copyright (c) G�nther Geiger.
//    Copyright (c) 2001-2011 IOhannes m zmoelnig. forum::f�r::uml�ute. IEM
//    For information on usage and redistribution, and for a DISCLAIMER OF ALL
//    WARRANTIES, see the file, "GEM.LICENSE.TERMS" in this distribution.
//
/////////////////////////////////////////////////////////
#include "Base/GemConfig.h"

#include "gemglxwindow.h"
#include "Base/GemGL.h"
#include <stdio.h>
#include <stdlib.h>


#ifdef HAVE_LIBXXF86VM
#  include <X11/extensions/xf86vmode.h>
#endif
#include <X11/cursorfont.h>

CPPEXTERN_NEW(gemglxwindow);

#define EVENT_MASK                                                      \
  ExposureMask|StructureNotifyMask|PointerMotionMask|ButtonMotionMask | \
  ButtonReleaseMask | ButtonPressMask | KeyPressMask | KeyReleaseMask | ResizeRedirectMask | DestroyNotify

// window creation variables
static int snglBuf24[] = {GLX_RGBA, 
                          GLX_RED_SIZE, 8, 
                          GLX_GREEN_SIZE, 8, 
                          GLX_BLUE_SIZE, 8, 
                          GLX_DEPTH_SIZE, 16, 
                          GLX_STENCIL_SIZE, 8, 
                          GLX_ACCUM_RED_SIZE, 8,
                          GLX_ACCUM_GREEN_SIZE, 8,
                          GLX_ACCUM_BLUE_SIZE, 8,
                          None};
static int snglBuf24Stereo[] = {GLX_RGBA, 
                                GLX_RED_SIZE, 8, 
                                GLX_GREEN_SIZE, 8, 
                                GLX_BLUE_SIZE, 8, 
                                GLX_DEPTH_SIZE, 16, 
                                GLX_STENCIL_SIZE, 8, 
                                GLX_ACCUM_RED_SIZE, 8,
                                GLX_ACCUM_GREEN_SIZE, 8,
                                GLX_ACCUM_BLUE_SIZE, 8,
                                GLX_STEREO,
                                None};
static int dblBuf24[] =  {GLX_RGBA, 
                          GLX_RED_SIZE, 4, 
                          GLX_GREEN_SIZE, 4, 
                          GLX_BLUE_SIZE, 4, 
                          GLX_DEPTH_SIZE, 16, 
                          GLX_STENCIL_SIZE, 8, 
                          GLX_ACCUM_RED_SIZE, 8,
                          GLX_ACCUM_GREEN_SIZE, 8,
                          GLX_ACCUM_BLUE_SIZE, 8,
                          GLX_DOUBLEBUFFER, 
                          None};
static int dblBuf24Stereo[] =  {GLX_RGBA, 
                                GLX_RED_SIZE, 4, 
                                GLX_GREEN_SIZE, 4, 
                                GLX_BLUE_SIZE, 4, 
                                GLX_DEPTH_SIZE, 16, 
                                GLX_STENCIL_SIZE, 8, 
                                GLX_ACCUM_RED_SIZE, 8,
                                GLX_ACCUM_GREEN_SIZE, 8,
                                GLX_ACCUM_BLUE_SIZE, 8,
                                GLX_DOUBLEBUFFER, 
                                GLX_STEREO,
                                None};
static int snglBuf8[] =  {GLX_RGBA, 
                          GLX_RED_SIZE, 3, 
                          GLX_GREEN_SIZE, 3, 
                          GLX_BLUE_SIZE, 2, 
                          GLX_DEPTH_SIZE, 16, 
                          None};
static int snglBuf8Stereo[] =  {GLX_RGBA, 
                                GLX_RED_SIZE, 3, 
                                GLX_GREEN_SIZE, 3, 
                                GLX_BLUE_SIZE, 2, 
                                GLX_DEPTH_SIZE, 16, 
                                GLX_STEREO,
                                None};
static int dblBuf8[] =   {GLX_RGBA, 
                          GLX_RED_SIZE, 1, 
                          GLX_GREEN_SIZE, 2, 
                          GLX_BLUE_SIZE, 1, 
                          GLX_DEPTH_SIZE, 16, 
                          GLX_DOUBLEBUFFER, 
                          None};

static int dblBuf8Stereo[] =   {GLX_RGBA, 
                                GLX_RED_SIZE, 1, 
                                GLX_GREEN_SIZE, 2, 
                                GLX_BLUE_SIZE, 1, 
                                GLX_DEPTH_SIZE, 16, 
                                GLX_DOUBLEBUFFER, 
                                GLX_STEREO,
                                None};

static int xerr;
static int ErrorHandler (Display *dpy, XErrorEvent *event)
{
  // we don't really care about the error
  // let's hope for the best
  if(event)
    xerr=event->error_code;  

  if ( event->error_code != BadWindow ) {
    char buf[256];
    XGetErrorText (dpy, event->error_code, buf, sizeof(buf));
    error("Xwin: %s\n", buf);
  } else
    error("Xwin: BadWindow (%d)\n", xerr);
  return (0);
}

static Bool WaitForNotify(Display *, XEvent *e, char *arg)
{
  return (e->type == MapNotify) && (e->xmap.window == (Window)arg);
}


 

struct gemglxwindow::Info {
  int         fs;                 // FullScreen
  bool        have_constContext;  // 1 if we have a constant context

  Display     *dpy;               // X Display
  Window      win;                // X Window
  int         screen;             // X Screen
  Colormap    cmap;               // X color map
  GLXContext  context;            // OpenGL context

#warning sharedContext in Info
  GLXContext  shared;// The GLXcontext to share rendering with

  Atom        delete_atom;
  
#ifdef HAVE_LIBXXF86VM
  XF86VidModeModeInfo deskMode; // originale ModeLine of the Desktop
#endif
  bool        have_border;

  bool doDispatch;


  Info(void) : 
    fs(0),
    have_constContext(false),
    dpy(NULL), 
    win(0), 
    cmap(0), 
    context(NULL), 
    shared(NULL),
    delete_atom(0),
#ifdef HAVE_LIBXXF86VM
    //    deskMode(0),
#endif
    have_border(false),
    doDispatch(false)
  {
  }
  ~Info(void) {
  }
};

/////////////////////////////////////////////////////////
//
// gemglxwindow
//
/////////////////////////////////////////////////////////
// Constructor
//
/////////////////////////////////////////////////////////
gemglxwindow :: gemglxwindow(void) :
  m_buffer(2),
  m_fsaa(0),
  m_title(std::string("GEM")),
  m_border(true),
  m_fullscreen(false),
  m_width(500), m_height(500),
  m_xoffset(0), m_yoffset(0),
  m_cursor(false),
  real_w(0), real_h(0), real_x(0), real_y(0),
  m_display(std::string("")),
  m_actuallyDisplay(true),
  m_info(new Info())
{
}

////////////////////////////////////////////////////////
// Destructor
//
/////////////////////////////////////////////////////////
gemglxwindow :: ~gemglxwindow()
{
  destroyMess();
}


bool gemglxwindow :: makeCurrent(void){
  if(!m_info->dpy || !m_info->win || !m_info->context)
    return false;

  xerr=0;
  glXMakeCurrent(m_info->dpy, m_info->win, m_info->context);
  if(xerr!=0) {
    return false;
  }
  return GemContext::makeCurrent();
}

/////////////////////////////////////////////////////////
// renderMess
//
/////////////////////////////////////////////////////////
void gemglxwindow :: renderMess()
{
  if(!makeCurrent()){ 
    error("no window made, cannot render!");
    return;
  }
  bang();
  if(m_buffer==2) {
    glXSwapBuffers(m_info->dpy, m_info->win);
  }
}

void gemglxwindow::dispatch(void) {
  if(!m_info->doDispatch)return;
  XEvent event; 
  XButtonEvent* eb = (XButtonEvent*)&event; 
  XKeyEvent* kb  = (XKeyEvent*)&event; 
  XResizeRequestEvent *res = (XResizeRequestEvent*)&event;
  char keystring[2];
  KeySym keysym_return;

  while (XCheckWindowEvent(m_info->dpy,m_info->win,
                           ResizeRedirectMask | 
                           KeyPressMask | KeyReleaseMask |
                           PointerMotionMask | 
                           ButtonMotionMask |
                           ButtonPressMask | 
                           ButtonReleaseMask,
                           &event))
    {
      switch (event.type)
        {
        case ButtonPress: 
          button(eb->button-1, 1);
          motion(eb->x, eb->y);
          break; 
        case ButtonRelease: 
          button(eb->button-1, 0);
          motion(eb->x, eb->y);
          break; 
        case MotionNotify: 
          motion(eb->x, eb->y);
          if(!m_info->have_border) {
            int err=XSetInputFocus(m_info->dpy, m_info->win, RevertToParent, CurrentTime);
            err=0;
          }
          break; 
        case KeyPress:
          if (XLookupString(kb,keystring,2,&keysym_return,NULL)==0) {
            //modifier key:use keysym
            //triggerKeyboardEvent(XKeysymToString(keysym_return), kb->keycode, 1);
          }
          if ( (keysym_return & 0xff00)== 0xff00 ) {
            //non alphanumeric key: use keysym
            key(XKeysymToString(keysym_return), kb->keycode, 1);
          } else {
            key(keystring                     , kb->keycode, 1);
          }
          break;
        case KeyRelease:
          if (XLookupString(kb,keystring,2,&keysym_return,NULL)==0) {
            //modifier key:use keysym
            //triggerKeyboardEvent(XKeysymToString(keysym_return), kb->keycode, 1);
          }
          if ( (keysym_return & 0xff00)== 0xff00 ) {
            //non alphanumeric key: use keysym
            key(XKeysymToString(keysym_return), kb->keycode, 0);
          } else {
            key(keystring                     , kb->keycode, 0);
          }
          break;

        case ResizeRequest:
          XResizeWindow(m_info->dpy, m_info->win, res->width, res->height);
          dimension(res->width, res->height);
          break;
        default:
          break; 
        }
    }
  
  if (XCheckTypedEvent(m_info->dpy,  ClientMessage, &event)) {
    info("window", "destroy");
    //    GemMan::destroyWindowSoon();
  }
}


/////////////////////////////////////////////////////////
// bufferMess
//
/////////////////////////////////////////////////////////
void gemglxwindow :: bufferMess(int buf)
{
  switch(buf) {
  case 1: case 2:
    m_buffer=buf;
    break;
  default:
    error("buffer can only be '1' (single) or '2' (double) buffered");
    break;
  }
}

/////////////////////////////////////////////////////////
// fsaaMess
//
/////////////////////////////////////////////////////////
void gemglxwindow :: fsaaMess(int value)
{
  m_fsaa=value;
}

/////////////////////////////////////////////////////////
// titleMess
//
/////////////////////////////////////////////////////////
void gemglxwindow :: titleMess(t_symbol* s)
{
  m_title=s->s_name;
}
/////////////////////////////////////////////////////////
// border
//
/////////////////////////////////////////////////////////
void gemglxwindow :: borderMess(bool setting)
{
  m_border=setting;
}
/////////////////////////////////////////////////////////
// dimensionsMess
//
/////////////////////////////////////////////////////////
void gemglxwindow :: dimensionsMess(int width, int height)
{
  if (width <= 0) {
    error("width must be greater than 0");
    return;
  }
    
  if (height <= 0 ) {
    error ("height must be greater than 0");
    return;
  }

  m_width=width;
  m_height=height;
}
/////////////////////////////////////////////////////////
// fullscreenMess
//
/////////////////////////////////////////////////////////
void gemglxwindow :: fullscreenMess(bool on)
{
  m_fullscreen = on;
}

/////////////////////////////////////////////////////////
// offsetMess
//
/////////////////////////////////////////////////////////
void gemglxwindow :: offsetMess(int x, int y)
{
  m_xoffset=x;
  m_yoffset=y;
}

/////////////////////////////////////////////////////////
// createMess
//
/////////////////////////////////////////////////////////
bool gemglxwindow :: create(void)
{
  int modeNum=4;
  int bestMode=0;
#ifdef HAVE_LIBXXF86VM
  XF86VidModeModeInfo **modes;
#endif
  int fullscreen=m_fullscreen;

  char svalue[3];
  snprintf(svalue, 3, "%d", m_fsaa);
  svalue[2]=0;
  if (m_fsaa!=0) setenv("__GL_FSAA_MODE", svalue, 1); // this works only for NVIDIA-cards

  XSetErrorHandler (ErrorHandler);

  if ( (m_info->dpy = XOpenDisplay(m_display.c_str())) == NULL) { 
    error("Could not open display %s",m_display.c_str());
    return false;
  }
  m_info->screen  = DefaultScreen(m_info->dpy);

  if ( !glXQueryExtension(m_info->dpy, NULL, NULL) ) {
    error("X server has no OpenGL GLX extension");
    return false;
  } 

  if (fullscreen){
    if (!m_display.empty()){
      error("fullscreen not available on remote display");
      fullscreen=false;
    } else {
#ifdef HAVE_LIBXXF86VM
      XF86VidModeGetAllModeLines(m_info->dpy, m_info->screen, &modeNum, &modes);
      m_info->deskMode = *modes[0];
#else
      error("no xxf86vm-support: cannot switch to fullscreen");
#endif
    }
  }
  XVisualInfo *vi;
  // the user wants double buffer
  if (m_buffer == 2) {
    // try for a double-buffered on 24bit machine (try stereo first)
    vi = glXChooseVisual(m_info->dpy, m_info->screen, dblBuf24Stereo);
    if (vi == NULL)
      vi = glXChooseVisual(m_info->dpy, m_info->screen, dblBuf24);
    if (vi == NULL) {
      // try for a double buffered on a 8bit machine (try stereo first)
      vi = glXChooseVisual(m_info->dpy, m_info->screen, dblBuf8Stereo);
      if(vi == NULL)
        vi = glXChooseVisual(m_info->dpy, m_info->screen, dblBuf8);
      if (vi == NULL) {
        error("Unable to create double buffer window");
        return false;
      }
      post("Only using 8 color bits");
    }
  }
  // the user wants single buffer
  else {
    // try for a single buffered on a 24bit machine (try stereo first)
    vi = glXChooseVisual(m_info->dpy, m_info->screen, snglBuf24Stereo);
    if (vi == NULL)
      vi = glXChooseVisual(m_info->dpy, m_info->screen, snglBuf24);
    if (vi == NULL) {
      // try for a single buffered on a 8bit machine (try stereo first)
      vi = glXChooseVisual(m_info->dpy, m_info->screen, snglBuf8Stereo);
      if (vi == NULL)
        vi = glXChooseVisual(m_info->dpy, m_info->screen, snglBuf8);
      if (vi == NULL) {
        error("Unable to create single buffer window");
        return false;
      }
      post("Only using 8 color bits");
    }
    m_buffer = 1;
  }

  if (vi->c_class != TrueColor && vi->c_class != DirectColor) {
    error("TrueColor visual required for this program (got %d)", vi->c_class);
    return false;
  }
  // create the rendering context
  try {
    m_info->context = glXCreateContext(m_info->dpy, vi, m_info->shared, GL_TRUE);
  } catch(void*e){
    m_info->context=NULL;
  }
  if (m_info->context == NULL) {
    error("Could not create rendering context");
    return false;
  }
  // create the X color map
  m_info->cmap = XCreateColormap(m_info->dpy, RootWindow(m_info->dpy, vi->screen), 
                                 vi->visual, AllocNone);
  if (!m_info->cmap) {
    error("Could not create X colormap");
    return false;
  }

  XSetWindowAttributes swa;
  swa.colormap = m_info->cmap;
  swa.border_pixel = 0;
  // event_mask creates signal that window has been created
  swa.event_mask = EVENT_MASK;

  real_w=m_width;
  real_h=m_height;

  real_x=m_xoffset;
  real_y=m_yoffset;

  int flags;
#ifdef HAVE_LIBXXF86VM
  if (fullscreen){
    /* look for mode with requested resolution */
    for (int i = 0; i < modeNum; i++) {
      if ((modes[i]->hdisplay == m_width) && (modes[i]->vdisplay == m_height)) {
        bestMode = i;
      }
    }
    
    XF86VidModeSwitchToMode(m_info->dpy, m_info->screen, modes[bestMode]);
    XF86VidModeSetViewPort(m_info->dpy, m_info->screen, 0, 0);
    real_w = modes[bestMode]->hdisplay;
    real_h = modes[bestMode]->vdisplay;
    real_x=real_y=0;
    XFree(modes);

    swa.override_redirect = True;
    flags=CWBorderPixel|CWColormap|CWEventMask|CWOverrideRedirect;
  } else
#endif
    { // !fullscren
      if (m_border){
        swa.override_redirect = False;
        flags=CWBorderPixel|CWColormap|CWEventMask|CWOverrideRedirect;
      } else {
        swa.override_redirect = True;
        flags=CWBorderPixel|CWColormap|CWEventMask|CWOverrideRedirect;
      }
    }
  m_info->fs = fullscreen;

  m_info->win = XCreateWindow(m_info->dpy, RootWindow(m_info->dpy, vi->screen),
                              m_xoffset, m_yoffset, real_w, real_h,
                              0, vi->depth, InputOutput, 
                              vi->visual, flags, &swa);
  if (!m_info->win) {
    error("Could not create X window");
    return false;
  }

  m_info->have_border=(True==swa.override_redirect);

  XSelectInput(m_info->dpy, m_info->win, EVENT_MASK);

  /* found a bit at
   * http://biology.ncsa.uiuc.edu/library/SGI_bookshelves/SGI_Developer/books/OpenGL_Porting/sgi_html/apf.html
   * LATER think about reacting on this event...
   */
  m_info->delete_atom = XInternAtom(m_info->dpy, "WM_DELETE_WINDOW", True);
  if (m_info->delete_atom != None)
    XSetWMProtocols(m_info->dpy, m_info->win, &m_info->delete_atom,1);

  XSetStandardProperties(m_info->dpy, m_info->win,
                         m_title.c_str(), "gem", 
                         None, 0, 0, NULL);

  try{
    xerr=0;
    glXMakeCurrent(m_info->dpy, m_info->win, m_info->context);

    if(xerr!=0) {
      /* seems like the error-handler was called; so something did not work the way it should
       * should we really prevent window-creation in this case?
       * LATER re-think the entire dual-context thing
       */

      error("problems making glX-context current: refusing to continue");
      error("try setting the environment variable GEM_SINGLE_CONTEXT=1");
      return false;
    }
  }catch(void*e){
    error("Could not make glX-context current");
    return false;
  }

  if (m_actuallyDisplay) {
    XMapRaised(m_info->dpy, m_info->win);
    //  XMapWindow(m_info->dpy, m_info->win);
    XEvent report;
    XIfEvent(m_info->dpy, &report, WaitForNotify, (char*)m_info->win);
    if (glXIsDirect(m_info->dpy, m_info->context))
      post("Direct Rendering enabled!");
  }
  return GemContext::create();
}
void gemglxwindow :: createMess(void)
{
  if(!create()) {
    destroyMess();
    return;
  }
  m_info->doDispatch=true;
}
/////////////////////////////////////////////////////////
// destroy window
//
/////////////////////////////////////////////////////////
void gemglxwindow :: destroy(void)
{
  /* both glXMakeCurrent() and XCloseDisplay() will crash the application
   * if the handler of the display (m_info->dpy) is invalid, e.g. because
   * somebody closed the Gem-window with xkill or by clicking on the "x" of the window
   */
  if (m_info->dpy) {
    int err=0;
    /* patch by cesare marilungo to prevent the crash "on my laptop" */
    glXMakeCurrent(m_info->dpy, None, NULL); /* this crashes if no window is there! */
    
    if (m_info->win)
      err=XDestroyWindow(m_info->dpy, m_info->win);
    if (m_info->have_constContext && m_info->context) {
      // this crashes sometimes on my laptop:
      glXDestroyContext(m_info->dpy, m_info->context);
    }
    if (m_info->cmap)
      err=XFreeColormap(m_info->dpy, m_info->cmap);
    
#ifdef HAVE_LIBXXF86VM
    if (m_info->fs){
      XF86VidModeSwitchToMode(m_info->dpy, m_info->screen, &m_info->deskMode);
      XF86VidModeSetViewPort(m_info->dpy, m_info->screen, 0, 0);
      m_info->fs=0;
    }
#endif
    
    err=XCloseDisplay(m_info->dpy); /* this crashes if no window is there */
  }
  m_info->dpy = NULL;
  m_info->win = 0;
  m_info->cmap = 0;
  m_info->context = NULL;
  if(m_info->delete_atom)m_info->delete_atom=None; /* not very sophisticated destruction...*/
  
  GemContext::destroy();
}
void gemglxwindow :: destroyMess(void)
{
  m_info->doDispatch=false;
  if(makeCurrent()) {
    destroy();
  } else {
    error("unable to destroy current window");
  }
}


/////////////////////////////////////////////////////////
// cursorMess
//
/////////////////////////////////////////////////////////
void gemglxwindow :: cursorMess(bool setting)
{

}
/////////////////////////////////////////////////////////
// static member function
//
/////////////////////////////////////////////////////////
void gemglxwindow :: obj_setupCallback(t_class *classPtr)
{
  class_addbang(classPtr, reinterpret_cast<t_method>(&gemglxwindow::renderMessCallback));
  
  class_addmethod(classPtr, reinterpret_cast<t_method>(&gemglxwindow::titleMessCallback),        gensym("title"), A_DEFSYM ,A_NULL);
  class_addmethod(classPtr, reinterpret_cast<t_method>(&gemglxwindow::createMessCallback),       gensym("create") ,A_NULL);
  class_addmethod(classPtr, reinterpret_cast<t_method>(&gemglxwindow::bufferMessCallback),       gensym("buffer"), A_FLOAT, A_NULL);
  class_addmethod(classPtr, reinterpret_cast<t_method>(&gemglxwindow::fullscreenMessCallback),   gensym("fullscreen"), A_FLOAT, A_NULL);
  class_addmethod(classPtr, reinterpret_cast<t_method>(&gemglxwindow::dimensionsMessCallback),   gensym("dimen"), A_FLOAT, A_FLOAT, A_NULL);
  class_addmethod(classPtr, reinterpret_cast<t_method>(&gemglxwindow::offsetMessCallback),       gensym("offset"), A_FLOAT, A_FLOAT, A_NULL);
  class_addmethod(classPtr, reinterpret_cast<t_method>(&gemglxwindow::cursorMessCallback),       gensym("cursor"), A_FLOAT, A_NULL);
  class_addmethod(classPtr, reinterpret_cast<t_method>(&gemglxwindow::destroyMessCallback),      gensym("destroy"), A_NULL);
  class_addmethod(classPtr, reinterpret_cast<t_method>(&gemglxwindow::printMessCallback),        gensym("print"), A_NULL);
  class_addmethod(classPtr, reinterpret_cast<t_method>(&gemglxwindow::borderMessCallback),       gensym("border"), A_FLOAT, A_NULL);
  class_addmethod(classPtr, reinterpret_cast<t_method>(&gemglxwindow::fsaaMessCallback),         gensym("FSAA"), A_FLOAT, A_NULL);
}
void gemglxwindow :: printMessCallback(void *)
{
  //  GemMan::printInfo();
}
void gemglxwindow :: borderMessCallback(void *data, t_floatarg state)
{
  GetMyClass(data)->borderMess(static_cast<int>(state));
}
void gemglxwindow :: destroyMessCallback(void *data)
{
  GetMyClass(data)->destroyMess();
}
void gemglxwindow :: renderMessCallback(void *data)
{
  GetMyClass(data)->renderMess();
}
void gemglxwindow :: titleMessCallback(void *data, t_symbol* disp)
{
  GetMyClass(data)->titleMess(disp);
}
void gemglxwindow :: createMessCallback(void *data)
{
  GetMyClass(data)->createMess();
}
void gemglxwindow :: bufferMessCallback(void *data, t_floatarg buf)
{
  GetMyClass(data)->bufferMess(static_cast<int>(buf));
}
void gemglxwindow :: fullscreenMessCallback(void *data, t_floatarg on)
{
  GetMyClass(data)->fullscreenMess(static_cast<int>(on));
}
void gemglxwindow :: dimensionsMessCallback(void *data, t_floatarg width, t_floatarg height)
{
  GetMyClass(data)->dimensionsMess(static_cast<int>(width), static_cast<int>(height));
}
void gemglxwindow :: offsetMessCallback(void *data, t_floatarg x, t_floatarg y)
{
  GetMyClass(data)->offsetMess(static_cast<int>(x), static_cast<int>(y));
}
void gemglxwindow :: cursorMessCallback(void *data, t_floatarg val)
{
  GetMyClass(data)->cursorMess(val);
}
void gemglxwindow :: fsaaMessCallback(void *data, t_floatarg val)
{
  GetMyClass(data)->fsaaMess(static_cast<int>(val));
}