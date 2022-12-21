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
#include <mdr_pf.h>
#include <jni.h>

extern bool mdr_constructor(JNIEnv *, jobject, mdr::mdr_t *);

static void fail(JNIEnv *env, jobject jobj, jclass cls, const char *msg)
{
    jfieldID reason;
    jstring jstr;

    reason = env->GetFieldID(cls, "reason", "Ljava/lang/String;");
    if (reason == 0)
    {
        std::cerr << "mdr_pf_t::fail: cannot fail" << std::endl;
        return;
    }
    jstr = env->NewStringUTF(msg);
    env->SetObjectField(jobj, reason, jstr);
}

extern "C" JNIEXPORT jboolean JNICALL
Java_com_capmet_metrics_jmdr_mdr_1pf_1t_pf_1constructor
    (JNIEnv *env, jobject jobj, jstring jhost)
{
    jclass cls = env->GetObjectClass(jobj);
    jfieldID fid;
    mdr::mdr_pf_t *pf;

    fid = env->GetFieldID(cls, "handle", "J");
    if (fid == 0)
    {
        fail(env, jobj, cls, "mdr_pf_t::mdr_pf_t: cannot find handle");
        return JNI_FALSE;
    }
    try
    {
        const char *remote_host = 0;

        if (jhost != 0)
            remote_host = env->GetStringUTFChars(jhost, 0);

        pf = new mdr::mdr_pf_t(remote_host);

        if (remote_host)
            env->ReleaseStringUTFChars(jhost, remote_host);

        if (!mdr_constructor(env, jobj, pf))
            return JNI_FALSE;
    }
    catch(std::exception &e)
    {
        fail(env, jobj, cls, e.what());
        return JNI_FALSE;
    }
    env->SetLongField(jobj, fid, (jlong) _POINTER_CAST_ pf);

    return JNI_TRUE;
}

extern "C" JNIEXPORT void JNICALL
Java_com_capmet_metrics_jmdr_mdr_1pf_1t_finalize
	(JNIEnv *env, jobject jobj)
{
	jclass cls = env->GetObjectClass(jobj);
	jfieldID fid;
    mdr::mdr_pf_t *pf;

	fid = env->GetFieldID(cls, "handle", "J");
	if (fid == 0)
		return;
    pf = (mdr::mdr_pf_t *) _POINTER_CAST_ env->GetLongField(jobj, fid);
	if (pf == 0)
		return;

	delete pf;

	env->SetIntField(jobj, fid, 0);
}

extern "C" JNIEXPORT jboolean JNICALL
Java_com_capmet_metrics_jmdr_mdr_1pf_1t_pf_1refresh
	(JNIEnv *env, jobject jobj)
{
    jclass cls = env->GetObjectClass(jobj);
    jfieldID fid;
    mdr::mdr_pf_t *pf;

    fid = env->GetFieldID(cls, "handle", "J");
    if (fid == 0)
    {
        fail(env, jobj, cls, "mdr_pf_t::pf_refresh: cannot find handle");
        return JNI_FALSE;
    }
    pf = (mdr::mdr_pf_t *) _POINTER_CAST_ env->GetLongField(jobj, fid);
    if (pf == 0)
    {
        fail(env, jobj, cls, "mdr_pf_t::pf_refresh: null mdr_pf_t handle");
        return JNI_FALSE;
    }
    try
    {
        pf->mdr_refresh();
    }
    catch(std::exception &e)
    {
        fail(env, jobj, cls, e.what());
        return JNI_FALSE;
    }

    return JNI_TRUE;
}

static bool pf_set
    (JNIEnv *env, jobject jobj, jobject jcan,
        mdr::mpf_can_t &can, jint field_number)
{
    jclass cls = env->GetObjectClass(jobj);
    jclass ccls = env->GetObjectClass(jcan);
    jfieldID fid;

    switch(field_number)
    {
    case mdr::MPF_SIZE_K:
        //
        // mpf_size_K
        //
        fid = env->GetFieldID(ccls, "mpf_size_K", "I");
        if (fid == 0)
        {
            fail(env, jobj, cls, "mdr_pf_t::pf_set: cannot find mpf_size_K");
            return false;
        }
        env->SetIntField(jcan, fid, (int) can.mpf_size_K);
        break;
    case mdr::MPF_FREE_K:
        //
        // mpf_free_K
        //
        fid = env->GetFieldID(ccls, "mpf_free_K", "I");
        if (fid == 0)
        {
            fail(env, jobj, cls, "mdr_pf_t::pf_set: cannot find mpf_free_K");
            return false;
        }
        env->SetIntField(jcan, fid, (int) can.mpf_free_K);
        break;
    case mdr::MPF_USED_K:
        //
        // mpf_used_K
        //
        fid = env->GetFieldID(ccls, "mpf_used_K", "I");
        if (fid == 0)
        {
            fail(env, jobj, cls, "mdr_pf_t::pf_set: cannot find mpf_used_K");
            return false;
        }
        env->SetIntField(jcan, fid, (int) can.mpf_used_K);
        break;
    }

    return true;
}

