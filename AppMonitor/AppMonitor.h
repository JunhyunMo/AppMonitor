
// AppMonitor.h : PROJECT_NAME ���� ���α׷��� ���� �� ��� �����Դϴ�.
//

#pragma once

#ifndef __AFXWIN_H__
	#error "PCH�� ���� �� ������ �����ϱ� ���� 'stdafx.h'�� �����մϴ�."
#endif

#include "resource.h"		// �� ��ȣ�Դϴ�.


// CAppMonitorApp:
// �� Ŭ������ ������ ���ؼ��� AppMonitor.cpp�� �����Ͻʽÿ�.
//

class CAppMonitorApp : public CWinApp
{
public:
	CAppMonitorApp();

// �������Դϴ�.
public:
	virtual BOOL InitInstance();

// �����Դϴ�.

	DECLARE_MESSAGE_MAP()
};

extern CAppMonitorApp theApp;