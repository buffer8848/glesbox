//
//  egl.m
//  MinimalIOSApp
//
//  Created by alco on 24.05.12.
//  Copyright (c) 2012 __MyCompanyName__. All rights reserved.
//

#import <EGL/egl.h>
#import <OpenGLES/EAGLDrawable.h>
#import <OpenGLES/ES2/glext.h>
#import <QuartzCore/QuartzCore.h>

#import <UIKit/UIKit.h>

#define STRINGIFY(x) #x


static const EGLint INTERNAL_AEGL_VERSION_MAJOR = 1;
static const EGLint INTERNAL_AEGL_VERSION_MINOR = 4;

static const int INTERNAL_AEGL_DISPLAY_SECRET = 0xf00d;


typedef struct internalAEGLContext_t AEGLContext;

// This is a dummy object with a single field that is used
// to check that the valid display object is passed to EGL functions.
typedef struct internalAEGLDisplay_t {
  int secret_number;  // equal to INTERNAL_AEGL_DISPLAY for a valid display
  AEGLContext *context;
} AEGLDisplay;

static AEGLDisplay s_defaultDisplay = { INTERNAL_AEGL_DISPLAY_SECRET, NULL };

typedef struct internalAEGLConfig_t {
  EGLint config_id;
  
  EGLint red_size;
  EGLint green_size;
  EGLint blue_size;
  EGLint alpha_size;
  EGLint buffer_size;
  
  EGLint depth_size;
  
  EGLint render_type_bitmask;
} AEGLConfig;

typedef struct internalAEGLPlatformConfig_t {
  EAGLRenderingAPI api_version;
  NSString        *color_format;
  BOOL             retained_backing;
  GLuint           depth_component;
} AEGLPlatformConfig;

// Enumerate all possible configs. The following table shows the
// available settings and their variations:
//
// +------+--------------+-------------------+-----------------+
// | API  | Color format | Depth-buffer bits | Retains backing |
// +======+==============+===================+=================+
// | ES 1 |    RGB565    |         0         |       no        |
// | ES 2 |    RGBA8     |         16        |       yes       |
// |      |              |         24        |                 |
// +------+--------------+-------------------+-----------------+
//
// This gives us a total of 24 possible configs.
static AEGLConfig s_configs[] = {
  {  0,   5, 6, 5, 0,  16,   0,   EGL_OPENGL_ES_BIT | EGL_OPENGL_ES2_BIT },
  {  1,   5, 6, 5, 0,  16,   16,  EGL_OPENGL_ES_BIT | EGL_OPENGL_ES2_BIT },
  {  2,   5, 6, 5, 0,  16,   24,  EGL_OPENGL_ES_BIT | EGL_OPENGL_ES2_BIT },
  {  3,   8, 8, 8, 8,  32,   0,   EGL_OPENGL_ES_BIT | EGL_OPENGL_ES2_BIT },
  {  4,   8, 8, 8, 8,  32,   16,  EGL_OPENGL_ES_BIT | EGL_OPENGL_ES2_BIT },
  {  5,   8, 8, 8, 8,  32,   24,  EGL_OPENGL_ES_BIT | EGL_OPENGL_ES2_BIT },
};

static AEGLConfig *s_defaultES1_config = &s_configs[1];
static AEGLConfig *s_defaultES2_config = &s_configs[4];
static const int INTERNAL_AEGL_NUM_CONFIGS = sizeof(s_configs) / sizeof(AEGLConfig);

typedef struct internalAEGLSurface_t {
  UIView      *glView;
  AEGLContext *context;
  NSUInteger   renderBufferTarget;
  GLuint       framebuffer;
  GLuint       colorRenderbuffer;
  GLuint       depthRenderbuffer;
  BOOL         retained_backing;
} AEGLSurface;

struct internalAEGLContext_t {
  EAGLContext *context;
  AEGLConfig  *config;
  EGLint       api_version;
};

static EGLint s_error = EGL_SUCCESS;


#define CHECK_DISPLAY(display)                                                     \
if (((AEGLDisplay *)display)->secret_number != INTERNAL_AEGL_DISPLAY_SECRET) { \
s_error = EGL_BAD_DISPLAY;                                                 \
return EGL_FALSE;                                                          \
}

#define CHECK_DISPLAY_2(display, retval)                                           \
if (((AEGLDisplay *)display)->secret_number != INTERNAL_AEGL_DISPLAY_SECRET) { \
s_error = EGL_BAD_DISPLAY;                                                 \
return retval;                                                             \
}

#define GET_CONFIG(config, field) ((AEGLConfig *)config)->field


#pragma mark -

_Bool _setupFrameBuffer(AEGLSurface *surface, AEGLContext *context);
_Bool _configToPlatform(AEGLConfig *config, AEGLPlatformConfig *out_config);

