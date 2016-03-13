// ThermoCalc.h : main header file for the THERMOCALC application
//

#if !defined(AFX_THERMOCALC_H__48FBB667_6A1B_4A57_B6D9_50B3DDDAF725__INCLUDED_)
#define AFX_THERMOCALC_H__48FBB667_6A1B_4A57_B6D9_50B3DDDAF725__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000

#ifndef __AFXWIN_H__
	#error include 'stdafx.h' before including this file for PCH
#endif

#include "resource.h"       // main symbols
#include "ChildFrm.h"
//#include "table.h"
#define PERIOD_TABLE_size 112
#define nSize_A6 10	// const int nSize_A6 = 10;

const int MAX_elem=50;
#define Numeral ( (c>='0') && (c<='9') )
#define Letter ( ((c>='A')&&(c<='Z')) || ((c>='a')&&(c<='z')) )//||(c=='+')||(c=='-')

#define NumFactorsGas 9
struct chem_dataGas
	{
		char view[80];
		double factors[NumFactorsGas];
		char name[80];
		chem_dataGas& operator=(const chem_dataGas& that) {
			strcpy(view,that.view);
			strcpy(name,that.name);
			for(int i=0;i<NumFactorsGas;i++)
				factors[i] = that.factors[i];
			return *this;
		}
		chem_dataGas& operator=(const int) {
			memset(view,0,sizeof(view));
			memset(name,0,sizeof(name));
			for(int i=0;i<NumFactorsGas;i++)
				factors[i] = 0;
			return *this;
		}
	};

// ������������ ����� ������
typedef const double MainCalcType;	// ��� ��� H(���������), T

#define NumFactorsCond 5
#define MaxTempIntervals 5
struct chem_dataCond
	{
		char view[80];
		int nIntervals;
		double dTemperature[MaxTempIntervals];
		double factors[MaxTempIntervals][NumFactorsCond];
		chem_dataCond& operator=(const chem_dataCond& that) {
			strcpy(view,that.view);
			nIntervals = that.nIntervals;
			for(int i1=0;i1<MaxTempIntervals;i1++) {
				dTemperature[i1] = that.dTemperature[i1];
				for(int i2=0;i2<NumFactorsCond;i2++)
					factors[i1][i2] = that.factors[i1][i2];
			}
			return *this;
		}
		chem_dataCond& operator=(const int) {
			memset(view,0,sizeof(view));
			for(int i1=0;i1<MaxTempIntervals;i1++) {
				dTemperature[i1] = 0;
				for(int i2=0;i2<NumFactorsCond;i2++)
					factors[i1][i2] = 0;
			}
			return *this;
		}
	};

const int MaxElemGasFromDB=200;	// �������� �������. ���������� �� ��
const int MaxElemCondFromDB=50;	// �������� ���������������� ���������� �� ��

inline int SIGN(double dSign) {
	if (dSign==0) return 0;
	if (dSign>=0)
		return +1;
	else
		return -1;
};

inline int SIGN(float dSign) {
	if (dSign==0) return 0;
	if (dSign>=0)
		return +1;
	else
		return -1;
};

inline int SIGN(int dSign) {
	if (dSign==0) return 0;
	if (dSign>=0)
		return +1;
	else
		return -1;
};

const unsigned int MAX_len = 50;				// ������������ ������ ��������� ������ + ������ '\0', ���� ��� ���
const unsigned int MaxEditBoxes = 5;		// ����. ���������� ���� �������������� ��� ����� �����. � ���.
const unsigned int MaxEditBoxesOx = 4;		// ����. ���������� ���� �������������� ��� ����� �����.
const unsigned int MaxEditBoxesComb = 4;	// ����. ���������� ���� �������������� ��� ����� ���.
const unsigned int MaxAtoms = 5;				// ����. ���-�� ���. ��. � ��������
const unsigned int MaxAtomNameLen = 4;		// ����. ������ � ��������, ����� ��. ���. ��������.
#define MaxQuantityFIHBox 10					// ����. ���-�� ������������� ����� � ��������
// ��� ��������� ��� ������ ���� �������
// ��������� ����� ��� ������������� "����" "������� �����" ����� �������� ���������
struct CALCSTRUCT
{
	int nEditBoxesOx;
	char szNameAtomOfOxides [MaxEditBoxesOx][MaxAtoms][MaxAtomNameLen];
	float factor_oxides [MaxEditBoxesOx][MaxAtoms];
	int nAtomsInStringOx[MaxEditBoxesOx];
	float percent_oxides[MaxEditBoxesOx]; 	//���������� ��������� �������� ���������� �� ���� ����� �����������
	float fEnthalpyOx[MaxEditBoxesOx];

