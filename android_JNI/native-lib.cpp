#include <jni.h>
#include <string>
#include <pthread.h>

#include "keybutton_check.h"  /* C++ Header file(keybutton event check) */
#include "native-lib.h"
#include "log.h"

#define NATIVE_METHODS_NUMBER   2

static JavaVM *java_vm;
static pthread_key_t current_jni_env;

struct fields_t {
    jmethodID   post_event;
    jfieldID    m_fid;
};
static fields_t fields;

jobject gs_object = NULL;
jclass mClass;

static void keycheck_integrate_Servicebuttoncheck_setUp_commonFuns(JNIEnv *env, jobject thiz);
static void keycheck_integrate_Servicebuttoncheck_init_keybutton_monitor(JNIEnv *env, jobject thiz);

/* Register this thread with the VM */
static JNIEnv *attach_current_thread(void);
static void detach_current_thread (void *env);
static JNIEnv *get_jni_env (void);

/* List of implemented native methods */
static JNINativeMethod native_methods[NATIVE_METHODS_NUMBER] = {
    { "init_keybutton_monitor", "()V", (void *)keycheck_integrate_Servicebuttoncheck_init_keybutton_monitor},
    { "setUp_commonFuns", "()V", (void *)keycheck_integrate_Servicebuttoncheck_setUp_commonFuns},
    // { "registResource", "(Ljava/lang/Object;Ljava/lang/String;)V", (void *) registResourceApi},
};

/* Library initializer - Dynamic loading */
jint JNI_OnLoad(JavaVM *vm, void *reserved) {
    JNIEnv *env = NULL;
    LOGI("nativeButtonCheck::JNI_OnLoad start");
    java_vm = vm;

    if ((vm)->GetEnv( (void**) &env, JNI_VERSION_1_6) != JNI_OK) {
        LOGD("nativeButtonCheck::JNI_OnLoad, Could not retrieve JNIEnv");
        return 0;
    }
    /* Get the class name of java's class */
    mClass = env->FindClass ( "com/eg/ek/keycheck_integrate/service_keybuttoncheck");
    if ( mClass == NULL ) {
        LOGD("nativeButtonCheck::FindClass() is NULL");
        return 0;
    }

    /* get the ID of java's methods */
    fields.post_event = env->GetMethodID( mClass, "sendBroadcastDisorder", "(I)V" );
    if ( fields.post_event == NULL ) {
        LOGD("nativeButtonCheck::GetMethodID() is NULL");
        return 0;
    }

    /* get the ID of java's variable */
    fields.m_fid = env->GetFieldID(mClass,"test_data","I");
    if ( fields.m_fid == NULL ) {
        LOGD("nativeButtonCheck::GetFieldID() is NULL");
        return 0;
    }

    /* Register the native methods - dynamic loading */
    env->RegisterNatives ( mClass, native_methods, NATIVE_METHODS_NUMBER);
    /* Create the private key of the current thread */
    pthread_key_create (&current_jni_env, detach_current_thread);

    LOGI("nativeButtonCheck::JNI_OnLoad end");
    return JNI_VERSION_1_6;
}

/* Stroage the JNIEnv enviroment */
void keycheck_integrate_Servicebuttoncheck_setUp_commonFuns(JNIEnv *env, jobject thiz) {
    
    LOGI("Enter setUp_commonFuns()...");

     /* Create the local variable(gs_object) of the current object */
    gs_object = env->NewGlobalRef(thiz);

    LOGI("leaving setUp_commonFuns()...");

    return;
}

/* Create the monitor keybutton thread */
void keycheck_integrate_Servicebuttoncheck_init_keybutton_monitor(JNIEnv *env, jobject thiz) {
    // TODO
    init_keybutton_monitor();
    return;
}

/* Java broadcast operation from hardkey event(C++) */
void Servicebuttoncheck_callbroadcast(int keyvalue) 
{
    LOGI("Enter Servicebuttoncheck_callbroadcast...");

    JNIEnv *env;

    env = get_jni_env ();
    env->SetIntField( gs_object, fields.m_fid, keyvalue); /* change the variable value of java's class */
    env->CallVoidMethod( gs_object, fields.post_event, keyvalue); /* call the method of java's class */

    LOGI("Leaving Servicebuttoncheck_callbroadcast.");

    return;
}


/* Retrieve the JNI environment for this thread */
static JNIEnv *get_jni_env (void) {
  JNIEnv *env;
  
  LOGI("Enter get_jni_env()...");
  if ((env = (JNIEnv*)pthread_getspecific(current_jni_env)) == NULL) {
    LOGI("Retrieve the JNI environment for this thread");
    env = attach_current_thread();
    pthread_setspecific(current_jni_env, env);
  }
  return env;
}

/* Register this thread with the VM */
static JNIEnv *attach_current_thread(void) {
  JNIEnv *env;
  JavaVMAttachArgs args;

  LOGD("Attaching thread 0x%x", pthread_self());
  args.version = JNI_VERSION_1_6;
  args.name = NULL;
  args.group = NULL;

  /* Attach the current thread to JVM, and get the JNIEnv pointer of thread */
  if ((java_vm)->AttachCurrentThread ( &env, &args) < 0) {
    LOGI("Failed to attach current thread");
    return NULL;
  }
  return env;
}

/* Unregister this thread from the VM */
static void detach_current_thread (void *env) {
  LOGI("Detaching thread %ld", pthread_self());
  (java_vm)->DetachCurrentThread();
}
