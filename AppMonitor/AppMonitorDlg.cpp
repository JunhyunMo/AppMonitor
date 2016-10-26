
// AppMonitorDlg.cpp : ���� ����
//

#include "stdafx.h"
#include "AppMonitor.h"
#include "AppMonitorDlg.h"
#include "afxdialogex.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#endif


// ���� ���α׷� ������ ���Ǵ� CAboutDlg ��ȭ �����Դϴ�.

class CAboutDlg : public CDialogEx
{
public:
	CAboutDlg();

// ��ȭ ���� �������Դϴ�.
	enum { IDD = IDD_ABOUTBOX };

	protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV �����Դϴ�.

// �����Դϴ�.
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


// CAppMonitorDlg ��ȭ ����



CAppMonitorDlg::CAppMonitorDlg(CWnd* pParent /*=NULL*/)
	: CDialogEx(CAppMonitorDlg::IDD, pParent)
	, m_strEditLog(_T(""))
{
	m_strZebraIP = L"";
	m_nZebraPort = 9100;
	m_nZebraRebootTime = 10;
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
END_MESSAGE_MAP()


// CAppMonitorDlg �޽��� ó����

BOOL CAppMonitorDlg::OnInitDialog()
{
	CDialogEx::OnInitDialog();

	// �ý��� �޴��� "����..." �޴� �׸��� �߰��մϴ�.

	// IDM_ABOUTBOX�� �ý��� ���� ������ �־�� �մϴ�.
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

	// �� ��ȭ ������ �������� �����մϴ�. ���� ���α׷��� �� â�� ��ȭ ���ڰ� �ƴ� ��쿡��
	//  �����ӿ�ũ�� �� �۾��� �ڵ����� �����մϴ�.
	SetIcon(m_hIcon, TRUE);			// ū �������� �����մϴ�.
	SetIcon(m_hIcon, FALSE);		// ���� �������� �����մϴ�.

	//�ߺ����� ����
	HANDLE hEvent;
    hEvent = CreateEvent(NULL, FALSE, TRUE, AfxGetAppName());
    if ( GetLastError() == ERROR_ALREADY_EXISTS)
    {
        PostQuitMessage(WM_QUIT);
    }
	//

	OnBnClickedBtStart();

	//ShowWindow(SW_MINIMIZE);

	// TODO: ���⿡ �߰� �ʱ�ȭ �۾��� �߰��մϴ�.

	return TRUE;  // ��Ŀ���� ��Ʈ�ѿ� �������� ������ TRUE�� ��ȯ�մϴ�.
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

// ��ȭ ���ڿ� �ּ�ȭ ���߸� �߰��� ��� �������� �׸�����
//  �Ʒ� �ڵ尡 �ʿ��մϴ�. ����/�� ���� ����ϴ� MFC ���� ���α׷��� ��쿡��
//  �����ӿ�ũ���� �� �۾��� �ڵ����� �����մϴ�.

void CAppMonitorDlg::OnPaint()
{
	if (IsIconic())
	{
		CPaintDC dc(this); // �׸��⸦ ���� ����̽� ���ؽ�Ʈ�Դϴ�.

		SendMessage(WM_ICONERASEBKGND, reinterpret_cast<WPARAM>(dc.GetSafeHdc()), 0);

		// Ŭ���̾�Ʈ �簢������ �������� ����� ����ϴ�.
		int cxIcon = GetSystemMetrics(SM_CXICON);
		int cyIcon = GetSystemMetrics(SM_CYICON);
		CRect rect;
		GetClientRect(&rect);
		int x = (rect.Width() - cxIcon + 1) / 2;
		int y = (rect.Height() - cyIcon + 1) / 2;

		// �������� �׸��ϴ�.
		dc.DrawIcon(x, y, m_hIcon);
	}
	else
	{
		CDialogEx::OnPaint();
	}
}

// ����ڰ� �ּ�ȭ�� â�� ���� ���ȿ� Ŀ���� ǥ�õǵ��� �ý��ۿ���
//  �� �Լ��� ȣ���մϴ�.
HCURSOR CAppMonitorDlg::OnQueryDragIcon()
{
	return static_cast<HCURSOR>(m_hIcon);
}

CString CAppMonitorDlg::GetExecuteDirectory()
{
	CString strFolderPath;

	// ���� ���α׷��� ���� ������ �ִ� �������� ������
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
		MessageBox(L"AppMonitor.ini ������ �������� �ʽ��ϴ�.", L"Error", MB_ICONERROR);
		CString strLog = L"[Error] AppMonitor.ini ������ �������� �ʽ��ϴ�.";
		GetLog()->Debug(strLog.GetBuffer());
		AddLog(strLog);
		return FALSE;
	}

	ZeroMemory(szValue, 0xFF); //����͸� �� Ŭ������
	if (GetPrivateProfileString(L"APP_MONITOR", L"APP_CLASS_NAME" , L"", szValue, sizeof(szValue), strPath))
	{
		m_strClassName = szValue; //
		SetDlgItemText(IDC_EDIT_APP,m_strClassName);
	}

	ZeroMemory(szValue, 0xFF); //����͸� �� ���ϸ�
	if (GetPrivateProfileString(L"APP_MONITOR", L"APP_FILE" , L"", szValue, sizeof(szValue), strPath))
	{
		m_strAppFile = szValue; //
		SetDlgItemText(IDC_EDIT_APP_FILE,m_strAppFile);
	}

	ZeroMemory(szValue, 0xFF); //����͸� �ð����� (SEC)
	if (GetPrivateProfileString(L"APP_MONITOR", L"MONITORING_TERM" , L"", szValue, sizeof(szValue), strPath))
	{
		m_nMntTerm = _ttoi(szValue); //
		SetDlgItemInt(IDC_EDIT_TERM,m_nMntTerm);
	}
	//ZeroMemory(szValue, 0xFF); //RESET ���Ѱ��� (SEC)
	//if (GetPrivateProfileString(L"APP_MONITOR", L"APP_RESET_LIMIT_SEC" , L"", szValue, sizeof(szValue), strPath))
	//{
	//	m_nResetLimit = _ttoi(szValue); //
	//}
	ZeroMemory(szValue, 0xFF); //ZEBRA ����ýð� (SEC)
	if (GetPrivateProfileString(L"APP_MONITOR", L"ZEBRA_REBOOT_SEC" , L"", szValue, sizeof(szValue), strPath))
	{
		m_nZebraRebootTime = _ttoi(szValue); //
	}

	ZeroMemory(szValue, 0xFF); 
	if (GetPrivateProfileString(L"ZEBRA", L"IP",L"", szValue, sizeof(szValue), strPath))
	{
		m_strZebraIP = szValue;
		SetDlgItemText(IDC_IPADDR,m_strZebraIP);
	}

	if (GetPrivateProfileString(L"ZEBRA", L"PORT" , L"", szValue, sizeof(szValue), strPath))
	{
		m_nZebraPort = _ttoi(szValue); //
	}
	
	return TRUE;
}

void CAppMonitorDlg::OnBnClickedBtStart()
{
	// TODO: ���⿡ ��Ʈ�� �˸� ó���� �ڵ带 �߰��մϴ�.
	BOOL bRet = ReadConfig();

	if(bRet == TRUE)
	{
		UINT nElapse = 1;
		nElapse = m_nMntTerm * 1000;
		SetTimer(IDD,nElapse,NULL);
		GetDlgItem(IDC_BT_START)->EnableWindow(FALSE);
		ShowWindow(SW_MINIMIZE);
		CString strLog;
		strLog.Format(L"%s �� ����͸��� ���۵Ǿ����ϴ�.",m_strAppFile);
		GetLog()->Debug(strLog.GetBuffer());
		AddLog(strLog);

		CheckRebootingProc(); //2016-11-02
	}
}

void CAppMonitorDlg::OnBnClickedBtStop()
{
	// TODO: ���⿡ ��Ʈ�� �˸� ó���� �ڵ带 �߰��մϴ�.
	KillTimer(IDD);
	KillTimer(IDD+1);
	GetDlgItem(IDC_BT_START)->EnableWindow(TRUE);

	CString strLog;
	strLog.Format(L"%s �� ����͸��� �����Ǿ����ϴ�.",m_strAppFile);
	GetLog()->Debug(strLog.GetBuffer());
	AddLog(strLog);
}

BOOL CAppMonitorDlg::IsAppAlive() //App ���� check
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
	strApp.Format(L"%s\\%s",GetExecuteDirectory(), m_strAppFile); // ���� ������...

	CString strLog = L"";

	BOOL bRun = ::CreateProcess(strApp, 
		NULL, NULL, NULL, FALSE, 0, NULL, NULL, &si, &pi);

	if(bRun == FALSE)
	{
		
		strLog.Format(_T("%s ���� ����!"), strApp);
		GetLog()->Debug(strLog.GetBuffer());
		AddLog(strLog);

		return;
	}

	DWORD dwWait = WaitForInputIdle(pi.hProcess, INFINITE);
	if(dwWait == 0)
	{
		strLog.Format(_T("%s ����"), strApp);
		GetLog()->Debug(strLog.GetBuffer());
		AddLog(strLog);

		strLog.Format(_T("%s Process Restart."),m_strClassName);
		GetLog()->Debug(strLog.GetBuffer());
		AddLog(strLog);
	}
	
}

void CAppMonitorDlg::OnTimer(UINT_PTR nIDEvent)
{
	// TODO: ���⿡ �޽��� ó���� �ڵ带 �߰� ��/�Ǵ� �⺻���� ȣ���մϴ�.
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
			
		//OLD code
			//BOOL bIsNeedToReboot = NeedToReboot();
			//if( bIsNeedToReboot == TRUE && m_bAliveCheck == TRUE)
			//{
			//	RebootZEBRA();
			//	m_bAliveCheck = FALSE;

			//	nElapse = m_nZebraRebootTime * 1000; //
			//	SetTimer(IDD+1,nElapse,NULL);

			//	nElapse = (m_nResetLimit+5) * 1000;
			//	SetTimer(IDD+2,nElapse,NULL);
			//	return;
			//}
			if(m_bInRebooting == FALSE)
			{
				nElapse = 100; //0.1 SEC
				SetTimer(IDD+1,nElapse,NULL);
			}
			else if(m_bInRebooting == TRUE)
			{
				strLog = _T("ZEBRA �������...App ���� ���...");
				GetLog()->Debug(strLog.GetBuffer());

				nElapse = m_nZebraRebootTime * 1000; //
				SetTimer(IDD+1,nElapse,NULL);
			}
		}
		else
		{
			/*strLog.Format(_T("%s Process On."),m_strClassName);
			GetLog()->Debug(strLog.GetBuffer());*/
			; //�ʹ� ���Ƽ� ����
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

//////////////////////////////////////////////////////////////////////////////////////////
// ZEBRA PRINTER ���� �Լ�
BOOL CAppMonitorDlg::ConnectToZEBRA(CString strIP, int nPort)
{
	//UINT nPort = 9100;

	m_Socket.Create();
	
	CString strLog;

	if(m_Socket.Connect(strIP, nPort) == FALSE) //
	{
		m_Socket.ShutDown(2);
		m_Socket.CancelBlockingCall(); //
		m_Socket.Close();
		
		strLog = L"ERROR: Failed to connect ZEBRA";
		GetLog()->Debug(strLog.GetBuffer());
		AddLog(strLog);
		return FALSE;
	}
	else
	{
		strLog = L"Connected to ZEBRA";
		GetLog()->Debug(strLog.GetBuffer());
		AddLog(strLog);
		return TRUE;
	}

	return FALSE;
}

void CAppMonitorDlg::DisconnectToZEBRA()
{
	m_Socket.ShutDown();
	m_Socket.CancelBlockingCall();
	m_Socket.Close();

	CString strLog = L"Disconnect ZEBRA";
	GetLog()->Debug(strLog.GetBuffer());
	AddLog(strLog);
}

int	CAppMonitorDlg::SendToZEBRA(CString strZPL)
{
	int nRet = SOCKET_ERROR;
	CString strLog;
	char chSendPacket[1024*10]; //size �˳���...
	memset(chSendPacket,0x00,sizeof(chSendPacket));
	
	Unicode2MBCS(strZPL.GetBuffer(),chSendPacket);

	if(ConnectToZEBRA(m_strZebraIP, m_nZebraPort))
	{
		nRet = m_Socket.Send((LPVOID)chSendPacket,strlen(chSendPacket)+1);

		if(nRet > 0 && nRet != SOCKET_ERROR) //If no error occurs, Send returns the number of characters sent. Otherwise, SOCKET_ERROR
		{
			strLog.Format(L"[SND] %s", strZPL);
			GetLog()->Debug(strLog.GetBuffer());
			AddLog(strLog);
		}
		else
		{
			strLog.Format(L"[ERROR] Send [%s] / ErrCode: %d ", strZPL ,nRet);
			GetLog()->Debug(strLog.GetBuffer());
			AddLog(strLog);
		}

		DisconnectToZEBRA();
	}

	return nRet;
}

void CAppMonitorDlg::OnBnClickedBtConfig()
{
	// TODO: ���⿡ ��Ʈ�� �˸� ó���� �ڵ带 �߰��մϴ�.
	OnBnClickedBtStop();
	ShellExecute(NULL,L"open",L"AppMonitor.ini",NULL,NULL,SW_SHOW); //�������� ����
	CString strLog = L"��������(AppMonitor.ini) ����.";
	GetLog()->Debug(strLog.GetBuffer());
	//AddLog(strLog);

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
		m_strEditLog = L"���� Event�� Log������ �����ϼ���.";
		m_strEditLog += L"\r\n";
	}
}

/*MFC �ð��� ���ϱ�
CTime start = CTime::GetCurrentTime();
Sleep(1000);
CTime end = CTime::GetCurrentTime();

CTimeSpan elapseTime = end - start;

CString str;
str.Format(L"%d�ʰ� �������ϴ�.",elapseTime.GetTotalSeconds();

AfxMessageBox(str);
*/

//������ App Reset Time check�ؼ� �����ð� ���ϸ�, ZEBRA�����ͼ��� �̻����� �Ǵ�, ������ϱ� ����
BOOL CAppMonitorDlg::NeedToReboot()
{
	//TCHAR szCurrentPath[1024], szValue[0xFF];
	//GetCurrentDirectory(1024, szCurrentPath);
	//CString strPath = szCurrentPath;
	//CString strExePath = GetExecuteDirectory();
	//strPath.Format(L"%s\\AppMonitor.ini", strExePath);
	//
	//CString strLog;

	//if(!PathFileExists(strPath)) 
	//{
	//	strLog = L"[Error] AppMonitor.ini ������ �������� �ʽ��ϴ�(2).";
	//	GetLog()->Debug(strLog.GetBuffer());
	//	AddLog(strLog);
	//	return FALSE;
	//}

	//CString strLogTime;
	//ZeroMemory(szValue, 0xFF); 
	//if (GetPrivateProfileString(L"APP_MONITOR", L"APP_EXIT_TIME" , L"", szValue, sizeof(szValue), strPath))
	//{
	//	strLogTime = szValue; //������, DMS "RESET" ��û(��Ŷ����)���� ���α׷� ����� �ð�.
	//}

	//int nYear,nMonth, nDay, nHour, nMinute, nSecond; 

	//nYear = _ttoi(strLogTime.Left(4));
	//nMonth = _ttoi(strLogTime.Mid(5,2));
	//nDay = _ttoi(strLogTime.Mid(8,2));

	//nHour = _ttoi(strLogTime.Mid(11,2));
	//nMinute = _ttoi(strLogTime.Mid(14,2));
	//
	////nSecond = _ttoi(strLogTime.Mid(17,2));
	//nSecond = _ttoi(strLogTime.Right(2));


	//CTime logTime(nYear,nMonth,nDay,nHour,nMinute, nSecond);
	//CTime currentTime = CTime::GetCurrentTime();

	//CTimeSpan elapseTime = currentTime - logTime;

	//
	//strLog.Format(L"������ RESET(%s)�� %d�� ���",
	//	strLogTime, elapseTime.GetTotalSeconds());
	//GetLog()->Debug(strLog.GetBuffer());
	//AddLog(strLog);
	//
	//if(elapseTime <= m_nResetLimit) //�����ð� ���� 
	//	return TRUE;
	//else
	//	return FALSE;
	return FALSE;
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