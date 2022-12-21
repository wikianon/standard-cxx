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
#include <mdr_category.h>
#include <jni.h>

static void fail(JNIEnv *env, jobject jobj, jclass cls, const char *msg)
{
    jfieldID reason;
    jstring jstr;

    reason = env->GetFieldID(cls, "reason", "Ljava/lang/String;");
    if (reason == 0)
    {
        std::cerr << "mdr_category_t::fail: cannot fail" << std::endl;
        return;
    }
    jstr = env->NewStringUTF(msg);
    env->SetObjectField(jobj, reason, jstr);
}

extern "C" JNIEXPORT jboolean JNICALL
Java_com_capmet_metrics_jmdr_mdr_1category_1t_category_1constructor
    (JNIEnv *env, jobject jobj, jstring jcategory, jstring jhost)
{
    jclass cls = env->GetObjectClass(jobj);
    jfieldID fid;
    mdr::mdr_category_t *cat;

    fid = env->GetFieldID(cls, "handle", "J");
    if (fid == 0)
    {
        fail(env, jobj, cls, "mdr_category_t::mdr_category_t: cannot find handle");
        return JNI_FALSE;
    }
    try
    {
        const char *remote_host = 0;
        const char *category;

        if (jhost != 0)
            remote_host = env->GetStringUTFChars(jhost, 0);

        category = env->GetStringUTFChars(jcategory, 0);

        cat = new mdr::mdr_category_t(category, remote_host);

        if (remote_host)
            env->ReleaseStringUTFChars(jhost, remote_host);
    }
    catch(std::exception & e)
    {
        fail(env, jobj, cls, e.what());
        return JNI_FALSE;
    }
    env->SetLongField(jobj, fid, (jlong) _POINTER_CAST_ cat);

    //
    // mdr_category_supported
    //
    fid = env->GetFieldID(cls, "mdr_category_supported", "Z");
    if (fid == 0)
    {
        fail(env, jobj, cls, "mdr_category_t::mdr_category_t: cannot find mdr_category_supported");
        return JNI_FALSE;
    }
    env->SetBooleanField(jobj, fid,
        cat->mdr_category_supported ? JNI_TRUE : JNI_FALSE);

    //
    // mdr_is_singleton
    //
    fid = env->GetFieldID(cls, "mdr_is_singleton", "Z");
    if (fid == 0)
    {
        fail(env, jobj, cls, "mdr_category_t::mdr_category_t: cannot find mdr_is_singleton");
        return JNI_FALSE;
    }
    env->SetBooleanField(jobj, fid,
        cat->mdr_is_singleton ? JNI_TRUE : JNI_FALSE);

    //
    // mdr_field_count
    //
    fid = env->GetFieldID(cls, "mdr_field_count", "I");
    if (fid == 0)
    {
        fail(env, jobj, cls, "mdr_category_t::mdr_category_t: cannot find mdr_field_count");
        return JNI_FALSE;
    }
    env->SetIntField(jobj, fid, (int) cat->mdr_field_count);

    return JNI_TRUE;
}

extern "C" JNIEXPORT jobject JNICALL
Java_com_capmet_metrics_jmdr_mdr_1category_1t_is_1supported
  (JNIEnv *env, jobject jobj, jint field_number)
{
    jclass cls = env->GetObjectClass(jobj);
    jclass rcls;
    jfieldID fid;
    jobject object;
    jmethodID id;

    fid = env->GetFieldID(cls, "handle", "J");
    if (fid == 0)
    {
        fail(env, jobj, cls, "mdr_category_t::is_supported: cannot find handle");
        return 0;
    }

    mdr::mdr_category_t *m = (mdr::mdr_category_t *) _POINTER_CAST_ env->GetLongField(jobj, fid);
    if (m == 0)
    {
        fail(env, jobj, cls, "mdr_category_t::is_supported: null mdr_category_t handle");
        return 0;
    }

    rcls = env->FindClass("java/lang/Boolean");
    if (rcls == 0)
    {
        fail(env, jobj, cls, "mdr_category_t::is_supported: cannot find java/lang/Boolean");
        return 0;
    }
    id = env->GetMethodID(rcls, "<init>", "(Z)V");
    if (id == 0)
    {
        fail(env, jobj, cls, "mdr_category_t::is_supported: cannot find public Boolean(boolean)");
        return 0;
    }
    object = env->NewObject(rcls, id,
        m->mdr_is_supported(field_number) ? JNI_TRUE : JNI_FALSE, 0);

    return object;
}

