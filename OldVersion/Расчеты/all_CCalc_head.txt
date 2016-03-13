#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000

#include "stdafx.h"
#include "ThermoCalc.h"
#include "all_excel.h"// _Application, Workbooks, Worksheets, _Worksheet, Range

// Для INTOCALCSTRUCT
struct CCSTRUCT {	// Для КС
	float T;
	float P;
	float IS;
	float SS;
	float RR;
	float ZK;
	float MG;
};
struct CSSTRUCT {	// Для Кр. сеч.
	float T;
	float P;
	float RR;
	float BTMIN;
};
struct ASSTRUCT {	// Для любых сеч. ??? если не нужна удалим
	float RR;
};

struct INTOCALCSTRUCT {		// Передается между функциями расчета вместе с давлениями ПС
	int A5[10];
	float B[10][10];
	float MK[120];				// 'MK' молекулярная масса конд. вещ.
	BOOL  FB;					// extern var for PABHPAC
	CCSTRUCT cc;
	CSSTRUCT cs;
	ASSTRUCT as;
};

struct THERDYNEQCALCSTRUCT	// Возвращается через указатель функцией термодин. расчета
{
	float IS;
	float SS;
	float CPR;
	float MS;
	float CP3;
	float ZK;
	float MG;
};

struct ProductsCombustion	// Передается между функциями расчета - давления ПС и имена ПС.
{
	float P[MaxElemGasFromDB];					// Сначало газообраз. потом конденсат
	char szNam[MaxElemGasFromDB][MAX_len];	// Сначало газообраз. потом конденсат
	int nMaxNamesLen;
	int nPElementaryElem;
	int nPGas;			// Выбрать что-нибудь одно
	int nPCond;
	int nPAll;
};

struct AllPCSTRUCT//С учетом что все имена ПГ остаются одинаковыми для каждого альфа и давления 
{
	char szNam[MaxElemGasFromDB][MAX_len];
	// Сохраним для каждого альфа и давления - количественный состав ПС
	float P[MaxQuantityFIHBox][MaxQuantityFIHBox][MaxQuantityFIHBox][MaxElemGasFromDB];	// [nP][nAlfa][i]
	int nMaxNamesLen;
	int nPElementaryElem;
	int nPGas;			// Выбрать что-нибудь одно
	int nPCond;
	int nPAll;
};

class JobWithDB;

class CTherDynEqCalc {

public:
	CTherDynEqCalc();
	BOOL Construct(JobWithDB *p);
	THERDYNEQCALCSTRUCT* Calc(float _T, float _PP, float P[MaxElemGasFromDB], INTOCALCSTRUCT &ics);

protected:
	JobWithDB *pDBs;
	float XF1[120];
	float XF2[120];

	void CALFN();
	int I;
	int J;
	int IC;
	int I1;
	int I2;
	int Y;
	int X;
	BOOL FSH;
	int SHY[50];
	int DR;
	BOOL FRY;
	chem_dataGas cdG[MaxElemGasFromDB];
	chem_dataCond cdC[MaxElemCondFromDB];

//	BOOL PABHPAC(double P[MaxElemGasFromDB], float XF1[120], float XF2[120]);
	BOOL PABHPAC(float P[MaxElemGasFromDB], INTOCALCSTRUCT &ics);
	THERDYNEQCALCSTRUCT tdecs;
	float T3AM;
	BOOL F3C;
	BOOL FP3;
	BOOL FHAC;
	BOOL F;
	float PP;
	int HAC[1+MaxAtoms];//,LRF;
	BOOL F1;
public:
	const static float R0;
protected:
	double A6[nSize_A6];
	int J1;

	float E1;
	float E2;
	float MIN;
	double LP[10];
//	double MG;
	BOOL FPP;
	float MK[120];		// 'MK' молекулярная масса конд. вещ.
	float IS1;
	float CP31;
	float E;
	float BT;

	unsigned int nMaxNamesLen;	// Инициализация во время связи с БД

	int nPElementaryElem;
	int nPGas;			// Выбрать что-нибудь одно
	int nPCond;
	int nPAll;

	int M;
	int N;
	int NG;
	float CK;
	float T;
	BOOL CKACH;
	float T2;
	float T1;
	double ENT[120];
	double S[120];
	double CP[120];
};

