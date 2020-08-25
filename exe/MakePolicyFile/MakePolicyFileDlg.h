// MakePolicyFileDlg.h : header file
//

#if !defined(AFX_MAKEPOLICYFILEDLG_H__B3BB3A8D_9947_4D22_8CF9_D1BCF059E859__INCLUDED_)
#define AFX_MAKEPOLICYFILEDLG_H__B3BB3A8D_9947_4D22_8CF9_D1BCF059E859__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000

/////////////////////////////////////////////////////////////////////////////
// CMakePolicyFileDlg dialog

class CMakePolicyFileDlg : public CDialog
{
// Construction
public:
	CMakePolicyFileDlg(CWnd* pParent = NULL);	// standard constructor

// Dialog Data
	//{{AFX_DATA(CMakePolicyFileDlg)
	enum { IDD = IDD_MAKEPOLICYFILE_DIALOG };
	CListCtrl	m_MsgList;
	//}}AFX_DATA

	// ClassWizard generated virtual function overrides
	//{{AFX_VIRTUAL(CMakePolicyFileDlg)
	protected:
	virtual void DoDataExchange(CDataExchange* pDX);	// DDX/DDV support
	//}}AFX_VIRTUAL

// Implementation
protected:
	HICON m_hIcon;

	// Generated message map functions
	//{{AFX_MSG(CMakePolicyFileDlg)
	virtual BOOL OnInitDialog();
	afx_msg void OnPaint();
	afx_msg HCURSOR OnQueryDragIcon();
	virtual void OnOK();
	afx_msg void OnDestroy();
	//}}AFX_MSG
	DECLARE_MESSAGE_MAP()

private:
	static VOID ThreadRouter(PVOID Context);
	BOOL AddDrvInfo(PCHAR FilePathName);
	ULONG MakeToMem(PCHAR lpPath);
	ULONG Make(PCHAR lpPath);
	BOOL LoadPolicyFile();
	CMapStringToPtr m_DrvInfoMap;
	HANDLE m_ThreadWaitEvent;
	ULONG m_uCol;
};

//{{AFX_INSERT_LOCATION}}
// Microsoft Visual C++ will insert additional declarations immediately before the previous line.

#endif // !defined(AFX_MAKEPOLICYFILEDLG_H__B3BB3A8D_9947_4D22_8CF9_D1BCF059E859__INCLUDED_)