#pragma mark

EGLint eglGetError(void)
{
  EGLint retval = s_error;
  s_error = EGL_SUCCESS;
  return retval;
}

EGLDisplay eglGetDisplay(EGLNativeDisplayType display_id)
{
  if (display_id != EGL_DEFAULT_DISPLAY) {
    return EGL_NO_DISPLAY;
  }
  return &s_defaultDisplay;
}

EGLBoolean eglInitialize(EGLDisplay dpy, EGLint *major, EGLint *minor)
{
  CHECK_DISPLAY(dpy);
  
  if (major) {
    *major = INTERNAL_AEGL_VERSION_MAJOR;
  }
  if (minor) {
    *minor = INTERNAL_AEGL_VERSION_MINOR;
  }
  
  // No further work needed
  
  return EGL_TRUE;
}

EGLBoolean eglTerminate(EGLDisplay dpy)
{
  CHECK_DISPLAY(dpy);
  
  // Do nothing
  
  return EGL_TRUE;
}

const char *eglQueryString(EGLDisplay dpy, EGLint name)
{
  CHECK_DISPLAY_2(dpy, NULL);
  
  switch (name) {
    case EGL_CLIENT_APIS:
      // Returns a string describing which client rendering APIs are
      // supported. The string contains a space-separate list of API names.
      // The list must include at least one of OpenGL, OpenGL_ES, or OpenVG.
      // These strings correspond respectively to values EGL_OPENGL_API,
      // EGL_OPENGL_ES_API, and EGL_OPENVG_API of the eglBindAPI, api
      // argument.
      return "OpenGL_ES";
      
    case EGL_VENDOR:
      // Returns the company responsible for this EGL implementation. This
      // name does not change from release to release.
      return "alco";
      
    case EGL_VERSION:
      // Returns a version or release number. The EGL_VERSION string is laid
      // out as follows:
      //
      // major_version.minor_version space vendor_specific_info
      return STRINGIFY(INTERNAL_AEGL_VERSION_MAJOR) "." STRINGIFY(INTERNAL_AEGL_VERSION_MINOR) " alpha";
      
    case EGL_EXTENSIONS:
      // Returns a space separated list of supported extensions to EGL.
      return "";
      
    default:
      s_error = EGL_BAD_PARAMETER;
      return NULL;
  }
  
  return NULL;
}

EGLBoolean eglGetConfigs(EGLDisplay dpy, EGLConfig *configs,
                         EGLint config_size, EGLint *num_config)
{
  CHECK_DISPLAY(dpy);
  
  if (!num_config) {
    s_error = EGL_BAD_PARAMETER;
    return EGL_FALSE;
  }
  
  if (!configs) {
    // Return the total number of supported configs.
    *num_config = INTERNAL_AEGL_NUM_CONFIGS;
    return EGL_TRUE;
  }
  
  if (config_size == 1) {
    // Return only OpenGL ES 1 for compatibility with old devices
    configs[0] = s_defaultES1_config;
    *num_config = 1;
  } else if (config_size == 2) {
    // Return both OpenGL ES 1 and OpenGL ES 2
    configs[0] = &s_defaultES1_config;
    configs[1] = &s_defaultES2_config;
    *num_config = 2;
  } else {
    int size = MIN(config_size, INTERNAL_AEGL_NUM_CONFIGS);
    for (int i = 0; i < size; ++i) {
      configs[i] = &s_configs[i];
    }
    *num_config = size;
  }
  
  return EGL_TRUE;
}


EGLBoolean eglChooseConfig(EGLDisplay dpy, const EGLint *attrib_list,
                           EGLConfig *configs, EGLint config_size,
                           EGLint *num_config)
{
  CHECK_DISPLAY(dpy);
  
  if (!configs) {
    // Return the total number of configs matching the attributes.
    // TODO: ...
    return EGL_TRUE;
  }
  
  if (attrib_list) {
    for (unsigned i = 0; attrib_list[i] != EGL_NONE; i += 2) {
        if (attrib_list[i] == EGL_RENDERABLE_TYPE) {
            if (attrib_list[i+1] == EGL_OPENGL_ES_BIT) {
              *configs = s_defaultES1_config;
            } else if (attrib_list[i+1] == EGL_OPENGL_ES2_BIT) {
              *configs = s_defaultES2_config;
            }
        }
    }
  }
  
  return EGL_TRUE;
}

