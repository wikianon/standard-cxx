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
#include <mdr_mnt.h>
#include <jni.h>

extern bool mdr_constructor(JNIEnv *, jobject, mdr::mdr_t *);

static void fail(JNIEnv *env, jobject jobj, jclass cls, const char *msg)
{
    jfieldID reason;
    jstring jstr;

    reason = env->GetFieldID(cls, "reason", "Ljava/lang/String;");
    if (reason == 0)
    {
        std::cerr << "mdr_mnt_t::fail: cannot fail" << std::endl;
        return;
    }
    jstr = env->NewStringUTF(msg);
    env->SetObjectField(jobj, reason, jstr);
}

extern "C" JNIEXPORT jboolean JNICALL
Java_com_capmet_metrics_jmdr_mdr_1mnt_1t_mnt_1constructor
    (JNIEnv *env, jobject jobj, jstring jhost)
{
    jclass cls = env->GetObjectClass(jobj);
    jfieldID fid;
    mdr::mdr_mnt_t *mnt;

    fid = env->GetFieldID(cls, "handle", "J");
    if (fid == 0)
    {
        fail(env, jobj, cls, "mdr_mnt_t::mdr_mnt_t: cannot find handle");
        return JNI_FALSE;
    }
    try
    {
        const char *remote_host = 0;

        if (jhost != 0)
            remote_host = env->GetStringUTFChars(jhost, 0);

        mnt = new mdr::mdr_mnt_t(remote_host);

        if (remote_host)
            env->ReleaseStringUTFChars(jhost, remote_host);

        if (!mdr_constructor(env, jobj, mnt))
            return JNI_FALSE;
    }
    catch(std::exception &e)
    {
        fail(env, jobj, cls, e.what());
        return JNI_FALSE;
    }
    env->SetLongField(jobj, fid, (jlong) _POINTER_CAST_ mnt);

    return JNI_TRUE;
}

extern "C" JNIEXPORT void JNICALL
Java_com_capmet_metrics_jmdr_mdr_1mnt_1t_finalize
	(JNIEnv *env, jobject jobj)
{
	jclass cls = env->GetObjectClass(jobj);
	jfieldID fid;
    mdr::mdr_mnt_t *mnt;

	fid = env->GetFieldID(cls, "handle", "J");
	if (fid == 0)
		return;
    mnt = (mdr::mdr_mnt_t *) _POINTER_CAST_ env->GetLongField(jobj, fid);
	if (mnt == 0)
		return;

	delete mnt;

	env->SetIntField(jobj, fid, 0);
}

extern "C" JNIEXPORT jboolean JNICALL
Java_com_capmet_metrics_jmdr_mdr_1mnt_1t_mnt_1refresh
	(JNIEnv *env, jobject jobj)
{
    jclass cls = env->GetObjectClass(jobj);
    jfieldID fid;
    mdr::mdr_mnt_t *mnt;

    fid = env->GetFieldID(cls, "handle", "J");
    if (fid == 0)
    {
        fail(env, jobj, cls, "mdr_mnt_t::mnt_refresh: cannot find handle");
        return JNI_FALSE;
    }
    mnt = (mdr::mdr_mnt_t *) _POINTER_CAST_ env->GetLongField(jobj, fid);
    if (mnt == 0)
    {
        fail(env, jobj, cls, "mdr_mnt_t::mnt_refresh: null mdr::mdr_mnt_t handle");
        return JNI_FALSE;
    }
    try
    {
        mnt->mdr_refresh();
    }
    catch(std::exception &e)
    {
        fail(env, jobj, cls, e.what());
        return JNI_FALSE;
    }

    return JNI_TRUE;
}

extern "C" JNIEXPORT jboolean JNICALL
Java_com_capmet_metrics_jmdr_mdr_1mnt_1t_mnt_1rates
	(JNIEnv *env, jobject jobj)
{
    jclass cls = env->GetObjectClass(jobj);
    jfieldID fid;
    mdr::mdr_mnt_t *mnt;

    fid = env->GetFieldID(cls, "handle", "J");
    if (fid == 0)
    {
        fail(env, jobj, cls, "mdr_mnt_t::mnt_rates: cannot find handle");
        return JNI_FALSE;
    }
    mnt = (mdr::mdr_mnt_t *) _POINTER_CAST_ env->GetLongField(jobj, fid);
    if (mnt == 0)
    {
        fail(env, jobj, cls, "mdr_mnt_t::mnt_rates: null mdr_mnt_t handle");
        return JNI_FALSE;
    }
    try
    {
        mnt->mdr_rates();
    }
    catch(std::exception &e)
    {
        fail(env, jobj, cls, e.what());
        return JNI_FALSE;
    }

    return JNI_TRUE;
}

