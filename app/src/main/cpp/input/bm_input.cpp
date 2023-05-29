//
// Created by Xaoyv on 2023/4/25.
//
#define LOG_TAG "bm_input"
#define LOG_NDEBUG 1

#include <jni.h>
#include <cstdlib>
#include <cstring>
#include <ctime>
#include "InputCode.h"

#include "log/log_main.h"
#include <android/keycodes.h>

#define GET_ID_0 arr_data[0]
#define GET_X_0 arr_data[1]
#define GET_Y_0 arr_data[2]

#define GET_ID arr_data[i * 3 + 0]
#define GET_X arr_data[i * 3 + 1]
#define GET_Y arr_data[i * 3 + 2]

#define SIZEOF_INPUTEVENT 24
#define SIZEOF_KEY_RESULT (2 * 24)

static int num_touch_major = 0;
static bool more_flinger = false;

#define CASE_KBCODE_AKEY(text, value) case AKEYCODE_##text : \
                return value;                                \

#define CASE_AKEY2KBCODE(value, text) CASE_KBCODE_AKEY(text, value)

void
copy_data(jbyte *ptr, uint64_t sec, uint64_t nsec, uint16_t type, uint16_t code, uint32_t value) {
//    memcpy(ptr, &sec, sizeof(long));
    *((uint64_t *) ptr) = sec;
    ptr += sizeof(uint64_t);
//    memcpy(ptr, &usec, sizeof(long));
    *((uint64_t *) ptr) = nsec;
    ptr += sizeof(uint64_t);
    memcpy(ptr, &type, 2);
    ptr += 2;
    memcpy(ptr, &code, 2);
    ptr += 2;
    memcpy(ptr, &value, 4);
//    ptr += 4;
}

