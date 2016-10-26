// ConnectSocket.cpp : implementation file
//

#include "stdafx.h"
#include "ConnectSocket.h"
#include "AppMonitor.h"
#include "AppMonitorDlg.h"

// CConnectSocket

CConnectSocket::CConnectSocket()
{
}

CConnectSocket::~CConnectSocket()
{
}

// CConnectSocket member functions
void CConnectSocket::OnClose(int nErrorCode)
{
	CAppMonitorDlg* pMain = (CAppMonitorDlg*)AfxGetMainWnd();
	pMain->DisconnectToZEBRA();
}

void CConnectSocket::OnReceive(int nErrorCode)
{
	int nRead = 0;
	CString strTemp;

	CHAR chBuff[1024];
	::ZeroMemory(chBuff, sizeof(chBuff));

	TCHAR Buff[1024*2];
	::ZeroMemory(Buff, sizeof(Buff));
	//CIPK_DMSApp* pApp = (CIPK_DMSApp*)AfxGetApp();
	CAppMonitorDlg* pMain = (CAppMonitorDlg*)AfxGetMainWnd();

	nRead = Receive((BYTE*)chBuff, sizeof(chBuff));
	if(nRead >= 0)
	{
		if(strlen(chBuff) > 0) 
		{
			pMain->MBCS2Unicode(chBuff,Buff);
			//pMain->ParseResponse(Buff);
			strTemp.Format(_T("[RCV] %s"),Buff);
			GetLog()->Debug(strTemp.GetBuffer());
			pMain->AddLog(strTemp);
		}
		//pMain->TcpIpDisconn();
		/*strTemp.Format(_T("PRINTER 01 DISCONNECT"));
		GetLog()->Debug(strTemp.GetBuffer());*/
	}
	
	CSocket::OnReceive(nErrorCode);
}



