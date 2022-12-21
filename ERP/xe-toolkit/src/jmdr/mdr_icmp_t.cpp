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
#include <mdr_icmp.h>
#include <jni.h>

extern bool mdr_constructor(JNIEnv *, jobject, mdr::mdr_t *);

static void fail(JNIEnv *env, jobject jobj, jclass cls, const char *msg)
{
    jfieldID reason;
    jstring jstr;

    reason = env->GetFieldID(cls, "reason", "Ljava/lang/String;");
    if (reason == 0)
    {
        std::cerr << "mdr_icmp_t::fail: cannot fail" << std::endl;
        return;
    }
    jstr = env->NewStringUTF(msg);
    env->SetObjectField(jobj, reason, jstr);
}

static bool
icmp_set(JNIEnv *env, jobject jobj, jclass cls, mdr::mdr_icmp_t *icmp)
{
    jfieldID fid;

    //
    // micmp_icmpInMsgs
    //
    fid = env->GetFieldID(cls, "micmp_icmpInMsgs", "I");
    if (fid == 0)
    {
        fail(env, jobj, cls, "mdr_icmp_t::icmp_set: cannot find micmp_icmpInMsgs");
        return false;
    }
    env->SetIntField(jobj, fid, (int) icmp->micmp_icmpInMsgs);

    //
    // micmp_icmpInErrors
    //
    fid = env->GetFieldID(cls, "micmp_icmpInErrors", "I");
    if (fid == 0)
    {
        fail(env, jobj, cls, "mdr_icmp_t::icmp_set: cannot find micmp_icmpInErrors");
        return false;
    }
    env->SetIntField(jobj, fid, (int) icmp->micmp_icmpInErrors);

    //
    // micmp_icmpInDestUnreachs
    //
    fid = env->GetFieldID(cls, "micmp_icmpInDestUnreachs", "I");
    if (fid == 0)
    {
        fail(env, jobj, cls, "mdr_icmp_t::icmp_set: cannot find micmp_icmpInDestUnreachs");
        return false;
    }
    env->SetIntField(jobj, fid, (int) icmp->micmp_icmpInDestUnreachs);

    //
    // micmp_icmpInTimeExcds
    //
    fid = env->GetFieldID(cls, "micmp_icmpInTimeExcds", "I");
    if (fid == 0)
    {
        fail(env, jobj, cls, "mdr_icmp_t::icmp_set: cannot find micmp_icmpInTimeExcds");
        return false;
    }
    env->SetIntField(jobj, fid, (int) icmp->micmp_icmpInTimeExcds);

    //
    // micmp_icmpInParmProbs
    //
    fid = env->GetFieldID(cls, "micmp_icmpInParmProbs", "I");
    if (fid == 0)
    {
        fail(env, jobj, cls, "mdr_icmp_t::icmp_set: cannot find micmp_icmpInParmProbs");
        return false;
    }
    env->SetIntField(jobj, fid, (int) icmp->micmp_icmpInParmProbs);

    //
    // micmp_icmpInSrcQuenchs
    //
    fid = env->GetFieldID(cls, "micmp_icmpInSrcQuenchs", "I");
    if (fid == 0)
    {
        fail(env, jobj, cls, "mdr_icmp_t::icmp_set: cannot find micmp_icmpInSrcQuenchs");
        return false;
    }
    env->SetIntField(jobj, fid, (int) icmp->micmp_icmpInSrcQuenchs);

    //
    // micmp_icmpInRedirects
    //
    fid = env->GetFieldID(cls, "micmp_icmpInRedirects", "I");
    if (fid == 0)
    {
        fail(env, jobj, cls, "mdr_icmp_t::icmp_set: cannot find micmp_icmpInRedirects");
        return false;
    }
    env->SetIntField(jobj, fid, (int) icmp->micmp_icmpInRedirects);

    //
    // micmp_icmpInEchos
    //
    fid = env->GetFieldID(cls, "micmp_icmpInEchos", "I");
    if (fid == 0)
    {
        fail(env, jobj, cls, "mdr_icmp_t::icmp_set: cannot find micmp_icmpInEchos");
        return false;
    }
    env->SetIntField(jobj, fid, (int) icmp->micmp_icmpInEchos);

    //
    // micmp_icmpInEchoReps
    //
    fid = env->GetFieldID(cls, "micmp_icmpInEchoReps", "I");
    if (fid == 0)
    {
        fail(env, jobj, cls, "mdr_icmp_t::icmp_set: cannot find micmp_icmpInEchoReps");
        return false;
    }
    env->SetIntField(jobj, fid, (int) icmp->micmp_icmpInEchoReps);

    //
    // micmp_icmpInTimestamps
    //
    fid = env->GetFieldID(cls, "micmp_icmpInTimestamps", "I");
    if (fid == 0)
    {
        fail(env, jobj, cls, "mdr_icmp_t::icmp_set: cannot find micmp_icmpInTimestamps");
        return false;
    }
    env->SetIntField(jobj, fid, (int) icmp->micmp_icmpInTimestamps);

    //
    // micmp_icmpInTimestampReps
    //
    fid = env->GetFieldID(cls, "micmp_icmpInTimestampReps", "I");
    if (fid == 0)
    {
        fail(env, jobj, cls, "mdr_icmp_t::icmp_set: cannot find micmp_icmpInTimestampReps");
        return false;
    }
    env->SetIntField(jobj, fid, (int) icmp->micmp_icmpInTimestampReps);

    //
    // micmp_icmpInAddrMasks
    //
    fid = env->GetFieldID(cls, "micmp_icmpInAddrMasks", "I");
    if (fid == 0)
    {
        fail(env, jobj, cls, "mdr_icmp_t::icmp_set: cannot find micmp_icmpInAddrMasks");
        return false;
    }
    env->SetIntField(jobj, fid, (int) icmp->micmp_icmpInAddrMasks);

    //
    // micmp_icmpInAddrMaskReps
    //
    fid = env->GetFieldID(cls, "micmp_icmpInAddrMaskReps", "I");
    if (fid == 0)
    {
        fail(env, jobj, cls, "mdr_icmp_t::icmp_set: cannot find micmp_icmpInAddrMaskReps");
        return false;
    }
    env->SetIntField(jobj, fid, (int) icmp->micmp_icmpInAddrMaskReps);

    //
    // micmp_icmpOutMsgs
    //
    fid = env->GetFieldID(cls, "micmp_icmpOutMsgs", "I");
    if (fid == 0)
    {
        fail(env, jobj, cls, "mdr_icmp_t::icmp_set: cannot find micmp_icmpOutMsgs");
        return false;
    }
    env->SetIntField(jobj, fid, (int) icmp->micmp_icmpOutMsgs);

    //
    // micmp_icmpOutErrors
    //
    fid = env->GetFieldID(cls, "micmp_icmpOutErrors", "I");
    if (fid == 0)
    {
        fail(env, jobj, cls, "mdr_icmp_t::icmp_set: cannot find micmp_icmpOutErrors");
        return false;
    }
    env->SetIntField(jobj, fid, (int) icmp->micmp_icmpOutErrors);

    //
    // micmp_icmpOutDestUnreachs
    //
    fid = env->GetFieldID(cls, "micmp_icmpOutDestUnreachs", "I");
    if (fid == 0)
    {
        fail(env, jobj, cls, "mdr_icmp_t::icmp_set: cannot find micmp_icmpOutDestUnreachs");
        return false;
    }
    env->SetIntField(jobj, fid, (int) icmp->micmp_icmpOutDestUnreachs);

    //
    // micmp_icmpOutTimeExcds
    //
    fid = env->GetFieldID(cls, "micmp_icmpOutTimeExcds", "I");
    if (fid == 0)
    {
        fail(env, jobj, cls, "mdr_icmp_t::icmp_set: cannot find micmp_icmpOutTimeExcds");
        return false;
    }
    env->SetIntField(jobj, fid, (int) icmp->micmp_icmpOutTimeExcds);

    //
    // micmp_icmpOutParmProbs
    //
    fid = env->GetFieldID(cls, "micmp_icmpOutParmProbs", "I");
    if (fid == 0)
    {
        fail(env, jobj, cls, "mdr_icmp_t::icmp_set: cannot find micmp_icmpOutParmProbs");
        return false;
    }
    env->SetIntField(jobj, fid, (int) icmp->micmp_icmpOutParmProbs);

    //
    // micmp_icmpOutSrcQuenchs
    //
    fid = env->GetFieldID(cls, "micmp_icmpOutSrcQuenchs", "I");
    if (fid == 0)
    {
        fail(env, jobj, cls, "mdr_icmp_t::icmp_set: cannot find micmp_icmpOutSrcQuenchs");
        return false;
    }
    env->SetIntField(jobj, fid, (int) icmp->micmp_icmpOutSrcQuenchs);

    //
    // micmp_icmpOutRedirects
    //
    fid = env->GetFieldID(cls, "micmp_icmpOutRedirects", "I");
    if (fid == 0)
    {
        fail(env, jobj, cls, "mdr_icmp_t::icmp_set: cannot find micmp_icmpOutRedirects");
        return false;
    }
    env->SetIntField(jobj, fid, (int) icmp->micmp_icmpOutRedirects);

    //
    // micmp_icmpOutEchos
    //
    fid = env->GetFieldID(cls, "micmp_icmpOutEchos", "I");
    if (fid == 0)
    {
        fail(env, jobj, cls, "mdr_icmp_t::icmp_set: cannot find micmp_icmpOutEchos");
        return false;
    }
    env->SetIntField(jobj, fid, (int) icmp->micmp_icmpOutEchos);

    //
    // micmp_icmpOutEchoReps
    //
    fid = env->GetFieldID(cls, "micmp_icmpOutEchoReps", "I");
    if (fid == 0)
    {
        fail(env, jobj, cls, "mdr_icmp_t::icmp_set: cannot find micmp_icmpOutEchoReps");
        return false;
    }
    env->SetIntField(jobj, fid, (int) icmp->micmp_icmpOutEchoReps);

    //
    // micmp_icmpOutTimestamps
    //
    fid = env->GetFieldID(cls, "micmp_icmpOutTimestamps", "I");
    if (fid == 0)
    {
        fail(env, jobj, cls, "mdr_icmp_t::icmp_set: cannot find micmp_icmpOutTimestamps");
        return false;
    }
    env->SetIntField(jobj, fid, (int) icmp->micmp_icmpOutTimestamps);

    //
    // micmp_icmpOutTimestampReps
    //
    fid = env->GetFieldID(cls, "micmp_icmpOutTimestampReps", "I");
    if (fid == 0)
    {
        fail(env, jobj, cls, "mdr_icmp_t::icmp_set: cannot find micmp_icmpOutTimestampReps");
        return false;
    }
    env->SetIntField(jobj, fid, (int) icmp->micmp_icmpOutTimestampReps);

    //
    // micmp_icmpOutAddrMasks
    //
    fid = env->GetFieldID(cls, "micmp_icmpOutAddrMasks", "I");
    if (fid == 0)
    {
        fail(env, jobj, cls, "mdr_icmp_t::icmp_set: cannot find micmp_icmpOutAddrMasks");
        return false;
    }
    env->SetIntField(jobj, fid, (int) icmp->micmp_icmpOutAddrMasks);

    //
    // micmp_icmpOutAddrMaskReps
    //
    fid = env->GetFieldID(cls, "micmp_icmpOutAddrMaskReps", "I");
    if (fid == 0)
    {
        fail(env, jobj, cls, "mdr_icmp_t::icmp_set: cannot find micmp_icmpOutAddrMaskReps");
        return false;
    }
    env->SetIntField(jobj, fid, (int) icmp->micmp_icmpOutAddrMaskReps);

    return true;
}

