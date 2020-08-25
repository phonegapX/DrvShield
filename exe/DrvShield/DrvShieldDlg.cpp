// DrvShieldDlg.cpp : implementation file
//

#include "stdafx.h"
#include "DrvShield.h"
#include "DrvShieldDlg.h"
#include "LoadDriver.h"
#include "..\MakePolicyFile\md5.h"
#include "..\..\Drv\DrvShield\common.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

/////////////////////////////////////////////////////////////////////////////
// CDrvShieldDlg dialog

CDrvShieldDlg::CDrvShieldDlg(CWnd* pParent /*=NULL*/)
	: CDialog(CDrvShieldDlg::IDD, pParent), m_trayIcon(IDR_POP_MENU)
{
	//{{AFX_DATA_INIT(CDrvShieldDlg)
		// NOTE: the ClassWizard will add member initialization here
	//}}AFX_DATA_INIT
	m_hIcon = AfxGetApp()->LoadIcon(IDR_MAINFRAME);
}

void CDrvShieldDlg::DoDataExchange(CDataExchange* pDX)
{
	CDialog::DoDataExchange(pDX);
	//{{AFX_DATA_MAP(CDrvShieldDlg)
	DDX_Control(pDX, IDC_MSG_LIST, m_MsgList);
	//}}AFX_DATA_MAP
}

BEGIN_MESSAGE_MAP(CDrvShieldDlg, CDialog)
	//{{AFX_MSG_MAP(CDrvShieldDlg)
	ON_WM_PAINT()
	ON_WM_QUERYDRAGICON()
	ON_WM_CLOSE()
	ON_WM_DESTROY()
	ON_MESSAGE(WM_USER_NOTIFYICON, OnNotifyIcon)
	ON_WM_TIMER()
	ON_COMMAND(ID_MENU_SHOW, OnMenuShow)
	ON_COMMAND(ID_MENU_ABOUT, OnMenuAbout)
	//}}AFX_MSG_MAP
END_MESSAGE_MAP()

/////////////////////////////////////////////////////////////////////////////
// CDrvShieldDlg message handlers

static void GetCurrentPathFile(LPSTR pCurrentPath)
{
	char lpPath[MAX_PATH];
	GetModuleFileName(NULL, lpPath, MAX_PATH);
	int nLen = strlen(lpPath);
	for (int i = nLen - 1 ; i >= 0 ; i --)
	{
		if (lpPath[i] == '\\')
		{
			lpPath[i] = '\0';
			break;
		}
	}
	strcpy(pCurrentPath, lpPath);
	strcat(pCurrentPath, PolicyFileName);
}

static void GetCurrentPath(LPSTR pCurrentPath)
{
	char lpPath[MAX_PATH];
	GetModuleFileName(NULL, lpPath, MAX_PATH);
	int nLen = strlen(lpPath);
	for (int i = nLen - 1 ; i >= 0 ; i --)
	{
		if (lpPath[i] == '\\')
		{
			lpPath[i] = '\0';
			break;
		}
	}
	strcpy(pCurrentPath, lpPath);
}

static VOID GetCurTimeStr(PCHAR strBuf)
{
	CTime   time = CTime::GetCurrentTime();
	CString str  = time.Format("20%y-%m-%d %H:%M:%S");
	lstrcpy(strBuf, str.GetBuffer(0));
}

void CDrvShieldDlg::OnMenuShow()
{
	ShowWindow(SW_SHOWNORMAL);
}

void CDrvShieldDlg::OnMenuAbout()
{
	MessageBox("奇东锐腾系统防火墙1.0版", "版本", MB_ICONINFORMATION);
}

