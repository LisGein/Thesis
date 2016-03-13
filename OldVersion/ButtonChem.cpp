// ButtonChem.cpp : implementation file
//

#include "stdafx.h"
#include "ThermoCalc.h"
#include "ButtonChem.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

/////////////////////////////////////////////////////////////////////////////
// CButtonChem

CButtonChem::CButtonChem() {	pFont = 0;	bCapture = FALSE;}

CButtonChem::~CButtonChem() {	if (pFont)	delete pFont;	}

BOOL CButtonChem::Create( LPCTSTR lpszCaption, DWORD dwStyle, const RECT& rect, CWnd* pParentWnd, UINT nID ) {
	return CButton::Create( lpszCaption, dwStyle, rect, pParentWnd, nID );
}

BEGIN_MESSAGE_MAP(CButtonChem, CButton)
	//{{AFX_MSG_MAP(CButtonChem)
	ON_WM_LBUTTONDBLCLK()
	ON_WM_LBUTTONUP()
	ON_WM_LBUTTONDOWN()
	ON_WM_CTLCOLOR_REFLECT()
	ON_WM_MOUSEMOVE()
	ON_WM_SETFOCUS()
	//}}AFX_MSG_MAP
END_MESSAGE_MAP()

/////////////////////////////////////////////////////////////////////////////
// CButtonChem message handlers

void CButtonChem::OnLButtonDblClk(UINT nFlags, CPoint point) {
	// TODO: Add your message handler code here and/or call default	
	//LONG lWL = GetWindowLong(m_hWnd, GWL_STYLE);
	UINT uiBS = GetButtonStyle();
	if ( ((uiBS&BS_RADIOBUTTON)==BS_RADIOBUTTON)||((uiBS&BS_AUTORADIOBUTTON)==BS_AUTORADIOBUTTON) ) {
		CWnd *pW = GetParent();
		CButton *pDGI = (CButton *)pW ->GetNextDlgGroupItem(this);
		//lWL = GetWindowLong(pDGI->m_hWnd, GWL_STYLE);
		while (pDGI != this) {
			uiBS = pDGI->GetButtonStyle();
			if ( ((uiBS&BS_RADIOBUTTON)==BS_RADIOBUTTON)||((uiBS&BS_AUTORADIOBUTTON)==BS_AUTORADIOBUTTON) ) {
				SetCheck(1);	//1 - Set the button state to checked. 
				pDGI ->SetCheck(0);
			}
			pDGI = (CButton *)pW ->GetNextDlgGroupItem(pDGI);
			//lWL = GetWindowLong(pDGI->m_hWnd, GWL_STYLE);
		}
	}
	else {
		// Reset the current default push button to a regular button.
		SendMessage(BM_SETSTYLE, BS_PUSHBUTTON, (LONG)TRUE);
		// Update the default push button's control ID.
		GetParent() ->SendMessage(DM_SETDEFID, IDC_BUTTON1, 0L);
		SetState(TRUE);
		bCapture = TRUE;
		SetCapture();
	}
//	CButton::OnLButtonDblClk(nFlags, point);
}

void CButtonChem::OnLButtonUp(UINT nFlags, CPoint point) {
	// TODO: Add your message handler code here and/or call default
//	CButton::OnLButtonUp(nFlags, point);
	if (bCapture) {
		bCapture = FALSE;
		ReleaseCapture();
		SetState(FALSE);
		GetClientRect(&R);
		if (R.PtInRect(point)) {
			// Послать сообщение род. окну о клике кнопки мыши на контрле
			LONG lID = GetWindowLong(m_hWnd, GWL_ID);
			WPARAM wp = HIWORD(BN_CLICKED) + LOWORD(lID);
			GetParent() ->SendMessage(WM_COMMAND, wp, (LPARAM)m_hWnd);
		}
	}
}

