// MakePolicyFile.cpp : Defines the class behaviors for the application.
//

#include "stdafx.h"
#include "MakePolicyFile.h"
#include "MakePolicyFileDlg.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

/////////////////////////////////////////////////////////////////////////////
// CMakePolicyFileApp

BEGIN_MESSAGE_MAP(CMakePolicyFileApp, CWinApp)
	//{{AFX_MSG_MAP(CMakePolicyFileApp)
	//}}AFX_MSG
	ON_COMMAND(ID_HELP, CWinApp::OnHelp)
END_MESSAGE_MAP()

/////////////////////////////////////////////////////////////////////////////
// CMakePolicyFileApp construction

CMakePolicyFileApp::CMakePolicyFileApp()
{
}

/////////////////////////////////////////////////////////////////////////////
// The one and only CMakePolicyFileApp object

CMakePolicyFileApp theApp;

/////////////////////////////////////////////////////////////////////////////
// CMakePolicyFileApp initialization

BOOL CMakePolicyFileApp::InitInstance()
{
	// Standard initialization

	CMakePolicyFileDlg dlg;
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
