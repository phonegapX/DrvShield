// MakePolicyFileDlg.cpp : implementation file
//

#include "stdafx.h"
#include "MakePolicyFile.h"
#include "MakePolicyFileDlg.h"
#include "PathSelectDlg.h"
#include "md5.h"
#include "..\..\Drv\common.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

/////////////////////////////////////////////////////////////////////////////
// CMakePolicyFileDlg dialog

CMakePolicyFileDlg::CMakePolicyFileDlg(CWnd* pParent /*=NULL*/)
	: CDialog(CMakePolicyFileDlg::IDD, pParent)
{
	//{{AFX_DATA_INIT(CMakePolicyFileDlg)
		// NOTE: the ClassWizard will add member initialization here
	//}}AFX_DATA_INIT
	m_hIcon = AfxGetApp()->LoadIcon(IDR_MAINFRAME);
}

void CMakePolicyFileDlg::DoDataExchange(CDataExchange* pDX)
{
	CDialog::DoDataExchange(pDX);
	//{{AFX_DATA_MAP(CMakePolicyFileDlg)
	DDX_Control(pDX, IDC_MSGLIST, m_MsgList);
	//}}AFX_DATA_MAP
}

BEGIN_MESSAGE_MAP(CMakePolicyFileDlg, CDialog)
	//{{AFX_MSG_MAP(CMakePolicyFileDlg)
	ON_WM_PAINT()
	ON_WM_QUERYDRAGICON()
	ON_WM_DESTROY()
	//}}AFX_MSG_MAP
END_MESSAGE_MAP()

/////////////////////////////////////////////////////////////////////////////
// CMakePolicyFileDlg message handlers

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
	strcat(pCurrentPath, PolicyFileName);
}

static VOID GetCurTimeStr(PCHAR strBuf)
{
	CTime   time = CTime::GetCurrentTime();   
	CString str  = time.Format("20%y-%m-%d %H:%M:%S");
	lstrcpy(strBuf, str.GetBuffer(0));
}

