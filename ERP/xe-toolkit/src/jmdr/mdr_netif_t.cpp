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
#include <mdr_netif.h>
#include <jni.h>

extern bool mdr_constructor(JNIEnv *, jobject, mdr::mdr_t *);

static void fail(JNIEnv *env, jobject jobj, jclass cls, const char *msg)
{
    jfieldID reason;
    jstring jstr;

    reason = env->GetFieldID(cls, "reason", "Ljava/lang/String;");
    if (reason == 0)
    {
        std::cerr << "mdr_netif_t::fail: cannot fail" << std::endl;
        return;
    }
    jstr = env->NewStringUTF(msg);
    env->SetObjectField(jobj, reason, jstr);
}

extern "C" JNIEXPORT jboolean JNICALL
Java_com_capmet_metrics_jmdr_mdr_1netif_1t_netif_1constructor
    (JNIEnv *env, jobject jobj, jstring jhost)
{
    jclass cls = env->GetObjectClass(jobj);
    jfieldID fid;
    mdr::mdr_netif_t *netif;

    fid = env->GetFieldID(cls, "handle", "J");
    if (fid == 0)
    {
        fail(env, jobj, cls, "mdr_netif_t::mdr_netif_t: cannot find handle");
        return JNI_FALSE;
    }
    try
    {
        const char *remote_host = 0;

        if (jhost != 0)
            remote_host = env->GetStringUTFChars(jhost, 0);

        netif = new mdr::mdr_netif_t(remote_host);

        if (remote_host)
            env->ReleaseStringUTFChars(jhost, remote_host);

        if (!mdr_constructor(env, jobj, netif))
            return JNI_FALSE;
    }
    catch(std::exception &e)
    {
        fail(env, jobj, cls, e.what());
        return JNI_FALSE;
    }
    env->SetLongField(jobj, fid, (jlong) _POINTER_CAST_ netif);

    return JNI_TRUE;
}

extern "C" JNIEXPORT void JNICALL
Java_com_capmet_metrics_jmdr_mdr_1netif_1t_finalize
	(JNIEnv *env, jobject jobj)
{
	jclass cls = env->GetObjectClass(jobj);
	jfieldID fid;
    mdr::mdr_netif_t *netif;

	fid = env->GetFieldID(cls, "handle", "J");
	if (fid == 0)
		return;
    netif = (mdr::mdr_netif_t *) _POINTER_CAST_ env->GetLongField(jobj, fid);
	if (netif == 0)
		return;

	delete netif;

	env->SetIntField(jobj, fid, 0);
}

extern "C" JNIEXPORT jboolean JNICALL
Java_com_capmet_metrics_jmdr_mdr_1netif_1t_netif_1refresh
	(JNIEnv *env, jobject jobj)
{
    jclass cls = env->GetObjectClass(jobj);
    jfieldID fid;
    mdr::mdr_netif_t *netif;

    fid = env->GetFieldID(cls, "handle", "J");
    if (fid == 0)
    {
        fail(env, jobj, cls, "mdr_netif_t::netif_refresh: cannot find handle");
        return JNI_FALSE;
    }
    netif = (mdr::mdr_netif_t *) _POINTER_CAST_ env->GetLongField(jobj, fid);
    if (netif == 0)
    {
        fail(env, jobj, cls, "mdr_netif_t::netif_refresh: null mdr_netif_t handle");
        return JNI_FALSE;
    }
    try
    {
        netif->mdr_refresh();
    }
    catch(std::exception &e)
    {
        fail(env, jobj, cls, e.what());
        return JNI_FALSE;
    }

    return JNI_TRUE;
}

