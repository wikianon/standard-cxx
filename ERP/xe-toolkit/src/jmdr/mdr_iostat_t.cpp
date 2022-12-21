/****************************************************************************
**
** Copyright (c) 2006-2007 Captive Metrics Software Corporation.
**                    All rights reserved.
**
** This file is part of the jmdr library of Captivity.
**
** This file may be used under the terms of the GNU General Public
** License version 2.0 as published by the Free Software Foundation
** and appearing in the file LICENSE-GPL.txt contained within the
** same package as this file. This software is subject to a
** dual-licensing mechanism, the details of which are outlined in
** file LICENSE-DUAL.txt, also contained within this package. Be sure
** to use the correct license for your needs. To view the commercial
** license, read LICENSE-COMMERCIAL.txt also contained within this
** package.
**
** If you do not have access to these files or are unsure which license
** is appropriate for your use, please contact the sales department at
** sales@captivemetrics.com.
**
** This file is provided AS IS with NO WARRANTY OF ANY KIND, INCLUDING THE
** WARRANTY OF DESIGN, MERCHANTABILITY AND FITNESS FOR A PARTICULAR PURPOSE.
**
****************************************************************************/


#include <iostream>
#include <exception>
#include <mdr_iostat.h>
#include <jni.h>

extern bool mdr_constructor(JNIEnv *, jobject, mdr::mdr_t *);

static void fail(JNIEnv *env, jobject jobj, jclass cls, const char *msg)
{
    jfieldID reason;
    jstring jstr;

    reason = env->GetFieldID(cls, "reason", "Ljava/lang/String;");
    if (reason == 0)
    {
        std::cerr << "mdr_iostat_t::fail: cannot fail" << std::endl;
        return;
    }
    jstr = env->NewStringUTF(msg);
    env->SetObjectField(jobj, reason, jstr);
}

extern "C" JNIEXPORT jboolean JNICALL
Java_com_capmet_metrics_jmdr_mdr_1iostat_1t_iostat_1constructor
    (JNIEnv *env, jobject jobj, jstring jhost)
{
    jclass cls = env->GetObjectClass(jobj);
    jfieldID fid;
    mdr::mdr_iostat_t *iostat;

    fid = env->GetFieldID(cls, "handle", "J");
    if (fid == 0)
    {
        fail(env, jobj, cls, "mdr_iostat_t::mdr_iostat_t: cannot find handle");
        return JNI_FALSE;
    }
    try
    {
        const char *remote_host = 0;

        if (jhost != 0)
            remote_host = env->GetStringUTFChars(jhost, 0);

        iostat = new mdr::mdr_iostat_t(remote_host);

        if (remote_host)
            env->ReleaseStringUTFChars(jhost, remote_host);

        if (!mdr_constructor(env, jobj, iostat))
            return JNI_FALSE;
    }
    catch(std::exception &e)
    {
        fail(env, jobj, cls, e.what());
        return JNI_FALSE;
    }
    env->SetLongField(jobj, fid, (jlong) _POINTER_CAST_ iostat);

    return JNI_TRUE;
}

extern "C" JNIEXPORT void JNICALL
Java_com_capmet_metrics_jmdr_mdr_1iostat_1t_finalize
	(JNIEnv *env, jobject jobj)
{
	jclass cls = env->GetObjectClass(jobj);
	jfieldID fid;
    mdr::mdr_iostat_t *iostat;

	fid = env->GetFieldID(cls, "handle", "J");
	if (fid == 0)
		return;
    iostat = (mdr::mdr_iostat_t *) _POINTER_CAST_ env->GetLongField(jobj, fid);
	if (iostat == 0)
		return;

	delete iostat;

	env->SetIntField(jobj, fid, 0);
}

extern "C" JNIEXPORT jboolean JNICALL
Java_com_capmet_metrics_jmdr_mdr_1iostat_1t_iostat_1refresh
	(JNIEnv *env, jobject jobj)
{
    jclass cls = env->GetObjectClass(jobj);
    jfieldID fid;
    mdr::mdr_iostat_t *iostat;

    fid = env->GetFieldID(cls, "handle", "J");
    if (fid == 0)
    {
        fail(env, jobj, cls, "mdr_iostat_t::iostat_refresh: cannot find handle");
        return JNI_FALSE;
    }
    iostat = (mdr::mdr_iostat_t *) _POINTER_CAST_ env->GetLongField(jobj, fid);
    if (iostat == 0)
    {
        fail(env, jobj, cls, "mdr_iostat_t::iostat_refresh: null mdr_iostat_t handle");
        return JNI_FALSE;
    }
    try
    {
        iostat->mdr_refresh();
    }
    catch(std::exception &e)
    {
        fail(env, jobj, cls, e.what());
        return JNI_FALSE;
    }

    return JNI_TRUE;
}

