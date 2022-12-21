/****************************************************************************
**
** Copyright (c) 2006-2007 Captive Metrics Software Corporation.
**                    All rights reserved.
**
** This file is part of the Solaris plugin library for CaptiveCollector.
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
#include <mdr_sun_cpu.h>
#include <jni.h>

extern bool mdr_constructor(JNIEnv *, jobject, mdr::mdr_t *);

static void fail(JNIEnv *env, jobject jobj, jclass cls, const char *msg)
{
    jfieldID reason;
    jstring jstr;

    reason = env->GetFieldID(cls, "reason", "Ljava/lang/String;");
    if (reason == 0)
    {
        std::cerr << "mdr_sun_cpu_t::fail: cannot fail" << std::endl;
        return;
    }
    jstr = env->NewStringUTF(msg);
    env->SetObjectField(jobj, reason, jstr);
}

extern "C" JNIEXPORT jboolean JNICALL
Java_com_capmet_plugins_CaptiveCollector_Solaris_mdr_1sun_1cpu_1t_sun_1cpu_1constructor
    (JNIEnv *env, jobject jobj, jstring jhost)
{
    jclass cls = env->GetObjectClass(jobj);
    jfieldID fid;
    mdr::mdr_sun_cpu_t *sun_cpu;

    fid = env->GetFieldID(cls, "handle", "J");
    if (fid == 0)
    {
        fail(env, jobj, cls, "mdr_sun_cpu_t::mdr_sun_cpu_t: cannot find handle");
        return JNI_FALSE;
    }
    try
    {
        const char *remote_host = 0;

        if (jhost != 0)
            remote_host = env->GetStringUTFChars(jhost, 0);

        sun_cpu = new mdr::mdr_sun_cpu_t(remote_host);

        if (remote_host)
            env->ReleaseStringUTFChars(jhost, remote_host);

        if (!mdr_constructor(env, jobj, sun_cpu))
            return JNI_FALSE;
    }
    catch(std::exception & e)
    {
        fail(env, jobj, cls, e.what());
        return JNI_FALSE;
    }
    env->SetLongField(jobj, fid, (jlong) _POINTER_CAST_ sun_cpu);

    return JNI_TRUE;
}

extern "C" JNIEXPORT void JNICALL
Java_com_capmet_plugins_CaptiveCollector_Solaris_mdr_1sun_1cpu_1t_finalize
	(JNIEnv *env, jobject jobj)
{
	jclass cls = env->GetObjectClass(jobj);
	jfieldID fid;
    mdr::mdr_sun_cpu_t *sun_cpu;

	fid = env->GetFieldID(cls, "handle", "J");
	if (fid == 0)
		return;
    sun_cpu = (mdr::mdr_sun_cpu_t *) _POINTER_CAST_ env->GetLongField(jobj, fid);
	if (sun_cpu == 0)
		return;

	delete sun_cpu;

	env->SetIntField(jobj, fid, 0);
}

extern "C" JNIEXPORT jboolean JNICALL
Java_com_capmet_plugins_CaptiveCollector_Solaris_mdr_1sun_1cpu_1t_sun_1cpu_1refresh
	(JNIEnv *env, jobject jobj)
{
    jclass cls = env->GetObjectClass(jobj);
    jfieldID fid;
    mdr::mdr_sun_cpu_t *sun_cpu;

    fid = env->GetFieldID(cls, "handle", "J");
    if (fid == 0)
    {
        fail(env, jobj, cls, "mdr_sun_cpu_t::cpu_refresh: cannot find handle");
        return JNI_FALSE;
    }
    sun_cpu = (mdr::mdr_sun_cpu_t *) _POINTER_CAST_ env->GetLongField(jobj, fid);
    if (sun_cpu == 0)
    {
        fail(env, jobj, cls, "mdr_sun_cpu_t::cpu_refresh: null mdr_sun_cpu_t handle");
        return JNI_FALSE;
    }
    try
    {
        sun_cpu->mdr_refresh();
    }
    catch(std::exception & e)
    {
        fail(env, jobj, cls, e.what());
        return JNI_FALSE;
    }

    return JNI_TRUE;
}

extern "C" JNIEXPORT jboolean JNICALL
Java_com_capmet_plugins_CaptiveCollector_Solaris_mdr_1sun_1cpu_1t_sun_1cpu_1rates
	(JNIEnv *env, jobject jobj)
{
    jclass cls = env->GetObjectClass(jobj);
    jfieldID fid;
    mdr::mdr_sun_cpu_t *sun_cpu;

    fid = env->GetFieldID(cls, "handle", "J");
    if (fid == 0)
    {
        fail(env, jobj, cls, "mdr_sun_cpu_t::sun_cpu_rates: cannot find handle");
        return JNI_FALSE;
    }
    sun_cpu = (mdr::mdr_sun_cpu_t *) _POINTER_CAST_ env->GetLongField(jobj, fid);
    if (sun_cpu == 0)
    {
        fail(env, jobj, cls, "mdr_sun_cpu_t::sun_cpu_rates: null mdr_sun_cpu_t handle");
        return JNI_FALSE;
    }
    try
    {
        sun_cpu->mdr_rates();
    }
    catch(std::exception & e)
    {
        fail(env, jobj, cls, e.what());
        return JNI_FALSE;
    }

    return JNI_TRUE;
}

static bool sun_cpu_set
    (JNIEnv *env, jobject jobj, jobject jcan,
        mdr::msc_can_t &can, jint field_number)
{
    jclass cls = env->GetObjectClass(jobj);
    jclass ccls = env->GetObjectClass(jcan);
    jfieldID fid;

    switch(field_number)
    {
	case mdr::MSC_CPU_INSTANCE:
		fid = env->GetFieldID(ccls, "msc_cpu_instance", "I");
		if (fid == 0)
		{
			fail(env, jobj, cls, "mdr_sun_cpu_t::sun_cpu_set: cannot find msc_cpu_instance");
			return false;
		}
		env->SetIntField(jcan, fid, (int) can.msc_cpu_instance);
		break;
	case mdr::MSC_BREAD:
		fid = env->GetFieldID(ccls, "msc_bread", "I");
		if (fid == 0)
		{
			fail(env, jobj, cls, "mdr_sun_cpu_t::sun_cpu_set: cannot find msc_bread");
			return false;
		}
		env->SetIntField(jcan, fid, (int) can.msc_bread);
		break;
	case mdr::MSC_BWRITE:
		fid = env->GetFieldID(ccls, "msc_bwrite", "I");
		if (fid == 0)
		{
			fail(env, jobj, cls, "mdr_sun_cpu_t::sun_cpu_set: cannot find msc_bwrite");
			return false;
		}
		env->SetIntField(jcan, fid, (int) can.msc_bwrite);
		break;
	case mdr::MSC_LREAD:
		fid = env->GetFieldID(ccls, "msc_lread", "I");
		if (fid == 0)
		{
			fail(env, jobj, cls, "mdr_sun_cpu_t::sun_cpu_set: cannot find msc_lread");
			return false;
		}
		env->SetIntField(jcan, fid, (int) can.msc_lread);
		break;
	case mdr::MSC_LWRITE:
		fid = env->GetFieldID(ccls, "msc_lwrite", "I");
		if (fid == 0)
		{
			fail(env, jobj, cls, "mdr_sun_cpu_t::sun_cpu_set: cannot find msc_lwrite");
			return false;
		}
		env->SetIntField(jcan, fid, (int) can.msc_lwrite);
		break;
	case mdr::MSC_PHREAD:
		fid = env->GetFieldID(ccls, "msc_phread", "I");
		if (fid == 0)
		{
			fail(env, jobj, cls, "mdr_sun_cpu_t::sun_cpu_set: cannot find msc_phread");
			return false;
		}
		env->SetIntField(jcan, fid, (int) can.msc_phread);
		break;
	case mdr::MSC_PHWRITE:
		fid = env->GetFieldID(ccls, "msc_phwrite", "I");
		if (fid == 0)
		{
			fail(env, jobj, cls, "mdr_sun_cpu_t::sun_cpu_set: cannot find msc_phwrite");
			return false;
		}
		env->SetIntField(jcan, fid, (int) can.msc_phwrite);
		break;
	case mdr::MSC_PSWITCH:
		fid = env->GetFieldID(ccls, "msc_pswitch", "I");
		if (fid == 0)
		{
			fail(env, jobj, cls, "mdr_sun_cpu_t::sun_cpu_set: cannot find msc_pswitch");
			return false;
		}
		env->SetIntField(jcan, fid, (int) can.msc_pswitch);
		break;
	case mdr::MSC_TRAP:
		fid = env->GetFieldID(ccls, "msc_trap", "I");
		if (fid == 0)
		{
			fail(env, jobj, cls, "mdr_sun_cpu_t::sun_cpu_set: cannot find msc_trap");
			return false;
		}
		env->SetIntField(jcan, fid, (int) can.msc_trap);
		break;
	case mdr::MSC_INTR:
		fid = env->GetFieldID(ccls, "msc_intr", "I");
		if (fid == 0)
		{
			fail(env, jobj, cls, "mdr_sun_cpu_t::sun_cpu_set: cannot find msc_intr");
			return false;
		}
		env->SetIntField(jcan, fid, (int) can.msc_intr);
		break;
	case mdr::MSC_SYSCALL:
		fid = env->GetFieldID(ccls, "msc_syscall", "I");
		if (fid == 0)
		{
			fail(env, jobj, cls, "mdr_sun_cpu_t::sun_cpu_set: cannot find msc_syscall");
			return false;
		}
		env->SetIntField(jcan, fid, (int) can.msc_syscall);
		break;
	case mdr::MSC_SYSREAD:
		fid = env->GetFieldID(ccls, "msc_sysread", "I");
		if (fid == 0)
		{
			fail(env, jobj, cls, "mdr_sun_cpu_t::sun_cpu_set: cannot find msc_sysread");
			return false;
		}
		env->SetIntField(jcan, fid, (int) can.msc_sysread);
		break;
	case mdr::MSC_SYSWRITE:
		fid = env->GetFieldID(ccls, "msc_syswrite", "I");
		if (fid == 0)
		{
			fail(env, jobj, cls, "mdr_sun_cpu_t::sun_cpu_set: cannot find msc_syswrite");
			return false;
		}
		env->SetIntField(jcan, fid, (int) can.msc_syswrite);
		break;
	case mdr::MSC_SYSFORK:
		fid = env->GetFieldID(ccls, "msc_sysfork", "I");
		if (fid == 0)
		{
			fail(env, jobj, cls, "mdr_sun_cpu_t::sun_cpu_set: cannot find msc_sysfork");
			return false;
		}
		env->SetIntField(jcan, fid, (int) can.msc_sysfork);
		break;
	case mdr::MSC_SYSVFORK:
		fid = env->GetFieldID(ccls, "msc_sysvfork", "I");
		if (fid == 0)
		{
			fail(env, jobj, cls, "mdr_sun_cpu_t::sun_cpu_set: cannot find msc_sysvfork");
			return false;
		}
		env->SetIntField(jcan, fid, (int) can.msc_sysvfork);
		break;
	case mdr::MSC_SYSEXEC:
		fid = env->GetFieldID(ccls, "msc_sysexec", "I");
		if (fid == 0)
		{
			fail(env, jobj, cls, "mdr_sun_cpu_t::sun_cpu_set: cannot find msc_sysexec");
			return false;
		}
		env->SetIntField(jcan, fid, (int) can.msc_sysexec);
		break;
	case mdr::MSC_READCH:
		fid = env->GetFieldID(ccls, "msc_readch", "I");
		if (fid == 0)
		{
			fail(env, jobj, cls, "mdr_sun_cpu_t::sun_cpu_set: cannot find msc_readch");
			return false;
		}
		env->SetIntField(jcan, fid, (int) can.msc_readch);
		break;
	case mdr::MSC_WRITECH:
		fid = env->GetFieldID(ccls, "msc_writech", "I");
		if (fid == 0)
		{
			fail(env, jobj, cls, "mdr_sun_cpu_t::sun_cpu_set: cannot find msc_writech");
			return false;
		}
		env->SetIntField(jcan, fid, (int) can.msc_writech);
		break;
	case mdr::MSC_RAWCH:
		fid = env->GetFieldID(ccls, "msc_rawch", "I");
		if (fid == 0)
		{
			fail(env, jobj, cls, "mdr_sun_cpu_t::sun_cpu_set: cannot find msc_rawch");
			return false;
		}
		env->SetIntField(jcan, fid, (int) can.msc_rawch);
		break;
	case mdr::MSC_CANCH:
		fid = env->GetFieldID(ccls, "msc_canch", "I");
		if (fid == 0)
		{
			fail(env, jobj, cls, "mdr_sun_cpu_t::sun_cpu_set: cannot find msc_canch");
			return false;
		}
		env->SetIntField(jcan, fid, (int) can.msc_canch);
		break;
	case mdr::MSC_OUTCH:
		fid = env->GetFieldID(ccls, "msc_outch", "I");
		if (fid == 0)
		{
			fail(env, jobj, cls, "mdr_sun_cpu_t::sun_cpu_set: cannot find msc_outch");
			return false;
		}
		env->SetIntField(jcan, fid, (int) can.msc_outch);
		break;
	case mdr::MSC_MSG:
		fid = env->GetFieldID(ccls, "msc_msg", "I");
		if (fid == 0)
		{
			fail(env, jobj, cls, "mdr_sun_cpu_t::sun_cpu_set: cannot find msc_msg");
			return false;
		}
		env->SetIntField(jcan, fid, (int) can.msc_msg);
		break;
	case mdr::MSC_SEMA:
		fid = env->GetFieldID(ccls, "msc_sema", "I");
		if (fid == 0)
		{
			fail(env, jobj, cls, "mdr_sun_cpu_t::sun_cpu_set: cannot find msc_sema");
			return false;
		}
		env->SetIntField(jcan, fid, (int) can.msc_sema);
		break;
	case mdr::MSC_NAMEI:
		fid = env->GetFieldID(ccls, "msc_namei", "I");
		if (fid == 0)
		{
			fail(env, jobj, cls, "mdr_sun_cpu_t::sun_cpu_set: cannot find msc_namei");
			return false;
		}
		env->SetIntField(jcan, fid, (int) can.msc_namei);
		break;
	case mdr::MSC_UFSIGET:
		fid = env->GetFieldID(ccls, "msc_ufsiget", "I");
		if (fid == 0)
		{
			fail(env, jobj, cls, "mdr_sun_cpu_t::sun_cpu_set: cannot find msc_ufsiget");
			return false;
		}
		env->SetIntField(jcan, fid, (int) can.msc_ufsiget);
		break;
	case mdr::MSC_UFSDIRBLK:
		fid = env->GetFieldID(ccls, "msc_ufsdirblk", "I");
		if (fid == 0)
		{
			fail(env, jobj, cls, "mdr_sun_cpu_t::sun_cpu_set: cannot find msc_ufsdirblk");
			return false;
		}
		env->SetIntField(jcan, fid, (int) can.msc_ufsdirblk);
		break;
	case mdr::MSC_UFSIPAGE:
		fid = env->GetFieldID(ccls, "msc_ufsipage", "I");
		if (fid == 0)
		{
			fail(env, jobj, cls, "mdr_sun_cpu_t::sun_cpu_set: cannot find msc_ufsipage");
			return false;
		}
		env->SetIntField(jcan, fid, (int) can.msc_ufsipage);
		break;
	case mdr::MSC_UFSINOPAGE:
		fid = env->GetFieldID(ccls, "msc_ufsinopage", "I");
		if (fid == 0)
		{
			fail(env, jobj, cls, "mdr_sun_cpu_t::sun_cpu_set: cannot find msc_ufsinopage");
			return false;
		}
		env->SetIntField(jcan, fid, (int) can.msc_ufsinopage);
		break;
	case mdr::MSC_INODEOVF:
		fid = env->GetFieldID(ccls, "msc_inodeovf", "I");
		if (fid == 0)
		{
			fail(env, jobj, cls, "mdr_sun_cpu_t::sun_cpu_set: cannot find msc_inodeovf");
			return false;
		}
		env->SetIntField(jcan, fid, (int) can.msc_inodeovf);
		break;
	case mdr::MSC_FILEOVF:
		fid = env->GetFieldID(ccls, "msc_fileovf", "I");
		if (fid == 0)
		{
			fail(env, jobj, cls, "mdr_sun_cpu_t::sun_cpu_set: cannot find msc_fileovf");
			return false;
		}
		env->SetIntField(jcan, fid, (int) can.msc_fileovf);
		break;
	case mdr::MSC_PROCOVF:
		fid = env->GetFieldID(ccls, "msc_procovf", "I");
		if (fid == 0)
		{
			fail(env, jobj, cls, "mdr_sun_cpu_t::sun_cpu_set: cannot find msc_procovf");
			return false;
		}
		env->SetIntField(jcan, fid, (int) can.msc_procovf);
		break;
	case mdr::MSC_INTRTHREAD:
		fid = env->GetFieldID(ccls, "msc_intrthread", "I");
		if (fid == 0)
		{
			fail(env, jobj, cls, "mdr_sun_cpu_t::sun_cpu_set: cannot find msc_intrthread");
			return false;
		}
		env->SetIntField(jcan, fid, (int) can.msc_intrthread);
		break;
	case mdr::MSC_INTRBLK:
		fid = env->GetFieldID(ccls, "msc_intrblk", "I");
		if (fid == 0)
		{
			fail(env, jobj, cls, "mdr_sun_cpu_t::sun_cpu_set: cannot find msc_intrblk");
			return false;
		}
		env->SetIntField(jcan, fid, (int) can.msc_intrblk);
		break;
	case mdr::MSC_IDLETHREAD:
		fid = env->GetFieldID(ccls, "msc_idlethread", "I");
		if (fid == 0)
		{
			fail(env, jobj, cls, "mdr_sun_cpu_t::sun_cpu_set: cannot find msc_idlethread");
			return false;
		}
		env->SetIntField(jcan, fid, (int) can.msc_idlethread);
		break;
	case mdr::MSC_INV_SWTCH:
		fid = env->GetFieldID(ccls, "msc_inv_swtch", "I");
		if (fid == 0)
		{
			fail(env, jobj, cls, "mdr_sun_cpu_t::sun_cpu_set: cannot find msc_inv_swtch");
			return false;
		}
		env->SetIntField(jcan, fid, (int) can.msc_inv_swtch);
		break;
	case mdr::MSC_NTHREADS:
		fid = env->GetFieldID(ccls, "msc_nthreads", "I");
		if (fid == 0)
		{
			fail(env, jobj, cls, "mdr_sun_cpu_t::sun_cpu_set: cannot find msc_nthreads");
			return false;
		}
		env->SetIntField(jcan, fid, (int) can.msc_nthreads);
		break;
	case mdr::MSC_CPUMIGRATE:
		fid = env->GetFieldID(ccls, "msc_cpumigrate", "I");
		if (fid == 0)
		{
			fail(env, jobj, cls, "mdr_sun_cpu_t::sun_cpu_set: cannot find msc_cpumigrate");
			return false;
		}
		env->SetIntField(jcan, fid, (int) can.msc_cpumigrate);
		break;
	case mdr::MSC_XCALLS:
		fid = env->GetFieldID(ccls, "msc_xcalls", "I");
		if (fid == 0)
		{
			fail(env, jobj, cls, "mdr_sun_cpu_t::sun_cpu_set: cannot find msc_xcalls");
			return false;
		}
		env->SetIntField(jcan, fid, (int) can.msc_xcalls);
		break;
	case mdr::MSC_MUTEX_ADENTERS:
		fid = env->GetFieldID(ccls, "msc_mutex_adenters", "I");
		if (fid == 0)
		{
			fail(env, jobj, cls, "mdr_sun_cpu_t::sun_cpu_set: cannot find msc_mutex_adenters");
			return false;
		}
		env->SetIntField(jcan, fid, (int) can.msc_mutex_adenters);
		break;
	case mdr::MSC_RW_RDFAILS:
		fid = env->GetFieldID(ccls, "msc_rw_rdfails", "I");
		if (fid == 0)
		{
			fail(env, jobj, cls, "mdr_sun_cpu_t::sun_cpu_set: cannot find msc_rw_rdfails");
			return false;
		}
		env->SetIntField(jcan, fid, (int) can.msc_rw_rdfails);
		break;
	case mdr::MSC_RW_WRFAILS:
		fid = env->GetFieldID(ccls, "msc_rw_wrfails", "I");
		if (fid == 0)
		{
			fail(env, jobj, cls, "mdr_sun_cpu_t::sun_cpu_set: cannot find msc_rw_wrfails");
			return false;
		}
		env->SetIntField(jcan, fid, (int) can.msc_rw_wrfails);
		break;
	case mdr::MSC_MODLOAD:
		fid = env->GetFieldID(ccls, "msc_modload", "I");
		if (fid == 0)
		{
			fail(env, jobj, cls, "mdr_sun_cpu_t::sun_cpu_set: cannot find msc_modload");
			return false;
		}
		env->SetIntField(jcan, fid, (int) can.msc_modload);
		break;
	case mdr::MSC_MODUNLOAD:
		fid = env->GetFieldID(ccls, "msc_modunload", "I");
		if (fid == 0)
		{
			fail(env, jobj, cls, "mdr_sun_cpu_t::sun_cpu_set: cannot find msc_modunload");
			return false;
		}
		env->SetIntField(jcan, fid, (int) can.msc_modunload);
		break;
	case mdr::MSC_BAWRITE:
		fid = env->GetFieldID(ccls, "msc_bawrite", "I");
		if (fid == 0)
		{
			fail(env, jobj, cls, "mdr_sun_cpu_t::sun_cpu_set: cannot find msc_bawrite");
			return false;
		}
		env->SetIntField(jcan, fid, (int) can.msc_bawrite);
		break;
	case mdr::MSC_IOWAIT:
		fid = env->GetFieldID(ccls, "msc_iowait", "I");
		if (fid == 0)
		{
			fail(env, jobj, cls, "mdr_sun_cpu_t::sun_cpu_set: cannot find msc_iowait");
			return false;
		}
		env->SetIntField(jcan, fid, (int) can.msc_iowait);
		break;
	case mdr::MSC_PGREC:
		fid = env->GetFieldID(ccls, "msc_pgrec", "I");
		if (fid == 0)
		{
			fail(env, jobj, cls, "mdr_sun_cpu_t::sun_cpu_set: cannot find msc_pgrec");
			return false;
		}
		env->SetIntField(jcan, fid, (int) can.msc_pgrec);
		break;
	case mdr::MSC_PGFREC:
		fid = env->GetFieldID(ccls, "msc_pgfrec", "I");
		if (fid == 0)
		{
			fail(env, jobj, cls, "mdr_sun_cpu_t::sun_cpu_set: cannot find msc_pgfrec");
			return false;
		}
		env->SetIntField(jcan, fid, (int) can.msc_pgfrec);
		break;
	case mdr::MSC_PGIN:
		fid = env->GetFieldID(ccls, "msc_pgin", "I");
		if (fid == 0)
		{
			fail(env, jobj, cls, "mdr_sun_cpu_t::sun_cpu_set: cannot find msc_pgin");
			return false;
		}
		env->SetIntField(jcan, fid, (int) can.msc_pgin);
		break;
	case mdr::MSC_PGPGIN:
		fid = env->GetFieldID(ccls, "msc_pgpgin", "I");
		if (fid == 0)
		{
			fail(env, jobj, cls, "mdr_sun_cpu_t::sun_cpu_set: cannot find msc_pgpgin");
			return false;
		}
		env->SetIntField(jcan, fid, (int) can.msc_pgpgin);
		break;
	case mdr::MSC_PGOUT:
		fid = env->GetFieldID(ccls, "msc_pgout", "I");
		if (fid == 0)
		{
			fail(env, jobj, cls, "mdr_sun_cpu_t::sun_cpu_set: cannot find msc_pgout");
			return false;
		}
		env->SetIntField(jcan, fid, (int) can.msc_pgout);
		break;
	case mdr::MSC_PGPGOUT:
		fid = env->GetFieldID(ccls, "msc_pgpgout", "I");
		if (fid == 0)
		{
			fail(env, jobj, cls, "mdr_sun_cpu_t::sun_cpu_set: cannot find msc_pgpgout");
			return false;
		}
		env->SetIntField(jcan, fid, (int) can.msc_pgpgout);
		break;
	case mdr::MSC_SWAPIN:
		fid = env->GetFieldID(ccls, "msc_swapin", "I");
		if (fid == 0)
		{
			fail(env, jobj, cls, "mdr_sun_cpu_t::sun_cpu_set: cannot find msc_swapin");
			return false;
		}
		env->SetIntField(jcan, fid, (int) can.msc_swapin);
		break;
	case mdr::MSC_PGSWAPIN:
		fid = env->GetFieldID(ccls, "msc_pgswapin", "I");
		if (fid == 0)
		{
			fail(env, jobj, cls, "mdr_sun_cpu_t::sun_cpu_set: cannot find msc_pgswapin");
			return false;
		}
		env->SetIntField(jcan, fid, (int) can.msc_pgswapin);
		break;
	case mdr::MSC_SWAPOUT:
		fid = env->GetFieldID(ccls, "msc_swapout", "I");
		if (fid == 0)
		{
			fail(env, jobj, cls, "mdr_sun_cpu_t::sun_cpu_set: cannot find msc_swapout");
			return false;
		}
		env->SetIntField(jcan, fid, (int) can.msc_swapout);
		break;
	case mdr::MSC_PGSWAPOUT:
		fid = env->GetFieldID(ccls, "msc_pgswapout", "I");
		if (fid == 0)
		{
			fail(env, jobj, cls, "mdr_sun_cpu_t::sun_cpu_set: cannot find msc_pgswapout");
			return false;
		}
		env->SetIntField(jcan, fid, (int) can.msc_pgswapout);
		break;
	case mdr::MSC_ZFOD:
		fid = env->GetFieldID(ccls, "msc_zfod", "I");
		if (fid == 0)
		{
			fail(env, jobj, cls, "mdr_sun_cpu_t::sun_cpu_set: cannot find msc_zfod");
			return false;
		}
		env->SetIntField(jcan, fid, (int) can.msc_zfod);
		break;
	case mdr::MSC_DFREE:
		fid = env->GetFieldID(ccls, "msc_dfree", "I");
		if (fid == 0)
		{
			fail(env, jobj, cls, "mdr_sun_cpu_t::sun_cpu_set: cannot find msc_dfree");
			return false;
		}
		env->SetIntField(jcan, fid, (int) can.msc_dfree);
		break;
	case mdr::MSC_SCAN:
		fid = env->GetFieldID(ccls, "msc_scan", "I");
		if (fid == 0)
		{
			fail(env, jobj, cls, "mdr_sun_cpu_t::sun_cpu_set: cannot find msc_scan");
			return false;
		}
		env->SetIntField(jcan, fid, (int) can.msc_scan);
		break;
	case mdr::MSC_REV:
		fid = env->GetFieldID(ccls, "msc_rev", "I");
		if (fid == 0)
		{
			fail(env, jobj, cls, "mdr_sun_cpu_t::sun_cpu_set: cannot find msc_rev");
			return false;
		}
		env->SetIntField(jcan, fid, (int) can.msc_rev);
		break;
	case mdr::MSC_HAT_FAULT:
		fid = env->GetFieldID(ccls, "msc_hat_fault", "I");
		if (fid == 0)
		{
			fail(env, jobj, cls, "mdr_sun_cpu_t::sun_cpu_set: cannot find msc_hat_fault");
			return false;
		}
		env->SetIntField(jcan, fid, (int) can.msc_hat_fault);
		break;
	case mdr::MSC_AS_FAULT:
		fid = env->GetFieldID(ccls, "msc_as_fault", "I");
		if (fid == 0)
		{
			fail(env, jobj, cls, "mdr_sun_cpu_t::sun_cpu_set: cannot find msc_as_fault");
			return false;
		}
		env->SetIntField(jcan, fid, (int) can.msc_as_fault);
		break;
	case mdr::MSC_MAJ_FAULT:
		fid = env->GetFieldID(ccls, "msc_maj_fault", "I");
		if (fid == 0)
		{
			fail(env, jobj, cls, "mdr_sun_cpu_t::sun_cpu_set: cannot find msc_maj_fault");
			return false;
		}
		env->SetIntField(jcan, fid, (int) can.msc_maj_fault);
		break;
	case mdr::MSC_COW_FAULT:
		fid = env->GetFieldID(ccls, "msc_cow_fault", "I");
		if (fid == 0)
		{
			fail(env, jobj, cls, "mdr_sun_cpu_t::sun_cpu_set: cannot find msc_cow_fault");
			return false;
		}
		env->SetIntField(jcan, fid, (int) can.msc_cow_fault);
		break;
	case mdr::MSC_PROT_FAULT:
		fid = env->GetFieldID(ccls, "msc_prot_fault", "I");
		if (fid == 0)
		{
			fail(env, jobj, cls, "mdr_sun_cpu_t::sun_cpu_set: cannot find msc_prot_fault");
			return false;
		}
		env->SetIntField(jcan, fid, (int) can.msc_prot_fault);
		break;
	case mdr::MSC_SOFTLOCK:
		fid = env->GetFieldID(ccls, "msc_softlock", "I");
		if (fid == 0)
		{
			fail(env, jobj, cls, "mdr_sun_cpu_t::sun_cpu_set: cannot find msc_softlock");
			return false;
		}
		env->SetIntField(jcan, fid, (int) can.msc_softlock);
		break;
	case mdr::MSC_KERNEL_ASFLT:
		fid = env->GetFieldID(ccls, "msc_kernel_asflt", "I");
		if (fid == 0)
		{
			fail(env, jobj, cls, "mdr_sun_cpu_t::sun_cpu_set: cannot find msc_kernel_asflt");
			return false;
		}
		env->SetIntField(jcan, fid, (int) can.msc_kernel_asflt);
		break;
	case mdr::MSC_PGRRUN:
		fid = env->GetFieldID(ccls, "msc_pgrrun", "I");
		if (fid == 0)
		{
			fail(env, jobj, cls, "mdr_sun_cpu_t::sun_cpu_set: cannot find msc_pgrrun");
			return false;
		}
		env->SetIntField(jcan, fid, (int) can.msc_pgrrun);
		break;
	case mdr::MSC_EXECPGIN:
		fid = env->GetFieldID(ccls, "msc_execpgin", "I");
		if (fid == 0)
		{
			fail(env, jobj, cls, "mdr_sun_cpu_t::sun_cpu_set: cannot find msc_execpgin");
			return false;
		}
		env->SetIntField(jcan, fid, (int) can.msc_execpgin);
		break;
	case mdr::MSC_EXECPGOUT:
		fid = env->GetFieldID(ccls, "msc_execpgout", "I");
		if (fid == 0)
		{
			fail(env, jobj, cls, "mdr_sun_cpu_t::sun_cpu_set: cannot find msc_execpgout");
			return false;
		}
		env->SetIntField(jcan, fid, (int) can.msc_execpgout);
		break;
	case mdr::MSC_EXECFREE:
		fid = env->GetFieldID(ccls, "msc_execfree", "I");
		if (fid == 0)
		{
			fail(env, jobj, cls, "mdr_sun_cpu_t::sun_cpu_set: cannot find msc_execfree");
			return false;
		}
		env->SetIntField(jcan, fid, (int) can.msc_execfree);
		break;
	case mdr::MSC_ANONPGIN:
		fid = env->GetFieldID(ccls, "msc_anonpgin", "I");
		if (fid == 0)
		{
			fail(env, jobj, cls, "mdr_sun_cpu_t::sun_cpu_set: cannot find msc_anonpgin");
			return false;
		}
		env->SetIntField(jcan, fid, (int) can.msc_anonpgin);
		break;
	case mdr::MSC_ANONPGOUT:
		fid = env->GetFieldID(ccls, "msc_anonpgout", "I");
		if (fid == 0)
		{
			fail(env, jobj, cls, "mdr_sun_cpu_t::sun_cpu_set: cannot find msc_anonpgout");
			return false;
		}
		env->SetIntField(jcan, fid, (int) can.msc_anonpgout);
		break;
	case mdr::MSC_ANONFREE:
		fid = env->GetFieldID(ccls, "msc_anonfree", "I");
		if (fid == 0)
		{
			fail(env, jobj, cls, "mdr_sun_cpu_t::sun_cpu_set: cannot find msc_anonfree");
			return false;
		}
		env->SetIntField(jcan, fid, (int) can.msc_anonfree);
		break;
	case mdr::MSC_FSPGIN:
		fid = env->GetFieldID(ccls, "msc_fspgin", "I");
		if (fid == 0)
		{
			fail(env, jobj, cls, "mdr_sun_cpu_t::sun_cpu_set: cannot find msc_fspgin");
			return false;
		}
		env->SetIntField(jcan, fid, (int) can.msc_fspgin);
		break;
	case mdr::MSC_FSPGOUT:
		fid = env->GetFieldID(ccls, "msc_fspgout", "I");
		if (fid == 0)
		{
			fail(env, jobj, cls, "mdr_sun_cpu_t::sun_cpu_set: cannot find msc_fspgout");
			return false;
		}
		env->SetIntField(jcan, fid, (int) can.msc_fspgout);
		break;
	case mdr::MSC_FSFREE:
		fid = env->GetFieldID(ccls, "msc_fsfree", "I");
		if (fid == 0)
		{
			fail(env, jobj, cls, "mdr_sun_cpu_t::sun_cpu_set: cannot find msc_fsfree");
			return false;
		}
		env->SetIntField(jcan, fid, (int) can.msc_fsfree);
		break;
    }

    return true;
}

static bool sun_cpu_set
    (JNIEnv *env, jobject jobj, jobject jcan, mdr::msc_can_t &can)
{
    return(
		sun_cpu_set(env, jobj, jcan, can, mdr::MSC_CPU_INSTANCE) &&
		sun_cpu_set(env, jobj, jcan, can, mdr::MSC_BREAD) &&
		sun_cpu_set(env, jobj, jcan, can, mdr::MSC_BWRITE) &&
		sun_cpu_set(env, jobj, jcan, can, mdr::MSC_LREAD) &&
		sun_cpu_set(env, jobj, jcan, can, mdr::MSC_LWRITE) &&
		sun_cpu_set(env, jobj, jcan, can, mdr::MSC_PHREAD) &&
		sun_cpu_set(env, jobj, jcan, can, mdr::MSC_PHWRITE) &&
		sun_cpu_set(env, jobj, jcan, can, mdr::MSC_PSWITCH) &&
		sun_cpu_set(env, jobj, jcan, can, mdr::MSC_TRAP) &&
		sun_cpu_set(env, jobj, jcan, can, mdr::MSC_INTR) &&
		sun_cpu_set(env, jobj, jcan, can, mdr::MSC_SYSCALL) &&
		sun_cpu_set(env, jobj, jcan, can, mdr::MSC_SYSREAD) &&
		sun_cpu_set(env, jobj, jcan, can, mdr::MSC_SYSWRITE) &&
		sun_cpu_set(env, jobj, jcan, can, mdr::MSC_SYSFORK) &&
		sun_cpu_set(env, jobj, jcan, can, mdr::MSC_SYSVFORK) &&
		sun_cpu_set(env, jobj, jcan, can, mdr::MSC_SYSEXEC) &&
		sun_cpu_set(env, jobj, jcan, can, mdr::MSC_READCH) &&
		sun_cpu_set(env, jobj, jcan, can, mdr::MSC_WRITECH) &&
		sun_cpu_set(env, jobj, jcan, can, mdr::MSC_RAWCH) &&
		sun_cpu_set(env, jobj, jcan, can, mdr::MSC_CANCH) &&
		sun_cpu_set(env, jobj, jcan, can, mdr::MSC_OUTCH) &&
		sun_cpu_set(env, jobj, jcan, can, mdr::MSC_MSG) &&
		sun_cpu_set(env, jobj, jcan, can, mdr::MSC_SEMA) &&
		sun_cpu_set(env, jobj, jcan, can, mdr::MSC_NAMEI) &&
		sun_cpu_set(env, jobj, jcan, can, mdr::MSC_UFSIGET) &&
		sun_cpu_set(env, jobj, jcan, can, mdr::MSC_UFSDIRBLK) &&
		sun_cpu_set(env, jobj, jcan, can, mdr::MSC_UFSIPAGE) &&
		sun_cpu_set(env, jobj, jcan, can, mdr::MSC_UFSINOPAGE) &&
		sun_cpu_set(env, jobj, jcan, can, mdr::MSC_INODEOVF) &&
		sun_cpu_set(env, jobj, jcan, can, mdr::MSC_FILEOVF) &&
		sun_cpu_set(env, jobj, jcan, can, mdr::MSC_PROCOVF) &&
		sun_cpu_set(env, jobj, jcan, can, mdr::MSC_INTRTHREAD) &&
		sun_cpu_set(env, jobj, jcan, can, mdr::MSC_INTRBLK) &&
		sun_cpu_set(env, jobj, jcan, can, mdr::MSC_IDLETHREAD) &&
		sun_cpu_set(env, jobj, jcan, can, mdr::MSC_INV_SWTCH) &&
		sun_cpu_set(env, jobj, jcan, can, mdr::MSC_NTHREADS) &&
		sun_cpu_set(env, jobj, jcan, can, mdr::MSC_CPUMIGRATE) &&
		sun_cpu_set(env, jobj, jcan, can, mdr::MSC_XCALLS) &&
		sun_cpu_set(env, jobj, jcan, can, mdr::MSC_MUTEX_ADENTERS) &&
		sun_cpu_set(env, jobj, jcan, can, mdr::MSC_RW_RDFAILS) &&
		sun_cpu_set(env, jobj, jcan, can, mdr::MSC_RW_WRFAILS) &&
		sun_cpu_set(env, jobj, jcan, can, mdr::MSC_MODLOAD) &&
		sun_cpu_set(env, jobj, jcan, can, mdr::MSC_MODUNLOAD) &&
		sun_cpu_set(env, jobj, jcan, can, mdr::MSC_BAWRITE) &&
		sun_cpu_set(env, jobj, jcan, can, mdr::MSC_IOWAIT) &&
		sun_cpu_set(env, jobj, jcan, can, mdr::MSC_PGREC) &&
		sun_cpu_set(env, jobj, jcan, can, mdr::MSC_PGFREC) &&
		sun_cpu_set(env, jobj, jcan, can, mdr::MSC_PGIN) &&
		sun_cpu_set(env, jobj, jcan, can, mdr::MSC_PGPGIN) &&
		sun_cpu_set(env, jobj, jcan, can, mdr::MSC_PGOUT) &&
		sun_cpu_set(env, jobj, jcan, can, mdr::MSC_PGPGOUT) &&
		sun_cpu_set(env, jobj, jcan, can, mdr::MSC_SWAPIN) &&
		sun_cpu_set(env, jobj, jcan, can, mdr::MSC_PGSWAPIN) &&
		sun_cpu_set(env, jobj, jcan, can, mdr::MSC_SWAPOUT) &&
		sun_cpu_set(env, jobj, jcan, can, mdr::MSC_PGSWAPOUT) &&
		sun_cpu_set(env, jobj, jcan, can, mdr::MSC_ZFOD) &&
		sun_cpu_set(env, jobj, jcan, can, mdr::MSC_DFREE) &&
		sun_cpu_set(env, jobj, jcan, can, mdr::MSC_SCAN) &&
		sun_cpu_set(env, jobj, jcan, can, mdr::MSC_REV) &&
		sun_cpu_set(env, jobj, jcan, can, mdr::MSC_HAT_FAULT) &&
		sun_cpu_set(env, jobj, jcan, can, mdr::MSC_AS_FAULT) &&
		sun_cpu_set(env, jobj, jcan, can, mdr::MSC_MAJ_FAULT) &&
		sun_cpu_set(env, jobj, jcan, can, mdr::MSC_COW_FAULT) &&
		sun_cpu_set(env, jobj, jcan, can, mdr::MSC_PROT_FAULT) &&
		sun_cpu_set(env, jobj, jcan, can, mdr::MSC_SOFTLOCK) &&
		sun_cpu_set(env, jobj, jcan, can, mdr::MSC_KERNEL_ASFLT) &&
		sun_cpu_set(env, jobj, jcan, can, mdr::MSC_PGRRUN) &&
		sun_cpu_set(env, jobj, jcan, can, mdr::MSC_EXECPGIN) &&
		sun_cpu_set(env, jobj, jcan, can, mdr::MSC_EXECPGOUT) &&
		sun_cpu_set(env, jobj, jcan, can, mdr::MSC_EXECFREE) &&
		sun_cpu_set(env, jobj, jcan, can, mdr::MSC_ANONPGIN) &&
		sun_cpu_set(env, jobj, jcan, can, mdr::MSC_ANONPGOUT) &&
		sun_cpu_set(env, jobj, jcan, can, mdr::MSC_ANONFREE) &&
		sun_cpu_set(env, jobj, jcan, can, mdr::MSC_FSPGIN) &&
		sun_cpu_set(env, jobj, jcan, can, mdr::MSC_FSPGOUT) &&
		sun_cpu_set(env, jobj, jcan, can, mdr::MSC_FSFREE));
}

extern "C" JNIEXPORT jint JNICALL
Java_com_capmet_plugins_CaptiveCollector_Solaris_mdr_1sun_1cpu_1t_mdr_1size
	(JNIEnv *env, jobject jobj)
{
    jclass cls = env->GetObjectClass(jobj);
    jfieldID fid;
    mdr::mdr_sun_cpu_t *sun_cpu;

    // find handle
    fid = env->GetFieldID(cls, "handle", "J");
    if (fid == 0)
    {
        fail(env, jobj, cls, "mdr_sun_cpu_t::mdr_size: cannot find handle");
        return JNI_FALSE;
    }

    // get the sun_cpu pointer
    sun_cpu = (mdr::mdr_sun_cpu_t *) _POINTER_CAST_ env->GetLongField(jobj, fid);
    if (sun_cpu == 0)
    {
        fail(env, jobj, cls, "mdr_sun_cpu_t::mdr_size: null mdr_sun_cpu_t handle");
        return JNI_FALSE;
    }

    return sun_cpu->size();
}

extern "C" JNIEXPORT jobjectArray JNICALL
Java_com_capmet_plugins_CaptiveCollector_Solaris_mdr_1sun_1cpu_1t_mdr_1keyset
	(JNIEnv *env, jobject jobj)
{
    jclass cls = env->GetObjectClass(jobj);
    jfieldID fid;
    mdr::mdr_sun_cpu_t *sun_cpu;
    jobjectArray jarray;

    // find handle
    fid = env->GetFieldID(cls, "handle", "J");
    if (fid == 0)
    {
        fail(env, jobj, cls, "mdr_sun_cpu_t::mdr_keyset: cannot find handle");
        return JNI_FALSE;
    }

    // get the sun_cpu pointer
    sun_cpu = (mdr::mdr_sun_cpu_t *) _POINTER_CAST_ env->GetLongField(jobj, fid);
    if (sun_cpu == 0)
    {
        fail(env, jobj, cls, "mdr_sun_cpu_t::mdr_keyset: null mdr_sun_cpu_t handle");
        return JNI_FALSE;
    }

    // retrieve the keys
    mdr::mdr_keyset_t keyset;
    mdr::mdr_key_t kn;
    int i;

    try
    {
        sun_cpu->mdr_keyset(keyset);
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
Java_com_capmet_plugins_CaptiveCollector_Solaris_mdr_1sun_1cpu_1t_mdr_1get
    (JNIEnv *env, jobject jobj, jstring jkey, jobject jcan)
{
    jclass cls = env->GetObjectClass(jobj);
    jfieldID fid;
    mdr::mdr_sun_cpu_t *sun_cpu;

    // find handle
    fid = env->GetFieldID(cls, "handle", "J");
    if (fid == 0)
    {
        fail(env, jobj, cls, "mdr_sun_cpu_t::mdr_get: cannot find handle");
        return JNI_FALSE;
    }

    // get the sun_cpu pointer
    sun_cpu = (mdr::mdr_sun_cpu_t *) _POINTER_CAST_ env->GetLongField(jobj, fid);
    if (sun_cpu == 0)
    {
        fail(env, jobj, cls, "mdr_sun_cpu_t::mdr_get: null mdr_sun_cpu_t handle");
        return JNI_FALSE;
    }

    // retrive the key and perform lookup
    const char *key;
    mdr::msc_node_t node;

    try
    {
        key = env->GetStringUTFChars(jkey, 0);
        node = sun_cpu->find(key);
        env->ReleaseStringUTFChars(jkey, key);
    }
    catch(std::exception &e)
    {
        fail(env, jobj, cls, e.what());
        return JNI_FALSE;
    }

    // not there
    if (node == sun_cpu->end())
        return JNI_FALSE;

    // now assign values from can to can
    return (sun_cpu_set(env, jobj, jcan, node->second) ? JNI_TRUE : JNI_FALSE);
}

extern "C" JNIEXPORT jboolean JNICALL
Java_com_capmet_plugins_CaptiveCollector_Solaris_mdr_1sun_1cpu_1t_mdr_1get_1field
    (JNIEnv *env, jobject jobj, jstring jkey, jobject jcan, jint field_number)
{
    jclass cls = env->GetObjectClass(jobj);
    jfieldID fid;
    mdr::mdr_sun_cpu_t *sun_cpu;

    // find handle
    fid = env->GetFieldID(cls, "handle", "J");
    if (fid == 0)
    {
        fail(env, jobj, cls, "mdr_sun_cpu_t::mdr_get_field: cannot find handle");
        return JNI_FALSE;
    }

    // get the sun_cpu pointer
    sun_cpu = (mdr::mdr_sun_cpu_t *) _POINTER_CAST_ env->GetLongField(jobj, fid);
    if (sun_cpu == 0)
    {
        fail(env, jobj, cls, "mdr_sun_cpu_t::mdr_get_field: null mdr_sun_cpu_t handle");
        return JNI_FALSE;
    }

    // retrive the key and perform lookup
    const char *key;
    mdr::msc_node_t node;

    try
    {
        key = env->GetStringUTFChars(jkey, 0);
        node = sun_cpu->find(key);
        env->ReleaseStringUTFChars(jkey, key);
    }
    catch(std::exception &e)
    {
        fail(env, jobj, cls, e.what());
        return JNI_FALSE;
    }

    // not there
    if (node == sun_cpu->end())
        return JNI_FALSE;

    // now assign values from can to can
    return (sun_cpu_set(env, jobj, jcan, node->second, field_number) ? JNI_TRUE : JNI_FALSE);
}
