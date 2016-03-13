#if !defined(AFX_FIELDINPUT_H__1BCD6FE0_ADE1_4E92_BFCF_86BCDF2D66F5__INCLUDED_)
#define AFX_FIELDINPUT_H__1BCD6FE0_ADE1_4E92_BFCF_86BCDF2D66F5__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000
// FieldInput.h : header file

//#include "ThermoCalcView.h"
#include "ThermoCalc.h"
#include "StaticChem.h"


/////////////////////////////////////////////////////////////////////////////
// CRichEditCtrlChem window

class CRichEditCtrlChem		: public 	CRichEditCtrl
{
	// !!! Проверить CHARFORMAT2
	// !!! EnumFontFamilies  !!!
public:
	CRichEditCtrlChem();
	void SetWindowTextChem(LPCTSTR lpszString);
	BOOL Create(DWORD dwStyle, const RECT& rect, CWnd* pParentWnd, UINT nID);

protected:
	char szAtoms[MaxQuantityElemInFuel][MaxAtomNameLen];
	float fFormF[MaxQuantityElemInFuel];
	int nQuantity;
	CString strTemp;
	CHARFORMAT cfSimple;
	CHARFORMAT cfITALIC;
	CHARFORMAT cfBOLD;
	CHARFORMAT cfITALIC_BOLD;
	CHARFORMAT cfINVALID;

	// Для наблюдения за вводом пользователя
	CString strObserveStr;
	CString strTempObserveStr;
	char szTempObserveStr[MAX_len];
	char szTemp[MAX_len];
	BOOL bMustBeLetter;
	BOOL bWasDot;
	long lIndexDot;
	int nLenAllStr;
	int nCurPos;
	int nLA;
	int nLenCurrStr;
	int nLenCurrStrLetter;
//	char szObserveStr[MAX_len];

// Operations
public:

// Overrides
	// ClassWizard generated virtual function overrides
	//{{AFX_VIRTUAL(CRichEditCtrlChem)
	//}}AFX_VIRTUAL
// Implementation
public:
	virtual ~CRichEditCtrlChem();

	// Generated message map functions
protected:
	//{{AFX_MSG(CRichEditCtrlChem)
	afx_msg void OnChar(UINT nChar, UINT nRepCnt, UINT nFlags);
	//}}AFX_MSG

	DECLARE_MESSAGE_MAP()
};

/////////////////////////////////////////////////////////////////////////////

/////////////////////////////////////////////////////////////////////////////
// FieldInput

// Поля ввода окислителя и горючего
class FieldInput	// : public CCmdTarget
{
// Attributes
public:

// Operations
public:
	FieldInput();           // protected constructor used by dynamic creation
	virtual ~FieldInput();
	void Coordinate(int &_nLeft, int &_nTop);
	void Coordinate(const int _nLeft, const int _nTop);
	BOOL GetPercent(int nCurrentItem, CString &sOxP);
	BOOL GetFormul(int nCurrentItem, CString &sOxF);
	BOOL GetEnthalpy(int nCurrentItem, CString &sOxE);
	int GetCountItems();

	BOOL SetPercentText(int nCurrentItem, char *psz);
	BOOL SetPercentText(int nCurrentItem, int &nOxP);
	BOOL SetPercentText(int nCurrentItem, CString &sOx);

	BOOL SetFormulText(int nCurrentItem, char *psz);
	BOOL SetFormulText(int nCurrentItem, CString &sOx);

	BOOL SetEnthalpyText(int nCurrentItem, char *psz);
	BOOL SetEnthalpyText(int nCurrentItem, int &nENT);
	BOOL SetEnthalpyText(int nCurrentItem, CString &sENT);

	BOOL Scroll(UINT nSBCode, UINT nPos, CScrollBar* pScrollBar);
	BOOL Remove();
	BOOL Add(CWnd *r);
	BOOL Add(CWnd *pTCV, CString &sFormul, int nPercent, CString &sEnthalpy);
	BOOL Add(CWnd *pTCV, CString &sFormul, CString &sPercent, CString &sEnthalpy);
	BOOL Add(CWnd *pTCV, char *szFormul, char *szPercent, char *szEnthalpy);
	BOOL Add(CWnd *pTCV, char *szFormul, int nPercent, char *szEnthalpy);
	void EnableWindow( BOOL bEnable = TRUE );

// Implementation
protected:

//	CRichEditCtrl EO[MaxEditBoxesOx];
	CRichEditCtrlChem EO[MaxEditBoxesOx];
	CRichEditCtrl EOP[MaxEditBoxesOx];
	CSpinButtonCtrl SpinO[MaxEditBoxesOx];
	CRichEditCtrl EENT[MaxEditBoxesOx];
	int nItems;
	char szTemp[MAX_len];
	int nLeft;
	int nTop;
};

