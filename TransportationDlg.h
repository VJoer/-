
// TransportationDlg.h : 头文件
//

#pragma once
#include "afxwin.h"
#include "mscomm1.h"


// CTransportationDlg 对话框
class CTransportationDlg : public CDialogEx
{
// 构造
public:
	CTransportationDlg(CWnd* pParent = NULL);	// 标准构造函数

// 对话框数据
	enum { IDD = IDD_TRANSPORTATION_DIALOG };

	protected:
	virtual void DoDataExchange(CDataExchange* pDX);	// DDX/DDV 支持


// 实现
protected:
	HICON m_hIcon;

	// 生成的消息映射函数
	virtual BOOL OnInitDialog();
	afx_msg void OnPaint();
	afx_msg HCURSOR OnQueryDragIcon();
	DECLARE_MESSAGE_MAP()
public:
	// 打开串口
	afx_msg void OnBnClickedInitCom();
	// 判断串口是否打开
	bool m_IsOpenCOM;
	// 串口名
	CString m_szCOMName;
	// 串口下拉框
	CComboBox m_ComListCtrl;
	//下拉框中串口名
	CString m_szCurretnCOMName;
	// AX控件
	CMscomm1 m_com;
	// AX控件函数
	DECLARE_EVENTSINK_MAP()
	void OnOncommMscomm1();
	// 串口的位图数据 
	int m_COMIndex;
	unsigned char m_COMBits[320 * 240 * 2];
	CImage m_COMImage;
	bool m_hasCOMImage;
	// 初始化CImage
	bool InitalImage(CImage  &image, int width, int height);
	// 保存串口数据
	void LoadImageData(CImage &image, unsigned char * data);
	// 波特率控件
	CComboBox m_BaudRateCtrl;
	// 波特率数字
	CString m_szBaudTate;
	// 保存图片
	afx_msg void OnBnClickedSavePiccom();
	CFileDialog m_dlgSaveFile;
	//设置波特率
	afx_msg void OnSelendokCombo1();
	//清空串口
	afx_msg void OnBnClickedComClear();
};
