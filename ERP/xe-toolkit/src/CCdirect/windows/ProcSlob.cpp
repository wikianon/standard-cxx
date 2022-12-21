/****************************************************************************
**
** Copyright (c) 2006-2007 Captive Metrics Software Corporation.
**                    All rights reserved.
**
** This file is part of the CCdirect library of Captivity.
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
#include "WinProcess.h"
#include <jni.h>

static const int PN_START_TIME             = 0;
static const int PN_PROCESS_ID             = 1;
static const int PN_PARENT_PROCESS_ID      = 2;
static const int PN_USER_NAME              = 3;
static const int PN_COMMAND_NAME           = 4;
static const int PN_COMMAND_LINE           = 5;
static const int PN_TOTAL_CPU_SECONDS      = 6;
static const int PN_USER_CPU_SECONDS       = 7;
static const int PN_KERNEL_CPU_SECONDS     = 8;
static const int PN_PERCENT_USER_TIME      = 9;
static const int PN_PERCENT_KERNEL_TIME    = 10;
static const int PN_TOTAL_SIZE_KB          = 11;
static const int PN_RESIDENT_SIZE_KB       = 12;
static const int PN_TOTAL_IO_KB            = 13;
static const int PN_PERCENT_CPU_USE        = 14;
static const int PN_PERCENT_PHYSMEM_USE    = 15;
static const int PN_PAGE_FAULTS            = 16;
static const int PN_PROCESS_STATE          = 17;
static const int PN_GLOBAL_PRIORITY        = 18;
static const int PN_NUMBER_OF_THREADS      = 19;
static const int PN_TOTAL_SIZE_DELTA_KB    = 20;
static const int PN_RESIDENT_SIZE_DELTA_KB = 21;
static const int PN_SYSTEM_CALLS           = 22;
static const int PN_CONTEXT_SWITCHES       = 23;
static const int PN_FIELD_COUNT            = 24;

static void fail(JNIEnv *env, jobject jobj, jclass cls, const char *msg)
{
    jfieldID reason;
    jstring jstr;

    reason = env->GetFieldID(cls, "reason", "Ljava/lang/String;");
    if (reason == 0)
    {
        std::cerr << "ProcSlob::fail: cannot fail" << std::endl;
        return;
    }
    jstr = env->NewStringUTF(msg);
    env->SetObjectField(jobj, reason, jstr);
}

extern "C" JNIEXPORT jboolean JNICALL
Java_com_capmet_Captivity_CaptiveCollector_ProcSlob_procSlobConstructor
    (JNIEnv *env, jobject jobj)
{
    jclass cls = env->GetObjectClass(jobj);
    jfieldID fid;
    WinProcess *ps;

    fid = env->GetFieldID(cls, "handle", "J");
    if (fid == 0)
    {
        fail(env, jobj, cls, "ProcSlob::ProcSlob: cannot find handle");
        return JNI_FALSE;
    }
    try
    {
        ps = new WinProcess();
    }
    catch(std::exception &e)
    {
        fail(env, jobj, cls, e.what());
        return JNI_FALSE;
    }
    env->SetLongField(jobj, fid, (jlong) ps);

    jlong supportFlags = 0;
    int i;

    for(i=0; i<PN_FIELD_COUNT; i++)
        if (i != PN_SYSTEM_CALLS)
            supportFlags |= (1LL << i);

    fid = env->GetFieldID(cls, "supportFlags", "J");
    if (fid == 0)
    {
        fail(env, jobj, cls, "ProcSlob::ProcSlob: cannot find supportFlags");
        return JNI_FALSE;
    }
    env->SetLongField(jobj, fid, supportFlags);

    return JNI_TRUE;
}

extern "C" JNIEXPORT jboolean JNICALL
Java_com_capmet_Captivity_CaptiveCollector_ProcSlob_procSlobRefreshRates
    (JNIEnv *env, jobject jobj, jobject jtree)
{
    jclass cls = env->GetObjectClass(jobj);
    jclass tcls = env->GetObjectClass(jtree);
    jclass ccls;
    jfieldID fid;
    jstring jstr;
    WinProcess *ps;

    // get handle
    fid = env->GetFieldID(cls, "handle", "J");
    if (fid == 0)
    {
        fail(env, jobj, cls, "ProcSlob::refreshRates: cannot find handle");
        return JNI_FALSE;
    }

    // get ps
    ps = (WinProcess *) env->GetLongField(jobj, fid);
    if (ps == 0)
    {
        fail(env, jobj, cls, "ProcSlob::refreshRates: null ManagedProcInfo handle");
        return JNI_FALSE;
    }

    // refresh
    try
    {
        ps->refreshRates();
    }
    catch(std::exception &e)
    {
        fail(env, jobj, cls, e.what());
        return JNI_FALSE;
    }

    // find public class PN
    ccls = env->FindClass("D/PN");
    if (ccls == 0)
    {
        fail(env, jobj, cls, "ProcSlob::refreshRates: cannot find D/PN");
        return JNI_FALSE;
    }

    // init a new instance
    jmethodID mid = env->GetMethodID(ccls, "<init>", "()V");
    if (mid == 0)
    {
        fail(env, jobj, cls, "ProcSlob::refreshRates: cannot find PN::PN");
        return JNI_FALSE;
    }

    jmethodID put = env->GetMethodID(tcls, "put",
        "(Ljava/lang/Object;Ljava/lang/Object;)Ljava/lang/Object;");
    if (put == 0)
    {
        fail(env, jobj, cls, "ProcSlob::refreshRates: cannot find put");
        return JNI_FALSE;
    }

    // for each process in ps
    for(ProcMapNode node=ps->begin(); node != ps->end(); node++)
    {
        // cut to the chase
        ProcInfo *psp = &node->second;

        // allocate a new java node
        jobject pn = env->NewObject(ccls, mid, 0);

        // get the object class of same
        ccls = env->GetObjectClass(pn);

        // assign members

        //
        // startTime
        //
        fid = env->GetFieldID(ccls, "startTime", "J");
        if (fid == 0)
        {
            fail(env, jobj, cls, "ProcSlob::refreshRates: cannot find startTime");
            return JNI_FALSE;
        }
        env->SetLongField(pn, fid, (jlong) psp->startTime);

        //
        // processID
        //
        fid = env->GetFieldID(ccls, "processID", "J");
        if (fid == 0)
        {
            fail(env, jobj, cls, "ProcSlob::refreshRates: cannot find processID");
            return JNI_FALSE;
        }
        env->SetLongField(pn, fid, (jlong) psp->processID);

        //
        // parentProcessID
        //
        fid = env->GetFieldID(ccls, "parentProcessID", "J");
        if (fid == 0)
        {
            fail(env, jobj, cls, "ProcSlob::refreshRates: cannot find parentProcessID");
            return JNI_FALSE;
        }
        env->SetLongField(pn, fid, (jlong) psp->parentProcessID);

        //
        // userName
        //
        fid = env->GetFieldID(ccls, "userName", "Ljava/lang/String;");
        if (fid == 0)
        {
            fail(env, jobj, cls, "ProcSlob::refreshRates: cannot find userName");
            return JNI_FALSE;
        }
        jstr = env->NewStringUTF(psp->userName.c_str());
        env->SetObjectField(pn, fid, jstr);

        //
        // commandName
        //
        fid = env->GetFieldID(ccls, "commandName", "Ljava/lang/String;");
        if (fid == 0)
        {
            fail(env, jobj, cls, "ProcSlob::refreshRates: cannot find commandName");
            return JNI_FALSE;
        }
        jstr = env->NewStringUTF(psp->commandName.c_str());
        env->SetObjectField(pn, fid, jstr);

        //
        // commandLine
        //
        fid = env->GetFieldID(ccls, "commandLine", "Ljava/lang/String;");
        if (fid == 0)
        {
            fail(env, jobj, cls, "ProcSlob::refreshRates: cannot find commandLine");
            return JNI_FALSE;
        }
        jstr = env->NewStringUTF(psp->commandLine.c_str());
        env->SetObjectField(pn, fid, jstr);

        //
        // totalCPUSeconds
        //
        fid = env->GetFieldID(ccls, "totalCPUSeconds", "D");
        if (fid == 0)
        {
            fail(env, jobj, cls, "ProcSlob::refreshRates: cannot find totalCPUSeconds");
            return JNI_FALSE;
        }
        env->SetDoubleField(pn, fid, psp->totalCPUSeconds);

        //
        // userCPUSeconds
        //
        fid = env->GetFieldID(ccls, "userCPUSeconds", "D");
        if (fid == 0)
        {
            fail(env, jobj, cls, "ProcSlob::refreshRates: cannot find userCPUSeconds");
            return JNI_FALSE;
        }
        env->SetDoubleField(pn, fid, psp->userCPUSeconds);

        //
        // kernelCPUSeconds
        //
        fid = env->GetFieldID(ccls, "kernelCPUSeconds", "D");
        if (fid == 0)
        {
            fail(env, jobj, cls, "ProcSlob::refreshRates: cannot find kernelCPUSeconds");
            return JNI_FALSE;
        }
        env->SetDoubleField(pn, fid, psp->kernelCPUSeconds);

        //
        // percentUserTime
        //
        fid = env->GetFieldID(ccls, "percentUserTime", "D");
        if (fid == 0)
        {
            fail(env, jobj, cls, "ProcSlob::refreshRates: cannot find percentUserTime");
            return JNI_FALSE;
        }
        env->SetDoubleField(pn, fid, psp->percentUserTime);

        //
        // percentKernelTime
        //
        fid = env->GetFieldID(ccls, "percentKernelTime", "D");
        if (fid == 0)
        {
            fail(env, jobj, cls, "ProcSlob::refreshRates: cannot find percentKernelTime");
            return JNI_FALSE;
        }
        env->SetDoubleField(pn, fid, psp->percentKernelTime);

        //
        // totalSizeKB
        //
        fid = env->GetFieldID(ccls, "totalSizeKB", "I");
        if (fid == 0)
        {
            fail(env, jobj, cls, "ProcSlob::refreshRates: cannot find totalSizeKB");
            return JNI_FALSE;
        }
        env->SetIntField(pn, fid, psp->totalSizeKB);

        //
        // residentSizeKB
        //
        fid = env->GetFieldID(ccls, "residentSizeKB", "I");
        if (fid == 0)
        {
            fail(env, jobj, cls, "ProcSlob::refreshRates: cannot find residentSizeKB");
            return JNI_FALSE;
        }
        env->SetIntField(pn, fid, psp->residentSizeKB);

        //
        // totalIOKB
        //
        fid = env->GetFieldID(ccls, "totalIOKB", "I");
        if (fid == 0)
        {
            fail(env, jobj, cls, "ProcSlob::refreshRates: cannot find totalIOKB");
            return JNI_FALSE;
        }
        env->SetIntField(pn, fid, psp->totalIOKB);

        //
        // percentCPUUse
        //
        fid = env->GetFieldID(ccls, "percentCPUUse", "D");
        if (fid == 0)
        {
            fail(env, jobj, cls, "ProcSlob::refreshRates: cannot find percentCPUUse");
            return JNI_FALSE;
        }
        env->SetDoubleField(pn, fid, psp->percentCPUUse);

        //
        // percentPhysmemUse
        //
        fid = env->GetFieldID(ccls, "percentPhysmemUse", "D");
        if (fid == 0)
        {
            fail(env, jobj, cls, "ProcSlob::refreshRates: cannot find percentPhysmemUse");
            return JNI_FALSE;
        }
        env->SetDoubleField(pn, fid, psp->percentPhysmemUse);

        //
        // pageFaults
        //
        fid = env->GetFieldID(ccls, "pageFaults", "I");
        if (fid == 0)
        {
            fail(env, jobj, cls, "ProcSlob::refreshRates: cannot find pageFaults");
            return JNI_FALSE;
        }
        env->SetIntField(pn, fid, psp->pageFaults);

        //
        // processState
        //
        fid = env->GetFieldID(ccls, "processState", "Ljava/lang/String;");
        if (fid == 0)
        {
            fail(env, jobj, cls, "ProcSlob::refreshRates: cannot find processState");
            return JNI_FALSE;
        }

        const char *state_name;

        switch(psp->processState) {
        case MPS_RUNNABLE:
            state_name = "Runnable";
            break;
        case MPS_SLEEPING:
            state_name = "Sleeping";
            break;
        case MPS_STOPPED:
            state_name = "Stopped";
            break;
        case MPS_BLOCKED:
            state_name = "Blocked";
            break;
        case MPS_ZOMBIE:
            state_name = "Unreaped";
            break;
        case MPS_SPECIFIC:
            state_name = "Specific";
            break;
        default:
            state_name = "Unknown";
            break;
        }
        jstr = env->NewStringUTF(state_name);
        env->SetObjectField(pn, fid, jstr);

        //
        // globalPriority
        //
        fid = env->GetFieldID(ccls, "globalPriority", "I");
        if (fid == 0)
        {
            fail(env, jobj, cls, "ProcSlob::refreshRates: cannot find globalPriority");
            return JNI_FALSE;
        }
        env->SetIntField(pn, fid, psp->globalPriority);

        //
        // numberOfThreads
        //
        fid = env->GetFieldID(ccls, "numberOfThreads", "I");
        if (fid == 0)
        {
            fail(env, jobj, cls, "ProcSlob::refreshRates: cannot find numberOfThreads");
            return JNI_FALSE;
        }
        env->SetIntField(pn, fid, psp->numberOfThreads);

        //
        // totalSizeDeltaKB
        //
        fid = env->GetFieldID(ccls, "totalSizeDeltaKB", "I");
        if (fid == 0)
        {
            fail(env, jobj, cls, "ProcSlob::refreshRates: cannot find totalSizeDeltaKB");
            return JNI_FALSE;
        }
        env->SetIntField(pn, fid, psp->totalSizeDeltaKB);

        //
        // residentSizeDeltaKB
        //
        fid = env->GetFieldID(ccls, "residentSizeDeltaKB", "I");
        if (fid == 0)
        {
            fail(env, jobj, cls, "ProcSlob::refreshRates: cannot find residentSizeDeltaKB");
            return JNI_FALSE;
        }
        env->SetIntField(pn, fid, psp->residentSizeDeltaKB);

        //
        // contextSwitches
        //
        fid = env->GetFieldID(ccls, "contextSwitches", "I");
        if (fid == 0)
        {
            fail(env, jobj, cls, "ProcSlob::refreshRates: cannot find contextSwitches");
            return JNI_FALSE;
        }
        env->SetIntField(pn, fid, psp->contextSwitches);

        //
        // isSystemProcess
        //
        fid = env->GetFieldID(ccls, "isSystemProcess", "Z");
        if (fid == 0)
        {
            fail(env, jobj, cls, "ProcSlob::refreshRates: cannot find isSystemProcess");
            return JNI_FALSE;
        }
        env->SetBooleanField(pn, fid, psp->isSystemProcess);

        // make the process-name-key
        char key[BUFSIZ];
        _snprintf_s(key, sizeof key, sizeof key, "%s[%d]", psp->commandName.c_str(), psp->processID);
        jstr = env->NewStringUTF(key);

        // call TreeMap::put(key, value)
        env->CallObjectMethod(jtree, put, jstr, pn);
    }

    return JNI_TRUE;
}

extern "C" JNIEXPORT void JNICALL
Java_com_capmet_Captivity_CaptiveCollector_ProcSlob_finalize
    (JNIEnv *env, jobject jobj)
{
    jclass cls = env->GetObjectClass(jobj);
    jfieldID fid;
    WinProcess *ps;

    // get handle
    fid = env->GetFieldID(cls, "handle", "J");
    if (fid == 0)
        return;

    // get ps
    ps = (WinProcess *) env->GetLongField(jobj, fid);
    if (ps == 0)
        return;

    delete ps;
}
