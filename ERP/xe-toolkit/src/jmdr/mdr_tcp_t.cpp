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
#include <mdr_tcp.h>
#include <jni.h>

extern bool mdr_constructor(JNIEnv *, jobject, mdr::mdr_t *);

static void fail(JNIEnv *env, jobject jobj, jclass cls, const char *msg)
{
    jfieldID reason;
    jstring jstr;

    reason = env->GetFieldID(cls, "reason", "Ljava/lang/String;");
    if (reason == 0)
    {
        std::cerr << "mdr_tcp_t::fail: cannot fail" << std::endl;
        return;
    }
    jstr = env->NewStringUTF(msg);
    env->SetObjectField(jobj, reason, jstr);
}

static bool
tcp_set(JNIEnv *env, jobject jobj, jclass cls, mdr::mdr_tcp_t *tcp)
{
    jfieldID fid;

	//
	// mtcp_tcpRtoAlgorithm
	//
	fid = env->GetFieldID(cls, "mtcp_tcpRtoAlgorithm", "I");
	if (fid == 0)
	{
		fail(env, jobj, cls, "mdr_tcp_t::tcp_set: cannot find mtcp_tcpRtoAlgorithm");
		return false;
	}
	env->SetIntField(jobj, fid, (int) tcp->mtcp_tcpRtoAlgorithm);

	//
	// mtcp_tcpRtoMin
	//
	fid = env->GetFieldID(cls, "mtcp_tcpRtoMin", "I");
	if (fid == 0)
	{
		fail(env, jobj, cls, "mdr_tcp_t::tcp_set: cannot find mtcp_tcpRtoMin");
		return false;
	}
	env->SetIntField(jobj, fid, (int) tcp->mtcp_tcpRtoMin);

	//
	// mtcp_tcpRtoMax
	//
	fid = env->GetFieldID(cls, "mtcp_tcpRtoMax", "I");
	if (fid == 0)
	{
		fail(env, jobj, cls, "mdr_tcp_t::tcp_set: cannot find mtcp_tcpRtoMax");
		return false;
	}
	env->SetIntField(jobj, fid, (int) tcp->mtcp_tcpRtoMax);

	//
	// mtcp_tcpMaxConn
	//
	fid = env->GetFieldID(cls, "mtcp_tcpMaxConn", "I");
	if (fid == 0)
	{
		fail(env, jobj, cls, "mdr_tcp_t::tcp_set: cannot find mtcp_tcpMaxConn");
		return false;
	}
	env->SetIntField(jobj, fid, (int) tcp->mtcp_tcpMaxConn);

	//
	// mtcp_tcpActiveOpens
	//
	fid = env->GetFieldID(cls, "mtcp_tcpActiveOpens", "I");
	if (fid == 0)
	{
		fail(env, jobj, cls, "mdr_tcp_t::tcp_set: cannot find mtcp_tcpActiveOpens");
		return false;
	}
	env->SetIntField(jobj, fid, (int) tcp->mtcp_tcpActiveOpens);

	//
	// mtcp_tcpPassiveOpens
	//
	fid = env->GetFieldID(cls, "mtcp_tcpPassiveOpens", "I");
	if (fid == 0)
	{
		fail(env, jobj, cls, "mdr_tcp_t::tcp_set: cannot find mtcp_tcpPassiveOpens");
		return false;
	}
	env->SetIntField(jobj, fid, (int) tcp->mtcp_tcpPassiveOpens);

	//
	// mtcp_tcpAttemptFails
	//
	fid = env->GetFieldID(cls, "mtcp_tcpAttemptFails", "I");
	if (fid == 0)
	{
		fail(env, jobj, cls, "mdr_tcp_t::tcp_set: cannot find mtcp_tcpAttemptFails");
		return false;
	}
	env->SetIntField(jobj, fid, (int) tcp->mtcp_tcpAttemptFails);

	//
	// mtcp_tcpEstabResets
	//
	fid = env->GetFieldID(cls, "mtcp_tcpEstabResets", "I");
	if (fid == 0)
	{
		fail(env, jobj, cls, "mdr_tcp_t::tcp_set: cannot find mtcp_tcpEstabResets");
		return false;
	}
	env->SetIntField(jobj, fid, (int) tcp->mtcp_tcpEstabResets);

	//
	// mtcp_tcpCurrEstab
	//
	fid = env->GetFieldID(cls, "mtcp_tcpCurrEstab", "I");
	if (fid == 0)
	{
		fail(env, jobj, cls, "mdr_tcp_t::tcp_set: cannot find mtcp_tcpCurrEstab");
		return false;
	}
	env->SetIntField(jobj, fid, (int) tcp->mtcp_tcpCurrEstab);

	//
	// mtcp_tcpInSegs
	//
	fid = env->GetFieldID(cls, "mtcp_tcpInSegs", "I");
	if (fid == 0)
	{
		fail(env, jobj, cls, "mdr_tcp_t::tcp_set: cannot find mtcp_tcpInSegs");
		return false;
	}
	env->SetIntField(jobj, fid, (int) tcp->mtcp_tcpInSegs);

	//
	// mtcp_tcpOutSegs
	//
	fid = env->GetFieldID(cls, "mtcp_tcpOutSegs", "I");
	if (fid == 0)
	{
		fail(env, jobj, cls, "mdr_tcp_t::tcp_set: cannot find mtcp_tcpOutSegs");
		return false;
	}
	env->SetIntField(jobj, fid, (int) tcp->mtcp_tcpOutSegs);

	//
	// mtcp_tcpRetransSegs
	//
	fid = env->GetFieldID(cls, "mtcp_tcpRetransSegs", "I");
	if (fid == 0)
	{
		fail(env, jobj, cls, "mdr_tcp_t::tcp_set: cannot find mtcp_tcpRetransSegs");
		return false;
	}
	env->SetIntField(jobj, fid, (int) tcp->mtcp_tcpRetransSegs);

	//
	// mtcp_tcpInErrs
	//
	fid = env->GetFieldID(cls, "mtcp_tcpInErrs", "I");
	if (fid == 0)
	{
		fail(env, jobj, cls, "mdr_tcp_t::tcp_set: cannot find mtcp_tcpInErrs");
		return false;
	}
	env->SetIntField(jobj, fid, (int) tcp->mtcp_tcpInErrs);

	//
	// mtcp_tcpOutRsts
	//
	fid = env->GetFieldID(cls, "mtcp_tcpOutRsts", "I");
	if (fid == 0)
	{
		fail(env, jobj, cls, "mdr_tcp_t::tcp_set: cannot find mtcp_tcpOutRsts");
		return false;
	}
	env->SetIntField(jobj, fid, (int) tcp->mtcp_tcpOutRsts);

    return true;
}

