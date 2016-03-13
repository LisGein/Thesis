#if !defined(AFX_PROPERTYPAGECHEM_H__ADF9EFAB_8225_4D85_BD1A_7C687E50D02A__INCLUDED_)
#define AFX_PROPERTYPAGECHEM_H__ADF9EFAB_8225_4D85_BD1A_7C687E50D02A__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000
// PropertyPageChem.h : header file
//

/////////////////////////////////////////////////////////////////////////////
// CPropertyPageChem dialog

class CPropertyPageChem : public CPropertyPage
{
	DECLARE_DYNCREATE(CPropertyPageChem)

// Construction
public:
	CPropertyPageChem();
	~CPropertyPageChem();

	BOOL bFocus;

// Dialog Data
	//{{AFX_DATA(CPropertyPageChem)
	enum { IDD = IDD_ABOUTBOX };
		// NOTE - ClassWizard will add data members here.
		//    DO NOT EDIT what you see in these blocks of generated code !
	//}}AFX_DATA


// Overrides
	// ClassWizard generate virtual function overrides
	//{{AFX_VIRTUAL(CPropertyPageChem)
	public:
	virtual BOOL OnSetActive();
	protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV support
	virtual BOOL OnCommand(WPARAM wParam, LPARAM lParam);
	//}}AFX_VIRTUAL

// Implementation
protected:
	// Generated message map functions
	//{{AFX_MSG(CPropertyPageChem)
	afx_msg void OnKillFocus(CWnd* pNewWnd);
	//}}AFX_MSG
	DECLARE_MESSAGE_MAP()

};

//{{AFX_INSERT_LOCATION}}
// Microsoft Visual C++ will insert additional declarations immediately before the previous line.

#endif // !defined(AFX_PROPERTYPAGECHEM_H__ADF9EFAB_8225_4D85_BD1A_7C687E50D02A__INCLUDED_)
