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
#include <mdr_thread.h>
#include <jni.h>

extern bool mdr_constructor(JNIEnv *, jobject, mdr::mdr_t *);

static void fail(JNIEnv *env, jobject jobj, jclass cls, const char *msg)
{
    jfieldID reason;
    jstring jstr;

    reason = env->GetFieldID(cls, "reason", "Ljava/lang/String;");
    if (reason == 0)
    {
        std::cerr << "mdr_thread_t::fail: cannot fail" << std::endl;
        return;
    }
    jstr = env->NewStringUTF(msg);
    env->SetObjectField(jobj, reason, jstr);
}

extern "C" JNIEXPORT jboolean JNICALL
Java_com_capmet_metrics_jmdr_mdr_1thread_1t_thread_1constructor
	(JNIEnv *env, jobject jobj, jstring jhost)
{
    jclass cls = env->GetObjectClass(jobj);
    jfieldID fid;
    mdr::mdr_thread_t *thread;

    fid = env->GetFieldID(cls, "handle", "J");
    if (fid == 0)
    {
        fail(env, jobj, cls, "mdr_thread_t::mdr_thread_t: cannot find handle");
        return JNI_FALSE;
    }
    try
    {
        const char *remote_host = 0;

        if (jhost != 0)
            remote_host = env->GetStringUTFChars(jhost, 0);

        thread = new mdr::mdr_thread_t(remote_host);

        if (remote_host)
            env->ReleaseStringUTFChars(jhost, remote_host);

        if (!mdr_constructor(env, jobj, thread))
            return JNI_FALSE;
    }
    catch(std::exception &e)
    {
        fail(env, jobj, cls, e.what());
        return JNI_FALSE;
    }
    env->SetLongField(jobj, fid, (jlong) _POINTER_CAST_ thread);

    return JNI_TRUE;
}

extern "C" JNIEXPORT void JNICALL
Java_com_capmet_metrics_jmdr_mdr_1thread_1t_finalize
	(JNIEnv *env, jobject jobj)
{
	jclass cls = env->GetObjectClass(jobj);
	jfieldID fid;
    mdr::mdr_thread_t *thread;

	fid = env->GetFieldID(cls, "handle", "J");
	if (fid == 0)
		return;
    thread = (mdr::mdr_thread_t *) _POINTER_CAST_ env->GetLongField(jobj, fid);
	if (thread == 0)
		return;

	delete thread;

	env->SetIntField(jobj, fid, 0);
}

extern "C" JNIEXPORT jboolean JNICALL
Java_com_capmet_metrics_jmdr_mdr_1thread_1t_thread_1refresh
	(JNIEnv *env, jobject jobj)
{
    jclass cls = env->GetObjectClass(jobj);
    jfieldID fid;
    mdr::mdr_thread_t *thread;

    fid = env->GetFieldID(cls, "handle", "J");
    if (fid == 0)
    {
        fail(env, jobj, cls, "mdr_thread_t::thread_refresh: cannot find handle");
        return JNI_FALSE;
    }
    thread = (mdr::mdr_thread_t *) _POINTER_CAST_ env->GetLongField(jobj, fid);
    if (thread == 0)
    {
        fail(env, jobj, cls, "mdr_thread_t::thread_refresh: null mdr_thread_t handle");
        return JNI_FALSE;
    }
    try
    {
        thread->mdr_refresh();
    }
    catch(std::exception &e)
    {
        fail(env, jobj, cls, e.what());
        return JNI_FALSE;
    }

	return JNI_TRUE;
}

extern "C" JNIEXPORT jboolean JNICALL
Java_com_capmet_metrics_jmdr_mdr_1thread_1t_thread_1rates
	(JNIEnv *env, jobject jobj)
{
    jclass cls = env->GetObjectClass(jobj);
    jfieldID fid;
    mdr::mdr_thread_t *thread;

    fid = env->GetFieldID(cls, "handle", "J");
    if (fid == 0)
    {
        fail(env, jobj, cls, "mdr_thread_t::thread_rates: cannot find handle");
        return JNI_FALSE;
    }
    thread = (mdr::mdr_thread_t *) _POINTER_CAST_ env->GetLongField(jobj, fid);
    if (thread == 0)
    {
        fail(env, jobj, cls, "mdr_thread_t::thread_rates: null mdr_thread_t handle");
        return JNI_FALSE;
    }
    try
    {
        thread->mdr_rates();
    }
    catch(std::exception &e)
    {
        fail(env, jobj, cls, e.what());
        return JNI_FALSE;
    }

    return JNI_TRUE;
}

