////////////////////////////////////////////////////////////////////////////////
//	Copyright (c) 2001, Brad Cupit -- see the bottom of this file for the BSD
//	license under which this source code is being released
////////////////////////////////////////////////////////////////////////////////
//	File Name:	ASSERT.h
//
//	Author:		Brad Cupit
//	Project:	C++ Smart Pointers
//	Platforms:	pure
//
//	Description:
//		Defines the ASSERT macro. For Microsoft Windows applications being
//		complied in Debug mode, a failed ASSERT is displays a Windows dialog.
//
////////////////////////////////////////////////////////////////////////////////
//	History:
//	09/28/01	file created
//
////////////////////////////////////////////////////////////////////////////////
#ifndef _ASSERT_H_
#define _ASSERT_H_



////////////////////////////////////////////////////////////////////////////////
//	ASSERT  [macro]
//
// description:
//		Accepts an expression which resolves to a boolean. If the expression
//		fails in Debug mode, the user is notified which file and line number it
//		ocurred on. In production code (release mode), the expression is not
//		evaluated and the ASSERT does nothing
#ifndef NDEBUG
	#ifdef _WINDOWS		// - debug mode under windows

		#include <stdlib.h>		// defines exit()
		#include <stdio.h>		// defines sprintf()
		#include "windows.h"	// defines ::MessageBox() and ::DebugBreak()

		// description:
		//		determines whether the user wants to debug the application or not
		//		NOTE: if the user chooses to cancel, the application will exit
		// return:
		//		true if the user wants to debug, false if the user wants to continue
		bool AssertFailed( int iLineNumber, char* szFileName, char *szExpression )
		{
			char szMessage[5000];
			sprintf( szMessage, "The following ASSERT failed:\n   ASSERT( %s )\n\nFile:   %s\nLine #%d\n\nPress 'Yes' to Debug, 'No' to continue, or 'Cancel' to quit.",
					 szExpression, szFileName, iLineNumber );
			bool bFuncRetVal;
			int iButtonChosen = ::MessageBox( NULL, szMessage,
											  "Would you like to Debug?",
											  MB_YESNOCANCEL | MB_ICONSTOP |
											  MB_SETFOREGROUND | MB_TASKMODAL );

			switch( iButtonChosen )
			{
			case IDYES:
				bFuncRetVal = true;
				break;
			case IDNO:
				bFuncRetVal = false;
				break;
			default:	// IDCANCEL
				// quit execution
				exit( 0 );
				break;
			}

			return( bFuncRetVal );
		}


		// __LINE__ is the line number the ASSERT is on
		// __FILE__ is the file name the ASSERT is on
		// exp is the expression passed in to this macro
		// ::DebugBreak() calls a breakpoint from this line in the code
		#define ASSERT(exp)	if( !(exp) )									 \
							{												 \
								if( AssertFailed(__LINE__, __FILE__, #exp) ) \
								{											 \
									::DebugBreak();							 \
								}											 \
							}

	#else	// #ifdef _WINDOWS
			// - debug mode, command line or non-Windows OS

		#include <iostream.h>	// defines cout
		#include <stdlib.h>		// defines exit()

		#define ASSERT(exp)	if( !(exp) )											\
							{														\
							   cout << "\nThe following ASSERT failed:\nASSERT( "	\
									<< #exp << " )\n\nFile: " << __FILE__			\
									<< "\nLine #" << __LINE__						\
									<< "\n\nPress enter to quit." << endl;			\
							   cin.get();											\
							   exit( 0 );											\
							}

	#endif	// #ifdef _WINDOWS

#else	// #ifndef NDEBUG
		// - production code (release mode)

	// do nothing in production code
	#define ASSERT(exp)  ;

#endif	// #ifndef NDEBUG



#endif // #define _ASSERT_H_


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
