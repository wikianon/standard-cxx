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
#include <mdr_misc.h>
#include <jni.h>

extern bool mdr_constructor(JNIEnv *, jobject, mdr::mdr_t *);

static void fail(JNIEnv *env, jobject jobj, jclass cls, const char *msg)
{
    jfieldID reason;
    jstring jstr;

    reason = env->GetFieldID(cls, "reason", "Ljava/lang/String;");
    if (reason == 0)
    {
        std::cerr << "mdr_misc_t::fail: cannot fail" << std::endl;
        return;
    }
    jstr = env->NewStringUTF(msg);
    env->SetObjectField(jobj, reason, jstr);
}

static bool misc_set(JNIEnv *env, jobject jobj, jclass cls,
                     mdr::mdr_misc_t *misc)
{
    jfieldID fid;
    jstring jstr;

    // 
    // mm_hostname
    // 
    fid = env->GetFieldID(cls, "mm_hostname", "Ljava/lang/String;");
    if (fid == 0)
    {
        fail(env, jobj, cls, "mdr_misc_t::misc_set: cannot find mm_hostname");
        return false;
    }
    jstr = env->NewStringUTF(misc->mm_hostname.c_str());
    env->SetObjectField(jobj, fid, jstr);

    // 
    // mm_os_platform
    // 
    fid = env->GetFieldID(cls, "mm_os_platform", "Ljava/lang/String;");
    if (fid == 0)
    {
        fail(env, jobj, cls, "mdr_misc_t::misc_set: cannot find mm_os_platform");
        return false;
    }
    jstr = env->NewStringUTF(misc->mm_os_platform.c_str());
    env->SetObjectField(jobj, fid, jstr);

    // 
    // mm_os_version
    // 
    fid = env->GetFieldID(cls, "mm_os_version", "Ljava/lang/String;");
    if (fid == 0)
    {
        fail(env, jobj, cls, "mdr_misc_t::misc_set: cannot find mm_os_version");
        return false;
    }
    jstr = env->NewStringUTF(misc->mm_os_version.c_str());
    env->SetObjectField(jobj, fid, jstr);

    // 
    // mm_physical_pages
    // 
    fid = env->GetFieldID(cls, "mm_physical_pages", "I");
    if (fid == 0)
    {
        fail(env, jobj, cls, "mdr_misc_t::misc_set: cannot find mm_physical_pages");
        return false;
    }
    env->SetIntField(jobj, fid, (int) misc->mm_physical_pages);

    // 
    // mm_native_page_size
    // 
    fid = env->GetFieldID(cls, "mm_native_page_size", "I");
    if (fid == 0)
    {
        fail(env, jobj, cls, "mdr_misc_t::misc_set: cannot find mm_native_page_size");
        return false;
    }
    env->SetIntField(jobj, fid, (int) misc->mm_native_page_size);

    // 
    // mm_processor_arch
    // 
    fid = env->GetFieldID(cls, "mm_processor_arch", "Ljava/lang/String;");
    if (fid == 0)
    {
        fail(env, jobj, cls, "mdr_misc_t::misc_set: cannot find mm_processor_arch");
        return false;
    }
    jstr = env->NewStringUTF(misc->mm_processor_arch.c_str());
    env->SetObjectField(jobj, fid, jstr);

    // 
    // mm_processors_configured
    // 
    fid = env->GetFieldID(cls, "mm_processors_configured", "I");
    if (fid == 0)
    {
        fail(env, jobj, cls, "mdr_misc_t::misc_set: cannot find mm_processors_configured");
        return false;
    }
    env->SetIntField(jobj, fid, misc->mm_processors_configured);

    // 
    // mm_processors_online
    // 
    fid = env->GetFieldID(cls, "mm_processors_online", "I");
    if (fid == 0)
    {
        fail(env, jobj, cls, "mdr_misc_t::misc_set: cannot find mm_processors_online");
        return false;
    }
    env->SetIntField(jobj, fid, misc->mm_processors_online);

    // 
    // mm_ticks_per_second
    // 
    fid = env->GetFieldID(cls, "mm_ticks_per_second", "I");
    if (fid == 0)
    {
        fail(env, jobj, cls, "mdr_misc_t::misc_set: cannot find mm_ticks_per_second");
        return false;
    }
    env->SetIntField(jobj, fid, misc->mm_ticks_per_second);

    // 
    // mm_boot_time
    // 
    fid = env->GetFieldID(cls, "mm_boot_time", "J");
    if (fid == 0)
    {
        fail(env, jobj, cls, "mdr_misc_t::misc_set: cannot find mm_boot_time");
        return false;
    }
    env->SetLongField(jobj, fid, misc->mm_boot_time);

    // 
    // mm_interrupts
    // 
    fid = env->GetFieldID(cls, "mm_interrupts", "J");
    if (fid == 0)
    {
        fail(env, jobj, cls, "mdr_misc_t::misc_set: cannot find mm_interrupts");
        return false;
    }
    env->SetLongField(jobj, fid, misc->mm_interrupts);

    // 
    // mm_clock_interrupts
    // 
    fid = env->GetFieldID(cls, "mm_clock_interrupts", "J");
    if (fid == 0)
    {
        fail(env, jobj, cls, "mdr_misc_t::misc_set: cannot find mm_clock_interrupts");
        return false;
    }
    env->SetLongField(jobj, fid, misc->mm_clock_interrupts);

    // 
    // mm_system_calls
    // 
    fid = env->GetFieldID(cls, "mm_system_calls", "I");
    if (fid == 0)
    {
        fail(env, jobj, cls, "mdr_misc_t::misc_set: cannot find mm_system_calls");
        return false;
    }
    env->SetIntField(jobj, fid, misc->mm_system_calls);

    // 
    // mm_context_switches
    // 
    fid = env->GetFieldID(cls, "mm_context_switches", "I");
    if (fid == 0)
    {
        fail(env, jobj, cls, "mdr_misc_t::misc_set: cannot find mm_context_switches");
        return false;
    }
    env->SetIntField(jobj, fid, misc->mm_context_switches);

    // 
    // mm_process_count
    // 
    fid = env->GetFieldID(cls, "mm_process_count", "I");
    if (fid == 0)
    {
        fail(env, jobj, cls, "mdr_misc_t::misc_set: cannot find mm_process_count");
        return false;
    }
    env->SetIntField(jobj, fid, misc->mm_process_count);

    // 
    // mm_1min_run_queue
    // 
    fid = env->GetFieldID(cls, "mm_1min_run_queue", "D");
    if (fid == 0)
    {
        fail(env, jobj, cls, "mdr_misc_t::misc_set: cannot find mm_1min_run_queue");
        return false;
    }
    env->SetDoubleField(jobj, fid, misc->mm_1min_run_queue);

    // 
    // mm_5min_run_queue
    // 
    fid = env->GetFieldID(cls, "mm_5min_run_queue", "D");
    if (fid == 0)
    {
        fail(env, jobj, cls, "mdr_misc_t::misc_set: cannot find mm_5min_run_queue");
        return false;
    }
    env->SetDoubleField(jobj, fid, misc->mm_5min_run_queue);

    // 
    // mm_15min_run_queue
    // 
    fid = env->GetFieldID(cls, "mm_15min_run_queue", "D");
    if (fid == 0)
    {
        fail(env, jobj, cls, "mdr_misc_t::misc_set: cannot find mm_15min_run_queue");
        return false;
    }
    env->SetDoubleField(jobj, fid, misc->mm_15min_run_queue);

    // 
    // mm_current_time
    // 
    fid = env->GetFieldID(cls, "mm_current_time", "J");
    if (fid == 0)
    {
        fail(env, jobj, cls, "mdr_misc_t::misc_set: cannot find mm_current_time");
        return false;
    }
    env->SetLongField(jobj, fid, misc->mm_current_time);

    return true;
}