	int nEditBoxesComb;
	char szNameAtomOfCombust [MaxEditBoxesComb][MaxAtoms][MaxAtomNameLen];
	float factor_combust [MaxEditBoxesComb][MaxAtoms];
	int nAtomsInStringComb[MaxEditBoxesComb];
	float percent_combust[MaxEditBoxesOx];
	float fEnthalpyComb[MaxEditBoxesOx];

	int nEditBoxesAlfa;
	float fAlfa[MaxQuantityFIHBox];
	float fKappaStroke0[MaxQuantityFIHBox];
//struct CALCBURNINGSTRUCT
	int nEditBoxesPressure;
	float fPressure[MaxQuantityFIHBox];
	float fMaxT;
	float fMinT;
	float fAssumedT;

//struct CALCFLOWSTRUCT
	BOOL bIncludeCritSect;
	BOOL bIncludeAtmosphereSection;
	BOOL bIncludePa;
	int nPa;
	float fPa[MaxQuantityFIHBox];
	BOOL bIncludeF;
	int nF;
	float fF[MaxQuantityFIHBox];
	BOOL bIncludeIS;
	int nIS;
	float fIS[MaxQuantityFIHBox];
	BOOL bIncludeEPS;
	int nEPS;
	float fEPS[MaxQuantityFIHBox];
	BOOL bIncludeV;
	int nV;
	float fV[MaxQuantityFIHBox];
};

struct CALCBURNINGSTRUCT {
	int nEditBoxesPressure;
	float fPressure[MaxQuantityFIHBox];
	float MaxT;
	float MinT;
	float AssumedT;
};

struct CALCFLOWSTRUCT {
	BOOL bIncudePa;
	int nPa;
	float fPa[MaxQuantityFIHBox];
	BOOL bIncudeF;
	int nF;
	float fF[MaxQuantityFIHBox];
	BOOL bIncudeIS;
	int nIS;
	float fIS[MaxQuantityFIHBox];
	BOOL bIncudeEPS;
	int nEPS;
	float fEPS[MaxQuantityFIHBox];
	BOOL bIncudeV;
	int nV;
	float fV[MaxQuantityFIHBox];
};

// �������� ������������ ��� �������� � ����������
// ��� �������� ������ �������� � ����������
const float fConditMolecularWeightC = 100;
const float fConditMolecularWeightO = 100;

const unsigned int MaxQuantityElemInFuel = MaxAtoms;		// ����. ���-�� ���. ��. � �������

struct OPTIONSTRUCT {
	// Output options
	int nShowOption;
	int nPressureOption;
	int nNumberView;
	int nSizeTab;
	BOOL bIncludeUnitMeasOption;	// ��������� ������ ������ ��������� � �����
	BOOL bMinNamesInOutput;			// ��������� �������� �������� � ������. ������� ��������, ������� ���� ��� ������� ��������� - ���� ��� ��������

	HDC	hdcOutput;
	char szFontName[MAX_len];		// �������� ������ ������������� ��� ������

	// Calc options
//	BOOL bIncludeCritSect;
//	BOOL bIncludePa;
//	BOOL bIncludeF;
//	BOOL bIncludeIS;
//	BOOL bIncludeEPS;
//	BOOL bIncludeV;
};

struct UXFSTRUCT {		// ������. ���. ������� �������
	char szAtomsOfF[MaxQuantityElemInFuel][MaxAtomNameLen];
	float fConditChimFormF[MaxQuantityElemInFuel];
	float fEnthalpyF;
	int nQuantityElemF;	// nQuantityElemInFuel
		UXFSTRUCT& operator=(const UXFSTRUCT& that)
		{
			for(int i1=0;i1<MaxQuantityElemInFuel;i1++) {
				strcpy(szAtomsOfF[i1],that.szAtomsOfF[i1]);
				fConditChimFormF[i1] = that.fConditChimFormF[i1];
			}
			fEnthalpyF = that.fEnthalpyF;
			nQuantityElemF = that.nQuantityElemF;
			return *this;
		}
};

