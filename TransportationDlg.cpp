
// TransportationDlg.cpp : ʵ���ļ�
//

#include "stdafx.h"
#include "Transportation.h"
#include "TransportationDlg.h"
#include "afxdialogex.h"

#include "cstringt.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#endif


// CTransportationDlg �Ի���


CTransportationDlg::CTransportationDlg(CWnd* pParent /*=NULL*/)
	: CDialogEx(CTransportationDlg::IDD, pParent)
	//��ʼ��
	, m_szCOMName(_T(""))
	, m_szCurretnCOMName(_T(""))
	, m_szBaudTate(_T(""))
	, m_COMIndex(0)
	, m_dlgSaveFile(FALSE, NULL, NULL, OFN_HIDEREADONLY, _T("Describe Files (*.bmp)|*.bmp||"), this)
	, m_hasCOMImage(false)
	, m_IsOpenCOM(false)
{
	m_hIcon = AfxGetApp()->LoadIcon(IDR_MAINFRAME);
}

void CTransportationDlg::DoDataExchange(CDataExchange* pDX)
{
	CDialogEx::DoDataExchange(pDX);
	DDX_CBString(pDX, IDC_COM_LIST, m_szCOMName);
	DDX_Control(pDX, IDC_COM_LIST, m_ComListCtrl);
	DDX_CBString(pDX, IDC_COMBO1, m_szBaudTate);
	DDX_Control(pDX, IDC_MSCOMM1, m_com);
	DDX_Control(pDX, IDC_COMBO1, m_BaudRateCtrl);
}

BEGIN_MESSAGE_MAP(CTransportationDlg, CDialogEx)
	ON_WM_PAINT()
	ON_WM_QUERYDRAGICON()
	ON_BN_CLICKED(IDC_INIT_COM, &CTransportationDlg::OnBnClickedInitCom)
	ON_BN_CLICKED(IDC_SAVE_PICCOM, &CTransportationDlg::OnBnClickedSavePiccom)
	ON_CBN_SELENDOK(IDC_COMBO1, &CTransportationDlg::OnSelendokCombo1)
	ON_BN_CLICKED(IDC_COM_CLEAR, &CTransportationDlg::OnBnClickedComClear)
END_MESSAGE_MAP()


// CTransportationDlg ��Ϣ�������

BOOL CTransportationDlg::OnInitDialog()
{
	CDialogEx::OnInitDialog();

	// ���ô˶Ի����ͼ�ꡣ  ��Ӧ�ó��������ڲ��ǶԻ���ʱ����ܽ��Զ�
	//  ִ�д˲���
	SetIcon(m_hIcon, TRUE);			// ���ô�ͼ��
	SetIcon(m_hIcon, FALSE);		// ����Сͼ��

	// TODO:  �ڴ���Ӷ���ĳ�ʼ������
	//��ʼ��CImage��д���ļ�ͷ����Ϣͷ
	InitalImage(m_COMImage, 320, 240);

	//���ڲ���
	HANDLE fd;
	char i = 1;
	wchar_t com_name[10];
	m_ComListCtrl.ResetContent();		//�б����Ƴ�������
	if (m_IsOpenCOM)
	{
		m_ComListCtrl.AddString(m_szCurretnCOMName);
	}
	while (i < 21)
	{
		wsprintf(com_name, L"COM%d", i);
		fd = CreateFile(com_name, GENERIC_READ | GENERIC_WRITE, 0, NULL, OPEN_EXISTING, 0, NULL);
		if (fd != INVALID_HANDLE_VALUE)
		{
			m_ComListCtrl.AddString(com_name);
			CloseHandle(fd);
		}
		i++;
	}
	if (m_ComListCtrl.GetCount() > 0)		//���ض������б��е�
	{
		m_ComListCtrl.SetCurSel(0);		//ѡ������Ͽ���б���е��ַ���
		//����ָ�����Ӵ���
		GetDlgItem(IDC_INIT_COM)->\
		//ָ���ض������Ƿ����û���á� ����˲����� TRUE�����ڽ����á� ����˲����� FALSE�����ڽ�������
		EnableWindow(TRUE);
	}
	else
		GetDlgItem(IDC_INIT_COM)->EnableWindow(FALSE);
 	if (m_ComListCtrl.GetCount() == 1)
 	{
 		OnBnClickedInitCom();
 	}
	m_BaudRateCtrl.SetCurSel(0);

	return TRUE;  // ���ǽ��������õ��ؼ������򷵻� TRUE
}

