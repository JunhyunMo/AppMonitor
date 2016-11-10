
// AppMonitorDlg.cpp : 구현 파일
//

#include "stdafx.h"
#include "AppMonitor.h"
#include "AppMonitorDlg.h"
#include "afxdialogex.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#endif


// 응용 프로그램 정보에 사용되는 CAboutDlg 대화 상자입니다.

class CAboutDlg : public CDialogEx
{
public:
	CAboutDlg();

// 대화 상자 데이터입니다.
	enum { IDD = IDD_ABOUTBOX };

	protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV 지원입니다.

// 구현입니다.
protected:
	DECLARE_MESSAGE_MAP()
};

CAboutDlg::CAboutDlg() : CDialogEx(CAboutDlg::IDD)
{
}

void CAboutDlg::DoDataExchange(CDataExchange* pDX)
{
	CDialogEx::DoDataExchange(pDX);
}

BEGIN_MESSAGE_MAP(CAboutDlg, CDialogEx)
END_MESSAGE_MAP()


// CAppMonitorDlg 대화 상자

CAppMonitorDlg::CAppMonitorDlg(CWnd* pParent /*=NULL*/)
	: CDialogEx(CAppMonitorDlg::IDD, pParent)
	, m_strEditLog(_T(""))
{
	m_nZebraRebootTime = 20;
	m_bInRebooting = FALSE;
	m_hIcon = AfxGetApp()->LoadIcon(IDR_MAINFRAME);
}

void CAppMonitorDlg::DoDataExchange(CDataExchange* pDX)
{
	CDialogEx::DoDataExchange(pDX);
	DDX_Control(pDX, IDC_EDIT_LOG, m_ctlEditLog);
	DDX_Text(pDX, IDC_EDIT_LOG, m_strEditLog);
}

BEGIN_MESSAGE_MAP(CAppMonitorDlg, CDialogEx)
	ON_WM_SYSCOMMAND()
	ON_WM_PAINT()
	ON_WM_QUERYDRAGICON()
	ON_BN_CLICKED(IDC_BT_START, &CAppMonitorDlg::OnBnClickedBtStart)
	ON_BN_CLICKED(IDC_BT_STOP, &CAppMonitorDlg::OnBnClickedBtStop)
	ON_WM_TIMER()
	ON_BN_CLICKED(IDC_BT_CONFIG, &CAppMonitorDlg::OnBnClickedBtConfig)
	ON_BN_CLICKED(IDC_BT_FILE_DEL, &CAppMonitorDlg::OnBnClickedBtFileDel)
	ON_BN_CLICKED(IDC_CHK_IMG_DEL, &CAppMonitorDlg::OnBnClickedChkImgDel)
END_MESSAGE_MAP()


// CAppMonitorDlg 메시지 처리기

BOOL CAppMonitorDlg::OnInitDialog()
{
	CDialogEx::OnInitDialog();

	// 시스템 메뉴에 "정보..." 메뉴 항목을 추가합니다.

	// IDM_ABOUTBOX는 시스템 명령 범위에 있어야 합니다.
	ASSERT((IDM_ABOUTBOX & 0xFFF0) == IDM_ABOUTBOX);
	ASSERT(IDM_ABOUTBOX < 0xF000);

	CMenu* pSysMenu = GetSystemMenu(FALSE);
	if (pSysMenu != NULL)
	{
		BOOL bNameValid;
		CString strAboutMenu;
		bNameValid = strAboutMenu.LoadString(IDS_ABOUTBOX);
		ASSERT(bNameValid);
		if (!strAboutMenu.IsEmpty())
		{
			pSysMenu->AppendMenu(MF_SEPARATOR);
			pSysMenu->AppendMenu(MF_STRING, IDM_ABOUTBOX, strAboutMenu);
		}
	}

	// 이 대화 상자의 아이콘을 설정합니다. 응용 프로그램의 주 창이 대화 상자가 아닐 경우에는
	//  프레임워크가 이 작업을 자동으로 수행합니다.
	SetIcon(m_hIcon, TRUE);			// 큰 아이콘을 설정합니다.
	SetIcon(m_hIcon, FALSE);		// 작은 아이콘을 설정합니다.

	//중복실행 방지
	HANDLE hEvent;
    hEvent = CreateEvent(NULL, FALSE, TRUE, AfxGetAppName());
    if ( GetLastError() == ERROR_ALREADY_EXISTS)
    {
        PostQuitMessage(WM_QUIT);
    }
	//

	OnBnClickedBtStart();

	// TODO: 여기에 추가 초기화 작업을 추가합니다.

	return TRUE;  // 포커스를 컨트롤에 설정하지 않으면 TRUE를 반환합니다.
}

