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
#include <mdr_mp.h>
#include <jni.h>

extern bool mdr_constructor(JNIEnv *, jobject, mdr::mdr_t *);

static void fail(JNIEnv *env, jobject jobj, jclass cls, const char *msg)
{
    jfieldID reason;
    jstring jstr;

    reason = env->GetFieldID(cls, "reason", "Ljava/lang/String;");
    if (reason == 0)
    {
        std::cerr << "mdr_mp_t::fail: cannot fail" << std::endl;
        return;
    }
    jstr = env->NewStringUTF(msg);
    env->SetObjectField(jobj, reason, jstr);
}

extern "C" JNIEXPORT jboolean JNICALL
Java_com_capmet_metrics_jmdr_mdr_1mp_1t_mp_1constructor
    (JNIEnv *env, jobject jobj, jstring jhost)
{
    jclass cls = env->GetObjectClass(jobj);
    jfieldID fid;
    mdr::mdr_mp_t *mp;

    fid = env->GetFieldID(cls, "handle", "J");
    if (fid == 0)
    {
        fail(env, jobj, cls, "mdr_mp_t::mdr_mp_t: cannot find handle");
        return JNI_FALSE;
    }
    try
    {
        const char *remote_host = 0;

        if (jhost != 0)
            remote_host = env->GetStringUTFChars(jhost, 0);

        mp = new mdr::mdr_mp_t(remote_host);

        if (remote_host)
            env->ReleaseStringUTFChars(jhost, remote_host);

        if (!mdr_constructor(env, jobj, mp))
            return JNI_FALSE;
    }
    catch(std::exception & e)
    {
        fail(env, jobj, cls, e.what());
        return JNI_FALSE;
    }
    env->SetLongField(jobj, fid, (jlong) _POINTER_CAST_ mp);

    return JNI_TRUE;
}

extern "C" JNIEXPORT void JNICALL
Java_com_capmet_metrics_jmdr_mdr_1mp_1t_finalize
	(JNIEnv *env, jobject jobj)
{
	jclass cls = env->GetObjectClass(jobj);
	jfieldID fid;
    mdr::mdr_mp_t *mp;

	fid = env->GetFieldID(cls, "handle", "J");
	if (fid == 0)
		return;
    mp = (mdr::mdr_mp_t *) _POINTER_CAST_ env->GetLongField(jobj, fid);
	if (mp == 0)
		return;

	delete mp;

	env->SetIntField(jobj, fid, 0);
}

extern "C" JNIEXPORT jboolean JNICALL
Java_com_capmet_metrics_jmdr_mdr_1mp_1t_mp_1refresh
	(JNIEnv *env, jobject jobj)
{
    jclass cls = env->GetObjectClass(jobj);
    jfieldID fid;
    mdr::mdr_mp_t *mp;

    fid = env->GetFieldID(cls, "handle", "J");
    if (fid == 0)
    {
        fail(env, jobj, cls, "mdr_mp_t::mp_refresh: cannot find handle");
        return JNI_FALSE;
    }
    mp = (mdr::mdr_mp_t *) _POINTER_CAST_ env->GetLongField(jobj, fid);
    if (mp == 0)
    {
        fail(env, jobj, cls, "mdr_mp_t::mp_refresh: null mdr_mp_t handle");
        return JNI_FALSE;
    }
    try
    {
        mp->mdr_refresh();
    }
    catch(std::exception & e)
    {
        fail(env, jobj, cls, e.what());
        return JNI_FALSE;
    }

    return JNI_TRUE;
}

extern "C" JNIEXPORT jboolean JNICALL
Java_com_capmet_metrics_jmdr_mdr_1mp_1t_mp_1rates
	(JNIEnv *env, jobject jobj)
{
    jclass cls = env->GetObjectClass(jobj);
    jfieldID fid;
    mdr::mdr_mp_t *mp;

    fid = env->GetFieldID(cls, "handle", "J");
    if (fid == 0)
    {
        fail(env, jobj, cls, "mdr_mp_t::mp_rates: cannot find handle");
        return JNI_FALSE;
    }
    mp = (mdr::mdr_mp_t *) _POINTER_CAST_ env->GetLongField(jobj, fid);
    if (mp == 0)
    {
        fail(env, jobj, cls, "mdr_mp_t::mp_rates: null mdr_mp_t handle");
        return JNI_FALSE;
    }
    try
    {
        mp->mdr_rates();
    }
    catch(std::exception & e)
    {
        fail(env, jobj, cls, e.what());
        return JNI_FALSE;
    }

    return JNI_TRUE;
}