EGLBoolean  eglGetConfigAttrib(EGLDisplay dpy, EGLConfig config,
                               EGLint attribute, EGLint *value)
{
  CHECK_DISPLAY(dpy);
  
  if (!value) {
    return EGL_FALSE;
  }
  
  switch (attribute) {
    case EGL_ALPHA_SIZE:
      // Returns the number of bits of alpha stored in the color buffer.
      *value = GET_CONFIG(config, alpha_size);
      break;
      
    case EGL_ALPHA_MASK_SIZE:
      // Returns the number of bits in the alpha mask buffer.
      *value = 0;
      break;
      
    case EGL_BIND_TO_TEXTURE_RGB:
      // Returns EGL_TRUE if color buffers can be bound to an RGB texture,
      // EGL_FALSE otherwise.
      *value = EGL_TRUE;
      break;
      
    case EGL_BIND_TO_TEXTURE_RGBA:
      // Returns EGL_TRUE if color buffers can be bound to an RGBA texture,
      // EGL_FALSE otherwise.
      *value = EGL_TRUE;
      break;
      
    case EGL_BLUE_SIZE:
      // Returns the number of bits of blue stored in the color buffer.
      *value = GET_CONFIG(config, blue_size);
      break;
      
    case EGL_BUFFER_SIZE:
      // Returns the depth of the color buffer. It is the sum of
      // EGL_RED_SIZE, EGL_GREEN_SIZE, EGL_BLUE_SIZE, and EGL_ALPHA_SIZE.
      *value = GET_CONFIG(config, buffer_size);
      break;
      
    case EGL_COLOR_BUFFER_TYPE:
      //Returns the color buffer type. Possible types are EGL_RGB_BUFFER and
      //EGL_LUMINANCE_BUFFER.
      *value = EGL_RGB_BUFFER;
      break;
      
    case EGL_CONFIG_CAVEAT:
      // Returns the caveats for the frame buffer configuration. Possible
      // caveat values are EGL_NONE, EGL_SLOW_CONFIG, and EGL_NON_CONFORMANT.
      *value = EGL_NONE;
      break;
      
    case EGL_CONFIG_ID:
      // Returns the ID of the frame buffer configuration.
      *value = GET_CONFIG(config, config_id);
      break;
      
    case EGL_CONFORMANT:
      // Returns a bitmask indicating which client API contexts created with
      // respect to this config are conformant.
      *value = GET_CONFIG(config, render_type_bitmask);
      break;
      
    case EGL_DEPTH_SIZE:
      // Returns the number of bits in the depth buffer.
      *value = GET_CONFIG(config, depth_size);
      break;
      
    case EGL_GREEN_SIZE:
      // Returns the number of bits of green stored in the color buffer.
      *value = GET_CONFIG(config, green_size);
      break;
      
    case EGL_LEVEL:
      // Returns the frame buffer level. Level zero is the default frame
      // buffer. Positive levels correspond to frame buffers that overlay the
      // default buffer and negative levels correspond to frame buffers that
      // underlay the default buffer.
      *value = 0;
      break;
      
    case EGL_LUMINANCE_SIZE:
      // Returns the number of bits of luminance stored in the luminance
      // buffer.
      *value = 0;
      break;
      
    case EGL_MAX_PBUFFER_WIDTH:
      // Returns the maximum width of a pixel buffer surface in pixels.
      *value = 0;
      break;
      
    case EGL_MAX_PBUFFER_HEIGHT:
      // Returns the maximum height of a pixel buffer surface in pixels.
      *value = 0;
      break;
      
    case EGL_MAX_PBUFFER_PIXELS:
      // Returns the maximum size of a pixel buffer surface in pixels.
      *value = 0;
      break;
      
    case EGL_MAX_SWAP_INTERVAL:
      // Returns the maximum value that can be passed to eglSwapInterval.
      *value = 10;
      break;
      
    case EGL_MIN_SWAP_INTERVAL:
      // Returns the minimum value that can be passed to eglSwapInterval.
      *value = 0;
      break;
      
    case EGL_NATIVE_RENDERABLE:
      // Returns EGL_TRUE if native rendering APIs can render into the
      // surface, EGL_FALSE otherwise.
      *value = EGL_TRUE;
      break;
      
    case EGL_NATIVE_VISUAL_ID:
      // Returns the ID of the associated native visual.
      *value = 0;
      break;
      
    case EGL_NATIVE_VISUAL_TYPE:
      // Returns the type of the associated native visual.
      *value = 0;
      break;
      
    case EGL_RED_SIZE:
      // Returns the number of bits of red stored in the color buffer.
      *value = GET_CONFIG(config, red_size);
      break;
      
    case EGL_RENDERABLE_TYPE:
      // Returns a bitmask indicating the types of supported client API
      // contexts.
      *value = GET_CONFIG(config, render_type_bitmask);
      break;
      
    case EGL_SAMPLE_BUFFERS:
      // Returns the number of multisample buffers.
      *value = 0;
      break;
      
    case EGL_SAMPLES:
      // Returns the number of samples per pixel.
      *value = 0;
      break;
      
    case EGL_STENCIL_SIZE:
      // Returns the number of bits in the stencil buffer.
      *value = 0;
      break;
      
    case EGL_SURFACE_TYPE:
      // Returns a bitmask indicating the types of supported EGL surfaces.
      *value = 0;
      break;
      
    case EGL_TRANSPARENT_TYPE:
      // Returns the type of supported transparency. Possible transparency
      // values are: EGL_NONE, and EGL_TRANSPARENT_RGB.
      *value = 0;
      break;
      
    case EGL_TRANSPARENT_RED_VALUE:
      // Returns the transparent red value.
      *value = 0;
      break;
      
    case EGL_TRANSPARENT_GREEN_VALUE:
      // Returns the transparent green value.
      *value = 0;
      break;
      
    case EGL_TRANSPARENT_BLUE_VALUE:
      // Returns the transparent blue value.
      *value = 0;
      break;
      
    default:
      s_error = EGL_BAD_ATTRIBUTE;
      return EGL_FALSE;
  }
  
  return EGL_TRUE;
}