class Report {
public:
	Report(
	int _rep_nP,
	int _rep_nAlfa,
	int _rep_nParam,
	UXFSTRUCT _wtew_uxf[MaxQuantityFIHBox],
	float _fAllAlfa[MaxQuantityFIHBox],
	float _fKappaStroke0[MaxQuantityFIHBox],
	float _fAllParam[MaxQuantityFIHBox],
	float _fAllP[MaxQuantityFIHBox],
	float _fAllProcessP[MaxQuantityFIHBox][MaxQuantityFIHBox][MaxQuantityFIHBox],
	float _fAllT[MaxQuantityFIHBox][MaxQuantityFIHBox][MaxQuantityFIHBox],
	float _fAllI[MaxQuantityFIHBox][MaxQuantityFIHBox][MaxQuantityFIHBox],
	float _fAllS[MaxQuantityFIHBox][MaxQuantityFIHBox][MaxQuantityFIHBox],
	float _fAllMS[MaxQuantityFIHBox][MaxQuantityFIHBox][MaxQuantityFIHBox],
	float _fAllCP3[MaxQuantityFIHBox][MaxQuantityFIHBox][MaxQuantityFIHBox],
	float _pfAllK3[MaxQuantityFIHBox][MaxQuantityFIHBox][MaxQuantityFIHBox],
	float _pfAllB[MaxQuantityFIHBox][MaxQuantityFIHBox][MaxQuantityFIHBox],
	float _pfAllPYD[MaxQuantityFIHBox][MaxQuantityFIHBox][MaxQuantityFIHBox],
	float _pfAllPYDP[MaxQuantityFIHBox][MaxQuantityFIHBox][MaxQuantityFIHBox],
	float _pfAllF[MaxQuantityFIHBox][MaxQuantityFIHBox][MaxQuantityFIHBox],
	float _pfAllF_div_FKP[MaxQuantityFIHBox][MaxQuantityFIHBox][MaxQuantityFIHBox],
	float _pfAllZK[MaxQuantityFIHBox][MaxQuantityFIHBox][MaxQuantityFIHBox]
	);

	BOOL PrepareToFirstCalc();
	BOOL NextAlfa();
	BOOL NextPressure();
	void SetOptionsReport(OPTIONSTRUCT &ext_os);
	void SetWayWrite(unsigned int uiWayWrite);
	CString GetReportA();
	void ClearReportStr();

// функции для характеристик процесса
	// String report
	int WriteAllCalcsInStr();
	int WriteCurrentAlfaCalcsInStr();
	// Excel report
	BOOL WriteAllCalcsInWS(_Worksheet &ws,Range &rngLeftTopCorner,Range *prngSquare);

// функции для ПС
	BOOL SaveSomePC(int nP, int nAlfa, int nParam, ProductsCombustion &PC);
	// String report
	int WriteToReportOnePCA(int nP, int nAlfa, int nParam);
	int WriteToReportOnePCW();//In future
	// Excel report
	BOOL WriteToWSOnePC(int nP, int nAlfa, int nParam, 
			_Worksheet &ws, Range &rngLeftTopCorner, Range *prngSquare = NULL, BOOL bMinNames = FALSE);

protected:
	int nCurrP;
	int nCurrUXF;
	CLimitString strReport;
	OPTIONSTRUCT rep_os;								// Настройки расчета
	int rep_nP;
	int rep_nAlfa;
	int rep_nParam;

// данные для характеристик процесса
	UXFSTRUCT rep_uxf[MaxQuantityFIHBox];	// УХФ
	float *pfAllAlfa;
	float *pfKappaStroke0;
	float *pfAllParam;
	float *pfAllP;
	float (*pfAllProcessP)[MaxQuantityFIHBox][MaxQuantityFIHBox];
	float (*pfAllT)[MaxQuantityFIHBox][MaxQuantityFIHBox];
	float (*pfAllI)[MaxQuantityFIHBox][MaxQuantityFIHBox];
	float (*pfAllS)[MaxQuantityFIHBox][MaxQuantityFIHBox];
	float (*pfAllMS)[MaxQuantityFIHBox][MaxQuantityFIHBox];
	float (*pfAllCP3)[MaxQuantityFIHBox][MaxQuantityFIHBox];
	float (*pfAllK3)[MaxQuantityFIHBox][MaxQuantityFIHBox];
	float (*pfAllB)[MaxQuantityFIHBox][MaxQuantityFIHBox];
	float (*pfAllPYD)[MaxQuantityFIHBox][MaxQuantityFIHBox];
	float (*pfAllPYDP)[MaxQuantityFIHBox][MaxQuantityFIHBox];
	float (*pfAllF)[MaxQuantityFIHBox][MaxQuantityFIHBox];
	float (*pfAllF_div_FKP)[MaxQuantityFIHBox][MaxQuantityFIHBox];
	float (*pfAllZK)[MaxQuantityFIHBox][MaxQuantityFIHBox];
// данные для ПГ
	AllPCSTRUCT apc;			// структура для работы с давлениями всех ПС
	BOOL bWasInitAllPCSTRUCT;

