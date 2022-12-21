////////////////////////////////////////////////////////////////////////////////
//	Copyright (c) 2001, Brad Cupit -- see the bottom of this file for the BSD
//	license under which this source code is being released
////////////////////////////////////////////////////////////////////////////////
//	File Name:	InactivePtr.h
//
//	Author:		Brad Cupit
//	Project:	C++ Smart Pointers
//	Platforms:	pure
//
//	Description:
//		Defines the template class InactivePtr, which is the Smart Pointer
//		implementation intended for use in a circular reference situation
//
////////////////////////////////////////////////////////////////////////////////
//	History:
//	10/06/01	file created
//
////////////////////////////////////////////////////////////////////////////////
#ifndef _INACTIVEPTR_H_
#define _INACTIVEPTR_H_



namespace ReferenceCountedSmartPointer
{


// class declaration

////////////////////////////////////////////////////////////////////////////////
//	InactivePtr:	Non-reference counted smart pointer, for use in a circular
//					reference situation. (A circular reference is when two
//					reference counted objects point to each other, their
//					reference count will never become zero and therefore they
//					will never get deleted.)
//					An Inactive pointer cannot be used; an Active pointer (Ptr)
//					should be created and initialized with the Inactive pointer.
//					Inactive pointers can become NULL at any time, so after
//					initialization, the Active Pointer should immediately be
//					checked for NULL.
//					Just like Active Pointers, Inactive pointers are guaranteed
//					to either be NULL, or point to a valid object.
////////////////////////////////////////////////////////////////////////////////
template<class T>
class InactivePtr
{
public:
	// templated copy constructor that takes type U which can be
	// implicitly cast to type T
	template<class U>
	InactivePtr( const InactivePtr<U>& pSmartPtr )
	:	m_pRawPtr( pSmartPtr.GetRawPtr() ),
		m_pRefCount( pSmartPtr.GetRefObject() )
	{
		AdjustRefCount();
	}
	
	// templated assignment operator that takes type U which can be
	// implicitly cast to type T
	template<class U>
	InactivePtr& operator=( const InactivePtr<U>& pRightSide )
	{
		ReassignSelf( pRightSide.GetRawPtr(), pRightSide.GetRefObject() );
		return( *this );
	}

	// constructor which takes an Active Pointer of a type U which can be
	// implicitly cast to type T
	template<class U>
	InactivePtr( const Ptr<U>& pSmartPtr )
	:	m_pRawPtr( pSmartPtr.GetRawPtr() ),
		m_pRefCount( pSmartPtr.GetRefObject() )
	{
		AdjustRefCount();
	}

	// assignment operator which takes an Active Pointer of a type U which can
	// be implicitly cast to type T
	template<class U>
	InactivePtr& operator=( const Ptr<U>& pRightSide )
	{
		ReassignSelf( pRightSide.GetRawPtr(), pRightSide.GetRefObject() );
		return( *this );
	}


	InactivePtr( void );
	InactivePtr( const InactivePtr& pSmartPtr );
	InactivePtr& operator=( const InactivePtr& pRightSide );
	
	~InactivePtr( void );
	inline	void ResetPtr( void );
	inline	T* GetRawPtr( void ) const;
	inline	PtrRefCounter* GetRefObject( void ) const;

private:
	inline void	AdjustRefCount( void );
	inline void ReassignSelf( T* pRawPtr, PtrRefCounter* pRefCount );

