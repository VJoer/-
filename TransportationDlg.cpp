
// TransportationDlg.cpp : 实现文件
//

#include "stdafx.h"
#include "Transportation.h"
#include "TransportationDlg.h"
#include "afxdialogex.h"

#include "cstringt.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#endif


// CTransportationDlg 对话框


CTransportationDlg::CTransportationDlg(CWnd* pParent /*=NULL*/)
	: CDialogEx(CTransportationDlg::IDD, pParent)
	//初始化
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


// CTransportationDlg 消息处理程序

BOOL CTransportationDlg::OnInitDialog()
{
	CDialogEx::OnInitDialog();

	// 设置此对话框的图标。  当应用程序主窗口不是对话框时，框架将自动
	//  执行此操作
	SetIcon(m_hIcon, TRUE);			// 设置大图标
	SetIcon(m_hIcon, FALSE);		// 设置小图标

	// TODO:  在此添加额外的初始化代码
	//初始化CImage，写入文件头，信息头
	InitalImage(m_COMImage, 320, 240);

	//串口部分
	HANDLE fd;
	char i = 1;
	wchar_t com_name[10];
	m_ComListCtrl.ResetContent();		//列表中移除所有项
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
	if (m_ComListCtrl.GetCount() > 0)		//返回对象数列表中的
	{
		m_ComListCtrl.SetCurSel(0);		//选择在组合框的列表框中的字符串
		//检索指定的子窗口
		GetDlgItem(IDC_INIT_COM)->\
		//指定特定窗口是否将启用或禁用。 如果此参数是 TRUE，窗口将启用。 如果此参数是 FALSE，窗口将被禁用
		EnableWindow(TRUE);
	}
	else
		GetDlgItem(IDC_INIT_COM)->EnableWindow(FALSE);
 	if (m_ComListCtrl.GetCount() == 1)
 	{
 		OnBnClickedInitCom();
 	}
	m_BaudRateCtrl.SetCurSel(0);

	return TRUE;  // 除非将焦点设置到控件，否则返回 TRUE
}

// 如果向对话框添加最小化按钮，则需要下面的代码
//  来绘制该图标。  对于使用文档/视图模型的 MFC 应用程序，
//  这将由框架自动完成。

void CTransportationDlg::OnPaint()
{
	int cx, cy;
	CRect	rect;
	CWnd *pWnd = NULL;
	if (IsIconic())
	{
		CPaintDC dc(this); // 用于绘制的设备上下文

		SendMessage(WM_ICONERASEBKGND, reinterpret_cast<WPARAM>(dc.GetSafeHdc()), 0);

		// 使图标在工作区矩形中居中
		int cxIcon = GetSystemMetrics(SM_CXICON);
		int cyIcon = GetSystemMetrics(SM_CYICON);
		CRect rect;
		GetClientRect(&rect);
		int x = (rect.Width() - cxIcon + 1) / 2;
		int y = (rect.Height() - cyIcon + 1) / 2;

		// 绘制图标
		dc.DrawIcon(x, y, m_hIcon);
	}
	else
	{
		//确认对话框数据中是否有一张完整的图像		绘制
		if (m_hasCOMImage && !m_COMImage.IsNull())
		{
			//获取图片的宽 高度
			cx = m_COMImage.GetWidth();
			cy = m_COMImage.GetHeight();
			//ResizeWindow(cx, cy);
			//获取IDC_PIC1的窗口指针
			pWnd = GetDlgItem(IDC_PICCOM);
			//获取Picture Control控件的大小
			pWnd->GetWindowRect(&rect);
			//将客户区选中到控件表示的矩形区域内
			ScreenToClient(&rect);
			//窗口移动到控件表示的区域
			pWnd->MoveWindow(rect.left, rect.top, cx, cy, TRUE);
			pWnd->GetClientRect(&rect);//获取句柄指向控件区域的大小
			CDC *pDc = NULL;
			pDc = pWnd->GetDC();//获取picture的DC
			//::CreateDIBSection()
			m_COMImage.StretchBlt(pDc->m_hDC, rect, SRCCOPY);//将图片绘制到picture表示的区域内
			//StretchBlt
			ReleaseDC(pDc);
		}
		CDialogEx::OnPaint();
	}
}


//当用户拖动最小化窗口时系统调用此函数取得光标
//显示。
HCURSOR CTransportationDlg::OnQueryDragIcon()
{
	return static_cast<HCURSOR>(m_hIcon);
}


//初始化CImage
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
	//写入调色板
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