EGLSurface eglCreateWindowSurface(EGLDisplay dpy, EGLConfig config,
                                  EGLNativeWindowType win,
                                  const EGLint *attrib_list)
{
  CHECK_DISPLAY(dpy);
  
  AEGLPlatformConfig cfg;
  _configToPlatform(config, &cfg);
  
  // TODO: handle the attrib_list
  
  UIView *glView = (UIView *)win;
  CAEAGLLayer *layer = (CAEAGLLayer *)glView.layer;
  [layer setDrawableProperties:
  [NSDictionary dictionaryWithObjectsAndKeys:
   [NSNumber numberWithBool:cfg.retained_backing], kEAGLDrawablePropertyRetainedBacking,
   cfg.color_format,                               kEAGLDrawablePropertyColorFormat,
   nil]];
  
  AEGLSurface *surface = malloc(sizeof(AEGLSurface));
  surface->glView = glView;
  surface->context = NULL;
  surface->framebuffer = 0;
  surface->colorRenderbuffer = 0;
  surface->depthRenderbuffer = 0;
  surface->retained_backing = cfg.retained_backing;
  
  return surface;
}

EGLSurface eglCreatePbufferSurface(EGLDisplay dpy, EGLConfig config,
                                   const EGLint *attrib_list)
{
  CHECK_DISPLAY(dpy);
  
  // FIXME: Not implemented
  
  return NULL;
}

EGLSurface eglCreatePixmapSurface(EGLDisplay dpy, EGLConfig config,
                                  EGLNativePixmapType pixmap,
                                  const EGLint *attrib_list)
{
  CHECK_DISPLAY(dpy);
  
  // FIXME: Not implemented
  
  return NULL;
}

EGLBoolean eglDestroySurface(EGLDisplay dpy, EGLSurface sfc)
{
  CHECK_DISPLAY(dpy);
  
  // TODO:
  // If the EGL surface surface is not current to any thread,
  // eglDestroySurface destroys it immediately. Otherwise, surface is
  // destroyed when it becomes not current to any thread.
  
  if (!sfc) {
    s_error = EGL_BAD_SURFACE;
    return EGL_FALSE;
  }
  
  EAGLContext *prevContext = nil;
  AEGLSurface *surface = (AEGLSurface *)sfc;
  if (surface->context) {
    prevContext = [EAGLContext currentContext];
    [EAGLContext setCurrentContext:surface->context->context];
  }
  if (surface->depthRenderbuffer) {
    glDeleteRenderbuffers(1, &surface->depthRenderbuffer);
  }
  if (surface->colorRenderbuffer) {
    glDeleteRenderbuffers(1, &surface->colorRenderbuffer);
  }
  if (surface->framebuffer) {
    glDeleteFramebuffers(1, &surface->framebuffer);
  }
  free(surface);
  
  if (prevContext) {
    [EAGLContext setCurrentContext:prevContext];
  }
  
  return EGL_TRUE;
}

