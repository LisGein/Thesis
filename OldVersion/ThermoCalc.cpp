// ThermoCalc.cpp : Defines the class behaviors for the application.
//

#include "stdafx.h"
#include "ThermoCalc.h"

#include "MainFrm.h"
#include "ChildFrm.h"
#include "ThermoCalcDoc.h"
#include "DlgEmptyDoc.h"
#include "ThermoCalcView.h"
#include "ViewOutput.h"
#include "FrameOutput.h"
#include "Calc.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

CLimitString sSaveAsExcel;
/////////////////////////////////////////////////////////////////////////////
// CThermoCalcApp

BEGIN_MESSAGE_MAP(CThermoCalcApp, CWinApp)
	//{{AFX_MSG_MAP(CThermoCalcApp)
	ON_COMMAND(ID_APP_ABOUT, OnAppAbout)
		// NOTE - the ClassWizard will add and remove mapping macros here.
		//    DO NOT EDIT what you see in these blocks of generated code!
	//}}AFX_MSG_MAP
	// Standard file based document commands
	ON_COMMAND(ID_FILE_NEW, CWinApp::OnFileNew)
	ON_COMMAND(ID_FILE_OPEN, CWinApp::OnFileOpen)
	// Standard print setup command
	ON_COMMAND(ID_FILE_PRINT_SETUP, CWinApp::OnFilePrintSetup)
END_MESSAGE_MAP()

/////////////////////////////////////////////////////////////////////////////
// CThermoCalcApp construction

CThermoCalcApp::CThermoCalcApp()
{
	// TODO: add construction code here,
	// Place all significant initialization in InitInstance
}

/////////////////////////////////////////////////////////////////////////////
// The one and only CThermoCalcApp object

CThermoCalcApp theApp;

/////////////////////////////////////////////////////////////////////////////
// CThermoCalcApp initialization