// ������ ���� ���� �������� ����� ��������� ���������� ������� ��������� ������� 
// ��������� ��, ��������� ����� � �� �� ��. ���. � �����
struct type_of {	// old flags
	BOOL F0T;
	BOOL FPP;// ��������� ������������� ���������� �����������
	BOOL CKACH;	// ��� ���� 'CKACH' ��� ������!
	BOOL FIS;
	BOOL KAMEPA;
//	BOOL FKP;
	BOOL FT;
	BOOL F3C;//��������������� � MENGE()���� ������� ����. ������ 'T3AM'
				//������������ � �������() ���� ������� ����. ������ 'T3AM'
				//	if (T<T3AM)	F3C=TRUE;
				//	if (T>T3AM) {	F3C = FALSE;		FP3 = FALSE;	}
	BOOL FV;
	BOOL FP3;	BOOL FB;
};
// ���� FKP - ������ ��� ������ ������� ���-�� ��������� � ������. ���.
// ���� FB - �������� ��������� ����������� �������� � �������()
struct type_ov {	// old variables
	float CK;		// ������� � 'CKACH'
	float P1;
	float MS;		// ������������ ����� �����
	float E;			// ����� ����� ��� ����� 'FHAC'-���-�� ��� ��� ���������, � 'F3C' � 'CK'
	float Q;	float R;	float T2;	float T1;float P2;
	double R0;double W;
	float PYD,PYDP;
	double DT;double C;
	int HAC[1+MaxAtoms];//,LRF;
};
struct type_ocv {	// old control variables
	int _M;
	int _NG;
	int _N;
	int _JE;
};

// PUT EDIT(': ','�����','K1',P KAM.','P B��.','I',T','M ��','CP3',
//			'K3','B','PYD','PYDP','F','F/FKP','Z',':')(SKIP,A,...);
struct type_ParamInCC {
	float Alfa;				//
	float K_actually;		//����������� ������� ����������� �����������(���./��.) � �������.
	float P_KAM;
	double I;
	float T;
	float MS;
	float CP3;
	float Z;
};

struct type_CriticalSection {
	float Alfa;				//
	float K_actually;		//����������� ������� ����������� �����������(���./��.) � �������.
	float PKP;
	double I;
	float T;
	float MS;
	float CP3;
	float K3;
	float B;
	float PYD;
	float PYDP;
	float F;
	float F_div_FKP;
	float Z;
};

#define _MAX_Sections 5
#if _MAX_Sections > MaxQuantityFIHBox
//	const unsigned int MAX_Sections = MaxQuantityFIHBox;
	#define MAX_Sections  MaxQuantityFIHBox
#else
	#define MAX_Sections  _MAX_Sections
#endif

struct type_Section {
	float Alfa;				//
	float K_actually;		//����������� ������� ����������� �����������(���./��.) � �������.
	float P_Section;
	float I;
	float T;
	float MS;
	float CP3;
	float K3;
	float B;
	float PYD;
	float PYDP;
	float F;
	float F_div_FKP;
	float Z;
};

// ��������� ��� ���������� � ����������� ������ ��������� ������������
const unsigned int cnFull = 1;
const unsigned int cnShort = 2;
const unsigned int cnPrP = 11;
const unsigned int cnPrM = 12;
const unsigned int cnASTRANumV = 21;
const unsigned int cnExpNumV = 22;

// ��������� ������
// ��� ".GetReport(char *)" ������������ ������ ������ � ��������
const unsigned int MAX_len_report = 64000;
// ��� "ToDotString()" ������ ���������� ������� "�" ����� 50 char
const unsigned int constToDotString = 5;		// Number of digits after decimal point < MAX_len
// ���������� ����� ����� �������
//const unsigned int cnDefStringLenght = 3;	// Number of digits after decimal point

// ������
const int cnFontSizeStaticChem = 8;				// ��� ���� ����������� �������
const int cnFontSizeRichEditCtrlChem = 11;	// ��� ���� ���� ��������������
const int cnDefFontSize = 10;						// ��� ����� ������
const char szDefFontFaceName[] = "Arial";
//const char szDefaultFontFaceName[] = "Courier";