EGLBoolean eglQuerySurface(EGLDisplay dpy, EGLSurface sfc,
                           EGLint attribute, EGLint *value)
{
  CHECK_DISPLAY(dpy);
  
  if (!value) {
    return EGL_FALSE;
  }
  
  if (!sfc) {
    s_error = EGL_BAD_SURFACE;
    return EGL_FALSE;
  }
  
  AEGLSurface *surface = (AEGLSurface *)sfc;
  
  switch (attribute) {
    case EGL_CONFIG_ID:
      // Returns the ID of the EGL frame buffer configuration with
      // respect to which the surface was created.
      *value = 0;  // FIXME
      break;
      
    case EGL_HEIGHT:
      // Returns the height of the surface in pixels.
      *value = (GLint)surface->glView.layer.bounds.size.height;
      break;
      
    case EGL_HORIZONTAL_RESOLUTION:
      // Returns the horizontal dot pitch of the display on which a
      // window surface is visible. The value returned is equal to the
      // actual dot pitch, in pixels/meter, multiplied by the constant
      // value EGL_DISPLAY_SCALING.
      *value = 0;  // FIXME
      break;
      
    case EGL_LARGEST_PBUFFER:
      // Returns the same attribute value specified when the surface was
      // created with eglCreatePbufferSurface. For a window or pixmap
      // surface, value is not modified.
      ;  // FIXME
      break;
      
    case EGL_MIPMAP_LEVEL:
      // Returns which level of the mipmap to render to, if texture has
      // mipmaps.
      ;  // FIXME
      break;
      
    case EGL_MIPMAP_TEXTURE:
      // Returns EGL_TRUE if texture has mipmaps, EGL_FALSE otherwise.
      *value = EGL_FALSE;
      break;
      
    case EGL_MULTISAMPLE_RESOLVE:
      // Returns the filter used when resolving the multisample buffer.
      // The filter may be either EGL_MULTISAMPLE_RESOLVE_DEFAULT or
      // EGL_MULTISAMPLE_RESOLVE_BOX, as described for eglSurfaceAttrib.
      ;  // FIXME
      break;
      
    case EGL_PIXEL_ASPECT_RATIO:
      // Returns the aspect ratio of an individual pixel (the ratio of a
      // pixel's width to its height). The value returned is equal to the
      // actual aspect ratio multiplied by the constant value
      // EGL_DISPLAY_SCALING.
      ;  // FIXME
      break;
      
    case EGL_RENDER_BUFFER:
      // Returns the buffer which client API rendering is requested to
      // use. For a window surface, this is the same attribute value
      // specified when the surface was created. For a pbuffer surface,
      // it is always EGL_BACK_BUFFER. For a pixmap surface, it is always
      // EGL_SINGLE_BUFFER. To determine the actual buffer being rendered
      // to by a context, call eglQueryContext.
      *value = 0;  // FIXME
      break;
      
    case EGL_SWAP_BEHAVIOR:
      // Returns the effect on the color buffer when posting a surface
      // with eglSwapBuffers. Swap behavior may be either
      // EGL_BUFFER_PRESERVED or EGL_BUFFER_DESTROYED, as described for
      // eglSurfaceAttrib.
      *value = 0;  // FIXME
      break;
      
    case EGL_TEXTURE_FORMAT:
      // Returns format of texture. Possible values are EGL_NO_TEXTURE,
      // EGL_TEXTURE_RGB, and EGL_TEXTURE_RGBA.
      *value = 0;  // FIXME
      break;
      
    case EGL_TEXTURE_TARGET:
      // Returns type of texture. Possible values are EGL_NO_TEXTURE, or
      // EGL_TEXTURE_2D.
      *value = 0;  // FIXME
      break;
      
    case EGL_VERTICAL_RESOLUTION:
      // Returns the vertical dot pitch of the display on which a window
      // surface is visible. The value returned is equal to the actual
      // dot pitch, in pixels/meter, multiplied by the constant value
      // EGL_DISPLAY_SCALING.
      *value = 0;  // FIXME
      break;
      
    case EGL_WIDTH:
      // Returns the width of the surface in pixels.
      *value = (GLint)surface->glView.layer.bounds.size.width;
      break;
      
    default:
      s_error = EGL_BAD_ATTRIBUTE;
      return EGL_FALSE;
  }
  
  return EGL_TRUE;
}


EGLBoolean eglBindAPI(EGLenum api)
{
  return EGL_FALSE;
}

EGLenum eglQueryAPI()
{
  return EGL_NONE;
}

EGLBoolean eglWaitClient(void)
{
  return EGL_FALSE;
}

EGLBoolean eglReleaseThread(void)
{
  return EGL_FALSE;
}

EGLSurface eglCreatePbufferFromClientBuffer(EGLDisplay dpy, EGLenum buftype, EGLClientBuffer buffer,
                                            EGLConfig config, const EGLint *attrib_list)
{
  CHECK_DISPLAY(dpy);
  
  // FIXME: not implemented
  
  return EGL_FALSE;
}