void CAppMonitorDlg::OnSysCommand(UINT nID, LPARAM lParam)
{
	if ((nID & 0xFFF0) == IDM_ABOUTBOX)
	{
		CAboutDlg dlgAbout;
		dlgAbout.DoModal();
	}
	else
	{
		CDialogEx::OnSysCommand(nID, lParam);
	}
}

// 대화 상자에 최소화 단추를 추가할 경우 아이콘을 그리려면
//  아래 코드가 필요합니다. 문서/뷰 모델을 사용하는 MFC 응용 프로그램의 경우에는
//  프레임워크에서 이 작업을 자동으로 수행합니다.

void CAppMonitorDlg::OnPaint()
{
	if (IsIconic())
	{
		CPaintDC dc(this); // 그리기를 위한 디바이스 컨텍스트입니다.

		SendMessage(WM_ICONERASEBKGND, reinterpret_cast<WPARAM>(dc.GetSafeHdc()), 0);

		// 클라이언트 사각형에서 아이콘을 가운데에 맞춥니다.
		int cxIcon = GetSystemMetrics(SM_CXICON);
		int cyIcon = GetSystemMetrics(SM_CYICON);
		CRect rect;
		GetClientRect(&rect);
		int x = (rect.Width() - cxIcon + 1) / 2;
		int y = (rect.Height() - cyIcon + 1) / 2;

		// 아이콘을 그립니다.
		dc.DrawIcon(x, y, m_hIcon);
	}
	else
	{
		CDialogEx::OnPaint();
	}
}

// 사용자가 최소화된 창을 끄는 동안에 커서가 표시되도록 시스템에서
//  이 함수를 호출합니다.
HCURSOR CAppMonitorDlg::OnQueryDragIcon()
{
	return static_cast<HCURSOR>(m_hIcon);
}

CString CAppMonitorDlg::GetExecuteDirectory()
{
	CString strFolderPath;

	// 현재 프로그램의 실행 파일이 있는 폴더명을 추출함
	::GetModuleFileName(NULL, strFolderPath.GetBuffer(MAX_PATH), MAX_PATH);
	strFolderPath.ReleaseBuffer();
	if (strFolderPath.Find('\\') != -1)
	{
		for (int i = strFolderPath.GetLength() - 1; i >= 0; i--) 
		{
			TCHAR ch = strFolderPath[i];
			strFolderPath.Delete(i);
			if (ch == '\\') break; 
		}
	}
	return strFolderPath;
}

BOOL CAppMonitorDlg::ReadConfig() // \\AppMonitor.ini
{
	TCHAR szCurrentPath[1024], szValue[0xFF];
	GetCurrentDirectory(1024, szCurrentPath);
	CString strPath = szCurrentPath;
	CString strExePath = GetExecuteDirectory();
	strPath.Format(L"%s\\AppMonitor.ini", strExePath);
	
	if(!PathFileExists(strPath)) 
	{
		MessageBox(L"AppMonitor.ini 파일이 존재하지 않습니다.", L"Error", MB_ICONERROR);
		CString strLog = L"[Error] AppMonitor.ini 파일이 존재하지 않습니다.";
		GetLog()->Debug(strLog.GetBuffer());
		AddLog(strLog);
		return FALSE;
	}

	ZeroMemory(szValue, 0xFF); //모니터링 앱 클래스명
	if (GetPrivateProfileString(L"APP_MONITOR", L"APP_CLASS_NAME" , L"", szValue, sizeof(szValue), strPath))
	{
		m_strClassName = szValue; //
		SetDlgItemText(IDC_EDIT_APP,m_strClassName);
	}

	ZeroMemory(szValue, 0xFF); //모니터링 앱 파일명
	if (GetPrivateProfileString(L"APP_MONITOR", L"APP_FILE" , L"", szValue, sizeof(szValue), strPath))
	{
		m_strAppFile = szValue; //
		SetDlgItemText(IDC_EDIT_APP_FILE,m_strAppFile);
	}

	ZeroMemory(szValue, 0xFF); //모니터링 시간간격 (SEC)
	if (GetPrivateProfileString(L"APP_MONITOR", L"MONITORING_TERM" , L"", szValue, sizeof(szValue), strPath))
	{
		m_nMntTerm = _ttoi(szValue); //
		SetDlgItemInt(IDC_EDIT_TERM,m_nMntTerm);
	}

	ZeroMemory(szValue, 0xFF); //라벨 이미지파일 저장경로
	if (GetPrivateProfileString(L"APP_MONITOR", L"LABEL_IMG_FILE_DIR" , L"", szValue, sizeof(szValue), strPath))
	{
		m_strLabelImgFileDir = szValue; // //
		SetDlgItemText(IDC_EDIT_IMG_DIR,m_strLabelImgFileDir);
	}

	ZeroMemory(szValue, 0xFF); //ZEBRA 재부팅시간 (SEC)
	if (GetPrivateProfileString(L"APP_MONITOR", L"ZEBRA_REBOOT_SEC" , L"", szValue, sizeof(szValue), strPath))
	{
		m_nZebraRebootTime = _ttoi(szValue); //
	}
	
	return TRUE;
}

