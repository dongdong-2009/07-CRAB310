// TestWeb.h : main header file for the TESTWEB application
//

#if !defined(AFX_TESTWEB_H__C5A6E521_6528_4144_B8CA_EFE1A05BD7A2__INCLUDED_)
#define AFX_TESTWEB_H__C5A6E521_6528_4144_B8CA_EFE1A05BD7A2__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000

#ifndef __AFXWIN_H__
	#error include 'stdafx.h' before including this file for PCH
#endif

#include "resource.h"		// main symbols

/////////////////////////////////////////////////////////////////////////////
// CTestWebApp:
// See TestWeb.cpp for the implementation of this class
//

class CTestWebApp : public CWinApp
{
public:
	CTestWebApp();

// Overrides
	// ClassWizard generated virtual function overrides
	//{{AFX_VIRTUAL(CTestWebApp)
	public:
	virtual BOOL InitInstance();
	//}}AFX_VIRTUAL

// Implementation

	//{{AFX_MSG(CTestWebApp)
		// NOTE - the ClassWizard will add and remove member functions here.
		//    DO NOT EDIT what you see in these blocks of generated code !
	//}}AFX_MSG
	DECLARE_MESSAGE_MAP()
};


/////////////////////////////////////////////////////////////////////////////

//{{AFX_INSERT_LOCATION}}
// Microsoft Visual C++ will insert additional declarations immediately before the previous line.

#endif // !defined(AFX_TESTWEB_H__C5A6E521_6528_4144_B8CA_EFE1A05BD7A2__INCLUDED_)
