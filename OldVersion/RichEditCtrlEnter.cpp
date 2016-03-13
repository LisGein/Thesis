// RichEditCtrlEnter.cpp : implementation file
//

#include "stdafx.h"
#include "ThermoCalc.h"
#include "RichEditCtrlEnter.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

/////////////////////////////////////////////////////////////////////////////
// CRichEditCtrlEnter

CRichEditCtrlEnter::CRichEditCtrlEnter()
{
}

CRichEditCtrlEnter::~CRichEditCtrlEnter()
{
}


BEGIN_MESSAGE_MAP(CRichEditCtrlEnter, CRichEditCtrl)
	//{{AFX_MSG_MAP(CRichEditCtrlEnter)
	ON_WM_CHAR()
	//}}AFX_MSG_MAP
END_MESSAGE_MAP()

/////////////////////////////////////////////////////////////////////////////
// CRichEditCtrlEnter message handlers

void CRichEditCtrlEnter::OnChar(UINT nChar, UINT nRepCnt, UINT nFlags) 
{
	// TODO: Add your message handler code here and/or call default
	
	CRichEditCtrl::OnChar(nChar, nRepCnt, nFlags);
}

LRESULT CRichEditCtrlEnter::WindowProc(UINT message, WPARAM wParam, LPARAM lParam) 
{
	// TODO: Add your specialized code here and/or call the base class
//	if ( message == WM_GETDLGCODE )
//      return DLGC_WANTALLKEYS;

	if (message==WM_KEYDOWN) {
		message=message;
	}

	if (message==WM_COMMAND) {
		message=message;
	}

	return CRichEditCtrl::WindowProc(message, wParam, lParam);
}