BOOL CThermoCalcApp::InitInstance()
{
	AfxEnableControlContainer();

	// Initialize RichEditCtrl
	BOOL bRV = AfxInitRichEdit();
	if (!bRV) {
		AfxMessageBox("'RichEditCtrl' Initialize failed");
		return FALSE;
	}

	// Initialize OLE libraries
	if (!AfxOleInit())
	{
		AfxMessageBox("OLE initialization failed.  Make sure that the OLE libraries are the correct version.");
		return FALSE;
	}

	// Standard initialization
	// If you are not using these features and wish to reduce the size
	//  of your final executable, you should remove from the following
	//  the specific initialization routines you do not need.

#ifdef _AFXDLL
	Enable3dControls();			// Call this when using MFC in a shared DLL
#else
	Enable3dControlsStatic();	// Call this when linking to MFC statically
#endif

	// Change the registry key under which our settings are stored.
	// TODO: You should modify this string to be something appropriate
	// such as the name of your company or organization.
	SetRegistryKey(_T("Local AppWizard-Generated Applications"));

	LoadStdProfileSettings();  // Load standard INI file options (including MRU)

	// Register the application's document templates.  Document templates
	//  serve as the connection between documents, frame windows and views.
//*
	CMultiDocTemplate* pDocTemplateDlg;
	pDocTemplateDlg = new CMultiDocTemplate(
		IDR_INPUTDLG,
		RUNTIME_CLASS(DlgEmptyDoc),
		RUNTIME_CLASS(CChildFrame), // custom MDI child frame
		RUNTIME_CLASS(CThermoCalcView));
	AddDocTemplate(pDocTemplateDlg);
//*/
	CMultiDocTemplate* pDocTemplateOutput;
	pDocTemplateOutput = new CMultiDocTemplate(
		IDR_OUTPUT,
		RUNTIME_CLASS(CThermoCalcDoc),
		RUNTIME_CLASS(CFrameOutput), // custom MDI child frame
		RUNTIME_CLASS(CViewOutput));
	AddDocTemplate(pDocTemplateOutput);


	// create main MDI Frame window
	CMainFrame* pMainFrame = new CMainFrame;
	if (!pMainFrame->LoadFrame(IDR_MAINFRAME))
		return FALSE;
	m_pMainWnd = pMainFrame;

/*
	CChildFrame *pDF = new CChildFrame;
	if (!pDF->LoadFrame(IDR_THERMOTYPE, WS_CHILD, pMainFrame, NULL))
		return FALSE;
	pDlgFrame = pDF;
	CThermoCalcView *pV = new CThermoCalcView;
	//CRuntimeClass *pRC = RUNTIME_CLASS(CThermoCalcView);
	//CObject * pCO = pRC -> CreateObject();
	CWnd * pW = (CWnd *)pV;
	//HWND h = pW->GetSafeHwnd( );
	//CThermoCalcView * pTCV = (CThermoCalcView *)pCO;
	const RECT R = {50,50,100,100};
	pW->Create("CFormView", "IsWindowName", WS_CHILD, R, pMainFrame, IDR_THERMOTYPE, NULL);
// CFormView// CWnd
	//pDF -> SetActiveView((CThermoCalcView *)pV,TRUE);
	pV -> ShowWindow(SW_SHOW);
	pV -> UpdateWindow();
	//pDF-> MDIActivate();
	pDF -> ShowWindow(SW_SHOW);
	pDF -> UpdateWindow();
*/

	pDocTemplateDlg -> OpenDocumentFile(NULL);

	POSITION pos = GetFirstDocTemplatePosition();
	CDocTemplate* pCDT = GetNextDocTemplate(pos);

	POSITION pos2 = pCDT->GetFirstDocPosition();
	CDocument* pD = pCDT->GetNextDoc(pos2);
	CThermoCalcDoc * pMyDoc = (CThermoCalcDoc *)pD;
	CString S;
	enum CDocTemplate::DocStringIndex i;
	i = CDocTemplate::fileNewName;
	BOOL bR = pCDT->GetDocString(S,i);

	CRect _rDlgRect = 0;
	if (bR) {
		if (S == "EmptyDlgDoc") {
			POSITION posV = pMyDoc ->GetFirstViewPosition();
			CThermoCalcView* pTCV = (CThermoCalcView *)(pMyDoc ->GetNextView(posV));
			ASSERT(posV == 0);	// У этого документа один вид, что важно
			(pTCV->GetParentFrame())->GetWindowRect(_rDlgRect);
			pMainFrame ->SetMainDlgParams((CChildFrame*)pTCV->GetParentFrame(), pTCV, _rDlgRect);
		}
	}
//	pMainFrame -> GetDlgView();
	pMainFrame -> SetMultiDocTemplates(pDocTemplateDlg, pDocTemplateOutput);

	//pDocTemplateOutput->OpenDocumentFile(NULL);
/*	CDocTemplate* pTemplate = (CDocTemplate*)m_templateList.GetHead();
	pTemplate->OpenDocumentFile(NULL);
	// Parse command line for standard shell commands, DDE, file open
	CCommandLineInfo cmdInfo;
	ParseCommandLine(cmdInfo);
	// Dispatch commands specified on the command line
	if (!ProcessShellCommand(cmdInfo))
		return FALSE;*/
	// The main window has been initialized, so show and update it.
	pMainFrame ->ShowWindow(m_nCmdShow);
//	pMainFrame ->UpdateWindow();
	pMainFrame ->CenterWindow();

	return TRUE;
}

/////////////////////////////////////////////////////////////////////////////
// CAboutDlg dialog used for App About

class CAboutDlg : public CDialog
{
public:
	CAboutDlg();

// Dialog Data
	//{{AFX_DATA(CAboutDlg)
	enum { IDD = IDD_ABOUTBOX };
	//}}AFX_DATA

	// ClassWizard generated virtual function overrides
	//{{AFX_VIRTUAL(CAboutDlg)
	protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV support
	//}}AFX_VIRTUAL

// Implementation
protected:
	//{{AFX_MSG(CAboutDlg)
		// No message handlers
	//}}AFX_MSG
	DECLARE_MESSAGE_MAP()
};

CAboutDlg::CAboutDlg() : CDialog(CAboutDlg::IDD)
{
	//{{AFX_DATA_INIT(CAboutDlg)
	//}}AFX_DATA_INIT
}

void CAboutDlg::DoDataExchange(CDataExchange* pDX)
{
	CDialog::DoDataExchange(pDX);
	//{{AFX_DATA_MAP(CAboutDlg)
	//}}AFX_DATA_MAP
}

BEGIN_MESSAGE_MAP(CAboutDlg, CDialog)
	//{{AFX_MSG_MAP(CAboutDlg)
		// No message handlers
	//}}AFX_MSG_MAP
END_MESSAGE_MAP()

// App command to run the dialog
void CThermoCalcApp::OnAppAbout()
{
	CAboutDlg aboutDlg;
	aboutDlg.DoModal();
}

/////////////////////////////////////////////////////////////////////////////
// CThermoCalcApp message handlers