int getKeyboardCode(int akey) {
    switch (akey) {
        //从qwerty.kl中copy
        CASE_AKEY2KBCODE(399, GRAVE)
        CASE_AKEY2KBCODE(2, 1)
        CASE_AKEY2KBCODE(3, 2)
        CASE_AKEY2KBCODE(4, 3)
        CASE_AKEY2KBCODE(5, 4)
        CASE_AKEY2KBCODE(6, 5)
        CASE_AKEY2KBCODE(7, 6)
        CASE_AKEY2KBCODE(8, 7)
        CASE_AKEY2KBCODE(9, 8)
        CASE_AKEY2KBCODE(10, 9)
        CASE_AKEY2KBCODE(11, 0)
        CASE_AKEY2KBCODE(158, BACK)
        CASE_AKEY2KBCODE(230, SOFT_RIGHT)
        CASE_AKEY2KBCODE(60, SOFT_LEFT)
        CASE_AKEY2KBCODE(107, ENDCALL)
//        CASE_AKEY2KBCODE(62, ENDCALL)
        CASE_AKEY2KBCODE(229, MENU)
//        CASE_AKEY2KBCODE(139, MENU)
//        CASE_AKEY2KBCODE(59, MENU)
        CASE_AKEY2KBCODE(127, SEARCH)
//        CASE_AKEY2KBCODE(217, SEARCH)
        CASE_AKEY2KBCODE(228, POUND)
        CASE_AKEY2KBCODE(227, STAR)
        CASE_AKEY2KBCODE(231, CALL)
//        CASE_AKEY2KBCODE(61, CALL)
        CASE_AKEY2KBCODE(232, DPAD_CENTER)
        CASE_AKEY2KBCODE(108, DPAD_DOWN)
        CASE_AKEY2KBCODE(103, DPAD_UP)
        CASE_AKEY2KBCODE(102, HOME)
        CASE_AKEY2KBCODE(105, DPAD_LEFT)
        CASE_AKEY2KBCODE(106, DPAD_RIGHT)
        CASE_AKEY2KBCODE(115, VOLUME_UP)
        CASE_AKEY2KBCODE(114, VOLUME_DOWN)
        CASE_AKEY2KBCODE(116, POWER)
        CASE_AKEY2KBCODE(212, CAMERA)

        CASE_AKEY2KBCODE(16, Q)
        CASE_AKEY2KBCODE(17, W)
        CASE_AKEY2KBCODE(18, E)
        CASE_AKEY2KBCODE(19, R)
        CASE_AKEY2KBCODE(20, T)
        CASE_AKEY2KBCODE(21, Y)
        CASE_AKEY2KBCODE(22, U)
        CASE_AKEY2KBCODE(23, I)
        CASE_AKEY2KBCODE(24, O)
        CASE_AKEY2KBCODE(25, P)
        CASE_AKEY2KBCODE(26, LEFT_BRACKET)
        CASE_AKEY2KBCODE(27, RIGHT_BRACKET)
        CASE_AKEY2KBCODE(43, BACKSLASH)

        CASE_AKEY2KBCODE(30, A)
        CASE_AKEY2KBCODE(31, S)
        CASE_AKEY2KBCODE(32, D)
        CASE_AKEY2KBCODE(33, F)
        CASE_AKEY2KBCODE(34, G)
        CASE_AKEY2KBCODE(35, H)
        CASE_AKEY2KBCODE(36, J)
        CASE_AKEY2KBCODE(37, K)
        CASE_AKEY2KBCODE(38, L)
        CASE_AKEY2KBCODE(39, SEMICOLON)
        CASE_AKEY2KBCODE(40, APOSTROPHE)
        CASE_AKEY2KBCODE(14, DEL)

        CASE_AKEY2KBCODE(44, Z)
        CASE_AKEY2KBCODE(45, X)
        CASE_AKEY2KBCODE(46, C)
        CASE_AKEY2KBCODE(47, V)
        CASE_AKEY2KBCODE(48, B)
        CASE_AKEY2KBCODE(49, N)
        CASE_AKEY2KBCODE(50, M)
        CASE_AKEY2KBCODE(51, COMMA)
        CASE_AKEY2KBCODE(52, PERIOD)
        CASE_AKEY2KBCODE(53, SLASH)
        CASE_AKEY2KBCODE(28, ENTER)

        CASE_AKEY2KBCODE(56, ALT_LEFT)
        CASE_AKEY2KBCODE(100, ALT_RIGHT)
        CASE_AKEY2KBCODE(42, SHIFT_LEFT)
        CASE_AKEY2KBCODE(54, SHIFT_RIGHT)
        CASE_AKEY2KBCODE(15, TAB)
        CASE_AKEY2KBCODE(57, SPACE)
        CASE_AKEY2KBCODE(150, EXPLORER)
        CASE_AKEY2KBCODE(155, ENVELOPE)

        CASE_AKEY2KBCODE(12, MINUS)
        CASE_AKEY2KBCODE(13, EQUALS)
        CASE_AKEY2KBCODE(215, AT)

//# On an AT keyboard: ESC, F10
//        CASE_AKEY2KBCODE(1, BACK)
//        CASE_AKEY2KBCODE(68, MENU)

//# App switch = Overview key
        CASE_AKEY2KBCODE(580, APP_SWITCH)

//# Media control keys
        CASE_AKEY2KBCODE(160, MEDIA_CLOSE)
        CASE_AKEY2KBCODE(161, MEDIA_EJECT)
        CASE_AKEY2KBCODE(163, MEDIA_NEXT)
        CASE_AKEY2KBCODE(164, MEDIA_PLAY_PAUSE)
        CASE_AKEY2KBCODE(165, MEDIA_PREVIOUS)
        CASE_AKEY2KBCODE(166, MEDIA_STOP)
        CASE_AKEY2KBCODE(167, MEDIA_RECORD)
        CASE_AKEY2KBCODE(168, MEDIA_REWIND)

        CASE_AKEY2KBCODE(142, SLEEP)
        CASE_AKEY2KBCODE(581, STEM_PRIMARY)
        CASE_AKEY2KBCODE(582, STEM_1)
        CASE_AKEY2KBCODE(583, STEM_2)
        CASE_AKEY2KBCODE(584, STEM_3)
        default:
            return 0;
    }
}

