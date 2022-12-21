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
#include "diskinfo.h"

static void fail(JNIEnv *env, jobject jobj, jclass cls, const char *msg)
{
    jfieldID reason;
    jstring jstr;

    reason = env->GetFieldID(cls, "reason", "Ljava/lang/String;");
    if (reason == 0)
    {
        std::cerr << "DiskInfo::fail: cannot fail" << std::endl;
        return;
    }
    jstr = env->NewStringUTF(msg);
    env->SetObjectField(jobj, reason, jstr);
}

/*
 * Class:     com_capmet_plugins_CaptiveCollector_Windows_DiskInfo
 * Method:    diskInfoConstructor
 * Signature: ()Z
 */
extern "C" JNIEXPORT jboolean JNICALL
Java_com_capmet_plugins_CaptiveCollector_Windows_DiskInfo_diskInfoConstructor
  (JNIEnv *env, jobject jobj)
{
    jclass cls = env->GetObjectClass(jobj);
    jfieldID fid;
    diskinfo_t *di;

    fid = env->GetFieldID(cls, "handle", "J");
    if (fid == 0)
    {
        fail(env, jobj, cls, "DiskInfo::DiskInfo: cannot find handle");
        return JNI_FALSE;
    }
    try
    {
        di = new diskinfo_t();
    }
    catch(std::exception &e)
    {
        fail(env, jobj, cls, e.what());
        return JNI_FALSE;
    }

    env->SetLongField(jobj, fid, (jlong) di);

    return JNI_TRUE;
}

/*
 * Class:     com_capmet_plugins_CaptiveCollector_Windows_DiskInfo
 * Method:    diskInfoRefresh
 * Signature: ()Z
 */
extern "C" JNIEXPORT jboolean JNICALL
Java_com_capmet_plugins_CaptiveCollector_Windows_DiskInfo_diskInfoRefresh
  (JNIEnv *env, jobject jobj)
{
    jclass cls = env->GetObjectClass(jobj);
    jfieldID fid;
    diskinfo_t *di;

    fid = env->GetFieldID(cls, "handle", "J");
    if (fid == 0)
    {
        fail(env, jobj, cls, "DiskInfo::DiskInfo: cannot find handle");
        return JNI_FALSE;
    }
    di = (diskinfo_t *) env->GetLongField(jobj, fid);
    if (di == 0)
    {
        fail(env, jobj, cls, "DiskInfo::categoryRefresh: null diskinfo_t handle");
        return JNI_FALSE;
    }

    try
    {
        di->refresh();
    }
    catch(std::exception &e)
    {
        fail(env, jobj, cls, e.what());
        return JNI_FALSE;
    }

    return JNI_TRUE;
}

/*
 * Class:     com_capmet_plugins_CaptiveCollector_Windows_DiskInfo
 * Method:    diskInfoInstanceNames
 * Signature: ()[Ljava/lang/String;
 */
extern "C" JNIEXPORT jobjectArray JNICALL
Java_com_capmet_plugins_CaptiveCollector_Windows_DiskInfo_diskInfoInstanceNames
  (JNIEnv *env, jobject jobj)
{
    jclass cls = env->GetObjectClass(jobj);
    jfieldID fid;
    diskinfo_t *di;
    mdr::mdr_keyset_t names;
    int i;
    jsize n;

    fid = env->GetFieldID(cls, "handle", "J");
    if (fid == 0)
    {
        fail(env, jobj, cls, "DiskInfo::DiskInfo: cannot find handle");
        return JNI_FALSE;
    }
    di = (diskinfo_t *) env->GetLongField(jobj, fid);
    if (di == 0)
    {
        fail(env, jobj, cls, "DiskInfo::getInstanceNames: null diskinfo_t handle");
        return JNI_FALSE;
    }

    di->getInstanceNames(names);
    n = (jsize) names.size();

    jobjectArray jarray = env->NewObjectArray(
        n, env->FindClass("java/lang/String"), env->NewStringUTF(""));

    for(i=0; i<n; i++)
    {
        jstring jstr = env->NewStringUTF(names[i].c_str());
        env->SetObjectArrayElement(jarray, i, jstr);
    }

    return jarray;
}

/*
 * Class:     com_capmet_plugins_CaptiveCollector_Windows_DiskInfo
 * Method:    diskInfoInstance
 * Signature: (Ljava/lang/String;)Lcom/capmet/plugins/CaptiveCollector/Windows/DiskInfoCan;
 */
extern "C" JNIEXPORT jobject JNICALL
Java_com_capmet_plugins_CaptiveCollector_Windows_DiskInfo_diskInfoInstance
  (JNIEnv *env, jobject jobj, jstring jinstance)
{
    jclass cls = env->GetObjectClass(jobj);
    const char *instance = env->GetStringUTFChars(jinstance, 0);
    jfieldID fid;
    diskinfo_t *di;
    dbox_node_t node;

    fid = env->GetFieldID(cls, "handle", "J");
    if (fid == 0)
    {
        fail(env, jobj, cls, "DiskInfo::getField: cannot find handle");
        return 0;
    }
    di = (diskinfo_t *) env->GetLongField(jobj, fid);
    if (di == 0)
    {
        fail(env, jobj, cls, "DiskInfo::getField: null diskinfo_t handle");
        return 0;
    }

    node = di->find(instance);
    if (node == di->end())
    {
        char msg[128];

        _snprintf_s(msg, sizeof msg, sizeof msg, "DiskInfo::getField: cannot find instance %s", instance);
        env->ReleaseStringUTFChars(jinstance, instance);
        fail(env, jobj, cls, msg);
        return 0;
    }
    env->ReleaseStringUTFChars(jinstance, instance);

    jclass canClass = env->FindClass("com/capmet/plugins/CaptiveCollector/Windows/DiskInfoCan");
    if (canClass == 0)
    {
        fail(env, jobj, cls, "DiskInfo::getField: cannot find DiskInfoCan");
        return 0;
    }
    jmethodID canId = env->GetMethodID(canClass, "<init>", "()V");
    if (canId == 0)
    {
        fail(env, jobj, cls, "DiskInfo::getField: cannot find DiskInfoCan::DiskInfoCan");
        return 0;
    }
    jobject can = env->NewObject(canClass, canId, 0);

    fid = env->GetFieldID(canClass, "diskType", "Ljava/lang/String;");
    if (fid == 0)
    {
        fail(env, jobj, cls, "DiskInfo::getField: cannot find diskType");
        return 0;
    }
    jstring diskType = env->NewStringUTF(node->second.diskType.c_str());
    env->SetObjectField(can, fid, diskType);

    fid = env->GetFieldID(canClass, "capacityMB", "I");
    if (fid == 0)
    {
        fail(env, jobj, cls, "DiskInfo::getField: cannot find capacityMB");
        return 0;
    }
    env->SetIntField(can, fid, (jint) node->second.capacityMB);

    return can;
}
