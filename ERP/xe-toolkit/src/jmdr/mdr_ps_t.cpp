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
#include <mdr_ps.h>
#include <jni.h>

extern bool mdr_constructor(JNIEnv *, jobject, mdr::mdr_t *);

static void fail(JNIEnv *env, jobject jobj, jclass cls, const char *msg)
{
    jfieldID reason;
    jstring jstr;

    reason = env->GetFieldID(cls, "reason", "Ljava/lang/String;");
    if (reason == 0)
    {
        std::cerr << "mdr_ps_t::fail: cannot fail" << std::endl;
        return;
    }
    jstr = env->NewStringUTF(msg);
    env->SetObjectField(jobj, reason, jstr);
}

extern "C" JNIEXPORT jboolean JNICALL
Java_com_capmet_metrics_jmdr_mdr_1ps_1t_ps_1constructor
    (JNIEnv *env, jobject jobj, jstring jhost)
{
    jclass cls = env->GetObjectClass(jobj);
    jfieldID fid;
    mdr::mdr_ps_t *ps;

    fid = env->GetFieldID(cls, "handle", "J");
    if (fid == 0)
    {
        fail(env, jobj, cls, "mdr_ps_t::mdr_ps_t: cannot find handle");
        return JNI_FALSE;
    }
    try
    {
        const char *remote_host = 0;

        if (jhost != 0)
            remote_host = env->GetStringUTFChars(jhost, 0);

        ps = new mdr::mdr_ps_t(remote_host);

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

    return JNI_TRUE;
}

extern "C" JNIEXPORT void JNICALL
Java_com_capmet_metrics_jmdr_mdr_1ps_1t_finalize
    (JNIEnv *env, jobject jobj)
{
    jclass cls = env->GetObjectClass(jobj);
    jfieldID fid;
    mdr::mdr_ps_t *ps;

    fid = env->GetFieldID(cls, "handle", "J");
    if (fid == 0)
        return;
    ps = (mdr::mdr_ps_t *) _POINTER_CAST_ env->GetLongField(jobj, fid);
    if (ps == 0)
        return;

    delete ps;

    env->SetIntField(jobj, fid, 0);
}

extern "C" JNIEXPORT jboolean JNICALL
Java_com_capmet_metrics_jmdr_mdr_1ps_1t_ps_1refresh
    (JNIEnv *env, jobject jobj)
{
    jclass cls = env->GetObjectClass(jobj);
    jfieldID fid;
    mdr::mdr_ps_t *ps;

    fid = env->GetFieldID(cls, "handle", "J");
    if (fid == 0)
    {
        fail(env, jobj, cls, "mdr_ps_t::ps_refresh: cannot find handle");
        return JNI_FALSE;
    }
    ps = (mdr::mdr_ps_t *) _POINTER_CAST_ env->GetLongField(jobj, fid);
    if (ps == 0)
    {
        fail(env, jobj, cls, "mdr_ps_t::ps_refresh: null mdr_ps_t handle");
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

    //
    // mps_procs_runnable
    //
    fid = env->GetFieldID(cls, "mps_procs_runnable", "I");
    if (fid == 0)
    {
        fail(env, jobj, cls, "mdr_ps_t::ps_refresh: cannot find mps_procs_runnable");
        return JNI_FALSE;
    }
    env->SetIntField(jobj, fid, (int) ps->mps_procs_runnable);

    //
    // mps_procs_sleeping
    //
    fid = env->GetFieldID(cls, "mps_procs_sleeping", "I");
    if (fid == 0)
    {
        fail(env, jobj, cls, "mdr_ps_t::ps_refresh: cannot find mps_procs_sleeping");
        return JNI_FALSE;
    }
    env->SetIntField(jobj, fid, (int) ps->mps_procs_sleeping);

    //
    // mps_procs_stopped
    //
    fid = env->GetFieldID(cls, "mps_procs_stopped", "I");
    if (fid == 0)
    {
        fail(env, jobj, cls, "mdr_ps_t::ps_refresh: cannot find mps_procs_stopped");
        return JNI_FALSE;
    }
    env->SetIntField(jobj, fid, (int) ps->mps_procs_stopped);

    //
    // mps_procs_blocked
    //
    fid = env->GetFieldID(cls, "mps_procs_blocked", "I");
    if (fid == 0)
    {
        fail(env, jobj, cls, "mdr_ps_t::ps_refresh: cannot find mps_procs_blocked");
        return JNI_FALSE;
    }
    env->SetIntField(jobj, fid, (int) ps->mps_procs_blocked);

    //
    // mps_procs_zombie
    //
    fid = env->GetFieldID(cls, "mps_procs_zombie", "I");
    if (fid == 0)
    {
        fail(env, jobj, cls, "mdr_ps_t::ps_refresh: cannot find mps_procs_zombie");
        return JNI_FALSE;
    }
    env->SetIntField(jobj, fid, (int) ps->mps_procs_zombie);

    //
    // mps_procs_specific
    //
    fid = env->GetFieldID(cls, "mps_procs_specific", "I");
    if (fid == 0)
    {
        fail(env, jobj, cls, "mdr_ps_t::ps_refresh: cannot find mps_procs_specific");
        return JNI_FALSE;
    }
    env->SetIntField(jobj, fid, (int) ps->mps_procs_specific);

    return JNI_TRUE;
}

extern "C" JNIEXPORT jboolean JNICALL
Java_com_capmet_metrics_jmdr_mdr_1ps_1t_ps_1rates
    (JNIEnv *env, jobject jobj)
{
    jclass cls = env->GetObjectClass(jobj);
    jfieldID fid;
    mdr::mdr_ps_t *ps;

    fid = env->GetFieldID(cls, "handle", "J");
    if (fid == 0)
    {
        fail(env, jobj, cls, "mdr_ps_t::ps_rates: cannot find handle");
        return JNI_FALSE;
    }
    ps = (mdr::mdr_ps_t *) _POINTER_CAST_ env->GetLongField(jobj, fid);
    if (ps == 0)
    {
        fail(env, jobj, cls, "mdr_ps_t::ps_rates: null mdr_ps_t handle");
        return JNI_FALSE;
    }
    try
    {
        ps->mdr_rates();
    }
    catch(std::exception & e)
    {
        fail(env, jobj, cls, e.what());
        return JNI_FALSE;
    }

    return JNI_TRUE;
}

static bool ps_set
    (JNIEnv *env, jobject jobj, jobject jcan,
        mdr::mps_can_t &can, jint field_number)
{
    jclass cls = env->GetObjectClass(jobj);
    jclass ccls = env->GetObjectClass(jcan);
    jfieldID fid;
    jstring jstr;

    switch(field_number)
    {
    case mdr::MPS_START:
        //
        // mps_start
        //
        fid = env->GetFieldID(ccls, "mps_start", "J");
        if (fid == 0)
        {
            fail(env, jobj, cls, "mdr_ps_t::ps_set: cannot find mps_start");
            return false;
        }
        env->SetLongField(jcan, fid, can.mps_start);
        break;
    case mdr::MPS_UID:
        //
        // mps_uid
        //
        fid = env->GetFieldID(ccls, "mps_uid", "I");
        if (fid == 0)
        {
            fail(env, jobj, cls, "mdr_ps_t::ps_set: cannot find mps_uid");
            return false;
        }
        env->SetIntField(jcan, fid, (int) can.mps_uid);
        break;
    case mdr::MPS_EUID:
        //
        // mps_euid
        //
        fid = env->GetFieldID(ccls, "mps_euid", "I");
        if (fid == 0)
        {
            fail(env, jobj, cls, "mdr_ps_t::ps_set: cannot find mps_euid");
            return false;
        }
        env->SetIntField(jcan, fid, (int) can.mps_euid);
        break;
    case mdr::MPS_PID:
        //
        // mps_pid
        //
        fid = env->GetFieldID(ccls, "mps_pid", "I");
        if (fid == 0)
        {
            fail(env, jobj, cls, "mdr_ps_t::ps_set: cannot find mps_pid");
            return false;
        }
        env->SetIntField(jcan, fid, (int) can.mps_pid);
        break;
    case mdr::MPS_PPID:
        //
        // mps_ppid
        //
        fid = env->GetFieldID(ccls, "mps_ppid", "I");
        if (fid == 0)
        {
            fail(env, jobj, cls, "mdr_ps_t::ps_set: cannot find mps_ppid");
            return false;
        }
        env->SetIntField(jcan, fid, (int) can.mps_ppid);
        break;
    case mdr::MPS_TTY_DEVICE:
        //
        // mps_tty_device
        //
        fid = env->GetFieldID(ccls, "mps_tty_device", "J");
        if (fid == 0)
        {
            fail(env, jobj, cls, "mdr_ps_t::ps_set: cannot find mps_tty_device");
            return false;
        }
        env->SetLongField(jcan, fid, can.mps_tty_device);
        break;
    case mdr::MPS_FILE_NAME:
        //
        // mps_file_name
        //
        fid = env->GetFieldID(ccls, "mps_file_name", "Ljava/lang/String;");
        if (fid == 0)
        {
            fail(env, jobj, cls, "mdr_ps_t::ps_set: cannot find mps_file_name");
            return false;
        }
        jstr = env->NewStringUTF(can.mps_file_name.c_str());
        env->SetObjectField(jcan, fid, jstr);
        break;
    case mdr::MPS_COMMAND:
        //
        // mps_command
        //
        fid = env->GetFieldID(ccls, "mps_command", "Ljava/lang/String;");
        if (fid == 0)
        {
            fail(env, jobj, cls, "mdr_ps_t::ps_set: cannot find mps_command");
            return false;
        }
        jstr = env->NewStringUTF(can.mps_command.c_str());
        env->SetObjectField(jcan, fid, jstr);
        break;
    case mdr::MPS_FIRST_ARGUMENT:
        //
        // mps_first_argument
        //
        fid = env->GetFieldID(ccls, "mps_first_argument", "Ljava/lang/String;");
        if (fid == 0)
        {
            fail(env, jobj, cls, "mdr_ps_t::ps_set: cannot find mps_first_argument");
            return false;
        }
        jstr = env->NewStringUTF(can.mps_first_argument.c_str());
        env->SetObjectField(jcan, fid, jstr);
        break;
    case mdr::MPS_CPU_TIME:
        //
        // mps_cpu_time
        //
        fid = env->GetFieldID(ccls, "mps_cpu_time", "D");
        if (fid == 0)
        {
            fail(env, jobj, cls, "mdr_ps_t::ps_set: cannot find mps_cpu_time");
            return false;
        }
        env->SetDoubleField(jcan, fid, can.mps_cpu_time);
        break;
    case mdr::MPS_USR_TIME:
        //
        // mps_user_time
        //
        fid = env->GetFieldID(ccls, "mps_user_time", "D");
        if (fid == 0)
        {
            fail(env, jobj, cls, "mdr_ps_t::ps_set: cannot find mps_user_time");
            return false;
        }
        env->SetDoubleField(jcan, fid, can.mps_user_time);
        break;
    case mdr::MPS_KERNEL_TIME:
        //
        // mps_kernel_time
        //
        fid = env->GetFieldID(ccls, "mps_kernel_time", "D");
        if (fid == 0)
        {
            fail(env, jobj, cls, "mdr_ps_t::ps_set: cannot find mps_kernel_time");
            return false;
        }
        env->SetDoubleField(jcan, fid, can.mps_kernel_time);
        break;
    case mdr::MPS_USR_TIME_PERCENT:
        //
        // mps_user_time_percent
        //
        fid = env->GetFieldID(ccls, "mps_user_time_percent", "D");
        if (fid == 0)
        {
            fail(env, jobj, cls, "mdr_ps_t::ps_set: cannot find mps_user_time_percent");
            return false;
        }
        env->SetDoubleField(jcan, fid, can.mps_user_time_percent);
        break;
    case mdr::MPS_KERNEL_TIME_PERCENT:
        //
        // mps_kernel_time_percent
        //
        fid = env->GetFieldID(ccls, "mps_kernel_time_percent", "D");
        if (fid == 0)
        {
            fail(env, jobj, cls, "mdr_ps_t::ps_set: cannot find mps_kernel_time_percent");
            return false;
        }
        env->SetDoubleField(jcan, fid, can.mps_kernel_time_percent);
        break;
    case mdr::MPS_SIZE_K:
        //
        // mps_size_K
        //
        fid = env->GetFieldID(ccls, "mps_size_K", "J");
        if (fid == 0)
        {
            fail(env, jobj, cls, "mdr_ps_t::ps_set: cannot find mps_size_K");
            return false;
        }
        env->SetLongField(jcan, fid, can.mps_size_K);
        break;
    case mdr::MPS_RSSIZE_K:
        //
        // mps_rssize_K
        //
        fid = env->GetFieldID(ccls, "mps_rssize_K", "J");
        if (fid == 0)
        {
            fail(env, jobj, cls, "mdr_ps_t::ps_set: cannot find mps_rssize_K");
            return false;
        }
        env->SetLongField(jcan, fid, can.mps_rssize_K);
        break;
    case mdr::MPS_IO_K:
        //
        // mps_io_K
        //
        fid = env->GetFieldID(ccls, "mps_io_K", "J");
        if (fid == 0)
        {
            fail(env, jobj, cls, "mdr_ps_t::ps_set: cannot find mps_io_K");
            return false;
        }
        env->SetLongField(jcan, fid, can.mps_io_K);
        break;
    case mdr::MPS_CPU_PERCENT:
        //
        // mps_cpu_percent
        //
        fid = env->GetFieldID(ccls, "mps_cpu_percent", "D");
        if (fid == 0)
        {
            fail(env, jobj, cls, "mdr_ps_t::ps_set: cannot find mps_cpu_percent");
            return false;
        }
        env->SetDoubleField(jcan, fid, can.mps_cpu_percent);
        break;
    case mdr::MPS_MEMORY_PERCENT:
        //
        // mps_memory_percent
        //
        fid = env->GetFieldID(ccls, "mps_memory_percent", "D");
        if (fid == 0)
        {
            fail(env, jobj, cls, "mdr_ps_t::ps_set: cannot find mps_memory_percent");
            return false;
        }
        env->SetDoubleField(jcan, fid, can.mps_memory_percent);
        break;
    case mdr::MPS_VOLUNTARY_CSW:
        //
        // mps_voluntary_csw
        //
        fid = env->GetFieldID(ccls, "mps_voluntary_csw", "I");
        if (fid == 0)
        {
            fail(env, jobj, cls, "mdr_ps_t::ps_set: cannot find mps_voluntary_csw");
            return false;
        }
        env->SetIntField(jcan, fid, (int) can.mps_voluntary_csw);
        break;
    case mdr::MPS_INVOLUNTARY_CSW:
        //
        // mps_involuntary_csw
        //
        fid = env->GetFieldID(ccls, "mps_involuntary_csw", "I");
        if (fid == 0)
        {
            fail(env, jobj, cls, "mdr_ps_t::ps_set: cannot find mps_involuntary_csw");
            return false;
        }
        env->SetIntField(jcan, fid, (int) can.mps_involuntary_csw);
        break;
    case mdr::MPS_DISK_FAULTS:
        //
        // mps_disk_faults
        //
        fid = env->GetFieldID(ccls, "mps_disk_faults", "I");
        if (fid == 0)
        {
            fail(env, jobj, cls, "mdr_ps_t::ps_set: cannot find mps_disk_faults");
            return false;
        }
        env->SetIntField(jcan, fid, (int) can.mps_disk_faults);
        break;
    case mdr::MPS_PAGE_RECLAIMS:
        //
        // mps_page_reclaims
        //
        fid = env->GetFieldID(ccls, "mps_page_reclaims", "I");
        if (fid == 0)
        {
            fail(env, jobj, cls, "mdr_ps_t::ps_set: cannot find mps_page_reclaims");
            return false;
        }
        env->SetIntField(jcan, fid, (int) can.mps_page_reclaims);
        break;
    case mdr::MPS_STATE:
        //
        // mps_state
        //
        fid = env->GetFieldID(ccls, "mps_state", "I");
        if (fid == 0)
        {
            fail(env, jobj, cls, "mdr_ps_t::ps_set: cannot find mps_state");
            return false;
        }
        env->SetIntField(jcan, fid, (int) can.mps_state);
        break;
    case mdr::MPS_PRIORITY:
        //
        // mps_priority
        //
        fid = env->GetFieldID(ccls, "mps_priority", "I");
        if (fid == 0)
        {
            fail(env, jobj, cls, "mdr_ps_t::ps_set: cannot find mps_priority");
            return false;
        }
        env->SetIntField(jcan, fid, (int) can.mps_priority);
        break;
    case mdr::MPS_NICE:
        //
        // mps_nice
        //
        fid = env->GetFieldID(ccls, "mps_nice", "I");
        if (fid == 0)
        {
            fail(env, jobj, cls, "mdr_ps_t::ps_set: cannot find mps_nice");
            return false;
        }
        env->SetIntField(jcan, fid, (int) can.mps_nice);
        break;
    case mdr::MPS_THREAD_COUNT:
        //
        // mps_thread_count
        //
        fid = env->GetFieldID(ccls, "mps_thread_count", "I");
        if (fid == 0)
        {
            fail(env, jobj, cls, "mdr_ps_t::ps_set: cannot find mps_thread_count");
            return false;
        }
        env->SetIntField(jcan, fid, (int) can.mps_thread_count);
        break;
    case mdr::MPS_SIZE_DELTA_K:
        //
        // mps_size_delta_K
        //
        fid = env->GetFieldID(ccls, "mps_size_delta_K", "J");
        if (fid == 0)
        {
            fail(env, jobj, cls, "mdr_ps_t::ps_set: cannot find mps_size_delta_K");
            return false;
        }
        env->SetLongField(jcan, fid, can.mps_size_delta_K);
        break;
    case mdr::MPS_RSSIZE_DELTA_K:
        //
        // mps_rssize_delta_K
        //
        fid = env->GetFieldID(ccls, "mps_rssize_delta_K", "J");
        if (fid == 0)
        {
            fail(env, jobj, cls, "mdr_ps_t::ps_set: cannot find mps_rssize_delta_K");
            return false;
        }
        env->SetLongField(jcan, fid, can.mps_rssize_delta_K);
        break;
    case mdr::MPS_USER_NAME:
        //
        // mps_user_name
        //
        fid = env->GetFieldID(ccls, "mps_user_name", "Ljava/lang/String;");
        if (fid == 0)
        {
            fail(env, jobj, cls, "mdr_ps_t::ps_set: cannot find mps_user_name");
            return false;
        }
        jstr = env->NewStringUTF(can.mps_user_name.c_str());
        env->SetObjectField(jcan, fid, jstr);
        break;
    case mdr::MPS_EFFECTIVE_USER_NAME:
        //
        // mps_effective_user_name
        //
        fid = env->GetFieldID(ccls, "mps_effective_user_name", "Ljava/lang/String;");
        if (fid == 0)
        {
            fail(env, jobj, cls, "mdr_ps_t::ps_set: cannot find mps_effective_user_name");
            return false;
        }
        jstr = env->NewStringUTF(can.mps_effective_user_name.c_str());
        env->SetObjectField(jcan, fid, jstr);
        break;
    case mdr::MPS_SYSTEM_CALLS:
        //
        // mps_system_calls
        //
        fid = env->GetFieldID(ccls, "mps_system_calls", "I");
        if (fid == 0)
        {
            fail(env, jobj, cls, "mdr_ps_t::ps_set: cannot find mps_system_calls");
            return false;
        }
        env->SetIntField(jcan, fid, can.mps_system_calls);
        break;
    case mdr::MPS_IS_SYSTEM_PROCESS:
        //
        // mps_is_system_process
        //
        fid = env->GetFieldID(ccls, "mps_is_system_process", "Z");
        if (fid == 0)
        {
            fail(env, jobj, cls, "mdr_ps_t::ps_set: cannot find mps_is_system_process");
            return false;
        }
        env->SetBooleanField(jcan, fid, can.mps_is_system_process);
        break;
    default:
        {
        char error[128];

        snprintf(error, sizeof error, "mdr_ps_t::ps_set: field: %d not found", (int)field_number);
        fail(env, jobj, cls, error);
        return false;
        }
    }

    return true;
}

static bool ps_set
    (JNIEnv *env, jobject jobj, jobject jcan, mdr::mps_can_t &can)
{
    return(
        ps_set(env, jobj, jcan, can, mdr::MPS_START) &&
        ps_set(env, jobj, jcan, can, mdr::MPS_UID) &&
        ps_set(env, jobj, jcan, can, mdr::MPS_EUID) &&
        ps_set(env, jobj, jcan, can, mdr::MPS_PID) &&
        ps_set(env, jobj, jcan, can, mdr::MPS_PPID) &&
        ps_set(env, jobj, jcan, can, mdr::MPS_TTY_DEVICE) &&
        ps_set(env, jobj, jcan, can, mdr::MPS_FILE_NAME) &&
        ps_set(env, jobj, jcan, can, mdr::MPS_COMMAND) &&
        ps_set(env, jobj, jcan, can, mdr::MPS_FIRST_ARGUMENT) &&
        ps_set(env, jobj, jcan, can, mdr::MPS_CPU_TIME) &&
        ps_set(env, jobj, jcan, can, mdr::MPS_USR_TIME) &&
        ps_set(env, jobj, jcan, can, mdr::MPS_KERNEL_TIME) &&
        ps_set(env, jobj, jcan, can, mdr::MPS_USR_TIME_PERCENT) &&
        ps_set(env, jobj, jcan, can, mdr::MPS_KERNEL_TIME_PERCENT) &&
        ps_set(env, jobj, jcan, can, mdr::MPS_SIZE_K) &&
        ps_set(env, jobj, jcan, can, mdr::MPS_RSSIZE_K) &&
        ps_set(env, jobj, jcan, can, mdr::MPS_IO_K) &&
        ps_set(env, jobj, jcan, can, mdr::MPS_CPU_PERCENT) &&
        ps_set(env, jobj, jcan, can, mdr::MPS_MEMORY_PERCENT) &&
        ps_set(env, jobj, jcan, can, mdr::MPS_VOLUNTARY_CSW) &&
        ps_set(env, jobj, jcan, can, mdr::MPS_INVOLUNTARY_CSW) &&
        ps_set(env, jobj, jcan, can, mdr::MPS_DISK_FAULTS) &&
        ps_set(env, jobj, jcan, can, mdr::MPS_PAGE_RECLAIMS) &&
        ps_set(env, jobj, jcan, can, mdr::MPS_STATE) &&
        ps_set(env, jobj, jcan, can, mdr::MPS_PRIORITY) &&
        ps_set(env, jobj, jcan, can, mdr::MPS_NICE) &&
        ps_set(env, jobj, jcan, can, mdr::MPS_THREAD_COUNT) &&
        ps_set(env, jobj, jcan, can, mdr::MPS_SIZE_DELTA_K) &&
        ps_set(env, jobj, jcan, can, mdr::MPS_RSSIZE_DELTA_K) &&
        ps_set(env, jobj, jcan, can, mdr::MPS_USER_NAME) &&
        ps_set(env, jobj, jcan, can, mdr::MPS_EFFECTIVE_USER_NAME));
}

extern "C" JNIEXPORT jboolean JNICALL
Java_com_capmet_metrics_jmdr_mdr_1ps_1t_mps_1next_1sorted_1ps
    (JNIEnv *env, jobject jobj, jobject jcan)
{
    jclass cls = env->GetObjectClass(jobj);
    jfieldID fid;
    mdr::mdr_ps_t *ps;
    std::string name;
    mdr::mps_can_t can;

    // find handle
    fid = env->GetFieldID(cls, "handle", "J");
    if (fid == 0)
    {
        fail(env, jobj, cls, "mdr_ps_t::mps_next_sorted_ps: cannot find handle");
        return JNI_FALSE;
    }

    // get the ps pointer
    ps = (mdr::mdr_ps_t *) _POINTER_CAST_ env->GetLongField(jobj, fid);
    if (ps == 0)
    {
        fail(env, jobj, cls, "mdr_ps_t::mps_next_sorted_ps: null mdr_ps_t handle");
        return JNI_FALSE;
    }

    try
    {
        // call method
        if (ps->mps_next_sorted_proc(can) == false)
            return JNI_FALSE;
    }
    catch(std::exception &e)
    {
        fail(env, jobj, cls, e.what());
        return JNI_FALSE;
    }

    // now assign values from can to can
    return (ps_set(env, jobj, jcan, can) ? JNI_TRUE : JNI_FALSE);
}

extern "C" JNIEXPORT jint JNICALL
Java_com_capmet_metrics_jmdr_mdr_1ps_1t_mdr_1size
    (JNIEnv *env, jobject jobj)
{
    jclass cls = env->GetObjectClass(jobj);
    jfieldID fid;
    mdr::mdr_ps_t *ps;

    // find handle
    fid = env->GetFieldID(cls, "handle", "J");
    if (fid == 0)
    {
        fail(env, jobj, cls, "mdr_ps_t::mdr_size: cannot find handle");
        return JNI_FALSE;
    }

    // get the ps pointer
    ps = (mdr::mdr_ps_t *) _POINTER_CAST_ env->GetLongField(jobj, fid);
    if (ps == 0)
    {
        fail(env, jobj, cls, "mdr_ps_t::mdr_size: null mdr_ps_t handle");
        return JNI_FALSE;
    }

    return ps->size();
}

extern "C" JNIEXPORT jobjectArray JNICALL
Java_com_capmet_metrics_jmdr_mdr_1ps_1t_mdr_1keyset
    (JNIEnv *env, jobject jobj)
{
    jclass cls = env->GetObjectClass(jobj);
    jfieldID fid;
    mdr::mdr_ps_t *ps;
    jobjectArray jarray;

    // find handle
    fid = env->GetFieldID(cls, "handle", "J");
    if (fid == 0)
    {
        fail(env, jobj, cls, "mdr_ps_t::mdr_keyset: cannot find handle");
        return JNI_FALSE;
    }

    // get the ps pointer
    ps = (mdr::mdr_ps_t *) _POINTER_CAST_ env->GetLongField(jobj, fid);
    if (ps == 0)
    {
        fail(env, jobj, cls, "mdr_ps_t::mdr_keyset: null mdr_ps_t handle");
        return JNI_FALSE;
    }

    // retrieve the keys
    mdr::mdr_keyset_t keyset;
    mdr::mdr_key_t kn;
    int i;

    try
    {
        ps->mdr_keyset(keyset);
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
Java_com_capmet_metrics_jmdr_mdr_1ps_1t_mdr_1get
    (JNIEnv *env, jobject jobj, jstring jkey, jobject jcan)
{
    jclass cls = env->GetObjectClass(jobj);
    jfieldID fid;
    mdr::mdr_ps_t *ps;

    // find handle
    fid = env->GetFieldID(cls, "handle", "J");
    if (fid == 0)
    {
        fail(env, jobj, cls, "mdr_ps_t::mdr_get: cannot find handle");
        return JNI_FALSE;
    }

    // get the ps pointer
    ps = (mdr::mdr_ps_t *) _POINTER_CAST_ env->GetLongField(jobj, fid);
    if (ps == 0)
    {
        fail(env, jobj, cls, "mdr_ps_t::mdr_get: null mdr_ps_t handle");
        return JNI_FALSE;
    }

    // retrive the key and perform lookup
    const char *key;
    mdr::mps_node_t node;

    try
    {
        key = env->GetStringUTFChars(jkey, 0);
        node = ps->find(key);
        env->ReleaseStringUTFChars(jkey, key);
    }
    catch(std::exception &e)
    {
        fail(env, jobj, cls, e.what());
        return JNI_FALSE;
    }

    // not there
    if (node == ps->end())
        return JNI_FALSE;

    // now assign values from can to can
    return (ps_set(env, jobj, jcan, node->second) ? JNI_TRUE : JNI_FALSE);
}

extern "C" JNIEXPORT jboolean JNICALL
Java_com_capmet_metrics_jmdr_mdr_1ps_1t_mdr_1get_1field
    (JNIEnv *env, jobject jobj, jstring jkey, jobject jcan, jint field_number)
{
    jclass cls = env->GetObjectClass(jobj);
    jfieldID fid;
    mdr::mdr_ps_t *ps;

    // find handle
    fid = env->GetFieldID(cls, "handle", "J");
    if (fid == 0)
    {
        fail(env, jobj, cls, "mdr_ps_t::mdr_get_field: cannot find handle");
        return JNI_FALSE;
    }

    // get the ps pointer
    ps = (mdr::mdr_ps_t *) _POINTER_CAST_ env->GetLongField(jobj, fid);
    if (ps == 0)
    {
        fail(env, jobj, cls, "mdr_ps_t::mdr_get_field: null mdr_ps_t handle");
        return JNI_FALSE;
    }

    // retrive the key and perform lookup
    const char *key;
    mdr::mps_node_t node;

    try
    {
        key = env->GetStringUTFChars(jkey, 0);
        node = ps->find(key);
        env->ReleaseStringUTFChars(jkey, key);
    }
    catch(std::exception &e)
    {
        fail(env, jobj, cls, e.what());
        return JNI_FALSE;
    }

    // not there
    if (node == ps->end())
        return JNI_FALSE;

    // now assign values from can to can
    return (ps_set(env, jobj, jcan, node->second, field_number) ? JNI_TRUE : JNI_FALSE);
}

extern "C" JNIEXPORT jboolean JNICALL
Java_com_capmet_metrics_jmdr_mdr_1ps_1t_ps_1sort
  (JNIEnv *env, jobject jobj, jobject jkey, jboolean jreverse)
{
    jclass cls = env->GetObjectClass(jobj);
    jclass kcls = env->GetObjectClass(jkey);
    jfieldID fid;
    mdr::mdr_ps_t *ps;
    mdr::mdr_ps_t::mps_sort_key_t key;
    bool reverse;

    // find handle
    fid = env->GetFieldID(cls, "handle", "J");
    if (fid == 0)
    {
        fail(env, jobj, cls, "mdr_ps_t::ps_sort: cannot find handle");
        return JNI_FALSE;
    }

    // get the ps pointer
    ps = (mdr::mdr_ps_t *) _POINTER_CAST_ env->GetLongField(jobj, fid);
    if (ps == 0)
    {
        fail(env, jobj, cls, "mdr_ps_t::ps_sort: null mdr_ps_t handle");
        return JNI_FALSE;
    }

    fid = env->GetFieldID(kcls, "value", "I");
    if (fid == 0)
    {
        fail(env, jobj, cls, "mdr_ps_t::ps_sort: cannot find value");
        return JNI_FALSE;
    }
    key = (mdr::mdr_ps_t::mps_sort_key_t) env->GetIntField(jkey, fid);
    reverse = (jreverse == JNI_TRUE ? true : false);

    try
    {
        ps->mps_sort(key, reverse);
    }
    catch(std::exception &e)
    {
        fail(env, jobj, cls, e.what());
        return JNI_FALSE;
    }

    return JNI_TRUE;
}

extern "C" JNIEXPORT jboolean JNICALL
Java_com_capmet_metrics_jmdr_mdr_1ps_1t_mps_1rewind
    (JNIEnv *env, jobject jobj)
{
    jclass cls = env->GetObjectClass(jobj);
    jfieldID fid;
    mdr::mdr_ps_t *ps;

    // find handle
    fid = env->GetFieldID(cls, "handle", "J");
    if (fid == 0)
    {
        fail(env, jobj, cls, "mdr_ps_t::mps_rewind: cannot find handle");
        return JNI_FALSE;
    }

    // get the ps pointer
    ps = (mdr::mdr_ps_t *) _POINTER_CAST_ env->GetLongField(jobj, fid);
    if (ps == 0)
    {
        fail(env, jobj, cls, "mdr_ps_t::mps_rewind: null mdr_ps_t handle");
        return JNI_FALSE;
    }

    try
    {
        ps->mps_rewind();
    }
    catch(std::exception &e)
    {
        fail(env, jobj, cls, e.what());
        return JNI_FALSE;
    }

    return JNI_TRUE;
}

extern "C" JNIEXPORT jstring JNICALL
Java_com_capmet_metrics_jmdr_mdr_1ps_1t_ps_1dev_1to_1tty
  (JNIEnv *env, jobject jobj, jlong dev)
{
    jclass cls = env->GetObjectClass(jobj);
    jfieldID fid;
    mdr::mdr_ps_t *ps;
    std::string tty;

    // find handle
    fid = env->GetFieldID(cls, "handle", "J");
    if (fid == 0)
    {
        fail(env, jobj, cls, "mdr_ps_t::ps_dev_to_tty: cannot find handle");
        return 0;
    }

    // get the ps pointer
    ps = (mdr::mdr_ps_t *) _POINTER_CAST_ env->GetLongField(jobj, fid);
    if (ps == 0)
    {
        fail(env, jobj, cls, "mdr_ps_t::ps_dev_to_tty: null mdr_ps_t handle");
        return 0;
    }

    try
    {
        tty = ps->mps_dev_to_tty((long) dev, tty);
    }
    catch(std::exception &e)
    {
        fail(env, jobj, cls, e.what());
        return 0;
    }

    return env->NewStringUTF(tty.c_str());
}