extern "C" JNIEXPORT jboolean JNICALL
Java_com_capmet_metrics_jmdr_mdr_1netif_1t_netif_1rates
	(JNIEnv *env, jobject jobj)
{
    jclass cls = env->GetObjectClass(jobj);
    jfieldID fid;
    mdr::mdr_netif_t *netif;

    fid = env->GetFieldID(cls, "handle", "J");
    if (fid == 0)
    {
        fail(env, jobj, cls, "mdr_netif_t::netif_rates: cannot find handle");
        return JNI_FALSE;
    }
    netif = (mdr::mdr_netif_t *) _POINTER_CAST_ env->GetLongField(jobj, fid);
    if (netif == 0)
    {
        fail(env, jobj, cls, "mdr_netif_t::netif_rates: null mdr_netif_t handle");
        return JNI_FALSE;
    }
    try
    {
        netif->mdr_rates();
    }
    catch(std::exception &e)
    {
        fail(env, jobj, cls, e.what());
        return JNI_FALSE;
    }

    return JNI_TRUE;
}

static bool netif_set
    (JNIEnv *env, jobject jobj, jobject jcan,
        mdr::mnet_can_t &can, int field_number)
{
    jclass cls = env->GetObjectClass(jobj);
    jclass ccls = env->GetObjectClass(jcan);
    jfieldID fid;
    jstring jstr;

    switch(field_number)
    {
    case mdr::MNET_IP_ADDRESS:
        //
        // mnet_ip_address
        //
        fid = env->GetFieldID(ccls, "mnet_ip_address", "Ljava/lang/String;");
        if (fid == 0)
        {
            fail(env, jobj, cls, "mdr_netif_t::netif_set: cannot find mnet_ip_address");
            return false;
        }
        jstr = env->NewStringUTF(can.mnet_ip_address.c_str());
        env->SetObjectField(jcan, fid, jstr);
        break;
    case mdr::MNET_NETMASK:
        //
        // mnet_netmask
        //
        fid = env->GetFieldID(ccls, "mnet_netmask", "Ljava/lang/String;");
        if (fid == 0)
        {
            fail(env, jobj, cls, "mdr_netif_t::netif_set: cannot find mnet_netmask");
            return false;
        }
        jstr = env->NewStringUTF(can.mnet_netmask.c_str());
        env->SetObjectField(jcan, fid, jstr);
        break;
    case mdr::MNET_BROADCAST_ADDRESS:
        //
        // mnet_broadcast_address
        //
        fid = env->GetFieldID(ccls, "mnet_broadcast_address", "Ljava/lang/String;");
        if (fid == 0)
        {
            fail(env, jobj, cls, "mdr_netif_t::netif_set: cannot find mnet_broadcast_address");
            return false;
        }
        jstr = env->NewStringUTF(can.mnet_broadcast_address.c_str());
        env->SetObjectField(jcan, fid, jstr);
        break;
    case mdr::MNET_MTU:
        //
        // mnet_mtu
        //
        fid = env->GetFieldID(ccls, "mnet_mtu", "I");
        if (fid == 0)
        {
            fail(env, jobj, cls, "mdr_netif_t::netif_set: cannot find mnet_mtu");
            return false;
        }
        env->SetIntField(jcan, fid, (int) can.mnet_mtu);
        break;
    case mdr::MNET_INPUT_BYTES:
        //
        // mnet_input_bytes
        //
        fid = env->GetFieldID(ccls, "mnet_input_bytes", "J");
        if (fid == 0)
        {
            fail(env, jobj, cls, "mdr_netif_t::netif_set: cannot find mnet_input_bytes");
            return false;
        }
        env->SetLongField(jcan, fid, (jlong) can.mnet_input_bytes);
        break;
    case mdr::MNET_OUTPUT_BYTES:
        //
        // mnet_output_bytes
        //
        fid = env->GetFieldID(ccls, "mnet_output_bytes", "J");
        if (fid == 0)
        {
            fail(env, jobj, cls, "mdr_netif_t::netif_set: cannot find mnet_output_bytes");
            return false;
        }
        env->SetLongField(jcan, fid, (jlong) can.mnet_output_bytes);
        break;
    case mdr::MNET_INPUT_PACKETS:
        //
        // mnet_input_packets
        //
        fid = env->GetFieldID(ccls, "mnet_input_packets", "J");
        if (fid == 0)
        {
            fail(env, jobj, cls, "mdr_netif_t::netif_set: cannot find mnet_input_packets");
            return false;
        }
        env->SetLongField(jcan, fid, (jlong) can.mnet_input_packets);
        break;
    case mdr::MNET_OUTPUT_PACKETS:
        //
        // mnet_output_packets
        //
        fid = env->GetFieldID(ccls, "mnet_output_packets", "J");
        if (fid == 0)
        {
            fail(env, jobj, cls, "mdr_netif_t::netif_set: cannot find mnet_output_packets");
            return false;
        }
        env->SetLongField(jcan, fid, (jlong) can.mnet_output_packets);
        break;
    case mdr::MNET_INPUT_ERRORS:
        //
        // mnet_input_errors
        //
        fid = env->GetFieldID(ccls, "mnet_input_errors", "J");
        if (fid == 0)
        {
            fail(env, jobj, cls, "mdr_netif_t::netif_set: cannot find mnet_input_errors");
            return false;
        }
        env->SetLongField(jcan, fid, (jlong) can.mnet_input_errors);
        break;
    case mdr::MNET_OUTPUT_ERRORS:
        //
        // mnet_output_errors
        //
        fid = env->GetFieldID(ccls, "mnet_output_errors", "J");
        if (fid == 0)
        {
            fail(env, jobj, cls, "mdr_netif_t::netif_set: cannot find mnet_output_errors");
            return false;
        }
        env->SetLongField(jcan, fid, (jlong) can.mnet_output_errors);
        break;
    case mdr::MNET_INPUT_FAILURES:
        //
        // mnet_input_failures
        //
        fid = env->GetFieldID(ccls, "mnet_input_failures", "J");
        if (fid == 0)
        {
            fail(env, jobj, cls, "mdr_netif_t::netif_set: cannot find mnet_input_failures");
            return false;
        }
        env->SetLongField(jcan, fid, (jlong) can.mnet_input_failures);
        break;
    case mdr::MNET_OUTPUT_FAILURES:
        //
        // mnet_output_failures
        //
        fid = env->GetFieldID(ccls, "mnet_output_failures", "J");
        if (fid == 0)
        {
            fail(env, jobj, cls, "mdr_netif_t::netif_set: cannot find mnet_output_failures");
            return false;
        }
        env->SetLongField(jcan, fid, (jlong) can.mnet_output_failures);
        break;
    case mdr::MNET_COLLISIONS:
        //
        // mnet_collisions
        //
        fid = env->GetFieldID(ccls, "mnet_collisions", "J");
        if (fid == 0)
        {
            fail(env, jobj, cls, "mdr_netif_t::netif_set: cannot find mnet_collisions");
            return false;
        }
        env->SetLongField(jcan, fid, (jlong) can.mnet_collisions);
        break;
    case mdr::MNET_INTERFACE_SPEED:
        //
        // mnet_interface_speed
        //
        fid = env->GetFieldID(ccls, "mnet_interface_speed", "J");
        if (fid == 0)
        {
            fail(env, jobj, cls, "mdr_netif_t::netif_set: cannot find mnet_interface_speed");
            return false;
        }
        env->SetLongField(jcan, fid, (jlong) can.mnet_interface_speed);
        break;
    case mdr::MNET_COLLISION_PERCENT:
        //
        // mnet_collision_percent
        //
        fid = env->GetFieldID(ccls, "mnet_collision_percent", "D");
        if (fid == 0)
        {
            fail(env, jobj, cls, "mdr_netif_t::netif_set: cannot find mnet_collision_percent");
            return false;
        }
        env->SetDoubleField(jcan, fid, can.mnet_collision_percent);
        break;
    case mdr::MNET_FULL_DUPLEX:
        //
        // mnet_full_duplex
        //
        fid = env->GetFieldID(ccls, "mnet_full_duplex", "Z");
        if (fid == 0)
        {
            fail(env, jobj, cls, "mdr_netif_t::netif_set: cannot find mnet_full_duplex");
            return false;
        }
        env->SetBooleanField(jcan, fid, can.mnet_full_duplex);
        break;
    }

    return true;
}

