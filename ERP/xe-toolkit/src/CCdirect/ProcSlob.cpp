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


#include <iostream>
#include <exception>
#include <pthread.h>
#include <mdr_ps.h>
#include <jni.h>

// these values mirror those in D/PN.java
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
static const int PN_IS_SYSTEM_PROCESS      = 24;

static mdr::mdr_ps_t  *processes = 0;
static uint64_t        last_update;
static pthread_mutex_t proc_lock = PTHREAD_MUTEX_INITIALIZER;

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

static void processesRefreshRates(void)
{
  struct timeval tv;

  pthread_mutex_lock(&proc_lock);

  gettimeofday(&tv, 0);
  // convert to milliseconds
  uint64_t now = (tv.tv_sec * 1000ULL) + (tv.tv_usec / 1000);
  char *thresh_env = getenv("MDR_INTERVAL_THRESHOLD");
  uint64_t threshold = 1000;

  // allow setting of threshold through environment
  if (thresh_env != 0)
    threshold = strtoull(thresh_env, 0, 0);

  // do not update category more than once per threshold
  if ((now - last_update) >= threshold) {
    processes->mdr_refresh_rates();
    last_update = now;
  }

  pthread_mutex_unlock(&proc_lock);
}

extern "C" JNIEXPORT jboolean JNICALL
Java_com_capmet_Captivity_CaptiveCollector_ProcSlob_procSlobConstructor
    (JNIEnv *env, jobject jobj)
{
    jclass cls = env->GetObjectClass(jobj);
    jfieldID fid;
    mdr::mdr_ps_t *ps;

    fid = env->GetFieldID(cls, "handle", "J");
    if (fid == 0)
    {
        fail(env, jobj, cls, "ProcSlob::ProcSlob: cannot find handle");
        return JNI_FALSE;
    }
    try
    {
        if (processes == 0) {
            pthread_mutex_lock(&proc_lock);

            struct timeval tv;

            processes = new mdr::mdr_ps_t();
            gettimeofday(&tv, 0);
            // convert to milliseconds
            last_update = (tv.tv_sec * 1000ULL) + (tv.tv_usec / 1000);

            pthread_mutex_unlock(&proc_lock);
        } else
          processesRefreshRates();

        ps = processes;
    }
    catch(std::exception &e)
    {
        fail(env, jobj, cls, e.what());
        return JNI_FALSE;
    }
    env->SetLongField(jobj, fid, (jlong) _POINTER_CAST_ ps);

    jlong supportFlags = 0;

    if (ps->mdr_is_supported(mdr::MPS_START))
      supportFlags |= (1 << PN_START_TIME);
    if (ps->mdr_is_supported(mdr::MPS_PID))
      supportFlags |= (1 << PN_PROCESS_ID);
    if (ps->mdr_is_supported(mdr::MPS_PPID))
      supportFlags |= (1 << PN_PARENT_PROCESS_ID);
    if (ps->mdr_is_supported(mdr::MPS_USER_NAME))
      supportFlags |= (1 << PN_USER_NAME);
    if (ps->mdr_is_supported(mdr::MPS_FILE_NAME))
      supportFlags |= (1 << PN_COMMAND_NAME);
    if (ps->mdr_is_supported(mdr::MPS_COMMAND))
      supportFlags |= (1 << PN_COMMAND_LINE);
    if (ps->mdr_is_supported(mdr::MPS_CPU_TIME))
      supportFlags |= (1 << PN_TOTAL_CPU_SECONDS);
    if (ps->mdr_is_supported(mdr::MPS_USR_TIME))
      supportFlags |= (1 << PN_USER_CPU_SECONDS);
    if (ps->mdr_is_supported(mdr::MPS_KERNEL_TIME))
      supportFlags |= (1 << PN_KERNEL_CPU_SECONDS);
    if (ps->mdr_is_supported(mdr::MPS_USR_TIME_PERCENT))
      supportFlags |= (1 << PN_PERCENT_USER_TIME);
    if (ps->mdr_is_supported(mdr::MPS_KERNEL_TIME_PERCENT))
      supportFlags |= (1 << PN_PERCENT_KERNEL_TIME);
    if (ps->mdr_is_supported(mdr::MPS_SIZE_K))
      supportFlags |= (1 << PN_TOTAL_SIZE_KB);
    if (ps->mdr_is_supported(mdr::MPS_RSSIZE_K))
      supportFlags |= (1 << PN_RESIDENT_SIZE_KB);
    if (ps->mdr_is_supported(mdr::MPS_IO_K))
      supportFlags |= (1 << PN_TOTAL_IO_KB);
    if (ps->mdr_is_supported(mdr::MPS_CPU_PERCENT))
      supportFlags |= (1 << PN_PERCENT_CPU_USE);
    if (ps->mdr_is_supported(mdr::MPS_MEMORY_PERCENT))
      supportFlags |= (1 << PN_PERCENT_PHYSMEM_USE);
    if (ps->mdr_is_supported(mdr::MPS_DISK_FAULTS))
      supportFlags |= (1 << PN_PAGE_FAULTS);
    if (ps->mdr_is_supported(mdr::MPS_STATE))
      supportFlags |= (1 << PN_PROCESS_STATE);
    if (ps->mdr_is_supported(mdr::MPS_PRIORITY))
      supportFlags |= (1 << PN_GLOBAL_PRIORITY);
    if (ps->mdr_is_supported(mdr::MPS_THREAD_COUNT))
      supportFlags |= (1 << PN_NUMBER_OF_THREADS);
    if (ps->mdr_is_supported(mdr::MPS_SIZE_DELTA_K))
      supportFlags |= (1 << PN_TOTAL_SIZE_DELTA_KB);
    if (ps->mdr_is_supported(mdr::MPS_RSSIZE_DELTA_K))
      supportFlags |= (1 << PN_RESIDENT_SIZE_DELTA_KB);
    if (ps->mdr_is_supported(mdr::MPS_SYSTEM_CALLS))
      supportFlags |= (1 << PN_SYSTEM_CALLS);
    if (ps->mdr_is_supported(mdr::MPS_VOLUNTARY_CSW) ||
        ps->mdr_is_supported(mdr::MPS_INVOLUNTARY_CSW))
      supportFlags |= (1 << PN_CONTEXT_SWITCHES);
    if (ps->mdr_is_supported(mdr::MPS_IS_SYSTEM_PROCESS))
      supportFlags |= (1 << PN_IS_SYSTEM_PROCESS);

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
    mdr::mdr_ps_t *ps = processes;

    // refresh
    try
    {
        processesRefreshRates();
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
    // jmethodID mid = env->GetMethodID(ccls, "<init>", "()V");

    jmethodID mid = env->GetMethodID(cls, "newNode", "()LD/PN;");
    if (mid == 0)
    {
        // fail(env, jobj, cls, "ProcSlob::refreshRates: cannot find PN::PN");
        fail(env, jobj, cls, "ProcSlob::refreshRates: cannot find ProcSlob::newNode");
        return JNI_FALSE;
    }

    jmethodID put = env->GetMethodID(tcls, "put",
        "(Ljava/lang/Object;Ljava/lang/Object;)Ljava/lang/Object;");
    if (put == 0)
    {
        fail(env, jobj, cls, "ProcSlob::refreshRates: cannot find put");
        return JNI_FALSE;
    }

    pthread_mutex_lock(&proc_lock);

    // for each process in ps
    for(mdr::mps_node_t node=ps->begin(); node != ps->end(); node++)
    {
        // cut to the chase
        mdr::mps_can_t *psp = &node->second;

        // allocate a new java node
        // jobject pn = env->NewObject(ccls, mid, 0);
        jobject pn = env->CallObjectMethod(jobj, mid, 0);

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
            pthread_mutex_unlock(&proc_lock);
            return JNI_FALSE;
        }
        env->SetLongField(pn, fid, (jlong) psp->mps_start);

        //
        // processID
        //
        fid = env->GetFieldID(ccls, "processID", "J");
        if (fid == 0)
        {
            fail(env, jobj, cls, "ProcSlob::refreshRates: cannot find processID");
            pthread_mutex_unlock(&proc_lock);
            return JNI_FALSE;
        }
        env->SetLongField(pn, fid, (jlong) psp->mps_pid);

        //
        // parentProcessID
        //
        fid = env->GetFieldID(ccls, "parentProcessID", "J");
        if (fid == 0)
        {
            fail(env, jobj, cls, "ProcSlob::refreshRates: cannot find parentProcessID");
            pthread_mutex_unlock(&proc_lock);
            return JNI_FALSE;
        }
        env->SetLongField(pn, fid, (jlong) psp->mps_ppid);

        //
        // userName
        //
        fid = env->GetFieldID(ccls, "userName", "Ljava/lang/String;");
        if (fid == 0)
        {
            fail(env, jobj, cls, "ProcSlob::refreshRates: cannot find userName");
            pthread_mutex_unlock(&proc_lock);
            return JNI_FALSE;
        }
        jstr = env->NewStringUTF(psp->mps_user_name.c_str());
        env->SetObjectField(pn, fid, jstr);

        //
        // commandName
        //
        fid = env->GetFieldID(ccls, "commandName", "Ljava/lang/String;");
        if (fid == 0)
        {
            fail(env, jobj, cls, "ProcSlob::refreshRates: cannot find commandName");
            pthread_mutex_unlock(&proc_lock);
            return JNI_FALSE;
        }
        jstr = env->NewStringUTF(psp->mps_file_name.c_str());
        env->SetObjectField(pn, fid, jstr);

        //
        // commandLine
        //
        fid = env->GetFieldID(ccls, "commandLine", "Ljava/lang/String;");
        if (fid == 0)
        {
            fail(env, jobj, cls, "ProcSlob::refreshRates: cannot find commandLine");
            pthread_mutex_unlock(&proc_lock);
            return JNI_FALSE;
        }
        jstr = env->NewStringUTF(psp->mps_command.c_str());
        env->SetObjectField(pn, fid, jstr);

        //
        // totalCPUSeconds
        //
        fid = env->GetFieldID(ccls, "totalCPUSeconds", "D");
        if (fid == 0)
        {
            fail(env, jobj, cls, "ProcSlob::refreshRates: cannot find totalCPUSeconds");
            pthread_mutex_unlock(&proc_lock);
            return JNI_FALSE;
        }
        env->SetDoubleField(pn, fid, psp->mps_cpu_time);

        //
        // userCPUSeconds
        //
        fid = env->GetFieldID(ccls, "userCPUSeconds", "D");
        if (fid == 0)
        {
            fail(env, jobj, cls, "ProcSlob::refreshRates: cannot find userCPUSeconds");
            pthread_mutex_unlock(&proc_lock);
            return JNI_FALSE;
        }
        env->SetDoubleField(pn, fid, psp->mps_user_time);

        //
        // kernelCPUSeconds
        //
        fid = env->GetFieldID(ccls, "kernelCPUSeconds", "D");
        if (fid == 0)
        {
            fail(env, jobj, cls, "ProcSlob::refreshRates: cannot find kernelCPUSeconds");
            pthread_mutex_unlock(&proc_lock);
            return JNI_FALSE;
        }
        env->SetDoubleField(pn, fid, psp->mps_kernel_time);

        //
        // percentUserTime
        //
        fid = env->GetFieldID(ccls, "percentUserTime", "D");
        if (fid == 0)
        {
            fail(env, jobj, cls, "ProcSlob::refreshRates: cannot find percentUserTime");
            pthread_mutex_unlock(&proc_lock);
            return JNI_FALSE;
        }
        env->SetDoubleField(pn, fid, psp->mps_user_time_percent);

        //
        // percentKernelTime
        //
        fid = env->GetFieldID(ccls, "percentKernelTime", "D");
        if (fid == 0)
        {
            fail(env, jobj, cls, "ProcSlob::refreshRates: cannot find percentKernelTime");
            pthread_mutex_unlock(&proc_lock);
            return JNI_FALSE;
        }
        env->SetDoubleField(pn, fid, psp->mps_kernel_time_percent);

        //
        // totalSizeKB
        //
        fid = env->GetFieldID(ccls, "totalSizeKB", "I");
        if (fid == 0)
        {
            fail(env, jobj, cls, "ProcSlob::refreshRates: cannot find totalSizeKB");
            pthread_mutex_unlock(&proc_lock);
            return JNI_FALSE;
        }
        env->SetIntField(pn, fid, (jlong) psp->mps_size_K);

        //
        // residentSizeKB
        //
        fid = env->GetFieldID(ccls, "residentSizeKB", "I");
        if (fid == 0)
        {
            fail(env, jobj, cls, "ProcSlob::refreshRates: cannot find residentSizeKB");
            pthread_mutex_unlock(&proc_lock);
            return JNI_FALSE;
        }
        env->SetIntField(pn, fid, (jlong) psp->mps_rssize_K);

        //
        // totalIOKB
        //
        fid = env->GetFieldID(ccls, "totalIOKB", "I");
        if (fid == 0)
        {
            fail(env, jobj, cls, "ProcSlob::refreshRates: cannot find totalIOKB");
            pthread_mutex_unlock(&proc_lock);
            return JNI_FALSE;
        }
        env->SetIntField(pn, fid, (jlong) psp->mps_io_K);

        //
        // percentCPUUse
        //
        fid = env->GetFieldID(ccls, "percentCPUUse", "D");
        if (fid == 0)
        {
            fail(env, jobj, cls, "ProcSlob::refreshRates: cannot find percentCPUUse");
            pthread_mutex_unlock(&proc_lock);
            return JNI_FALSE;
        }
        env->SetDoubleField(pn, fid, psp->mps_cpu_percent);

        //
        // percentPhysmemUse
        //
        fid = env->GetFieldID(ccls, "percentPhysmemUse", "D");
        if (fid == 0)
        {
            fail(env, jobj, cls, "ProcSlob::refreshRates: cannot find percentPhysmemUse");
            pthread_mutex_unlock(&proc_lock);
            return JNI_FALSE;
        }
        env->SetDoubleField(pn, fid, psp->mps_memory_percent);

        //
        // pageFaults
        //
        fid = env->GetFieldID(ccls, "pageFaults", "I");
        if (fid == 0)
        {
            fail(env, jobj, cls, "ProcSlob::refreshRates: cannot find pageFaults");
            pthread_mutex_unlock(&proc_lock);
            return JNI_FALSE;
        }
        env->SetIntField(pn, fid, psp->mps_disk_faults);

        //
        // processState
        //
        fid = env->GetFieldID(ccls, "processState", "Ljava/lang/String;");
        if (fid == 0)
        {
            fail(env, jobj, cls, "ProcSlob::refreshRates: cannot find processState");
            pthread_mutex_unlock(&proc_lock);
            return JNI_FALSE;
        }

        const char *state_name;

        switch(psp->mps_state) {
        case mdr::MPS_RUNNABLE:
            state_name = "Runnable";
            break;
        case mdr::MPS_SLEEPING:
            state_name = "Sleeping";
            break;
        case mdr::MPS_STOPPED:
            state_name = "Stopped";
            break;
        case mdr::MPS_BLOCKED:
            state_name = "Blocked";
            break;
        case mdr::MPS_ZOMBIE:
            state_name = "Unreaped";
            break;
        case mdr::MPS_SPECIFIC:
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
            pthread_mutex_unlock(&proc_lock);
            return JNI_FALSE;
        }
        env->SetIntField(pn, fid, psp->mps_priority);

        //
        // numberOfThreads
        //
        fid = env->GetFieldID(ccls, "numberOfThreads", "I");
        if (fid == 0)
        {
            fail(env, jobj, cls, "ProcSlob::refreshRates: cannot find numberOfThreads");
            pthread_mutex_unlock(&proc_lock);
            return JNI_FALSE;
        }
        env->SetIntField(pn, fid, psp->mps_thread_count);

        //
        // totalSizeDeltaKB
        //
        fid = env->GetFieldID(ccls, "totalSizeDeltaKB", "I");
        if (fid == 0)
        {
            fail(env, jobj, cls, "ProcSlob::refreshRates: cannot find totalSizeDeltaKB");
            pthread_mutex_unlock(&proc_lock);
            return JNI_FALSE;
        }
        env->SetIntField(pn, fid, psp->mps_size_delta_K);

        //
        // residentSizeDeltaKB
        //
        fid = env->GetFieldID(ccls, "residentSizeDeltaKB", "I");
        if (fid == 0)
        {
            fail(env, jobj, cls, "ProcSlob::refreshRates: cannot find residentSizeDeltaKB");
            pthread_mutex_unlock(&proc_lock);
            return JNI_FALSE;
        }
        env->SetIntField(pn, fid, psp->mps_rssize_delta_K);

        //
        // systemCalls
        //
        fid = env->GetFieldID(ccls, "systemCalls", "I");
        if (fid == 0)
        {
            fail(env, jobj, cls, "ProcSlob::refreshRates: cannot find systemCalls");
            pthread_mutex_unlock(&proc_lock);
            return JNI_FALSE;
        }
        env->SetIntField(pn, fid, psp->mps_system_calls);

        //
        // contextSwitches
        //
        fid = env->GetFieldID(ccls, "contextSwitches", "I");
        if (fid == 0)
        {
            fail(env, jobj, cls, "ProcSlob::refreshRates: cannot find contextSwitches");
            pthread_mutex_unlock(&proc_lock);
            return JNI_FALSE;
        }
        env->SetIntField(pn, fid, psp->mps_voluntary_csw + psp->mps_involuntary_csw);

        //
        // isSystemProcess
        //
        fid = env->GetFieldID(ccls, "isSystemProcess", "Z");
        if (fid == 0)
        {
            fail(env, jobj, cls, "ProcSlob::refreshRates: cannot find isSystemProcess");
            pthread_mutex_unlock(&proc_lock);
            return JNI_FALSE;
        }
        env->SetBooleanField(pn, fid, psp->mps_is_system_process);

        // make the process-name-key
        jstr = env->NewStringUTF(node->first.c_str());

        // call TreeMap::put(key, value)
        env->CallObjectMethod(jtree, put, jstr, pn, 0);
    }

    pthread_mutex_unlock(&proc_lock);

    return JNI_TRUE;
}

extern "C" JNIEXPORT void JNICALL
Java_com_capmet_Captivity_CaptiveCollector_ProcSlob_finalize
    (JNIEnv *env, jobject jobj)
{
    // "processes" never goes away (see declaration at top of file)
#if 0
    jclass cls = env->GetObjectClass(jobj);
    jfieldID fid;
    mdr::mdr_ps_t *ps;

    // get handle
    fid = env->GetFieldID(cls, "handle", "J");
    if (fid == 0)
        return;

    // get ps
    ps = (mdr::mdr_ps_t *) _POINTER_CAST_ env->GetLongField(jobj, fid);
    if (ps == 0)
        return;

    delete ps;
#endif
}
