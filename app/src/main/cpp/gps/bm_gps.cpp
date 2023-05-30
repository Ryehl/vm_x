//
// Created by Xaoyv on 2023/5/29.
//

#define PATH "/gps"
#define LOG_TAG "bm_gps"
#define SLEEP_SECONDS 30

#include <jni.h>
#include <sys/socket.h>
#include <linux/un.h>
#include <cstring>
#include <ctime>
#include <unistd.h>
#include <pthread.h>
#include <csignal>
#include <iostream>
#include "gps.h"
#include "log/log_main.h"

static double latitude = 0;
static double longitude = 0;
static double metersElevation = 0;
static int nSatellites = 0;

extern "C"
void *sendGps(void *arg) {
    timespec t = {0, 0};
    int fd = *((int *) arg);

    while (true) {
        clock_gettime(CLOCK_REALTIME, &t);
        int sendResult = android_gps_send_location(fd, latitude, longitude,
                                                   metersElevation, nSatellites,
                                                   reinterpret_cast<const timeval *>(&t.tv_sec));
        if (sendResult < 0) {
            close(fd);
            ALOGE("gps send(%d) result: %d", fd, sendResult);
            break;
        }
        sleep(SLEEP_SECONDS);
    }
    return nullptr;
}

extern "C"
JNIEXPORT void JNICALL
Java_com_magic_vm_cpputil_GpsFun_startGps(JNIEnv *env, jclass clazz, jstring vm_id) {
    //waiting accept
    const char *c_vm_id = env->GetStringUTFChars(vm_id, NULL);
    int fd = socket(AF_UNIX, SOCK_STREAM, 0);
    if (fd <= 0) {
        return;
    }
    sockaddr_un sock_service{};
    sock_service.sun_family = AF_UNIX;
    sock_service.sun_path[0] = '\0';
    strcpy(sock_service.sun_path + 1, PATH);
    strcat(sock_service.sun_path + 1, c_vm_id);

    int len_service = strlen(PATH)
                      + strlen(c_vm_id)
                      + 1
                      + offsetof(sockaddr_un, sun_path);
    int bind_result = bind(fd, reinterpret_cast<const sockaddr *>(&sock_service),
                           (socklen_t) len_service);
    if (bind_result < 0) {
        return;
    }
    int listen_result = listen(fd, 10);
    if (listen_result < 0) {
        return;
    }
    static int last_accept = -1;
    do {
        sockaddr_un sock_client{};
        socklen_t len_client = sizeof(sock_client);
        int accept_fd = accept(fd, reinterpret_cast<sockaddr *>(&sock_client), &len_client);
        if (accept_fd < 0) {
            continue;
        }
        if (last_accept == accept_fd) {
            continue;
        }
        last_accept = accept_fd;
        pthread_t p;
        pthread_create(&p, NULL, sendGps, &accept_fd);
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