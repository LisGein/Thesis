// ThermoCalcView.h : interface of the CThermoCalcView class
//
/////////////////////////////////////////////////////////////////////////////

#if !defined(AFX_THERMOCALCVIEW_H__73EE4E9A_63E9_4ED1_8124_2CA2E28C5D4E__INCLUDED_)
#define AFX_THERMOCALCVIEW_H__73EE4E9A_63E9_4ED1_8124_2CA2E28C5D4E__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000

#include "DlgEmptyDoc.h"
#include "ViewOutput.h"
#include "FieldInput.h"
#include "StaticChem.h"
#include "PropertySheetChem.h"
#include "PropertyPageChem.h"
#include "ButtonChem.h"
//#include "RichEditCtrlEnter.h"

class CThermoCalcView : public CFormView
{
	DECLARE_DYNCREATE(CThermoCalcView)
public:
	CThermoCalcView();

public:
	//{{AFX_DATA(CThermoCalcView)
	enum { IDD = IDD_THERMOCALC_FORM };
	CButtonChem	m_ButtonDisFormulO;
	CButtonChem	m_ButtonAppendFormulO;
	CButtonChem	m_ButtonDisFormulC;
	CButtonChem	m_ButtonAppendFormulC;
	CButtonChem	m_ButtonDisPressure;
	CButtonChem	m_ButtonAppendPressure;
	CButtonChem	m_ButtonDisAlfa;
	CButtonChem	m_ButtonAppendAlfa;
	CButtonChem	m_ButtonCalcUxf;
	CString	strT;
	CString	strMaxT;
	CString	strMinT;
	//}}AFX_DATA

// Attributes
public:

protected:
//	CALCSTRUCT cs;
	// Для документов
	// Опции через меню
	int _nShowOption;
	BOOL bEnableFull;
	BOOL bEnableShort;
	int _nPressureOption;
	BOOL bPartialPressures;
	BOOL bMolesFraction;
	int _nNumberView;
	BOOL bAstraNumberView;
	BOOL bExponentNumberView;
	int os_nSizeTab;

	BOOL _bIncludeUnitMeasOption;
	BOOL _bMinNamesInOutput;

	// Внутренние
	// Опции через диалог
	BOOL bCalcOnlyWithUxf;
	BOOL bCalcCritSect;
	BOOL bCalcAtmosphereSection;

	// Объекты и переменные для обработки данных ввода
	FieldInput fiFieldInputO;
	CString sOx[MaxEditBoxesOx];
	CString sOxP[MaxEditBoxesOx];
	int nOxP[MaxEditBoxesOx];			// Можно потом убрать
	CString sOxENT[MaxEditBoxesOx];
	int nOxFolmuls;

	FieldInput fiFieldInputC;
	CString sComb[MaxEditBoxesOx];
	CString sCombP[MaxEditBoxesOx];
	int nCombP[MaxEditBoxesOx];		// Можно потом убрать
	CString sCombENT[MaxEditBoxesOx];
	int nCombFolmuls;

	FieldInputH fihAlfa;
	CString sAlfa[MaxQuantityFIHBox];
	float fAlfa[MaxQuantityFIHBox];

	// Для "Tab Control" UXF
	CPropertySheetChem ps;												// PROPSHEETHEADER
	CPropertyPageChem pp[MaxQuantityFIHBox];						// PROPSHEETPAGE
	char szTitilePP[MaxQuantityFIHBox][MAX_len];

	CRichEditCtrlChem reccUXF[MaxQuantityFIHBox];
	CString sUXF[MaxQuantityFIHBox];

	CRichEditCtrl recEnthalpyUXF[MaxQuantityFIHBox];
	float fEnthalpyUXF[MaxQuantityFIHBox];
	CString sEnthalpyUXF[MaxQuantityFIHBox];

	CStaticChem	stUXF[MaxQuantityFIHBox];
	CStaticChem	stEnthalpyUXF[MaxQuantityFIHBox];
	const static CString cnstrUXF;
	const static CString cnstrEnthalpyUXF;

	FieldInputH fihPressures;
	CString sPressures[MaxQuantityFIHBox];
	float fPressures[MaxQuantityFIHBox];

	float fT;
	float fMaxT;
	float fMinT;