extern "C" JNIEXPORT jboolean JNICALL
Java_com_capmet_metrics_jmdr_mdr_1misc_1t_misc_1constructor
    (JNIEnv *env, jobject jobj, jstring jhost)
{
    jclass cls = env->GetObjectClass(jobj);
    jfieldID fid;
    mdr::mdr_misc_t *misc;

    fid = env->GetFieldID(cls, "handle", "J");
    if (fid == 0)
    {
        fail(env, jobj, cls, "mdr_misc_t::mdr_misc_t: cannot find handle");
        return JNI_FALSE;
    }
    try
    {
        const char *remote_host = 0;

        if (jhost != 0)
            remote_host = env->GetStringUTFChars(jhost, 0);

        misc = new mdr::mdr_misc_t(remote_host);

        if (remote_host)
            env->ReleaseStringUTFChars(jhost, remote_host);

        if (!mdr_constructor(env, jobj, misc))
            return JNI_FALSE;
    }
    catch(std::exception & e)
    {
        fail(env, jobj, cls, e.what());
        return JNI_FALSE;
    }
    env->SetLongField(jobj, fid, (jlong) _POINTER_CAST_ misc);

    return (misc_set(env, jobj, cls, misc) ? JNI_TRUE : JNI_FALSE);
}

extern "C" JNIEXPORT void JNICALL
Java_com_capmet_metrics_jmdr_mdr_1misc_1t_finalize
	(JNIEnv *env, jobject jobj)
{
	jclass cls = env->GetObjectClass(jobj);
	jfieldID fid;
    mdr::mdr_misc_t *misc;

	fid = env->GetFieldID(cls, "handle", "J");
	if (fid == 0)
		return;
    misc = (mdr::mdr_misc_t *) _POINTER_CAST_ env->GetLongField(jobj, fid);
	if (misc == 0)
		return;

	delete misc;

	env->SetIntField(jobj, fid, 0);
}