static bool mnt_set
    (JNIEnv *env, jobject jobj, jobject jcan,
        mdr::mmnt_can_t &can, jint field_number)
{
    jclass cls = env->GetObjectClass(jobj);
    jclass ccls = env->GetObjectClass(jcan);
    jfieldID fid;
    jstring jstr;

    switch(field_number)
    {
    case mdr::MMNT_SPECIAL:
        //
        // mmnt_special
        //
        fid = env->GetFieldID(ccls, "mmnt_special", "Ljava/lang/String;");
        if (fid == 0)
        {
            fail(env, jobj, cls, "cannot find mmnt_special");
            return false;
        }
        jstr = env->NewStringUTF(can.mmnt_special.c_str());
        env->SetObjectField(jcan, fid, jstr);
        break;
    case mdr::MMNT_MOUNT_POINT:
        //
        // mmnt_mount_point
        //
        fid = env->GetFieldID(ccls, "mmnt_mount_point", "Ljava/lang/String;");
        if (fid == 0)
        {
            fail(env, jobj, cls, "cannot find mmnt_mount_point");
            return false;
        }
        jstr = env->NewStringUTF(can.mmnt_mount_point.c_str());
        env->SetObjectField(jcan, fid, jstr);
        break;
    case mdr::MMNT_FS_TYPE:
        //
        // mmnt_fs_type
        //
        fid = env->GetFieldID(ccls, "mmnt_fs_type", "Ljava/lang/String;");
        if (fid == 0)
        {
            fail(env, jobj, cls, "cannot find mmnt_fs_type");
            return false;
        }
        jstr = env->NewStringUTF(can.mmnt_fs_type.c_str());
        env->SetObjectField(jcan, fid, jstr);
        break;
    case mdr::MMNT_MOUNT_OPTIONS:
        //
        // mmnt_mount_options
        //
        fid = env->GetFieldID(ccls, "mmnt_mount_options", "Ljava/lang/String;");
        if (fid == 0)
        {
            fail(env, jobj, cls, "cannot find mmnt_mount_options");
            return false;
        }
        jstr = env->NewStringUTF(can.mmnt_mount_options.c_str());
        env->SetObjectField(jcan, fid, jstr);
        break;
    case mdr::MMNT_MOUNT_TIME:
        //
        // mmnt_mount_time
        //
        fid = env->GetFieldID(ccls, "mmnt_mount_time", "J");
        if (fid == 0)
        {
            fail(env, jobj, cls, "cannot find mmnt_mount_time");
            return false;
        }
        env->SetLongField(jcan, fid, (jlong) can.mmnt_mount_time);
        break;
    case mdr::MMNT_SIZE:
        //
        // mmnt_size_K
        //
        fid = env->GetFieldID(ccls, "mmnt_size_K", "J");
        if (fid == 0)
        {
            fail(env, jobj, cls, "cannot find mmnt_size_K");
            return false;
        }
        env->SetLongField(jcan, fid, (jlong) can.mmnt_size_K);
        break;
    case mdr::MMNT_USED_K:
        //
        // mmnt_used_K
        //
        fid = env->GetFieldID(ccls, "mmnt_used_K", "J");
        if (fid == 0)
        {
            fail(env, jobj, cls, "cannot find mmnt_used_K");
            return false;
        }
        env->SetLongField(jcan, fid, (jlong) can.mmnt_used_K);
        break;
    case mdr::MMNT_FREE_K:
        //
        // mmnt_free_K
        //
        fid = env->GetFieldID(ccls, "mmnt_free_K", "J");
        if (fid == 0)
        {
            fail(env, jobj, cls, "cannot find mmnt_free_K");
            return false;
        }
        env->SetLongField(jcan, fid, (jlong) can.mmnt_free_K);
        break;
    case mdr::MMNT_PERCENT_USED:
        //
        // mmnt_percent_used
        //
        fid = env->GetFieldID(ccls, "mmnt_percent_used", "D");
        if (fid == 0)
        {
            fail(env, jobj, cls, "cannot find mmnt_percent_used");
            return false;
        }
        env->SetDoubleField(jcan, fid, can.mmnt_percent_used);
        break;
    case mdr::MMNT_FILES:
        //
        // mmnt_files
        //
        fid = env->GetFieldID(ccls, "mmnt_files", "J");
        if (fid == 0)
        {
            fail(env, jobj, cls, "cannot find mmnt_files");
            return false;
        }
        env->SetLongField(jcan, fid, (jlong) can.mmnt_files);
        break;
    case mdr::MMNT_FREE_FILES:
        //
        // mmnt_free_files
        //
        fid = env->GetFieldID(ccls, "mmnt_free_files", "J");
        if (fid == 0)
        {
            fail(env, jobj, cls, "cannot find mmnt_free_files");
            return false;
        }
        env->SetLongField(jcan, fid, (jlong) can.mmnt_free_files);
        break;
    case mdr::MMNT_MAJOR:
        //
        // mmnt_major
        //
        fid = env->GetFieldID(ccls, "mmnt_major", "I");
        if (fid == 0)
        {
            fail(env, jobj, cls, "cannot find mmnt_major");
            return false;
        }
        env->SetIntField(jcan, fid, (int) can.mmnt_major);
        break;
    case mdr::MMNT_MINOR:
        //
        // mmnt_minor
        //
        fid = env->GetFieldID(ccls, "mmnt_minor", "I");
        if (fid == 0)
        {
            fail(env, jobj, cls, "cannot find mmnt_minor");
            return false;
        }
        env->SetIntField(jcan, fid, (int) can.mmnt_minor);
        break;
    case mdr::MMNT_FILE_SYSTEM_ID:
        //
        // mmnt_file_system_id
        //
        fid = env->GetFieldID(ccls, "mmnt_file_system_id", "J");
        if (fid == 0)
        {
            fail(env, jobj, cls, "cannot find mmnt_file_system_id");
            return false;
        }
        env->SetLongField(jcan, fid, (jlong) can.mmnt_file_system_id);
        break;
    }

    return true;
}

