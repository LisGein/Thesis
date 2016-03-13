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
	// !!! ��������� CHARFORMAT2
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

	// ��� ���������� �� ������ ������������
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

// ���� ����� ���������� � ��������
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

	void SetFont( CFont* pFont, BOOL bRedraw = TRUE ); // �� CWnd
	DWORD GetDefaultCharFormat(CHARFORMAT &_cf);

// Implementation
protected:
	CRichEditCtrl FIH[MaxQuantityFIHBox];	// ������ �������� ����� ��������
	int nItems;										// ���������� �������� ����� ��������
	char szTemp[MAX_len];
	CString strTemp;
	int nLeft;										// ����������
	int nTop;										//
	int nWidth;										//
	int nMaxQuantityItems;						// ������������ ���������� �������� ����� ��������
	BOOL bMaybeZeroItems;
	BOOL bDefaultCharFormat;
	CHARFORMAT cfDefaultCharFormat;

// ��� ������ ����� ����� '����'
//	CRichEditCtrl stepFIH[3]; // ��������� ��������, �������� ��������, ���
// Operations
public:
	void Transform(CWnd *pTCV, unsigned int uiTransVariant);
	enum TransVariant{
		TransStateIsStep,
		TransStateIsValue
	};

	unsigned int uiTransformState;			// ������� ��������� ������ �������
	CStaticChem stStep;							// ������� ������������ ������, ������� ��������� ��� ����� ����� ���
	CStaticChem stStartValue;					//
	CStaticChem stEndValue;						//
	const static CString strStep;				// ����� ��� �������� ������������ ������
	const static CString strStartValue;		//
	const static CString strEndValue;		//

// ���������� � ������� ������� �������� ���������� ������������ �������
	unsigned int GetTransformState();
	float GetStep();
	float GetStartValue();
	float GetEndValue();
// ���������� � ������� ����� ��� ��������� �������� ����� "���" � ��� �����
	BOOL CalcSteps();	// ������� ����� ����� ��������������
	int GetAmountSteps();
	BOOL GetSomeValueOfSteps(int nNumValue, float *pfV);
protected:
	float fSomeValueOfSteps[MaxQuantityFIHBox];
	CString strSaveValue[MaxQuantityFIHBox];		// ������ ��� �������� ���������� ���� ����� ��������
	CString strSaveSteps[3];							// ������ ��� �������� ���������� ����� ����� '���'
	int nAmountOfSteps;
	BOOL bStepsWasCalc;// ������� ����, ��� ���������� ����� ����� '���' ��������� � �������� � fSomeValueOfSteps[]
	const static int nIndexStep;	// ���������, ������� ���� ����� � ���� '���'
	const static int nIndexStartValue;
	const static int nIndexEndValue;
	float fStep;											// �������� ��������� � ���� ����� � ���� '���'
	float fStartValue;
	float fEndValue;
};

/////////////////////////////////////////////////////////////////////////////
//{{AFX_INSERT_LOCATION}}
// Microsoft Visual C++ will insert additional declarations immediately before the previous line.

#endif // !defined(AFX_FIELDINPUT_H__1BCD6FE0_ADE1_4E92_BFCF_86BCDF2D66F5__INCLUDED_)