static bool netif_set
    (JNIEnv *env, jobject jobj, jobject jcan, mdr::mnet_can_t &can)
{
    return (
        netif_set(env, jobj, jcan, can, mdr::MNET_IP_ADDRESS) &&
        netif_set(env, jobj, jcan, can, mdr::MNET_NETMASK) &&
        netif_set(env, jobj, jcan, can, mdr::MNET_BROADCAST_ADDRESS) &&
        netif_set(env, jobj, jcan, can, mdr::MNET_MTU) &&
        netif_set(env, jobj, jcan, can, mdr::MNET_INPUT_BYTES) &&
        netif_set(env, jobj, jcan, can, mdr::MNET_OUTPUT_BYTES) &&
        netif_set(env, jobj, jcan, can, mdr::MNET_INPUT_PACKETS) &&
        netif_set(env, jobj, jcan, can, mdr::MNET_OUTPUT_PACKETS) &&
        netif_set(env, jobj, jcan, can, mdr::MNET_INPUT_ERRORS) &&
        netif_set(env, jobj, jcan, can, mdr::MNET_OUTPUT_ERRORS) &&
        netif_set(env, jobj, jcan, can, mdr::MNET_INPUT_FAILURES) &&
        netif_set(env, jobj, jcan, can, mdr::MNET_OUTPUT_FAILURES) &&
        netif_set(env, jobj, jcan, can, mdr::MNET_COLLISIONS) &&
        netif_set(env, jobj, jcan, can, mdr::MNET_INTERFACE_SPEED) &&
        netif_set(env, jobj, jcan, can, mdr::MNET_COLLISION_PERCENT));
}

