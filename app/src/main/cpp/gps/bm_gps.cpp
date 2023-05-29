//
// Created by Xaoyv on 2023/5/29.
//

#define PATH "/gps"
#define LOG_TAG "bm_gps"

#include <jni.h>
#include <sys/socket.h>
#include <linux/un.h>
#include <cstring>
#include <ctime>
#include <unistd.h>
#include "gps.h"
#include "log/log_main.h"


static double latitude;
static double longitude;
static double metersElevation;
static int nSatellites;

extern "C"
void sendGps(int fd) {
    timespec t = {0, 0};
    while (true) {
        clock_gettime(CLOCK_REALTIME, &t);
        android_gps_send_location(fd, latitude, longitude, metersElevation, nSatellites,
                                  reinterpret_cast<const timeval *>(&t.tv_sec));
        ALOGE("gps sendend");
        sleep(30 * 3000);
    }
}

extern "C"
JNIEXPORT void JNICALL
Java_com_magic_vm_cpputil_GpsFun_startGps(JNIEnv *env, jclass clazz) {
    //waiting accept
    int fd = socket(AF_UNIX, SOCK_STREAM, 0);
    if (fd <= 0) {
        return;
    }
    sockaddr_un sockaddrUn;
    sockaddrUn.sun_family = AF_UNIX;
    sockaddrUn.sun_path[0] = '\0';
    strcpy(sockaddrUn.sun_path + 1, PATH);

    int bind_result = bind(fd, reinterpret_cast<const sockaddr *>(&sockaddrUn),
                           (socklen_t) strlen(PATH) + offsetof(sockaddr_un, sun_path));
    if (bind_result < 0) {
        return;
    }
    int listen_result = listen(fd, 10);
    if (listen_result < 0) {
        return;
    }
    do {
        sockaddr_un sock_client;
        socklen_t len = sizeof(sock_client);
        int accept_fd = accept(fd, reinterpret_cast<sockaddr *>(&sock_client), &len);
        if (accept_fd < 0) {
            continue;
        }
        sendGps(accept_fd);
    } while (true);
}

extern "C"
JNIEXPORT void JNICALL
Java_com_magic_vm_cpputil_GpsFun_updateGps(JNIEnv *env, jclass clazz, jdouble _latitude,
                                           jdouble _longitude, jdouble _meters_elevation,
                                           jint _n_satellites) {
    latitude = _latitude;
    longitude = _longitude;
    metersElevation = _meters_elevation;
    nSatellites = _n_satellites;
}