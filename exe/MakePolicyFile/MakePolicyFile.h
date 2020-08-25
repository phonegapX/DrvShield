// MakePolicyFile.h : main header file for the MAKEPOLICYFILE application
//

#if !defined(AFX_MAKEPOLICYFILE_H__81BA8A1B_8B48_4B65_8581_5456EE39F0EA__INCLUDED_)
#define AFX_MAKEPOLICYFILE_H__81BA8A1B_8B48_4B65_8581_5456EE39F0EA__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000

#ifndef __AFXWIN_H__
	#error include 'stdafx.h' before including this file for PCH
#endif

#include "resource.h"		// main symbols

/////////////////////////////////////////////////////////////////////////////
// CMakePolicyFileApp:
// See MakePolicyFile.cpp for the implementation of this class
//

class CMakePolicyFileApp : public CWinApp
{
public:
	CMakePolicyFileApp();

// Overrides
	// ClassWizard generated virtual function overrides
	//{{AFX_VIRTUAL(CMakePolicyFileApp)
	public:
	virtual BOOL InitInstance();
	//}}AFX_VIRTUAL

// Implementation

	//{{AFX_MSG(CMakePolicyFileApp)
	//}}AFX_MSG
	DECLARE_MESSAGE_MAP()
};


/////////////////////////////////////////////////////////////////////////////

//{{AFX_INSERT_LOCATION}}
// Microsoft Visual C++ will insert additional declarations immediately before the previous line.

#endif // !defined(AFX_MAKEPOLICYFILE_H__81BA8A1B_8B48_4B65_8581_5456EE39F0EA__INCLUDED_)