static bool mp_set
    (JNIEnv *env, jobject jobj, jobject jcan,
        mdr::mmp_can_t &can, jint field_number)
{
    jclass cls = env->GetObjectClass(jobj);
    jclass ccls = env->GetObjectClass(jcan);
    jfieldID fid;

    switch(field_number)
    {
    case mdr::MMP_CPU_INSTANCE:
        //
        // mmp_cpu_instance
        //
        fid = env->GetFieldID(ccls, "mmp_cpu_instance", "I");
        if (fid == 0)
        {
            fail(env, jobj, cls, "mdr_mp_t::mp_set: cannot find mmp_cpu_instance");
            return false;
        }
        env->SetIntField(jcan, fid, (int) can.mmp_cpu_instance);
        break;
    case mdr::MMP_USER_TICKS:
        //
        // mmp_user_ticks
        //
        fid = env->GetFieldID(ccls, "mmp_user_ticks", "I");
        if (fid == 0)
        {
            fail(env, jobj, cls, "mdr_mp_t::mp_set: cannot find mmp_user_ticks");
            return false;
        }
        env->SetIntField(jcan, fid, (int) can.mmp_user_ticks);
        break;
    case mdr::MMP_KERNEL_TICKS:
        //
        // mmp_kernel_ticks
        //
        fid = env->GetFieldID(ccls, "mmp_kernel_ticks", "I");
        if (fid == 0)
        {
            fail(env, jobj, cls, "mdr_mp_t::mp_set: cannot find mmp_kernel_ticks");
            return false;
        }
        env->SetIntField(jcan, fid, (int) can.mmp_kernel_ticks);
        break;
    case mdr::MMP_IDLE_TICKS:
        //
        // mmp_idle_ticks
        //
        fid = env->GetFieldID(ccls, "mmp_idle_ticks", "I");
        if (fid == 0)
        {
            fail(env, jobj, cls, "mdr_mp_t::mp_set: cannot find mmp_idle_ticks");
            return false;
        }
        env->SetIntField(jcan, fid, (int) can.mmp_idle_ticks);
        break;
    case mdr::MMP_NICE_TICKS:
        //
        // mmp_nice_ticks
        //
        fid = env->GetFieldID(ccls, "mmp_nice_ticks", "I");
        if (fid == 0)
        {
            fail(env, jobj, cls, "mdr_mp_t::mp_set: cannot find mmp_nice_ticks");
            return false;
        }
        env->SetIntField(jcan, fid, (int) can.mmp_nice_ticks);
        break;
    case mdr::MMP_WAIT_TICKS:
        //
        // mmp_wait_ticks
        //
        fid = env->GetFieldID(ccls, "mmp_wait_ticks", "I");
        if (fid == 0)
        {
            fail(env, jobj, cls, "mdr_mp_t::mp_set: cannot find mmp_wait_ticks");
            return false;
        }
        env->SetIntField(jcan, fid, (int) can.mmp_wait_ticks);
        break;
    case mdr::MMP_IRQ_TICKS:
        //
        // mmp_irq_ticks
        //
        fid = env->GetFieldID(ccls, "mmp_irq_ticks", "I");
        if (fid == 0)
        {
            fail(env, jobj, cls, "mdr_mp_t::mp_set: cannot find mmp_irq_ticks");
            return false;
        }
        env->SetIntField(jcan, fid, (int) can.mmp_irq_ticks);
        break;
    case mdr::MMP_SOFT_IRQ_TICKS:
        //
        // mmp_soft_irq_ticks
        //
        fid = env->GetFieldID(ccls, "mmp_soft_irq_ticks", "I");
        if (fid == 0)
        {
            fail(env, jobj, cls, "mdr_mp_t::mp_set: cannot find mmp_soft_irq_ticks");
            return false;
        }
        env->SetIntField(jcan, fid, (int) can.mmp_soft_irq_ticks);
        break;
    }

    return true;
}

static bool mp_set
    (JNIEnv *env, jobject jobj, jobject jcan, mdr::mmp_can_t &can)
{
    return (
        mp_set(env, jobj, jcan, can, mdr::MMP_CPU_INSTANCE) &&
        mp_set(env, jobj, jcan, can, mdr::MMP_USER_TICKS) &&
        mp_set(env, jobj, jcan, can, mdr::MMP_KERNEL_TICKS) &&
        mp_set(env, jobj, jcan, can, mdr::MMP_IDLE_TICKS) &&
        mp_set(env, jobj, jcan, can, mdr::MMP_NICE_TICKS) &&
        mp_set(env, jobj, jcan, can, mdr::MMP_WAIT_TICKS) &&
        mp_set(env, jobj, jcan, can, mdr::MMP_IRQ_TICKS) &&
        mp_set(env, jobj, jcan, can, mdr::MMP_SOFT_IRQ_TICKS));
}

