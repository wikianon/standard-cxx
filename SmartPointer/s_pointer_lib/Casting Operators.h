////////////////////////////////////////////////////////////////////////////////
//	Copyright (c) 2001, Brad Cupit -- see the bottom of this file for the BSD
//	license under which this source code is being released
////////////////////////////////////////////////////////////////////////////////
//	File Name:	Casting Operators.h
//
//	Author:		Brad Cupit
//	Project:	C++ Smart Pointers
//	Platforms:	pure
//
//	Description:
//		This file defines all of the necessary casting operators for performing
//		C++ like casts on the smart pointers.
//
////////////////////////////////////////////////////////////////////////////////
//	History:
//	10/07/01	file created
//
////////////////////////////////////////////////////////////////////////////////
#ifndef _CASTING_OPERATORS_H_
#define _CASTING_OPERATORS_H_



namespace ReferenceCountedSmartPointer
{


// Const_Cast:			Active pointer --> Active pointer
//						Active pointer --> Inactive pointer
//						Inactive pointer --> Active pointer
//						Inactive pointer --> Inactive pointer
template<class T>
inline Ptr<T> Const_Cast( const Ptr<const T>& pPtrToCast )
{
	// create a temporary local object by directly calling the constructor
	return( Ptr<T>( const_cast<T*>(pPtrToCast.GetRawPtr()),
					pPtrToCast.GetRefObject()
				  )
		  );
}


// Reinterpret_Cast:	Active pointer --> Active pointer
//						Active pointer --> Inactive pointer
template<class T, class U>
inline Ptr<T> Reinterpret_Cast( const Ptr<U>& pPtrToCast )
{
	// create a temporary local object by directly calling the constructor
	return( Ptr<T>( reinterpret_cast<T*>(pPtrToCast.GetRawPtr()),
					pPtrToCast.GetRefObject()
				  )
		  );
}


// Reinterpret_Cast:	Inactive pointer --> Active pointer
//						Inactive pointer --> Inactive pointer
template<class T, class U>
inline Ptr<T> Reinterpret_Cast( const InactivePtr<U>& pPtrToCast )
{
	// create a temporary local object by directly calling the constructor
	return( Ptr<T>( reinterpret_cast<T*>(pPtrToCast.GetRawPtr()),
					pPtrToCast.GetRefObject()
				  )
		  );
}


// Dynamic_Cast:		Active pointer --> Active pointer
//						Active pointer --> Inactive pointer
template<class T, class U>
inline Ptr<T> Dynamic_Cast( const Ptr<U>& pPtrToCast )
{
	// create a temporary local object by directly calling the constructor
	return( Ptr<T>( dynamic_cast<T*>(pPtrToCast.GetRawPtr()),
					pPtrToCast.GetRefObject()
				  )
		  );
}


// Dynamic_Cast:		Inactive pointer --> Active pointer
//						Inactive pointer --> Inactive pointer
// This function is explicitly not allowed because if the Inactive Pointer
// points to an object that has already been deleted, the v-table won't exist
// anymore, which means a call to dynamic_cast could crash the program.
template<class T, class U>
inline InactivePtr<T> Dynamic_Cast( const InactivePtr<U>& pPtrToCast );


}	// namespace ReferenceCountedSmartPointer


#endif	// _CASTING_OPERATORS_H_


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