	BOOL bPa;
	BOOL bPEN;	// PowerExpNozzle - Степень расширения сопла
	BOOL bIS;	// Температура для IS диаграммы
	BOOL bV;
	BOOL bEPS;

// Впомогательные переменные
	CString str1;
	CString str2;
	CString strHeader;
	Range rngThroughOffset;	// for MACROGetOffset(), MACROClearOffset(), MACROMerge(str1,str2)
	LPDISPATCH pOffset;
	int	precision;
	int	decimal, sign;
	char	*buffer;
	float	fSum;
	int	cur_elem;
	char	szOut[MAX_len];

public:
	enum WayWrite {
		showPa,
		showPEN,
		showIS,
		showV,
		showEPS
	};
};


class CalcBase {

public:
	CalcBase();
	BOOL CommonConstr(CTherDynEqCalc *p, UXFSTRUCT _uxf[MaxQuantityFIHBox], CALCSTRUCT _calcs);
	BOOL ConstrShortRep(CTherDynEqCalc *p, UXFSTRUCT _uxf[MaxQuantityFIHBox], CALCSTRUCT _calcs);
	BOOL ConstrLongRep(CTherDynEqCalc *p, UXFSTRUCT _uxf[MaxQuantityFIHBox], CALCSTRUCT _calcs);
	BOOL ConstrLongParamRep(CTherDynEqCalc *p, UXFSTRUCT _uxf[MaxQuantityFIHBox], CALCSTRUCT _calcs);
	~CalcBase();
	BOOL Calc(ProductsCombustion &PC, INTOCALCSTRUCT &ics);

	BOOL PrepareToFirstCalc();
	BOOL NextAlfa();
	BOOL NextPressure();
	void FirstParam();
	BOOL NextParam();

	void SetOptionsReport(OPTIONSTRUCT &ext_os);
	int GetReportA(CString &strReport, int nMaxLenReport = MAX_len_report);
	int GetReportW(CString &strReport, int nMaxLenReport = MAX_len_report);
	BOOL WriteToExcelWorksheet(_Worksheet &ws,Range &rngLeftTopCorner,Range *prngSquare);

protected:
	// объект отчета
	::Report *pRep;
// Входные данные для текущего расчета
	int nCurrUXF;
	int nCurrP;
	int nCurrParam;
	// Флаг перехода на новое альфа
	BOOL bWasNextAlfa;	

