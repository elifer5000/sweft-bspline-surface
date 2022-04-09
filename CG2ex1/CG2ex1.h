// CG2ex1.h : main header file for the PROJECT_NAME application
//

#pragma once

#ifndef __AFXWIN_H__
	#error "include 'stdafx.h' before including this file for PCH"
#endif

#include "resource.h"		// main symbols


// CCG2ex1App:
// See CG2ex1.cpp for the implementation of this class
//

class CCG2ex1App : public CWinApp
{
public:
	CCG2ex1App();

// Overrides
	public:
	virtual BOOL InitInstance();

// Implementation

	DECLARE_MESSAGE_MAP()
};

extern CCG2ex1App theApp;