extern "C"
JNIEXPORT jint JNICALL
Java_com_magic_vm_cpputil_InputEventFun_formatMotionData(JNIEnv *env, jclass clazz,
                                                         jintArray data, jint count, jint action,
                                                         jbyteArray result) {
    timespec t = {0, 0};
    clock_gettime(CLOCK_MONOTONIC, &t);
    uint64_t nsec = t.tv_nsec / 1000;// + 10 * 1000;
    uint64_t sec = t.tv_sec;
//    ALOGD("key sec = %ld, nsec = %ld", sec, nsec);
    jint *arr_data = env->GetIntArrayElements(data, NULL);
    jbyte *buf = (jbyte *) malloc(SIZEOF_INPUTEVENT * 256);
    if (buf == nullptr) {
        ALOGE("xxx malloc error: 0");
        return 0;
    }
    jbyte *ptr = buf;
    jint actionMask = (action & ACTION_MASK);
    //用于判断哪个手指
    jint actionIndex = (action & ACTION_POINTER_INDEX_MASK) >> ACTION_POINTER_INDEX_SHIFT;
    //单指/第一次按下
    if (actionMask == ACTION_DOWN) {
        copy_data(ptr, sec, nsec, EV_ABS, ABS_MT_SLOT, 0);//这里把ID设置成0
        ptr += SIZEOF_INPUTEVENT;
        copy_data(ptr, sec, nsec, EV_ABS, ABS_MT_TRACKING_ID, 0);//57 0
        ptr += SIZEOF_INPUTEVENT;
        copy_data(ptr, sec, nsec, EV_ABS, ABS_MT_TOUCH_MAJOR, num_touch_major++);//48 count
        ptr += SIZEOF_INPUTEVENT;
        copy_data(ptr, sec, nsec, EV_ABS, ABS_MT_PRESSURE, 0x81); //129
        ptr += SIZEOF_INPUTEVENT;
        copy_data(ptr, sec, nsec, EV_ABS, ABS_MT_POSITION_X, GET_X_0);
        ptr += SIZEOF_INPUTEVENT;
        copy_data(ptr, sec, nsec, EV_ABS, ABS_MT_POSITION_Y, GET_Y_0);
        ptr += SIZEOF_INPUTEVENT;
        more_flinger = false;//多指
    }
    //多指按下
    if (actionMask == ACTION_POINTER_DOWN) {
        more_flinger = true;//多指
//        ALOGD("ACTION_POINTER_DOWN actionIndex = %d", actionIndex);
        for (int i = 0; i < count; ++i) {
            //一般一个MotionEvent对应一个手指的事件，两个手指同时「点击」会产生两个事件，所以这里只发一次
            if (GET_ID != actionIndex) continue;
            copy_data(ptr, sec, nsec, EV_ABS, ABS_MT_SLOT, GET_ID);
            ptr += SIZEOF_INPUTEVENT;
            copy_data(ptr, sec, nsec, EV_ABS, ABS_MT_TRACKING_ID, actionIndex);//57 1 多指
            ptr += SIZEOF_INPUTEVENT;
            copy_data(ptr, sec, nsec, EV_ABS, ABS_MT_TOUCH_MAJOR, num_touch_major++);//48 count
            ptr += SIZEOF_INPUTEVENT;
            copy_data(ptr, sec, nsec, EV_ABS, ABS_MT_PRESSURE, 0x81); //129
            ptr += SIZEOF_INPUTEVENT;
            copy_data(ptr, sec, nsec, EV_ABS, ABS_MT_POSITION_X, GET_X);
            ptr += SIZEOF_INPUTEVENT;
            copy_data(ptr, sec, nsec, EV_ABS, ABS_MT_POSITION_Y, GET_Y);
            ptr += SIZEOF_INPUTEVENT;
        }
    }
    //移动
    if (actionMask == ACTION_MOVE) {
        for (int i = 0; i < count; ++i) {
//            ALOGD("ACTION_MOVE actionMove PointId = %d", GET_ID);
            /// 郑重声明： 追踪移动事件时，请认准 PointId，这是唯一官方指定标准，不要相信 ActionIndex 那个小婊砸。
            /// 以上话术来自：https://juejin.cn/post/6844903449344081934
            if (more_flinger) {
                copy_data(ptr, sec, nsec, EV_ABS, ABS_MT_SLOT, GET_ID);
                ptr += SIZEOF_INPUTEVENT;
            }
            copy_data(ptr, sec, nsec, EV_ABS, ABS_MT_POSITION_X, GET_X);
            ptr += SIZEOF_INPUTEVENT;
            if (more_flinger) {
                copy_data(ptr, sec, nsec, EV_ABS, ABS_MT_SLOT, GET_ID);
                ptr += SIZEOF_INPUTEVENT;
            }
            copy_data(ptr, sec, nsec, EV_ABS, ABS_MT_POSITION_Y, GET_Y);
            ptr += SIZEOF_INPUTEVENT;
        }
    }
    //多指抬起
    if (actionMask == ACTION_POINTER_UP) {
//        ALOGD("ACTION_POINTER_UP actionIndex = %d", actionIndex);
        copy_data(ptr, sec, nsec, EV_ABS, ABS_MT_SLOT, actionIndex);
        ptr += SIZEOF_INPUTEVENT;
        copy_data(ptr, sec, nsec, EV_ABS, ABS_MT_PRESSURE, 0);
        ptr += SIZEOF_INPUTEVENT;
        copy_data(ptr, sec, nsec, EV_ABS, ABS_MT_TRACKING_ID, -1);
        ptr += SIZEOF_INPUTEVENT;
    }
    //取消
    if (actionMask == ACTION_UP) {
//        ALOGD("ACTION_UP actionIndex = %d, id0 = %d", actionIndex, GET_ID_0);
        if (more_flinger) {
            copy_data(ptr, sec, nsec, EV_ABS, ABS_MT_SLOT, GET_ID_0);
            ptr += SIZEOF_INPUTEVENT;
            more_flinger = false;
        }
        copy_data(ptr, sec, nsec, EV_ABS, ABS_MT_PRESSURE, 0);
        ptr += SIZEOF_INPUTEVENT;
        copy_data(ptr, sec, nsec, EV_ABS, ABS_MT_TRACKING_ID, -1);
        ptr += SIZEOF_INPUTEVENT;
    }
    if (actionMask == ACTION_CANCEL) {
        more_flinger = false;
        for (int i = 0; i < count; ++i) {
            copy_data(ptr, sec, nsec, EV_ABS, ABS_MT_SLOT, GET_ID);
            ptr += SIZEOF_INPUTEVENT;
            copy_data(ptr, sec, nsec, EV_ABS, ABS_MT_PRESSURE, 0);
            ptr += SIZEOF_INPUTEVENT;
            copy_data(ptr, sec, nsec, EV_ABS, ABS_MT_TRACKING_ID, -1);
            ptr += SIZEOF_INPUTEVENT;
        }
    }
    if (actionMask != ACTION_DOWN
        && actionMask != ACTION_POINTER_DOWN
        && actionMask != ACTION_MOVE
        && actionMask != ACTION_POINTER_UP
        && actionMask != ACTION_UP
        && actionMask != ACTION_CANCEL) {
        ALOGE("ERROR actionMask %d", actionMask);
    }
    //end
    copy_data(ptr, sec, nsec, EV_SYN, 0, 0);
    ptr += SIZEOF_INPUTEVENT;

    int size = (int) (ptr - buf);
    env->SetByteArrayRegion(result, 0, (jsize) (size), buf);
    free(buf);
    return (jint) (size);
}

