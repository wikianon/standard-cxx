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
#include <mdr_ip.h>
#include <jni.h>

extern bool mdr_constructor(JNIEnv *, jobject, mdr::mdr_t *);

static void fail(JNIEnv *env, jobject jobj, jclass cls, const char *msg)
{
    jfieldID reason;
    jstring jstr;

    reason = env->GetFieldID(cls, "reason", "Ljava/lang/String;");
    if (reason == 0)
    {
        std::cerr << "mdr_ip_t::fail: cannot fail" << std::endl;
        return;
    }
    jstr = env->NewStringUTF(msg);
    env->SetObjectField(jobj, reason, jstr);
}

static bool
ip_set(JNIEnv *env, jobject jobj, jclass cls, mdr::mdr_ip_t *ip)
{
    jfieldID fid;

    //
    // mip_ipForwarding
    //
    fid = env->GetFieldID(cls, "mip_ipForwarding", "I");
    if (fid == 0)
    {
        fail(env, jobj, cls, "mdr_ip_t::ip_set: cannot find mip_ipForwarding");
        return false;
    }
    env->SetIntField(jobj, fid, (int) ip->mip_ipForwarding);

    //
    // mip_ipDefaultTTL
    //
    fid = env->GetFieldID(cls, "mip_ipDefaultTTL", "I");
    if (fid == 0)
    {
        fail(env, jobj, cls, "mdr_ip_t::ip_set: cannot find mip_ipDefaultTTL");
        return false;
    }
    env->SetIntField(jobj, fid, (int) ip->mip_ipDefaultTTL);

    //
    // mip_ipInReceives
    //
    fid = env->GetFieldID(cls, "mip_ipInReceives", "I");
    if (fid == 0)
    {
        fail(env, jobj, cls, "mdr_ip_t::ip_set: cannot find mip_ipInReceives");
        return false;
    }
    env->SetIntField(jobj, fid, (int) ip->mip_ipInReceives);

    //
    // mip_ipInHdrErrors
    //
    fid = env->GetFieldID(cls, "mip_ipInHdrErrors", "I");
    if (fid == 0)
    {
        fail(env, jobj, cls, "mdr_ip_t::ip_set: cannot find mip_ipInHdrErrors");
        return false;
    }
    env->SetIntField(jobj, fid, (int) ip->mip_ipInHdrErrors);

    //
    // mip_ipInAddrErrors
    //
    fid = env->GetFieldID(cls, "mip_ipInAddrErrors", "I");
    if (fid == 0)
    {
        fail(env, jobj, cls, "mdr_ip_t::ip_set: cannot find mip_ipInAddrErrors");
        return false;
    }
    env->SetIntField(jobj, fid, (int) ip->mip_ipInAddrErrors);

    //
    // mip_ipForwDatagrams
    //
    fid = env->GetFieldID(cls, "mip_ipForwDatagrams", "I");
    if (fid == 0)
    {
        fail(env, jobj, cls, "mdr_ip_t::ip_set: cannot find mip_ipForwDatagrams");
        return false;
    }
    env->SetIntField(jobj, fid, (int) ip->mip_ipForwDatagrams);

    //
    // mip_ipInUnknownProtos
    //
    fid = env->GetFieldID(cls, "mip_ipInUnknownProtos", "I");
    if (fid == 0)
    {
        fail(env, jobj, cls, "mdr_ip_t::ip_set: cannot find mip_ipInUnknownProtos");
        return false;
    }
    env->SetIntField(jobj, fid, (int) ip->mip_ipInUnknownProtos);

    //
    // mip_ipInDiscards
    //
    fid = env->GetFieldID(cls, "mip_ipInDiscards", "I");
    if (fid == 0)
    {
        fail(env, jobj, cls, "mdr_ip_t::ip_set: cannot find mip_ipInDiscards");
        return false;
    }
    env->SetIntField(jobj, fid, (int) ip->mip_ipInDiscards);

    //
    // mip_ipInDelivers
    //
    fid = env->GetFieldID(cls, "mip_ipInDelivers", "I");
    if (fid == 0)
    {
        fail(env, jobj, cls, "mdr_ip_t::ip_set: cannot find mip_ipInDelivers");
        return false;
    }
    env->SetIntField(jobj, fid, (int) ip->mip_ipInDelivers);

    //
    // mip_ipOutRequests
    //
    fid = env->GetFieldID(cls, "mip_ipOutRequests", "I");
    if (fid == 0)
    {
        fail(env, jobj, cls, "mdr_ip_t::ip_set: cannot find mip_ipOutRequests");
        return false;
    }
    env->SetIntField(jobj, fid, (int) ip->mip_ipOutRequests);

    //
    // mip_ipOutDiscards
    //
    fid = env->GetFieldID(cls, "mip_ipOutDiscards", "I");
    if (fid == 0)
    {
        fail(env, jobj, cls, "mdr_ip_t::ip_set: cannot find mip_ipOutDiscards");
        return false;
    }
    env->SetIntField(jobj, fid, (int) ip->mip_ipOutDiscards);

    //
    // mip_ipOutNoRoutes
    //
    fid = env->GetFieldID(cls, "mip_ipOutNoRoutes", "I");
    if (fid == 0)
    {
        fail(env, jobj, cls, "mdr_ip_t::ip_set: cannot find mip_ipOutNoRoutes");
        return false;
    }
    env->SetIntField(jobj, fid, (int) ip->mip_ipOutNoRoutes);

    //
    // mip_ipReasmTimeout
    //
    fid = env->GetFieldID(cls, "mip_ipReasmTimeout", "I");
    if (fid == 0)
    {
        fail(env, jobj, cls, "mdr_ip_t::ip_set: cannot find mip_ipReasmTimeout");
        return false;
    }
    env->SetIntField(jobj, fid, (int) ip->mip_ipReasmTimeout);

    //
    // mip_ipReasmReqds
    //
    fid = env->GetFieldID(cls, "mip_ipReasmReqds", "I");
    if (fid == 0)
    {
        fail(env, jobj, cls, "mdr_ip_t::ip_set: cannot find mip_ipReasmReqds");
        return false;
    }
    env->SetIntField(jobj, fid, (int) ip->mip_ipReasmReqds);

    //
    // mip_ipReasmOKs
    //
    fid = env->GetFieldID(cls, "mip_ipReasmOKs", "I");
    if (fid == 0)
    {
        fail(env, jobj, cls, "mdr_ip_t::ip_set: cannot find mip_ipReasmOKs");
        return false;
    }
    env->SetIntField(jobj, fid, (int) ip->mip_ipReasmOKs);

    //
    // mip_ipReasmFails
    //
    fid = env->GetFieldID(cls, "mip_ipReasmFails", "I");
    if (fid == 0)
    {
        fail(env, jobj, cls, "mdr_ip_t::ip_set: cannot find mip_ipReasmFails");
        return false;
    }
    env->SetIntField(jobj, fid, (int) ip->mip_ipReasmFails);

    //
    // mip_ipFragOKs
    //
    fid = env->GetFieldID(cls, "mip_ipFragOKs", "I");
    if (fid == 0)
    {
        fail(env, jobj, cls, "mdr_ip_t::ip_set: cannot find mip_ipFragOKs");
        return false;
    }
    env->SetIntField(jobj, fid, (int) ip->mip_ipFragOKs);

    //
    // mip_ipFragFails
    //
    fid = env->GetFieldID(cls, "mip_ipFragFails", "I");
    if (fid == 0)
    {
        fail(env, jobj, cls, "mdr_ip_t::ip_set: cannot find mip_ipFragFails");
        return false;
    }
    env->SetIntField(jobj, fid, (int) ip->mip_ipFragFails);

    //
    // mip_ipFragCreates
    //
    fid = env->GetFieldID(cls, "mip_ipFragCreates", "I");
    if (fid == 0)
    {
        fail(env, jobj, cls, "mdr_ip_t::ip_set: cannot find mip_ipFragCreates");
        return false;
    }
    env->SetIntField(jobj, fid, (int) ip->mip_ipFragCreates);

    //
    // mip_ipRoutingDiscards
    //
    fid = env->GetFieldID(cls, "mip_ipRoutingDiscards", "I");
    if (fid == 0)
    {
        fail(env, jobj, cls, "mdr_ip_t::ip_set: cannot find mip_ipRoutingDiscards");
        return false;
    }
    env->SetIntField(jobj, fid, (int) ip->mip_ipRoutingDiscards);


    return true;
}

