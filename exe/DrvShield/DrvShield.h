// DrvShield.h : main header file for the DRVSHIELD application
//

#if !defined(AFX_DRVSHIELD_H__5AF52AB4_2536_41EF_8F54_ED0208871C57__INCLUDED_)
#define AFX_DRVSHIELD_H__5AF52AB4_2536_41EF_8F54_ED0208871C57__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000

#ifndef __AFXWIN_H__
	#error include 'stdafx.h' before including this file for PCH
#endif

#include "resource.h"		// main symbols

/////////////////////////////////////////////////////////////////////////////
// CDrvShieldApp:
// See DrvShield.cpp for the implementation of this class
//

class CDrvShieldApp : public CWinApp
{
public:
	CDrvShieldApp();

// Overrides
	// ClassWizard generated virtual function overrides
	//{{AFX_VIRTUAL(CDrvShieldApp)
	public:
	virtual BOOL InitInstance();
	//}}AFX_VIRTUAL

// Implementation

	//{{AFX_MSG(CDrvShieldApp)
	//}}AFX_MSG
	DECLARE_MESSAGE_MAP()
};


/////////////////////////////////////////////////////////////////////////////

//{{AFX_INSERT_LOCATION}}
// Microsoft Visual C++ will insert additional declarations immediately before the previous line.

#endif // !defined(AFX_DRVSHIELD_H__5AF52AB4_2536_41EF_8F54_ED0208871C57__INCLUDED_)