	// Лист с закладками для расчета течения
	CPropertySheetChem psCalcFlow;									// PROPSHEETHEADER
	CPropertyPageChem ppCalcFlow_Pa;									// PROPSHEETPAGE
	CPropertyPageChem ppCalcFlow_F;									// PROPSHEETPAGE
	CPropertyPageChem ppCalcFlow_IS;									// PROPSHEETPAGE
	CPropertyPageChem ppCalcFlow_V;									// PROPSHEETPAGE
	CPropertyPageChem ppCalcFlow_EPS;								// PROPSHEETPAGE// Степень расширения газа в сопле
	DLGTEMPLATE dt;
		// По параметрам давления в сечениях
		FieldInputH fihPressureInSection;
		CString sPressureInSection[MAX_Sections];
		float fPressureInSection[MAX_Sections];
		CStaticChem	st_Pa;
		CButtonChem	m_ButtonDisPressureInSection;
		CButtonChem	m_ButtonAppendPressureInSection;

		// По геометрической степени расширения сопла Fа/ Fкр
		FieldInputH fihPowerExpNozzle;
		CString sPowerExpNozzle[MaxQuantityFIHBox];
		float fPowerExpNozzle[MaxQuantityFIHBox];
		CStaticChem	st_F;
		CButtonChem	m_ButtonDisPowerExpNozzle;
		CButtonChem	m_ButtonAppendPowerExpNozzle;

		// По отношению площадей в КС и сечении Fа/Fс

		// IS диаграмма
		FieldInputH fihIS;
		CString sIS[MaxQuantityFIHBox];
		float fIS[MaxQuantityFIHBox];
		CStaticChem	st_IS;
		CButtonChem	m_ButtonDisIS;
		CButtonChem	m_ButtonAppendIS;
		CButtonChem	m_RButtonValueIS;
		CButtonChem	m_RButtonStepIS;

		// По горению в постоянном объеме
		FieldInputH fihV;
		CString sV[MaxQuantityFIHBox];
		float fV[MaxQuantityFIHBox];
		CStaticChem	st_V;
		CButtonChem	m_ButtonDisV;
		CButtonChem	m_ButtonAppendV;

