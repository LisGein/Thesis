// PropertySheetChem.cpp : implementation file
//

#include "stdafx.h"
#include "ThermoCalc.h"
#include "PropertySheetChem.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

/////////////////////////////////////////////////////////////////////////////
// CPropertySheetChem

BEGIN_MESSAGE_MAP(CPropertySheetChem, CPropertySheet)
	//{{AFX_MSG_MAP(CPropertySheetChem)
	ON_WM_SETFOCUS()
	ON_WM_KILLFOCUS()
	ON_WM_DRAWITEM()
	ON_WM_ENABLE()
	ON_WM_GETDLGCODE()
	ON_WM_KEYDOWN()
	//}}AFX_MSG_MAP
END_MESSAGE_MAP()

BOOL CALLBACK EXPORT CPropertySheetChem::CallbackFuncGrayString( HDC hDC, LPARAM lpData, int nCount ) {
	char * p = (char *)lpData;
	CDC *phMem = CDC::FromHandle(hDC);
	CDC *_pDCItem;
	int _xString;
	int _yString;

	memcpy(&_pDCItem, &p[strlen(p)+ 1], sizeof (_pDCItem));
	memcpy(&_xString, &p[strlen(p) + 1 + sizeof (_pDCItem)], sizeof (_xString));
	memcpy(&_yString, &p[strlen(p) + 1 + sizeof (_pDCItem) + sizeof (_xString)], sizeof (_yString));

	_pDCItem ->SetTextColor(GetSysColor(COLOR_3DHIGHLIGHT));
	_pDCItem ->TextOut(_xString+1, _yString+1, p, nCount);

	phMem ->SetTextColor(GetSysColor(COLOR_GRAYTEXT));
//	COLORREF colorM = phMem ->GetTextColor();
//	if (colorM != GetSysColor(COLOR_GRAYTEXT))	phMem ->SetTextColor(GetSysColor(COLOR_GRAYTEXT));
	phMem ->TextOut(0, 0, p, nCount);
	return TRUE;
}

IMPLEMENT_DYNAMIC(CPropertySheetChem, CPropertySheet)

CPropertySheetChem::CPropertySheetChem( ) 
	:CPropertySheet()
{
	_bEnable = TRUE;
	xString = 0;
	yString = 0;
	pDCItem = 0;
}

CPropertySheetChem::CPropertySheetChem(UINT nIDCaption, CWnd* pParentWnd, UINT iSelectPage)
	:CPropertySheet(nIDCaption, pParentWnd, iSelectPage)
{
	_bEnable = TRUE;
	xString = 0;
	yString = 0;
	pDCItem = 0;
}

CPropertySheetChem::CPropertySheetChem(LPCTSTR pszCaption, CWnd* pParentWnd, UINT iSelectPage)
	:CPropertySheet(pszCaption, pParentWnd, iSelectPage)
{
	_bEnable = TRUE;
	xString = 0;
	yString = 0;
	pDCItem = 0;
}

CPropertySheetChem::~CPropertySheetChem() {	}

void CPropertySheetChem::EnablePage (int nPage, BOOL bEnable)
{
    // if we want to enable the page
    if (bEnable)
    {
		// remove the index from the map
		m_DisabledPages.RemoveKey (nPage);
		RedrawWindow();
    }
    // if we want to disable it
    else
    {
		// add the index to the map
		m_DisabledPages.SetAt (nPage, nPage);
		RedrawWindow();
    }
}

//*
BOOL CPropertySheetChem::Create( CWnd* pParentWnd, DWORD dwStyle, DWORD dwExStyle) {
	_bEnable = TRUE;
	xString = 0;
	yString = 0;
	pDCItem = 0;
	m_nLastActive = 0;
     m_DisabledPages.RemoveAll();

	return CPropertySheet::Create(pParentWnd, dwStyle, dwExStyle);
}

void CPropertySheetChem::RemovePage( CPropertyPage *pPage ) {
	int nPage = GetPageIndex(pPage);
	m_DisabledPages.RemoveKey(nPage );

	CPropertySheet::RemovePage(pPage);
}

void CPropertySheetChem::RemovePage( int nPage ) {
	m_DisabledPages.RemoveKey (nPage);
	CPropertySheet::RemovePage(nPage);
}
//*/

/////////////////////////////////////////////////////////////////////////////
// CPropertySheetChem message handlers

