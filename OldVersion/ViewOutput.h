#if !defined(AFX_VIEWOUTPUT_H__113CE7AC_DE06_4522_8F41_0AD974E4ED19__INCLUDED_)
#define AFX_VIEWOUTPUT_H__113CE7AC_DE06_4522_8F41_0AD974E4ED19__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000
// ViewOutput.h : header file
//

/////////////////////////////////////////////////////////////////////////////
// CViewOutput view

class CViewOutput : public CRichEditView
{
	//friend class CThermoCalcView;
protected:
	CViewOutput();           // protected constructor used by dynamic creation
	DECLARE_DYNCREATE(CViewOutput)

// Attributes
public:

// Operations
public:
	//BOOL Show(OPTIONSTRUCT &os, double P[], char szNam[MaxElemGasFromDB][MAX_len], int nPGas, int nPCond, int nPAll, unsigned int nMaxNamesLen, CALCSTRUCT &calcs,UXFSTRUCT uxf[], type_ParamInCC &ParamInCC, type_CriticalSection &ParamInCriticalSection, type_Section ParamInSections[], int nSections);
	BOOL Show(OPTIONSTRUCT &os, float P[], char szNam[MaxElemGasFromDB][MAX_len], int nPGas, int nPCond, int nPAll, unsigned int nMaxNamesLen, CALCSTRUCT &calcs,UXFSTRUCT uxf[], type_ParamInCC &ParamInCC, type_CriticalSection &ParamInCriticalSection, type_Section ParamInSections[], int nSections);
	BOOL Show(CString &str);
	void GetOptionsFromOutput(OPTIONSTRUCT &os);

// Overrides
	// ClassWizard generated virtual function overrides
	//{{AFX_VIRTUAL(CViewOutput)
	public:
	virtual void OnInitialUpdate();
	protected:
	virtual void OnDraw(CDC* pDC);      // overridden to draw this view
	virtual void OnUpdate(CView* pSender, LPARAM lHint, CObject* pHint);
	virtual void OnActivateView(BOOL bActivate, CView* pActivateView, CView* pDeactiveView);
	virtual BOOL PreCreateWindow(CREATESTRUCT& cs);
	//}}AFX_VIRTUAL

// Implementation
protected:
	virtual ~CViewOutput();
#ifdef _DEBUG
	virtual void AssertValid() const;
	virtual void Dump(CDumpContext& dc) const;
#endif

	// Generated message map functions
protected:
	//{{AFX_MSG(CViewOutput)
	afx_msg void OnOutputShort();
	afx_msg void OnUpdateOutputShort(CCmdUI* pCmdUI);
	afx_msg void OnOutputFull();
	afx_msg void OnUpdateOutputFull(CCmdUI* pCmdUI);
	afx_msg void OnViewPartialpressures();
	afx_msg void OnUpdateViewPartialpressures(CCmdUI* pCmdUI);
	afx_msg void OnViewMolesfraction();
	afx_msg void OnUpdateViewMolesfraction(CCmdUI* pCmdUI);
	afx_msg void OnFileOpenInExcel();
	afx_msg void OnFileSaveText();
	//}}AFX_MSG
	DECLARE_MESSAGE_MAP()
};

/////////////////////////////////////////////////////////////////////////////

//{{AFX_INSERT_LOCATION}}
// Microsoft Visual C++ will insert additional declarations immediately before the previous line.

#endif // !defined(AFX_VIEWOUTPUT_H__113CE7AC_DE06_4522_8F41_0AD974E4ED19__INCLUDED_)
