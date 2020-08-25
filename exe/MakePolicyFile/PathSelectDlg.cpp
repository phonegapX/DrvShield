#include "StdAfx.h"
#include <shlobj.h>
#include "PathSelectDlg.h"

CPathSelectDlg::CPathSelectDlg(CWnd * hwndOwner /*=NULL*/, CString strTitle /*=L""*/) : m_hwndOwner(hwndOwner ? hwndOwner->m_hWnd : NULL), m_strTitle(strTitle)
{
}

CPathSelectDlg::~CPathSelectDlg(void)
{
}

BOOL CPathSelectDlg::DoModal()
{
	LPMALLOC pMalloc;
	if (SHGetMalloc(&pMalloc) != NOERROR)
	{   
		return FALSE;   
	}
	BROWSEINFO   bInfo;
	LPITEMIDLIST pidl;
	ZeroMemory((PVOID)&bInfo, sizeof(BROWSEINFO));

	if (!m_strInitDir.IsEmpty())
	{   
		OLECHAR       olePath[MAX_PATH];   
		ULONG         chEaten;   
		ULONG         dwAttributes;   
		HRESULT       hr;   
		LPSHELLFOLDER pDesktopFolder;   
		//   Get   a   pointer   to   the   Desktop's   IShellFolder   interface.   //   
		if (SUCCEEDED(SHGetDesktopFolder(&pDesktopFolder)))
		{
			//
			//   IShellFolder::ParseDisplayName   requires   the   file   name   be   in   Unicode.   
			//
#ifdef UNICODE
			wcscpy_s(olePath, m_strInitDir.GetBuffer());
#else
			MultiByteToWideChar(CP_ACP, MB_PRECOMPOSED, m_strInitDir.GetBuffer(MAX_PATH), -1, olePath, MAX_PATH);
			m_strInitDir.ReleaseBuffer(-1);
#endif
			//
			//   Convert   the   path   to   an   ITEMIDLIST.   
			//   
			hr = pDesktopFolder->ParseDisplayName(NULL, NULL, olePath, &chEaten, &pidl, &dwAttributes);   
			if (FAILED(hr))   
			{   
				pMalloc->Free(pidl);   
				pMalloc->Release();   
				return FALSE;   
			}   
			bInfo.pidlRoot = pidl;
		}   
	}

	bInfo.hwndOwner = (m_hwndOwner) ? m_hwndOwner : NULL;
	bInfo.pszDisplayName = m_strPath.GetBuffer(MAX_PATH);   
	bInfo.lpszTitle = (m_strTitle.IsEmpty()) ? "Open" : m_strTitle;
	bInfo.ulFlags = BIF_RETURNFSANCESTORS | BIF_RETURNONLYFSDIRS;

	if ((pidl = ::SHBrowseForFolder(&bInfo)) == NULL)   
	{   
		return FALSE;
	}   
	m_strPath.ReleaseBuffer();   
	m_iImageIndex = bInfo.iImage;

	if (::SHGetPathFromIDList(pidl, m_strPath.GetBuffer(MAX_PATH)) == FALSE)
	{   
		pMalloc->Free(pidl);   
		pMalloc->Release();   
		return FALSE;   
	}   

	m_strPath.ReleaseBuffer();

	pMalloc->Free(pidl);   
	pMalloc->Release();   
	return TRUE;
}