static bool thread_set
    (JNIEnv *env, jobject jobj, jobject jcan,
        mdr::mt_can_t &can, jint field_number)
{
    jclass cls = env->GetObjectClass(jobj);
    jclass ccls = env->GetObjectClass(jcan);
    jfieldID fid;

    switch(field_number)
    {
	case mdr::MT_THREAD_ID:
		//
		// mt_thread_id
		//
		fid = env->GetFieldID(ccls, "mt_thread_id", "I");
		if (fid == 0)
		{
			fail(env, jobj, cls, "mdr_thread_t::thread_set: cannot find mt_thread_id");
			return false;
		}
		env->SetIntField(jcan, fid, (int) can.mt_thread_id);
		break;
	case mdr::MT_CPU_TIME:
		//
		// mt_cpu_time
		//
		fid = env->GetFieldID(ccls, "mt_cpu_time", "D");
		if (fid == 0)
		{
			fail(env, jobj, cls, "mdr_thread_t::thread_set: cannot find mt_cpu_time");
			return false;
		}
		env->SetDoubleField(jcan, fid, can.mt_cpu_time);
		break;
	case mdr::MT_USR_TIME:
		//
		// mt_user_time
		//
		fid = env->GetFieldID(ccls, "mt_user_time", "D");
		if (fid == 0)
		{
			fail(env, jobj, cls, "mdr_thread_t::thread_set: cannot find mt_user_time");
			return false;
		}
		env->SetDoubleField(jcan, fid, can.mt_user_time);
		break;
	case mdr::MT_KERNEL_TIME:
		//
		// mt_kernel_time
		//
		fid = env->GetFieldID(ccls, "mt_kernel_time", "D");
		if (fid == 0)
		{
			fail(env, jobj, cls, "mdr_thread_t::thread_set: cannot find mt_kernel_time");
			return false;
		}
		env->SetDoubleField(jcan, fid, can.mt_kernel_time);
		break;
	case mdr::MT_USR_TIME_PERCENT:
		//
		// mt_user_time_percent
		//
		fid = env->GetFieldID(ccls, "mt_user_time_percent", "D");
		if (fid == 0)
		{
			fail(env, jobj, cls, "mdr_thread_t::thread_set: cannot find mt_user_time_percent");
			return false;
		}
		env->SetDoubleField(jcan, fid, can.mt_user_time_percent);
		break;
	case mdr::MT_KERNEL_TIME_PERCENT:
		//
		// mt_kernel_time_percent
		//
		fid = env->GetFieldID(ccls, "mt_kernel_time_percent", "D");
		if (fid == 0)
		{
			fail(env, jobj, cls, "mdr_thread_t::thread_set: cannot find mt_kernel_time_percent");
			return false;
		}
		env->SetDoubleField(jcan, fid, can.mt_kernel_time_percent);
		break;
	case mdr::MT_SIZE_K:
		//
		// mt_size_K
		//
		fid = env->GetFieldID(ccls, "mt_size_K", "J");
		if (fid == 0)
		{
			fail(env, jobj, cls, "mdr_thread_t::thread_set: cannot find mt_size_K");
			return false;
		}
		env->SetLongField(jcan, fid, can.mt_size_K);
		break;
	case mdr::MT_RSSIZE_K:
		//
		// mt_rssize_K
		//
		fid = env->GetFieldID(ccls, "mt_rssize_K", "J");
		if (fid == 0)
		{
			fail(env, jobj, cls, "mdr_thread_t::thread_set: cannot find mt_rssize_K");
			return false;
		}
		env->SetLongField(jcan, fid, can.mt_rssize_K);
		break;
	case mdr::MT_IO_K:
		//
		// mt_io_K
		//
		fid = env->GetFieldID(ccls, "mt_io_K", "J");
		if (fid == 0)
		{
			fail(env, jobj, cls, "mdr_thread_t::thread_set: cannot find mt_io_K");
			return false;
		}
		env->SetLongField(jcan, fid, can.mt_io_K);
		break;
	case mdr::MT_CPU_PERCENT:
		//
		// mt_cpu_percent
		//
		fid = env->GetFieldID(ccls, "mt_cpu_percent", "D");
		if (fid == 0)
		{
			fail(env, jobj, cls, "mdr_thread_t::thread_set: cannot find mt_cpu_percent");
			return false;
		}
		env->SetDoubleField(jcan, fid, can.mt_cpu_percent);
		break;
	case mdr::MT_MEMORY_PERCENT:
		//
		// mt_memory_percent
		//
		fid = env->GetFieldID(ccls, "mt_memory_percent", "D");
		if (fid == 0)
		{
			fail(env, jobj, cls, "mdr_thread_t::thread_set: cannot find mt_memory_percent");
			return false;
		}
		env->SetDoubleField(jcan, fid, can.mt_memory_percent);
		break;
	case mdr::MT_VOLUNTARY_CSW:
		//
		// mt_voluntary_csw
		//
		fid = env->GetFieldID(ccls, "mt_voluntary_csw", "I");
		if (fid == 0)
		{
			fail(env, jobj, cls, "mdr_thread_t::thread_set: cannot find mt_voluntary_csw");
			return false;
		}
		env->SetIntField(jcan, fid, (int) can.mt_voluntary_csw);
		break;
	case mdr::MT_INVOLUNTARY_CSW:
		//
		// mt_involuntary_csw
		//
		fid = env->GetFieldID(ccls, "mt_involuntary_csw", "I");
		if (fid == 0)
		{
			fail(env, jobj, cls, "mdr_thread_t::thread_set: cannot find mt_involuntary_csw");
			return false;
		}
		env->SetIntField(jcan, fid, (int) can.mt_involuntary_csw);
		break;
	case mdr::MT_DISK_FAULTS:
		//
		// mt_disk_faults
		//
		fid = env->GetFieldID(ccls, "mt_disk_faults", "I");
		if (fid == 0)
		{
			fail(env, jobj, cls, "mdr_thread_t::thread_set: cannot find mt_disk_faults");
			return false;
		}
		env->SetIntField(jcan, fid, (int) can.mt_disk_faults);
		break;
	case mdr::MT_PAGE_RECLAIMS:
		//
		// mt_page_reclaims
		//
		fid = env->GetFieldID(ccls, "mt_page_reclaims", "I");
		if (fid == 0)
		{
			fail(env, jobj, cls, "mdr_thread_t::thread_set: cannot find mt_page_reclaims");
			return false;
		}
		env->SetIntField(jcan, fid, (int) can.mt_page_reclaims);
		break;
	case mdr::MT_STATE:
		//
		// mt_state
		//
		fid = env->GetFieldID(ccls, "mt_state", "I");
		if (fid == 0)
		{
			fail(env, jobj, cls, "mdr_thread_t::thread_set: cannot find mt_state");
			return false;
		}
		env->SetIntField(jcan, fid, (int) can.mt_state);
		break;
	case mdr::MT_PRIORITY:
		//
		// mt_priority
		//
		fid = env->GetFieldID(ccls, "mt_priority", "I");
		if (fid == 0)
		{
			fail(env, jobj, cls, "mdr_thread_t::thread_set: cannot find mt_priority");
			return false;
		}
		env->SetIntField(jcan, fid, (int) can.mt_priority);
		break;
	case mdr::MT_NICE:
		//
		// mt_nice
		//
		fid = env->GetFieldID(ccls, "mt_nice", "I");
		if (fid == 0)
		{
			fail(env, jobj, cls, "mdr_thread_t::thread_set: cannot find mt_nice");
			return false;
		}
		env->SetIntField(jcan, fid, (int) can.mt_nice);
		break;
	case mdr::MT_SIZE_DELTA_K:
		//
		// mt_size_delta_K
		//
		fid = env->GetFieldID(ccls, "mt_size_delta_K", "J");
		if (fid == 0)
		{
			fail(env, jobj, cls, "mdr_thread_t::thread_set: cannot find mt_size_delta_K");
			return false;
		}
		env->SetLongField(jcan, fid, can.mt_size_delta_K);
		break;
	case mdr::MT_RSSIZE_DELTA_K:
		//
		// mt_rssize_delta_K
		//
		fid = env->GetFieldID(ccls, "mt_rssize_delta_K", "J");
		if (fid == 0)
		{
			fail(env, jobj, cls, "mdr_thread_t::thread_set: cannot find mt_rssize_delta_K");
			return false;
		}
		env->SetLongField(jcan, fid, can.mt_rssize_delta_K);
		break;

	default:
		{
		char error[128];

		snprintf(error, sizeof error, "mdr_thread_t::thread_set: field: %d not found", (int)field_number);
        fail(env, jobj, cls, error);
        return false;
		}
	}

    return true;
}