extern "C" JNIEXPORT jstring JNICALL
Java_com_capmet_metrics_jmdr_mdr_1category_1t_field_1name
    (JNIEnv *env, jobject jobj, jint field)
{
    jclass cls = env->GetObjectClass(jobj);
    jfieldID fid;

    fid = env->GetFieldID(cls, "handle", "J");
    if (fid == 0)
    {
        fail(env, jobj, cls, "mdr_category_t::field_name: cannot find handle");
        return 0;
    }

    mdr::mdr_category_t *m = (mdr::mdr_category_t *) _POINTER_CAST_ env->GetLongField(jobj, fid);
    if (m == 0)
    {
        fail(env, jobj, cls, "mdr_category_t::field_name: null mdr_category_t handle");
        return 0;
    }

    return env->NewStringUTF(m->mdr_field_name(field).c_str());
}

extern "C" JNIEXPORT jstring JNICALL
Java_com_capmet_metrics_jmdr_mdr_1category_1t_field_1explanation
    (JNIEnv *env, jobject jobj, jint field)
{
    jclass cls = env->GetObjectClass(jobj);
    jfieldID fid;
    std::string explanation;

    fid = env->GetFieldID(cls, "handle", "J");
    if (fid == 0)
    {
        fail(env, jobj, cls, "mdr_category_t::field_explanation: cannot find handle");
        return 0;
    }

    mdr::mdr_category_t *m = (mdr::mdr_category_t *) _POINTER_CAST_ env->GetLongField(jobj, fid);
    if (m == 0)
    {
        fail(env, jobj, cls, "mdr_category_t::field_explanation: null mdr_category_t handle");
        return 0;
    }

    return env->NewStringUTF(m->mdr_field_explanation(field, explanation).c_str());
}

extern "C" JNIEXPORT jstring JNICALL
Java_com_capmet_metrics_jmdr_mdr_1category_1t_category_1explanation
  (JNIEnv *env, jobject jobj)
{
    jclass cls = env->GetObjectClass(jobj);
    jfieldID fid;

    fid = env->GetFieldID(cls, "handle", "J");
    if (fid == 0)
    {
        fail(env, jobj, cls, "mdr_category_t::field_explanation: cannot find handle");
        return 0;
    }

    mdr::mdr_category_t *m = (mdr::mdr_category_t *) _POINTER_CAST_ env->GetLongField(jobj, fid);
    if (m == 0)
    {
        fail(env, jobj, cls, "mdr_category_t::field_explanation: null mdr_category_t handle");
        return 0;
    }

    return env->NewStringUTF(m->mdr_category_explanation().c_str());
}

extern "C" JNIEXPORT jint JNICALL
Java_com_capmet_metrics_jmdr_mdr_1category_1t_field_1name_1to_1number
    (JNIEnv *env, jobject jobj, jstring jfield)
{
    jclass cls = env->GetObjectClass(jobj);
    jfieldID fid;

    fid = env->GetFieldID(cls, "handle", "J");
    if (fid == 0)
    {
        fail(env, jobj, cls, "mdr_category_t::field_name_to_number: cannot find handle");
        return -1;
    }

    mdr::mdr_category_t *m = (mdr::mdr_category_t *) _POINTER_CAST_ env->GetLongField(jobj, fid);
    if (m == 0)
    {
        fail(env, jobj, cls, "mdr_category_t::field_name_to_number: null mdr_category_t handle");
        return -1;
    }

    const char *field = env->GetStringUTFChars(jfield, 0);
    jint field_number = m->mdr_field_name_to_number(field);

    env->ReleaseStringUTFChars(jfield, field);
    return field_number;
}

