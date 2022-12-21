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
#include <mdr_mpstat.h>
#include <jni.h>

extern bool mdr_constructor(JNIEnv *, jobject, mdr::mdr_t *);

static void fail(JNIEnv *env, jobject jobj, jclass cls, const char *msg)
{
    jfieldID reason;
    jstring jstr;

    reason = env->GetFieldID(cls, "reason", "Ljava/lang/String;");
    if (reason == 0)
    {
        std::cerr << "mdr_mpstat_t::fail: cannot fail" << std::endl;
        return;
    }
    jstr = env->NewStringUTF(msg);
    env->SetObjectField(jobj, reason, jstr);
}

extern "C" JNIEXPORT jboolean JNICALL
Java_com_capmet_metrics_jmdr_mdr_1mpstat_1t_mpstat_1constructor
    (JNIEnv *env, jobject jobj, jstring jhost)
{
    jclass cls = env->GetObjectClass(jobj);
    jfieldID fid;
    mdr::mdr_mpstat_t *mpstat;

    fid = env->GetFieldID(cls, "handle", "J");
    if (fid == 0)
    {
        fail(env, jobj, cls, "mdr_mpstat_t::mdr_mpstat_t: cannot find handle");
        return JNI_FALSE;
    }
    try
    {
        const char *remote_host = 0;

        if (jhost != 0)
            remote_host = env->GetStringUTFChars(jhost, 0);

        mpstat = new mdr::mdr_mpstat_t(remote_host);

        if (remote_host)
            env->ReleaseStringUTFChars(jhost, remote_host);

        if (!mdr_constructor(env, jobj, mpstat))
            return JNI_FALSE;
    }
    catch(std::exception &e)
    {
        fail(env, jobj, cls, e.what());
        return JNI_FALSE;
    }
    env->SetLongField(jobj, fid, (jlong) _POINTER_CAST_ mpstat);

    return JNI_TRUE;
}

extern "C" JNIEXPORT void JNICALL
Java_com_capmet_metrics_jmdr_mdr_1mpstat_1t_finalize
	(JNIEnv *env, jobject jobj)
{
	jclass cls = env->GetObjectClass(jobj);
	jfieldID fid;
    mdr::mdr_mpstat_t *mpstat;

	fid = env->GetFieldID(cls, "handle", "J");
	if (fid == 0)
		return;
    mpstat = (mdr::mdr_mpstat_t *) _POINTER_CAST_ env->GetLongField(jobj, fid);
	if (mpstat == 0)
		return;

	delete mpstat;

	env->SetIntField(jobj, fid, 0);
}

extern "C" JNIEXPORT jboolean JNICALL
Java_com_capmet_metrics_jmdr_mdr_1mpstat_1t_mpstat_1refresh
	(JNIEnv *env, jobject jobj)
{
    jclass cls = env->GetObjectClass(jobj);
    jfieldID fid;
    mdr::mdr_mpstat_t *mpstat;

    fid = env->GetFieldID(cls, "handle", "J");
    if (fid == 0)
    {
        fail(env, jobj, cls, "mdr_mpstat_t::mpstat_refresh: cannot find handle");
        return JNI_FALSE;
    }
    mpstat = (mdr::mdr_mpstat_t *) _POINTER_CAST_ env->GetLongField(jobj, fid);
    if (mpstat == 0)
    {
        fail(env, jobj, cls, "mdr_mpstat_t::mpstat_refresh: null mdr_mpstat_t handle");
        return JNI_FALSE;
    }
    try
    {
        mpstat->mdr_refresh();
    }
    catch(std::exception &e)
    {
        fail(env, jobj, cls, e.what());
        return JNI_FALSE;
    }

    return JNI_TRUE;
}