BOOL CDrvShieldDlg::OnInitDialog()
{
	CDialog::OnInitDialog();

	SetIcon(m_hIcon, TRUE);			// Set big icon
	SetIcon(m_hIcon, FALSE);		// Set small icon

	SetTimer(0, 2000, NULL);		//2秒后隐藏界面
	// TODO: Add extra initialization here
	// Set up tray icon
	m_trayIcon.SetNotificationWnd(this, WM_USER_NOTIFYICON);
	m_trayIcon.SetIcon(IDR_MAINFRAME);	//右下角图标

	CHAR szTime[128];

	m_MsgList.InsertColumn(0, "捕获时间", LVCFMT_LEFT, 140);
	m_MsgList.InsertColumn(1, "输出信息", LVCFMT_LEFT, 410);

	ListView_SetExtendedListViewStyle(m_MsgList.m_hWnd, LVS_EX_FULLROWSELECT);

	m_uCol = 0;
	m_MsgList.InsertItem(m_uCol, (GetCurTimeStr(szTime), szTime), 0);
	m_MsgList.SetItem(m_uCol, 1, LVIF_TEXT , "奇东锐腾系统防火墙开始启动", 0, 0, 0, NULL);
	m_uCol++;

	if (LoadPolicyFile())
	{
		m_MsgList.InsertItem(m_uCol, (GetCurTimeStr(szTime), szTime), 0);
		m_MsgList.SetItem(m_uCol, 1, LVIF_TEXT , "成功读取策略签名文件", 0, 0, 0, NULL);
		m_uCol++;	
	}
	else
	{
		m_MsgList.InsertItem(m_uCol, (GetCurTimeStr(szTime), szTime), 0);
		m_MsgList.SetItem(m_uCol, 1, LVIF_TEXT , "读取策略签名文件失败", 0, 0, 0, NULL);
		m_uCol++;
		return TRUE;
	}

	CHAR szTempPath[MAX_PATH];
	GetTempPath(sizeof(szTempPath), szTempPath);
	lstrcat(szTempPath, "KeydoneDrvShield\\");
	CreateDirectory(szTempPath, NULL);
	lstrcat(szTempPath, "DrvShield.sys");
	if (!ExpandResource(IDR_DAT, "DAT", szTempPath) || !LoadDriver("DrvShield", szTempPath))
	{
		m_MsgList.InsertItem(m_uCol, (GetCurTimeStr(szTime), szTime), 0);
		m_MsgList.SetItem(m_uCol, 1, LVIF_TEXT , "初始化保护核心模块失败", 0, 0, 0, NULL);
		m_uCol++;
		return TRUE;
	}

	if (DeviceIO())
	{
		m_MsgList.InsertItem(m_uCol, (GetCurTimeStr(szTime), szTime), 0);
		m_MsgList.SetItem(m_uCol, 1, LVIF_TEXT , "初始化监控管理器成功", 0, 0, 0, NULL);
		m_uCol++;
	}
	else
	{
		m_hDriverEntryEvent   = OpenEvent(SYNCHRONIZE, FALSE, "KDDriverEntryEvent");
		m_hDriverClearedEvent = OpenEvent(SYNCHRONIZE, FALSE, "KDDriverClearedEvent");
		if (m_hDriverEntryEvent != NULL && m_hDriverClearedEvent != NULL)
		{
			m_MsgList.InsertItem(m_uCol, (GetCurTimeStr(szTime), szTime), 0);
			m_MsgList.SetItem(m_uCol, 1, LVIF_TEXT , "重新与监控管理器建立通讯", 0, 0, 0, NULL);
			m_uCol++;
			if (!DeviceIO(TRUE))
			{
				m_MsgList.InsertItem(m_uCol, (GetCurTimeStr(szTime), szTime), 0);
				m_MsgList.SetItem(m_uCol, 1, LVIF_TEXT , "重新与监控管理器建立通讯失败", 0, 0, 0, NULL);
				m_uCol++;
				return TRUE;
			}
			_beginthread(ThreadRouter, 0, this);
			return TRUE;
		}

		m_MsgList.InsertItem(m_uCol, (GetCurTimeStr(szTime), szTime), 0);
		m_MsgList.SetItem(m_uCol, 1, LVIF_TEXT , "初始化监控管理器失败", 0, 0, 0, NULL);
		m_uCol++;
		return TRUE;
	}

	m_hDriverEntryEvent   = OpenEvent(SYNCHRONIZE, FALSE, "KDDriverEntryEvent");
	m_hDriverClearedEvent = OpenEvent(SYNCHRONIZE, FALSE, "KDDriverClearedEvent");

	if (m_hDriverEntryEvent == NULL || m_hDriverClearedEvent == NULL)
	{
		m_MsgList.InsertItem(m_uCol, (GetCurTimeStr(szTime), szTime), 0);
		m_MsgList.SetItem(m_uCol, 1, LVIF_TEXT , "无法与监控管理器建立通讯", 0, 0, 0, NULL);
		m_uCol++;
		return TRUE;
	}

	m_MsgList.InsertItem(m_uCol, (GetCurTimeStr(szTime), szTime), 0);
	m_MsgList.SetItem(m_uCol, 1, LVIF_TEXT , "成功与监控管理器建立通讯", 0, 0, 0, NULL);
	m_uCol++;

	_beginthread(ThreadRouter, 0, this);

	return TRUE;  // return TRUE  unless you set the focus to a control
}

