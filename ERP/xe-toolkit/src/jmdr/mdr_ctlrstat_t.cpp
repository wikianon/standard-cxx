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
#include <mdr_ctlrstat.h>
#include <jni.h>

extern bool mdr_constructor(JNIEnv *, jobject, mdr::mdr_t *);

static void fail(JNIEnv *env, jobject jobj, jclass cls, const char *msg)
{
    jfieldID reason;
    jstring jstr;

    reason = env->GetFieldID(cls, "reason", "Ljava/lang/String;");
    if (reason == 0)
    {
        std::cerr << "mdr_ctlrstat_t::fail: cannot fail" << std::endl;
        return;
    }
    jstr = env->NewStringUTF(msg);
    env->SetObjectField(jobj, reason, jstr);
}

extern "C" JNIEXPORT jboolean JNICALL
Java_com_capmet_metrics_jmdr_mdr_1ctlrstat_1t_ctlrstat_1constructor
    (JNIEnv *env, jobject jobj, jstring jhost)
{
    jclass cls = env->GetObjectClass(jobj);
    jfieldID fid;
    mdr::mdr_ctlrstat_t *ctlr;

    fid = env->GetFieldID(cls, "handle", "J");
    if (fid == 0)
    {
        fail(env, jobj, cls, "mdr_ctlrstat_t::mdr_ctlrstat_t: cannot find handle");
        return JNI_FALSE;
    }
    try
    {
        const char *remote_host = 0;

        if (jhost != 0)
            remote_host = env->GetStringUTFChars(jhost, 0);

        ctlr = new mdr::mdr_ctlrstat_t(remote_host);

        if (remote_host)
            env->ReleaseStringUTFChars(jhost, remote_host);

        if (!mdr_constructor(env, jobj, ctlr))
            return JNI_FALSE;
    }
    catch(std::exception &e)
    {
        fail(env, jobj, cls, e.what());
        return JNI_FALSE;
    }
    env->SetLongField(jobj, fid, (jlong) _POINTER_CAST_ ctlr);

    return JNI_TRUE;
}

extern "C" JNIEXPORT void JNICALL
Java_com_capmet_metrics_jmdr_mdr_1ctlrstat_1t_finalize
	(JNIEnv *env, jobject jobj)
{
    jclass cls = env->GetObjectClass(jobj);
    jfieldID fid;
    mdr::mdr_ctlrstat_t *ctlr;

    fid = env->GetFieldID(cls, "handle", "J");
    if (fid == 0)
        return;
    ctlr = (mdr::mdr_ctlrstat_t *) _POINTER_CAST_ env->GetLongField(jobj, fid);
    if (ctlr == 0)
        return;

	delete ctlr;

	env->SetIntField(jobj, fid, 0);
}

extern "C" JNIEXPORT jboolean JNICALL
Java_com_capmet_metrics_jmdr_mdr_1ctlrstat_1t_ctlrstat_1refresh
	(JNIEnv *env, jobject jobj)
{
    jclass cls = env->GetObjectClass(jobj);
    jfieldID fid;
    mdr::mdr_ctlrstat_t *ctlr;

    fid = env->GetFieldID(cls, "handle", "J");
    if (fid == 0)
    {
        fail(env, jobj, cls, "mdr_ctlrstat_t::ctlrstat_refresh: cannot find handle");
        return JNI_FALSE;
    }
    ctlr = (mdr::mdr_ctlrstat_t *) _POINTER_CAST_ env->GetLongField(jobj, fid);
    if (ctlr == 0)
    {
        fail(env, jobj, cls, "mdr_ctlrstat_t::ctlrstat_refresh: null mdr_ctlrstat_t handle");
        return JNI_FALSE;
    }
    try
    {
        ctlr->mdr_refresh();
    }
    catch(std::exception &e)
    {
        fail(env, jobj, cls, e.what());
        return JNI_FALSE;
    }

    return JNI_TRUE;
}