static bool iostat_set
    (JNIEnv *env, jobject jobj, jobject jcan,
        mdr::md_can_t &can, jint field_number)
{
    jclass cls = env->GetObjectClass(jobj);
    jclass ccls = env->GetObjectClass(jcan);
    jfieldID fid;
    jstring jstr;

    switch(field_number)
    {
    case mdr::MD_TYPE:
        //
        // md_type
        //
        fid = env->GetFieldID(ccls, "md_type", "Ljava/lang/String;");
        if (fid == 0)
        {
            fail(env, jobj, cls, "mdr_iostat_t::iostat_set: cannot find md_type");
            return false;
        }
        jstr = env->NewStringUTF(can.md_type.c_str());
        env->SetObjectField(jcan, fid, jstr);
        break;
    case mdr::MD_READS:
        //
        // md_reads
        //
        fid = env->GetFieldID(ccls, "md_reads", "I");
        if (fid == 0)
        {
            fail(env, jobj, cls, "mdr_iostat_t::iostat_set: cannot find md_reads");
            return false;
        }
        env->SetIntField(jcan, fid, (int) can.md_reads);
        break;
    case mdr::MD_READ_K:
        //
        // md_read_K
        //
        fid = env->GetFieldID(ccls, "md_read_K", "I");
        if (fid == 0)
        {
            fail(env, jobj, cls, "mdr_iostat_t::iostat_set: cannot find md_read_K");
            return false;
        }
        env->SetIntField(jcan, fid, (int) can.md_read_K);
        break;
    case mdr::MD_READ_MSECS:
        //
        // md_read_msecs
        //
        fid = env->GetFieldID(ccls, "md_read_msecs", "I");
        if (fid == 0)
        {
            fail(env, jobj, cls, "mdr_iostat_t::iostat_set: cannot find md_read_msecs");
            return false;
        }
        env->SetIntField(jcan, fid, (int) can.md_read_msecs);
        break;
    case mdr::MD_WRITES:
        //
        // md_writes
        //
        fid = env->GetFieldID(ccls, "md_writes", "I");
        if (fid == 0)
        {
            fail(env, jobj, cls, "mdr_iostat_t::iostat_set: cannot find md_writes");
            return false;
        }
        env->SetIntField(jcan, fid, (int) can.md_writes);
        break;
    case mdr::MD_WRITTEN_K:
        //
        // md_written_K
        //
        fid = env->GetFieldID(ccls, "md_written_K", "I");
        if (fid == 0)
        {
            fail(env, jobj, cls, "mdr_iostat_t::iostat_set: cannot find md_written_K");
            return false;
        }
        env->SetIntField(jcan, fid, (int) can.md_written_K);
        break;
    case mdr::MD_WRITE_MSECS:
        //
        // md_write_msecs
        //
        fid = env->GetFieldID(ccls, "md_write_msecs", "I");
        if (fid == 0)
        {
            fail(env, jobj, cls, "mdr_iostat_t::iostat_set: cannot find md_write_msecs");
            return false;
        }
        env->SetIntField(jcan, fid, (int) can.md_write_msecs);
        break;
    case mdr::MD_RUN_MSECS:
        //
        // md_run_msecs
        //
        fid = env->GetFieldID(ccls, "md_run_msecs", "I");
        if (fid == 0)
        {
            fail(env, jobj, cls, "mdr_iostat_t::iostat_set: cannot find md_run_msecs");
            return false;
        }
        env->SetIntField(jcan, fid, (int) can.md_run_msecs);
        break;
    case mdr::MD_WAIT_MSECS:
        //
        // md_wait_msecs
        //
        fid = env->GetFieldID(ccls, "md_wait_msecs", "I");
        if (fid == 0)
        {
            fail(env, jobj, cls, "mdr_iostat_t::iostat_set: cannot find md_wait_msecs");
            return false;
        }
        env->SetIntField(jcan, fid, (int) can.md_wait_msecs);
        break;
    case mdr::MD_RUNQ_MSECS:
        //
        // md_runq_msecs
        //
        fid = env->GetFieldID(ccls, "md_runq_msecs", "I");
        if (fid == 0)
        {
            fail(env, jobj, cls, "mdr_iostat_t::iostat_set: cannot find md_runq_msecs");
            return false;
        }
        env->SetIntField(jcan, fid, (int) can.md_runq_msecs);
        break;
    case mdr::MD_WAITQ_MSECS:
        //
        // md_waitq_msecs
        //
        fid = env->GetFieldID(ccls, "md_waitq_msecs", "I");
        if (fid == 0)
        {
            fail(env, jobj, cls, "mdr_iostat_t::iostat_set: cannot find md_waitq_msecs");
            return false;
        }
        env->SetIntField(jcan, fid, (int) can.md_waitq_msecs);
        break;
    case mdr::MD_AVG_QUEUE_LENGTH:
        //
        // md_avg_queue_length
        //
        fid = env->GetFieldID(ccls, "md_avg_queue_length", "D");
        if (fid == 0)
        {
            fail(env, jobj, cls, "mdr_iostat_t::iostat_set: cannot find md_avg_queue_length");
            return false;
        }
        env->SetDoubleField(jcan, fid, can.md_avg_queue_length);
        break;
    case mdr::MD_UTIL_PERCENT:
        //
        // md_util_percent
        //
        fid = env->GetFieldID(ccls, "md_util_percent", "D");
        if (fid == 0)
        {
            fail(env, jobj, cls, "mdr_iostat_t::iostat_set: cannot find md_util_percent");
            return false;
        }
        env->SetDoubleField(jcan, fid, can.md_util_percent);
        break;
    case mdr::MD_SERVICE_TIME:
        //
        // md_service_time
        //
        fid = env->GetFieldID(ccls, "md_service_time", "D");
        if (fid == 0)
        {
            fail(env, jobj, cls, "mdr_iostat_t::iostat_set: cannot find md_service_time");
            return false;
        }
        env->SetDoubleField(jcan, fid, can.md_service_time);
        break;
    }

    return true;
}

