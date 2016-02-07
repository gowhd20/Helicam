
// controlDlg.cpp : implementation file
//
#define _CRT_SECURE_NO_DEPRECATE
#include "stdafx.h"
#include "control.h"
#include "controlDlg.h"
#include "afxdialogex.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#endif

// CAboutDlg dialog used for App About

class CAboutDlg : public CDialogEx
{
public:
	CAboutDlg();

// Dialog Data
	enum { IDD = IDD_ABOUTBOX };

	protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV support

// Implementation
protected:
	DECLARE_MESSAGE_MAP()
};

CAboutDlg::CAboutDlg() : CDialogEx(CAboutDlg::IDD)
{
}

void CAboutDlg::DoDataExchange(CDataExchange* pDX)
{
	CDialogEx::DoDataExchange(pDX);
}

BEGIN_MESSAGE_MAP(CAboutDlg, CDialogEx)
END_MESSAGE_MAP()


// CcontrolDlg dialog


CcontrolDlg::CcontrolDlg(CWnd* pParent /*=NULL*/)
	: CDialogEx(CcontrolDlg::IDD, pParent)
	, m_pDirction(_T(""))
{
	controller.addListener(listener);
//	stcDirection = ::GetDlgItem(this->m_hWnd, IDC_STATIC1);
//	::GetClientRect(stcDirection, &rcDirection);
//	hWndVideo = ::GetDlgItem(this->m_hWnd, IDC_STATIC);
//	::GetClientRect(hWndVideo, &rcVideo);
	m_hIcon = AfxGetApp()->LoadIcon(IDR_MAINFRAME);
	soc = new SSocket;
	isRunning = false;
	isRunning_s = false;
	m_bDo = true;
	m_bDo_seoncd = true;
	strFps.Empty();
	strCommand.Empty();
	isConnected = false;
}

void CcontrolDlg::DoDataExchange(CDataExchange* pDX)
{
	CDialogEx::DoDataExchange(pDX);
	//DDX_Text(pDX, IDC_STATIC1, m_pDirction);
}

BEGIN_MESSAGE_MAP(CcontrolDlg, CDialogEx)
	ON_WM_SYSCOMMAND()
	ON_WM_PAINT()
	ON_WM_QUERYDRAGICON()
	ON_BN_CLICKED(IDC_BUTTON1, &CcontrolDlg::OnBnClickedButton1)
	ON_BN_CLICKED(IDC_BUTTON2, &CcontrolDlg::OnBnClickedButton2)
	ON_BN_CLICKED(IDC_BUTTON3, &CcontrolDlg::OnBnClickedButton3)
END_MESSAGE_MAP()


// CcontrolDlg message handlers

BOOL CcontrolDlg::OnInitDialog()
{
	CDialogEx::OnInitDialog();

	// Add "About..." menu item to system menu.

	// IDM_ABOUTBOX must be in the system command range.
	ASSERT((IDM_ABOUTBOX & 0xFFF0) == IDM_ABOUTBOX);
	ASSERT(IDM_ABOUTBOX < 0xF000);

	CMenu* pSysMenu = GetSystemMenu(FALSE);
	if (pSysMenu != NULL)
	{
		BOOL bNameValid;
		CString strAboutMenu;
		bNameValid = strAboutMenu.LoadString(IDS_ABOUTBOX);
		ASSERT(bNameValid);
		if (!strAboutMenu.IsEmpty())
		{
			pSysMenu->AppendMenu(MF_SEPARATOR);
			pSysMenu->AppendMenu(MF_STRING, IDM_ABOUTBOX, strAboutMenu);
		}
	}

	// Set the icon for this dialog.  The framework does this automatically
	//  when the application's main window is not a dialog
	SetIcon(m_hIcon, TRUE);			// Set big icon
	SetIcon(m_hIcon, FALSE);		// Set small icon

	// TODO: Add extra initialization here
	
	return TRUE;  // return TRUE  unless you set the focus to a control
}

void CcontrolDlg::OnSysCommand(UINT nID, LPARAM lParam)
{
	if ((nID & 0xFFF0) == IDM_ABOUTBOX)
	{
		CAboutDlg dlgAbout;
		dlgAbout.DoModal();
	}
	else
	{
		CDialogEx::OnSysCommand(nID, lParam);
	}

}

// If you add a minimize button to your dialog, you will need the code below
//  to draw the icon.  For MFC applications using the document/view model,
//  this is automatically done for you by the framework.

void CcontrolDlg::OnPaint()
{
	if (IsIconic())
	{
		CPaintDC dc(this); // device context for painting

		SendMessage(WM_ICONERASEBKGND, reinterpret_cast<WPARAM>(dc.GetSafeHdc()), 0);

		// Center icon in client rectangle
		int cxIcon = GetSystemMetrics(SM_CXICON);
		int cyIcon = GetSystemMetrics(SM_CYICON);
		CRect rect;
		GetClientRect(&rect);
		int x = (rect.Width() - cxIcon + 1) / 2;
		int y = (rect.Height() - cyIcon + 1) / 2;

		// Draw the icon
		dc.DrawIcon(x, y, m_hIcon);
	}
	else
	{
		if(m_pImage) {
			//m_bDo = false;
			CRect rt;
			CDC* pStaticDC = GetDlgItem(IDC_STATIC)->GetDC();
			GetDlgItem(IDC_STATIC)->GetClientRect(&rt);
			
			m_cImage.CopyOf(m_pImage);
			m_cImage.DrawToHDC(pStaticDC->m_hDC, &rt);
			ReleaseDC(pStaticDC);
			//m_bDo = true;
		}
		CDialogEx::OnPaint();
		strFps.Format(_T("%.3f"), soc->fps);
		SetDlgItemText(IDC_STATIC2, strFps);
	}
	strCommand = soc->buf_s;
	SetDlgItemText(IDC_STATIC5, strCommand);
	SetDlgItemText(IDC_STATIC1, listener.direction);
}

