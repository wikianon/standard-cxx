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
#include <mdr_systat.h>
#include <jni.h>

extern bool mdr_constructor(JNIEnv *, jobject, mdr::mdr_t *);

static void fail(JNIEnv *env, jobject jobj, jclass cls, const char *msg)
{
    jfieldID reason;
    jstring jstr;

    reason = env->GetFieldID(cls, "reason", "Ljava/lang/String;");
    if (reason == 0)
    {
        std::cerr << "mdr_systat_t::fail: cannot fail" << std::endl;
        return;
    }
    jstr = env->NewStringUTF(msg);
    env->SetObjectField(jobj, reason, jstr);
}

static bool systat_set(JNIEnv *env, jobject jobj, jclass cls,
                     mdr::mdr_systat_t *systat)
{
    jfieldID fid;

    // 
    // msys_user_time_percent
    // 
    fid = env->GetFieldID(cls, "msys_user_time_percent", "I");
    if (fid == 0)
    {
        fail(env, jobj, cls, "mdr_systat_t::systat_set: cannot find msys_user_time_percent");
        return false;
    }
    env->SetIntField(jobj, fid, (int) systat->msys_user_time_percent);

    // 
    // msys_kernel_time_percent
    // 
    fid = env->GetFieldID(cls, "msys_kernel_time_percent", "I");
    if (fid == 0)
    {
        fail(env, jobj, cls, "mdr_systat_t::systat_set: cannot find msys_kernel_time_percent");
        return false;
    }
    env->SetIntField(jobj, fid, (int) systat->msys_kernel_time_percent);

    // 
    // msys_wait_time_percent
    // 
    fid = env->GetFieldID(cls, "msys_wait_time_percent", "I");
    if (fid == 0)
    {
        fail(env, jobj, cls, "mdr_systat_t::systat_set: cannot find msys_wait_time_percent");
        return false;
    }
    env->SetIntField(jobj, fid, systat->msys_wait_time_percent);

    // 
    // msys_idle_time_percent
    // 
    fid = env->GetFieldID(cls, "msys_idle_time_percent", "I");
    if (fid == 0)
    {
        fail(env, jobj, cls, "mdr_systat_t::systat_set: cannot find msys_idle_time_percent");
        return false;
    }
    env->SetIntField(jobj, fid, systat->msys_idle_time_percent);

    // 
    // msys_page_K_in_per_second
    // 
    fid = env->GetFieldID(cls, "msys_page_K_in_per_second", "I");
    if (fid == 0)
    {
        fail(env, jobj, cls, "mdr_systat_t::systat_set: cannot find msys_page_K_in_per_second");
        return false;
    }
    env->SetIntField(jobj, fid, systat->msys_page_K_in_per_second);

    // 
    // msys_page_K_out_per_second
    // 
    fid = env->GetFieldID(cls, "msys_page_K_out_per_second", "I");
    if (fid == 0)
    {
        fail(env, jobj, cls, "mdr_systat_t::systat_set: cannot find msys_page_K_out_per_second");
        return false;
    }
    env->SetIntField(jobj, fid, systat->msys_page_K_out_per_second);

    // 
    // msys_interrupts_per_second
    // 
    fid = env->GetFieldID(cls, "msys_interrupts_per_second", "I");
    if (fid == 0)
    {
        fail(env, jobj, cls, "mdr_systat_t::systat_set: cannot find msys_interrupts_per_second");
        return false;
    }
    env->SetIntField(jobj, fid, systat->msys_interrupts_per_second);

    // 
    // msys_context_switches_per_second
    // 
    fid = env->GetFieldID(cls, "msys_context_switches_per_second", "I");
    if (fid == 0)
    {
        fail(env, jobj, cls, "mdr_systat_t::systat_set: cannot find msys_context_switches_per_second");
        return false;
    }
    env->SetIntField(jobj, fid, systat->msys_context_switches_per_second);

    // 
    // msys_system_calls_per_second
    // 
    fid = env->GetFieldID(cls, "msys_system_calls_per_second", "I");
    if (fid == 0)
    {
        fail(env, jobj, cls, "mdr_systat_t::systat_set: cannot find msys_system_calls_per_second");
        return false;
    }
    env->SetIntField(jobj, fid, systat->msys_system_calls_per_second);

    // 
    // msys_procs_runnable
    // 
    fid = env->GetFieldID(cls, "msys_procs_runnable", "I");
    if (fid == 0)
    {
        fail(env, jobj, cls, "mdr_systat_t::systat_set: cannot find msys_procs_runnable");
        return false;
    }
    env->SetIntField(jobj, fid, systat->msys_procs_runnable);

    // 
    // msys_procs_blocked
    // 
    fid = env->GetFieldID(cls, "msys_procs_blocked", "I");
    if (fid == 0)
    {
        fail(env, jobj, cls, "mdr_systat_t::systat_set: cannot find msys_procs_blocked");
        return false;
    }
    env->SetIntField(jobj, fid, systat->msys_procs_blocked);

    // 
    // msys_procs_zombied
    // 
    fid = env->GetFieldID(cls, "msys_procs_zombied", "I");
    if (fid == 0)
    {
        fail(env, jobj, cls, "mdr_systat_t::systat_set: cannot find msys_procs_zombied");
        return false;
    }
    env->SetIntField(jobj, fid, systat->msys_procs_zombied);

    // 
    // msys_free_memory_percent
    // 
    fid = env->GetFieldID(cls, "msys_free_memory_percent", "D");
    if (fid == 0)
    {
        fail(env, jobj, cls, "mdr_systat_t::systat_set: cannot find msys_free_memory_percent");
        return false;
    }
    env->SetDoubleField(jobj, fid, systat->msys_free_memory_percent);

    // 
    // msys_free_paging_file_percent
    // 
    fid = env->GetFieldID(cls, "msys_free_paging_file_percent", "D");
    if (fid == 0)
    {
        fail(env, jobj, cls, "mdr_systat_t::systat_set: cannot find msys_free_paging_file_percent");
        return false;
    }
    env->SetDoubleField(jobj, fid, systat->msys_free_paging_file_percent);

    // 
    // msys_read_K_per_second
    // 
    fid = env->GetFieldID(cls, "msys_read_K_per_second", "I");
    if (fid == 0)
    {
        fail(env, jobj, cls, "mdr_systat_t::systat_set: cannot find msys_read_K_per_second");
        return false;
    }
    env->SetIntField(jobj, fid, systat->msys_read_K_per_second);

    // 
    // msys_written_K_per_second
    // 
    fid = env->GetFieldID(cls, "msys_written_K_per_second", "I");
    if (fid == 0)
    {
        fail(env, jobj, cls, "mdr_systat_t::systat_set: cannot find msys_written_K_per_second");
        return false;
    }
    env->SetIntField(jobj, fid, systat->msys_written_K_per_second);

    // 
    // msys_pages_scanned
    // 
    fid = env->GetFieldID(cls, "msys_pages_scanned", "I");
    if (fid == 0)
    {
        fail(env, jobj, cls, "mdr_systat_t::systat_set: cannot find msys_pages_scanned");
        return false;
    }
    env->SetIntField(jobj, fid, systat->msys_pages_scanned);

    return true;
}