static bool pf_set
    (JNIEnv *env, jobject jobj, jobject jcan, mdr::mpf_can_t &can)
{
    return(
        pf_set(env, jobj, jcan, can, mdr::MPF_SIZE_K) &&
        pf_set(env, jobj, jcan, can, mdr::MPF_FREE_K) &&
        pf_set(env, jobj, jcan, can, mdr::MPF_USED_K));
}

extern "C" JNIEXPORT jint JNICALL
Java_com_capmet_metrics_jmdr_mdr_1pf_1t_mdr_1size
	(JNIEnv *env, jobject jobj)
{
    jclass cls = env->GetObjectClass(jobj);
    jfieldID fid;
    mdr::mdr_pf_t *pf;

    // find handle
    fid = env->GetFieldID(cls, "handle", "J");
    if (fid == 0)
    {
        fail(env, jobj, cls, "mdr_pf_t::mdr_size: cannot find handle");
        return JNI_FALSE;
    }

    // get the pf pointer
    pf = (mdr::mdr_pf_t *) _POINTER_CAST_ env->GetLongField(jobj, fid);
    if (pf == 0)
    {
        fail(env, jobj, cls, "mdr_pf_t::mdr_size: null mdr_pf_t handle");
        return JNI_FALSE;
    }

    return pf->size();
}

extern "C" JNIEXPORT jobjectArray JNICALL
Java_com_capmet_metrics_jmdr_mdr_1pf_1t_mdr_1keyset
	(JNIEnv *env, jobject jobj)
{
    jclass cls = env->GetObjectClass(jobj);
    jfieldID fid;
    mdr::mdr_pf_t *pf;
    jobjectArray jarray;

    // find handle
    fid = env->GetFieldID(cls, "handle", "J");
    if (fid == 0)
    {
        fail(env, jobj, cls, "mdr_pf_t::mdr_keyset: cannot find handle");
        return JNI_FALSE;
    }

    // get the pf pointer
    pf = (mdr::mdr_pf_t *) _POINTER_CAST_ env->GetLongField(jobj, fid);
    if (pf == 0)
    {
        fail(env, jobj, cls, "mdr_pf_t::mdr_keyset: null mdr_pf_t handle");
        return JNI_FALSE;
    }

    // retrieve the keys
    mdr::mdr_keyset_t keyset;
    mdr::mdr_key_t kn;
    int i;

    try
    {
        pf->mdr_keyset(keyset);
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
Java_com_capmet_metrics_jmdr_mdr_1pf_1t_mdr_1get
    (JNIEnv *env, jobject jobj, jstring jkey, jobject jcan)
{
    jclass cls = env->GetObjectClass(jobj);
    jfieldID fid;
    mdr::mdr_pf_t *pf;

    // find handle
    fid = env->GetFieldID(cls, "handle", "J");
    if (fid == 0)
    {
        fail(env, jobj, cls, "mdr_pf_t::mdr_get: cannot find handle");
        return JNI_FALSE;
    }

    // get the pf pointer
    pf = (mdr::mdr_pf_t *) _POINTER_CAST_ env->GetLongField(jobj, fid);
    if (pf == 0)
    {
        fail(env, jobj, cls, "mdr_pf_t::mdr_get: null mdr_pf_t handle");
        return JNI_FALSE;
    }

    // retrive the key and perform lookup
    const char *key;
    mdr::mpf_node_t node;

    try
    {
        key = env->GetStringUTFChars(jkey, 0);
        node = pf->find(key);
        env->ReleaseStringUTFChars(jkey, key);
    }
    catch(std::exception &e)
    {
        fail(env, jobj, cls, e.what());
        return JNI_FALSE;
    }

    // not there
    if (node == pf->end())
        return JNI_FALSE;

    // now assign values from can to can
    return (pf_set(env, jobj, jcan, node->second) ? JNI_TRUE : JNI_FALSE);
}

extern "C" JNIEXPORT jboolean JNICALL
Java_com_capmet_metrics_jmdr_mdr_1pf_1t_mdr_1get_1field
    (JNIEnv *env, jobject jobj, jstring jkey, jobject jcan, jint field_number)
{
    jclass cls = env->GetObjectClass(jobj);
    jfieldID fid;
    mdr::mdr_pf_t *pf;

    // find handle
    fid = env->GetFieldID(cls, "handle", "J");
    if (fid == 0)
    {
        fail(env, jobj, cls, "mdr_pf_t::mdr_get_field: cannot find handle");
        return JNI_FALSE;
    }

    // get the pf pointer
    pf = (mdr::mdr_pf_t *) _POINTER_CAST_ env->GetLongField(jobj, fid);
    if (pf == 0)
    {
        fail(env, jobj, cls, "mdr_pf_t::mdr_get_field: null mdr_pf_t handle");
        return JNI_FALSE;
    }

    // retrive the key and perform lookup
    const char *key;
    mdr::mpf_node_t node;

    try
    {
        key = env->GetStringUTFChars(jkey, 0);
        node = pf->find(key);
        env->ReleaseStringUTFChars(jkey, key);
    }
    catch(std::exception &e)
    {
        fail(env, jobj, cls, e.what());
        return JNI_FALSE;
    }

    // not there
    if (node == pf->end())
        return JNI_FALSE;

    // now assign values from can to can
    return (pf_set(env, jobj, jcan, node->second, field_number) ? JNI_TRUE : JNI_FALSE);
}