extern "C" JNIEXPORT jboolean JNICALL
Java_com_capmet_metrics_jmdr_mdr_1ip_1t_ip_1constructor
    (JNIEnv *env, jobject jobj, jstring jhost)
{
    jclass cls = env->GetObjectClass(jobj);
    jfieldID fid;
    mdr::mdr_ip_t *ip;

    fid = env->GetFieldID(cls, "handle", "J");
    if (fid == 0)
    {
        fail(env, jobj, cls, "mdr_ip_t::mdr_ip_t: cannot find handle");
        return JNI_FALSE;
    }
    try
    {
        const char *remote_host = 0;

        if (jhost != 0)
            remote_host = env->GetStringUTFChars(jhost, 0);

        ip = new mdr::mdr_ip_t(remote_host);

        if (remote_host)
            env->ReleaseStringUTFChars(jhost, remote_host);

        if (!mdr_constructor(env, jobj, ip))
            return JNI_FALSE;
    }
    catch(std::exception & e)
    {
        fail(env, jobj, cls, e.what());
        return JNI_FALSE;
    }
    env->SetLongField(jobj, fid, (jlong) _POINTER_CAST_ ip);

    return (ip_set(env, jobj, cls, ip) ? JNI_TRUE : JNI_FALSE);
}

extern "C" JNIEXPORT void JNICALL
Java_com_capmet_metrics_jmdr_mdr_1ip_1t_finalize
	(JNIEnv *env, jobject jobj)
{
	jclass cls = env->GetObjectClass(jobj);
	jfieldID fid;
    mdr::mdr_ip_t *ip;

	fid = env->GetFieldID(cls, "handle", "J");
	if (fid == 0)
		return;
    ip = (mdr::mdr_ip_t *) _POINTER_CAST_ env->GetLongField(jobj, fid);
	if (ip == 0)
		return;

	delete ip;

	env->SetIntField(jobj, fid, 0);
}

