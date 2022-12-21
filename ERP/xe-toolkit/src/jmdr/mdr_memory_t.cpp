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
#include <mdr_memory.h>
#include <jni.h>

extern bool mdr_constructor(JNIEnv *, jobject, mdr::mdr_t *);

static void fail(JNIEnv *env, jobject jobj, jclass cls, const char *msg)
{
    jfieldID reason;
    jstring jstr;

    reason = env->GetFieldID(cls, "reason", "Ljava/lang/String;");
    if (reason == 0)
    {
        std::cerr << "mdr_memory_t::fail: cannot fail: "
                  << msg << std::endl;
        return;
    }
    jstr = env->NewStringUTF(msg);
    env->SetObjectField(jobj, reason, jstr);
}

static bool memory_set(JNIEnv *env, jobject jobj, jclass cls,
                     mdr::mdr_memory_t *memory)
{
    jfieldID fid;

    // 
    // mmem_paged_in_K
    // 
    fid = env->GetFieldID(cls, "mmem_paged_in_K", "I");
    if (fid == 0)
    {
        fail(env, jobj, cls, "mdr_memory_t::memory_set: cannot find mmem_paged_in_K");
        return false;
    }
    env->SetIntField(jobj, fid, (int) memory->mmem_paged_in_K);

    // 
    // mmem_paged_out_K
    // 
    fid = env->GetFieldID(cls, "mmem_paged_out_K", "I");
    if (fid == 0)
    {
        fail(env, jobj, cls, "mdr_memory_t::memory_set: cannot find mmem_paged_out_K");
        return false;
    }
    env->SetIntField(jobj, fid, (int) memory->mmem_paged_out_K);

    // 
    // mmem_pages_scanned
    // 
    fid = env->GetFieldID(cls, "mmem_pages_scanned", "I");
    if (fid == 0)
    {
        fail(env, jobj, cls, "mdr_memory_t::memory_set: cannot find mmem_pages_scanned");
        return false;
    }
    env->SetIntField(jobj, fid, memory->mmem_pages_scanned);

    // 
    // mmem_free_memory_K
    // 
    fid = env->GetFieldID(cls, "mmem_free_memory_K", "I");
    if (fid == 0)
    {
        fail(env, jobj, cls, "mdr_memory_t::memory_set: cannot find mmem_free_memory_K");
        return false;
    }
    env->SetIntField(jobj, fid, memory->mmem_free_memory_K);

    // 
    // mmem_memory_freed_K
    // 
    fid = env->GetFieldID(cls, "mmem_memory_freed_K", "I");
    if (fid == 0)
    {
        fail(env, jobj, cls, "mdr_memory_t::memory_set: cannot find mmem_memory_freed_K");
        return false;
    }
    env->SetIntField(jobj, fid, memory->mmem_memory_freed_K);

    // 
    // mmem_page_faults
    // 
    fid = env->GetFieldID(cls, "mmem_page_faults", "I");
    if (fid == 0)
    {
        fail(env, jobj, cls, "mdr_memory_t::memory_set: cannot find mmem_page_faults");
        return false;
    }
    env->SetIntField(jobj, fid, memory->mmem_page_faults);

    // 
    // mmem_major_faults
    // 
    fid = env->GetFieldID(cls, "mmem_major_faults", "I");
    if (fid == 0)
    {
        fail(env, jobj, cls, "mdr_memory_t::memory_set: cannot find mmem_major_faults");
        return false;
    }
    env->SetIntField(jobj, fid, memory->mmem_major_faults);

    return true;
}

extern "C" JNIEXPORT jboolean JNICALL
Java_com_capmet_metrics_jmdr_mdr_1memory_1t_memory_1constructor
    (JNIEnv *env, jobject jobj, jstring jhost)
{
    jclass cls = env->GetObjectClass(jobj);
    jfieldID fid;
    mdr::mdr_memory_t *memory;

    fid = env->GetFieldID(cls, "handle", "J");
    if (fid == 0)
    {
        fail(env, jobj, cls, "mdr_memory_t::mdr_memory_t: cannot find handle");
        return JNI_FALSE;
    }
    try
    {
        const char *remote_host = 0;

        if (jhost != 0)
            remote_host = env->GetStringUTFChars(jhost, 0);

        memory = new mdr::mdr_memory_t(remote_host);

        if (remote_host)
            env->ReleaseStringUTFChars(jhost, remote_host);

        if (!mdr_constructor(env, jobj, memory))
            return JNI_FALSE;
    }
    catch(std::exception & e)
    {
        fail(env, jobj, cls, e.what());
        return JNI_FALSE;
    }
    env->SetLongField(jobj, fid, (jlong) _POINTER_CAST_ memory);

    return (memory_set(env, jobj, cls, memory) ? JNI_TRUE : JNI_FALSE);
}

extern "C" JNIEXPORT void JNICALL
Java_com_capmet_metrics_jmdr_mdr_1memory_1t_finalize
	(JNIEnv *env, jobject jobj)
{
	jclass cls = env->GetObjectClass(jobj);
	jfieldID fid;
    mdr::mdr_memory_t *memory;

	fid = env->GetFieldID(cls, "handle", "J");
	if (fid == 0)
		return;
    memory = (mdr::mdr_memory_t *) _POINTER_CAST_ env->GetLongField(jobj, fid);
	if (memory == 0)
		return;

	delete memory;

	env->SetIntField(jobj, fid, 0);
}

extern "C" JNIEXPORT jboolean JNICALL
Java_com_capmet_metrics_jmdr_mdr_1memory_1t_memory_1refresh
	(JNIEnv *env, jobject jobj)
{
    jclass cls = env->GetObjectClass(jobj);
    jfieldID fid;
    mdr::mdr_memory_t *memory;

    fid = env->GetFieldID(cls, "handle", "J");
    if (fid == 0)
    {
        fail(env, jobj, cls, "mdr_memory_t::memory_refresh: cannot find handle");
        return JNI_FALSE;
    }
    memory = (mdr::mdr_memory_t *) _POINTER_CAST_ env->GetLongField(jobj, fid);
    if (memory == 0)
    {
        fail(env, jobj, cls, "mdr_memory_t::memory_refresh: null mdr_memory_t handle");
        return JNI_FALSE;
    }
    try
    {
        memory->mdr_refresh();
    }
    catch(std::exception & e)
    {
        fail(env, jobj, cls, e.what());
        return JNI_FALSE;
    }

    return (memory_set(env, jobj, cls, memory) ? JNI_TRUE : JNI_FALSE);
}

extern "C" JNIEXPORT jboolean JNICALL
Java_com_capmet_metrics_jmdr_mdr_1memory_1t_memory_1rates
	(JNIEnv *env, jobject jobj)
{
    jclass cls = env->GetObjectClass(jobj);
    jfieldID fid;
    mdr::mdr_memory_t *memory;

    fid = env->GetFieldID(cls, "handle", "J");
    if (fid == 0)
    {
        fail(env, jobj, cls, "mdr_memory_t::memory_rates: cannot find handle");
        return JNI_FALSE;
    }
    memory = (mdr::mdr_memory_t *) _POINTER_CAST_ env->GetLongField(jobj, fid);
    if (memory == 0)
    {
        fail(env, jobj, cls, "mdr_memory_t::memory_rates: null mdr_memory_t handle");
        return JNI_FALSE;
    }
    try
    {
        memory->mdr_rates();
    }
    catch(std::exception & e)
    {
        fail(env, jobj, cls, e.what());
        return JNI_FALSE;
    }

    return (memory_set(env, jobj, cls, memory) ? JNI_TRUE : JNI_FALSE);
}
