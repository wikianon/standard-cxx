#include <iostream>
#include <exception>
#include "mdr_perf_counter.h"
#include <jni.h>

static void fail(JNIEnv *env, jobject jobj, jclass cls, const char *msg)
{
    jfieldID reason;
    jstring jstr;

    reason = env->GetFieldID(cls, "reason", "Ljava/lang/String;");
    if (reason == 0)
    {
        std::cerr << "mdr_performance_counter_t::fail: cannot fail" << std::endl;
        return;
    }
    jstr = env->NewStringUTF(msg);
    env->SetObjectField(jobj, reason, jstr);
}

extern "C" JNIEXPORT jboolean JNICALL
Java_com_capmet_metrics_jmdr_mdr_1performance_1counter_1t_perf_1counter_1constructor
    (JNIEnv *env, jobject jobj,
     jstring jcategory, jstring jfield,
     jstring jinstance, jstring jremote_host)
{
    jfieldID fid;
    jclass cls = env->GetObjectClass(jobj);
    const char *category = env->GetStringUTFChars(jcategory, 0);
    const char *field = env->GetStringUTFChars(jfield, 0);
    const char *instance = 0;
    const char *remote_host = 0;
    mdr::mdr_performance_counter_t *perfcnt;

    fid = env->GetFieldID(cls, "handle", "J");
    if (fid == 0)
    {
        env->ReleaseStringUTFChars(jcategory, category);
        env->ReleaseStringUTFChars(jfield, field);
        fail(env, jobj, cls, "mdr_performance_counter_t::mdr_performance_counter_t: cannot find handle");
        return JNI_FALSE;
    }
    try
    {
        if (jinstance != 0)
            instance = env->GetStringUTFChars(jinstance, 0);

        if (jremote_host != 0)
            remote_host = env->GetStringUTFChars(jremote_host, 0);

        perfcnt = new mdr::mdr_performance_counter_t(
            category, field, instance, remote_host);
    }
    catch(std::exception &e)
    {
        fail(env, jobj, cls, e.what());
        return JNI_FALSE;
    }
    env->SetLongField(jobj, fid, (jlong) (unsigned int) perfcnt);

    env->ReleaseStringUTFChars(jcategory, category);
    env->ReleaseStringUTFChars(jfield, field);
    if (instance)
        env->ReleaseStringUTFChars(jinstance, instance);
    if (remote_host)
        env->ReleaseStringUTFChars(jremote_host, remote_host);

    return JNI_TRUE;
}

extern "C" JNIEXPORT void JNICALL
Java_com_capmet_metrics_jmdr_mdr_1performance_1counter_1t_finalize
    (JNIEnv *env, jobject jobj)
{
    jclass cls = env->GetObjectClass(jobj);
    jfieldID fid;
    mdr::mdr_performance_counter_t *perfcnt;

    fid = env->GetFieldID(cls, "handle", "J");
    if (fid == 0)
        return;
    perfcnt = (mdr::mdr_performance_counter_t *) (unsigned int) env->GetLongField(jobj, fid);
    if (perfcnt == 0)
        return;

    delete perfcnt;

    env->SetIntField(jobj, fid, 0);
}