extern "C" JNIEXPORT jboolean JNICALL
Java_com_capmet_metrics_jmdr_mdr_1ip_1t_ip_1refresh
	(JNIEnv *env, jobject jobj)
{
    jclass cls = env->GetObjectClass(jobj);
    jfieldID fid;
    mdr::mdr_ip_t *ip;

    fid = env->GetFieldID(cls, "handle", "J");
    if (fid == 0)
    {
        fail(env, jobj, cls, "mdr_ip_t::ip_refresh: cannot find handle");
        return JNI_FALSE;
    }
    ip = (mdr::mdr_ip_t *) _POINTER_CAST_ env->GetLongField(jobj, fid);
    if (ip == 0)
    {
        fail(env, jobj, cls, "mdr_ip_t::ip_refresh: null mdr_ip_t handle");
        return JNI_FALSE;
    }
    try
    {
        ip->mdr_refresh();
    }
    catch(std::exception & e)
    {
        fail(env, jobj, cls, e.what());
        return JNI_FALSE;
    }

    return (ip_set(env, jobj, cls, ip) ? JNI_TRUE : JNI_FALSE);
}

extern "C" JNIEXPORT jboolean JNICALL
Java_com_capmet_metrics_jmdr_mdr_1ip_1t_ip_1rates
	(JNIEnv *env, jobject jobj)
{
    jclass cls = env->GetObjectClass(jobj);
    jfieldID fid;
    mdr::mdr_ip_t *ip;

    fid = env->GetFieldID(cls, "handle", "J");
    if (fid == 0)
    {
        fail(env, jobj, cls, "mdr_ip_t::ip_rates: cannot find handle");
        return JNI_FALSE;
    }
    ip = (mdr::mdr_ip_t *) _POINTER_CAST_ env->GetLongField(jobj, fid);
    if (ip == 0)
    {
        fail(env, jobj, cls, "mdr_ip_t::ip_rates: null mdr_ip_t handle");
        return JNI_FALSE;
    }
    try
    {
        ip->mdr_rates();
    }
    catch(std::exception & e)
    {
        fail(env, jobj, cls, e.what());
        return JNI_FALSE;
    }

    return (ip_set(env, jobj, cls, ip) ? JNI_TRUE : JNI_FALSE);
}