/////////////////////////////////////////////////////////////////////////////

/////////////////////////////////////////////////////////////////////////////
// FieldInputH
class FieldInputH //: public CCmdTarget
{
// Attributes
public:
// Operations
public:
	BOOL GetText(int nCurrentItem, char *);
	BOOL SetText(int nCurrentItem, char *psz);
	FieldInputH();           // protected constructor used by dynamic creation
	virtual ~FieldInputH();
	void Coordinate(int &_nLeft, int &_nTop);
	void Coordinate(const int _nLeft, const int _nTop);
	void Coordinate(const int _nLeft, const int _nTop, const int _nWidth);
	void Coordinate
	(const int _nLeft, const int _nTop, const int _nWidth,const int _MaxQuantityItems);
	void MaybeZeroItems(BOOL b);

	BOOL Add(CWnd *r);
	BOOL Remove();

	BOOL GetText(int nCurrentItem, CString &);
	BOOL SetText(int nCurrentItem, double &);
	BOOL SetText(int nCurrentItem, const double &);
	BOOL SetText(int nItem, CString &);
	int GetCountItems();

	void SetFont( CFont* pFont, BOOL bRedraw = TRUE ); // из CWnd
	DWORD GetDefaultCharFormat(CHARFORMAT &_cf);

// Implementation
protected:
	CRichEditCtrl FIH[MaxQuantityFIHBox];	// Массив объектов ввода значения
	int nItems;										// Количество объектов ввода значения
	char szTemp[MAX_len];
	CString strTemp;
	int nLeft;										// Координаты
	int nTop;										//
	int nWidth;										//
	int nMaxQuantityItems;						// Максимальное количество объектов ввода значения
	BOOL bMaybeZeroItems;
	BOOL bDefaultCharFormat;
	CHARFORMAT cfDefaultCharFormat;

// Для работы полей ввода 'ШАГА'
//	CRichEditCtrl stepFIH[3]; // Начальное значение, конечное значение, шаг
// Operations
public:
	void Transform(CWnd *pTCV, unsigned int uiTransVariant);
	enum TransVariant{
		TransStateIsStep,
		TransStateIsValue
	};

	unsigned int uiTransformState;			// Текущее состояние нашего объекта
	CStaticChem stStep;							// Объекты статического текста, который выводятся для полей ввода шаг
	CStaticChem stStartValue;					//
	CStaticChem stEndValue;						//
	const static CString strStep;				// Текст для объектов статического текста
	const static CString strStartValue;		//
	const static CString strEndValue;		//

// Реализация в которой подсчет значений производит пользователь объекта
	unsigned int GetTransformState();
	float GetStep();
	float GetStartValue();
	float GetEndValue();
// Реализация в которой класс сам переводит значения полей "ШАГ" в ряд чисел
	BOOL CalcSteps();	// Просчет шагов перед использованием
	int GetAmountSteps();
	BOOL GetSomeValueOfSteps(int nNumValue, float *pfV);
protected:
	float fSomeValueOfSteps[MaxQuantityFIHBox];
	CString strSaveValue[MaxQuantityFIHBox];		// Строка для хранения содержания окон ввода значений
	CString strSaveSteps[3];							// Строка для хранения содержания полей ввода 'ШАГ'
	int nAmountOfSteps;
	BOOL bStepsWasCalc;// Признак того, что содержание полей ввода 'ШАГ' расчитано и записано в fSomeValueOfSteps[]
	const static int nIndexStep;	// Константы, индексы окон ввода в поле 'ШАГ'
	const static int nIndexStartValue;
	const static int nIndexEndValue;
	float fStep;											// Значения введенные в окна ввода в поле 'ШАГ'
	float fStartValue;
	float fEndValue;
};

/////////////////////////////////////////////////////////////////////////////
//{{AFX_INSERT_LOCATION}}
// Microsoft Visual C++ will insert additional declarations immediately before the previous line.

#endif // !defined(AFX_FIELDINPUT_H__1BCD6FE0_ADE1_4E92_BFCF_86BCDF2D66F5__INCLUDED_)