extern "C" JNIEXPORT jint JNICALL
Java_com_capmet_metrics_jmdr_mdr_1mp_1t_mdr_1size
	(JNIEnv *env, jobject jobj)
{
    jclass cls = env->GetObjectClass(jobj);
    jfieldID fid;
    mdr::mdr_mp_t *mp;

    // find handle
    fid = env->GetFieldID(cls, "handle", "J");
    if (fid == 0)
    {
        fail(env, jobj, cls, "mdr_mp_t::mdr_size: cannot find handle");
        return JNI_FALSE;
    }

    // get the mp pointer
    mp = (mdr::mdr_mp_t *) _POINTER_CAST_ env->GetLongField(jobj, fid);
    if (mp == 0)
    {
        fail(env, jobj, cls, "mdr_mp_t::mdr_size: null mdr_mp_t handle");
        return JNI_FALSE;
    }

    return mp->size();
}

extern "C" JNIEXPORT jobjectArray JNICALL
Java_com_capmet_metrics_jmdr_mdr_1mp_1t_mdr_1keyset
	(JNIEnv *env, jobject jobj)
{
    jclass cls = env->GetObjectClass(jobj);
    jfieldID fid;
    mdr::mdr_mp_t *mp;
    jobjectArray jarray;

    // find handle
    fid = env->GetFieldID(cls, "handle", "J");
    if (fid == 0)
    {
        fail(env, jobj, cls, "mdr_mp_t::mdr_keyset: cannot find handle");
        return JNI_FALSE;
    }

    // get the mp pointer
    mp = (mdr::mdr_mp_t *) _POINTER_CAST_ env->GetLongField(jobj, fid);
    if (mp == 0)
    {
        fail(env, jobj, cls, "mdr_mp_t::mdr_keyset: null mdr_mp_t handle");
        return JNI_FALSE;
    }

    // retrieve the keys
    mdr::mdr_keyset_t keyset;
    mdr::mdr_key_t kn;
    int i;

    try
    {
        mp->mdr_keyset(keyset);
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
Java_com_capmet_metrics_jmdr_mdr_1mp_1t_mdr_1get
    (JNIEnv *env, jobject jobj, jstring jkey, jobject jcan)
{
    jclass cls = env->GetObjectClass(jobj);
    jfieldID fid;
    mdr::mdr_mp_t *mp;

    // find handle
    fid = env->GetFieldID(cls, "handle", "J");
    if (fid == 0)
    {
        fail(env, jobj, cls, "mdr_mp_t::mdr_get: cannot find handle");
        return JNI_FALSE;
    }

    // get the mp pointer
    mp = (mdr::mdr_mp_t *) _POINTER_CAST_ env->GetLongField(jobj, fid);
    if (mp == 0)
    {
        fail(env, jobj, cls, "mdr_mp_t::mdr_get: null mdr_mp_t handle");
        return JNI_FALSE;
    }

    // retrive the key and perform lookup
    const char *key;
    mdr::mmp_node_t node;

    try
    {
        key = env->GetStringUTFChars(jkey, 0);
        node = mp->find(key);
        env->ReleaseStringUTFChars(jkey, key);
    }
    catch(std::exception &e)
    {
        fail(env, jobj, cls, e.what());
        return JNI_FALSE;
    }

    // not there
    if (node == mp->end())
        return JNI_FALSE;

    // now assign values from can to can
    return (mp_set(env, jobj, jcan, node->second) ? JNI_TRUE : JNI_FALSE);
}

extern "C" JNIEXPORT jboolean JNICALL
Java_com_capmet_metrics_jmdr_mdr_1mp_1t_mdr_1get_1field
    (JNIEnv *env, jobject jobj, jstring jkey, jobject jcan, jint field_number)
{
    jclass cls = env->GetObjectClass(jobj);
    jfieldID fid;
    mdr::mdr_mp_t *mp;

    // find handle
    fid = env->GetFieldID(cls, "handle", "J");
    if (fid == 0)
    {
        fail(env, jobj, cls, "mdr_mp_t::mdr_get_field: cannot find handle");
        return JNI_FALSE;
    }

    // get the mp pointer
    mp = (mdr::mdr_mp_t *) _POINTER_CAST_ env->GetLongField(jobj, fid);
    if (mp == 0)
    {
        fail(env, jobj, cls, "mdr_mp_t::mdr_get_field: null mdr_mp_t handle");
        return JNI_FALSE;
    }

    // retrive the key and perform lookup
    const char *key;
    mdr::mmp_node_t node;

    try
    {
        key = env->GetStringUTFChars(jkey, 0);
        node = mp->find(key);
        env->ReleaseStringUTFChars(jkey, key);
    }
    catch(std::exception &e)
    {
        fail(env, jobj, cls, e.what());
        return JNI_FALSE;
    }

    // not there
    if (node == mp->end())
        return JNI_FALSE;

    // now assign values from can to can
    return (mp_set(env, jobj, jcan, node->second, field_number) ? JNI_TRUE : JNI_FALSE);
}
