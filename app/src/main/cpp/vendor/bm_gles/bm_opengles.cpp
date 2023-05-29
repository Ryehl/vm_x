//
// Created by Xaoyv on 2023/4/7.
//

#define LOG_NDEBUG 0
#define LOG_TAG "bm_opengles"

#include "bm_opengles.h"

#include "android/utils/debug.h"

#include "render_api.h"
#include "TcpStream.h"

static emugl::RenderLibPtr sRenderLib = nullptr;
static emugl::RendererPtr sRenderer = nullptr;

//init
int android_initOpenglesEmulation(void) {
    if (sRenderLib != NULL)
        return 0;

    ALOGD("Initializing hardware OpenGLES emulation support");

    sRenderLib = initLibrary();
    if (!sRenderLib) {
        derror("OpenGLES initialization failed!");
        goto BAD_EXIT;
    }

    return 0;

    BAD_EXIT:
    derror("OpenGLES emulation library could not be initialized!");
    return -1;
}

#define STREAM_NAME  "opengles/rootfs"
#define STREAM_BUFFER_SIZE  (4*1024*1024)

int
android_startTcpListener(void) {
    TcpStream tcpStream(STREAM_BUFFER_SIZE);
    tcpStream.listen(STREAM_NAME);
    while (true) {
        SocketStream *socketStream = tcpStream.accept();
        if (!socketStream) {
            ALOGE("xxx tcp accept error: %d, info: %s", errno, strerror(errno));
            continue;
        }
        sRenderer.get()->createRenderChannel(socketStream);
        ALOGD("xxx tcp accept");
    }
    return 0;
}

void
android_resetGuestPostedAFrame(void) {
    if (sRenderer) {
        sRenderer->resetGuestPostedAFrame();
    }
}

bool
android_hasGuestPostedAFrame(void) {
    if (sRenderer) {
        return sRenderer->hasGuestPostedAFrame();
    }
    return false;
}

void
android_redrawOpenglesWindow(void) {
    if (sRenderer) {
        sRenderer->repaintOpenGLDisplay();
    }
}

int
android_showOpenglesWindow(void *window, int wx, int wy, int ww, int wh,
                           int fbw, int fbh, float dpr, float rotation,
                           bool deleteExisting) {
    if (!sRenderer) {
        return -1;
    }
    FBNativeWindowType win = (FBNativeWindowType) (uintptr_t) window;
    bool success = sRenderer->showOpenGLSubwindow(
            win, wx, wy, ww, wh, fbw, fbh, dpr, rotation,
            deleteExisting);

    return success ? 0 : -1;
}

int
android_hideOpenglesWindow(void) {
    if (!sRenderer) {
        return -1;
    }
    bool success = sRenderer->destroyOpenGLSubwindow();
    return success ? 0 : -1;
}

int
android_startOpenglesRenderer(int width, int height, bool guestPhoneApi, int guestApiLevel,
                              int *glesMajorVersion_out,
                              int *glesMinorVersion_out) {
    if (!sRenderLib) {
        ALOGD("Can't start OpenGLES renderer without support libraries");
        return -1;
    }

    if (sRenderer) {
        return 0;
    }

//    android_init_opengl_logger();

    sRenderLib->setRenderer(SELECTED_RENDERER_UNKNOWN);
    sRenderLib->setAvdInfo(guestPhoneApi, guestApiLevel);
//    sRenderLib->setCrashReporter(&crashhandler_die_format);
//    sRenderLib->setFeatureController(&android::featurecontrol::isEnabled);
//    sRenderLib->setSyncDevice(goldfish_sync_create_timeline,
//                              goldfish_sync_create_fence,
//                              goldfish_sync_timeline_inc,
//                              goldfish_sync_destroy_timeline,
//                              goldfish_sync_register_trigger_wait,
//                              goldfish_sync_device_exists);

//    emugl_logger_struct logfuncs;
//    logfuncs.coarse = android_opengl_logger_write;
//    logfuncs.fine = android_opengl_cxt_logger_write;
//    sRenderLib->setLogger(logfuncs);
//    emugl_dma_ops dma_ops;
//    dma_ops.add_buffer = android_goldfish_dma_ops.add_buffer;
//    dma_ops.remove_buffer = android_goldfish_dma_ops.remove_buffer;
//    dma_ops.get_host_addr = android_goldfish_dma_ops.get_host_addr;
//    dma_ops.invalidate_host_mappings = android_goldfish_dma_ops.invalidate_host_mappings;
//    dma_ops.unlock = android_goldfish_dma_ops.unlock;
//    sRenderLib->setDmaOps(dma_ops);

    sRenderer = sRenderLib->initRenderer(width, height, true, true);
    if (!sRenderer) {
        ALOGD("Can't start OpenGLES renderer?");
        return -1;
    }

    // after initRenderer is a success, the maximum GLES API is calculated depending
    // on feature control and host GPU support. Set the obtained GLES version here.
    if (glesMajorVersion_out && glesMinorVersion_out)
        sRenderLib->getGlesVersion(glesMajorVersion_out, glesMinorVersion_out);
    return 0;
}