BOOL CMakePolicyFileDlg::LoadPolicyFile()
{
	HANDLE hFile        = INVALID_HANDLE_VALUE;
	HANDLE hMapping     = NULL;		
	void * pBasePointer = NULL;
	BOOL   bResult      = FALSE;
	CHAR strName[MAX_PATH];
	GetCurrentPath(strName);
	__try
	{
		LARGE_INTEGER FileSize;
		PDRV_ANTI_DATA DrvAntiData;
		PDRV_INFO DrvInfo;
		CHAR szMd5[128];
		hFile = CreateFile(strName, GENERIC_READ, FILE_SHARE_READ, 0, OPEN_EXISTING, FILE_ATTRIBUTE_NORMAL, 0);
		if (hFile == INVALID_HANDLE_VALUE)
		{
			CHAR strTime[MAX_PATH];
			this->m_MsgList.InsertItem(this->m_uCol, (GetCurTimeStr(strTime), strTime), 0);
			this->m_MsgList.SetItem(this->m_uCol, 1, LVIF_TEXT , "原策略文件不存在", 0, 0, 0, NULL);
			this->m_uCol++;
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
			CHAR strTime[MAX_PATH];
			this->m_MsgList.InsertItem(this->m_uCol, (GetCurTimeStr(strTime), strTime), 0);
			this->m_MsgList.SetItem(this->m_uCol, 1, LVIF_TEXT , "原策略文件已经损坏", 0, 0, 0, NULL);
			this->m_uCol++;
			__leave;
		}
		for (ULONG i = 0; i < DrvAntiData->Count; i++)
		{
			memset(szMd5, 0, sizeof(szMd5));
			DrvInfo = new DRV_INFO;
			*DrvInfo = DrvAntiData->DrvInfoEntry[i];
			memcpy(szMd5, DrvInfo->Md5Digest, 32);
			m_DrvInfoMap[szMd5] = DrvInfo;
		}
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

BOOL CMakePolicyFileDlg::OnInitDialog()
{
	CDialog::OnInitDialog();

	SetIcon(m_hIcon, TRUE);			// Set big icon
	SetIcon(m_hIcon, FALSE);		// Set small icon

	// TODO: Add extra initialization here

	m_ThreadWaitEvent = CreateEvent(NULL, TRUE, FALSE, NULL);
	_beginthread(ThreadRouter, 0, this);

	m_MsgList.InsertColumn(0, "时间", LVCFMT_LEFT, 130);
	m_MsgList.InsertColumn(1, "信息", LVCFMT_LEFT, 370);
	ListView_SetExtendedListViewStyle(m_MsgList.m_hWnd, LVS_EX_FULLROWSELECT);

	m_DrvInfoMap.InitHashTable(20000, TRUE);

	return TRUE;  // return TRUE  unless you set the focus to a control
}

// If you add a minimize button to your dialog, you will need the code below
//  to draw the icon.  For MFC applications using the document/view model,
//  this is automatically done for you by the framework.

void CMakePolicyFileDlg::OnPaint() 
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

HCURSOR CMakePolicyFileDlg::OnQueryDragIcon()
{
	return (HCURSOR) m_hIcon;
}

BOOL CMakePolicyFileDlg::AddDrvInfo(PCHAR FilePathName)
{
	BOOL   bResult      = FALSE;
	HANDLE hFile        = INVALID_HANDLE_VALUE;
	HANDLE hMapping     = NULL;		
	void * pBasePointer = NULL;
	__try
	{
		LARGE_INTEGER FileSize;
		IMAGE_DOS_HEADER *imDos_Headers;	//定义DOS头
		IMAGE_NT_HEADERS *imNT_Headers;		//定义PE头
		PVOID lpDrvInfo;
		CHAR szMd5[128];
		hFile = CreateFile(FilePathName, GENERIC_READ, FILE_SHARE_READ, 0, OPEN_EXISTING, FILE_ATTRIBUTE_NORMAL, 0);
		if (hFile == INVALID_HANDLE_VALUE)
		{
			__leave;
		}
		FileSize.LowPart = GetFileSize(hFile, (PULONG)&FileSize.HighPart);
		if (!(hMapping = CreateFileMapping(hFile, 0, PAGE_READONLY|SEC_COMMIT, 0, 0, 0)))
		{
			__leave;
		}
		if (!(pBasePointer = ::MapViewOfFile(hMapping, FILE_MAP_READ, 0, 0, 0)))
		{					
			__leave;
		}
		imDos_Headers = (IMAGE_DOS_HEADER *)pBasePointer;	//设置初始指针地址
		if(imDos_Headers->e_magic != IMAGE_DOS_SIGNATURE)
		{
			__leave;
		}
		imNT_Headers = (IMAGE_NT_HEADERS *)((char *)pBasePointer+imDos_Headers->e_lfanew);//NT头指针地址
		if(imNT_Headers->Signature != IMAGE_NT_SIGNATURE)//判断是否是PE格式文件
		{
			__leave;
		}
		if (imNT_Headers->FileHeader.Machine != IMAGE_FILE_MACHINE_I386)//判断文件对CPU的运行要求
		{
			__leave;
		}
		if (imNT_Headers->OptionalHeader.Subsystem != IMAGE_SUBSYSTEM_NATIVE) //对要求子系统进行判断
		{
			__leave;
		}
		md5(pBasePointer, FileSize.LowPart, szMd5);
		if (!m_DrvInfoMap.Lookup(szMd5, lpDrvInfo))
		{
			lpDrvInfo = new DRV_INFO;
			((PDRV_INFO)lpDrvInfo)->CheckSum      = imNT_Headers->OptionalHeader.CheckSum;
			((PDRV_INFO)lpDrvInfo)->TimeDateStamp = imNT_Headers->FileHeader.TimeDateStamp;
			memcpy(((PDRV_INFO)lpDrvInfo)->Md5Digest, szMd5, 32);
			m_DrvInfoMap[szMd5] = lpDrvInfo;
		}
		bResult = TRUE;
		{
			CHAR strMsg[128];
			CHAR strTime[MAX_PATH];
			wsprintf(strMsg, "读取: %s", FilePathName);
			this->m_MsgList.InsertItem(this->m_uCol, (GetCurTimeStr(strTime), strTime), 0);
			this->m_MsgList.SetItem(this->m_uCol, 1, LVIF_TEXT , strMsg, 0, 0, 0, NULL);
			this->m_uCol++;
		}
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

ULONG CMakePolicyFileDlg::MakeToMem(PCHAR lpPath)
{
	ULONG uError = ERROR_SUCCESS;
	WIN32_FIND_DATA stData;
	CHAR szNewPath[MAX_PATH];
	CHAR szPath[MAX_PATH];
	strcpy(szNewPath, lpPath);
	if (szNewPath[strlen(szNewPath) - 1] != '\\') 
	{ 
		strcat(szNewPath, "\\");
	}
	strcpy(szPath, szNewPath);
	strcat(szNewPath, "*.*");
	HANDLE hSearch = ::FindFirstFile(szNewPath, &stData);
	if (hSearch == INVALID_HANDLE_VALUE) 
	{ 
		uError = GetLastError();
		return uError;
	}
	do
	{
		if (!strcmp(stData.cFileName, "..") || !strcmp(stData.cFileName, ".")) { continue; }
		if (stData.dwFileAttributes & FILE_ATTRIBUTE_DIRECTORY) 
		{	//是目录
			strcpy(szNewPath, szPath);
			if (szNewPath[strlen(szNewPath) - 1] != '\\') { strcat(szNewPath, "\\"); }
			strcat(szNewPath, stData.cFileName);
			strcat(szNewPath, "\\");
			uError = MakeToMem(szNewPath);
			if (uError != ERROR_SUCCESS)
			{
				break;
			}
		}
		else
		{	
			HANDLE hFile;
			//判断是否驱动文件
			if (
				strlen(stData.cFileName) > 4 &&
				(
				stricmp(&stData.cFileName[strlen(stData.cFileName)-4], ".sys") == 0 ||
				stricmp(&stData.cFileName[strlen(stData.cFileName)-4], ".drv") == 0
				)
			)
			{
				strcpy(szNewPath, szPath);
				strcat(szNewPath, stData.cFileName);
				hFile = CreateFile(szNewPath, GENERIC_READ, FILE_SHARE_READ, 0, OPEN_EXISTING, FILE_ATTRIBUTE_NORMAL, 0);
				if (hFile != INVALID_HANDLE_VALUE)
				{
					LARGE_INTEGER FileSize;
					FileSize.LowPart = GetFileSize(hFile, (PULONG)&FileSize.HighPart);
					CloseHandle(hFile);
					if (FileSize.QuadPart < 1024*1024*100 && FileSize.QuadPart > 100)
					{
						AddDrvInfo(szNewPath);
					}
				}
			}
		}
	}
	while (::FindNextFile(hSearch, &stData));
	FindClose(hSearch);
	return uError;
}

ULONG CMakePolicyFileDlg::Make(PCHAR lpPath)
{
	HANDLE hFile  = INVALID_HANDLE_VALUE;
	ULONG  uError = ERROR_SUCCESS;
	PDRV_ANTI_DATA DrvAntiData = NULL;
	try
	{
		ULONG     Count;
		POSITION  pos;
		ULONG     DrvInfoCount;
		ULONG     FileSize;
		PDRV_INFO DrvInfo;
		CHAR      strName[MAX_PATH];
		LoadPolicyFile();	//读取已经存在的策略文件
		MakeToMem(lpPath);  //遍历目录下面所有的驱动文件并且生成策略信息
		DrvInfoCount = m_DrvInfoMap.GetCount();
		if (DrvInfoCount == 0) //不存在任何一个驱动的信息
		{
			CHAR strTime[MAX_PATH];
			this->m_MsgList.InsertItem(this->m_uCol, (GetCurTimeStr(strTime), strTime), 0);
			this->m_MsgList.SetItem(this->m_uCol, 1, LVIF_TEXT , "不存在任何一个驱动的信息", 0, 0, 0, NULL);
			this->m_uCol++;
			uError = ERROR_INVALID_DATA;
			throw(0);
		}
		DrvAntiData = (PDRV_ANTI_DATA)new CHAR[DrvInfoCount * sizeof(DRV_INFO) + 4096];
		if (DrvAntiData == NULL)
		{
			uError = ERROR_NOT_ENOUGH_MEMORY;
			throw(0);
		}
		DrvAntiData->DataMagic = FileMagicFlag;
		DrvAntiData->Count     = DrvInfoCount;
		for(pos = m_DrvInfoMap.GetStartPosition(), Count = 0; pos != NULL; Count++)
		{
			m_DrvInfoMap.GetNextAssoc(pos, CString(), (void*&)DrvInfo);
			DrvAntiData->DrvInfoEntry[Count] = *DrvInfo;
		}
		if (Count != DrvInfoCount)
		{
			uError = ERROR_INVALID_DATA;
			throw(0);
		}
		FileSize = sizeof(DRV_ANTI_DATA)+(DrvAntiData->Count-1)*sizeof(DRV_INFO);
		md5(DrvAntiData, FileSize, ((PCHAR)DrvAntiData)+FileSize);
		FileSize += 32;
		GetCurrentPath(strName);
		hFile = CreateFile(strName, GENERIC_READ|GENERIC_WRITE, 0, 0, CREATE_ALWAYS, FILE_ATTRIBUTE_NORMAL, 0);
		if (hFile == INVALID_HANDLE_VALUE)
		{
			uError = GetLastError();
			throw(0);
		}
		{
			CHAR strMsg[128];
			CHAR strTime[MAX_PATH];
			wsprintf(strMsg, "驱动信息总数: %u  完毕!", DrvAntiData->Count);
			this->m_MsgList.InsertItem(this->m_uCol, (GetCurTimeStr(strTime), strTime), 0);
			this->m_MsgList.SetItem(this->m_uCol, 1, LVIF_TEXT , strMsg, 0, 0, 0, NULL);
			this->m_uCol++;
		}
		if (!WriteFile(hFile, DrvAntiData, FileSize, &Count, NULL))
		{
			uError = GetLastError();
			throw(0);
		}
	}
	catch (...) 
	{
	}
	if (DrvAntiData != NULL)
	{
		delete [] ((PCHAR)DrvAntiData);
	}
	if (hFile != INVALID_HANDLE_VALUE)
	{
		CloseHandle(hFile);
	}
	return uError;
}

VOID CMakePolicyFileDlg::ThreadRouter(PVOID Context)
{
	CMakePolicyFileDlg * Dlg = (CMakePolicyFileDlg *)Context;
	while (TRUE)
	{
		WaitForSingleObject(Dlg->m_ThreadWaitEvent, INFINITE);
		CPathSelectDlg PathSelectDlg(Dlg, "请选择一个目录");
		if(PathSelectDlg.DoModal())
		{
			Dlg->m_MsgList.DeleteAllItems();	//显示框清空
			Dlg->m_uCol = 0;					//显示框计数清0
			Dlg->Make(PathSelectDlg.m_strPath.GetBuffer(0));
		}
		ResetEvent(Dlg->m_ThreadWaitEvent);
	}
}

void CMakePolicyFileDlg::OnOK() 
{
	// TODO: Add extra validation here
	SetEvent(m_ThreadWaitEvent);
}

void CMakePolicyFileDlg::OnDestroy() 
{
	CDialog::OnDestroy();
	
	// TODO: Add your message handler code here
	POSITION  pos;
	PDRV_INFO DrvInfo;
	for(pos = m_DrvInfoMap.GetStartPosition(); pos != NULL; )
	{
		m_DrvInfoMap.GetNextAssoc(pos, CString(), (void*&)DrvInfo);
		delete DrvInfo;
	}
	m_DrvInfoMap.RemoveAll();
}