static bool thread_set
    (JNIEnv *env, jobject jobj, jobject jcan, mdr::mt_can_t &can)
{
    return(
		thread_set(env, jobj, jcan, can, mdr::MT_THREAD_ID) &&
		thread_set(env, jobj, jcan, can, mdr::MT_CPU_TIME) &&
		thread_set(env, jobj, jcan, can, mdr::MT_USR_TIME) &&
		thread_set(env, jobj, jcan, can, mdr::MT_KERNEL_TIME) &&
		thread_set(env, jobj, jcan, can, mdr::MT_USR_TIME_PERCENT) &&
		thread_set(env, jobj, jcan, can, mdr::MT_KERNEL_TIME_PERCENT) &&
		thread_set(env, jobj, jcan, can, mdr::MT_SIZE_K) &&
		thread_set(env, jobj, jcan, can, mdr::MT_RSSIZE_K) &&
		thread_set(env, jobj, jcan, can, mdr::MT_IO_K) &&
		thread_set(env, jobj, jcan, can, mdr::MT_CPU_PERCENT) &&
		thread_set(env, jobj, jcan, can, mdr::MT_MEMORY_PERCENT) &&
		thread_set(env, jobj, jcan, can, mdr::MT_VOLUNTARY_CSW) &&
		thread_set(env, jobj, jcan, can, mdr::MT_INVOLUNTARY_CSW) &&
		thread_set(env, jobj, jcan, can, mdr::MT_DISK_FAULTS) &&
		thread_set(env, jobj, jcan, can, mdr::MT_PAGE_RECLAIMS) &&
		thread_set(env, jobj, jcan, can, mdr::MT_STATE) &&
		thread_set(env, jobj, jcan, can, mdr::MT_PRIORITY) &&
		thread_set(env, jobj, jcan, can, mdr::MT_NICE) &&
		thread_set(env, jobj, jcan, can, mdr::MT_SIZE_DELTA_K) &&
		thread_set(env, jobj, jcan, can, mdr::MT_RSSIZE_DELTA_K));
}

