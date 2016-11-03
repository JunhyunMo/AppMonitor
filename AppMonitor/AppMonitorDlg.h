
// AppMonitorDlg.h : ��� ����
//

#pragma once
#include "ConnectSocket.h"
#include "afxwin.h"

// CAppMonitorDlg ��ȭ ����
class CAppMonitorDlg : public CDialogEx
{
// �����Դϴ�.
public:
	CAppMonitorDlg(CWnd* pParent = NULL);	// ǥ�� �������Դϴ�.

	CString GetExecuteDirectory();
	BOOL	ReadConfig();
	BOOL    IsAppAlive(); //App ���� check
	void    StartApp();

	int		m_nMntTerm;
	int		m_nSckSvrPort;
	CString m_strClassName;
	CString m_strAppFile;

// ��ȭ ���� �������Դϴ�.
	enum { IDD = IDD_APPMONITOR_DIALOG };

	protected:
	virtual void DoDataExchange(CDataExchange* pDX);	// DDX/DDV �����Դϴ�.


// �����Դϴ�.
protected:
	HICON m_hIcon;

	// ������ �޽��� �� �Լ�
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
	static UINT ThreadCheckRebooting(LPVOID lpvoid); //ZEBRA ������������ üũ
	BOOL		m_bInRebooting;
	//2016-11-02
	void		CheckRebootingProc(); 

};
