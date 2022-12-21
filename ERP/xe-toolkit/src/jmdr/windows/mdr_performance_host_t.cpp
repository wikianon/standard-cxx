#include <iostream>
#include <exception>
#include <jni.h>
#include "mdr_tuple.h"
#include "mdr_perf_host.h"

// XXX: gak: lifted from mdr_perf_host.h
enum mdr_os_flavor_t {
    MOS_LINUX     = 1,
    MOS_SOLARIS   = 2,
    MOS_AIX       = 4,
    MOS_HPUX      = 8,
    MOS_MACOS     = 16,
    MOS_BSD       = 32,
    MOS_WINDOWS   = 64
};

static void fail(JNIEnv *env, jobject jobj, jclass cls, const char *msg)
{
    jfieldID reason;
    jstring jstr;

    reason = env->GetFieldID(cls, "reason", "Ljava/lang/String;");
    if (reason == 0)
    {
        std::cerr << "mdr_performance_host_t::fail: cannot fail" << std::endl;
        return;
    }
    jstr = env->NewStringUTF(msg);
    env->SetObjectField(jobj, reason, jstr);
}

extern "C" JNIEXPORT jboolean JNICALL
Java_com_capmet_metrics_jmdr_mdr_1performance_1host_1t_cat_1constructor
    (JNIEnv *env, jobject jobj, jstring jhost)
{
    jclass cls = env->GetObjectClass(jobj);
    jfieldID fid;
    mdr::mdr_performance_host_t *host;

    fid = env->GetFieldID(cls, "handle", "J");
    if (fid == 0)
    {
        fail(env, jobj, cls, "mdr_performance_host_t::mdr_performance_host_t: cannot find handle");
        return JNI_FALSE;
    }
    try
    {
        const char *remote_host = 0;

        if (jhost != 0)
            remote_host = env->GetStringUTFChars(jhost, 0);

        host = new mdr::mdr_performance_host_t(remote_host);

        if (remote_host)
            env->ReleaseStringUTFChars(jhost, remote_host);
    }
    catch(std::exception &e)
    {
        fail(env, jobj, cls, e.what());
        return JNI_FALSE;
    }
    env->SetLongField(jobj, fid, (jlong) (unsigned int) host);

    fid = env->GetFieldID(cls, "os_flavor", "I");
    if (fid == 0)
    {
        fail(env, jobj, cls, "mdr_performance_host_t::mdr_performance_host_t: cannot find os_flavor");
        return JNI_FALSE;
    }
    env->SetIntField(jobj, fid, MOS_WINDOWS);

    return JNI_TRUE;
}

extern "C" JNIEXPORT void JNICALL
Java_com_capmet_metrics_jmdr_mdr_1performance_1host_1t_finalize
  (JNIEnv *env, jobject jobj)
{
    jclass cls = env->GetObjectClass(jobj);
    jfieldID fid;
    mdr::mdr_performance_host_t *host;

    fid = env->GetFieldID(cls, "handle", "J");
    if (fid == 0)
        return;
    host = (mdr::mdr_performance_host_t *) (unsigned int) env->GetLongField(jobj, fid);
    if (host == 0)
        return;

    delete host;

    env->SetIntField(jobj, fid, 0);
}

static int utf8seqlen(const unsigned char *utf8s)
{
  static const unsigned char seqLens[64] = {
    1, 1, 1, 1, 1, 1, 1, 1,
    1, 1, 1, 1, 1, 1, 1, 1,
    1, 1, 1, 1, 1, 1, 1, 1,
    1, 1, 1, 1, 1, 1, 1, 1,
    0, 0, 0, 0, 0, 0, 0, 0,
    0, 0, 0, 0, 0, 0, 0, 0,
    2, 2, 2, 2, 2, 2, 2, 2,
    3, 3, 3, 3, 4, 4, 5, 6
  };
  return (int) seqLens[*utf8s >> 2];
}

static int utf8strlen(const char *utf8s)
{
  const char *s = utf8s;
  int length = 0;

  while(*s != '\0') {
    int sl = utf8seqlen((const unsigned char *) s);
    if (sl == 0)
      break;
    s += sl;
    length++;
  }
  return length;
}

extern "C" JNIEXPORT jobjectArray JNICALL
Java_com_capmet_metrics_jmdr_mdr_1performance_1host_1t_get_1category_1names
  (JNIEnv *env, jobject jobj)
{
    jclass cls = env->GetObjectClass(jobj);
    jfieldID fid;
    mdr::mdr_performance_host_t *host;

    fid = env->GetFieldID(cls, "handle", "J");
    if (fid == 0)
    {
        fail(env, jobj, cls, "mdr_performance_host_t::get_category_names: cannot find handle");
        return 0;
    }
    host = (mdr::mdr_performance_host_t *) (unsigned int) env->GetLongField(jobj, fid);
    if (host == 0)
    {
        fail(env, jobj, cls, "mdr_performance_host_t::get_category_names: null mdr_performance_host_t handle");
        return 0;
    }

    try
    {
        const mdr::mdr_keyset_t &names = host->mdr_get_category_names();
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
Java_com_capmet_metrics_jmdr_mdr_1performance_1host_1t_localized_1name
  (JNIEnv *env, jobject jobj, jstring jen_us_name)
{
    jclass cls = env->GetObjectClass(jobj);
    jfieldID fid;
    mdr::mdr_performance_host_t *host;

    fid = env->GetFieldID(cls, "handle", "J");
    if (fid == 0)
    {
        fail(env, jobj, cls, "mdr_performance_host_t::localized_name: cannot find handle");
        return 0;
    }
    host = (mdr::mdr_performance_host_t *) (unsigned int) env->GetLongField(jobj, fid);
    if (host == 0)
    {
        fail(env, jobj, cls, "mdr_performance_host_t::localized_name: null mdr_performance_host_t handle");
        return 0;
    }

    const char *en_us_name = env->GetStringUTFChars(jen_us_name, 0);
    std::string us(en_us_name);
    std::string local;

    env->ReleaseStringUTFChars(jen_us_name, en_us_name);

    try
    {
        host->mdr_localized_name(us, local);
    }
    catch(std::exception &e)
    {
        fail(env, jobj, cls, e.what());
        return 0;
    }

    return env->NewStringUTF(local.c_str());
}

extern "C" JNIEXPORT jstring JNICALL
Java_com_capmet_metrics_jmdr_mdr_1performance_1host_1t_en_1us_1name
  (JNIEnv *env, jobject jobj, jstring jlocalized_name)
{
    jclass cls = env->GetObjectClass(jobj);
    jfieldID fid;
    mdr::mdr_performance_host_t *host;

    fid = env->GetFieldID(cls, "handle", "J");
    if (fid == 0)
    {
        fail(env, jobj, cls, "mdr_performance_host_t::en_us_name: cannot find handle");
        return 0;
    }
    host = (mdr::mdr_performance_host_t *) (unsigned int) env->GetLongField(jobj, fid);
    if (host == 0)
    {
        fail(env, jobj, cls, "mdr_performance_host_t::en_us_name: null mdr_performance_host_t handle");
        return 0;
    }

    const char *local_name = env->GetStringUTFChars(jlocalized_name, 0);
    std::string us;
    std::string local(local_name);

    env->ReleaseStringUTFChars(jlocalized_name, local_name);

    try
    {
        host->mdr_en_us_name(local, us);
    }
    catch(std::exception &e)
    {
        fail(env, jobj, cls, e.what());
        return 0;
    }

    return env->NewStringUTF(us.c_str());
}
