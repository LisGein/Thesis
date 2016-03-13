#if !defined(AFX_STATICCHEM_H__E22901E2_09BA_453C_970C_1FEE7B49677A__INCLUDED_)
#define AFX_STATICCHEM_H__E22901E2_09BA_453C_970C_1FEE7B49677A__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000
// StaticChem.h : header file
//

/////////////////////////////////////////////////////////////////////////////
// CStaticChem window

class CStaticChem : public CStatic
{
// Construction
public:
	CStaticChem();

// Attributes
public:
protected:
	CFont *pFont;

// Operations
public:

// Overrides
	// ClassWizard generated virtual function overrides
	//{{AFX_VIRTUAL(CStaticChem)
	//}}AFX_VIRTUAL

// Implementation
public:
	virtual ~CStaticChem();

	// Generated message map functions
protected:
	//{{AFX_MSG(CStaticChem)
	afx_msg HBRUSH CtlColor(CDC* pDC, UINT nCtlColor);
	//}}AFX_MSG

	DECLARE_MESSAGE_MAP()
};

/////////////////////////////////////////////////////////////////////////////

//{{AFX_INSERT_LOCATION}}
// Microsoft Visual C++ will insert additional declarations immediately before the previous line.

#endif // !defined(AFX_STATICCHEM_H__E22901E2_09BA_453C_970C_1FEE7B49677A__INCLUDED_)
