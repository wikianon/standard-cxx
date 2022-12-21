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
#include <mdr_udp.h>
#include <jni.h>

extern bool mdr_constructor(JNIEnv *, jobject, mdr::mdr_t *);

static void fail(JNIEnv *env, jobject jobj, jclass cls, const char *msg)
{
    jfieldID reason;
    jstring jstr;

    reason = env->GetFieldID(cls, "reason", "Ljava/lang/String;");
    if (reason == 0)
    {
        std::cerr << "mdr_udp_t::fail: cannot fail" << std::endl;
        return;
    }
    jstr = env->NewStringUTF(msg);
    env->SetObjectField(jobj, reason, jstr);
}

static bool
udp_set(JNIEnv *env, jobject jobj, jclass cls, mdr::mdr_udp_t *udp)
{
    jfieldID fid;

	//
	// mudp_udpInDatagrams
	//
	fid = env->GetFieldID(cls, "mudp_udpInDatagrams", "I");
	if (fid == 0)
	{
		fail(env, jobj, cls, "mdr_udp_t::udp_set: cannot find mudp_udpInDatagrams");
		return false;
	}
	env->SetIntField(jobj, fid, (int) udp->mudp_udpInDatagrams);

	//
	// mudp_udpNoPorts
	//
	fid = env->GetFieldID(cls, "mudp_udpNoPorts", "I");
	if (fid == 0)
	{
		fail(env, jobj, cls, "mdr_udp_t::udp_set: cannot find mudp_udpNoPorts");
		return false;
	}
	env->SetIntField(jobj, fid, (int) udp->mudp_udpNoPorts);

	//
	// mudp_udpInErrors
	//
	fid = env->GetFieldID(cls, "mudp_udpInErrors", "I");
	if (fid == 0)
	{
		fail(env, jobj, cls, "mdr_udp_t::udp_set: cannot find mudp_udpInErrors");
		return false;
	}
	env->SetIntField(jobj, fid, (int) udp->mudp_udpInErrors);

	//
	// mudp_udpOutDatagrams
	//
	fid = env->GetFieldID(cls, "mudp_udpOutDatagrams", "I");
	if (fid == 0)
	{
		fail(env, jobj, cls, "mdr_udp_t::udp_set: cannot find mudp_udpOutDatagrams");
		return false;
	}
	env->SetIntField(jobj, fid, (int) udp->mudp_udpOutDatagrams);


    return true;
}

extern "C" JNIEXPORT jboolean JNICALL
Java_com_capmet_metrics_jmdr_mdr_1udp_1t_udp_1constructor
    (JNIEnv *env, jobject jobj, jstring jhost)
{
    jclass cls = env->GetObjectClass(jobj);
    jfieldID fid;
    mdr::mdr_udp_t *udp;

    fid = env->GetFieldID(cls, "handle", "J");
    if (fid == 0)
    {
        fail(env, jobj, cls, "mdr_udp_t::mdr_udp_t: cannot find handle");
        return JNI_FALSE;
    }
    try
    {
        const char *remote_host = 0;

        if (jhost != 0)
            remote_host = env->GetStringUTFChars(jhost, 0);

        udp = new mdr::mdr_udp_t(remote_host);

        if (remote_host)
            env->ReleaseStringUTFChars(jhost, remote_host);

        if (!mdr_constructor(env, jobj, udp))
            return JNI_FALSE;
    }
    catch(std::exception & e)
    {
        fail(env, jobj, cls, e.what());
        return JNI_FALSE;
    }
    env->SetLongField(jobj, fid, (jlong) _POINTER_CAST_ udp);

    return (udp_set(env, jobj, cls, udp) ? JNI_TRUE : JNI_FALSE);
}

extern "C" JNIEXPORT void JNICALL
Java_com_capmet_metrics_jmdr_mdr_1udp_1t_finalize
	(JNIEnv *env, jobject jobj)
{
	jclass cls = env->GetObjectClass(jobj);
	jfieldID fid;
    mdr::mdr_udp_t *udp;

	fid = env->GetFieldID(cls, "handle", "J");
	if (fid == 0)
		return;
    udp = (mdr::mdr_udp_t *) _POINTER_CAST_ env->GetLongField(jobj, fid);
	if (udp == 0)
		return;

	delete udp;

	env->SetIntField(jobj, fid, 0);
}

extern "C" JNIEXPORT jboolean JNICALL
Java_com_capmet_metrics_jmdr_mdr_1udp_1t_udp_1refresh
	(JNIEnv *env, jobject jobj)
{
    jclass cls = env->GetObjectClass(jobj);
    jfieldID fid;
    mdr::mdr_udp_t *udp;

    fid = env->GetFieldID(cls, "handle", "J");
    if (fid == 0)
    {
        fail(env, jobj, cls, "mdr_udp_t::udp_refresh: cannot find handle");
        return JNI_FALSE;
    }
    udp = (mdr::mdr_udp_t *) _POINTER_CAST_ env->GetLongField(jobj, fid);
    if (udp == 0)
    {
        fail(env, jobj, cls, "mdr_udp_t::udp_refresh: null mdr_udp_t handle");
        return JNI_FALSE;
    }
    try
    {
        udp->mdr_refresh();
    }
    catch(std::exception & e)
    {
        fail(env, jobj, cls, e.what());
        return JNI_FALSE;
    }

    return (udp_set(env, jobj, cls, udp) ? JNI_TRUE : JNI_FALSE);
}

extern "C" JNIEXPORT jboolean JNICALL
Java_com_capmet_metrics_jmdr_mdr_1udp_1t_udp_1rates
	(JNIEnv *env, jobject jobj)
{
    jclass cls = env->GetObjectClass(jobj);
    jfieldID fid;
    mdr::mdr_udp_t *udp;

    fid = env->GetFieldID(cls, "handle", "J");
    if (fid == 0)
    {
        fail(env, jobj, cls, "mdr_udp_t::udp_rates: cannot find handle");
        return JNI_FALSE;
    }
    udp = (mdr::mdr_udp_t *) _POINTER_CAST_ env->GetLongField(jobj, fid);
    if (udp == 0)
    {
        fail(env, jobj, cls, "mdr_udp_t::udp_rates: null mdr_udp_t handle");
        return JNI_FALSE;
    }
    try
    {
        udp->mdr_rates();
    }
    catch(std::exception & e)
    {
        fail(env, jobj, cls, e.what());
        return JNI_FALSE;
    }

    return (udp_set(env, jobj, cls, udp) ? JNI_TRUE : JNI_FALSE);
}
