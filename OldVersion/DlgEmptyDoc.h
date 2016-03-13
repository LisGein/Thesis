#if !defined(AFX_DLGEMPTYDOC_H__0259C5D9_4B9C_471C_A2ED_AB3351456223__INCLUDED_)
#define AFX_DLGEMPTYDOC_H__0259C5D9_4B9C_471C_A2ED_AB3351456223__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000
// DlgEmptyDoc.h : header file
//

/////////////////////////////////////////////////////////////////////////////
// DlgEmptyDoc document

class DlgEmptyDoc : public CDocument
{
protected:
	DlgEmptyDoc();           // protected constructor used by dynamic creation
	DECLARE_DYNCREATE(DlgEmptyDoc)

// Attributes
public:

// Operations
public:

// Overrides
	// ClassWizard generated virtual function overrides
	//{{AFX_VIRTUAL(DlgEmptyDoc)
	public:
	virtual void Serialize(CArchive& ar);   // overridden for document i/o
	protected:
	virtual BOOL OnNewDocument();
	//}}AFX_VIRTUAL

// Implementation
public:
	virtual ~DlgEmptyDoc();
#ifdef _DEBUG
	virtual void AssertValid() const;
	virtual void Dump(CDumpContext& dc) const;
#endif

	// Generated message map functions
protected:
	//{{AFX_MSG(DlgEmptyDoc)
		// NOTE - the ClassWizard will add and remove member functions here.
	//}}AFX_MSG
	DECLARE_MESSAGE_MAP()
};

//{{AFX_INSERT_LOCATION}}
// Microsoft Visual C++ will insert additional declarations immediately before the previous line.

#endif // !defined(AFX_DLGEMPTYDOC_H__0259C5D9_4B9C_471C_A2ED_AB3351456223__INCLUDED_)
