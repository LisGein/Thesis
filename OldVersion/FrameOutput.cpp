// FrameOutput.cpp : implementation file
//

#include "stdafx.h"
#include "MainFrm.h"
#include "FrameOutput.h"
#include "ThermoCalc.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

/////////////////////////////////////////////////////////////////////////////
// CFrameOutput

IMPLEMENT_DYNCREATE(CFrameOutput, CMDIChildWnd)

CFrameOutput::CFrameOutput(){}
CFrameOutput::~CFrameOutput(){}

BEGIN_MESSAGE_MAP(CFrameOutput, CMDIChildWnd)
	//{{AFX_MSG_MAP(CFrameOutput)
	ON_WM_SIZE()
	//}}AFX_MSG_MAP
END_MESSAGE_MAP()

/////////////////////////////////////////////////////////////////////////////
// CFrameOutput message handlers

BOOL CFrameOutput::PreCreateWindow(CREATESTRUCT& cs) 
{
	// TODO: Add your specialized code here and/or call the base class
	RECT R;
	CWnd * pCW = AfxGetMainWnd();
	((CMainFrame *)pCW) -> GetMainDlgRect(R);

	cs.x = 0;
	cs.y = 0;
	if (constOutputWndWidth)
		cs.cx = constOutputWndWidth;
	else
		cs.cx = R.right - R.left;
	cs.cy = constOutputWndHigh;

//	cs.style |= WS_HSCROLL;
//	cs.style |= WS_VSCROLL;
	cs.style |= ES_AUTOHSCROLL;
	cs.style |= ES_AUTOVSCROLL;
	cs.style |= ES_NOHIDESEL;

	return CMDIChildWnd::PreCreateWindow(cs);
}

void CFrameOutput::OnSize(UINT nType, int cx, int cy) 
{
	CMDIChildWnd::OnSize(nType, cx, cy);
	// TODO: Add your message handler code here	
}