static bool ctlrstat_set
    (JNIEnv *env, jobject jobj, jobject jcan,
        mdr::mctl_can_t &can, jint field_number)
{
    jclass cls = env->GetObjectClass(jobj);
    jclass ccls = env->GetObjectClass(jcan);
    jfieldID fid;

    switch(field_number)
    {
    case mdr::MCTL_READS:
        //
        // mctl_reads
        //
        fid = env->GetFieldID(ccls, "mctl_reads", "I");
        if (fid == 0)
        {
            fail(env, jobj, cls, "mdr_ctlrstat_t::ctlrstat_set: cannot find mctl_reads");
            return false;
        }
        env->SetIntField(jcan, fid, (int) can.mctl_reads);
        break;
    case mdr::MCTL_READ_K:
        //
        // mctl_read_K
        //
        fid = env->GetFieldID(ccls, "mctl_read_K", "I");
        if (fid == 0)
        {
            fail(env, jobj, cls, "mdr_ctlrstat_t::ctlrstat_set: cannot find mctl_read_K");
            return false;
        }
        env->SetIntField(jcan, fid, (int) can.mctl_read_K);
        break;
    case mdr::MCTL_READ_MSECS:
        //
        // mctl_read_msecs
        //
        fid = env->GetFieldID(ccls, "mctl_read_msecs", "I");
        if (fid == 0)
        {
            fail(env, jobj, cls, "mdr_ctlrstat_t::ctlrstat_set: cannot find mctl_read_msecs");
            return false;
        }
        env->SetIntField(jcan, fid, (int) can.mctl_read_msecs);
        break;
    case mdr::MCTL_WRITES:
        //
        // mctl_writes
        //
        fid = env->GetFieldID(ccls, "mctl_writes", "I");
        if (fid == 0)
        {
            fail(env, jobj, cls, "mdr_ctlrstat_t::ctlrstat_set: cannot find mctl_writes");
            return false;
        }
        env->SetIntField(jcan, fid, (int) can.mctl_writes);
        break;
    case mdr::MCTL_WRITTEN_K:
        //
        // mctl_written_K
        //
        fid = env->GetFieldID(ccls, "mctl_written_K", "I");
        if (fid == 0)
        {
            fail(env, jobj, cls, "mdr_ctlrstat_t::ctlrstat_set: cannot find mctl_written_K");
            return false;
        }
        env->SetIntField(jcan, fid, (int) can.mctl_written_K);
        break;
    case mdr::MCTL_WRITE_MSECS:
        //
        // mctl_write_msecs
        //
        fid = env->GetFieldID(ccls, "mctl_write_msecs", "I");
        if (fid == 0)
        {
            fail(env, jobj, cls, "mdr_ctlrstat_t::ctlrstat_set: cannot find mctl_write_msecs");
            return false;
        }
        env->SetIntField(jcan, fid, (int) can.mctl_write_msecs);
        break;
    case mdr::MCTL_RUN_MSECS:
        //
        // mctl_run_msecs
        //
        fid = env->GetFieldID(ccls, "mctl_run_msecs", "I");
        if (fid == 0)
        {
            fail(env, jobj, cls, "mdr_ctlrstat_t::ctlrstat_set: cannot find mctl_run_msecs");
            return false;
        }
        env->SetIntField(jcan, fid, (int) can.mctl_run_msecs);
        break;
    case mdr::MCTL_WAIT_MSECS:
        //
        // mctl_wait_msecs
        //
        fid = env->GetFieldID(ccls, "mctl_wait_msecs", "I");
        if (fid == 0)
        {
            fail(env, jobj, cls, "mdr_ctlrstat_t::ctlrstat_set: cannot find mctl_wait_msecs");
            return false;
        }
        env->SetIntField(jcan, fid, (int) can.mctl_wait_msecs);
        break;
    case mdr::MCTL_RUNQ_MSECS:
        //
        // mctl_runq_msecs
        //
        fid = env->GetFieldID(ccls, "mctl_runq_msecs", "I");
        if (fid == 0)
        {
            fail(env, jobj, cls, "mdr_ctlrstat_t::ctlrstat_set: cannot find mctl_runq_msecs");
            return false;
        }
        env->SetIntField(jcan, fid, (int) can.mctl_runq_msecs);
        break;
    case mdr::MCTL_WAITQ_MSECS:
        //
        // mctl_waitq_msecs
        //
        fid = env->GetFieldID(ccls, "mctl_waitq_msecs", "I");
        if (fid == 0)
        {
            fail(env, jobj, cls, "mdr_ctlrstat_t::ctlrstat_set: cannot find mctl_waitq_msecs");
            return false;
        }
        env->SetIntField(jcan, fid, (int) can.mctl_waitq_msecs);
        break;
    case mdr::MCTL_AVG_QUEUE_LENGTH:
        //
        // mctl_avg_queue_length
        //
        fid = env->GetFieldID(ccls, "mctl_avg_queue_length", "D");
        if (fid == 0)
        {
            fail(env, jobj, cls, "mdr_ctlrstat_t::ctlrstat_set: cannot find mctl_avg_queue_length");
            return false;
        }
        env->SetDoubleField(jcan, fid, can.mctl_avg_queue_length);
        break;
    case mdr::MCTL_UTIL_PERCENT:
        //
        // mctl_util_percent
        //
        fid = env->GetFieldID(ccls, "mctl_util_percent", "D");
        if (fid == 0)
        {
            fail(env, jobj, cls, "mdr_ctlrstat_t::ctlrstat_set: cannot find mctl_util_percent");
            return false;
        }
        env->SetDoubleField(jcan, fid, can.mctl_util_percent);
        break;
    case mdr::MCTL_SERVICE_TIME:
        //
        // mctl_service_time
        //
        fid = env->GetFieldID(ccls, "mctl_service_time", "D");
        if (fid == 0)
        {
            fail(env, jobj, cls, "mdr_ctlrstat_t::ctlrstat_set: cannot find mctl_service_time");
            return false;
        }
        env->SetDoubleField(jcan, fid, can.mctl_service_time);
        break;
    case mdr::MCTL_DISK_COUNT:
        //
        // mctl_disk_count
        //
        fid = env->GetFieldID(ccls, "mctl_disk_count", "I");
        if (fid == 0)
        {
            fail(env, jobj, cls, "mdr_ctlrstat_t::ctlrstat_set: cannot find mctl_disk_count");
            return false;
        }
        env->SetIntField(jcan, fid, (int) can.mctl_disk_count);
        break;
    }

    return true;
}

