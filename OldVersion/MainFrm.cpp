// MainFrm.cpp : implementation of the CMainFrame class
//

#include "stdafx.h"
#include "ThermoCalc.h"

#include "MainFrm.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

/////////////////////////////////////////////////////////////////////////////
// CMainFrame

IMPLEMENT_DYNAMIC(CMainFrame, CMDIFrameWnd)

BEGIN_MESSAGE_MAP(CMainFrame, CMDIFrameWnd)
	//{{AFX_MSG_MAP(CMainFrame)
		// NOTE - the ClassWizard will add and remove mapping macros here.
		//    DO NOT EDIT what you see in these blocks of generated code !
	ON_WM_CREATE()
	//}}AFX_MSG_MAP
END_MESSAGE_MAP()

static UINT indicators[] =
{
	ID_SEPARATOR,           // status line indicator
	ID_INDICATOR_CAPS,
//	ID_INDICATOR_NUM,
//	ID_INDICATOR_SCRL,
};

/////////////////////////////////////////////////////////////////////////////
// CMainFrame construction/destruction

CMainFrame::CMainFrame()
{
	// TODO: add member initialization code here
	_pDlgFrame = NULL;
	_rDlgRect = 0;
	_pDlgView = 0;
	_pDocTemplateDlg = 0;
	_pDocTemplateOutput = 0;
}

CMainFrame::~CMainFrame()
{
}

int CMainFrame::OnCreate(LPCREATESTRUCT lpCreateStruct)
{
	if (CMDIFrameWnd::OnCreate(lpCreateStruct) == -1)
		return -1;

	if (!m_wndStatusBar.Create(this) ||
		!m_wndStatusBar.SetIndicators(indicators,
		  sizeof(indicators)/sizeof(UINT)))
	{
		TRACE0("Failed to create status bar\n");
		return -1;      // fail to create
	}

	return 0;
}

BOOL CMainFrame::PreCreateWindow(CREATESTRUCT& cs)
{
	if( !CMDIFrameWnd::PreCreateWindow(cs) )
		return FALSE;
	// TODO: Modify the Window class or styles here by modifying
	//  the CREATESTRUCT cs
	//CWnd * pCW = AfxGetMainWnd();
	//CWinApp* pCWA = AfxGetApp( );
	//((CThermoCalcApp * )pCWA) -> pDlgFrame -> MDIActivate();
	// ј может загрузить из ресурсов и измерить?
	// Resources
	cs.cx = 780;
	cs.cy = 600;
//	cs.style |= WS_THICKFRAME;
//	cs.dwExStyle |= WS_EX_LEFTSCROLLBAR;
	return TRUE;
}

/////////////////////////////////////////////////////////////////////////////
// CMainFrame diagnostics

#ifdef _DEBUG
void CMainFrame::AssertValid() const
{
	CMDIFrameWnd::AssertValid();
}

void CMainFrame::Dump(CDumpContext& dc) const
{
	CMDIFrameWnd::Dump(dc);
}

#endif //_DEBUG

/////////////////////////////////////////////////////////////////////////////
// CMainFrame message handlers

void CMainFrame::GetMainDlgRect(RECT &R)
{
	R = _rDlgRect;	//R->left = rDlgRect.left;	//R->right = rDlgRect.right;	//R->top = rDlgRect.top;	//R->bottom = rDlgRect.bottom;
}

void CMainFrame::SetMainDlgParams(CChildFrame* e_pDlgFrame, CThermoCalcView* e_pDlgView, 
		RECT &e_R)
{
	_rDlgRect = e_R;
	_pDlgView = e_pDlgView;
	_pDlgFrame = e_pDlgFrame;
}

void CMainFrame::SetMultiDocTemplates(CMultiDocTemplate* e_pDocTemplateDlg, 
		CMultiDocTemplate* e_pDocTemplateOutput)
{
	_pDocTemplateDlg = e_pDocTemplateDlg;
	_pDocTemplateOutput = e_pDocTemplateOutput;
}

CMultiDocTemplate* CMainFrame::GetDlgDocTemplate()
{	return _pDocTemplateDlg;	}

CMultiDocTemplate* CMainFrame::GetOutputDocTemplate()
{	return _pDocTemplateOutput;	}

CThermoCalcView * CMainFrame::GetDlgView()
{	return _pDlgView;	}