extern "C" JNIEXPORT jboolean JNICALL
Java_com_capmet_metrics_jmdr_mdr_1thread_1t_mt_1next_1sorted_1thread
	(JNIEnv *env, jobject jobj, jobject jcan)
{
    jclass cls = env->GetObjectClass(jobj);
    jfieldID fid;
    mdr::mdr_thread_t *thread;
    std::string name;
    mdr::mt_can_t can;

    // find handle
    fid = env->GetFieldID(cls, "handle", "J");
    if (fid == 0)
    {
        fail(env, jobj, cls, "mdr_thread_t::mt_next_sorted_thread: cannot find handle");
        return JNI_FALSE;
    }

    // get the thread pointer
    thread = (mdr::mdr_thread_t *) _POINTER_CAST_ env->GetLongField(jobj, fid);
    if (thread == 0)
    {
        fail(env, jobj, cls, "mdr_thread_t::mt_next_sorted_thread: null mdr_thread_t handle");
        return JNI_FALSE;
    }

    try
    {
        // call method
        if (thread->mt_next_sorted_thread(can) == false)
            return JNI_FALSE;
    }
    catch(std::exception &e)
    {
        fail(env, jobj, cls, e.what());
        return JNI_FALSE;
    }

    // now assign values from can to can
    return (thread_set(env, jobj, jcan, can) ? JNI_TRUE : JNI_FALSE);
}