// If you add a minimize button to your dialog, you will need the code below
//  to draw the icon.  For MFC applications using the document/view model,
//  this is automatically done for you by the framework.

void CDrvShieldDlg::OnPaint() 
{
	if (IsIconic())
	{
		CPaintDC dc(this); // device context for painting

		SendMessage(WM_ICONERASEBKGND, (WPARAM) dc.GetSafeHdc(), 0);

		// Center icon in client rectangle
		int cxIcon = GetSystemMetrics(SM_CXICON);
		int cyIcon = GetSystemMetrics(SM_CYICON);
		CRect rect;
		GetClientRect(&rect);
		int x = (rect.Width() - cxIcon + 1) / 2;
		int y = (rect.Height() - cyIcon + 1) / 2;

		// Draw the icon
		dc.DrawIcon(x, y, m_hIcon);
	}
	else
	{
		CDialog::OnPaint();
	}
}

HCURSOR CDrvShieldDlg::OnQueryDragIcon()
{
	return (HCURSOR) m_hIcon;
}

BOOL CDrvShieldDlg::LoadPolicyFile()
{
	HANDLE hFile        = INVALID_HANDLE_VALUE;
	HANDLE hMapping     = NULL;
	void * pBasePointer = NULL;
	BOOL   bResult      = FALSE;
	CHAR strName[MAX_PATH];
	GetCurrentPathFile(strName);
	__try
	{
		LARGE_INTEGER FileSize;
		PDRV_ANTI_DATA DrvAntiData;
		CHAR szMd5[128];
		hFile = CreateFile(strName, GENERIC_READ, FILE_SHARE_READ, 0, OPEN_EXISTING, FILE_ATTRIBUTE_NORMAL, 0);
		if (hFile == INVALID_HANDLE_VALUE)
		{
			__leave;
		}
		FileSize.LowPart = GetFileSize(hFile, (PULONG)&FileSize.HighPart);
		if (FileSize.QuadPart < sizeof(DRV_ANTI_DATA)+32 || FileSize.QuadPart > 1024*1024*50)
		{
			__leave;
		}
		if (!(hMapping = CreateFileMapping(hFile, 0, PAGE_READONLY|SEC_COMMIT, 0, 0, 0)))
		{
			__leave;
		}
		if (!(pBasePointer = ::MapViewOfFile(hMapping, FILE_MAP_READ, 0, 0, 0)))
		{
			__leave;
		}
		DrvAntiData = (PDRV_ANTI_DATA)pBasePointer;
		if (DrvAntiData->DataMagic != FileMagicFlag)
		{
			__leave;
		}
		md5(pBasePointer, FileSize.LowPart-32, szMd5);
		if (memcmp(szMd5, ((PCHAR)pBasePointer)+(FileSize.LowPart-32), 32) != 0)
		{
			__leave;
		}
		m_DataSize = FileSize.LowPart;
		m_DrvAntiData = (PDRV_ANTI_DATA)new CHAR[m_DataSize];
		if (m_DrvAntiData == NULL)
		{
			__leave;
		}
		memcpy(m_DrvAntiData, DrvAntiData, m_DataSize);
		bResult = TRUE;
	}
	__finally
	{
		if (pBasePointer != NULL)
		{
			UnmapViewOfFile(pBasePointer);
		}
		if (hMapping != NULL)
		{
			CloseHandle(hMapping);
		}
		if (hFile != INVALID_HANDLE_VALUE)
		{
			CloseHandle(hFile);
		}
	}
	return bResult;
}

