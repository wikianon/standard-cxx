////////////////////////////////////////////////////////////////////////////////
//	Copyright (c) 2001, Brad Cupit -- see the bottom of this file for the BSD
//	license under which this source code is being released
////////////////////////////////////////////////////////////////////////////////
//	File Name:	Thread Synchronization.h
//
//	Author:		Brad Cupit
//	Project:	C++ Smart Pointers
//	Platforms:	pure
//
//	Description:
//		Includes the necessary files to define the proper thread synchronization
//		objects to be used on the platform being compiled for.
//
////////////////////////////////////////////////////////////////////////////////
//	History:
//	10/08/01	file created
//
////////////////////////////////////////////////////////////////////////////////
#ifndef _THREAD_SYNCHRONIZATION_H_
#define _THREAD_SYNCHRONIZATION_H_



#ifdef _WIN32
	////////////////////////////////////////////////////////////////////////////
	// Use Win32 thread synchronization constructs
	////////////////////////////////////////////////////////////////////////////
	#ifndef WIN32_LEAN_AND_MEAN
		#define  WIN32_LEAN_AND_MEAN
		#include "windows.h"
		#undef WIN32_LEAN_AND_MEAN
	#else
		#include "windows.h"
	#endif	// WIN32_LEAN_AND_MEAN

	#include "QuickMutex Win32.h"

#else
	////////////////////////////////////////////////////////////////////////////
	// Use POSIX thread synchronization constructs
	////////////////////////////////////////////////////////////////////////////
	#include <pthread.h>
	#include "QuickMutex POSIX.h"

#endif	// _WIN32



#endif	// _THREAD_SYNCHRONIZATION_H_


/*******************************************************************************
Copyright (c) 2001, Brad Cupit
All rights reserved.

Redistribution and use in source and binary forms, with or without modification,
are permitted provided that the following conditions are met:

  - Redistributions of source code must retain the above copyright notice, this
    list of conditions and the following disclaimer.
  - Redistributions in binary form must reproduce the above copyright notice,
    this list of conditions and the following disclaimer in the documentation
    and/or other materials provided with the distribution.
  - Neither the name of this project nor the names of its contributors may be
    used to endorse or promote products derived from this software without
    specific prior written permission. 

THIS SOFTWARE IS PROVIDED BY THE COPYRIGHT HOLDERS AND CONTRIBUTORS "AS IS" AND
ANY EXPRESS OR IMPLIED WARRANTIES, INCLUDING, BUT NOT LIMITED TO, THE IMPLIED
WARRANTIES OF MERCHANTABILITY AND FITNESS FOR A PARTICULAR PURPOSE ARE
DISCLAIMED. IN NO EVENT SHALL THE REGENTS OR CONTRIBUTORS BE LIABLE FOR ANY
DIRECT, INDIRECT, INCIDENTAL, SPECIAL, EXEMPLARY, OR CONSEQUENTIAL DAMAGES
(INCLUDING, BUT NOT LIMITED TO, PROCUREMENT OF SUBSTITUTE GOODS OR SERVICES;
LOSS OF USE, DATA, OR PROFITS; OR BUSINESS INTERRUPTION) HOWEVER CAUSED AND ON
ANY THEORY OF LIABILITY, WHETHER IN CONTRACT, STRICT LIABILITY, OR TORT
(INCLUDING NEGLIGENCE OR OTHERWISE) ARISING IN ANY WAY OUT OF THE USE OF THIS
SOFTWARE, EVEN IF ADVISED OF THE POSSIBILITY OF SUCH DAMAGE.
*******************************************************************************/
