////////////////////////////////////////////////////////////////////////////////
//	Copyright (c) 2001, Brad Cupit -- see the bottom of this file for the BSD
//	license under which this source code is being released
////////////////////////////////////////////////////////////////////////////////
//	File Name:	Smart Pointer.h
//
//	Author:		Brad Cupit
//	Project:	C++ Smart Pointers
//	Platforms:	pure
//
//	Description:
//		This is the file that should be #included at the top of any file using
//		this smart pointer implementation. It will include and define anything
//		else that is needed.
//
////////////////////////////////////////////////////////////////////////////////
//	History:
//	09/28/01	file created
//
////////////////////////////////////////////////////////////////////////////////
#ifndef _SMART_POINTER_H_
#define _SMART_POINTER_H_



// if using the Microsoft Visual C++ compiler version 6.0 or earlier
#if defined(_MSC_VER) && (_MSC_VER <= 1200)

	// disable the compiler warning that says operator-> should not be
	// applied to built in types (i.e. int, float, etc.)
	#pragma warning( disable : 4284 )

	// disable the compiler warning stating that C++ Exception
	// Specifications are ignored
	#pragma warning( disable : 4290 )


	// Microsoft compilers have a bug that prevents the developer from
	// deleting a pointer-to-const -- this function is the workaround
	inline void operator delete( const void* pointer )
	{
		// cast away the constness to overcome the bug in the compiler
		operator delete( (void*)pointer );
	}

#endif	// defined(_MSC_VER) && (_MSC_VER <= 1200)


#ifndef NULL
	#define NULL 0
#endif


// forward references
namespace ReferenceCountedSmartPointer
{
	template<class T> class Ptr;
	template<class T> class InactivePtr;
	class PtrRefCounter;
}


#include "ASSERT.h"
#include "Thread Synchronization.h"
#include "PtrRefCounter.h"
#include "Ptr.h"
#include "InactivePtr.h"
#include "Casting Operators.h"


using namespace PtrExceptions;
using namespace ReferenceCountedSmartPointer;



#endif	// _SMART_POINTER_H_


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