	T*				m_pRawPtr;		// could point to bad memory
	PtrRefCounter*	m_pRefCount;

};	// class PassivePtr



////////////////////////////////////////////////////////////////////////////////
//	inline function implementations
////////////////////////////////////////////////////////////////////////////////

////////////////////////////////////////////////////////////////////////////////
//	constructor  [public]
//
// description:
//		This is the default constructor, it initalizes all internal pointers
//		to NULL.
template<class T>
InactivePtr<T>::InactivePtr( void )
:	m_pRawPtr( NULL ),
	m_pRefCount( NULL )
{}


////////////////////////////////////////////////////////////////////////////////
//	constructor  [public]
//
// description:
//		This is the copy constructor.
// arguments:
//		pInactivePtr: a reference to another Inactive Pointer
template<class T>
InactivePtr<T>::InactivePtr( const InactivePtr<T>& pInactivePtr )
:	m_pRawPtr( pInactivePtr.GetRawPtr() ),
	m_pRefCount( pInactivePtr.GetRefObject() )
{
	AdjustRefCount();
}


////////////////////////////////////////////////////////////////////////////////
//	operator=  [public]
//
// description:
//		This is the assignment operator which will reset the current pointer
//		values and reassign them.
// arguments:
//		pRightSide: a reference to another Inactive Pointer
// return:
//		a reference to the dereferenced 'this' pointer
template<class T>
InactivePtr<T>& InactivePtr<T>::operator=( const InactivePtr<T>& pRightSide )
{
	ReassignSelf( pRightSide.GetRawPtr(), pRightSide.GetRefObject() );
	return( *this );
}


////////////////////////////////////////////////////////////////////////////////
//	destructor  [public]
//
// description:
//		This is the destructor. It will remove one reference from the inactive
//		count of the Reference Counting Object (m_pRefCount). This could
//		cause the Reference Counting Object to get deleted.
template<class T>
InactivePtr<T>::~InactivePtr( void )
{
	ResetPtr();
}


////////////////////////////////////////////////////////////////////////////////
//	ResetPtr  [public]
//
// description:
//		Resets the pointer. It will remove one reference from the inactive
//		count of the Reference Counting Object (m_pRefCount). This could
//		cause the Reference Counting Object to get deleted.
//		Also sets both internal pointers to NULL.
template<class T>
inline void InactivePtr<T>::ResetPtr( void )
{
	if( m_pRefCount != NULL )
	{
		if( m_pRefCount->SubtractInactiveRef() == false )
		{
			delete m_pRefCount;
		}

		m_pRawPtr = NULL;
		m_pRefCount = NULL;
	}
}


////////////////////////////////////////////////////////////////////////////////
//	GetRawPtr  [public]
//
// description:
//		This function should never be called by a developer! It can possibly
//		return a pointer to bad memory. In any instance, the pointer returned
//		could go bad at any point in time (in a multi-threaded environment)
//		If you need this pointer, create an Active Pointer and use this Inactive
//		pointer to initialize it, then call GetRawPtr() on the newly created
//		Active pointer.
//		This function was only implemented because Microsoft Visual C++ 6.0 does
//		not allow template classes to be friends of other template classes.
// return:
//		the raw pointer (which could be a pointer to bad memory)
template<class T>
inline T* InactivePtr<T>::GetRawPtr( void ) const
{
	return( m_pRawPtr );
}


////////////////////////////////////////////////////////////////////////////////
//	GetRefObject  [public]
//
// description:
//		This function should never be called by a developer! It was only
//		implemented because Microsoft Visual C++ 6.0 does not allow template
//		classes to be friends of other template classes.
// return:
//		the pointer to the Reference Counting Object
template<class T>
inline PtrRefCounter* InactivePtr<T>::GetRefObject( void ) const
{
	return( m_pRefCount );
}


////////////////////////////////////////////////////////////////////////////////
//	AdjustRefCount  [private]
//
// description:
//		Increments the Reference Count in the Reference Counting Object.
//		If this Active Pointer has been initialized with an Inactive Pointer,
//		this function will handle determining if the values are valid. It is
//		possible that a call to this function could set both the Raw and the
//		Reference Counter to NULL
template<class T>
inline void	InactivePtr<T>::AdjustRefCount( void )
{
	if( m_pRefCount != NULL )
	{
		if( m_pRefCount->AddInactiveRef() == false )
		{
			// if AddInactiveRef() was unsucessful, then the Reference
			// Counting Object and the Raw pointer are invalid
			m_pRefCount = NULL;
			m_pRawPtr = NULL;
		}
	}
}


////////////////////////////////////////////////////////////////////////////////
//	ReassignSelf  [private]
//
// description:
//		Reassigns what the smart pointer is pointing to.
// arguments:
//		pRawPtr:   a raw pointer received from some other smart pointer
//		pRefCount: a pointer to a reference counting object, received from
//				   some other smart pointer
template<class T>
inline void InactivePtr<T>::ReassignSelf( T* pRawPtr, PtrRefCounter* pRefCount )
{
	// check for self assignment or assignment to the same Reference Counter
	// that is already being pointed to
	if( m_pRefCount != pRefCount )
	{
		ResetPtr();

		m_pRawPtr = pRawPtr;
		m_pRefCount = pRefCount;

		AdjustRefCount();
	}
}


}	// namespace ReferenceCountedSmartPointer


#endif	// _INACTIVEPTR_H_


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
