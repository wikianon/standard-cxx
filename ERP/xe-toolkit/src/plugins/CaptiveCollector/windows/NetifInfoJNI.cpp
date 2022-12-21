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
#include "netifinfo.h"

static void fail(JNIEnv *env, jobject jobj, jclass cls, const char *msg)
{
    jfieldID reason;
    jstring jstr;

    reason = env->GetFieldID(cls, "reason", "Ljava/lang/String;");
    if (reason == 0)
    {
        std::cerr << "NetifInfo::fail: cannot fail" << std::endl;
        return;
    }
    jstr = env->NewStringUTF(msg);
    env->SetObjectField(jobj, reason, jstr);
}

/*
 * Class:     com_capmet_plugins_CaptiveCollector_Windows_NetifInfo
 * Method:    netifInfoConstructor
 * Signature: ()Z
 */
extern "C" JNIEXPORT jboolean JNICALL
Java_com_capmet_plugins_CaptiveCollector_Windows_NetifInfo_netifInfoConstructor
  (JNIEnv *env, jobject jobj)
{
    jclass cls = env->GetObjectClass(jobj);
    jfieldID fid;
    netif_info_t *ni;

    fid = env->GetFieldID(cls, "handle", "J");
    if (fid == 0)
    {
        fail(env, jobj, cls, "NetifInfo::NetifInfo: cannot find handle");
        return JNI_FALSE;
    }
    try
    {
        ni = new netif_info_t();
    }
    catch(std::exception &e)
    {
        fail(env, jobj, cls, e.what());
        return JNI_FALSE;
    }

    env->SetLongField(jobj, fid, (jlong) ni);

    return JNI_TRUE;
}

/*
 * Class:     com_capmet_plugins_CaptiveCollector_Windows_NetifInfo
 * Method:    netifInfoRefresh
 * Signature: ()Z
 */
extern "C" JNIEXPORT jboolean JNICALL
Java_com_capmet_plugins_CaptiveCollector_Windows_NetifInfo_netifInfoRefresh
  (JNIEnv *env, jobject jobj)
{
    jclass cls = env->GetObjectClass(jobj);
    jfieldID fid;
    netif_info_t *ni;

    fid = env->GetFieldID(cls, "handle", "J");
    if (fid == 0)
    {
        fail(env, jobj, cls, "NetifInfo::NetifInfo: cannot find handle");
        return JNI_FALSE;
    }
    ni = (netif_info_t *) env->GetLongField(jobj, fid);
    if (ni == 0)
    {
        fail(env, jobj, cls, "NetifInfo::categoryRefresh: null netif_info_t handle");
        return JNI_FALSE;
    }

    try
    {
        ni->refresh();
    }
    catch(std::exception &e)
    {
        fail(env, jobj, cls, e.what());
        return JNI_FALSE;
    }

    return JNI_TRUE;
}

/*
 * Class:     com_capmet_plugins_CaptiveCollector_Windows_NetifInfo
 * Method:    netifInfoInstanceNames
 * Signature: ()[Ljava/lang/String;
 */
extern "C" JNIEXPORT jobjectArray JNICALL
Java_com_capmet_plugins_CaptiveCollector_Windows_NetifInfo_netifInfoInstanceNames
  (JNIEnv *env, jobject jobj)
{
    jclass cls = env->GetObjectClass(jobj);
    jfieldID fid;
    netif_info_t *ni;
    mdr::mdr_keyset_t names;
    int i;
    jsize n;

    fid = env->GetFieldID(cls, "handle", "J");
    if (fid == 0)
    {
        fail(env, jobj, cls, "NetifInfo::NetifInfo: cannot find handle");
        return JNI_FALSE;
    }
    ni = (netif_info_t *) env->GetLongField(jobj, fid);
    if (ni == 0)
    {
        fail(env, jobj, cls, "NetifInfo::getInstanceNames: null netif_info_t handle");
        return JNI_FALSE;
    }

    ni->getInstanceNames(names);
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
 * Class:     com_capmet_plugins_CaptiveCollector_Windows_NetifInfo
 * Method:    netifInfoInstance
 * Signature: (Ljava/lang/String;)Lcom/capmet/plugins/CaptiveCollector/Windows/NetifInfoCan;
 */
extern "C" JNIEXPORT jobject JNICALL
Java_com_capmet_plugins_CaptiveCollector_Windows_NetifInfo_netifInfoInstance
  (JNIEnv *env, jobject jobj, jstring jinstance)
{
    jclass cls = env->GetObjectClass(jobj);
    const char *instance = env->GetStringUTFChars(jinstance, 0);
    jfieldID fid;
    netif_info_t *ni;
    ibox_node_t node;

    fid = env->GetFieldID(cls, "handle", "J");
    if (fid == 0)
    {
        fail(env, jobj, cls, "NetifInfo::getField: cannot find handle");
        return 0;
    }
    ni = (netif_info_t *) env->GetLongField(jobj, fid);
    if (ni == 0)
    {
        fail(env, jobj, cls, "NetifInfo::getField: null netif_info_t handle");
        return 0;
    }

    node = ni->find(instance);
    if (node == ni->end())
    {
        char msg[128];

        _snprintf_s(msg, sizeof msg, sizeof msg, "NetifInfo::getField: cannot find instance %s", instance);
        env->ReleaseStringUTFChars(jinstance, instance);
        fail(env, jobj, cls, msg);
        return 0;
    }
    env->ReleaseStringUTFChars(jinstance, instance);

    jclass canClass = env->FindClass("com/capmet/plugins/CaptiveCollector/Windows/NetifInfoCan");
    if (canClass == 0)
    {
        fail(env, jobj, cls, "NetifInfo::getField: cannot find NetifInfoCan");
        return 0;
    }
    jmethodID canId = env->GetMethodID(canClass, "<init>", "()V");
    if (canId == 0)
    {
        fail(env, jobj, cls, "NetifInfo::getField: cannot find NetifInfoCan::NetifInfoCan");
        return 0;
    }
    jobject can = env->NewObject(canClass, canId, 0);

    fid = env->GetFieldID(canClass, "ipAddress", "Ljava/lang/String;");
    if (fid == 0)
    {
        fail(env, jobj, cls, "NetifInfo::getField: cannot find ipAddress");
        return 0;
    }
    jstring ipAddress = env->NewStringUTF(node->second.ipAddress.c_str());
    env->SetObjectField(can, fid, ipAddress);

    fid = env->GetFieldID(canClass, "subnetMask", "Ljava/lang/String;");
    if (fid == 0)
    {
        fail(env, jobj, cls, "NetifInfo::getField: cannot find subnetMask");
        return 0;
    }
    jstring subnetMask = env->NewStringUTF(node->second.subnetMask.c_str());
    env->SetObjectField(can, fid, subnetMask);

    return can;
}