void TransDeviceToLetter(PCHAR lpPath)
{
	CHAR szBuf[MAX_PATH];
	CHAR szDeviceName[MAX_PATH];
	CHAR szPath[MAX_PATH];
	CHAR ShortVolumeName[] = " :";
	memset(szDeviceName, 0, sizeof(szDeviceName));
	memcpy(szDeviceName, lpPath, strlen("\\Device\\HarddiskVolumeX"));
	for(UCHAR DriveLetter = 'C'; DriveLetter <= 'Z'; DriveLetter++)
	{
		ShortVolumeName[0] = DriveLetter;
		if (QueryDosDevice(ShortVolumeName, szBuf, MAX_PATH) == 0) 
		{
			continue;
		}
		if (stricmp(szDeviceName, szBuf) == 0) 
		{
			strcpy(szPath, ShortVolumeName);
			strcat(szPath, &lpPath[strlen("\\Device\\HarddiskVolumeX")]);
			strcpy(lpPath, szPath);
			break;
		}
	}
}

void TransFormat(PCHAR lpPath)
{
	CHAR szPath[MAX_PATH];

	strcpy(szPath, lpPath);

	if (
		strlen(szPath) > strlen("\\SystemRoot") &&
		strnicmp(szPath, "\\SystemRoot", strlen("\\SystemRoot")) == 0
		)
	{
		CHAR szBuf[MAX_PATH];
		GetEnvironmentVariable("SystemRoot", szBuf, MAX_PATH);
		strcat(szBuf, &szPath[strlen("\\SystemRoot")]);
		strcpy(szPath, szBuf);
	}
	else if (
		strlen(szPath) > strlen("\\??\\") &&
		strnicmp(szPath, "\\??\\", strlen("\\??\\")) == 0
		)
	{
		CHAR szBuf[MAX_PATH];
		strcpy(szBuf, &szPath[strlen("\\??\\")]);
		strcpy(szPath, szBuf);
	}
	else if (
		strlen(szPath) > strlen("\\Device\\HarddiskVolume") &&
		strnicmp(szPath, "\\Device\\HarddiskVolume", strlen("\\Device\\HarddiskVolume")) == 0
		)
	{
		TransDeviceToLetter(szPath);
	}
	strcpy(lpPath, szPath);
}

