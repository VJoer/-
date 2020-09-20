
// TransportationDlg.h : ͷ�ļ�
//

#pragma once
#include "afxwin.h"
#include "mscomm1.h"


// CTransportationDlg �Ի���
class CTransportationDlg : public CDialogEx
{
// ����
public:
	CTransportationDlg(CWnd* pParent = NULL);	// ��׼���캯��

// �Ի�������
	enum { IDD = IDD_TRANSPORTATION_DIALOG };

	protected:
	virtual void DoDataExchange(CDataExchange* pDX);	// DDX/DDV ֧��


// ʵ��
protected:
	HICON m_hIcon;

	// ���ɵ���Ϣӳ�亯��
	virtual BOOL OnInitDialog();
	afx_msg void OnPaint();
	afx_msg HCURSOR OnQueryDragIcon();
	DECLARE_MESSAGE_MAP()
public:
	// �򿪴���
	afx_msg void OnBnClickedInitCom();
	// �жϴ����Ƿ��
	bool m_IsOpenCOM;
	// ������
	CString m_szCOMName;
	// ����������
	CComboBox m_ComListCtrl;
	//�������д�����
	CString m_szCurretnCOMName;
	// AX�ؼ�
	CMscomm1 m_com;
	// AX�ؼ�����
	DECLARE_EVENTSINK_MAP()
	void OnOncommMscomm1();
	// ���ڵ�λͼ���� 
	int m_COMIndex;
	unsigned char m_COMBits[320 * 240 * 2];
	CImage m_COMImage;
	bool m_hasCOMImage;
	// ��ʼ��CImage
	bool InitalImage(CImage  &image, int width, int height);
	// ���洮������
	void LoadImageData(CImage &image, unsigned char * data);
	// �����ʿؼ�
	CComboBox m_BaudRateCtrl;
	// ����������
	CString m_szBaudTate;
	// ����ͼƬ
	afx_msg void OnBnClickedSavePiccom();
	CFileDialog m_dlgSaveFile;
	//���ò�����
	afx_msg void OnSelendokCombo1();
	//��մ���
	afx_msg void OnBnClickedComClear();
};
