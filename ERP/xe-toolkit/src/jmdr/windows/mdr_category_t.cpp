#include <iostream>
#include <exception>
#include <jni.h>
#include "mdr_category.h"
#include "mdr_tuple.h"

static void fail(JNIEnv *env, jobject jobj, jclass cls, const char *msg)
{
    jfieldID reason;
    jstring jstr;

    reason = env->GetFieldID(cls, "reason", "Ljava/lang/String;");
    if (reason == 0)
    {
        std::cerr << "mdr_category_t::fail: cannot fail" << std::endl;
        return;
    }
    jstr = env->NewStringUTF(msg);
    env->SetObjectField(jobj, reason, jstr);
}

extern "C" JNIEXPORT jboolean JNICALL
Java_com_capmet_metrics_jmdr_mdr_1category_1t_category_1constructor
    (JNIEnv *env, jobject jobj, jstring jcategory, jstring jhost)
{
    jclass cls = env->GetObjectClass(jobj);
    jfieldID fid;
    mdr::mdr_category_t *cat;
    const char *category;

    fid = env->GetFieldID(cls, "handle", "J");
    if (fid == 0)
    {
        fail(env, jobj, cls, "mdr_category_t::mdr_category_t: cannot find handle");
        return JNI_FALSE;
    }
    try
    {
        const char *remote_host = 0;

        if (jhost != 0)
            remote_host = env->GetStringUTFChars(jhost, 0);

        category = env->GetStringUTFChars(jcategory, 0);

        cat = new mdr::mdr_category_t(category, remote_host);

        if (remote_host)
            env->ReleaseStringUTFChars(jcategory, remote_host);
    }
    catch(std::exception &e)
    {
        fail(env, jobj, cls, e.what());
        return JNI_FALSE;
    }
    env->SetLongField(jobj, fid, (jlong) (unsigned int) cat);

    //
    // mdr_category_supported
    //
    fid = env->GetFieldID(cls, "mdr_category_supported", "Z");
    if (fid == 0)
    {
        fail(env, jobj, cls, "mdr_category_t::mdr_category_t: cannot find mdr_category_supported");
        return JNI_FALSE;
    }
    env->SetBooleanField(jobj, fid, cat ? JNI_TRUE : JNI_FALSE);

    //
    // mdr_is_singleton
    //
    fid = env->GetFieldID(cls, "mdr_is_singleton", "Z");
    if (fid == 0)
    {
        fail(env, jobj, cls, "mdr_category_t::mdr_category_t: cannot find mdr_is_singleton");
        return JNI_FALSE;
    }
    env->SetBooleanField(jobj, fid,
        cat ? (cat->mdr_is_singleton ? JNI_TRUE : JNI_FALSE) : JNI_TRUE);

    //
    // mdr_field_count
    //
    fid = env->GetFieldID(cls, "mdr_field_count", "I");
    if (fid == 0)
    {
        fail(env, jobj, cls, "mdr_category_t::mdr_category_t: cannot find mdr_field_count");
        return JNI_FALSE;
    }
    env->SetIntField(jobj, fid, cat ? (int) cat->mdr_field_count : 0);

    return JNI_TRUE;
}

extern "C" JNIEXPORT jobject JNICALL
Java_com_capmet_metrics_jmdr_mdr_1category_1t_is_1supported
  (JNIEnv *env, jobject jobj, jint field_number)
{
    jclass cls = env->GetObjectClass(jobj);
    jclass rcls;
    jobject object;
    jmethodID id;

    // Windows: if the field is there, then it's supported
    rcls = env->FindClass("java/lang/Boolean");
    if (rcls == 0)
    {
        fail(env, jobj, cls, "cannot find java/lang/Boolean");
        return 0;
    }
    id = env->GetMethodID(rcls, "<init>", "(Z)V");
    if (id == 0)
    {
        fail(env, jobj, cls, "mdr_category_t::is_supported: cannot find public Boolean(boolean)");
        return 0;
    }
    object = env->NewObject(rcls, id, JNI_TRUE, 0);

    return object;
}

extern "C" JNIEXPORT jstring JNICALL
Java_com_capmet_metrics_jmdr_mdr_1category_1t_field_1name
    (JNIEnv *env, jobject jobj, jint field)
{
    jclass cls = env->GetObjectClass(jobj);
    jfieldID fid;
    mdr::mdr_category_t *cat;

    fid = env->GetFieldID(cls, "handle", "J");
    if (fid == 0)
    {
        fail(env, jobj, cls, "mdr_category_t::field_name: cannot find handle");
        return 0;
    }

    cat = (mdr::mdr_category_t *) (unsigned int) env->GetLongField(jobj, fid);
    if (cat == 0)
    {
        fail(env, jobj, cls, "mdr_category_t::field_name: null mdr_category_t handle");
        return 0;
    }

    return env->NewStringUTF(cat->mdr_field_name(field).c_str());
}