void CAppMonitorDlg::OnBnClickedBtStart()
{
	// TODO: 여기에 컨트롤 알림 처리기 코드를 추가합니다.
	BOOL bRet = ReadConfig();

	if(bRet == TRUE)
	{
		UINT nElapse = 1;
		nElapse = m_nMntTerm * 1000;
		SetTimer(IDD,nElapse,NULL);
		GetDlgItem(IDC_BT_START)->EnableWindow(FALSE);
		ShowWindow(SW_MINIMIZE);
		CString strLog;
		strLog.Format(L"%s 앱 모니터링이 시작되었습니다.",m_strAppFile);
		GetLog()->Debug(strLog.GetBuffer());
		AddLog(strLog);

		CheckRebootingProc(); //2016-11-02
	}
}

void CAppMonitorDlg::OnBnClickedBtStop()
{
	// TODO: 여기에 컨트롤 알림 처리기 코드를 추가합니다.
	KillTimer(IDD);
	KillTimer(IDD+1);
	GetDlgItem(IDC_BT_START)->EnableWindow(TRUE);

	CString strLog;
	strLog.Format(L"%s 앱 모니터링이 중지되었습니다.",m_strAppFile);
	GetLog()->Debug(strLog.GetBuffer());
	AddLog(strLog);
}

BOOL CAppMonitorDlg::IsAppAlive() //App 실행 check
{
	HWND hWnd = NULL;
	hWnd = ::FindWindow(m_strClassName,NULL);

	if(hWnd == NULL)
	{
		return FALSE;
	}
	else
	{
		return TRUE;
	}
}

void CAppMonitorDlg::StartApp()
{
	STARTUPINFO si = {0};
	PROCESS_INFORMATION pi;
	si.cb = sizeof( STARTUPINFO );

	CString strApp;
	strApp.Format(L"%s\\%s",GetExecuteDirectory(), m_strAppFile); // 같은 폴더에...

	CString strLog = L"";

	BOOL bRun = ::CreateProcess(strApp, 
		NULL, NULL, NULL, FALSE, 0, NULL, NULL, &si, &pi);

	if(bRun == FALSE)
	{
		
		strLog.Format(_T("%s 실행 에러!"), strApp);
		GetLog()->Debug(strLog.GetBuffer());
		AddLog(strLog);

		return;
	}

	DWORD dwWait = WaitForInputIdle(pi.hProcess, INFINITE);
	if(dwWait == 0)
	{
		strLog.Format(_T("%s 실행"), strApp);
		GetLog()->Debug(strLog.GetBuffer());
		AddLog(strLog);

		strLog.Format(_T("%s Process Restart."),m_strClassName);
		GetLog()->Debug(strLog.GetBuffer());
		AddLog(strLog);
	}
	
}

