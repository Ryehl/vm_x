//
// Created by Xaoyv on 2023/4/7.
//

#ifndef VM_BM_OPENGLES_H
#define VM_BM_OPENGLES_H

int
android_initOpenglesEmulation(void);

int
android_startTcpListener(void);

void
android_resetGuestPostedAFrame(void);

bool
android_hasGuestPostedAFrame(void);

int
android_hideOpenglesWindow(void);

//opengles_redraw
void
android_redrawOpenglesWindow(void);

//opengles_show
int
android_showOpenglesWindow(void *window, int wx, int wy, int ww, int wh,
                           int fbw, int fbh, float dpr, float rotation,
                           bool deleteExisting);

//TODO orientation
//set_device_orientation  window->updateRotation(rotation)

//start renderThread
int
android_startOpenglesRenderer(int width, int height, bool guestPhoneApi, int guestApiLevel,
                              int *glesMajorVersion_out,
                              int *glesMinorVersion_out);

#endif //VM_BM_OPENGLES_H
