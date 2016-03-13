// PropertyPageChem.cpp : implementation file
//

#include "stdafx.h"
#include "ThermoCalc.h"
#include "PropertyPageChem.h"
#include "ThermoCalcView.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

/////////////////////////////////////////////////////////////////////////////
// CPropertyPageChem property page

IMPLEMENT_DYNCREATE(CPropertyPageChem, CPropertyPage)

CPropertyPageChem::CPropertyPageChem() : CPropertyPage(CPropertyPageChem::IDD) {
	//{{AFX_DATA_INIT(CPropertyPageChem)
		// NOTE: the ClassWizard will add member initialization here
	//}}AFX_DATA_INIT
	bFocus = FALSE;
}

CPropertyPageChem::~CPropertyPageChem() {	}

void CPropertyPageChem::DoDataExchange(CDataExchange* pDX) {
	CPropertyPage::DoDataExchange(pDX);
	//{{AFX_DATA_MAP(CPropertyPageChem)
		// NOTE: the ClassWizard will add DDX and DDV calls here
	//}}AFX_DATA_MAP
}

BEGIN_MESSAGE_MAP(CPropertyPageChem, CPropertyPage)
	//{{AFX_MSG_MAP(CPropertyPageChem)
	ON_WM_KILLFOCUS()
	//}}AFX_MSG_MAP
END_MESSAGE_MAP()

/////////////////////////////////////////////////////////////////////////////
// CPropertyPageChem message handlers

void CPropertyPageChem::OnKillFocus(CWnd* pNewWnd) {
	bFocus = FALSE;
	CPropertyPage::OnKillFocus(pNewWnd);
	// TODO: Add your message handler code here	
}

BOOL CPropertyPageChem::OnSetActive() {
	// TODO: Add your specialized code here and/or call the base class
	bFocus = TRUE;
	BOOL b = CPropertyPage::OnSetActive();
	GetParent()->SetFocus();
	return b;
}

BOOL CPropertyPageChem::OnCommand(WPARAM wParam, LPARAM lParam) 
{
	// TODO: Add your specialized code here and/or call the base class
	if (
			((int) LOWORD(wParam))== IDC_ButtonAppendPowerExpNozzle
			||
			((int) LOWORD(wParam))== IDC_ButtonDisPowerExpNozzle
			||
			((int) LOWORD(wParam))== IDC_ButtonAppendPressureInSection
			||
			((int) LOWORD(wParam))== IDC_ButtonDisPressureInSection	
			||
			((int) LOWORD(wParam))== IDC_ButtonAppendIS
			||
			((int) LOWORD(wParam))== IDC_ButtonDisIS
			||
			((int) LOWORD(wParam))== IDC_ButtonAppendV
			||
			((int) LOWORD(wParam))== IDC_ButtonDisV
			||
			((int) LOWORD(wParam))== IDC_ButtonAppendEPS
			||
			((int) LOWORD(wParam))== IDC_ButtonDisEPS
			||
			((int) LOWORD(wParam))== IDC_ButtonValueIS
			||
			((int) LOWORD(wParam))== IDC_ButtonStepIS
		) {
		GetParent()->GetParent()->SendMessage(WM_COMMAND,HIWORD(BN_CLICKED) + wParam, lParam);
		return NULL;
	}

	return CPropertyPage::OnCommand(wParam, lParam);
}