static bool iostat_set
    (JNIEnv *env, jobject jobj, jobject jcan, mdr::md_can_t &can)
{
    return (
            iostat_set(env, jobj, jcan, can, mdr::MD_TYPE) &&
            iostat_set(env, jobj, jcan, can, mdr::MD_READS) &&
            iostat_set(env, jobj, jcan, can, mdr::MD_READ_K) &&
            iostat_set(env, jobj, jcan, can, mdr::MD_READ_MSECS) &&
            iostat_set(env, jobj, jcan, can, mdr::MD_WRITES) &&
            iostat_set(env, jobj, jcan, can, mdr::MD_WRITTEN_K) &&
            iostat_set(env, jobj, jcan, can, mdr::MD_WRITE_MSECS) &&
            iostat_set(env, jobj, jcan, can, mdr::MD_RUN_MSECS) &&
            iostat_set(env, jobj, jcan, can, mdr::MD_WAIT_MSECS) &&
            iostat_set(env, jobj, jcan, can, mdr::MD_RUNQ_MSECS) &&
            iostat_set(env, jobj, jcan, can, mdr::MD_WAITQ_MSECS) &&
            iostat_set(env, jobj, jcan, can, mdr::MD_AVG_QUEUE_LENGTH) &&
            iostat_set(env, jobj, jcan, can, mdr::MD_UTIL_PERCENT) &&
            iostat_set(env, jobj, jcan, can, mdr::MD_SERVICE_TIME));
}

extern "C" JNIEXPORT jint JNICALL
Java_com_capmet_metrics_jmdr_mdr_1iostat_1t_mdr_1size
	(JNIEnv *env, jobject jobj)
{
    jclass cls = env->GetObjectClass(jobj);
    jfieldID fid;
    mdr::mdr_iostat_t *iostat;

    // find handle
    fid = env->GetFieldID(cls, "handle", "J");
    if (fid == 0)
    {
        fail(env, jobj, cls, "mdr_iostat_t::mdr_size: cannot find handle");
        return JNI_FALSE;
    }

    // get the iostat pointer
    iostat = (mdr::mdr_iostat_t *) _POINTER_CAST_ env->GetLongField(jobj, fid);
    if (iostat == 0)
    {
        fail(env, jobj, cls, "mdr_iostat_t::mdr_size: null mdr_iostat_t handle");
        return JNI_FALSE;
    }

    return iostat->size();
}