void CAppMonitorDlg::OnTimer(UINT_PTR nIDEvent)
{
	// TODO: 여기에 메시지 처리기 코드를 추가 및/또는 기본값을 호출합니다.
	CString strLog = L"";
	UINT nElapse = 1;

	if(nIDEvent == IDD)
	{
		if(IsAppAlive() == FALSE)
		{
			KillTimer(IDD);

			strLog.Format(_T("%s Process Off."),m_strClassName);
			GetLog()->Debug(strLog.GetBuffer());
			AddLog(strLog);
			
			if(m_bInRebooting == FALSE)
			{
				nElapse = 100; //0.1 SEC
				SetTimer(IDD+1,nElapse,NULL);
			}
			else if(m_bInRebooting == TRUE)
			{
				strLog = _T("ZEBRA 재부팅중...App 실행 대기...");
				GetLog()->Debug(strLog.GetBuffer());

				nElapse = m_nZebraRebootTime * 1000; //
				SetTimer(IDD+1,nElapse,NULL);
			}
		}
		else
		{
			/*strLog.Format(_T("%s Process On."),m_strClassName);
			GetLog()->Debug(strLog.GetBuffer());*/
			; //너무 많아서 생략
		}
	
	}
	else if(nIDEvent == IDD+1)
	{
			KillTimer(IDD+1);
			StartApp();

			nElapse = m_nMntTerm * 1000;

			SetTimer(IDD,nElapse,NULL);
	}
	
	CDialogEx::OnTimer(nIDEvent);
}

void CAppMonitorDlg::Unicode2MBCS(LPWSTR lpData,LPSTR lpRtd)
{
	memset(lpRtd,0x00,wcslen(lpData)*2+1);
	WideCharToMultiByte(949,0,lpData,-1,lpRtd,wcslen(lpData)*2,NULL,NULL);
	return;
}

void CAppMonitorDlg::MBCS2Unicode(LPCSTR lpData,LPWSTR ReturnData)
{
	memset(ReturnData,0,(strlen(lpData)/2)+5);
	MultiByteToWideChar(949,0,lpData,-1,ReturnData,strlen(lpData));
	return;
}

void CAppMonitorDlg::AddLog(CString strEvent)
{
	SYSTEMTIME st;
	GetLocalTime(&st);
	CString strLog;
	strLog.Format( _T("%04d-%02d-%02d %02d:%02d:%02d %s\r\n"),
			st.wYear, st.wMonth, st.wDay, st.wHour, st.wMinute, st.wSecond, strEvent);

	m_strEditLog += strLog;
	UpdateData(FALSE);

	m_ctlEditLog.LineScroll(m_ctlEditLog.GetLineCount());

	if(m_ctlEditLog.GetLineCount() > 1000)
	{
		m_strEditLog = L"이전 Event는 Log파일을 참조하세요.";
		m_strEditLog += L"\r\n";
	}
}

UINT CAppMonitorDlg::ThreadCheckRebooting(LPVOID lpvoid)
{
	CAppMonitorDlg* pDlg = (CAppMonitorDlg*)lpvoid;
	CString strFlag;

	while(1)
	{
		TCHAR szCurrentPath[1024], szValue[0xFF];
		GetCurrentDirectory(1024, szCurrentPath);
		CString strPath = szCurrentPath;
		CString strExePath = pDlg->GetExecuteDirectory();
		strPath.Format(L"%s\\AppMonitor.ini", strExePath);
	
		if(PathFileExists(strPath)) 
		{
			ZeroMemory(szValue, 0xFF); 
			if (GetPrivateProfileString(L"ZEBRA", L"IN_REBOOTING",L"", szValue, sizeof(szValue), strPath))
			{
				strFlag = szValue;
			}

		}

		if(strFlag == L"1")
		{
			pDlg->m_bInRebooting = TRUE;
		}
		else
		{
			pDlg->m_bInRebooting = FALSE;
		}

		Sleep(100);
	}

	return 0;
}

void CAppMonitorDlg::CheckRebootingProc()
{
	CWinThread *p1 = NULL;
	p1 = AfxBeginThread(ThreadCheckRebooting,this);

	if(p1 == NULL)
	{
		CString strLog = L"[ERROR] AfxBeginThread(ThreadCheckRebooting,this) - NULL";
	    GetLog()->Debug(strLog.GetBuffer());
	}
	else
	{
		CString strLog = L"AfxBeginThread(ThreadCheckRebooting,this)";
	    GetLog()->Debug(strLog.GetBuffer());
	}
}

