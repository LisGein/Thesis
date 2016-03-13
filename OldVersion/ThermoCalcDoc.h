// ThermoCalcDoc.h : interface of the CThermoCalcDoc class
//
/////////////////////////////////////////////////////////////////////////////

#if !defined(AFX_THERMOCALCDOC_H__82526EEE_35F9_452C_BB5C_FF859F521789__INCLUDED_)
#define AFX_THERMOCALCDOC_H__82526EEE_35F9_452C_BB5C_FF859F521789__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000

#include "ThermoCalcView.h"
#include "ViewOutput.h"
#include "FrameOutput.h"
#include "all_excel.h"// _Application, Workbooks, Worksheets, _Worksheet, Range
#include "all_CCalc.h"

class CThermoCalcDoc : public CDocument
{
protected: // create from serialization only
	CThermoCalcDoc();
	DECLARE_DYNCREATE(CThermoCalcDoc)

// Attributes
public:
	// Output
	unsigned int nMaxNamesLen;
	int nPGas;
	int nPCond;
	int nPAll;

protected:
	// Данные из окон редактирования вида документа
	CALCSTRUCT calcs;
//	int nCurrentP;
	// Условн. хим. формула топлива
	UXFSTRUCT uxf[MaxQuantityFIHBox];
	BOOL bMaybeChangeCS;
	BOOL bMaybeChangeUXF;

	// Продукты сгорания
	chem_dataGas cdG[MaxElemGasFromDB];
	chem_dataCond cdC[MaxElemCondFromDB];

	// Объекты расчета
	JobWithDB db;
	CTherDynEqCalc tdec;
	FirstApproximation fa;
	CalcCameraCombustion ccc;
	CalcCriticalSection ccs;
	CalcAnySectionWithF caswf;
	CalcAnySectionWithPa caswpa;
	CalcAnySectionWithPa caswpa_atm;
	float fAtmospherePressure;
	CalcISDiagram cisd;
	CalcConstV ccv;
	CalcEPS ceps;

	float PP;
	float T;
	float H;
	double IS, CPR, SS;
	type_of	of;			// old flags
	type_ov	ov;			// old variables
	type_ocv	ocv;		// old control variables
	type_ParamInCC ParamInCC;
	type_CriticalSection ParamInCriticalSection;
	type_Section ParamInSections[MAX_Sections];
	int nSections;

	// Ходули для "termo"
	float _XF1[120];
	float _XF2[120];
	float _MC[PERIOD_TABLE_size];

	// Связи
	OPTIONSTRUCT os;							// Output and calc options
	CThermoCalcView* pDlgView;			friend CViewOutput;	// Для как бы общего меню
	CViewOutput * pViewOutput;
	CFrameOutput *pFrameOutput;
	BOOL bInvisibleFlag;
	CString reportTextBuffer;

	// Debug
	char Mstr[256];									//сообщение об ошибках

// Operations
public:
	BOOL CalcUXF();
	BOOL CalcAndShow();
	CALCSTRUCT * GetpCS();
	CALCSTRUCT & GetCS();
	UXFSTRUCT * GetpUXF();
//	UXFSTRUCT *& GetUXF();
	BOOL TherDynEqCalc(MainCalcType &dCurrentT);

// Вспомогательные методы
protected:
	BOOL CalcUXF(int nCurrentAlfa);
	BOOL Calc(int _nCurrentUXF);
	BOOL CalcAndShow(int _nCurrentAlfa);
	void ShowShort();
	void ShowFull();
	void SetInvisibleFlag();									friend CThermoCalcView;
	void SetValidUXF();											friend CThermoCalcView;
	void SetFocus_Output();
	BOOL WriteToExcelWorksheet(_Worksheet &ws, 
			Range &rngLeftTopCorner, 
			Range *prngSquare = NULL);							friend CViewOutput;
	BOOL SaveAsText();											friend CViewOutput;

// Overrides
	// ClassWizard generated virtual function overrides
	//{{AFX_VIRTUAL(CThermoCalcDoc)
	public:
	virtual void Serialize(CArchive& ar);
	virtual BOOL OnNewDocument();
	//}}AFX_VIRTUAL

// Implementation
public:
	virtual ~CThermoCalcDoc();
#ifdef _DEBUG
	virtual void AssertValid() const;
	virtual void Dump(CDumpContext& dc) const;
#endif

// Generated message map functions
protected:
	//{{AFX_MSG(CThermoCalcDoc)
		// NOTE - the ClassWizard will add and remove member functions here.
		//    DO NOT EDIT what you see in these blocks of generated code !
	//}}AFX_MSG
	DECLARE_MESSAGE_MAP()
};

/////////////////////////////////////////////////////////////////////////////

//{{AFX_INSERT_LOCATION}}
// Microsoft Visual C++ will insert additional declarations immediately before the previous line.

#endif // !defined(AFX_THERMOCALCDOC_H__82526EEE_35F9_452C_BB5C_FF859F521789__INCLUDED_)