EGLBoolean eglSurfaceAttrib(EGLDisplay dpy, EGLSurface sfc,
                            EGLint attribute, EGLint value)
{
  CHECK_DISPLAY(dpy);
  
  if (!sfc) {
    s_error = EGL_BAD_SURFACE;
    return EGL_FALSE;
  }
  
  AEGLSurface *surface = (AEGLSurface *)sfc;
  
  switch (attribute) {
    case EGL_MIPMAP_LEVEL:
      // For mipmap textures, the EGL_MIPMAP_LEVEL attribute indicates
      // which level of the mipmap should be rendered. If the value of
      // this attribute is outside the range of supported mipmap levels,
      // the closest valid mipmap level is selected for rendering. The
      // default value is 0.
      
      /* not implemented */
      s_error = EGL_BAD_ATTRIBUTE;
      return EGL_FALSE;
      break;
      
    case EGL_MULTISAMPLE_RESOLVE:
      // Specifies the filter to use when resolving the multisample
      // buffer (this may occur when swapping or copying the surface, or
      // when changing the client API context bound to the surface). A
      // value of EGL_MULTISAMPLE_RESOLVE_DEFAULT chooses the default
      // implementation-defined filtering method, while
      // EGL_MULTISAMPLE_RESOLVE_BOX chooses a one-pixel wide box filter
      // placing equal weighting on all multisample values.
      //
      // The initial value of EGL_MULTISAMPLE_RESOLVE is
      // EGL_MULTISAMPLE_RESOLVE_DEFAULT.
      
      /* not implemented */
      s_error = EGL_BAD_ATTRIBUTE;
      return EGL_FALSE;
      break;
      
    case EGL_SWAP_BEHAVIOR:
      // Specifies the effect on the color buffer of posting a surface
      // with eglSwapBuffers. A value of EGL_BUFFER_PRESERVED indicates
      // that color buffer contents are unaffected, while
      // EGL_BUFFER_DESTROYED indicates that color buffer contents may be
      // destroyed or changed by the operation.
      //
      // The initial value of EGL_SWAP_BEHAVIOR is chosen by the
      // implementation.
      if (value == EGL_BUFFER_PRESERVED) {
        surface->retained_backing = YES;
      } else if (value == EGL_BUFFER_DESTROYED) {
        surface->retained_backing = NO;
      } else {
        // do not change the value
      }
      break;
      
    default:
      s_error = EGL_BAD_ATTRIBUTE;
      return EGL_FALSE;
  }
  
  return EGL_TRUE;
}

EGLBoolean eglBindTexImage(EGLDisplay dpy, EGLSurface surface, EGLint buffer)
{
  CHECK_DISPLAY(dpy);
  
  // FIXME: not implemented
  
  return EGL_FALSE;
}

EGLBoolean eglReleaseTexImage(EGLDisplay dpy, EGLSurface surface, EGLint buffer)
{
  CHECK_DISPLAY(dpy);
  
  // FIXME: not implemented
  
  return EGL_FALSE;
}

EGLBoolean eglSwapInterval(EGLDisplay dpy, EGLint interval)
{
  CHECK_DISPLAY(dpy);
  
  // FIXME: not implemented
  
  // NOTE:
  // This function doesn't make much sense because the clients are expected
  // to handle the updates themselves.
  
  return EGL_FALSE;
}

EGLContext eglCreateContext(EGLDisplay dpy, EGLConfig config,
                            EGLContext share_context,
                            const EGLint *attrib_list)
{
  CHECK_DISPLAY(dpy);
  
  AEGLPlatformConfig cfg;
  _configToPlatform(config, &cfg);
  
  EAGLRenderingAPI api_version = cfg.api_version;
  if (attrib_list) {
    for (unsigned i = 0; attrib_list[i] != EGL_NONE; i += 2) {
      if (attrib_list[i] == EGL_CONTEXT_CLIENT_VERSION) {
        if (attrib_list[i+1] == 1) {
          api_version = kEAGLRenderingAPIOpenGLES1;
        } else if (attrib_list[i+1] == 2) {
          api_version = kEAGLRenderingAPIOpenGLES2;
        }
      }
    }
  }
  
  EAGLContext *ctx;
  if (share_context == EGL_NO_CONTEXT) {
    ctx = [[EAGLContext alloc] initWithAPI:api_version];
  } else {
    ctx = [[EAGLContext alloc] initWithAPI:api_version
                                sharegroup:[((AEGLContext *)share_context)->context sharegroup]];
  }
  
  if (!ctx) {
    return EGL_NO_CONTEXT;
  }
  
  AEGLContext *context = malloc(sizeof(AEGLContext));
  context->context = ctx;
  context->config = config;
  context->api_version = (api_version == kEAGLRenderingAPIOpenGLES1
                          ? 1
                          : 2);
  
  return context;
}