static bool mpstat_set
    (JNIEnv *env, jobject jobj, jobject jcan,
        mdr::mmps_can_t &can, jint field_number)
{
    jclass cls = env->GetObjectClass(jobj);
    jclass ccls = env->GetObjectClass(jcan);
    jfieldID fid;

    switch(field_number)
    {
    case mdr::MMPS_USER_TIME_PERCENT:
        //
        // mmps_user_time_percent
        //
        fid = env->GetFieldID(ccls, "mmps_user_time_percent", "I");
        if (fid == 0)
        {
            fail(env, jobj, cls, "mdr_mpstat_t::mpstat_set: cannot find mmps_user_time_percent");
            return false;
        }
        env->SetIntField(jcan, fid, (int) can.mmps_user_time_percent);
        break;
    case mdr::MMPS_KERNEL_TIME_PERCENT:
        //
        // mmps_kernel_time_percent
        //
        fid = env->GetFieldID(ccls, "mmps_kernel_time_percent", "I");
        if (fid == 0)
        {
            fail(env, jobj, cls, "mdr_mpstat_t::mpstat_set: cannot find mmps_kernel_time_percent");
            return false;
        }
        env->SetIntField(jcan, fid, (int) can.mmps_kernel_time_percent);
        break;
    case mdr::MMPS_WAIT_TIME_PERCENT:
        //
        // mmps_wait_time_percent
        //
        fid = env->GetFieldID(ccls, "mmps_wait_time_percent", "I");
        if (fid == 0)
        {
            fail(env, jobj, cls, "mdr_mpstat_t::mpstat_set: cannot find mmps_wait_time_percent");
            return false;
        }
        env->SetIntField(jcan, fid, (int) can.mmps_wait_time_percent);
        break;
    case mdr::MMPS_IDLE_TIME_PERCENT:
        //
        // mmps_idle_time_percent
        //
        fid = env->GetFieldID(ccls, "mmps_idle_time_percent", "I");
        if (fid == 0)
        {
            fail(env, jobj, cls, "mdr_mpstat_t::mpstat_set: cannot find mmps_idle_time_percent");
            return false;
        }
        env->SetIntField(jcan, fid, (int) can.mmps_idle_time_percent);
        break;
    }

    return true;
}

static bool mpstat_set
    (JNIEnv *env, jobject jobj, jobject jcan, mdr::mmps_can_t &can)
{
    return (
        mpstat_set(env, jobj, jcan, can, mdr::MMPS_USER_TIME_PERCENT) &&
        mpstat_set(env, jobj, jcan, can, mdr::MMPS_KERNEL_TIME_PERCENT) &&
        mpstat_set(env, jobj, jcan, can, mdr::MMPS_WAIT_TIME_PERCENT) &&
        mpstat_set(env, jobj, jcan, can, mdr::MMPS_IDLE_TIME_PERCENT));
}

extern "C" JNIEXPORT jint JNICALL
Java_com_capmet_metrics_jmdr_mdr_1mpstat_1t_mdr_1size
	(JNIEnv *env, jobject jobj)
{
    jclass cls = env->GetObjectClass(jobj);
    jfieldID fid;
    mdr::mdr_mpstat_t *mpstat;

    // find handle
    fid = env->GetFieldID(cls, "handle", "J");
    if (fid == 0)
    {
        fail(env, jobj, cls, "mdr_mpstat_t::mdr_size: cannot find handle");
        return JNI_FALSE;
    }

    // get the mpstat pointer
    mpstat = (mdr::mdr_mpstat_t *) _POINTER_CAST_ env->GetLongField(jobj, fid);
    if (mpstat == 0)
    {
        fail(env, jobj, cls, "mdr_mpstat_t::mdr_size: null mdr_mpstat_t handle");
        return JNI_FALSE;
    }

    return mpstat->size();
}

