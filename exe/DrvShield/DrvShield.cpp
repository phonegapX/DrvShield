// DrvShield.cpp : Defines the class behaviors for the application.
//

#include "stdafx.h"
#include "DrvShield.h"
#include "DrvShieldDlg.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

/////////////////////////////////////////////////////////////////////////////
// CDrvShieldApp

BEGIN_MESSAGE_MAP(CDrvShieldApp, CWinApp)
	//{{AFX_MSG_MAP(CDrvShieldApp)
	//}}AFX_MSG
	ON_COMMAND(ID_HELP, CWinApp::OnHelp)
END_MESSAGE_MAP()

/////////////////////////////////////////////////////////////////////////////
// CDrvShieldApp construction

CDrvShieldApp::CDrvShieldApp()
{
}

/////////////////////////////////////////////////////////////////////////////
// The one and only CDrvShieldApp object

CDrvShieldApp theApp;

/////////////////////////////////////////////////////////////////////////////
// CDrvShieldApp initialization

BOOL CDrvShieldApp::InitInstance()
{
	// Standard initialization

	Sleep(4000); //—”≥Ÿ4√Î

	CDrvShieldDlg dlg;
	m_pMainWnd = &dlg;
	int nResponse = dlg.DoModal();
	if (nResponse == IDOK)
	{
	}
	else if (nResponse == IDCANCEL)
	{
	}

	// Since the dialog has been closed, return FALSE so that we exit the
	//  application, rather than start the application's message pump.
	return FALSE;
}