extern "C" JNIEXPORT jboolean JNICALL
Java_com_capmet_metrics_jmdr_mdr_1icmp_1t_icmp_1constructor
    (JNIEnv *env, jobject jobj, jstring jhost)
{
    jclass cls = env->GetObjectClass(jobj);
    jfieldID fid;
    mdr::mdr_icmp_t *icmp;

    fid = env->GetFieldID(cls, "handle", "J");
    if (fid == 0)
    {
        fail(env, jobj, cls, "mdr_icmp_t::mdr_icmp_t: cannot find handle");
        return JNI_FALSE;
    }
    try
    {
        const char *remote_host = 0;

        if (jhost != 0)
            remote_host = env->GetStringUTFChars(jhost, 0);

        icmp = new mdr::mdr_icmp_t(remote_host);

        if (remote_host)
            env->ReleaseStringUTFChars(jhost, remote_host);

        if (!mdr_constructor(env, jobj, icmp))
            return JNI_FALSE;
    }
    catch(std::exception & e)
    {
        fail(env, jobj, cls, e.what());
        return JNI_FALSE;
    }
    env->SetLongField(jobj, fid, (jlong) _POINTER_CAST_ icmp);

    return (icmp_set(env, jobj, cls, icmp) ? JNI_TRUE : JNI_FALSE);
}