extern "C" JNIEXPORT jboolean JNICALL
Java_com_capmet_metrics_jmdr_mdr_1systat_1t_systat_1constructor
    (JNIEnv *env, jobject jobj, jstring jhost)
{
    jclass cls = env->GetObjectClass(jobj);
    jfieldID fid;
    mdr::mdr_systat_t *systat;

    fid = env->GetFieldID(cls, "handle", "J");
    if (fid == 0)
    {
        fail(env, jobj, cls, "mdr_systat_t::mdr_systat_t: cannot find handle");
        return JNI_FALSE;
    }
    try
    {
        const char *remote_host = 0;

        if (jhost != 0)
            remote_host = env->GetStringUTFChars(jhost, 0);

        systat = new mdr::mdr_systat_t(remote_host);

        if (remote_host)
            env->ReleaseStringUTFChars(jhost, remote_host);

        if (!mdr_constructor(env, jobj, systat))
            return JNI_FALSE;
    }
    catch(std::exception & e)
    {
        fail(env, jobj, cls, e.what());
        return JNI_FALSE;
    }
    env->SetLongField(jobj, fid, (jlong) _POINTER_CAST_ systat);

    return (systat_set(env, jobj, cls, systat) ? JNI_TRUE : JNI_FALSE);
}

extern "C" JNIEXPORT void JNICALL
Java_com_capmet_metrics_jmdr_mdr_1systat_1t_finalize
    (JNIEnv *env, jobject jobj)
{
    jclass cls = env->GetObjectClass(jobj);
    jfieldID fid;
    mdr::mdr_systat_t *systat;

    fid = env->GetFieldID(cls, "handle", "J");
    if (fid == 0)
        return;
    systat = (mdr::mdr_systat_t *) _POINTER_CAST_ env->GetLongField(jobj, fid);
    if (systat == 0)
        return;

    delete systat;

    env->SetIntField(jobj, fid, 0);
}

extern "C" JNIEXPORT jboolean JNICALL
Java_com_capmet_metrics_jmdr_mdr_1systat_1t_systat_1refresh
    (JNIEnv *env, jobject jobj)
{
    jclass cls = env->GetObjectClass(jobj);
    jfieldID fid;
    mdr::mdr_systat_t *systat;

    fid = env->GetFieldID(cls, "handle", "J");
    if (fid == 0)
    {
        fail(env, jobj, cls, "mdr_systat_t::systat_refresh: cannot find handle");
        return JNI_FALSE;
    }
    systat = (mdr::mdr_systat_t *) _POINTER_CAST_ env->GetLongField(jobj, fid);
    if (systat == 0)
    {
        fail(env, jobj, cls, "mdr_systat_t::systat_refresh: null mdr_systat_t handle");
        return JNI_FALSE;
    }
    try
    {
        systat->mdr_refresh();
    }
    catch(std::exception & e)
    {
        fail(env, jobj, cls, e.what());
        return JNI_FALSE;
    }

    return (systat_set(env, jobj, cls, systat) ? JNI_TRUE : JNI_FALSE);
}