static bool mnt_set
    (JNIEnv *env, jobject jobj, jobject jcan, mdr::mmnt_can_t &can)
{
    return (
        mnt_set(env, jobj, jcan, can, mdr::MMNT_SPECIAL) &&
        mnt_set(env, jobj, jcan, can, mdr::MMNT_MOUNT_POINT) &&
        mnt_set(env, jobj, jcan, can, mdr::MMNT_FS_TYPE) &&
        mnt_set(env, jobj, jcan, can, mdr::MMNT_MOUNT_OPTIONS) &&
        mnt_set(env, jobj, jcan, can, mdr::MMNT_MOUNT_TIME) &&
        mnt_set(env, jobj, jcan, can, mdr::MMNT_SIZE) &&
        mnt_set(env, jobj, jcan, can, mdr::MMNT_USED_K) &&
        mnt_set(env, jobj, jcan, can, mdr::MMNT_FREE_K) &&
        mnt_set(env, jobj, jcan, can, mdr::MMNT_PERCENT_USED) &&
        mnt_set(env, jobj, jcan, can, mdr::MMNT_FILES) &&
        mnt_set(env, jobj, jcan, can, mdr::MMNT_FREE_FILES) &&
        mnt_set(env, jobj, jcan, can, mdr::MMNT_MAJOR) &&
        mnt_set(env, jobj, jcan, can, mdr::MMNT_MINOR) &&
        mnt_set(env, jobj, jcan, can, mdr::MMNT_FILE_SYSTEM_ID));
}

extern "C" JNIEXPORT jint JNICALL
Java_com_capmet_metrics_jmdr_mdr_1mnt_1t_mdr_1size
	(JNIEnv *env, jobject jobj)
{
    jclass cls = env->GetObjectClass(jobj);
    jfieldID fid;
    mdr::mdr_mnt_t *mnt;

    // find handle
    fid = env->GetFieldID(cls, "handle", "J");
    if (fid == 0)
    {
        fail(env, jobj, cls, "mdr_mnt_t::mdr_size: cannot find handle");
        return JNI_FALSE;
    }

    // get the mnt pointer
    mnt = (mdr::mdr_mnt_t *) _POINTER_CAST_ env->GetLongField(jobj, fid);
    if (mnt == 0)
    {
        fail(env, jobj, cls, "mdr_mnt_t::mdr_size: null mdr_mnt_t handle");
        return JNI_FALSE;
    }

    return mnt->size();
}