void CAppMonitorDlg::OnBnClickedBtConfig()
{
	// TODO: 여기에 컨트롤 알림 처리기 코드를 추가합니다.
	OnBnClickedBtStop();
	ShellExecute(NULL,L"open",L"AppMonitor.ini",NULL,NULL,SW_SHOW); //설정파일 열기
	CString strLog = L"설정파일(AppMonitor.ini) 열림.";
	GetLog()->Debug(strLog.GetBuffer());
}

/////////////////////////////////////////////////////////////////////////////
//2016-11-10 적용여부 미정 - 파일삭제 기능 code 참고만 
void CAppMonitorDlg::OnBnClickedChkImgDel() // Control Visable FALSE
{
	// TODO: 여기에 컨트롤 알림 처리기 코드를 추가합니다.
	if( ((CButton*)GetDlgItem(IDC_CHK_IMG_DEL))->GetCheck() )
	{
		SetDlgItemText(IDC_EDIT_NUM, L"365"); //365일- 1년 default
		GetDlgItem(IDC_EDIT_NUM)->ShowWindow(SW_SHOW);
		GetDlgItem(IDC_STATIC_PERIOD)->ShowWindow(SW_SHOW);
		GetDlgItem(IDC_BT_FILE_DEL)->ShowWindow(SW_SHOW);

		OnBnClickedBtStop();
		GetDlgItem(IDC_BT_CONFIG)->ShowWindow(SW_HIDE);
		GetDlgItem(IDC_BT_START)->EnableWindow(FALSE);
		GetDlgItem(IDC_BT_STOP)->EnableWindow(FALSE);
	}
	else
	{
		GetDlgItem(IDC_EDIT_NUM)->ShowWindow(SW_HIDE);
		GetDlgItem(IDC_STATIC_PERIOD)->ShowWindow(SW_HIDE);
		GetDlgItem(IDC_BT_FILE_DEL)->ShowWindow(SW_HIDE);

		GetDlgItem(IDC_BT_CONFIG)->ShowWindow(SW_SHOW);
		GetDlgItem(IDC_BT_START)->EnableWindow(TRUE);
		GetDlgItem(IDC_BT_STOP)->EnableWindow(TRUE);
	}
}

void CAppMonitorDlg::OnBnClickedBtFileDel()
{
	// TODO: 여기에 컨트롤 알림 처리기 코드를 추가합니다.

	//CString strDay;
	//GetDlgItemText(IDC_EDIT_NUM,strDay);
	//int nElapse = _wtoi(strDay);

	//if(nElapse <= 0 )
	//{
	//	MessageBox(L"정확한 경과일(1일이상)을 입력하세요.", L"ERROR", MB_ICONERROR);
	//	return;
	//}

	//CString strDirName = L"D:/Label";
	//CFileFind finder;

	//BOOL bWorking = finder.FindFile((CString)strDirName + L"/*.*");

	//while(bWorking)
	//{
	//	bWorking = finder.FindNextFileW();

	//	if(finder.IsDots())
	//	{
	//		continue;
	//	}
	//	CString filePath = finder.GetFilePath();
	//	CString strFileName = finder.GetFileName();
	//	CString strTemp;
	//	int nYear, nMonth, nDay, nHour, nMinute, nSec;
	//	//161102164634.png
	//	if(strFileName.GetLength() == 16)
	//	{
	//		strTemp = L"20" + strFileName.Left(2);
	//		nYear = _wtoi(strTemp);
	//		nMonth = _wtoi(strFileName.Mid(2,2));
	//		nDay = _wtoi(strFileName.Mid(4,2));
	//		nHour = _wtoi(strFileName.Mid(6,2));
	//		nMinute = _wtoi(strFileName.Mid(8,2));
	//		nSec = _wtoi(strFileName.Mid(10,2));
	//	
	//		CTime tStart(nYear, nMonth, nDay, nHour, nMinute, nSec);
	//		CTime tEnd = CTime::GetCurrentTime();

	//		CTimeSpan elapseTime = tEnd -tStart;

	//		if(elapseTime.GetTotalSeconds() > 3600) // ex) 1시간 이전 파일 삭제
	//		{
	//			//AddLog(strFileName); 
	//			; //삭제 or ...
	//		}
	//	}
	//}
}