extern "C" JNIEXPORT jboolean JNICALL
Java_com_capmet_metrics_jmdr_mdr_1tcp_1t_tcp_1constructor
    (JNIEnv *env, jobject jobj, jstring jhost)
{
    jclass cls = env->GetObjectClass(jobj);
    jfieldID fid;
    mdr::mdr_tcp_t *tcp;

    fid = env->GetFieldID(cls, "handle", "J");
    if (fid == 0)
    {
        fail(env, jobj, cls, "mdr_tcp_t::mdr_tcp_t: cannot find handle");
        return JNI_FALSE;
    }
    try
    {
        const char *remote_host = 0;

        if (jhost != 0)
            remote_host = env->GetStringUTFChars(jhost, 0);

        tcp = new mdr::mdr_tcp_t(remote_host);

        if (remote_host)
            env->ReleaseStringUTFChars(jhost, remote_host);

        if (!mdr_constructor(env, jobj, tcp))
            return JNI_FALSE;
    }
    catch(std::exception & e)
    {
        fail(env, jobj, cls, e.what());
        return JNI_FALSE;
    }
    env->SetLongField(jobj, fid, (jlong) _POINTER_CAST_ tcp);

    return (tcp_set(env, jobj, cls, tcp) ? JNI_TRUE : JNI_FALSE);
}

extern "C" JNIEXPORT void JNICALL
Java_com_capmet_metrics_jmdr_mdr_1tcp_1t_finalize
	(JNIEnv *env, jobject jobj)
{
	jclass cls = env->GetObjectClass(jobj);
	jfieldID fid;
    mdr::mdr_tcp_t *tcp;

	fid = env->GetFieldID(cls, "handle", "J");
	if (fid == 0)
		return;
    tcp = (mdr::mdr_tcp_t *) _POINTER_CAST_ env->GetLongField(jobj, fid);
	if (tcp == 0)
		return;

	delete tcp;

	env->SetIntField(jobj, fid, 0);
}

extern "C" JNIEXPORT jboolean JNICALL
Java_com_capmet_metrics_jmdr_mdr_1tcp_1t_tcp_1refresh
	(JNIEnv *env, jobject jobj)
{
    jclass cls = env->GetObjectClass(jobj);
    jfieldID fid;
    mdr::mdr_tcp_t *tcp;

    fid = env->GetFieldID(cls, "handle", "J");
    if (fid == 0)
    {
        fail(env, jobj, cls, "mdr_tcp_t::tcp_refresh: cannot find handle");
        return JNI_FALSE;
    }
    tcp = (mdr::mdr_tcp_t *) _POINTER_CAST_ env->GetLongField(jobj, fid);
    if (tcp == 0)
    {
        fail(env, jobj, cls, "mdr_tcp_t::tcp_refresh: null mdr_tcp_t handle");
        return JNI_FALSE;
    }
    try
    {
        tcp->mdr_refresh();
    }
    catch(std::exception & e)
    {
        fail(env, jobj, cls, e.what());
        return JNI_FALSE;
    }

    return (tcp_set(env, jobj, cls, tcp) ? JNI_TRUE : JNI_FALSE);
}

extern "C" JNIEXPORT jboolean JNICALL
Java_com_capmet_metrics_jmdr_mdr_1tcp_1t_tcp_1rates
	(JNIEnv *env, jobject jobj)
{
    jclass cls = env->GetObjectClass(jobj);
    jfieldID fid;
    mdr::mdr_tcp_t *tcp;

    fid = env->GetFieldID(cls, "handle", "J");
    if (fid == 0)
    {
        fail(env, jobj, cls, "mdr_tcp_t::tcp_rates: cannot find handle");
        return JNI_FALSE;
    }
    tcp = (mdr::mdr_tcp_t *) _POINTER_CAST_ env->GetLongField(jobj, fid);
    if (tcp == 0)
    {
        fail(env, jobj, cls, "mdr_tcp_t::tcp_rates: null mdr_tcp_t handle");
        return JNI_FALSE;
    }
    try
    {
        tcp->mdr_rates();
    }
    catch(std::exception & e)
    {
        fail(env, jobj, cls, e.what());
        return JNI_FALSE;
    }

    return (tcp_set(env, jobj, cls, tcp) ? JNI_TRUE : JNI_FALSE);
}