		// По !!!???какой-то там плотности
		FieldInputH fihEPS;
		CString sEPS[MaxQuantityFIHBox];
		float fEPS[MaxQuantityFIHBox];
		CStaticChem	st_EPS;
		CButtonChem	m_ButtonDisEPS;
		CButtonChem	m_ButtonAppendEPS;

// Operations
public:
	DlgEmptyDoc* GetDocument();	// сгенерировал AppWizard
	void ShowLoadView(CALCSTRUCT &cs, UXFSTRUCT uxf[MaxQuantityFIHBox]);//friend CThermoCalcDoc;
	void GetOptionsFromDlg(OPTIONSTRUCT &os);

protected:
	BOOL FillCalcsStruct(CALCSTRUCT &cs);
	BOOL GetUXFFromBox(UXFSTRUCT * pUXF);
	void ShowUXF(UXFSTRUCT * pUXF);
	void RedrawPSUXF();
	int TakeDlg (WORD Id, DLGTEMPLATE *pdt, WORD *pwFontDlgSize, char *pszFontDlgName, 
		DLGITEMTEMPLATE pdit[], char szItemInitText[][MAX_len], char szItemClassName[][MAX_len], int nItemSysClass[]);
	WORD CThermoCalcView::SetCountItemInDlg (WORD Id, WORD wNewCount);
	void DrawNozzle( RECT *pMainRect, CDC* pDC);
	void DrawEmptyPSUXF();
	BOOL CreatePSCalcFlow();
// Overrides
	// ClassWizard generated virtual function overrides
	//{{AFX_VIRTUAL(CThermoCalcView)
	public:
	virtual BOOL PreCreateWindow(CREATESTRUCT& cs);
	protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV support
	virtual void OnInitialUpdate(); // called first time after construct
	virtual BOOL OnPreparePrinting(CPrintInfo* pInfo);
	virtual void OnBeginPrinting(CDC* pDC, CPrintInfo* pInfo);
	virtual void OnEndPrinting(CDC* pDC, CPrintInfo* pInfo);
	virtual void OnPrint(CDC* pDC, CPrintInfo* pInfo);
	virtual void OnDraw(CDC* pDC);
	//}}AFX_VIRTUAL

// Implementation
public:
//	BOOL GetFormulsFromEdit(CString StringsO[], int &nStringsO, CString StringsC[], int &nStringsC,CString StringsA[], int &nA);
	virtual ~CThermoCalcView();
#ifdef _DEBUG
	virtual void AssertValid() const;
	virtual void Dump(CDumpContext& dc) const;
#endif

// Generated message map functions
protected:												friend CViewOutput;
	//{{AFX_MSG(CThermoCalcView)
	afx_msg void OnButton2();
	afx_msg void OnButtonCalc();
	afx_msg void OnUpdateOutputFull(CCmdUI* pCmdUI);
	afx_msg void OnOutputFull();
	afx_msg void OnOutputShort();
	afx_msg void OnUpdateOutputShort(CCmdUI* pCmdUI);
	afx_msg void OnViewPartialpressures();
	afx_msg void OnUpdateViewPartialpressures(CCmdUI* pCmdUI);
	afx_msg void OnViewMolesfraction();
	afx_msg void OnUpdateViewMolesfraction(CCmdUI* pCmdUI);
	afx_msg void OnFileSaveAs();
	afx_msg void OnFileSave();
	afx_msg void OnFileClose();
	afx_msg void OnButtonCalcUxf();
	afx_msg void OnCheckCalcOnlyWithUxf();
	afx_msg void OnButtonAppend_FormulO();
	afx_msg void OnButtonDis_FormulO();
	afx_msg void OnVScroll(UINT nSBCode, UINT nPos, CScrollBar* pScrollBar);
	afx_msg void OnButtonAppend_Alfa();
	afx_msg void OnButtonDis_Alfa();
	afx_msg void OnButtonAppend_Pressure();
	afx_msg void OnButtonDis_Pressure();
	afx_msg void OnButtonAppend_FormulC();
	afx_msg void OnButtonDis_FormulC();
	afx_msg void OnButtonAppend_PowerExpNozzle();						friend CPropertyPageChem;
	afx_msg void OnButtonDis_PowerExpNozzle();							friend CPropertyPageChem;
	afx_msg void OnButtonAppend_PressureInSection();					friend CPropertyPageChem;
	afx_msg void OnButtonDis_PressureInSection();						friend CPropertyPageChem;
	afx_msg void OnButtonAppend_IS();										friend CPropertyPageChem;
	afx_msg void OnButtonDis_IS();											friend CPropertyPageChem;
	afx_msg void OnButtonAppend_V();											friend CPropertyPageChem;
	afx_msg void OnButtonDis_V();												friend CPropertyPageChem;
	afx_msg void OnButtonAppend_EPS();										friend CPropertyPageChem;
	afx_msg void OnButtonDis_EPS();											friend CPropertyPageChem;
	afx_msg void OnButtonValue_IS();											friend CPropertyPageChem;
	afx_msg void OnButtonStep_IS();											friend CPropertyPageChem;
	afx_msg int OnCreate(LPCREATESTRUCT lpCreateStruct);
	afx_msg void OnCheckCriticSection();
	afx_msg void OnCheckAtmosphereSection();
	afx_msg void OnIncudeUnitMeasure();
	afx_msg void OnUpdateIncudeUnitMeasure(CCmdUI* pCmdUI);
	afx_msg void OnMinNamesInOutput();
	afx_msg void OnUpdateMinNamesInOutput(CCmdUI* pCmdUI);
	afx_msg void OnViewAstraNumberView();
	afx_msg void OnUpdateViewAstraNumberView(CCmdUI* pCmdUI);
	afx_msg void OnViewExponentNumberView();
	afx_msg void OnUpdateViewExponentNumberView(CCmdUI* pCmdUI);
	//}}AFX_MSG
	DECLARE_MESSAGE_MAP()
};

#ifndef _DEBUG  // debug version in ThermoCalcView.cpp
inline DlgEmptyDoc* CThermoCalcView::GetDocument()
   { return (DlgEmptyDoc*)m_pDocument; }
#endif

/*
class CPropertyPageChem : public CPropertyPage 
{
public:
	//old
	void Construct( UINT nIDTemplate, UINT nIDCaption = 0 ) 
		{	CPropertyPage::Construct(nIDTemplate, nIDCaption);	}
	void Construct( LPCTSTR lpszTemplateName, UINT nIDCaption = 0 )
		{	CPropertyPage::Construct(lpszTemplateName, nIDCaption);	}

	//new
	void Construct( LPCTSTR lpszTemplateName, LPCTSTR lpszIDCaption = 0 ) {
		CPropertyPage::Construct(lpszTemplateName);
		if (lpszIDCaption) {
			//m_strCaption = lpszIDCaption;
			m_psp.dwFlags |= PSP_USETITLE;
			m_psp.pszTitle = lpszIDCaption;
		}
	}
};
*/

/////////////////////////////////////////////////////////////////////////////

//{{AFX_INSERT_LOCATION}}
// Microsoft Visual C++ will insert additional declarations immediately before the previous line.

#endif // !defined(AFX_THERMOCALCVIEW_H__73EE4E9A_63E9_4ED1_8124_2CA2E28C5D4E__INCLUDED_)