static bool ctlrstat_set
    (JNIEnv *env, jobject jobj, jobject jcan, mdr::mctl_can_t &can)
{
    return(
        ctlrstat_set(env, jobj, jcan, can, mdr::MCTL_READS) &&
        ctlrstat_set(env, jobj, jcan, can, mdr::MCTL_READ_K) &&
        ctlrstat_set(env, jobj, jcan, can, mdr::MCTL_READ_MSECS) &&
        ctlrstat_set(env, jobj, jcan, can, mdr::MCTL_WRITES) &&
        ctlrstat_set(env, jobj, jcan, can, mdr::MCTL_WRITTEN_K) &&
        ctlrstat_set(env, jobj, jcan, can, mdr::MCTL_WRITE_MSECS) &&
        ctlrstat_set(env, jobj, jcan, can, mdr::MCTL_RUN_MSECS) &&
        ctlrstat_set(env, jobj, jcan, can, mdr::MCTL_WAIT_MSECS) &&
        ctlrstat_set(env, jobj, jcan, can, mdr::MCTL_RUNQ_MSECS) &&
        ctlrstat_set(env, jobj, jcan, can, mdr::MCTL_WAITQ_MSECS) &&
        ctlrstat_set(env, jobj, jcan, can, mdr::MCTL_AVG_QUEUE_LENGTH) &&
        ctlrstat_set(env, jobj, jcan, can, mdr::MCTL_UTIL_PERCENT) &&
        ctlrstat_set(env, jobj, jcan, can, mdr::MCTL_SERVICE_TIME) &&
        ctlrstat_set(env, jobj, jcan, can, mdr::MCTL_DISK_COUNT));
}

extern "C" JNIEXPORT jint JNICALL
Java_com_capmet_metrics_jmdr_mdr_1ctlrstat_1t_mdr_1size
	(JNIEnv *env, jobject jobj)
{
    jclass cls = env->GetObjectClass(jobj);
    jfieldID fid;
    mdr::mdr_ctlrstat_t *ctlr;

    // find handle
    fid = env->GetFieldID(cls, "handle", "J");
    if (fid == 0)
    {
        fail(env, jobj, cls, "mdr_ctlrstat_t::mdr_size: cannot find handle");
        return JNI_FALSE;
    }

    // get the ctlr pointer
    ctlr = (mdr::mdr_ctlrstat_t *) _POINTER_CAST_ env->GetLongField(jobj, fid);
    if (ctlr == 0)
    {
        fail(env, jobj, cls, "mdr_ctlrstat_t::mdr_size: null mdr_ctlrstat_t handle");
        return JNI_FALSE;
    }

    return ctlr->size();
}