BOOL CPropertySheetChem::OnInitDialog() {
// OnInitDialog() must be called before EnablePage()
// or GetTabControl() will fail.
	BOOL bReturn = CPropertySheet::OnInitDialog();
	CTabCtrl *ptc = GetTabControl();
	ASSERT(ptc);
	BOOL b = ptc ->ModifyStyle(TCS_TABS|TCS_FIXEDWIDTH, 
			TCS_MULTILINE|TCS_RIGHTJUSTIFY|TCS_SCROLLOPPOSITE|TCS_OWNERDRAWFIXED|TCS_FOCUSNEVER, 
//			TCS_MULTILINE|TCS_RAGGEDRIGHT |TCS_SCROLLOPPOSITE|TCS_OWNERDRAWFIXED|TCS_FOCUSNEVER, 
			SWP_NOACTIVATE);

	ASSERT(b);
	BOOL b2 = ptc ->ModifyStyle(WS_TABSTOP|WS_GROUP, 0, SWP_NOACTIVATE);
//	BOOL b2 = ptc ->ModifyStyle(WS_TABSTOP|WS_GROUP, TCS_MULTILINE|TCS_RIGHTJUSTIFY, SWP_NOACTIVATE);
	ASSERT(b2);
	return b;
}

BOOL CPropertySheetChem::OnNotify(WPARAM wParam, LPARAM lParam, LRESULT* pResult) {
    NMHDR* pnmh = (NMHDR*)lParam;
    // tab is about to change
  // собирается изменится "return TRUE" чтобы предотвратить изменение
    if (TCN_SELCHANGING == pnmh->code)
        // save the current page index
        m_nLastActive = GetActiveIndex ();
    // tab has been changed - была изменена
    else if (TCN_SELCHANGE == pnmh->code)
    {
        // get the current page index
        int nCurrentPage = GetActiveIndex ();
        // if current page is in our map of disabled pages
        if (m_DisabledPages.Lookup (nCurrentPage, nCurrentPage))
        // activate the previous page
        PostMessage (PSM_SETCURSEL, m_nLastActive);
    }
    return CPropertySheet::OnNotify(wParam, lParam, pResult);
}

void CPropertySheetChem::OnDrawItem(int nIDCtl, LPDRAWITEMSTRUCT lpDrawItemStruct) 
{
	// TODO: Add your message handler code here and/or call default	
	LPDRAWITEMSTRUCT lpdis = lpDrawItemStruct;
	CTabCtrl *ptc = GetTabControl();
	ASSERT(ptc);

	if (ptc->m_hWnd == lpdis->hwndItem) {
		pDCItem = CDC::FromHandle(lpdis->hDC);

		memset(szTabText, '\0', sizeof(szTabText));
		tci.mask        = TCIF_TEXT;
		tci.pszText     = szTabText;
		tci.cchTextMax  = sizeof(szTabText)-1;
		ptc->GetItem(lpdis->itemID, &tci);

		CBrush m_brBk(pDCItem ->GetBkColor());
		pDCItem ->FillRect(&lpdis->rcItem, &m_brBk);

		pDCItem ->SetTextAlign(TA_LEFT);
		CBrush m_brT(GetSysColor(COLOR_GRAYTEXT));
		int nRectItemW = lpdis ->rcItem.right - lpdis->rcItem.left;
		int nRectItemH = lpdis ->rcItem.bottom - lpdis->rcItem.top;
		CSize sT = pDCItem ->GetTextExtent(tci.pszText, lstrlen(tci.pszText));
		CSize sR(lpdis->rcItem.left, lpdis->rcItem.top);
		CSize sM;
		int nDrawPage = lpdis ->itemID;
		if (lpdis ->itemState == ODS_SELECTED) {
			if (nDrawPage == 0)
			{	sM.cx = -1; sM.cy = 0;	}
			else
			{	sM.cx = -1; sM.cy = 0;	}
		}
		else
		{	sM.cx = 0; sM.cy = 2;	}

		sR += sM;
		xString = sR.cx+ (nRectItemW  - sT.cx)/2;
		yString = sR.cy + (nRectItemH  - sT.cy)/2;
		if (_bEnable) {
			if (!m_DisabledPages.Lookup (nDrawPage, nDrawPage)) {
				pDCItem ->SetTextColor(GetSysColor(COLOR_WINDOWTEXT));
				pDCItem ->TextOut(xString , yString , tci.pszText, lstrlen(tci.pszText));
				CPropertySheet::OnDrawItem(nIDCtl, lpDrawItemStruct);
				return;
			}
		}
		memcpy(&szTabText[strlen(szTabText) + 1], &pDCItem, sizeof (CDC *));
		memcpy(&szTabText[strlen(szTabText) + 1 +sizeof (CDC *)], &xString, sizeof (xString));
		memcpy(&szTabText[strlen(szTabText) + 1 +sizeof (CDC *) + sizeof (xString)], &yString, sizeof (yString));
		//dc->TextOut(xString, yString), tci.pszText, lstrlen(tci.pszText));	dc->SetTextColor(GetSysColor(COLOR_GRAYTEXT));	
		//RECT r = {xString, yString , sT.cx+xString , sT.cy+yString };	dc->DrawText( tci.pszText, lstrlen(tci.pszText), &r, DT_LEFT|DT_SINGLELINE);
		pDCItem ->GrayString(&m_brT, CallbackFuncGrayString, (LPARAM )tci.pszText, lstrlen(tci.pszText), xString , yString , 0, 0);
	}
	CPropertySheet::OnDrawItem(nIDCtl, lpDrawItemStruct);
}

