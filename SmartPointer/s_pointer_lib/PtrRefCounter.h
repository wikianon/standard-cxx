////////////////////////////////////////////////////////////////////////////////
//	Copyright (c) 2001, Brad Cupit -- see the bottom of this file for the BSD
//	license under which this source code is being released
////////////////////////////////////////////////////////////////////////////////
//	File Name:	PtrRefCounter.h
//
//	Author:		Brad Cupit
//	Project:	C++ Smart Pointers
//	Platforms:	pure
//
//	Description:
//		Defines the PtrRefCounter class, inline function implementations, and
//		any constants the class uses.
//
////////////////////////////////////////////////////////////////////////////////
//	History:
//	09/30/01	file created
//
////////////////////////////////////////////////////////////////////////////////
#ifndef _PTRREFCOUNTER_H_
#define _PTRREFCOUNTER_H_



namespace ReferenceCountedSmartPointer
{


// constants
const int DELETE_REF_COUNTER = 22;


// class declaration

////////////////////////////////////////////////////////////////////////////////
//	PtrRefCounter:	Object used to keep track of the reference count for a group
//					of smart pointers pointing to the same object. When the last
//					Active reference is removed, the Raw Object that is pointed
//					to should be deleted by the smart pointer. When the last
//					Reference is removed (either active or inactive), the
//					Reference Counting Object (this instantiated class) should
//					be destroyed by the smart pointer
////////////////////////////////////////////////////////////////////////////////
class PtrRefCounter
{
public:
	inline PtrRefCounter( void )
	:	m_uActiveCount( 1 ),
		m_uInactiveCount( 0 )
	{}

	inline	bool AddActiveRef( void );
	inline	bool AddInactiveRef( void );

	inline	int  SubtractActiveRef( void );
	inline	bool SubtractInactiveRef( void );

	inline	bool IsActive( void );

private:
	inline bool ShouldThisObjectBeDeleted( void );

	// intentionally unimplimented to prevent copying
	PtrRefCounter( const PtrRefCounter& );
	PtrRefCounter& operator=( const PtrRefCounter& );

	unsigned long	m_uActiveCount;
	unsigned long	m_uInactiveCount;

	ThreadSafe::QuickMutex  m_pMutex;

};	// class PreRefCounter



////////////////////////////////////////////////////////////////////////////////
//	inline function implementations
////////////////////////////////////////////////////////////////////////////////

////////////////////////////////////////////////////////////////////////////////
//	AddActiveRef  [public]
//
// description:
//		thread safe, adds an active reference to the count as long as the active
//		count is not zero
// return:
//		true if sucessful;
//		false if the Reference Counter Object and Raw pointer (from the calling
//		Smart Pointer) are no longer valid (i.e. should be set to NULL)
inline bool PtrRefCounter::AddActiveRef( void )
{
	bool bActive = false;		// functions value that will be returned

	// this mutex ensures that once the active count is zero, it will
	// not get incremented
	m_pMutex.Lock();
	{
		if( IsActive() )
		{
			++m_uActiveCount;
			bActive = true;
		}
	}
	m_pMutex.Unlock();

	return( bActive );
}


////////////////////////////////////////////////////////////////////////////////
//	AddInactiveRef  [public]
//
// description:
//		thread safe, adds an inactive reference to the count as long as the
//		inactive count is not zero
// return:
//		true if sucessful;
//		false if the Reference Counter Object and Raw pointer (from the calling
//		Smart Pointer) are no longer valid (i.e. should be set to NULL)
inline bool PtrRefCounter::AddInactiveRef( void )
{
	bool bActive = false;		// functions value that will be returned

	// this mutex ensures that once the inactive count is zero, it will
	// not get incremented
	m_pMutex.Lock();
	{
		if( IsActive() )
		{
			++m_uInactiveCount;
			bActive = true;
		}
	}
	m_pMutex.Unlock();

	return( bActive );
}


////////////////////////////////////////////////////////////////////////////////
//	SubtractActiveRef  [public]
//
// description:
//		thread safe, subtracts an active reference from the count
//		NOTE: a call to this function automatically invalidates the pointer that
//		was previously pointing to this object - it should immediately be set
//		to NULL
// return:
//		0 if nothing needs to be done;
//		non-zero if the caller should delete the Raw object it is pointing to;
//		DELETE_REF_COUNTER if the caller should delete the Reference Counter
//		Object (DELETE_REF_COUNTER is a non-zero number, so if it is returned,
//		obviously, both objects should be deleted)
inline int PtrRefCounter::SubtractActiveRef( void )
{
	ASSERT( m_uActiveCount > 0 );

	int iFunctionRetVal = 0;		// functions value that will be returned
	m_pMutex.Lock();
	{
		--m_uActiveCount;
		if( ShouldThisObjectBeDeleted() )
		{
			// no more active or inactive references, so both objects should
			// be deleted
			iFunctionRetVal = DELETE_REF_COUNTER;
		}
		else if( m_uActiveCount == 0)
		{
			// no more active counts, but at least one inactive count remains,
			// so delete the raw pointer, but not the Reference Counting Object
			iFunctionRetVal = -1;	// return a non-zero number
		}
	}
	m_pMutex.Unlock();

	return( iFunctionRetVal );
}


////////////////////////////////////////////////////////////////////////////////
//	SubtractInactiveRef  [public]
//
// description:
//		thread safe, subtracts an inactive reference from the count
//		NOTE: a call to this function automatically invalidates the pointer that
//		was previously pointing to this object - it should immediately be set
//		to NULL
// return:
//		true if nothing need to be done
//		false if the caller should delete the Reference Counting object
inline bool PtrRefCounter::SubtractInactiveRef( void )
{
	ASSERT( m_uInactiveCount > 0 );

	bool bShouldBeDeleted;
	m_pMutex.Lock();
	{
		--m_uInactiveCount;
		bShouldBeDeleted = ShouldThisObjectBeDeleted();
	}
	m_pMutex.Unlock();

	return( !bShouldBeDeleted );
}


////////////////////////////////////////////////////////////////////////////////
//	IsActive  [public]
//
// description:
//		determines if the Raw Object (pointed to by the Smart Pointer) is active
//		(i.e. has at least one active pointer pointing to it)
//		NOTE: this function is not thread safe; the results should not be
//		trusted if being called from an Inactive pointer in a mutlti-threaded
//		environment without being called within a mutex
// return:
//		true if the Raw Object is active
//		false if the Raw Object is invalid
inline bool PtrRefCounter::IsActive( void )
{
	return( m_uActiveCount > 0 );
}


////////////////////////////////////////////////////////////////////////////////
//	ShouldThisObjectBeDeleted  [private]
//
// description:
//		determines if the Reference Counting Object (this instantiated class)
//		should be deleted
// return:
//		true if this object needs to be deleted
//		false if it should remain active
inline bool PtrRefCounter::ShouldThisObjectBeDeleted( void )
{
	if( m_uActiveCount == 0 && m_uInactiveCount == 0 )
	{
		return( true );
	}

	return( false );
}


}	// namespace ReferenceCountedSmartPointer



#endif // _PTRREFCOUNTER_H_


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
