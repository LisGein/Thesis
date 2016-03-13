#if !defined(AFX_PROPERTYSHEETCHEM_H__1E41EE30_3B94_4F7C_8AD8_CEB803F3490B__INCLUDED_)
#define AFX_PROPERTYSHEETCHEM_H__1E41EE30_3B94_4F7C_8AD8_CEB803F3490B__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000
// PropertySheetChem.h : header file
//

/////////////////////////////////////////////////////////////////////////////
// CPropertySheetChem

class CPropertySheetChem : public CPropertySheet
{
	DECLARE_DYNAMIC(CPropertySheetChem)

// Construction
public:
	CPropertySheetChem( );
	CPropertySheetChem(UINT nIDCaption, CWnd* pParentWnd = NULL, UINT iSelectPage = 0);
	CPropertySheetChem(LPCTSTR pszCaption, CWnd* pParentWnd = NULL, UINT iSelectPage = 0);

BOOL Create( CWnd* pParentWnd = NULL, DWORD dwStyle = (DWORD) -1, DWORD dwExStyle = 0 );
void RemovePage( CPropertyPage *pPage );
void RemovePage( int nPage );

// Attributes
public:
protected:
	BOOL bFocus;
	BOOL _bEnable;
	CDC *pDCItem;
	int xString;
	int yString;
	char        szTabText[MAX_len +sizeof(CDC *) + sizeof(int) + sizeof(int)];
	TC_ITEM     tci;

// Operations
public:
	static BOOL CALLBACK EXPORT CallbackFuncGrayString( HDC hDC, LPARAM lpData, int nCount );

   protected:
       // we save the current page in TCN_SELCHANGING
       int m_nLastActive;
       // list of indexes of disabled pages
       CMap <int, int&, int, int&> m_DisabledPages;
   public:
       void EnablePage (int nPage, BOOL bEnable = TRUE);


// Overrides
	// ClassWizard generated virtual function overrides
	//{{AFX_VIRTUAL(CPropertySheetChem)
	public:
	virtual BOOL OnInitDialog();
	virtual BOOL PreTranslateMessage(MSG* pMsg);
	protected:
	virtual BOOL OnNotify(WPARAM wParam, LPARAM lParam, LRESULT* pResult);
	virtual LRESULT WindowProc(UINT message, WPARAM wParam, LPARAM lParam);
	virtual BOOL OnCommand(WPARAM wParam, LPARAM lParam);
	//}}AFX_VIRTUAL

// Implementation
public:
	virtual ~CPropertySheetChem();

	// Generated message map functions
protected:
	//{{AFX_MSG(CPropertySheetChem)
	afx_msg void OnSetFocus(CWnd* pOldWnd);
	afx_msg void OnKillFocus(CWnd* pNewWnd);
	afx_msg void OnDrawItem(int nIDCtl, LPDRAWITEMSTRUCT lpDrawItemStruct);
	afx_msg void OnEnable(BOOL bEnable);
	afx_msg UINT OnGetDlgCode();
	afx_msg void OnKeyDown(UINT nChar, UINT nRepCnt, UINT nFlags);
	//}}AFX_MSG
	DECLARE_MESSAGE_MAP()
};

/////////////////////////////////////////////////////////////////////////////

//{{AFX_INSERT_LOCATION}}
// Microsoft Visual C++ will insert additional declarations immediately before the previous line.

#endif // !defined(AFX_PROPERTYSHEETCHEM_H__1E41EE30_3B94_4F7C_8AD8_CEB803F3490B__INCLUDED_)