extern "C" JNIEXPORT jobjectArray JNICALL
Java_com_capmet_metrics_jmdr_mdr_1mpstat_1t_mdr_1keyset
	(JNIEnv *env, jobject jobj)
{
    jclass cls = env->GetObjectClass(jobj);
    jfieldID fid;
    mdr::mdr_mpstat_t *mpstat;
    jobjectArray jarray;

    // find handle
    fid = env->GetFieldID(cls, "handle", "J");
    if (fid == 0)
    {
        fail(env, jobj, cls, "mdr_mpstat_t::mdr_keyset: cannot find handle");
        return JNI_FALSE;
    }

    // get the mpstat pointer
    mpstat = (mdr::mdr_mpstat_t *) _POINTER_CAST_ env->GetLongField(jobj, fid);
    if (mpstat == 0)
    {
        fail(env, jobj, cls, "mdr_mpstat_t::mdr_keyset: null mdr_mpstat_t handle");
        return JNI_FALSE;
    }

    // retrieve the keys
    mdr::mdr_keyset_t keyset;
    mdr::mdr_key_t kn;
    int i;

    try
    {
        mpstat->mdr_keyset(keyset);
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
Java_com_capmet_metrics_jmdr_mdr_1mpstat_1t_mdr_1get
    (JNIEnv *env, jobject jobj, jstring jkey, jobject jcan)
{
    jclass cls = env->GetObjectClass(jobj);
    jfieldID fid;
    mdr::mdr_mpstat_t *mpstat;

    // find handle
    fid = env->GetFieldID(cls, "handle", "J");
    if (fid == 0)
    {
        fail(env, jobj, cls, "mdr_mpstat_t::mdr_get: cannot find handle");
        return JNI_FALSE;
    }

    // get the mpstat pointer
    mpstat = (mdr::mdr_mpstat_t *) _POINTER_CAST_ env->GetLongField(jobj, fid);
    if (mpstat == 0)
    {
        fail(env, jobj, cls, "mdr_mpstat_t::mdr_get: null mdr_mpstat_t handle");
        return JNI_FALSE;
    }

    // retrive the key and perform lookup
    const char *key;
    mdr::mmps_node_t node;

    try
    {
        key = env->GetStringUTFChars(jkey, 0);
        node = mpstat->find(key);
        env->ReleaseStringUTFChars(jkey, key);
    }
    catch(std::exception &e)
    {
        fail(env, jobj, cls, e.what());
        return JNI_FALSE;
    }

    // not there
    if (node == mpstat->end())
        return JNI_FALSE;

    // now assign values from can to can
    return (mpstat_set(env, jobj, jcan, node->second) ? JNI_TRUE : JNI_FALSE);
}

extern "C" JNIEXPORT jboolean JNICALL
Java_com_capmet_metrics_jmdr_mdr_1mpstat_1t_mdr_1get_1field
    (JNIEnv *env, jobject jobj, jstring jkey, jobject jcan, jint field_number)
{
    jclass cls = env->GetObjectClass(jobj);
    jfieldID fid;
    mdr::mdr_mpstat_t *mpstat;

    // find handle
    fid = env->GetFieldID(cls, "handle", "J");
    if (fid == 0)
    {
        fail(env, jobj, cls, "mdr_mpstat_t::mdr_get_field: cannot find handle");
        return JNI_FALSE;
    }

    // get the mpstat pointer
    mpstat = (mdr::mdr_mpstat_t *) _POINTER_CAST_ env->GetLongField(jobj, fid);
    if (mpstat == 0)
    {
        fail(env, jobj, cls, "mdr_mpstat_t::mdr_get_field: null mdr_mpstat_t handle");
        return JNI_FALSE;
    }

    // retrive the key and perform lookup
    const char *key;
    mdr::mmps_node_t node;

    try
    {
        key = env->GetStringUTFChars(jkey, 0);
        node = mpstat->find(key);
        env->ReleaseStringUTFChars(jkey, key);
    }
    catch(std::exception &e)
    {
        fail(env, jobj, cls, e.what());
        return JNI_FALSE;
    }

    // not there
    if (node == mpstat->end())
        return JNI_FALSE;

    // now assign values from can to can
    return (mpstat_set(env, jobj, jcan, node->second, field_number) ? JNI_TRUE : JNI_FALSE);
}
