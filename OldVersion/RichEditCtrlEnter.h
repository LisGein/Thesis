#if !defined(AFX_RICHEDITCTRLENTER_H__B2EDA4D5_6D40_4B88_A475_D455B08AE628__INCLUDED_)
#define AFX_RICHEDITCTRLENTER_H__B2EDA4D5_6D40_4B88_A475_D455B08AE628__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000
// RichEditCtrlEnter.h : header file
//

/////////////////////////////////////////////////////////////////////////////
// CRichEditCtrlEnter window

class CRichEditCtrlEnter : public CRichEditCtrl
{
// Construction
public:
	CRichEditCtrlEnter();

// Attributes
public:

// Operations
public:

// Overrides
	// ClassWizard generated virtual function overrides
	//{{AFX_VIRTUAL(CRichEditCtrlEnter)
	protected:
	virtual LRESULT WindowProc(UINT message, WPARAM wParam, LPARAM lParam);
	//}}AFX_VIRTUAL

// Implementation
public:
	virtual ~CRichEditCtrlEnter();

	// Generated message map functions
protected:
	//{{AFX_MSG(CRichEditCtrlEnter)
	afx_msg void OnChar(UINT nChar, UINT nRepCnt, UINT nFlags);
	//}}AFX_MSG

	DECLARE_MESSAGE_MAP()
};

/////////////////////////////////////////////////////////////////////////////

//{{AFX_INSERT_LOCATION}}
// Microsoft Visual C++ will insert additional declarations immediately before the previous line.

#endif // !defined(AFX_RICHEDITCTRLENTER_H__B2EDA4D5_6D40_4B88_A475_D455B08AE628__INCLUDED_)