extern "C" JNIEXPORT jobject JNICALL
Java_com_capmet_metrics_jmdr_mdr_1category_1t_all_1field_1names
    (JNIEnv *env, jobject jobj)
{
    jclass cls = env->GetObjectClass(jobj);
    jfieldID fid;
    mdr::mdr_category_t *mdrp;

    fid = env->GetFieldID(cls, "handle", "J");
    if (fid == 0)
    {
        fail(env, jobj, cls, "mdr_category_t::all_field_names: cannot find handle");
        return 0;
    }
    mdrp = (mdr::mdr_category_t *) _POINTER_CAST_ env->GetLongField(jobj, fid);
    if (mdrp == 0)
    {
        fail(env, jobj, cls, "mdr_category_t::all_field_names: null mdr_category_t handle");
        return 0;
    }

    try
    {
        mdr::mdr_keyset_t keyset;
        const mdr::mdr_keyset_t &names = mdrp->mdr_all_field_names(keyset);

        size_t n = names.size();
        size_t i;

        jobjectArray jarray = env->NewObjectArray(
            n, env->FindClass("java/lang/String"), env->NewStringUTF(""));

        for(i=0; i<n; i++)
        {
            jstring jstr = env->NewStringUTF(names[i].c_str());
            env->SetObjectArrayElement(jarray, i, jstr);
        }

        return jarray;
    }
    catch(std::exception &e)
    {
        fail(env, jobj, cls, e.what());
        return 0;
    }
}

extern "C" JNIEXPORT jobject JNICALL
Java_com_capmet_metrics_jmdr_mdr_1category_1t_supported_1field_1names
    (JNIEnv *env, jobject jobj)
{
    jclass cls = env->GetObjectClass(jobj);
    jfieldID fid;
    mdr::mdr_category_t *mdrp;

    fid = env->GetFieldID(cls, "handle", "J");
    if (fid == 0)
    {
        fail(env, jobj, cls, "mdr_category_t::supported_field_names: cannot find handle");
        return 0;
    }
    mdrp = (mdr::mdr_category_t *) _POINTER_CAST_ env->GetLongField(jobj, fid);
    if (mdrp == 0)
    {
        fail(env, jobj, cls, "mdr_category_t::supported_field_names: null mdr_category_t handle");
        return 0;
    }

    try
    {
        mdr::mdr_keyset_t keyset;
        const mdr::mdr_keyset_t &names = mdrp->mdr_supported_field_names(keyset);

        size_t n = names.size();
        size_t i;

        jobjectArray jarray = env->NewObjectArray(
            n, env->FindClass("java/lang/String"), env->NewStringUTF(""));

        for(i=0; i<n; i++)
        {
            jstring jstr = env->NewStringUTF(names[i].c_str());
            env->SetObjectArrayElement(jarray, i, jstr);
        }

        return jarray;
    }
    catch(std::exception &e)
    {
        fail(env, jobj, cls, e.what());
        return 0;
    }
}

extern "C" JNIEXPORT jboolean JNICALL
Java_com_capmet_metrics_jmdr_mdr_1category_1t_refresh
    (JNIEnv *env, jobject jobj)
{
    jclass cls = env->GetObjectClass(jobj);
    jfieldID fid;
    mdr::mdr_category_t *mdrp;

    fid = env->GetFieldID(cls, "handle", "J");
    if (fid == 0)
    {
        fail(env, jobj, cls, "mdr_category_t::refresh: cannot find handle");
        return JNI_FALSE;
    }
    mdrp = (mdr::mdr_category_t *) _POINTER_CAST_ env->GetLongField(jobj, fid);
    if (mdrp == 0)
    {
        fail(env, jobj, cls, "mdr_category_t::refresh: null mdr_category_t handle");
        return JNI_FALSE;
    }

    try
    {
        mdrp->mdr_refresh();
    }
    catch(std::exception &e)
    {
        fail(env, jobj, cls, e.what());
        return JNI_FALSE;
    }

    return JNI_TRUE;
}