extern "C" JNIEXPORT jboolean JNICALL
Java_com_capmet_metrics_jmdr_mdr_1misc_1t_misc_1refresh
	(JNIEnv *env, jobject jobj)
{
    jclass cls = env->GetObjectClass(jobj);
    jfieldID fid;
    mdr::mdr_misc_t *misc;

    fid = env->GetFieldID(cls, "handle", "J");
    if (fid == 0)
    {
        fail(env, jobj, cls, "mdr_misc_t::misc_refresh: cannot find handle");
        return JNI_FALSE;
    }
    misc = (mdr::mdr_misc_t *) _POINTER_CAST_ env->GetLongField(jobj, fid);
    if (misc == 0)
    {
        fail(env, jobj, cls, "mdr_misc_t::misc_refresh: null mdr_misc_t handle");
        return JNI_FALSE;
    }
    try
    {
        misc->mdr_refresh();
    }
    catch(std::exception & e)
    {
        fail(env, jobj, cls, e.what());
        return JNI_FALSE;
    }

    return (misc_set(env, jobj, cls, misc) ? JNI_TRUE : JNI_FALSE);
}

extern "C" JNIEXPORT jboolean JNICALL
Java_com_capmet_metrics_jmdr_mdr_1misc_1t_misc_1rates
	(JNIEnv *env, jobject jobj)
{
    jclass cls = env->GetObjectClass(jobj);
    jfieldID fid;
    mdr::mdr_misc_t *misc;

    fid = env->GetFieldID(cls, "handle", "J");
    if (fid == 0)
    {
        fail(env, jobj, cls, "mdr_misc_t::misc_rates: cannot find handle");
        return JNI_FALSE;
    }
    misc = (mdr::mdr_misc_t *) _POINTER_CAST_ env->GetLongField(jobj, fid);
    if (misc == 0)
    {
        fail(env, jobj, cls, "mdr_misc_t::misc_rates: null mdr_misc_t handle");
        return JNI_FALSE;
    }
    try
    {
        misc->mdr_rates();
    }
    catch(std::exception & e)
    {
        fail(env, jobj, cls, e.what());
        return JNI_FALSE;
    }

    return (misc_set(env, jobj, cls, misc) ? JNI_TRUE : JNI_FALSE);
}