extern "C" JNIEXPORT jobjectArray JNICALL
Java_com_capmet_metrics_jmdr_mdr_1ctlrstat_1t_mdr_1keyset
	(JNIEnv *env, jobject jobj)
{
    jclass cls = env->GetObjectClass(jobj);
    jfieldID fid;
    mdr::mdr_ctlrstat_t *ctlr;
    jobjectArray jarray;

    // find handle
    fid = env->GetFieldID(cls, "handle", "J");
    if (fid == 0)
    {
        fail(env, jobj, cls, "mdr_ctlrstat_t::mdr_keyset: cannot find handle");
        return JNI_FALSE;
    }

    // get the ctlr pointer
    ctlr = (mdr::mdr_ctlrstat_t *) _POINTER_CAST_ env->GetLongField(jobj, fid);
    if (ctlr == 0)
    {
        fail(env, jobj, cls, "mdr_ctlrstat_t::mdr_keyset: null mdr_ctlrstat_t handle");
        return JNI_FALSE;
    }

    // retrieve the keys
    mdr::mdr_keyset_t keyset;
    mdr::mdr_key_t kn;
    int i;

    try
    {
        ctlr->mdr_keyset(keyset);
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
Java_com_capmet_metrics_jmdr_mdr_1ctlrstat_1t_mdr_1get
    (JNIEnv *env, jobject jobj, jstring jkey, jobject jcan)
{
    jclass cls = env->GetObjectClass(jobj);
    jfieldID fid;
    mdr::mdr_ctlrstat_t *ctlr;

    // find handle
    fid = env->GetFieldID(cls, "handle", "J");
    if (fid == 0)
    {
        fail(env, jobj, cls, "mdr_ctlrstat_t::mdr_get: cannot find handle");
        return JNI_FALSE;
    }

    // get the ctlr pointer
    ctlr = (mdr::mdr_ctlrstat_t *) _POINTER_CAST_ env->GetLongField(jobj, fid);
    if (ctlr == 0)
    {
        fail(env, jobj, cls, "mdr_ctlrstat_t::mdr_get: null mdr_ctlrstat_t handle");
        return JNI_FALSE;
    }

    // retrive the key and perform lookup
    const char *key;
    mdr::mctl_node_t node;

    try
    {
        key = env->GetStringUTFChars(jkey, 0);
        node = ctlr->find(key);
        env->ReleaseStringUTFChars(jkey, key);
    }
    catch(std::exception &e)
    {
        fail(env, jobj, cls, e.what());
        return JNI_FALSE;
    }

    // not there
    if (node == ctlr->end())
        return JNI_FALSE;

    // now assign values from can to can
    return (ctlrstat_set(env, jobj, jcan, node->second) ? JNI_TRUE : JNI_FALSE);
}

extern "C" JNIEXPORT jboolean JNICALL
Java_com_capmet_metrics_jmdr_mdr_1ctlrstat_1t_mdr_1get_1field
    (JNIEnv *env, jobject jobj, jstring jkey, jobject jcan, jint field_number)
{
    jclass cls = env->GetObjectClass(jobj);
    jfieldID fid;
    mdr::mdr_ctlrstat_t *ctlrstat;

    // find handle
    fid = env->GetFieldID(cls, "handle", "J");
    if (fid == 0)
    {
        fail(env, jobj, cls, "mdr_ctlrstat_t::mdr_get_field: cannot find handle");
        return JNI_FALSE;
    }

    // get the ctlrstat pointer
    ctlrstat = (mdr::mdr_ctlrstat_t *) _POINTER_CAST_ env->GetLongField(jobj, fid);
    if (ctlrstat == 0)
    {
        fail(env, jobj, cls, "mdr_ctlrstat_t::mdr_get_field: null mdr_ctlrstat_t handle");
        return JNI_FALSE;
    }

    // retrive the key and perform lookup
    const char *key;
    mdr::mctl_node_t node;

    try
    {
        key = env->GetStringUTFChars(jkey, 0);
        node = ctlrstat->find(key);
        env->ReleaseStringUTFChars(jkey, key);
    }
    catch(std::exception &e)
    {
        fail(env, jobj, cls, e.what());
        return JNI_FALSE;
    }

    // not there
    if (node == ctlrstat->end())
        return JNI_FALSE;

    // now assign values from can to can
    return (ctlrstat_set(env, jobj, jcan, node->second, field_number) ? JNI_TRUE : JNI_FALSE);
}