//打开串口
void CTransportationDlg::OnBnClickedInitCom()
{
	// TODO:  在此添加控件通知处理程序代码
	if (m_IsOpenCOM)
	{
		UpdateData(TRUE);		//更新窗口
		if (m_szCOMName == L"")
			return;
		UpdateData(TRUE);
		m_com.put_PortOpen(FALSE);
		GetDlgItem(IDC_INIT_COM)->SetWindowText(L"打开串口");
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
		m_com.put__CommPort(num);//选择com1，可根据具体情况更改
		if (!m_com.get_PortOpen())
			m_com.put_PortOpen(TRUE);//打开串口
		else
			AfxMessageBox(L"can not open serial port");

		//m_com.put_InBufferSize(1024);//设置输入缓冲区的大小，Bytes
		//m_com.put_OutBufferSize(1024);//设置输入缓冲区的大小，Bytes
		CString str = m_szBaudTate + L", n, 8, 1";
		m_com.put_Settings(str);//波特率96000，无校验，8个数据位，1个停止位
		m_com.put_InputMode(1);//1：表示以二进制方式检取数据
		m_com.put_RThreshold(1);
		//参数1表示每当串口接收缓冲区中有多于或等于1个字符时将引发一个接收数据的OnComm事件
		m_com.put_InputLen(0); //设置当前接收区数据长度为0，表示全部读取
		GetDlgItem(IDC_INIT_COM)->SetWindowText(L"关闭串口");
		m_IsOpenCOM = true;
		UpdateData(FALSE);
	}
}


BEGIN_EVENTSINK_MAP(CTransportationDlg, CDialogEx)
	ON_EVENT(CTransportationDlg, IDC_MSCOMM1, 1, CTransportationDlg::OnOncommMscomm1, VTS_NONE)
END_EVENTSINK_MAP()


//AX控件函数		当串口发送数据时就会触发执行
void CTransportationDlg::OnOncommMscomm1()
{
	// TODO:  在此处添加消息处理程序代码
	VARIANT variant_inp;
	COleSafeArray safearray_inp;
	unsigned char * data = m_COMBits + m_COMIndex;		//int m_COMIndex = 0
	LONG len, k;
	CString strtemp;
	if (m_com.get_CommEvent() == 2) //事件值为2表示接收缓冲区内有字符  
	{
		variant_inp = m_com.get_Input(); //读缓冲区  
		safearray_inp = variant_inp; //VARIANT型变量转换为ColeSafeArray型变量  
		len = safearray_inp.GetOneDimSize(); //得到有效数据长度
		for (k = 0; k < len; ++k, ++m_COMIndex)
		{
			if (m_COMIndex > 320 * 240 * 2 - 1)
				break;
			safearray_inp.GetElement(&k, data + k);//转换为BYTE型数组
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


//保存串口数据
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


//保存图片
void CTransportationDlg::OnBnClickedSavePiccom()
{
	// TODO:  在此添加控件通知处理程序代码
	//判断是否存在结果图像
	if (!m_hasCOMImage)
	{
		AfxMessageBox(L"串口不存在图像");
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

	//CString strFile;				//文件全名
	//strFile = m_dlgSaveFile.GetPathName();
	//strFile += L".bmp";
	//m_COMImage.Save(strFile);

	return;
}


//设置波特率
void CTransportationDlg::OnSelendokCombo1()
{
	// TODO:  在此添加控件通知处理程序代码
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
		m_com.put__CommPort(num);//选择com1，可根据具体情况更改
		if (!m_com.get_PortOpen())
			m_com.put_PortOpen(TRUE);//打开串口
		else
			AfxMessageBox(L"can not open serial port");

		// 	m_com.put_InBufferSize(1024);//设置输入缓冲区的大小，Bytes
		// 	m_com.put_OutBufferSize(1024);//设置输入缓冲区的大小，Bytes
		CString str = m_szBaudTate + L", n, 8, 1";
		m_com.put_Settings(str);//波特率9600，无校验，8个数据位，1个停止位
		m_com.put_InputMode(1);//1：表示以二进制方式检取数据
		m_com.put_RThreshold(1);
		//参数1表示每当串口接收缓冲区中有多于或等于1个字符时将引发一个接收数据的OnComm事件
		m_com.put_InputLen(0); //设置当前接收区数据长度为0
		UpdateData(FALSE);
	}
}


//清空串口缓存
void CTransportationDlg::OnBnClickedComClear()
{
	// TODO:  在此添加控件通知处理程序代码
	m_COMIndex = 0;
}