extern "C" JNIEXPORT jstring JNICALL
Java_com_capmet_metrics_jmdr_mdr_1category_1t_field_1explanation
    (JNIEnv *env, jobject jobj, jint field)
{
    jclass cls = env->GetObjectClass(jobj);
    jfieldID fid;
    mdr::mdr_category_t *cat;
    std::string explanation;

    fid = env->GetFieldID(cls, "handle", "J");
    if (fid == 0)
    {
        fail(env, jobj, cls, "mdr_category_t::field_explanation: cannot find handle");
        return 0;
    }

    cat = (mdr::mdr_category_t *) (unsigned int) env->GetLongField(jobj, fid);
    if (cat == 0)
    {
        fail(env, jobj, cls, "mdr_category_t::field_explanation: null mdr_category_t handle");
        return 0;
    }

    return env->NewStringUTF(cat->mdr_field_explanation(field, explanation).c_str());
}

extern "C" JNIEXPORT jstring JNICALL
Java_com_capmet_metrics_jmdr_mdr_1category_1t_category_1explanation
  (JNIEnv *env, jobject jobj)
{
    jclass cls = env->GetObjectClass(jobj);
    jfieldID fid;
    mdr::mdr_category_t *cat;

    fid = env->GetFieldID(cls, "handle", "J");
    if (fid == 0)
    {
        fail(env, jobj, cls, "mdr_category_t::field_explanation: cannot find handle");
        return 0;
    }

    cat = (mdr::mdr_category_t *) (unsigned int) env->GetLongField(jobj, fid);
    if (cat == 0)
    {
        fail(env, jobj, cls, "mdr_category_t::field_explanation: null mdr_category_t handle");
        return 0;
    }

    return env->NewStringUTF(cat->mdr_category_explanation().c_str());
}

extern "C" JNIEXPORT jint JNICALL
Java_com_capmet_metrics_jmdr_mdr_1category_1t_field_1name_1to_1number
    (JNIEnv *env, jobject jobj, jstring jfield)
{
    jclass cls = env->GetObjectClass(jobj);
    jfieldID fid;
    mdr::mdr_category_t *cat;

    fid = env->GetFieldID(cls, "handle", "J");
    if (fid == 0)
    {
        fail(env, jobj, cls, "mdr_category_t::field_name_to_number: cannot find handle");
        return -1;
    }

    cat = (mdr::mdr_category_t *) (unsigned int) env->GetLongField(jobj, fid);
    if (cat == 0)
    {
        fail(env, jobj, cls, "mdr_category_t::field_name_to_number: null mdr_category_t handle");
        return -1;
    }

    try
    {
        const char *field = env->GetStringUTFChars(jfield, 0);
        jint field_number = cat->mdr_field_name_to_number(field);

        env->ReleaseStringUTFChars(jfield, field);
        return field_number;
    }
    catch(std::exception &e)
    {
        fail(env, jobj, cls, e.what());
        return -1;
    }
}

extern "C" JNIEXPORT jobject JNICALL
Java_com_capmet_metrics_jmdr_mdr_1category_1t_all_1field_1names
    (JNIEnv *env, jobject jobj)
{
    jclass cls = env->GetObjectClass(jobj);
    jfieldID fid;
    mdr::mdr_category_t *cat;

    fid = env->GetFieldID(cls, "handle", "J");
    if (fid == 0)
    {
        fail(env, jobj, cls, "mdr_category_t::all_field_names: cannot find handle");
        return 0;
    }
    cat = (mdr::mdr_category_t *) (unsigned int) env->GetLongField(jobj, fid);
    if (cat == 0)
    {
        fail(env, jobj, cls, "mdr_category_t::all_field_names: null mdr_category_t handle");
        return 0;
    }

    try
    {
        mdr::mdr_keyset_t name_set;
        const mdr::mdr_keyset_t &names = cat->mdr_all_field_names(name_set);

        size_t n = names.size();
        size_t i;

        jobjectArray jarray = env->NewObjectArray(
            n, env->FindClass("java/lang/String"), env->NewStringUTF(""));

        for(i=0; i<n; i++)
        {
            jstring jstr = env->NewStringUTF(names[i].c_str());
            env->SetObjectArrayElement(jarray, i, jstr);
        }

        return jarray;
    }
    catch(std::exception &e)
    {
        fail(env, jobj, cls, e.what());
        return 0;
    }
}

extern "C" JNIEXPORT jobject JNICALL
Java_com_capmet_metrics_jmdr_mdr_1category_1t_supported_1field_1names
    (JNIEnv *env, jobject jobj)
{
    // all fields are supported fields on Windows
    return Java_com_capmet_metrics_jmdr_mdr_1category_1t_all_1field_1names
            (env, jobj);
}