extern "C" JNIEXPORT void JNICALL
Java_com_capmet_metrics_jmdr_mdr_1icmp_1t_finalize
	(JNIEnv *env, jobject jobj)
{
	jclass cls = env->GetObjectClass(jobj);
	jfieldID fid;
    mdr::mdr_icmp_t *icmp;

	fid = env->GetFieldID(cls, "handle", "J");
	if (fid == 0)
		return;
    icmp = (mdr::mdr_icmp_t *) _POINTER_CAST_ env->GetLongField(jobj, fid);
	if (icmp == 0)
		return;

	delete icmp;

	env->SetIntField(jobj, fid, 0);
}

extern "C" JNIEXPORT jboolean JNICALL
Java_com_capmet_metrics_jmdr_mdr_1icmp_1t_icmp_1refresh
	(JNIEnv *env, jobject jobj)
{
    jclass cls = env->GetObjectClass(jobj);
    jfieldID fid;
    mdr::mdr_icmp_t *icmp;

    fid = env->GetFieldID(cls, "handle", "J");
    if (fid == 0)
    {
        fail(env, jobj, cls, "mdr_icmp_t::icmp_refresh: cannot find handle");
        return JNI_FALSE;
    }
    icmp = (mdr::mdr_icmp_t *) _POINTER_CAST_ env->GetLongField(jobj, fid);
    if (icmp == 0)
    {
        fail(env, jobj, cls, "mdr_icmp_t::icmp_refresh: null mdr_icmp_t handle");
        return JNI_FALSE;
    }
    try
    {
        icmp->mdr_refresh();
    }
    catch(std::exception & e)
    {
        fail(env, jobj, cls, e.what());
        return JNI_FALSE;
    }

    return (icmp_set(env, jobj, cls, icmp) ? JNI_TRUE : JNI_FALSE);
}

extern "C" JNIEXPORT jboolean JNICALL
Java_com_capmet_metrics_jmdr_mdr_1icmp_1t_icmp_1rates
	(JNIEnv *env, jobject jobj)
{
    jclass cls = env->GetObjectClass(jobj);
    jfieldID fid;
    mdr::mdr_icmp_t *icmp;

    fid = env->GetFieldID(cls, "handle", "J");
    if (fid == 0)
    {
        fail(env, jobj, cls, "mdr_icmp_t::icmp_rates: cannot find handle");
        return JNI_FALSE;
    }
    icmp = (mdr::mdr_icmp_t *) _POINTER_CAST_ env->GetLongField(jobj, fid);
    if (icmp == 0)
    {
        fail(env, jobj, cls, "mdr_icmp_t::icmp_rates: null mdr_icmp_t handle");
        return JNI_FALSE;
    }
    try
    {
        icmp->mdr_rates();
    }
    catch(std::exception & e)
    {
        fail(env, jobj, cls, e.what());
        return JNI_FALSE;
    }

    return (icmp_set(env, jobj, cls, icmp) ? JNI_TRUE : JNI_FALSE);
}