extern "C" JNIEXPORT jobjectArray JNICALL
Java_com_capmet_metrics_jmdr_mdr_1iostat_1t_mdr_1keyset
	(JNIEnv *env, jobject jobj)
{
    jclass cls = env->GetObjectClass(jobj);
    jfieldID fid;
    mdr::mdr_iostat_t *iostat;
    jobjectArray jarray;

    // find handle
    fid = env->GetFieldID(cls, "handle", "J");
    if (fid == 0)
    {
        fail(env, jobj, cls, "mdr_iostat_t::mdr_keyset: cannot find handle");
        return JNI_FALSE;
    }

    // get the iostat pointer
    iostat = (mdr::mdr_iostat_t *) _POINTER_CAST_ env->GetLongField(jobj, fid);
    if (iostat == 0)
    {
        fail(env, jobj, cls, "mdr_iostat_t::mdr_keyset: null mdr_iostat_t handle");
        return JNI_FALSE;
    }

    // retrieve the keys
    mdr::mdr_keyset_t keyset;
    mdr::mdr_key_t kn;
    int i;

    try
    {
        iostat->mdr_keyset(keyset);
    }
    catch(std::exception &e)
    {
        fail(env, jobj, cls, e.what());
        return JNI_FALSE;
    }

    // create a new array object of type String[]
    jarray =
        env->NewObjectArray(keyset.size(), env->FindClass("java/lang/String"),
                            env->NewStringUTF(""));

    // populate
    for (i = 0, kn = keyset.begin(); kn != keyset.end(); kn++, i++)
    {
        jstring jstr = env->NewStringUTF((*kn).c_str());
        env->SetObjectArrayElement(jarray, i, jstr);
    }

    // bye
    return jarray;
}

extern "C" JNIEXPORT jboolean JNICALL
Java_com_capmet_metrics_jmdr_mdr_1iostat_1t_mdr_1get
    (JNIEnv *env, jobject jobj, jstring jkey, jobject jcan)
{
    jclass cls = env->GetObjectClass(jobj);
    jfieldID fid;
    mdr::mdr_iostat_t *iostat;

    // find handle
    fid = env->GetFieldID(cls, "handle", "J");
    if (fid == 0)
    {
        fail(env, jobj, cls, "mdr_iostat_t::mdr_get: cannot find handle");
        return JNI_FALSE;
    }

    // get the iostat pointer
    iostat = (mdr::mdr_iostat_t *) _POINTER_CAST_ env->GetLongField(jobj, fid);
    if (iostat == 0)
    {
        fail(env, jobj, cls, "mdr_iostat_t::mdr_get: null mdr_iostat_t handle");
        return JNI_FALSE;
    }

    // retrive the key and perform lookup
    const char *key;
    mdr::md_node_t node;

    try
    {
        key = env->GetStringUTFChars(jkey, 0);
        node = iostat->find(key);
        env->ReleaseStringUTFChars(jkey, key);
    }
    catch(std::exception &e)
    {
        fail(env, jobj, cls, e.what());
        return JNI_FALSE;
    }

    // not there
    if (node == iostat->end())
        return JNI_FALSE;

    // now assign values from can to can
    return (iostat_set(env, jobj, jcan, node->second) ? JNI_TRUE : JNI_FALSE);
}

extern "C" JNIEXPORT jboolean JNICALL
Java_com_capmet_metrics_jmdr_mdr_1iostat_1t_mdr_1get_1field
    (JNIEnv *env, jobject jobj, jstring jkey, jobject jcan, jint field_number)
{
    jclass cls = env->GetObjectClass(jobj);
    jfieldID fid;
    mdr::mdr_iostat_t *iostat;

    // find handle
    fid = env->GetFieldID(cls, "handle", "J");
    if (fid == 0)
    {
        fail(env, jobj, cls, "mdr_iostat_t::mdr_get_field: cannot find handle");
        return JNI_FALSE;
    }

    // get the iostat pointer
    iostat = (mdr::mdr_iostat_t *) _POINTER_CAST_ env->GetLongField(jobj, fid);
    if (iostat == 0)
    {
        fail(env, jobj, cls, "mdr_iostat_t::mdr_get_field: null mdr_iostat_t handle");
        return JNI_FALSE;
    }

    // retrive the key and perform lookup
    const char *key;
    mdr::md_node_t node;

    try
    {
        key = env->GetStringUTFChars(jkey, 0);
        node = iostat->find(key);
        env->ReleaseStringUTFChars(jkey, key);
    }
    catch(std::exception &e)
    {
        fail(env, jobj, cls, e.what());
        return JNI_FALSE;
    }

    // not there
    if (node == iostat->end())
        return JNI_FALSE;

    // now assign values from can to can
    return (iostat_set(env, jobj, jcan, node->second, field_number) ? JNI_TRUE : JNI_FALSE);
}