// ������� ���� ������
const int constOutputWndHigh = 250;
const int constOutputWndWidth = 0;	// ���� 0 ����� �� ������ ������� �����

// Excel
	// ���-�� �������� � ��������������� ������ ��������
const int cnColWithUnitMeasOption = 3; // ���� ����� �������� ������� ���������
const int cnColWithoutUnitMeasOption = 2; // ���� ����� �������� ������� ���������
// ������ ������ ������ �� �������, � ��������
const	int cnIndentReports = 1;
	// ���-�� �������� � ��������������� ������ �������� ���� �������� ����� �������� ����
const	int cnColWithMinNames = 1;

// �������
	// ��� ������ � Excel ����� COM
		// ������???: ����� �� ���-�� ������� ����� .AttachDispatch();
#define MACROGetOffset(RowOffset, ColumnOffset) {\
pOffset = rngLeftTopCorner.GetOffset(COleVariant((long)RowOffset), COleVariant((long)ColumnOffset));\
rngThroughOffset.AttachDispatch(pOffset);}

#define MACROClearOffset() {\
rngThroughOffset.ReleaseDispatch();}

#define MACROMerge(cell1, cell2) {\
pOffset = ws.GetRange(COleVariant((CString)cell1), COleVariant((CString)cell2));\
rngThroughOffset.AttachDispatch(pOffset);\
rngThroughOffset.Merge(COleVariant((long)FALSE, VT_BOOL));\
rngThroughOffset.ReleaseDispatch();}

			// �������� � ������ ��������������� �������� �� ������ � ������ �����
#define MACROSetFormatnValue(M_Row, M_Column, M_str, M_strText, M_nValue) {	\
	MACROGetOffset(((long)M_Row), ((long)M_Column));		\
	M_str.Format(M_strText,M_nValue);							\
	rngThroughOffset.SetValue2(COleVariant(M_str));			\
	MACROClearOffset();												\
}

			// �������� � ������ �������� � ��������� ������
#define MACROSetfValue(M_Row, M_Column, M_str, M_fValue) {	\
	MACROGetOffset(((long)M_Row), ((long)M_Column));		\
	M_str.Empty();														\
	ToDotString(M_fValue, M_str);									\
	rngThroughOffset.SetValue2(COleVariant(M_str));			\
	MACROClearOffset();												\
}

			// �������� � ������ ��������� ������
#define MACROSetStrValue(M_Row, M_Column, M_strText) {	\
	MACROGetOffset(((long)M_Row), ((long)M_Column));		\
	rngThroughOffset.SetValue2(COleVariant(M_strText));	\
	MACROClearOffset();												\
}

			// ���������� N ������� �������������� �������, �������� - ����� ������, �������� �� ������ � ������ ��� ������� ������
#define MACROMergeRightToLeft(M_Row, M_Column, M_N, M_str1, M_str2) {		\
	MACROGetOffset(((long)M_Row), ((long)M_Column));		\
	M_str1 = rngThroughOffset.GetAddress();					\
	MACROClearOffset();												\
	MACROGetOffset(((long)M_Row), ((long)M_Column+M_N));	\
	M_str2 = rngThroughOffset.GetAddress();					\
	MACROClearOffset();												\
	pOffset = ws.GetRange(COleVariant((CString)M_str1), COleVariant((CString)M_str2));\
	rngThroughOffset.AttachDispatch(pOffset);					\
	rngThroughOffset.Merge(COleVariant((long)FALSE, VT_BOOL));\
	rngThroughOffset.ReleaseDispatch();							\
}


// ������� ����-�������������� ����� �������
//��� ������� Box  x_coef = 1.7333333333333333333333333333333, y = 1.5238095238095238095238095238095
//��� �����. Box  x_coef = 1.4545454545454545454545454545455, y = 1.6153846153846153846153846153846

// FieldInput
// Common
const int cnBoxCommonHigh = 26;
const int cnBoxCommonVSpace = 5;
const int cnBoxCommonHSpace = 0;
// Formul
const int cnBoxFormulWidth = 160;
// Formul Oxidant
const int cnBoxFormulOLeft = 18;
const int cnBoxFormulOTop = 55;
// Formul Combustible
const int cnBoxFormulCLeft = 315;
const int cnBoxFormulCTop = cnBoxFormulOTop ;
// Percent
const int cnBoxPercentWidth = 40;
// Spin
const int cnSpinWidth = 15;
const int cnSpinHigh = cnBoxCommonHigh;
// Enthalpy
const int cnBoxEnthalpyWidth = 60;

