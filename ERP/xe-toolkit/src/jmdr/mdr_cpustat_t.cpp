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
#include <mdr_cpustat.h>
#include <jni.h>

extern bool mdr_constructor(JNIEnv *, jobject, mdr::mdr_t *);

static void fail(JNIEnv *env, jobject jobj, jclass cls, const char *msg)
{
    jfieldID reason;
    jstring jstr;

    reason = env->GetFieldID(cls, "reason", "Ljava/lang/String;");
    if (reason == 0)
    {
        std::cerr << "mdr_cpustat_t::fail: cannot fail" << std::endl;
        return;
    }
    jstr = env->NewStringUTF(msg);
    env->SetObjectField(jobj, reason, jstr);
}

static bool cpustat_set(JNIEnv *env, jobject jobj, jclass cls,
                     mdr::mdr_cpustat_t *cpustat)
{
    jfieldID fid;

    // 
    // mcs_user_time_percent
    // 
    fid = env->GetFieldID(cls, "mcs_user_time_percent", "I");
    if (fid == 0)
    {
        fail(env, jobj, cls, "mdr_cpustat_t::cpustat_set: cannot find mcs_user_time_percent");
        return false;
    }
    env->SetIntField(jobj, fid, (int) cpustat->mcs_user_time_percent);

    // 
    // mcs_kernel_time_percent
    // 
    fid = env->GetFieldID(cls, "mcs_kernel_time_percent", "I");
    if (fid == 0)
    {
        fail(env, jobj, cls, "mdr_cpustat_t::cpustat_set: cannot find mcs_kernel_time_percent");
        return false;
    }
    env->SetIntField(jobj, fid, (int) cpustat->mcs_kernel_time_percent);

    // 
    // mcs_wait_time_percent
    // 
    fid = env->GetFieldID(cls, "mcs_wait_time_percent", "I");
    if (fid == 0)
    {
        fail(env, jobj, cls, "mdr_cpustat_t::cpustat_set: cannot find mcs_wait_time_percent");
        return false;
    }
    env->SetIntField(jobj, fid, cpustat->mcs_wait_time_percent);

    // 
    // mcs_idle_time_percent
    // 
    fid = env->GetFieldID(cls, "mcs_idle_time_percent", "I");
    if (fid == 0)
    {
        fail(env, jobj, cls, "mdr_cpustat_t::cpustat_set: cannot find mcs_idle_time_percent");
        return false;
    }
    env->SetIntField(jobj, fid, cpustat->mcs_idle_time_percent);

    return true;
}

extern "C" JNIEXPORT jboolean JNICALL
Java_com_capmet_metrics_jmdr_mdr_1cpustat_1t_cpustat_1constructor
    (JNIEnv *env, jobject jobj, jstring jhost)
{
    jclass cls = env->GetObjectClass(jobj);
    jfieldID fid;
    mdr::mdr_cpustat_t *cpustat;

    fid = env->GetFieldID(cls, "handle", "J");
    if (fid == 0)
    {
        fail(env, jobj, cls, "mdr_cpustat_t::mdr_cpustat_t: cannot find handle");
        return JNI_FALSE;
    }
    try
    {
        const char *remote_host = 0;

        if (jhost != 0)
            remote_host = env->GetStringUTFChars(jhost, 0);

        cpustat = new mdr::mdr_cpustat_t(remote_host);

        if (remote_host)
            env->ReleaseStringUTFChars(jhost, remote_host);

        if (!mdr_constructor(env, jobj, cpustat))
            return JNI_FALSE;
    }
    catch(std::exception & e)
    {
        fail(env, jobj, cls, e.what());
        return JNI_FALSE;
    }
    env->SetLongField(jobj, fid, (jlong) _POINTER_CAST_ cpustat);

    return (cpustat_set(env, jobj, cls, cpustat) ? JNI_TRUE : JNI_FALSE);
}

extern "C" JNIEXPORT void JNICALL
Java_com_capmet_metrics_jmdr_mdr_1cpustat_1t_finalize
	(JNIEnv *env, jobject jobj)
{
    jclass cls = env->GetObjectClass(jobj);
    jfieldID fid;
    mdr::mdr_cpustat_t *cpustat;

    fid = env->GetFieldID(cls, "handle", "J");
    if (fid == 0)
        return;
    cpustat = (mdr::mdr_cpustat_t *) _POINTER_CAST_ env->GetLongField(jobj, fid);
    if (cpustat == 0)
        return;

	delete cpustat;

	env->SetIntField(jobj, fid, 0);
}

extern "C" JNIEXPORT jboolean JNICALL
Java_com_capmet_metrics_jmdr_mdr_1cpustat_1t_cpustat_1refresh
	(JNIEnv *env, jobject jobj)
{
    jclass cls = env->GetObjectClass(jobj);
    jfieldID fid;
    mdr::mdr_cpustat_t *cpustat;

    fid = env->GetFieldID(cls, "handle", "J");
    if (fid == 0)
    {
        fail(env, jobj, cls, "mdr_cpustat_t::cpustat_refresh: cannot find handle");
        return JNI_FALSE;
    }
    cpustat = (mdr::mdr_cpustat_t *) _POINTER_CAST_ env->GetLongField(jobj, fid);
    if (cpustat == 0)
    {
        fail(env, jobj, cls, "mdr_cpustat_t::cpustat_refresh: null mdr_cpustat_t handle");
        return JNI_FALSE;
    }
    try
    {
        cpustat->mdr_refresh();
    }
    catch(std::exception & e)
    {
        fail(env, jobj, cls, e.what());
        return JNI_FALSE;
    }

    return (cpustat_set(env, jobj, cls, cpustat) ? JNI_TRUE : JNI_FALSE);
}

extern "C" JNIEXPORT jboolean JNICALL
Java_com_capmet_metrics_jmdr_mdr_1cpustat_1t_cpustat_1rates
	(JNIEnv *env, jobject jobj)
{
    jclass cls = env->GetObjectClass(jobj);
    jfieldID fid;
    mdr::mdr_cpustat_t *cpustat;

    fid = env->GetFieldID(cls, "handle", "J");
    if (fid == 0)
    {
        fail(env, jobj, cls, "mdr_cpustat_t::cpustat_rates: cannot find handle");
        return JNI_FALSE;
    }
    cpustat = (mdr::mdr_cpustat_t *) _POINTER_CAST_ env->GetLongField(jobj, fid);
    if (cpustat == 0)
    {
        fail(env, jobj, cls, "mdr_cpustat_t::cpustat_rates: null mdr_cpustat_t handle");
        return JNI_FALSE;
    }
    try
    {
        cpustat->mdr_rates();
    }
    catch(std::exception & e)
    {
        fail(env, jobj, cls, e.what());
        return JNI_FALSE;
    }

    return (cpustat_set(env, jobj, cls, cpustat) ? JNI_TRUE : JNI_FALSE);
}