	float fAllEnthalpyF[MaxQuantityFIHBox];	// Энтальпия топлива
	int nEnthalpyF;
	float fAllP[MaxQuantityFIHBox];				// Давление в КС
	int nP;
	float fAllAlfa[MaxQuantityFIHBox];
	float fKappaStroke0[MaxQuantityFIHBox];
	int nAlfa;
	float fAllParam[MaxQuantityFIHBox];
	int nParam;

// Связь
	CTherDynEqCalc *pTherDynEqCalc;
	THERDYNEQCALCSTRUCT tdecs;

// Массивы для хранения результатов всех расчетов
	float fAllProcessP[MaxQuantityFIHBox][MaxQuantityFIHBox][MaxQuantityFIHBox];	// Давление процесса полученное после основного расчета
	float fAllT[MaxQuantityFIHBox][MaxQuantityFIHBox][MaxQuantityFIHBox];	// Сохраним для каждого альфа, давления и Pa(F,IS)- температуру
	float fAllI[MaxQuantityFIHBox][MaxQuantityFIHBox][MaxQuantityFIHBox];	// Сохраним для каждого альфа и давления - энтальпию
	float fAllS[MaxQuantityFIHBox][MaxQuantityFIHBox][MaxQuantityFIHBox];	// Сохраним для каждого альфа и давления - энтропию
	float fAllMS[MaxQuantityFIHBox][MaxQuantityFIHBox][MaxQuantityFIHBox];
	float fAllCP3[MaxQuantityFIHBox][MaxQuantityFIHBox][MaxQuantityFIHBox];
	float fAllK3[MaxQuantityFIHBox][MaxQuantityFIHBox][MaxQuantityFIHBox];
	float fAllB[MaxQuantityFIHBox][MaxQuantityFIHBox][MaxQuantityFIHBox];		// Расходный комплекс
	float fAllPYD[MaxQuantityFIHBox][MaxQuantityFIHBox][MaxQuantityFIHBox];
	float fAllPYDP[MaxQuantityFIHBox][MaxQuantityFIHBox][MaxQuantityFIHBox];
 	float fAllF[MaxQuantityFIHBox][MaxQuantityFIHBox][MaxQuantityFIHBox];		// Удельная площадь сечения
	float fAllF_div_FKP[MaxQuantityFIHBox][MaxQuantityFIHBox][MaxQuantityFIHBox];
	float fAllZK[MaxQuantityFIHBox][MaxQuantityFIHBox][MaxQuantityFIHBox];

	// Текущие входные данные
	float PP;
	float T;												// Температура ПГ
	float T1;
	float T2;
// Вспомогательные переменные
	float fAssumedT;
	float fMinT;
	float fMaxT;
};

class CalcCameraCombustion : public CalcBase {

public:
	CalcCameraCombustion ();
	BOOL Construct (CTherDynEqCalc *p, UXFSTRUCT _uxf[MaxQuantityFIHBox], CALCSTRUCT _calcs);
	BOOL Calc(ProductsCombustion &PC, INTOCALCSTRUCT &ics);
protected:
// Вспомогательные переменные
	float ALFA[5];
	int IA;
	float KAPPA;
};

const char szReport_T[] = "Температура";
const char szReport_unitT[] = "К";
const char szReport_I[] = "Энтальпия";
const char szReport_unitI[] = "кДж/кг";
const char szReport_S[] = "Энтропия";
const char szReport_unitS[] = "кДж/(кг*К)";
const char szReport_P[] = "Давление";
const char szReport_unitP[] = "МПа";
const char szReport_MS[] = "Молек. масса смеси";
const char szReport_unitMS[] = "кг/моль";
const char szReport_CP3[] = "Сp3";
const char szReport_unitCP3[] = "кДж/(кг*К)";
const char szReport_K3[] = "K3";
const char szReport_unitK3[] = " ";
const char szReport_B[] = "Расходный комплекс";
const char szReport_unitB[] = "с";
const char szReport_PYD[] = "Уд. импульс";
const char szReport_unitPYD[] = "м/с";
const char szReport_PYDP[] = "Уд. импульс в пуст.";
const char szReport_unitPYDP[] = "м/с";
const char szReport_F[] = "Удельная площ. сеч.";
const char szReport_unitF[] = "м**2*с/кг";
const char szReport_F_div_FKP[] = "Степень расш. сопла";
const char szReport_unitF_div_FKP[] = " ";
const char szReport_ZK[] = "Доля конденсата";
const char szReport_unitZK[] = " ";

class CalcCriticalSection : public CalcBase {

public:
	CalcCriticalSection ();
	BOOL Construct (CTherDynEqCalc *p, UXFSTRUCT _uxf[MaxQuantityFIHBox], CALCSTRUCT _calcs);
	BOOL Calc(ProductsCombustion &PC, INTOCALCSTRUCT &ics);

protected:
// Вспомогательные переменные
	float PPK;
	float PPA;
	float W;
	float PYD;
	float PYDP;
	float BT;
	float BTMIN;
	float _ST[12];
};