const unsigned int MAX_Items = 6;	// ����. ���������� ��������� � ���� ��� � CalcFlow

#define Item_Button			0x0080
#define Item_Edit				0x0081
#define Item_Static			0x0082
#define Item_ListBox			0x0083
#define Item_ScrollBar		0x0084	
#define Item_ComboBox		0x0085

// PropertySheet UXF
const int cnPSUXFHigh = 99-6;
const int cnPSUXFWidth = 529;
const int cnPSUXFLeft = 12;
const int cnPSUXFTop = 213;
	// StaticText UXF
const int cnStUXFHigh = 26;
const int cnStUXFWidth = 44;
const int cnStUXFLeft = 0;
const int cnStUXFTop = 0;
		// RichEditCtrlChem UXF
const int cnBoxUXFHigh = cnBoxCommonHigh;
const int cnBoxUXFWidth = cnPSUXFWidth - cnStUXFWidth -cnStUXFLeft -29;
const int cnBoxUXFLeft = cnStUXFWidth + 3;
const int cnBoxUXFTop = cnStUXFTop + 1;
	// Static EnthalpyUXF
const int cnStEnthalpyUXFHigh = 26;
const int cnStEnthalpyUXFWidth = 56;
const int cnStEnthalpyUXFLeft = 0;
const int cnStEnthalpyUXFTop = 27;
		// RichEditCtrl EnthalpyUXF
const int cnBoxEnthalpyUXFHigh = cnBoxCommonHigh;
const int cnBoxEnthalpyUXFWidth = 150;
const int cnBoxEnthalpyUXFLeft = cnStEnthalpyUXFWidth + 3;
const int cnBoxEnthalpyUXFTop = cnStEnthalpyUXFTop + 2;

// FieldInputH
const int cnBoxFIHWidth = 40;
// Alfa
const int cnBoxAlfaLeft = 259;
const int cnBoxAlfaTop = 182;
// Pressure
const int cnBoxPressureWidth = 45;
const int cnBoxPressureLeft = 130;
const int cnBoxPressureTop = 333;
// Power expansion nozzle
//const int cnPowerExpNozzleLeft = 140;
//const int cnPowerExpNozzleTop = 10;

// Pressure in section
//const int cnBoxPressureInSectionWidth = 45;
//const int cnBoxPressureInSectionLeft = cnPowerExpNozzleLeft-39;
//const int cnBoxPressureInSectionTop = cnPowerExpNozzleTop + 35;

// PropertySheet for CalcFlow
const int cnQuantityPagesCalcFlow = 5;
//const int cnPSCalcFlowHigh = 110;
//const int cnPSCalcFlowWidth = 505;
const int cnPSCalcFlowLeft = 8;
const int cnPSCalcFlowTop = 383;

// 64K
const int cnSizeOutputText = MAX_len_report;
class CLimitString	: public CString
{
// Attributes
	public:
		CLimitString() { CString::GetBufferSetLength (cnSizeOutputText); }
};

/////////////////////////////////////////////////////////////////////////////
// CThermoCalcApp:
// See ThermoCalc.cpp for the implementation of this class
//

class CThermoCalcApp : public CWinApp
{
public:
	CThermoCalcApp();
// Overrides
	// ClassWizard generated virtual function overrides
	//{{AFX_VIRTUAL(CThermoCalcApp)
	public:
	virtual BOOL InitInstance();
	//}}AFX_VIRTUAL

// Implementation
	//{{AFX_MSG(CThermoCalcApp)
	afx_msg void OnAppAbout();
		// NOTE - the ClassWizard will add and remove member functions here.
		//    DO NOT EDIT what you see in these blocks of generated code !
	//}}AFX_MSG
	DECLARE_MESSAGE_MAP()
//	char cEmptyCargo[1073741824/2];
};


/////////////////////////////////////////////////////////////////////////////

//{{AFX_INSERT_LOCATION}}
// Microsoft Visual C++ will insert additional declarations immediately before the previous line.

#endif // !defined(AFX_THERMOCALC_H__48FBB667_6A1B_4A57_B6D9_50B3DDDAF725__INCLUDED_)
