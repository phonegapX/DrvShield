#pragma once

class CPathSelectDlg
{
public:
	CPathSelectDlg(CWnd * hwndOwner = NULL, CString strTitle = "");
	virtual ~CPathSelectDlg(void);
	BOOL DoModal();
public:
	CString m_strPath;   
	CString m_strInitDir;   
	CString m_strTitle;   
	int     m_iImageIndex;
protected:
	HWND    m_hwndOwner;
};