// The system calls this function to obtain the cursor to display while the user drags
//  the minimized window.
HCURSOR CcontrolDlg::OnQueryDragIcon()
{
	return static_cast<HCURSOR>(m_hIcon);
}



void CcontrolDlg::OnBnClickedButton1()
{
	
	switch(soc->udpConnect())
	{
	case 1:
		AfxMessageBox(L"connection has successfully made");
		GetDlgItem(IDC_BUTTON1)->SetWindowText(_T("connected"));
		isConnected = true;
		break;
	case 2:
		AfxMessageBox(L"Could not create socket");
		break;
	case 3:
		AfxMessageBox(L"Bind failed");
		break;
	case 0:
		AfxMessageBox(L"Failed");
		break;
	}
	
	soc->udpConnectForCommand();
}


void CcontrolDlg::OnBnClickedButton2()
{
	if(isConnected == false)
		AfxMessageBox(L"no network is connected");
	else{
	//	if(soc->isDataReceived == true){
			//AfxMessageBox(L"real-time video streaming");
			CcontrolDlg::StartThread();
			CcontrolDlg::StartSecondThread();
			GetDlgItem(IDC_BUTTON2)->SetWindowText(_T("streaming"));
			isRunning = true;
			isRunning_s = true;
		}
		//else
		//	AfxMessageBox(L"no client is streaming");
	//}

	//IplImage* objectImg = cvLoadImage("anna.jpg", CV_LOAD_IMAGE_COLOR);
	//cv::Mat imgMat = cv::imread("anna.jpg", 3);
	//cv::Mat deimgMat = cv::Mat(1050,1400,CV_8U, &imgMat);
	//const static int encodeParams[] = { CV_IMWRITE_JPEG_QUALITY, 30 };
	//cv::vector<uchar> buf[BUFLEN];
	//cv::vector<uchar> afterbuf[BUFLEN];
	//cv::imencode(".jpg", imgMat, *buf, std::vector<int>());
	//deimgMat=cv::imdecode(*buf, 3);

	//m_pImage = cvCloneImage(&(IplImage)deimgMat);
	//Invalidate(FALSE);
}

void CcontrolDlg::OnBnClickedButton3()
{
	if(isRunning == true){
		isRunning == false;
		m_pThread->SuspendThread();
		GetDlgItem(IDC_BUTTON3)->SetWindowText(_T("stopped"));
		GetDlgItem(IDC_BUTTON2)->SetWindowText(_T("begin to stream"));
	}
	else{
		m_pThread->ResumeThread();
		GetDlgItem(IDC_BUTTON3)->SetWindowText(_T("stop to stream"));
		//AfxMessageBox(L"");
	}
}



// ------------------------------thread related functions ----------------------------------
UINT CcontrolDlg::MyThreadFunc(LPVOID pThreadParam)
{
	//THREADPARAM *pParam = (THREADPARAM*)pThreadParam;
	CcontrolDlg *pMyDlg = (CcontrolDlg*)pThreadParam;
	
	//delete pParam;

	while(pMyDlg->m_bDo)
	{
		pMyDlg->MyWorkFunc();
		//Sleep(30);
	}
	return 0;
}

UINT CcontrolDlg::MySecondThreadFunc(LPVOID pThreadParam)
{
	//THREADPARAM *pParam = (THREADPARAM*)pThreadParam;
	CcontrolDlg *pMyDlg = (CcontrolDlg*)pThreadParam;
	
	//delete pParam;

	while(pMyDlg->m_bDo_seoncd)
	{
		pMyDlg->MyWorkSecondFunc();
		//Sleep(30);
	}
	return 0;
}

void CcontrolDlg::StartThread()
{
	THREADPARAM *pThreadParam = new THREADPARAM;
	pThreadParam->pWnd = this;
	pThreadParam->pDo = &m_bDo;

	m_pThread = AfxBeginThread(MyThreadFunc, this); // 스레드 실행
}

void CcontrolDlg::StartSecondThread()
{
	THREADPARAM *pThreadParam = new THREADPARAM;
	pThreadParam->pWnd = this;
	pThreadParam->pDo_second = &m_bDo_seoncd;

	m_pThread_second = AfxBeginThread(MySecondThreadFunc, this); // 스레드 실행
}

void CcontrolDlg::StopThread()
{
	m_bDo = FALSE;
	Sleep(100);

	if(::WaitForSingleObject(m_pThread, INFINITE))
	{
		m_pThread = NULL;
	}
}

void CcontrolDlg::StopSecondThread()
{
	m_bDo_seoncd = FALSE;
	Sleep(100);

	if(::WaitForSingleObject(m_pThread_second, INFINITE))
	{
		m_pThread_second = NULL;
	}
}

//반복 호출 함수
void CcontrolDlg::MyWorkFunc()
{	
	soc->camPlay();
	//soc->getFPS();
	m_pImage = soc->img;
	Invalidate(FALSE);
}

//반복 호출 함수
void CcontrolDlg::MyWorkSecondFunc()
{	
	soc->receiveCommand();
}
