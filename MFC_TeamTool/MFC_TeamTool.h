
// MFC_TeamTool.h : MFC_TeamTool ���� ���α׷��� ���� �� ��� ����
//
#pragma once

#ifndef __AFXWIN_H__
	#error "PCH�� ���� �� ������ �����ϱ� ���� 'stdafx.h'�� �����մϴ�."
#endif

#include "resource.h"       // �� ��ȣ�Դϴ�.


// CMFC_TeamToolApp:
// �� Ŭ������ ������ ���ؼ��� MFC_TeamTool.cpp�� �����Ͻʽÿ�.
//

class CMFC_TeamToolApp : public CWinAppEx
{

public:
	CMFC_TeamToolApp();


// �������Դϴ�.
public:
	virtual BOOL InitInstance();
	virtual int ExitInstance();

// �����Դϴ�.
	afx_msg void OnAppAbout();
	DECLARE_MESSAGE_MAP()
};

extern CMFC_TeamToolApp theApp;