extern "C" JNIEXPORT jboolean JNICALL
Java_com_capmet_metrics_jmdr_mdr_1category_1t_refresh
    (JNIEnv *env, jobject jobj)
{
    jclass cls = env->GetObjectClass(jobj);
    jfieldID fid;
    mdr::mdr_category_t *cat;

    fid = env->GetFieldID(cls, "handle", "J");
    if (fid == 0)
    {
        fail(env, jobj, cls, "mdr_category_t::refresh: cannot find handle");
        return JNI_FALSE;
    }
    cat = (mdr::mdr_category_t *) (unsigned int) env->GetLongField(jobj, fid);
    if (cat == 0)
    {
        fail(env, jobj, cls, "mdr_category_t::refresh: null mdr_category_t handle");
        return JNI_FALSE;
    }

    try
    {
        cat->mdr_refresh();
    }
    catch(std::exception &e)
    {
        fail(env, jobj, cls, e.what());
        return JNI_FALSE;
    }

    return JNI_TRUE;
}

extern "C" JNIEXPORT jboolean JNICALL
Java_com_capmet_metrics_jmdr_mdr_1category_1t_refresh_1rates
    (JNIEnv *env, jobject jobj)
{
    return Java_com_capmet_metrics_jmdr_mdr_1category_1t_refresh(env, jobj);
}

extern "C" JNIEXPORT jobjectArray JNICALL
Java_com_capmet_metrics_jmdr_mdr_1category_1t_get_1instance_1names
  (JNIEnv *env, jobject jobj)
{
    jclass cls = env->GetObjectClass(jobj);
    jfieldID fid;
    mdr::mdr_category_t *cat;

    fid = env->GetFieldID(cls, "handle", "J");
    if (fid == 0)
    {
        fail(env, jobj, cls, "mdr_category_t::get_instance_names: cannot find handle");
        return 0;
    }
    cat = (mdr::mdr_category_t *) (unsigned int) env->GetLongField(jobj, fid);
    if (cat == 0)
    {
        fail(env, jobj, cls, "mdr_category_t::get_instance_names: null mdr_category_t handle");
        return 0;
    }

    if (cat->mdr_is_singleton)
    {
        fail(env, jobj, cls, "mdr_category_t::get_instance_names: category is a singleton");
        return 0;
    }

    try
    {
        mdr::mdr_keyset_t name_set;
        const mdr::mdr_keyset_t &names = cat->mdr_get_instance_names(name_set);

        size_t n = names.size();
        size_t i;

        jobjectArray jarray = env->NewObjectArray(
            n, env->FindClass("java/lang/String"), env->NewStringUTF(""));

        for(i=0; i<n; i++)
        {
            jstring jstr = env->NewStringUTF(names[i].c_str());
            env->SetObjectArrayElement(jarray, i, jstr);
        }

        return jarray;
    }
    catch(std::exception &e)
    {
        fail(env, jobj, cls, e.what());
        return 0;
    }
}

extern "C" JNIEXPORT jstring JNICALL
Java_com_capmet_metrics_jmdr_mdr_1category_1t_release_1host
    (JNIEnv *env, jobject jobj, jstring jhost)
{
    try
    {
        const char *host = env->GetStringUTFChars(jhost, 0);

        mdr::mdr_category_t::mdr_release_host(host);
        env->ReleaseStringUTFChars(jhost, host);
    }
    catch(std::exception &e)
    {
        jstring why = env->NewStringUTF(e.what());
        return why;
    }

    return 0;
}

extern "C" JNIEXPORT void JNICALL
Java_com_capmet_metrics_jmdr_mdr_1category_1t_finalize
    (JNIEnv *env, jobject jobj)
{
    jclass cls = env->GetObjectClass(jobj);
    jfieldID fid;
    mdr::mdr_category_t *cat;

    fid = env->GetFieldID(cls, "handle", "J");
    if (fid == 0)
        return;
    cat = (mdr::mdr_category_t *) (unsigned int) env->GetLongField(jobj, fid);
    if (cat == 0)
        return;

    delete cat;

    env->SetIntField(jobj, fid, 0);
}

//
// this function is used by CaptiveCollector
//
extern "C" JNIEXPORT void JNICALL
Java_com_capmet_Captivity_CaptiveCollector_RMIServer_chmod
    (JNIEnv *env, jobject jobj, jstring jpath, jint mode)
{
    const char *path = env->GetStringUTFChars(jpath, 0);

    // this will have to do
    switch(mode) {
    case 0400:
        SetFileAttributes(path,
            GetFileAttributes(path) | FILE_ATTRIBUTE_READONLY);
        break;
    }

    env->ReleaseStringUTFChars(jpath, path);
}