// �����Ի��������С����ť������Ҫ����Ĵ���
//  �����Ƹ�ͼ�ꡣ  ����ʹ���ĵ�/��ͼģ�͵� MFC Ӧ�ó���
//  �⽫�ɿ���Զ���ɡ�

void CTransportationDlg::OnPaint()
{
	int cx, cy;
	CRect	rect;
	CWnd *pWnd = NULL;
	if (IsIconic())
	{
		CPaintDC dc(this); // ���ڻ��Ƶ��豸������

		SendMessage(WM_ICONERASEBKGND, reinterpret_cast<WPARAM>(dc.GetSafeHdc()), 0);

		// ʹͼ���ڹ����������о���
		int cxIcon = GetSystemMetrics(SM_CXICON);
		int cyIcon = GetSystemMetrics(SM_CYICON);
		CRect rect;
		GetClientRect(&rect);
		int x = (rect.Width() - cxIcon + 1) / 2;
		int y = (rect.Height() - cyIcon + 1) / 2;

		// ����ͼ��
		dc.DrawIcon(x, y, m_hIcon);
	}
	else
	{
		//ȷ�϶Ի����������Ƿ���һ��������ͼ��		����
		if (m_hasCOMImage && !m_COMImage.IsNull())
		{
			//��ȡͼƬ�Ŀ� �߶�
			cx = m_COMImage.GetWidth();
			cy = m_COMImage.GetHeight();
			//ResizeWindow(cx, cy);
			//��ȡIDC_PIC1�Ĵ���ָ��
			pWnd = GetDlgItem(IDC_PICCOM);
			//��ȡPicture Control�ؼ��Ĵ�С
			pWnd->GetWindowRect(&rect);
			//���ͻ���ѡ�е��ؼ���ʾ�ľ���������
			ScreenToClient(&rect);
			//�����ƶ����ؼ���ʾ������
			pWnd->MoveWindow(rect.left, rect.top, cx, cy, TRUE);
			pWnd->GetClientRect(&rect);//��ȡ���ָ��ؼ�����Ĵ�С
			CDC *pDc = NULL;
			pDc = pWnd->GetDC();//��ȡpicture��DC
			//::CreateDIBSection()
			m_COMImage.StretchBlt(pDc->m_hDC, rect, SRCCOPY);//��ͼƬ���Ƶ�picture��ʾ��������
			//StretchBlt
			ReleaseDC(pDc);
		}
		CDialogEx::OnPaint();
	}
}


//���û��϶���С������ʱϵͳ���ô˺���ȡ�ù��
//��ʾ��
HCURSOR CTransportationDlg::OnQueryDragIcon()
{
	return static_cast<HCURSOR>(m_hIcon);
}


//��ʼ��CImage
bool CTransportationDlg::InitalImage(CImage &image, int width, int height)
{
	DWORD adwBitmasks[3] = { 0x0000F800, 0x000007E0, 0x0000001F };
	if (image.IsNull())
		image.CreateEx(width, height, 16, BI_BITFIELDS, adwBitmasks, 0);
	else
	{
		if (width <= 0 || height <= 0)
			return false;
		else if (image.GetHeight() == width && image.GetWidth() == height)
			return true;
		else
		{
			image.Destroy();
			image.CreateEx(width, height, 16, BI_BITFIELDS, adwBitmasks, 0);
		}
	}
	//д���ɫ��
	/*RGBQUAD ColorTable[256];
	image.GetColorTable(0, 256, ColorTable);
	for (int i = 0; i < 256; i++)
	{
		ColorTable[i].rgbBlue = (BYTE)i;
		ColorTable[i].rgbGreen = (BYTE)i;
		ColorTable[i].rgbRed = (BYTE)i;
	}
	image.SetColorTable(0, 256, ColorTable);*/
	return true;
}


