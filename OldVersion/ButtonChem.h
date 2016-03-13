#if !defined(AFX_BUTTONCHEM_H__3608A8CC_E271_432D_A2E1_B0F06A4DC634__INCLUDED_)
#define AFX_BUTTONCHEM_H__3608A8CC_E271_432D_A2E1_B0F06A4DC634__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000
// ButtonChem.h : header file
//

/////////////////////////////////////////////////////////////////////////////
// CButtonChem window

class CButtonChem : public CButton
{
// Construction
public:
	CButtonChem();

// Attributes
public:
protected:
	CFont *pFont;
	BOOL bCapture;
	CRect R;
// Operations
public:
	BOOL Create( LPCTSTR lpszCaption, DWORD dwStyle, const RECT& rect, CWnd* pParentWnd, UINT nID );

// Overrides
	// ClassWizard generated virtual function overrides
	//{{AFX_VIRTUAL(CButtonChem)
	//}}AFX_VIRTUAL

// Implementation
public:
	virtual ~CButtonChem();

	// Generated message map functions
protected:
	//{{AFX_MSG(CButtonChem)
	afx_msg void OnLButtonDblClk(UINT nFlags, CPoint point);
	afx_msg void OnLButtonUp(UINT nFlags, CPoint point);
	afx_msg void OnLButtonDown(UINT nFlags, CPoint point);
	afx_msg HBRUSH CtlColor(CDC* pDC, UINT nCtlColor);
	afx_msg void OnMouseMove(UINT nFlags, CPoint point);
	afx_msg void OnSetFocus(CWnd* pOldWnd);
	//}}AFX_MSG

	DECLARE_MESSAGE_MAP()
};

/////////////////////////////////////////////////////////////////////////////

//{{AFX_INSERT_LOCATION}}
// Microsoft Visual C++ will insert additional declarations immediately before the previous line.

#endif // !defined(AFX_BUTTONCHEM_H__3608A8CC_E271_432D_A2E1_B0F06A4DC634__INCLUDED_)
