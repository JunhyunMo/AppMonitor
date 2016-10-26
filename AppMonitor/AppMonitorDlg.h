
// AppMonitorDlg.h : 헤더 파일
//

#pragma once
#include "ConnectSocket.h"
#include "afxwin.h"

// CAppMonitorDlg 대화 상자
class CAppMonitorDlg : public CDialogEx
{
// 생성입니다.
public:
	CAppMonitorDlg(CWnd* pParent = NULL);	// 표준 생성자입니다.

	CString GetExecuteDirectory();
	BOOL	ReadConfig();
	BOOL    IsAppAlive(); //App 실행 check
	void    StartApp();

	int		m_nMntTerm;
	int		m_nSckSvrPort;
	CString m_strClassName;
	CString m_strAppFile;

// 대화 상자 데이터입니다.
	enum { IDD = IDD_APPMONITOR_DIALOG };

	protected:
	virtual void DoDataExchange(CDataExchange* pDX);	// DDX/DDV 지원입니다.


// 구현입니다.
protected:
	HICON m_hIcon;

	// 생성된 메시지 맵 함수
	virtual BOOL OnInitDialog();
	afx_msg void OnSysCommand(UINT nID, LPARAM lParam);
	afx_msg void OnPaint();
	afx_msg HCURSOR OnQueryDragIcon();
	DECLARE_MESSAGE_MAP()
public:
	afx_msg void OnBnClickedBtStart();
	afx_msg void OnBnClickedBtStop();
	afx_msg void OnTimer(UINT_PTR nIDEvent);
	afx_msg void OnBnClickedBtConfig();

	CConnectSocket  m_Socket;
	CEdit			m_ctlEditLog;
	CString			m_strEditLog;
	CString			m_strZebraIP;
	UINT			m_nZebraPort;
	//UINT			m_nResetLimit;
	UINT			m_nZebraRebootTime;
	//BOOL			m_bAliveCheck;

	void Unicode2MBCS(LPWSTR lpData,LPSTR lpRtd);
	void MBCS2Unicode(LPCSTR lpData,LPWSTR ReturnData);
	BOOL ConnectToZEBRA(CString strIP,int nPort);
	void DisconnectToZEBRA();
	int	 SendToZEBRA(CString strZPL);
	//BOOL RebootZEBRA();
	void AddLog(CString strLog);
	BOOL NeedToReboot();

	//2016-10-25
	static UINT ThreadCheckRebooting(LPVOID lpvoid); //ZEBRA 리부팅중인지 체크
	BOOL		m_bInRebooting;
	//2016-11-02
	void		CheckRebootingProc(); 

};