//�򿪴���
void CTransportationDlg::OnBnClickedInitCom()
{
	// TODO:  �ڴ���ӿؼ�֪ͨ����������
	if (m_IsOpenCOM)
	{
		UpdateData(TRUE);		//���´���
		if (m_szCOMName == L"")
			return;
		UpdateData(TRUE);
		m_com.put_PortOpen(FALSE);
		GetDlgItem(IDC_INIT_COM)->SetWindowText(L"�򿪴���");
		m_IsOpenCOM = false;
		UpdateData(TRUE);
	}
	else
	{
		UpdateData(TRUE);
		if (m_szCOMName == L"")
			return;
		UpdateData(TRUE);
		if (m_com.get_PortOpen())
			m_com.put_PortOpen(FALSE);
		CString szNum = m_szCOMName;
		m_szCurretnCOMName = m_szCOMName;
		szNum.TrimLeft(L"COM");
		USES_CONVERSION;
		int num = atoi(W2A(szNum.GetBuffer()));
		m_com.put__CommPort(num);//ѡ��com1���ɸ��ݾ����������
		if (!m_com.get_PortOpen())
			m_com.put_PortOpen(TRUE);//�򿪴���
		else
			AfxMessageBox(L"can not open serial port");

		//m_com.put_InBufferSize(1024);//�������뻺�����Ĵ�С��Bytes
		//m_com.put_OutBufferSize(1024);//�������뻺�����Ĵ�С��Bytes
		CString str = m_szBaudTate + L", n, 8, 1";
		m_com.put_Settings(str);//������96000����У�飬8������λ��1��ֹͣλ
		m_com.put_InputMode(1);//1����ʾ�Զ����Ʒ�ʽ��ȡ����
		m_com.put_RThreshold(1);
		//����1��ʾÿ�����ڽ��ջ��������ж��ڻ����1���ַ�ʱ������һ���������ݵ�OnComm�¼�
		m_com.put_InputLen(0); //���õ�ǰ���������ݳ���Ϊ0����ʾȫ����ȡ
		GetDlgItem(IDC_INIT_COM)->SetWindowText(L"�رմ���");
		m_IsOpenCOM = true;
		UpdateData(FALSE);
	}
}


BEGIN_EVENTSINK_MAP(CTransportationDlg, CDialogEx)
	ON_EVENT(CTransportationDlg, IDC_MSCOMM1, 1, CTransportationDlg::OnOncommMscomm1, VTS_NONE)
END_EVENTSINK_MAP()


//AX�ؼ�����		�����ڷ�������ʱ�ͻᴥ��ִ��
void CTransportationDlg::OnOncommMscomm1()
{
	// TODO:  �ڴ˴������Ϣ����������
	VARIANT variant_inp;
	COleSafeArray safearray_inp;
	unsigned char * data = m_COMBits + m_COMIndex;		//int m_COMIndex = 0
	LONG len, k;
	CString strtemp;
	if (m_com.get_CommEvent() == 2) //�¼�ֵΪ2��ʾ���ջ����������ַ�  
	{
		variant_inp = m_com.get_Input(); //��������  
		safearray_inp = variant_inp; //VARIANT�ͱ���ת��ΪColeSafeArray�ͱ���  
		len = safearray_inp.GetOneDimSize(); //�õ���Ч���ݳ���
		for (k = 0; k < len; ++k, ++m_COMIndex)
		{
			if (m_COMIndex > 320 * 240 * 2 - 1)
				break;
			safearray_inp.GetElement(&k, data + k);//ת��ΪBYTE������
		}
		if (m_COMIndex > 320 * 240 * 2 - 1)
		{
			m_COMIndex = 0;
			m_hasCOMImage = true;
			LoadImageData(m_COMImage, m_COMBits);
			OnPaint();
			OnBnClickedSavePiccom();
		}
	}
}