void CPropertySheetChem::OnSetFocus(CWnd* pOldWnd) {
	CPropertySheet::OnSetFocus(pOldWnd);
	// TODO: Add your message handler code here
	bFocus = TRUE;
}

void CPropertySheetChem::OnKillFocus(CWnd* pNewWnd) {
	CPropertySheet::OnKillFocus(pNewWnd);
	// TODO: Add your message handler code here
	bFocus = FALSE;
//	CWnd* pW3 = GetFocus();
	GetParent()->SetFocus();
}

void CPropertySheetChem::OnEnable(BOOL bEnable) {
	CPropertySheet::OnEnable(bEnable);	
	// TODO: Add your message handler code here
	_bEnable = bEnable;
//	RedrawWindow();
}

LRESULT CPropertySheetChem::WindowProc(UINT message, WPARAM wParam, LPARAM lParam) {
	// TODO: Add your specialized code here and/or call the base class

	if (message == WM_COMMAND) {
		if(LOWORD(wParam) == BN_CLICKED) {
			lParam = lParam;
		}
		if(wParam == IDOK)
			lParam = lParam;
	}

	if (message==WM_KEYDOWN) {
		message=message;
	}

	if (message == DM_GETDEFID) {
		if (bFocus && (0x8000 & GetKeyState(VK_RETURN))) {
			return TRUE;
		}
	}

	return CPropertySheet::WindowProc(message, wParam, lParam);
}

BOOL CPropertySheetChem::OnCommand(WPARAM wParam, LPARAM lParam) {
	// TODO: Add your specialized code here and/or call the base class
	WPARAM wp = wParam|IDOK;
//	if(wParam=IDOFDEFBUTTON || IDOK)
//		lParam = lParam;

	if(LOWORD(wParam) == BN_CLICKED) {
		lParam = lParam;
	}
	
	return CPropertySheet::OnCommand(wParam, lParam);
}

UINT CPropertySheetChem::OnGetDlgCode() {
	// TODO: Add your message handler code here and/or call default
	UINT uiRV = CPropertySheet::OnGetDlgCode();
//	uiRV = DLGC_DEFPUSHBUTTON|DLGC_WANTMESSAGE;
//	uiRV |= DLGC_WANTALLKEYS;
	return uiRV; 
//	return DLGC_WANTALLKEYS;
}

void CPropertySheetChem::OnKeyDown(UINT nChar, UINT nRepCnt, UINT nFlags) 
{
	// TODO: Add your message handler code here and/or call default
	
	CPropertySheet::OnKeyDown(nChar, nRepCnt, nFlags);
}

BOOL CPropertySheetChem::PreTranslateMessage(MSG* pMsg) 
{
	// TODO: Add your specialized code here and/or call the base class
	if (pMsg->message == WM_KEYDOWN) {
		if ( pMsg ->wParam == VK_RETURN) {
			CWnd *pW = GetParent()->GetDlgItem(IDC_BUTTON1);//	наоборот ::GetWindowLong();
			ASSERT(pW);
			HWND h = pW->m_hWnd;
			GetParent()->SendMessage(WM_COMMAND,
					HIWORD(BN_CLICKED)+LOWORD(IDC_BUTTON1),(WPARAM)h);
//			GetParent()->SendMessage(WM_COMMAND,BM_CLICK);
//			GetParent()->SendDlgItemMessage(IDC_BUTTON1,
//				HIWORD(BN_CLICKED)+LOWORD(IDC_BUTTON1),(WPARAM)h);
			return TRUE;
		}
	}

	return CPropertySheet::PreTranslateMessage(pMsg);
}