extern "C"
JNIEXPORT jint JNICALL
Java_com_magic_vm_cpputil_InputEventFun_formatKeyEventData(JNIEnv *env, jclass clazz, jint key_code,
                                                           jboolean is_down, jbyteArray result) {
    timespec t = {0, 0};
    clock_gettime(CLOCK_MONOTONIC, &t);
    uint64_t nsec = t.tv_nsec / 1000;// + 10 * 1000;
    uint64_t sec = t.tv_sec;
//    ALOGD("key sec = %ld, nsec = %ld", sec, nsec);
    jbyte *buf = (jbyte *) malloc(SIZEOF_KEY_RESULT);
    if (buf == nullptr) {
//        ALOGD("xxx keyevent malloc error: 0");
        return 0;
    }
    jbyte *ptr = buf;
    if (is_down) is_down = 1;
    else is_down = 0;

    //获取键盘code，发送keyboard code，dispatch会转换成对应的AKEYCODE_xxx
    int keyboardCode = getKeyboardCode(key_code);

    copy_data(ptr, sec, nsec, EV_KEY, keyboardCode, is_down);//这里把ID设置成0
    ptr += SIZEOF_INPUTEVENT;
    copy_data(ptr, sec, nsec, EV_SYN, 0, 0);//这里把ID设置成0

    //result
    env->SetByteArrayRegion(result, 0, (jsize) (SIZEOF_KEY_RESULT), buf);
    free(buf);
    return SIZEOF_KEY_RESULT;
}