//���洮������
void CTransportationDlg::LoadImageData(CImage &image, unsigned char * data)
{
	if (data == nullptr)
		return;
	byte *pS;
	byte *pImg = (byte *)image.GetBits();
	int step = image.GetPitch();		//-640 
	int height = image.GetHeight();		//240
	int width = image.GetWidth();		//320
	for (int i = 0; i < image.GetHeight(); ++i)
	{
		pS = data + i * width * 2;
		for (int j = 0; j < image.GetWidth()*2;++j)
		{
			*(pImg + (height - i - 1)*step + width * 2 - j) = pS[j];
		}
	}
}


//����ͼƬ
void CTransportationDlg::OnBnClickedSavePiccom()
{
	// TODO:  �ڴ���ӿؼ�֪ͨ����������
	//�ж��Ƿ���ڽ��ͼ��
	if (!m_hasCOMImage)
	{
		AfxMessageBox(L"���ڲ�����ͼ��");
		return;
	}

	//if (m_dlgSaveFile.DoModal() != IDOK)
	//	return;

	CString sTmp;
	CString sFileName;
	CTime t = CTime::GetCurrentTime();
	sTmp.Format(
		L"D://License-Plate-Recognition-master//Illegal//%d-%d-%d %d-%d-%d",
		t.GetYear(), t.GetMonth(), t.GetDay(), t.GetHour(), t.GetMinute(), t.GetSecond());
	sFileName = (LPCTSTR)(sTmp + ".jpeg");
	m_COMImage.Save(sFileName);

	//CString strFile;				//�ļ�ȫ��
	//strFile = m_dlgSaveFile.GetPathName();
	//strFile += L".bmp";
	//m_COMImage.Save(strFile);

	return;
}


//���ò�����
void CTransportationDlg::OnSelendokCombo1()
{
	// TODO:  �ڴ���ӿؼ�֪ͨ����������
	if (m_IsOpenCOM)
	{
		UpdateData(TRUE);
		CByteArray pause;
		unsigned char temp = 48 + m_BaudRateCtrl.GetCurSel();
		pause.Add(temp);
		m_com.put_Output(COleVariant(pause));
		if (m_com.get_PortOpen())
			m_com.put_PortOpen(FALSE);
		CString szNum = m_szCurretnCOMName;
		szNum.TrimLeft(L"COM");
		USES_CONVERSION;
		int num = atoi(W2A(szNum.GetBuffer()));
		m_com.put__CommPort(num);//ѡ��com1���ɸ��ݾ����������
		if (!m_com.get_PortOpen())
			m_com.put_PortOpen(TRUE);//�򿪴���
		else
			AfxMessageBox(L"can not open serial port");

		// 	m_com.put_InBufferSize(1024);//�������뻺�����Ĵ�С��Bytes
		// 	m_com.put_OutBufferSize(1024);//�������뻺�����Ĵ�С��Bytes
		CString str = m_szBaudTate + L", n, 8, 1";
		m_com.put_Settings(str);//������9600����У�飬8������λ��1��ֹͣλ
		m_com.put_InputMode(1);//1����ʾ�Զ����Ʒ�ʽ��ȡ����
		m_com.put_RThreshold(1);
		//����1��ʾÿ�����ڽ��ջ��������ж��ڻ����1���ַ�ʱ������һ���������ݵ�OnComm�¼�
		m_com.put_InputLen(0); //���õ�ǰ���������ݳ���Ϊ0
		UpdateData(FALSE);
	}
}


//��մ��ڻ���
void CTransportationDlg::OnBnClickedComClear()
{
	// TODO:  �ڴ���ӿؼ�֪ͨ����������
	m_COMIndex = 0;
}
