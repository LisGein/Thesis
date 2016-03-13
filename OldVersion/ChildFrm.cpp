// ChildFrm.cpp : implementation of the CChildFrame class
//

#include "stdafx.h"
#include "ThermoCalc.h"

#include "ThermoCalcDoc.h"
#include "ChildFrm.h"
#include "ViewOutput.h"
#include "FrameOutput.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

/////////////////////////////////////////////////////////////////////////////
// CChildFrame

IMPLEMENT_DYNCREATE(CChildFrame, CMDIChildWnd)

BEGIN_MESSAGE_MAP(CChildFrame, CMDIChildWnd)
	//{{AFX_MSG_MAP(CChildFrame)
		// NOTE - the ClassWizard will add and remove mapping macros here.
		//    DO NOT EDIT what you see in these blocks of generated code !
	//}}AFX_MSG_MAP
END_MESSAGE_MAP()

/////////////////////////////////////////////////////////////////////////////
// CChildFrame construction/destruction

CChildFrame::CChildFrame() {
	// TODO: add member initialization code here
}

CChildFrame::~CChildFrame() {}

BOOL CChildFrame::PreCreateWindow(CREATESTRUCT& cs) {
	// TODO: Modify the Window class or styles here by modifying
	//  the CREATESTRUCT cs
	cs.x = 0;
	cs.y = 0;
//	cs.style = NULL;
//	cs.style = WS_CHILD;
//	cs.style = WS_CHILD | WS_BORDER;
//	cs.style = WS_CHILD | WS_DLGFRAME;
	cs.style = WS_CHILD | WS_THICKFRAME;

	cs.dwExStyle = NULL;
//	cs.dwExStyle = WS_EX_MDICHILD;
//	cs.dwExStyle = WS_EX_MDICHILD | WS_EX_CLIENTEDGE;
//	cs.dwExStyle = WS_EX_CLIENTEDGE;
//	cs.dwExStyle = WS_EX_STATICEDGE;
//	cs.dwExStyle = WS_EX_STATICEDGE | WS_EX_CLIENTEDGE;
//	cs.dwExStyle = WS_EX_WINDOWEDGE;
	//cs.dwExStyle = WS_EX_CLIENTEDGE;
	if( !CMDIChildWnd::PreCreateWindow(cs) )
		return FALSE;
	return TRUE;
}



/////////////////////////////////////////////////////////////////////////////
// CChildFrame diagnostics

#ifdef _DEBUG
void CChildFrame::AssertValid() const
{
	CMDIChildWnd::AssertValid();
}

void CChildFrame::Dump(CDumpContext& dc) const
{
	CMDIChildWnd::Dump(dc);
}

#endif //_DEBUG

/////////////////////////////////////////////////////////////////////////////
// CChildFrame message handlers
