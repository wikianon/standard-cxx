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
#include <mdr_proc_states.h>
#include <jni.h>

extern bool mdr_constructor(JNIEnv *, jobject, mdr::mdr_t *);

static void fail(JNIEnv *env, jobject jobj, jclass cls, const char *msg)
{
    jfieldID reason;
    jstring jstr;

    reason = env->GetFieldID(cls, "reason", "Ljava/lang/String;");
    if (reason == 0)
    {
        std::cerr << "mdr_proc_states_t::fail: cannot fail" << std::endl;
        return;
    }
    jstr = env->NewStringUTF(msg);
    env->SetObjectField(jobj, reason, jstr);
}

static bool proc_states_set(JNIEnv *env, jobject jobj, jclass cls,
                            mdr::mdr_proc_states_t *ps)
{
    jfieldID fid;

    // 
    // mpst_procs_runnable
    // 
    fid = env->GetFieldID(cls, "mpst_procs_runnable", "I");
    if (fid == 0)
    {
        fail(env, jobj, cls, "mdr_proc_states_t::proc_states_set: cannot find mpst_procs_runnable");
        return false;
    }
    env->SetIntField(jobj, fid, ps->mpst_procs_runnable);

    // 
    // mpst_procs_sleeping
    // 
    fid = env->GetFieldID(cls, "mpst_procs_sleeping", "I");
    if (fid == 0)
    {
        fail(env, jobj, cls, "mdr_proc_states_t::proc_states_set: cannot find mpst_procs_sleeping");
        return false;
    }
    env->SetIntField(jobj, fid, ps->mpst_procs_sleeping);

    // 
    // mpst_procs_stopped
    // 
    fid = env->GetFieldID(cls, "mpst_procs_stopped", "I");
    if (fid == 0)
    {
        fail(env, jobj, cls, "mdr_proc_states_t::proc_states_set: cannot find mpst_procs_stopped");
        return false;
    }
    env->SetIntField(jobj, fid, ps->mpst_procs_stopped);

    // 
    // mpst_procs_blocked
    // 
    fid = env->GetFieldID(cls, "mpst_procs_blocked", "I");
    if (fid == 0)
    {
        fail(env, jobj, cls, "mdr_proc_states_t::proc_states_set: cannot find mpst_procs_blocked");
        return false;
    }
    env->SetIntField(jobj, fid, ps->mpst_procs_blocked);

    // 
    // mpst_procs_zombie
    // 
    fid = env->GetFieldID(cls, "mpst_procs_zombie", "I");
    if (fid == 0)
    {
        fail(env, jobj, cls, "mdr_proc_states_t::proc_states_set: cannot find mpst_procs_zombie");
        return false;
    }
    env->SetIntField(jobj, fid, ps->mpst_procs_zombie);

    // 
    // mpst_procs_specific
    // 
    fid = env->GetFieldID(cls, "mpst_procs_specific", "I");
    if (fid == 0)
    {
        fail(env, jobj, cls, "mdr_proc_states_t::proc_states_set: cannot find mpst_procs_specific");
        return false;
    }
    env->SetIntField(jobj, fid, ps->mpst_procs_specific);

    return true;
}

extern "C" JNIEXPORT jboolean JNICALL
Java_com_capmet_metrics_jmdr_mdr_1proc_1states_1t_proc_1states_1constructor
    (JNIEnv *env, jobject jobj, jstring jhost)
{
    jclass cls = env->GetObjectClass(jobj);
    jfieldID fid;
    mdr::mdr_proc_states_t *ps;

    fid = env->GetFieldID(cls, "handle", "J");
    if (fid == 0)
    {
        fail(env, jobj, cls, "mdr_proc_states_t::mdr_proc_states_t: cannot find handle");
        return JNI_FALSE;
    }
    try
    {
        const char *remote_host = 0;

        if (jhost != 0)
            remote_host = env->GetStringUTFChars(jhost, 0);

        ps = new mdr::mdr_proc_states_t(remote_host);

        if (remote_host)
            env->ReleaseStringUTFChars(jhost, remote_host);

        if (!mdr_constructor(env, jobj, ps))
            return JNI_FALSE;
    }
    catch(std::exception & e)
    {
        fail(env, jobj, cls, e.what());
        return JNI_FALSE;
    }
    env->SetLongField(jobj, fid, (jlong) _POINTER_CAST_ ps);

    return (proc_states_set(env, jobj, cls, ps) ? JNI_TRUE : JNI_FALSE);
}

extern "C" JNIEXPORT void JNICALL
Java_com_capmet_metrics_jmdr_mdr_1proc_1states_1t_finalize
    (JNIEnv *env, jobject jobj)
{
    jclass cls = env->GetObjectClass(jobj);
    jfieldID fid;
    mdr::mdr_proc_states_t *ps;

    fid = env->GetFieldID(cls, "handle", "J");
    if (fid == 0)
        return;
    ps = (mdr::mdr_proc_states_t *) _POINTER_CAST_ env->GetLongField(jobj, fid);
    if (ps == 0)
        return;

    delete ps;

    env->SetIntField(jobj, fid, 0);
}

extern "C" JNIEXPORT jboolean JNICALL
Java_com_capmet_metrics_jmdr_mdr_1proc_1states_1t_proc_1states_1refresh
    (JNIEnv *env, jobject jobj)
{
    jclass cls = env->GetObjectClass(jobj);
    jfieldID fid;
    mdr::mdr_proc_states_t *ps;

    fid = env->GetFieldID(cls, "handle", "J");
    if (fid == 0)
    {
        fail(env, jobj, cls, "mdr_proc_states_t::proc_states_refresh: cannot find handle");
        return JNI_FALSE;
    }
    ps = (mdr::mdr_proc_states_t *) _POINTER_CAST_ env->GetLongField(jobj, fid);
    if (ps == 0)
    {
        fail(env, jobj, cls, "mdr_proc_states_t::proc_states_refresh: null mdr_proc_states_t handle");
        return JNI_FALSE;
    }
    try
    {
        ps->mdr_refresh();
    }
    catch(std::exception & e)
    {
        fail(env, jobj, cls, e.what());
        return JNI_FALSE;
    }

    return (proc_states_set(env, jobj, cls, ps) ? JNI_TRUE : JNI_FALSE);
}
