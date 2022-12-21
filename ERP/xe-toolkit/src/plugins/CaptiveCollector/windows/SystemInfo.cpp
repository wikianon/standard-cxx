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
#include "sysinfo.h"

static void fail(JNIEnv *env, jobject jobj, jclass cls, const char *msg)
{
    jfieldID reason;
    jstring jstr;

    reason = env->GetFieldID(cls, "reason", "Ljava/lang/String;");
    if (reason == 0)
    {
        std::cerr << "SystemInfo::fail: cannot fail" << std::endl;
        return;
    }
    jstr = env->NewStringUTF(msg);
    env->SetObjectField(jobj, reason, jstr);
}

/*
 * Class:     com_capmet_plugins_CaptiveCollector_Windows_SystemInfo
 * Method:    sysInfoConstructor
 * Signature: ()Z
 */
extern "C" JNIEXPORT jboolean JNICALL
Java_com_capmet_plugins_CaptiveCollector_Windows_SystemInfo_sysInfoConstructor
  (JNIEnv *env, jobject jobj)
{
    jclass cls = env->GetObjectClass(jobj);
    jfieldID fid;
    sysinfo_t *si;
    jstring jstr;

    fid = env->GetFieldID(cls, "handle", "J");
    if (fid == 0)
    {
        fail(env, jobj, cls, "SystemInfo::SystemInfo: cannot find handle");
        return JNI_FALSE;
    }
    try
    {
        si = new sysinfo_t();
    }
    catch(std::exception &e)
    {
        fail(env, jobj, cls, e.what());
        return JNI_FALSE;
    }
    env->SetLongField(jobj, fid, (jlong) si);

    //
    // MBytesPagingFileTotal
    //
    fid = env->GetFieldID(cls, "MBytesPagingFileTotal", "I");
    if (fid == 0)
    {
        fail(env, jobj, cls,
            "SystemInfo::SystemInfo: cannot find MBytesPagingFileTotal");
        return JNI_FALSE;
    }
    env->SetIntField(jobj, fid, si->MBytesPagingFileTotal);

    //
    // MBytesMemoryTotal
    //
    fid = env->GetFieldID(cls, "MBytesMemoryTotal", "I");
    if (fid == 0)
    {
        fail(env, jobj, cls,
            "SystemInfo::SystemInfo: cannot find MBytesMemoryTotal");
        return JNI_FALSE;
    }
    env->SetIntField(jobj, fid, si->MBytesMemoryTotal);

    //
    // clockTicksPerSecond
    //
    fid = env->GetFieldID(cls, "clockTicksPerSecond", "I");
    if (fid == 0)
    {
        fail(env, jobj, cls,
            "SystemInfo::SystemInfo: cannot find clockTicksPerSecond");
        return JNI_FALSE;
    }
    env->SetIntField(jobj, fid, si->clockTicksPerSecond);

    //
    // osPlatform
    //
    fid = env->GetFieldID(cls, "osPlatform", "Ljava/lang/String;");
    if (fid == 0)
    {
        fail(env, jobj, cls,
            "SystemInfo::SystemInfo: cannot find osPlatform");
        return JNI_FALSE;
    }
    jstr = env->NewStringUTF(si->osPlatform.c_str());
    env->SetObjectField(jobj, fid, jstr);

    //
    // osVersion
    //
    fid = env->GetFieldID(cls, "osVersion", "Ljava/lang/String;");
    if (fid == 0)
    {
        fail(env, jobj, cls,
            "SystemInfo::SystemInfo: cannot find osVersion");
        return JNI_FALSE;
    }
    jstr = env->NewStringUTF(si->osVersion.c_str());
    env->SetObjectField(jobj, fid, jstr);

    //
    // pageSize
    //
    fid = env->GetFieldID(cls, "pageSize", "I");
    if (fid == 0)
    {
        fail(env, jobj, cls,
            "SystemInfo::SystemInfo: cannot find pageSize");
        return JNI_FALSE;
    }
    env->SetIntField(jobj, fid, si->pageSize);

    //
    // cpuArchitecture
    //
    fid = env->GetFieldID(cls, "cpuArchitecture", "Ljava/lang/String;");
    if (fid == 0)
    {
        fail(env, jobj, cls,
            "SystemInfo::SystemInfo: cannot find cpuArchitecture");
        return JNI_FALSE;
    }
    jstr = env->NewStringUTF(si->cpuArchitecture.c_str());
    env->SetObjectField(jobj, fid, jstr);

    //
    // cpuModel
    //
    fid = env->GetFieldID(cls, "cpuModel", "Ljava/lang/String;");
    if (fid == 0)
    {
        fail(env, jobj, cls,
            "SystemInfo::SystemInfo: cannot find cpuModel");
        return JNI_FALSE;
    }
    jstr = env->NewStringUTF(si->cpuModel.c_str());
    env->SetObjectField(jobj, fid, jstr);

    //
    // cpuCountConfigured
    //
    fid = env->GetFieldID(cls, "cpuCountConfigured", "I");
    if (fid == 0)
    {
        fail(env, jobj, cls,
            "SystemInfo::SystemInfo: cannot find cpuCountConfigured");
        return JNI_FALSE;
    }
    env->SetIntField(jobj, fid, si->cpuCountConfigured);

    //
    // cpuCountOnline
    //
    fid = env->GetFieldID(cls, "cpuCountOnline", "I");
    if (fid == 0)
    {
        fail(env, jobj, cls,
            "SystemInfo::SystemInfo: cannot find cpuCountOnline");
        return JNI_FALSE;
    }
    env->SetIntField(jobj, fid, si->cpuCountOnline);

    //
    // cpuClockSpeed
    //
    fid = env->GetFieldID(cls, "cpuClockSpeed", "I");
    if (fid == 0)
    {
        fail(env, jobj, cls,
            "SystemInfo::SystemInfo: cannot find cpuClockSpeed");
        return JNI_FALSE;
    }
    env->SetIntField(jobj, fid, si->cpuClockSpeed);

    return JNI_TRUE;
}