VOID CDrvShieldDlg::ThreadRouter(PVOID Context)
{
	CDrvShieldDlg * Dlg = (CDrvShieldDlg *)Context;
	CHAR szDriverName[MAX_PATH];
	CHAR szTime[128];
	CHAR szMsg[512];
	DWORD uReturnByteNum;
	while (TRUE)
	{
		WaitForSingleObject(Dlg->m_hDriverEntryEvent, INFINITE);
		HANDLE hDevice = CreateFile("\\\\.\\KeydoneSysMon", GENERIC_READ|GENERIC_WRITE, FILE_SHARE_READ|FILE_SHARE_WRITE, NULL, OPEN_EXISTING, FILE_ATTRIBUTE_NORMAL, NULL);
		if (hDevice != INVALID_HANDLE_VALUE)
		{
			memset(szDriverName, 0, sizeof(szDriverName));
			if (DeviceIoControl(hDevice, IOCTL_SYS_QUERY_DRIVER, NULL, 0, szDriverName, sizeof(szDriverName), &uReturnByteNum, NULL))
			{
				TransFormat(szDriverName);
				wsprintf(szMsg, "拒绝加载驱动: %s", szDriverName);
				Dlg->m_trayIcon.ShowBalloonTip(szMsg, "警告", 2000);
				Dlg->m_MsgList.InsertItem(Dlg->m_uCol, (GetCurTimeStr(szTime), szTime), 0);
				Dlg->m_MsgList.SetItem(Dlg->m_uCol, 1, LVIF_TEXT , szMsg, 0, 0, 0, NULL);
				Dlg->m_uCol++;
			}
			DeviceIoControl(hDevice, IOCTL_SYS_QUERY_COMPLETE, NULL, 0, NULL, 0, &uReturnByteNum, NULL);
			CloseHandle(hDevice);
		}
	}
}

BOOL CDrvShieldDlg::DeviceIO(BOOL bReConn)
{
	BOOL   bResult = FALSE;
	HANDLE hDevice = INVALID_HANDLE_VALUE;
	__try
	{
		DWORD dwByteNum;
		PARAMETER_BLOCK_PROCESS_ID ParamDataProcessID;
		ULONG ParamBlockSize = sizeof(PARAMETER_BLOCK_DATA)-sizeof(DRV_ANTI_DATA)+m_DataSize;
		PPARAMETER_BLOCK_DATA ParamBlockData = (PPARAMETER_BLOCK_DATA)new CHAR[ParamBlockSize];
		ParamBlockData->ParamBlockMagic = ParamBlockMagicFlag;
		GetCurrentPath(ParamBlockData->WorkPath);
		ParamBlockData->DataSize = m_DataSize;
		memcpy(&ParamBlockData->DrvAntiData, m_DrvAntiData, m_DataSize);
		hDevice = CreateFile("\\\\.\\KeydoneSysMon", GENERIC_READ|GENERIC_WRITE, FILE_SHARE_READ|FILE_SHARE_WRITE, NULL, OPEN_EXISTING, FILE_ATTRIBUTE_NORMAL, NULL);
		if (hDevice == INVALID_HANDLE_VALUE)
		{
			__leave;
		}
		if (!bReConn)
		{
			if (!DeviceIoControl(hDevice, IOCTL_SYS_DRV_MON_SET, ParamBlockData, ParamBlockSize, NULL, 0, &dwByteNum, NULL))
			{
				__leave;
			}
		}
		ParamDataProcessID.ParamBlockMagic = ParamBlockMagicFlag;
		ParamDataProcessID.ProcessID       = GetCurrentProcessId();
		if (!DeviceIoControl(hDevice, IOCTL_SYS_ADD_PROCESSID, &ParamDataProcessID, sizeof(ParamDataProcessID), NULL, 0, &dwByteNum, NULL))
		{
			__leave;
		}
		bResult = TRUE;
	}
	__finally
	{
		if (hDevice != INVALID_HANDLE_VALUE)
		{
			CloseHandle(hDevice);
		}
	}
	return bResult;
}

void CDrvShieldDlg::OnNotifyIcon(WPARAM wParam, LPARAM lParam)
{
	// 响应在托盘图标上的消息
}

void CDrvShieldDlg::OnClose() 
{
	// TODO: Add your message handler code here and/or call default
	ShowWindow(SW_HIDE);
}

void CDrvShieldDlg::OnDestroy()
{
}

void CDrvShieldDlg::OnTimer(UINT nIDEvent)
{
	// TODO: Add your message handler code here and/or call default
	SendMessage(WM_CLOSE);
	KillTimer(nIDEvent);
	CDialog::OnTimer(nIDEvent);
}
