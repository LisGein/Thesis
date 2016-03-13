// MainFrm.h : interface of the CMainFrame class
//
/////////////////////////////////////////////////////////////////////////////

#if !defined(AFX_MAINFRM_H__D5A02FC2_B750_45C5_B149_4780A98D1F76__INCLUDED_)
#define AFX_MAINFRM_H__D5A02FC2_B750_45C5_B149_4780A98D1F76__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000

#include "ChildFrm.h"
#include "ThermoCalc.h"
#include "ThermoCalcView.h"

class CMainFrame : public CMDIFrameWnd
{
	DECLARE_DYNAMIC(CMainFrame)
public:
	CMainFrame();

// Attributes
public:

protected:
	CRect _rDlgRect;
	CChildFrame* _pDlgFrame;
	CThermoCalcView* _pDlgView;
	CMultiDocTemplate* _pDocTemplateDlg;

//	CViewOutput * pViewOutput;
	CMultiDocTemplate* _pDocTemplateOutput;

// Operations
public:
	void GetMainDlgRect(RECT &R);
	CMultiDocTemplate* GetOutputDocTemplate();
	CMultiDocTemplate* GetDlgDocTemplate();
	CThermoCalcView * GetDlgView();

protected:
	void SetMainDlgParams(CChildFrame* DlgFrame, CThermoCalcView* pDlgView, 
			RECT &R);														friend CThermoCalcApp;
	void SetMultiDocTemplates(CMultiDocTemplate* pDocTemplateDlg, 
			CMultiDocTemplate* pDocTemplateOutput); 	friend CThermoCalcApp;

// Overrides
	// ClassWizard generated virtual function overrides
	//{{AFX_VIRTUAL(CMainFrame)
	public:
	virtual BOOL PreCreateWindow(CREATESTRUCT& cs);
	//}}AFX_VIRTUAL

// Implementation
public:
	virtual ~CMainFrame();
#ifdef _DEBUG
	virtual void AssertValid() const;
	virtual void Dump(CDumpContext& dc) const;
#endif

protected:  // control bar embedded members
	CStatusBar  m_wndStatusBar;

// Generated message map functions
protected:
	//{{AFX_MSG(CMainFrame)
	afx_msg int OnCreate(LPCREATESTRUCT lpCreateStruct);
		// NOTE - the ClassWizard will add and remove member functions here.
		//    DO NOT EDIT what you see in these blocks of generated code!
	//}}AFX_MSG
	DECLARE_MESSAGE_MAP()
};

/////////////////////////////////////////////////////////////////////////////

//{{AFX_INSERT_LOCATION}}
// Microsoft Visual C++ will insert additional declarations immediately before the previous line.

#endif // !defined(AFX_MAINFRM_H__D5A02FC2_B750_45C5_B149_4780A98D1F76__INCLUDED_)