extern "C" JNIEXPORT jint JNICALL
Java_com_capmet_metrics_jmdr_mdr_1thread_1t_mdr_1size
	(JNIEnv *env, jobject jobj)
{
    jclass cls = env->GetObjectClass(jobj);
    jfieldID fid;
    mdr::mdr_thread_t *thread;

    // find handle
    fid = env->GetFieldID(cls, "handle", "J");
    if (fid == 0)
    {
        fail(env, jobj, cls, "mdr_thread_t::mdr_size: cannot find handle");
        return JNI_FALSE;
    }

    // get the thread pointer
    thread = (mdr::mdr_thread_t *) _POINTER_CAST_ env->GetLongField(jobj, fid);
    if (thread == 0)
    {
        fail(env, jobj, cls, "mdr_thread_t::mdr_size: null mdr_thread_t handle");
        return JNI_FALSE;
    }

    return thread->size();
}

extern "C" JNIEXPORT jobjectArray JNICALL
Java_com_capmet_metrics_jmdr_mdr_1thread_1t_mdr_1keyset
	(JNIEnv *env, jobject jobj)
{
    jclass cls = env->GetObjectClass(jobj);
    jfieldID fid;
    mdr::mdr_thread_t *thread;
    jobjectArray jarray;

    // find handle
    fid = env->GetFieldID(cls, "handle", "J");
    if (fid == 0)
    {
        fail(env, jobj, cls, "mdr_thread_t::mdr_keyset: cannot find handle");
        return JNI_FALSE;
    }

    // get the thread pointer
    thread = (mdr::mdr_thread_t *) _POINTER_CAST_ env->GetLongField(jobj, fid);
    if (thread == 0)
    {
        fail(env, jobj, cls, "mdr_thread_t::mdr_keyset: null mdr_thread_t handle");
        return JNI_FALSE;
    }

    // retrieve the keys
    mdr::mdr_keyset_t keyset;
    mdr::mdr_key_t kn;
    int i;

    try
    {
        thread->mdr_keyset(keyset);
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
Java_com_capmet_metrics_jmdr_mdr_1thread_1t_mdr_1get
    (JNIEnv *env, jobject jobj, jstring jkey, jobject jcan)
{
    jclass cls = env->GetObjectClass(jobj);
    jfieldID fid;
    mdr::mdr_thread_t *thread;

    // find handle
    fid = env->GetFieldID(cls, "handle", "J");
    if (fid == 0)
    {
        fail(env, jobj, cls, "mdr_thread_t::mdr_get: cannot find handle");
        return JNI_FALSE;
    }

    // get the thread pointer
    thread = (mdr::mdr_thread_t *) _POINTER_CAST_ env->GetLongField(jobj, fid);
    if (thread == 0)
    {
        fail(env, jobj, cls, "mdr_thread_t::mdr_get: null mdr_thread_t handle");
        return JNI_FALSE;
    }

    // retrive the key and perform lookup
    const char *key;
    mdr::mt_node_t node;

    try
    {
        key = env->GetStringUTFChars(jkey, 0);
        node = thread->find(key);
        env->ReleaseStringUTFChars(jkey, key);
    }
    catch(std::exception &e)
    {
        fail(env, jobj, cls, e.what());
        return JNI_FALSE;
    }

    // not there
    if (node == thread->end())
        return JNI_FALSE;

    // now assign values from can to can
    return (thread_set(env, jobj, jcan, node->second) ? JNI_TRUE : JNI_FALSE);
}

extern "C" JNIEXPORT jboolean JNICALL
Java_com_capmet_metrics_jmdr_mdr_1thread_1t_mdr_1get_1field
    (JNIEnv *env, jobject jobj, jstring jkey, jobject jcan, jint field_number)
{
    jclass cls = env->GetObjectClass(jobj);
    jfieldID fid;
    mdr::mdr_thread_t *thread;

    // find handle
    fid = env->GetFieldID(cls, "handle", "J");
    if (fid == 0)
    {
        fail(env, jobj, cls, "mdr_thread_t::mdr_get_field: cannot find handle");
        return JNI_FALSE;
    }

    // get the thread pointer
    thread = (mdr::mdr_thread_t *) _POINTER_CAST_ env->GetLongField(jobj, fid);
    if (thread == 0)
    {
        fail(env, jobj, cls, "mdr_thread_t::mdr_get_field: null mdr_thread_t handle");
        return JNI_FALSE;
    }

    // retrive the key and perform lookup
    const char *key;
    mdr::mt_node_t node;

    try
    {
        key = env->GetStringUTFChars(jkey, 0);
        node = thread->find(key);
        env->ReleaseStringUTFChars(jkey, key);
    }
    catch(std::exception &e)
    {
        fail(env, jobj, cls, e.what());
        return JNI_FALSE;
    }

    // not there
    if (node == thread->end())
        return JNI_FALSE;

    // now assign values from can to can
    return (thread_set(env, jobj, jcan, node->second, field_number) ? JNI_TRUE : JNI_FALSE);
}

extern "C" JNIEXPORT jboolean JNICALL
Java_com_capmet_metrics_jmdr_mdr_1thread_1t_thread_1sort
  (JNIEnv *env, jobject jobj, jobject jkey, jboolean jreverse)
{
    jclass cls = env->GetObjectClass(jobj);
	jclass kcls = env->GetObjectClass(jkey);
    jfieldID fid;
    mdr::mdr_thread_t *thread;
	mdr::mdr_thread_t::mt_sort_key_t key;
	bool reverse;

    // find handle
    fid = env->GetFieldID(cls, "handle", "J");
    if (fid == 0)
    {
        fail(env, jobj, cls, "mdr_thread_t::thread_sort: cannot find handle");
        return JNI_FALSE;
    }

    // get the thread pointer
    thread = (mdr::mdr_thread_t *) _POINTER_CAST_ env->GetLongField(jobj, fid);
    if (thread == 0)
    {
        fail(env, jobj, cls, "mdr_thread_t::thread_sort: null mdr_thread_t handle");
        return JNI_FALSE;
    }

	fid = env->GetFieldID(kcls, "value", "I");
	if (fid == 0)
	{
		fail(env, jobj, cls, "mdr_thread_t::thread_sort: cannot find value");
		return JNI_FALSE;
	}
	key = (mdr::mdr_thread_t::mt_sort_key_t) env->GetIntField(jkey, fid);
	reverse = (jreverse == JNI_TRUE ? true : false);

	try
	{
		thread->mt_sort(key, reverse);
	}
    catch(std::exception &e)
    {
        fail(env, jobj, cls, e.what());
        return JNI_FALSE;
    }

	return JNI_TRUE;
}

extern "C" JNIEXPORT jboolean JNICALL
Java_com_capmet_metrics_jmdr_mdr_1thread_1t_mt_1rewind
	(JNIEnv *env, jobject jobj)
{
    jclass cls = env->GetObjectClass(jobj);
    jfieldID fid;
    mdr::mdr_thread_t *thread;

    // find handle
    fid = env->GetFieldID(cls, "handle", "J");
    if (fid == 0)
    {
        fail(env, jobj, cls, "mdr_thread_t::mt_rewind: cannot find handle");
        return JNI_FALSE;
    }

    // get the thread pointer
    thread = (mdr::mdr_thread_t *) _POINTER_CAST_ env->GetLongField(jobj, fid);
    if (thread == 0)
    {
        fail(env, jobj, cls, "mdr_thread_t::mt_rewind: null mdr_thread_t handle");
        return JNI_FALSE;
    }

	try
	{
		thread->mt_rewind();
	}
    catch(std::exception &e)
    {
        fail(env, jobj, cls, e.what());
        return JNI_FALSE;
    }

	return JNI_TRUE;
}