extern "C" JNIEXPORT jobjectArray JNICALL
Java_com_capmet_metrics_jmdr_mdr_1mnt_1t_mdr_1keyset
	(JNIEnv *env, jobject jobj)
{
    jclass cls = env->GetObjectClass(jobj);
    jfieldID fid;
    mdr::mdr_mnt_t *mnt;
    jobjectArray jarray;

    // find handle
    fid = env->GetFieldID(cls, "handle", "J");
    if (fid == 0)
    {
        fail(env, jobj, cls, "mdr_mnt_t::mdr_keyset: cannot find handle");
        return JNI_FALSE;
    }

    // get the mnt pointer
    mnt = (mdr::mdr_mnt_t *) _POINTER_CAST_ env->GetLongField(jobj, fid);
    if (mnt == 0)
    {
        fail(env, jobj, cls, "mdr_mnt_t::mdr_keyset: null mdr_mnt_t handle");
        return JNI_FALSE;
    }

    // retrieve the keys
    mdr::mdr_keyset_t keyset;
    mdr::mdr_key_t kn;
    int i;

    try
    {
        mnt->mdr_keyset(keyset);
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
Java_com_capmet_metrics_jmdr_mdr_1mnt_1t_mdr_1get
    (JNIEnv *env, jobject jobj, jstring jkey, jobject jcan)
{
    jclass cls = env->GetObjectClass(jobj);
    jfieldID fid;
    mdr::mdr_mnt_t *mnt;

    // find handle
    fid = env->GetFieldID(cls, "handle", "J");
    if (fid == 0)
    {
        fail(env, jobj, cls, "mdr_mnt_t::mdr_get: cannot find handle");
        return JNI_FALSE;
    }

    // get the mnt pointer
    mnt = (mdr::mdr_mnt_t *) _POINTER_CAST_ env->GetLongField(jobj, fid);
    if (mnt == 0)
    {
        fail(env, jobj, cls, "mdr_mnt_t::mdr_get: null mdr_mnt_t handle");
        return JNI_FALSE;
    }

    // retrive the key and perform lookup
    const char *key;
    mdr::mmnt_node_t node;

    try
    {
        key = env->GetStringUTFChars(jkey, 0);
        node = mnt->find(key);
        env->ReleaseStringUTFChars(jkey, key);
    }
    catch(std::exception &e)
    {
        fail(env, jobj, cls, e.what());
        return JNI_FALSE;
    }

    // not there
    if (node == mnt->end())
        return JNI_FALSE;

    // now assign values from can to can
    return (mnt_set(env, jobj, jcan, node->second) ? JNI_TRUE : JNI_FALSE);
}

extern "C" JNIEXPORT jboolean JNICALL
Java_com_capmet_metrics_jmdr_mdr_1mnt_1t_mdr_1get_1field
    (JNIEnv *env, jobject jobj, jstring jkey, jobject jcan, jint field_number)
{
    jclass cls = env->GetObjectClass(jobj);
    jfieldID fid;
    mdr::mdr_mnt_t *mnt;

    // find handle
    fid = env->GetFieldID(cls, "handle", "J");
    if (fid == 0)
    {
        fail(env, jobj, cls, "mdr_mnt_t::mdr_get_field: cannot find handle");
        return JNI_FALSE;
    }

    // get the mnt pointer
    mnt = (mdr::mdr_mnt_t *) _POINTER_CAST_ env->GetLongField(jobj, fid);
    if (mnt == 0)
    {
        fail(env, jobj, cls, "mdr_mnt_t::mdr_get_field: null mdr_mnt_t handle");
        return JNI_FALSE;
    }

    // retrive the key and perform lookup
    const char *key;
    mdr::mmnt_node_t node;

    try
    {
        key = env->GetStringUTFChars(jkey, 0);
        node = mnt->find(key);
        env->ReleaseStringUTFChars(jkey, key);
    }
    catch(std::exception &e)
    {
        fail(env, jobj, cls, e.what());
        return JNI_FALSE;
    }

    // not there
    if (node == mnt->end())
        return JNI_FALSE;

    // now assign values from can to can
    return (mnt_set(env, jobj, jcan, node->second, field_number) ? JNI_TRUE : JNI_FALSE);
}