void CButtonChem::OnLButtonDown(UINT nFlags, CPoint point) {
	// TODO: Add your message handler code here and/or call default
	//LONG lWL = GetWindowLong(m_hWnd, GWL_STYLE);
	UINT uiBS = GetButtonStyle();
	if ( ((uiBS&BS_RADIOBUTTON)==BS_RADIOBUTTON)||((uiBS&BS_AUTORADIOBUTTON)==BS_AUTORADIOBUTTON) ) {
		CWnd *pW = GetParent();
		CButton *pDGI = (CButton *)pW ->GetNextDlgGroupItem(this);
		//lWL = GetWindowLong(pDGI->m_hWnd, GWL_STYLE);
		while (pDGI != this) {
			uiBS = pDGI->GetButtonStyle();
			if ( ((uiBS&BS_RADIOBUTTON)==BS_RADIOBUTTON)||((uiBS&BS_AUTORADIOBUTTON)==BS_AUTORADIOBUTTON) ) {
				pDGI ->SetCheck(0);
				SetCheck(1);	//1 - Set the button state to checked. 
				// Послать сообщение род. окну о клике кнопки мыши на контрле
				LONG lID = GetWindowLong(m_hWnd, GWL_ID);
				WPARAM wp = HIWORD(BN_CLICKED) + LOWORD(lID);
				GetParent() ->SendMessage(WM_COMMAND, wp, (LPARAM)m_hWnd);
			}
			pDGI = (CButton *)pW ->GetNextDlgGroupItem(pDGI);
			//lWL = GetWindowLong(pDGI->m_hWnd, GWL_STYLE);
		}
	}
	else {
		// Reset the current default push button to a regular button.
		SendMessage(BM_SETSTYLE, BS_PUSHBUTTON, (LONG)TRUE);
		// Update the default push button's control ID.
		GetParent() ->SendMessage(DM_SETDEFID, IDC_BUTTON1, 0L);
		SetState(TRUE);
		bCapture = TRUE;
		SetCapture();
	}
//	CButton::OnLButtonDown(nFlags, point);
}

HBRUSH CButtonChem::CtlColor(CDC* pDC, UINT nCtlColor) {
		// TODO: Change any attributes of the DC here
	if (!pFont) {
		pFont = new CFont;

		LOGFONT lf;
		memset(&lf,0,sizeof(LOGFONT));
		lf.lfHeight = -cnFontSizeStaticChem;
		//lf.lfHeight = -cnFontSizeStaticChem-8;
		lf.lfWeight = FW_NORMAL;
		lf.lfCharSet = RUSSIAN_CHARSET;
		lf.lfOutPrecision = OUT_STRING_PRECIS;
		lf.lfClipPrecision = CLIP_STROKE_PRECIS;
		lf.lfQuality = DRAFT_QUALITY;
		lf.lfPitchAndFamily = VARIABLE_PITCH|FF_SWISS;
		strcpy(lf.lfFaceName, "MS Sans Serif");
		pFont ->CreateFontIndirect(&lf);
	}
	pDC->SelectObject(pFont);

	pDC ->SetBkColor(GetSysColor(COLOR_3DLIGHT));
	pDC ->SetTextColor(GetSysColor(COLOR_WINDOWTEXT));
//	CGdiObject * pGO_oldBrush = pDC ->SelectStockObject(COLOR_3DLIGHT));
//	CGdiObject * pGO_oldPen = pDC ->SelectStockObject(BLACK_PEN);
//	CPen pen(PS_DOT,2,RGB(254,0,0));
//	CGdiObject * pGO_oldPen2 = pDC ->SelectObject(&pen);
	// TODO: Return a non-NULL brush if the parent's handler should not be called
	return (HBRUSH)GetStockObject(GetSysColor(COLOR_3DLIGHT));
//	return NULL;
}

void CButtonChem::OnMouseMove(UINT nFlags, CPoint point) 
{
	// TODO: Add your message handler code here and/or call default
	if (bCapture) {
		GetClientRect(&R);
		if (!R.PtInRect(point))
			SetState(FALSE);
		else
			SetState(TRUE);
	}
	CButton::OnMouseMove(nFlags, point);
}

// Когда фокус ввода передается на окно редактирования он может передаться по кнопку, его нужно вернуть главному окну
void CButtonChem::OnSetFocus(CWnd* pOldWnd) {
//	CButton::OnSetFocus(pOldWnd);	
	// TODO: Add your message handler code here
	GetParent()->GetParent()->SetFocus();		// Переда
}
