/****************************************************************************
**
** Copyright (c) 2006-2007 Captive Metrics Software Corporation.
**                    All rights reserved.
**
** This file is part of the Windows plugin library for CaptiveCollector.
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


#include "Stdafx.h"
#include <iostream>
#include <exception>
#include <jni.h>
#include "tcp_mib.h"

static void fail(JNIEnv *env, jobject jobj, jclass cls, const char *msg)
{
    jfieldID reason;
    jstring jstr;

    reason = env->GetFieldID(cls, "reason", "Ljava/lang/String;");
    if (reason == 0)
    {
        std::cerr << "TcpMibInfo::fail: cannot fail" << std::endl;
        return;
    }
    jstr = env->NewStringUTF(msg);
    env->SetObjectField(jobj, reason, jstr);
}

static jboolean assign(JNIEnv *env, jobject jobj, jclass cls, tcp_mib_t *mib)
{
    jfieldID fid;

    //
    // tcpRtoAlgorithm
    //
    fid = env->GetFieldID(cls, "tcpRtoAlgorithm", "I");
    if (fid == 0)
    {
        fail(env, jobj, cls,
            "TcpMibInfo::TcpMibInfo: cannot find tcpRtoAlgorithm");
        return JNI_FALSE;
    }
    env->SetIntField(jobj, fid, mib->tcpRtoAlgorithm);

    //
    // tcpRtoMin
    //
    fid = env->GetFieldID(cls, "tcpRtoMin", "I");
    if (fid == 0)
    {
        fail(env, jobj, cls,
            "TcpMibInfo::TcpMibInfo: cannot find tcpRtoMin");
        return JNI_FALSE;
    }
    env->SetIntField(jobj, fid, mib->tcpRtoMin);

    //
    // tcpRtoMax
    //
    fid = env->GetFieldID(cls, "tcpRtoMax", "I");
    if (fid == 0)
    {
        fail(env, jobj, cls,
            "TcpMibInfo::TcpMibInfo: cannot find tcpRtoMax");
        return JNI_FALSE;
    }
    env->SetIntField(jobj, fid, mib->tcpRtoMax);

    //
    // tcpMaxConn
    //
    fid = env->GetFieldID(cls, "tcpMaxConn", "I");
    if (fid == 0)
    {
        fail(env, jobj, cls,
            "TcpMibInfo::TcpMibInfo: cannot find tcpMaxConn");
        return JNI_FALSE;
    }
    env->SetIntField(jobj, fid, mib->tcpMaxConn);

    //
    // tcpActiveOpens
    //
    fid = env->GetFieldID(cls, "tcpActiveOpens", "I");
    if (fid == 0)
    {
        fail(env, jobj, cls,
            "TcpMibInfo::TcpMibInfo: cannot find tcpActiveOpens");
        return JNI_FALSE;
    }
    env->SetIntField(jobj, fid, mib->tcpActiveOpens);

    //
    // tcpPassiveOpens
    //
    fid = env->GetFieldID(cls, "tcpPassiveOpens", "I");
    if (fid == 0)
    {
        fail(env, jobj, cls,
            "TcpMibInfo::TcpMibInfo: cannot find tcpPassiveOpens");
        return JNI_FALSE;
    }
    env->SetIntField(jobj, fid, mib->tcpPassiveOpens);

    //
    // tcpAttemptFails
    //
    fid = env->GetFieldID(cls, "tcpAttemptFails", "I");
    if (fid == 0)
    {
        fail(env, jobj, cls,
            "TcpMibInfo::TcpMibInfo: cannot find tcpAttemptFails");
        return JNI_FALSE;
    }
    env->SetIntField(jobj, fid, mib->tcpAttemptFails);

    //
    // tcpEstabResets
    //
    fid = env->GetFieldID(cls, "tcpEstabResets", "I");
    if (fid == 0)
    {
        fail(env, jobj, cls,
            "TcpMibInfo::TcpMibInfo: cannot find tcpEstabResets");
        return JNI_FALSE;
    }
    env->SetIntField(jobj, fid, mib->tcpEstabResets);

    //
    // tcpCurrEstab
    //
    fid = env->GetFieldID(cls, "tcpCurrEstab", "I");
    if (fid == 0)
    {
        fail(env, jobj, cls,
            "TcpMibInfo::TcpMibInfo: cannot find tcpCurrEstab");
        return JNI_FALSE;
    }
    env->SetIntField(jobj, fid, mib->tcpCurrEstab);

    //
    // tcpInSegs
    //
    fid = env->GetFieldID(cls, "tcpInSegs", "I");
    if (fid == 0)
    {
        fail(env, jobj, cls,
            "TcpMibInfo::TcpMibInfo: cannot find tcpInSegs");
        return JNI_FALSE;
    }
    env->SetIntField(jobj, fid, mib->tcpInSegs);

    //
    // tcpOutSegs
    //
    fid = env->GetFieldID(cls, "tcpOutSegs", "I");
    if (fid == 0)
    {
        fail(env, jobj, cls,
            "TcpMibInfo::TcpMibInfo: cannot find tcpOutSegs");
        return JNI_FALSE;
    }
    env->SetIntField(jobj, fid, mib->tcpOutSegs);

    //
    // tcpRetransSegs
    //
    fid = env->GetFieldID(cls, "tcpRetransSegs", "I");
    if (fid == 0)
    {
        fail(env, jobj, cls,
            "TcpMibInfo::TcpMibInfo: cannot find tcpRetransSegs");
        return JNI_FALSE;
    }
    env->SetIntField(jobj, fid, mib->tcpRetransSegs);

    //
    // tcpInErrs
    //
    fid = env->GetFieldID(cls, "tcpInErrs", "I");
    if (fid == 0)
    {
        fail(env, jobj, cls,
            "TcpMibInfo::TcpMibInfo: cannot find tcpInErrs");
        return JNI_FALSE;
    }
    env->SetIntField(jobj, fid, mib->tcpInErrs);

    //
    // tcpOutRsts
    //
    fid = env->GetFieldID(cls, "tcpOutRsts", "I");
    if (fid == 0)
    {
        fail(env, jobj, cls,
            "TcpMibInfo::TcpMibInfo: cannot find tcpOutRsts");
        return JNI_FALSE;
    }
    env->SetIntField(jobj, fid, mib->tcpOutRsts);

    return JNI_TRUE;
}

extern "C" JNIEXPORT jboolean JNICALL
Java_com_capmet_plugins_CaptiveCollector_Windows_TcpMibInfo_tcpInfoConstructor
  (JNIEnv *env, jobject jobj)
{
    jclass cls = env->GetObjectClass(jobj);
    jfieldID fid;
    tcp_mib_t *mib;

    fid = env->GetFieldID(cls, "handle", "J");
    if (fid == 0)
    {
        fail(env, jobj, cls, "TcpMibInfo::TcpMibInfo: cannot find handle");
        return JNI_FALSE;
    }
    try
    {
        mib = new tcp_mib_t();
    }
    catch(std::exception &e)
    {
        fail(env, jobj, cls, e.what());
        return JNI_FALSE;
    }
    env->SetLongField(jobj, fid, (jlong) mib);

    return assign(env, jobj, cls, mib);
}

extern "C" JNIEXPORT jboolean JNICALL
Java_com_capmet_plugins_CaptiveCollector_Windows_TcpMibInfo_tcpMibInfoRefresh
  (JNIEnv *env, jobject jobj)
{
    jclass cls = env->GetObjectClass(jobj);
    jfieldID fid;
    tcp_mib_t *mib;

    fid = env->GetFieldID(cls, "handle", "J");
    if (fid == 0)
    {
        fail(env, jobj, cls, "TcpMibInfo::categoryRefresh: cannot find handle");
        return 0;
    }

    mib = (tcp_mib_t *) env->GetLongField(jobj, fid);
    if (mib == 0)
    {
        fail(env, jobj, cls, "TcpMibInfo::categoryRefresh: null handle");
        return 0;
    }

    try
    {
        mib->tcp_refresh();
    }
    catch(std::exception &e)
    {
        fail(env, jobj, cls, e.what());
        return JNI_FALSE;
    }

    return assign(env, jobj, cls, mib);
}

extern "C" JNIEXPORT jboolean JNICALL
Java_com_capmet_plugins_CaptiveCollector_Windows_TcpMibInfo_tcpMibInfoRefreshRates
  (JNIEnv *env, jobject jobj)
{
    jclass cls = env->GetObjectClass(jobj);
    jfieldID fid;
    tcp_mib_t *mib;

    fid = env->GetFieldID(cls, "handle", "J");
    if (fid == 0)
    {
        fail(env, jobj, cls, "TcpMibInfo::categoryRefreshRates: cannot find handle");
        return 0;
    }

    mib = (tcp_mib_t *) env->GetLongField(jobj, fid);
    if (mib == 0)
    {
        fail(env, jobj, cls, "TcpMibInfo::categoryRefreshRates: null handle");
        return 0;
    }

    try
    {
        mib->tcp_refresh_rates();
    }
    catch(std::exception &e)
    {
        fail(env, jobj, cls, e.what());
        return JNI_FALSE;
    }

    return assign(env, jobj, cls, mib);
}
