#if !defined(AFX_FRAMEOUTPUT_H__FBD26435_9D16_42C6_AE13_696F2DF3835E__INCLUDED_)
#define AFX_FRAMEOUTPUT_H__FBD26435_9D16_42C6_AE13_696F2DF3835E__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000
// FrameOutput.h : header file
//

/////////////////////////////////////////////////////////////////////////////
// CFrameOutput frame

class CFrameOutput : public CMDIChildWnd
{
	DECLARE_DYNCREATE(CFrameOutput)
protected:
	CFrameOutput();           // protected constructor used by dynamic creation

// Attributes
public:

// Operations
public:

// Overrides
	// ClassWizard generated virtual function overrides
	//{{AFX_VIRTUAL(CFrameOutput)
	protected:
	virtual BOOL PreCreateWindow(CREATESTRUCT& cs);
	//}}AFX_VIRTUAL

// Implementation
protected:
	virtual ~CFrameOutput();

	// Generated message map functions
	//{{AFX_MSG(CFrameOutput)
	afx_msg void OnSize(UINT nType, int cx, int cy);
	//}}AFX_MSG
	DECLARE_MESSAGE_MAP()
};

/////////////////////////////////////////////////////////////////////////////

//{{AFX_INSERT_LOCATION}}
// Microsoft Visual C++ will insert additional declarations immediately before the previous line.

#endif // !defined(AFX_FRAMEOUTPUT_H__FBD26435_9D16_42C6_AE13_696F2DF3835E__INCLUDED_)