/*
 * Class:     com_capmet_plugins_CaptiveCollector_Windows_SystemInfo
 * Method:    sysInfoRefresh
 * Signature: ()Z
 */
extern "C" JNIEXPORT jboolean JNICALL
Java_com_capmet_plugins_CaptiveCollector_Windows_SystemInfo_sysInfoRefresh
  (JNIEnv *env, jobject jobj)
{
    jclass cls = env->GetObjectClass(jobj);
    jfieldID fid;
    sysinfo_t *si;

    fid = env->GetFieldID(cls, "handle", "J");
    if (fid == 0)
    {
        fail(env, jobj, cls, "SystemInfo::categoryRefresh: cannot find handle");
        return 0;
    }

    si = (sysinfo_t *) env->GetLongField(jobj, fid);
    if (si == 0)
    {
        fail(env, jobj, cls, "SystemInfo::categoryRefresh: null handle");
        return 0;
    }

    try
    {
        si->refresh();
    }
    catch(std::exception &e)
    {
        fail(env, jobj, cls, e.what());
        return JNI_FALSE;
    }

    //
    // MBytesPagingFileTotal
    //
    fid = env->GetFieldID(cls, "MBytesPagingFileTotal", "I");
    if (fid == 0)
    {
        fail(env, jobj, cls,
            "SystemInfo::categoryRefresh: cannot find MBytesPagingFileTotal");
        return JNI_FALSE;
    }
    env->SetIntField(jobj, fid, si->MBytesPagingFileTotal);

    //
    // cpuCountOnline
    //
    fid = env->GetFieldID(cls, "cpuCountOnline", "I");
    if (fid == 0)
    {
        fail(env, jobj, cls,
            "SystemInfo::categoryRefresh: cannot find cpuCountOnline");
        return JNI_FALSE;
    }
    env->SetIntField(jobj, fid, si->cpuCountOnline);

    return JNI_TRUE;
}
