#ifndef ADSDK_GUARD_H
#define ADSDK_GUARD_H

#include <jni.h>

#define _ANONYMOUS(type, var, line, ...) type var##line(env, __VA_ARGS__);
#define GUARD_ANONYMOUS(type, line, ...) _ANONYMOUS(type, guard, line, __VA_ARGS__)

#define DeleteLocalRef_On_Exist(...) GUARD_ANONYMOUS(LocalRefScopeGuard, __LINE__, __VA_ARGS__)
#define ReleaseStringUTFChars_On_Exist(...) GUARD_ANONYMOUS(StringUTFCharsScopeGuard, __LINE__, __VA_ARGS__)
#define ReleaseByteArrayElements_On_Exist(...) GUARD_ANONYMOUS(ByteArrayElementsScopeGuard, __LINE__, __VA_ARGS__)

class LocalRefScopeGuard {
private:
    JNIEnv* env;
    jobject ref;

public:
    LocalRefScopeGuard(JNIEnv* env, jobject ref) {
        this->env = env;
        this->ref = ref;
    }

    ~LocalRefScopeGuard() {
        if (NULL != ref) {
            env->DeleteLocalRef(ref);
        }
    }
};

class StringUTFCharsScopeGuard {
private:
    JNIEnv* env;
    jstring string;
    const char* utf;

public:
    StringUTFCharsScopeGuard(JNIEnv* env, jstring string, const char* utf) {
        this->env = env;
        this->string = string;
        this->utf = utf;
    }

    ~StringUTFCharsScopeGuard() {
        if (NULL != utf) {
            env->ReleaseStringUTFChars(string, utf);
        }
    }
};

class ByteArrayElementsScopeGuard {
private:
    JNIEnv* env;
    jbyteArray array;
    jbyte* elems;

public:
    ByteArrayElementsScopeGuard(JNIEnv* env, jbyteArray array, jbyte* elems) {
        this->env = env;
        this->array = array;
        this->elems = elems;
    }

    ~ByteArrayElementsScopeGuard() {
        if (NULL != elems) {
            env->ReleaseByteArrayElements(array, elems, JNI_FALSE);
        }
    }
};

#endif //ADSDK_GUARD_H