EGLBoolean eglDestroyContext(EGLDisplay dpy, EGLContext ctx)
{
  CHECK_DISPLAY(dpy);
  
  // TODO:
  // If the EGL rendering context context is not current to any thread,
  // eglDestroyContext destroys it immediately. Otherwise, context is
  // destroyed when it becomes not current to any thread.
  
  if (!ctx) {
    s_error = EGL_BAD_CONTEXT;
    return EGL_FALSE;
  }
  
  AEGLContext *context = (AEGLContext *)ctx;
  [EAGLContext setCurrentContext:nil];
  [context->context release];
  
  free(context);
  
  return EGL_TRUE;
}

EGLBoolean eglMakeCurrent(EGLDisplay dpy, EGLSurface draw, EGLSurface read, EGLContext ctx)
{
  CHECK_DISPLAY(dpy);
  
  if (draw != read)
    return EGL_FALSE;
  if (draw == EGL_NO_SURFACE || read == EGL_NO_SURFACE || ctx == EGL_NO_CONTEXT) {
    glBindFramebuffer(GL_FRAMEBUFFER, 0);
    [EAGLContext setCurrentContext:nil];
    return EGL_TRUE;
  }
  
  AEGLContext *context = (AEGLContext *)ctx;
  
  AEGLDisplay *display = (AEGLDisplay *)dpy;
  display->context = context;
  
  AEGLSurface *surface = (AEGLSurface *)draw;
  surface->context = context;
  surface->renderBufferTarget = (GL_RENDERBUFFER);
  //GLKView *glview = (GLKView *)surface->glView;
  //glview.context = context->context;
  
  [EAGLContext setCurrentContext:context->context];
  if (surface->framebuffer != 0) {
    glBindFramebuffer(GL_FRAMEBUFFER, surface->framebuffer);
    return EGL_TRUE;
  }
  
  if (_setupFrameBuffer(surface, surface->context)) {
    return EGL_TRUE;
  }
  
  return EGL_FALSE;
}


EGLContext eglGetCurrentContext(void)
{
  // FIXME
  return [EAGLContext currentContext];
}

EGLSurface eglGetCurrentSurface(EGLint readdraw)
{
  return NULL;
}

EGLDisplay eglGetCurrentDisplay(void)
{
  return &s_defaultDisplay;
}

EGLBoolean eglQueryContext(EGLDisplay dpy, EGLContext ctx,
                           EGLint attribute, EGLint *value)
{
  CHECK_DISPLAY(dpy);
  
  if (!value) {
    return EGL_FALSE;
  }
  
  if (!ctx) {
    s_error = EGL_BAD_CONTEXT;
    return EGL_FALSE;
  }
  
  AEGLContext *context = (AEGLContext *)ctx;
  
  switch (attribute) {
    case EGL_CONFIG_ID:
      // Returns the ID of the EGL frame buffer configuration with
      // respect to which the context was created.
      *value = 0;  // FIXME
      break;
      
    case EGL_CONTEXT_CLIENT_TYPE:
      // Returns the type of client API which the context supports (one
      // of EGL_OPENGL_API, EGL_OPENGL_ES_API, or EGL_OPENVG_API).
      *value = EGL_OPENGL_ES_API;
      break;
      
    case EGL_CONTEXT_CLIENT_VERSION:
      // Returns the version of the client API which the context
      // supports, as specified at context creation time. The resulting
      // value is only meaningful for an OpenGL ES context.
      *value = context->api_version;
      break;
      
    case EGL_RENDER_BUFFER:
      // Returns the buffer which client API rendering via the context
      // will use. The value returned depends on properties of both the
      // context, and the surface to which the context is bound:
      //
      //   * If the context is bound to a pixmap surface, then
      //     EGL_SINGLE_BUFFER will be returned.
      //
      //   * If the context is bound to a pbuffer surface, then
      //     EGL_BACK_BUFFER will be returned.
      //
      //   * If the context is bound to a window surface, then either
      //     EGL_BACK_BUFFER or EGL_SINGLE_BUFFER may be returned. The
      //     value returned depends on both the buffer requested by the
      //     setting of the EGL_RENDER_BUFFER property of the surface
      //     (which may be queried by calling eglQuerySurface), and on the
      //     client API (not all client APIs support single-buffer
      //     rendering to window surfaces).
      //
      //   * If the context is not bound to a surface, such as an OpenGL
      //     ES context bound to a framebuffer object, then EGL_NONE will
      //     be returned.
      *value = 0;  // FIXME
      break;
      
    default:
      s_error = EGL_BAD_ATTRIBUTE;
      return EGL_FALSE;
  }
  
  return EGL_TRUE;
}


EGLBoolean eglWaitGL(void)
{
  return false;
}

EGLBoolean eglWaitNative(EGLint engine)
{
  return false;
}

