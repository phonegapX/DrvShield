// DrvShieldDlg.h : header file
//

#if !defined(AFX_DRVSHIELDDLG_H__67D611FD_FEAB_4FD5_894E_A99FFAE10351__INCLUDED_)
#define AFX_DRVSHIELDDLG_H__67D611FD_FEAB_4FD5_894E_A99FFAE10351__INCLUDED_

#include "..\..\DRV\DrvShield\common.h"	// Added by ClassView
#include "TrayIcon.h"	// Added by ClassView
#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000

/////////////////////////////////////////////////////////////////////////////
// CDrvShieldDlg dialog

class CDrvShieldDlg : public CDialog
{
// Construction
public:
	CDrvShieldDlg(CWnd* pParent = NULL);	// standard constructor

// Dialog Data
	//{{AFX_DATA(CDrvShieldDlg)
	enum { IDD = IDD_DRVSHIELD_DIALOG };
	CListCtrl	m_MsgList;
	//}}AFX_DATA

	// ClassWizard generated virtual function overrides
	//{{AFX_VIRTUAL(CDrvShieldDlg)
	protected:
	virtual void DoDataExchange(CDataExchange* pDX);	// DDX/DDV support
	//}}AFX_VIRTUAL

// Implementation
protected:
	CTrayIcon m_trayIcon;
	HICON m_hIcon;

	// Generated message map functions
	//{{AFX_MSG(CDrvShieldDlg)
	virtual BOOL OnInitDialog();
	afx_msg void OnPaint();
	afx_msg HCURSOR OnQueryDragIcon();
	afx_msg void OnClose();
	afx_msg void OnDestroy();
	afx_msg void OnNotifyIcon(WPARAM wParam, LPARAM lParam);
	afx_msg void OnTimer(UINT nIDEvent);
	afx_msg void OnMenuShow();
	afx_msg void OnMenuAbout();
	//}}AFX_MSG
	DECLARE_MESSAGE_MAP()
private:
	BOOL DeviceIO(BOOL bReConn = FALSE);
	static VOID ThreadRouter(PVOID Context);
	BOOL LoadPolicyFile();
	PDRV_ANTI_DATA m_DrvAntiData;
	ULONG m_DataSize;
	HANDLE m_hDriverEntryEvent;
	HANDLE m_hDriverClearedEvent;
	ULONG  m_uCol;
};

//{{AFX_INSERT_LOCATION}}
// Microsoft Visual C++ will insert additional declarations immediately before the previous line.

#endif // !defined(AFX_DRVSHIELDDLG_H__67D611FD_FEAB_4FD5_894E_A99FFAE10351__INCLUDED_)
