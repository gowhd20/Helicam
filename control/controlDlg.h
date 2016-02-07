
// controlDlg.h : header file
//
#include "Leap.h"
#include "Source.h"
#include "CvvImage.h"
#include "Socket.h"
#pragma once


// CcontrolDlg dialog
class CcontrolDlg : public CDialogEx
{
// Construction
public:
	CcontrolDlg(CWnd* pParent = NULL);	// standard constructor

// Dialog Data
	enum { IDD = IDD_CONTROL_DIALOG };

	protected:
	virtual void DoDataExchange(CDataExchange* pDX);	// DDX/DDV support

// Implementation
protected:
	HICON m_hIcon;

	// Generated message map functions
	virtual BOOL OnInitDialog();
	afx_msg void OnSysCommand(UINT nID, LPARAM lParam);
	afx_msg void OnPaint();
	afx_msg HCURSOR OnQueryDragIcon();
	DECLARE_MESSAGE_MAP()
public:
	afx_msg void OnBnClickedButton1();
	afx_msg void OnBnClickedButton2();

	HWND stcDirection;
	SampleListener listener;
	Controller controller;
	CWinThread *m_pThread;
	CWinThread *m_pThread_second;

	CString strDirection;
	CString strCommand;
	CvvImage m_cImage;
	IplImage* m_pImage;
	RECT rect;
	BOOL isRunning;
	BOOL isRunning_s;
	SSocket *soc;
	HWND hWndVideo;
	RECT rcDirection;
	RECT rcVideo;
	BOOL m_bDo;
	BOOL m_bDo_seoncd;
	CString strFps;
	bool isConnected;

protected:
	void StartThread();
	void StartSecondThread();
	void StopThread();
	void StopSecondThread();
	static UINT MyThreadFunc(LPVOID pThreadParam);
	static UINT MySecondThreadFunc(LPVOID pThreadParam);
	void MyWorkFunc();
	void MyWorkSecondFunc();
	
public:
	CString m_pDirction;
	afx_msg void OnBnClickedButton3();
};

typedef struct tagThreadParam
{
	CWnd *pWnd;
	BOOL *pDo;
	BOOL *pDo_second;
}THREADPARAM;