EGLBoolean eglSwapBuffers(EGLDisplay dpy, EGLSurface sfc)
{
  CHECK_DISPLAY(dpy);
  
  AEGLSurface *surface = (AEGLSurface *)sfc;
  if ([surface->context->context presentRenderbuffer:surface->renderBufferTarget]) {
    return EGL_TRUE;
  }
  
  return EGL_FALSE;
}

EGLBoolean eglCopyBuffers(EGLDisplay dpy, EGLSurface surface,
                          EGLNativePixmapType target)
{
  CHECK_DISPLAY(dpy);
  
  // FIXME: not implemented
  return false;
}

#pragma mark -

GLuint _createDepthBuffer(GLint width, GLint height, GLuint bits)
{
  // It assumes that a render buffer is bound to the current context
  
  if (bits == 0) {
    return 0;
  }
  
  GLuint depth = 0;
  glGenRenderbuffers(1, &depth);
  if (!depth)
    return 0;
  
  GLuint depth_component = 0;
  if (bits == 16) {
    depth_component = GL_DEPTH_COMPONENT16;
  } else if (bits == 24) {
    depth_component = GL_DEPTH_COMPONENT24_OES;
  } else {
    /* ignore the bits value and use the default 16 bits */
    depth_component = GL_DEPTH_COMPONENT16;
  }
  
  glBindRenderbuffer(GL_RENDERBUFFER, depth);
  glRenderbufferStorage(GL_RENDERBUFFER, depth_component, width, height);
  glFramebufferRenderbuffer(GL_FRAMEBUFFER, GL_DEPTH_ATTACHMENT, GL_RENDERBUFFER, depth);
  return depth;
}

_Bool _setupFrameBuffer(AEGLSurface *surface, AEGLContext *context)
{
  GLuint framebuffer, color, depth = 0;
  
  // Create the framebuffer
  glGenFramebuffers(1, &framebuffer);
  glBindFramebuffer(GL_FRAMEBUFFER, framebuffer);
  
  // Create the color buffer
  glGenRenderbuffers(1, &color);
  glBindRenderbuffer(GL_RENDERBUFFER, color);
  //glRenderbufferStorage(GL_RENDERBUFFER, GL_RGBA4, 1080, 1920);
  [context->context renderbufferStorage:(GL_RENDERBUFFER) fromDrawable:(CAEAGLLayer*)surface->glView.layer];
  glFramebufferRenderbuffer(GL_FRAMEBUFFER, GL_COLOR_ATTACHMENT0, GL_RENDERBUFFER, color);
  
  if (context->config->depth_size) {
    // Create the depth buffer
    GLint backingWidth, backingHeight;
    glGetRenderbufferParameteriv(GL_RENDERBUFFER, GL_RENDERBUFFER_WIDTH, &backingWidth);
    glGetRenderbufferParameteriv(GL_RENDERBUFFER, GL_RENDERBUFFER_HEIGHT, &backingHeight);
    depth = _createDepthBuffer(backingWidth, backingHeight, context->config->depth_size);
    glViewport(0, 0, backingWidth, backingHeight);
  }
  
  if (glCheckFramebufferStatus(GL_FRAMEBUFFER) != GL_FRAMEBUFFER_COMPLETE) {
    return false;
  }
  
  surface->framebuffer = framebuffer;
  surface->colorRenderbuffer = color;
  surface->depthRenderbuffer = depth;
  
  // Make color buffer the current bound renderbuffer
  //glBindRenderbuffer(GL_RENDERBUFFER, color);
  
  return true;
}

_Bool _configToPlatform(AEGLConfig *config, AEGLPlatformConfig *out_config)
{
  if (config->render_type_bitmask & EGL_OPENGL_ES2_BIT) {
    out_config->api_version = kEAGLRenderingAPIOpenGLES2;
  } else if (config->render_type_bitmask & EGL_OPENGL_ES_BIT) {
    out_config->api_version = kEAGLRenderingAPIOpenGLES1;
  } else {
    // Return the minimum available version
    out_config->api_version = kEAGLRenderingAPIOpenGLES1;
  }
  
  if (config->red_size >= 8 || config->green_size >= 8 || config->blue_size >= 8 || config->alpha_size > 0) {
    out_config->color_format = kEAGLColorFormatRGBA8;
  } else {
    out_config->color_format = kEAGLColorFormatRGB565;
  }
  
  if (config->depth_size == 0) {
    out_config->depth_component = 0;
  } else if (config->depth_size == 16) {
    out_config->depth_component = GL_DEPTH_COMPONENT16;
  } else if (config->depth_size == 24) {
    out_config->depth_component = GL_DEPTH_COMPONENT24_OES;
  } else {
    out_config->depth_component = GL_DEPTH_COMPONENT16;
  }
  
  out_config->retained_backing = YES;
  
  return true;
}

