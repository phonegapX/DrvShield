// stdafx.h : include file for standard system include files,
//  or project specific include files that are used frequently, but
//      are changed infrequently
//

#if !defined(AFX_STDAFX_H__0E92AB67_CF6A_41FE_A079_FAC333E37AE0__INCLUDED_)
#define AFX_STDAFX_H__0E92AB67_CF6A_41FE_A079_FAC333E37AE0__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000

#ifndef _WIN32_IE // 允许使用 IE 4.0 或更高版本的特定功能。 
#define _WIN32_IE 0x0500 //为 IE 5.0 及更新版本改变为适当的值。 
#endif

#define VC_EXTRALEAN		// Exclude rarely-used stuff from Windows headers

#include <afxwin.h>         // MFC core and standard components
#include <afxext.h>         // MFC extensions
#include <afxdtctl.h>		// MFC support for Internet Explorer 4 Common Controls
#ifndef _AFX_NO_AFXCMN_SUPPORT
#include <afxcmn.h>			// MFC support for Windows Common Controls
#endif // _AFX_NO_AFXCMN_SUPPORT

#include <process.h>
#include <winioctl.h>

BOOL ExpandResource(int nId, PCHAR lpResType, PCHAR lpFileName);

#define WM_USER_NOTIFYICON		(WM_USER + 103)

//{{AFX_INSERT_LOCATION}}
// Microsoft Visual C++ will insert additional declarations immediately before the previous line.

#endif // !defined(AFX_STDAFX_H__0E92AB67_CF6A_41FE_A079_FAC333E37AE0__INCLUDED_)