static jobject object_value_of(JNIEnv *env, jobject jobj,
    const mdr::mdr_value_t &value)
{
    jclass cls = env->GetObjectClass(jobj);
    jclass rcls;
    jmethodID id;
    jstring jstr;
    jobject object = 0;

    switch(value.mv_type)
    {
    case mdr::MVT_I32:
    case mdr::MVT_UI32:
#ifndef _LP64
    case mdr::MVT_LONG:
    case mdr::MVT_ULONG:
#endif
        rcls = env->FindClass("java/lang/Integer");
        if (rcls == 0)
        {
            fail(env, jobj, cls, "mdr_performance_counter_t::object_value_of: cannot find java/lang/Integer");
            return 0;
        }
        id = env->GetMethodID(rcls, "<init>", "(I)V");
        if (id == 0)
        {
            fail(env, jobj, cls, "mdr_performance_counter_t::object_value_of: cannot find public Integer(int)");
            return 0;
        }
        object = env->NewObject(rcls, id, value.mv_ui32, 0);
        break;
#ifdef _LP64
    case mdr::MVT_LONG:
    case mdr::MVT_ULONG:
#endif
    case mdr::MVT_I64:
    case mdr::MVT_UI64:
        rcls = env->FindClass("java/lang/Long");
        if (rcls == 0)
        {
            fail(env, jobj, cls, "mdr_performance_counter_t::object_value_of: cannot find java/lang/Long");
            return 0;
        }
        id = env->GetMethodID(rcls, "<init>", "(J)V");
        if (id == 0)
        {
            fail(env, jobj, cls, "mdr_performance_counter_t::object_value_of: cannot find public Long(long)");
            return 0;
        }
        object = env->NewObject(rcls, id, value.mv_ui64, 0);
        break;
    case mdr::MVT_DOUBLE:
        rcls = env->FindClass("java/lang/Double");
        if (rcls == 0)
        {
            fail(env, jobj, cls, "cannot find java/lang/Double");
            return 0;
        }
        id = env->GetMethodID(rcls, "<init>", "(D)V");
        if (id == 0)
        {
            fail(env, jobj, cls, "mdr_performance_counter_t::object_value_of: cannot find public Double(double)");
            return 0;
        }
        object = env->NewObject(rcls, id, value.mv_d, 0);
        break;
    case mdr::MVT_STRING:
        rcls = env->FindClass("java/lang/String");
        if (rcls == 0)
        {
            fail(env, jobj, cls, "mdr_performance_counter_t::object_value_of: cannot find java/lang/String");
            return 0;
        }
        id = env->GetMethodID(rcls, "<init>", "(Ljava/lang/String;)V");
        if (id == 0)
        {
            fail(env, jobj, cls, "mdr_performance_counter_t::object_value_of: cannot find public String(String)");
            return 0;
        }
        jstr = env->NewStringUTF(value.mv_s->c_str());
        object = env->NewObject(rcls, id, jstr, 0);
        break;
    case mdr::MVT_BOOL:
        rcls = env->FindClass("java/lang/Boolean");
        if (rcls == 0)
        {
            fail(env, jobj, cls, "mdr_performance_counter_t::object_value_of: cannot find java/lang/Boolean");
            return 0;
        }
        id = env->GetMethodID(rcls, "<init>", "(Z)V");
        if (id == 0)
        {
            fail(env, jobj, cls, "mdr_performance_counter_t::object_value_of: cannot find public Boolean(boolean)");
            return 0;
        }
        object = env->NewObject(rcls, id, value.mv_b ? JNI_TRUE : JNI_FALSE, 0);
        break;
    default:
        fail(env, jobj, cls, "mdr_performance_counter_t::object_value_of: invalid mdr_value_t");
        break;
    }

    return object;
}

extern "C" JNIEXPORT jobject JNICALL
Java_com_capmet_metrics_jmdr_mdr_1performance_1counter_1t_is_1supported
    (JNIEnv *env, jobject jobj)
{
    jclass cls = env->GetObjectClass(jobj);
    jclass rcls;
    jmethodID id;

    rcls = env->FindClass("java/lang/Boolean");
    if (rcls == 0)
    {
        fail(env, jobj, cls, "mdr_performance_counter_t::is_supported: cannot find java.lang.Boolean");
        return 0;
    }
    id = env->GetMethodID(rcls, "<init>", "(Z)V");
    if (id == 0)
    {
        fail(env, jobj, cls, "mdr_performance_counter_t::is_supported: cannot find Boolean(boolean)");
        return 0;
    }
    return env->NewObject(rcls, id, JNI_TRUE, 0);
}

extern "C" JNIEXPORT jobject JNICALL
Java_com_capmet_metrics_jmdr_mdr_1performance_1counter_1t_perf_1value
    (JNIEnv *env, jobject jobj)
{
    jclass cls = env->GetObjectClass(jobj);
    jfieldID fid;
    mdr::mdr_performance_counter_t *perfcnt;

    fid = env->GetFieldID(cls, "handle", "J");
    if (fid == 0)
    {
        fail(env, jobj, cls, "mdr_performance_counter_t::is_supported: cannot find handle");
        return 0;
    }
    perfcnt = (mdr::mdr_performance_counter_t *) (unsigned int) env->GetLongField(jobj, fid);
    if (perfcnt == 0)
    {
        fail(env, jobj, cls, "mdr_performance_counter_t::is_supported: null mdr_performance_counter_t handle");
        return 0;
    }
    try
    {
        const mdr::mdr_value_t &value = perfcnt->mdr_value();

        return object_value_of(env, cls, value);
    }
    catch(std::exception &e)
    {
        fail(env, jobj, cls, e.what());
        return 0;
    }

    return 0;
}

extern "C" JNIEXPORT jobject JNICALL
Java_com_capmet_metrics_jmdr_mdr_1performance_1counter_1t_perf_1next_1value
    (JNIEnv *env, jobject jobj)
{
    jclass cls = env->GetObjectClass(jobj);
    jfieldID fid;
    mdr::mdr_performance_counter_t *perfcnt;

    fid = env->GetFieldID(cls, "handle", "J");
    if (fid == 0)
    {
        fail(env, jobj, cls, "mdr_performance_counter_t::perf_next_value: cannot find handle");
        return 0;
    }
    perfcnt = (mdr::mdr_performance_counter_t *) (unsigned int) env->GetLongField(jobj, fid);
    if (perfcnt == 0)
    {
        fail(env, jobj, cls, "mdr_performance_counter_t::perf_next_value: null mdr_performance_counter_t handle");
        return 0;
    }
    try
    {
        const mdr::mdr_value_t &value = perfcnt->mdr_next_value();

        return object_value_of(env, cls, value);
    }
    catch(std::exception &e)
    {
        fail(env, jobj, cls, e.what());
        return 0;
    }

    return 0;
}