extern "C" JNIEXPORT jboolean JNICALL
Java_com_capmet_metrics_jmdr_mdr_1category_1t_refresh_1rates
    (JNIEnv *env, jobject jobj)
{
    jclass cls = env->GetObjectClass(jobj);
    jfieldID fid;
    mdr::mdr_category_t *mdrp;

    fid = env->GetFieldID(cls, "handle", "J");
    if (fid == 0)
    {
        fail(env, jobj, cls, "mdr_category_t::refresh_rates: cannot find handle");
        return JNI_FALSE;
    }
    mdrp = (mdr::mdr_category_t *) _POINTER_CAST_ env->GetLongField(jobj, fid);
    if (mdrp == 0)
    {
        fail(env, jobj, cls, "mdr_category_t::refresh_rates: null mdr_category_t handle");
        return JNI_FALSE;
    }

    try
    {
        mdrp->mdr_refresh_rates();
    }
    catch(std::exception &e)
    {
        fail(env, jobj, cls, e.what());
        return JNI_FALSE;
    }

    return JNI_TRUE;
}

extern "C" JNIEXPORT jobjectArray JNICALL
Java_com_capmet_metrics_jmdr_mdr_1category_1t_get_1instance_1names
  (JNIEnv *env, jobject jobj)
{
    jclass cls = env->GetObjectClass(jobj);
    jfieldID fid;
    mdr::mdr_category_t *cat;

    fid = env->GetFieldID(cls, "handle", "J");
    if (fid == 0)
    {
        fail(env, jobj, cls, "mdr_category_t::get_instance_names: cannot find handle");
        return 0;
    }
    cat = (mdr::mdr_category_t *) _POINTER_CAST_ env->GetLongField(jobj, fid);
    if (cat == 0)
    {
        fail(env, jobj, cls, "mdr_category_t::get_instance_names: null mdr_category_t handle");
        return 0;
    }

    try
    {
        mdr::mdr_keyset_t keyset;
        const mdr::mdr_keyset_t &names = cat->mdr_get_instance_names(keyset);

        size_t n = names.size();
        size_t i;

        jobjectArray jarray = env->NewObjectArray(
            n, env->FindClass("java/lang/String"), env->NewStringUTF(""));

        for(i=0; i<n; i++)
        {
            jstring jstr = env->NewStringUTF(names[i].c_str());
            env->SetObjectArrayElement(jarray, i, jstr);
        }

        return jarray;
    }
    catch(std::exception &e)
    {
        fail(env, jobj, cls, e.what());
        return 0;
    }
}

extern "C" JNIEXPORT jstring JNICALL
Java_com_capmet_metrics_jmdr_mdr_1category_1t_release_1host
    (JNIEnv *env, jobject jobj, jstring jhost)
{
    try
    {
        const char *host = env->GetStringUTFChars(jhost, 0);

        mdr::mdr_category_t::mdr_release_host(host);
        env->ReleaseStringUTFChars(jhost, host);
    }
    catch(std::exception &e)
    {
        jstring why = env->NewStringUTF(e.what());
        return why;
    }

    return 0;
}

extern "C" JNIEXPORT void JNICALL
Java_com_capmet_metrics_jmdr_mdr_1category_1t_finalize
    (JNIEnv *env, jobject jobj)
{
    jclass cls = env->GetObjectClass(jobj);
    jfieldID fid;
    mdr::mdr_category_t *cat;

    fid = env->GetFieldID(cls, "handle", "J");
    if (fid == 0)
        return;
    cat = (mdr::mdr_category_t *) _POINTER_CAST_ env->GetLongField(jobj, fid);
    if (cat == 0)
        return;

    delete cat;

    env->SetIntField(jobj, fid, 0);
}
