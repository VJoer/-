
// Transportation.h : PROJECT_NAME Ӧ�ó������ͷ�ļ�
//

#pragma once

#ifndef __AFXWIN_H__
	#error "�ڰ������ļ�֮ǰ������stdafx.h�������� PCH �ļ�"
#endif

#include "resource.h"		// ������


// CTransportationApp: 
// �йش����ʵ�֣������ Transportation.cpp
//

class CTransportationApp : public CWinApp
{
public:
	CTransportationApp();

// ��д
public:
	virtual BOOL InitInstance();

// ʵ��

	DECLARE_MESSAGE_MAP()
};

extern CTransportationApp theApp;