class CalcAnySectionWithPa : public CalcBase {

public:
	CalcAnySectionWithPa ();
	BOOL Construct (CTherDynEqCalc *p, UXFSTRUCT _uxf[MaxQuantityFIHBox], CALCSTRUCT _calcs);
	BOOL Calc(ProductsCombustion &PC, INTOCALCSTRUCT &ics);

protected:
// Вспомогательные переменные
	float PPK;
	float PPA;
	float W;
	float PYD;
	float PYDP;
	float BT;
	float E;
};

class CalcAnySectionWithF : public CalcBase {

public:
	CalcAnySectionWithF ();
	BOOL Construct (CTherDynEqCalc *p, UXFSTRUCT _uxf[MaxQuantityFIHBox], CALCSTRUCT _calcs);
	BOOL Calc (ProductsCombustion &PC, INTOCALCSTRUCT &ics);

protected:
// Вспомогательные переменные
	float PPK;
	float PPA;
	float W;
	float PYD;
	float PYDP;
	float BT;
	float E;
	int DA;
	float FA;
	float PX;
	float PY;
	float FX;
	float FY;
	float fDifference;
};

class CalcISDiagram : public CalcBase {

public:
	CalcISDiagram ();
	BOOL Construct (CTherDynEqCalc *p, UXFSTRUCT _uxf[MaxQuantityFIHBox], CALCSTRUCT _calcs);
	BOOL Calc (ProductsCombustion &PC, INTOCALCSTRUCT &ics);

protected:
// Вспомогательные переменные
	float PPK;
	float PPA;
	float W;
	float PYD;
	float PYDP;
	float BT;
	float E;
};

class CalcConstV : public CalcBase {

public:
	CalcConstV ();
	BOOL Construct (CTherDynEqCalc *p, UXFSTRUCT _uxf[MaxQuantityFIHBox], CALCSTRUCT _calcs);
	BOOL Calc (ProductsCombustion &PC, INTOCALCSTRUCT &ics);
};

class CalcEPS : public CalcBase {

public:
	CalcEPS ();
	BOOL Construct (CTherDynEqCalc *p, UXFSTRUCT _uxf[MaxQuantityFIHBox], CALCSTRUCT _calcs);
	BOOL Calc (ProductsCombustion &PC, INTOCALCSTRUCT &ics);

protected:
// Вспомогательные переменные
	float PPK;
	float PPA;
	float W;
	float PYD;
	float PYDP;
	float BT;
	float E;
};

class FirstApproximation {

public:
	FirstApproximation ();
	BOOL Construct(JobWithDB *p, UXFSTRUCT _uxf[MaxQuantityFIHBox], CALCSTRUCT _calcs);
	BOOL Calc(ProductsCombustion &PC, INTOCALCSTRUCT &ics);
	void PrepareToFirstCalc();
	BOOL NextAlfa();

protected:
	JobWithDB *pDBs;
	float XF1[120];
	float XF2[120];
	UXFSTRUCT fa_uxf[MaxQuantityFIHBox];
	int nCurrUXF;
	int nAlfa;

	float MC[PERIOD_TABLE_size];
};

class JobWithDB {

public:
	JobWithDB();
	BOOL Construct(UXFSTRUCT _uxf);
	BOOL ReadDBs();
	BOOL GetNamesPC(ProductsCombustion &PC);
	BOOL GetTERMO_XF1andXF2(float _XF1[120], float _XF2[120]);
	void GetAllchem_data(int &ext_nPElementaryElem, int &ext_nPGas, 
			chem_dataGas ext_cdG[MaxElemGasFromDB], 
			int &ext_nPCond, chem_dataCond ext_cdC[MaxElemCondFromDB]);
	void GetNamesP(char szNam[MaxElemGasFromDB][MAX_len], unsigned int &nMaxNamesLen);

protected:
	BOOL OpenAndReadGasDB();
	BOOL OpenAndReadCondensateDB();

	// Для отладки нужны как внешние что бы видеть данные в отладчике как массив
	// потом будут как указатели на переменные
	//char szNam[MaxElemGasFromDB][MAX_len];
	unsigned int nMaxNamesLen;

	UXFSTRUCT uxf_with_common_atoms;
	chem_dataGas cdG[MaxElemGasFromDB];
	chem_dataCond cdC[MaxElemCondFromDB];
	int nPElementaryElem;
	int nPGas;
	int nPCond;
	int nPAll;
	BOOL 	bWasReadDBs;
};