extern "C" JNIEXPORT jint JNICALL
Java_com_capmet_metrics_jmdr_mdr_1netif_1t_mdr_1size
	(JNIEnv *env, jobject jobj)
{
    jclass cls = env->GetObjectClass(jobj);
    jfieldID fid;
    mdr::mdr_netif_t *netif;

    // find handle
    fid = env->GetFieldID(cls, "handle", "J");
    if (fid == 0)
    {
        fail(env, jobj, cls, "mdr_netif_t::mdr_size: cannot find handle");
        return JNI_FALSE;
    }

    // get the netif pointer
    netif = (mdr::mdr_netif_t *) _POINTER_CAST_ env->GetLongField(jobj, fid);
    if (netif == 0)
    {
        fail(env, jobj, cls, "mdr_netif_t::mdr_size: null mdr_netif_t handle");
        return JNI_FALSE;
    }

    return netif->size();
}

extern "C" JNIEXPORT jobjectArray JNICALL
Java_com_capmet_metrics_jmdr_mdr_1netif_1t_mdr_1keyset
	(JNIEnv *env, jobject jobj)
{
    jclass cls = env->GetObjectClass(jobj);
    jfieldID fid;
    mdr::mdr_netif_t *netif;
    jobjectArray jarray;

    // find handle
    fid = env->GetFieldID(cls, "handle", "J");
    if (fid == 0)
    {
        fail(env, jobj, cls, "mdr_netif_t::mdr_keyset: cannot find handle");
        return JNI_FALSE;
    }

    // get the netif pointer
    netif = (mdr::mdr_netif_t *) _POINTER_CAST_ env->GetLongField(jobj, fid);
    if (netif == 0)
    {
        fail(env, jobj, cls, "mdr_netif_t::mdr_keyset: null mdr_netif_t handle");
        return JNI_FALSE;
    }

    // retrieve the keys
    mdr::mdr_keyset_t keyset;
    mdr::mdr_key_t kn;
    int i;

    try
    {
        netif->mdr_keyset(keyset);
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
Java_com_capmet_metrics_jmdr_mdr_1netif_1t_mdr_1get
    (JNIEnv *env, jobject jobj, jstring jkey, jobject jcan)
{
    jclass cls = env->GetObjectClass(jobj);
    jfieldID fid;
    mdr::mdr_netif_t *netif;

    // find handle
    fid = env->GetFieldID(cls, "handle", "J");
    if (fid == 0)
    {
        fail(env, jobj, cls, "mdr_netif_t::mdr_get: cannot find handle");
        return JNI_FALSE;
    }

    // get the netif pointer
    netif = (mdr::mdr_netif_t *) _POINTER_CAST_ env->GetLongField(jobj, fid);
    if (netif == 0)
    {
        fail(env, jobj, cls, "mdr_netif_t::mdr_get: null mdr_netif_t handle");
        return JNI_FALSE;
    }

    // retrive the key and perform lookup
    const char *key;
    mdr::mnet_node_t node;

    try
    {
        key = env->GetStringUTFChars(jkey, 0);
        node = netif->find(key);
        env->ReleaseStringUTFChars(jkey, key);
    }
    catch(std::exception &e)
    {
        fail(env, jobj, cls, e.what());
        return JNI_FALSE;
    }

    // not there
    if (node == netif->end())
        return JNI_FALSE;

    // now assign values from can to can
    return (netif_set(env, jobj, jcan, node->second) ? JNI_TRUE : JNI_FALSE);
}

extern "C" JNIEXPORT jboolean JNICALL
Java_com_capmet_metrics_jmdr_mdr_1netif_1t_mdr_1get_1field
  (JNIEnv *env, jobject jobj, jstring jkey, jobject jcan, jint field_number)
{
    jclass cls = env->GetObjectClass(jobj);
    jfieldID fid;
    mdr::mdr_netif_t *netif;

    // find handle
    fid = env->GetFieldID(cls, "handle", "J");
    if (fid == 0)
    {
        fail(env, jobj, cls, "mdr_netif_t::mdr_get_field: cannot find handle");
        return JNI_FALSE;
    }

    // get the netif pointer
    netif = (mdr::mdr_netif_t *) _POINTER_CAST_ env->GetLongField(jobj, fid);
    if (netif == 0)
    {
        fail(env, jobj, cls, "mdr_netif_t::mdr_get_field: null mdr_netif_t handle");
        return JNI_FALSE;
    }

    // retrive the key and perform lookup
    const char *key;
    mdr::mnet_node_t node;

    try
    {
        key = env->GetStringUTFChars(jkey, 0);
        node = netif->find(key);
        env->ReleaseStringUTFChars(jkey, key);
    }
    catch(std::exception &e)
    {
        fail(env, jobj, cls, e.what());
        return JNI_FALSE;
    }

    // not there
    if (node == netif->end())
        return JNI_FALSE;

    // now assign values from can to can
    return (netif_set(env, jobj, jcan, node->second, field_number) ? JNI_TRUE : JNI_FALSE);
}

extern "C" JNIEXPORT void JNICALL
Java_com_capmet_metrics_jmdr_mdr_1netif_1t_mdr_1full_1duplex
  (JNIEnv *env, jobject jobj, jstring jkey, jboolean jduplex)
{
    jclass cls = env->GetObjectClass(jobj);
    jfieldID fid;
    mdr::mdr_netif_t *netif;

    // find handle
    fid = env->GetFieldID(cls, "handle", "J");
    if (fid == 0)
    {
        fail(env, jobj, cls, "mdr_netif_t::mdr_full_duplex: cannot find handle");
        return;
    }

    // get the netif pointer
    netif = (mdr::mdr_netif_t *) _POINTER_CAST_ env->GetLongField(jobj, fid);
    if (netif == 0)
    {
        fail(env, jobj, cls, "mdr_netif_t::mdr_full_duplex: null mdr_netif_t handle");
        return;
    }

    try
    {
        // retrive the key and perform lookup
        const char *key = env->GetStringUTFChars(jkey, 0);
        std::string std_key(key);

        netif->mnet_duplex(std_key, (jduplex == JNI_TRUE ? true : false));
    }
    catch(std::exception &e)
    {
        // shucks
    }
}
