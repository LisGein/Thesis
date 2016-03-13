// ThermoCalcDoc.cpp : implementation of the CThermoCalcDoc class
//

#include "stdafx.h"
#include <math.h>

#include "ThermoCalc.h"
#include "Calc.h"
#include "ThermoCalcDoc.h"
#include "ThermoCalcView.h"
#include "MainFrm.h"
#include "_from_termo.h"
#include "all_CCalc.h"
#include "all_excel.h"// _Application, Workbooks, Worksheets, _Worksheet, Range

//#include "table.h"
extern int Valence[];
extern double M[];
extern char cf[3];

#pragma warning (disable : 4244)

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

char szNam[MaxElemGasFromDB][MAX_len];
// Вспомогательные временные переменные
char szElemsFromDB[MaxAtoms][MaxAtomNameLen];	// Для разбора эл. из базы
float fT[MaxAtoms];										// Для разбора эл. из базы

// После разработки должны быть в документе
extern double ENT[120];
extern double S[120];
extern double CP[120];
extern float ALFA[25],PA[25];
extern float KAPPA;
extern int NA,NPA;
extern long IPK,IA,IPA;
extern double PPK,PKP;
extern float FX,FY,PX,PY;
extern BOOL F3,FF,FE;
extern CLimitString sSaveAsExcel;
float P[MaxElemGasFromDB];
float dMolMas[MaxElemGasFromDB];	// Расчет мольных масс для всех соединений
float P_PrP[MaxElemGasFromDB];

/////////////////////////////////////////////////////////////////////////////
// CThermoCalcDoc

IMPLEMENT_DYNCREATE(CThermoCalcDoc, CDocument)

BEGIN_MESSAGE_MAP(CThermoCalcDoc, CDocument)
	//{{AFX_MSG_MAP(CThermoCalcDoc)
		// NOTE - the ClassWizard will add and remove mapping macros here.
		//    DO NOT EDIT what you see in these blocks of generated code!
	//}}AFX_MSG_MAP
END_MESSAGE_MAP()

/////////////////////////////////////////////////////////////////////////////
// CThermoCalcDoc construction/destruction

CThermoCalcDoc::CThermoCalcDoc() {
	// TODO: add one-time construction code here
	pDlgView = 0;
	pViewOutput = 0;
	memset(&calcs,0,sizeof(calcs));
//	os.bBurningFlag = FALSE;
//	os.bFlowFlag = FALSE;
	bMaybeChangeCS = FALSE;
	bMaybeChangeUXF = FALSE;
	nSections = 0;
	memset(&os,0,sizeof(os));
	fAtmospherePressure = (float)0.1;
};

CThermoCalcDoc::~CThermoCalcDoc() { }

BOOL CThermoCalcDoc::OnNewDocument() {
	if (!CDocument::OnNewDocument())
		return FALSE;

	// TODO: add reinitialization code here
	// (SDI documents will reuse this document)
	CMainFrame *pMF = (CMainFrame *)AfxGetMainWnd();
	pDlgView =  pMF ->GetDlgView();

	POSITION pos = GetFirstViewPosition();
	pViewOutput = (CViewOutput *)GetNextView(pos);
	ASSERT (pos == 0);
	pFrameOutput = (CFrameOutput *)pViewOutput ->GetParentFrame();

	//if(m_bEmbedded)	// Если документ не создан видимым
	//	m_bEmbedded = m_bEmbedded;

	return TRUE;
}

/////////////////////////////////////////////////////////////////////////////
// CThermoCalcDoc serialization

void CThermoCalcDoc::Serialize(CArchive& ar)
{
	char c[MaxAtomNameLen];
	memset (c, 0, sizeof(c));
	CString S;
	int i1, i2;

	if (ar.IsStoring())
	{
		// TODO: add storing code here
		ar << calcs.nEditBoxesOx;
		for (i1 = 0; i1<calcs.nEditBoxesOx; i1++) {
			ar << calcs.percent_oxides[i1];
			ar << calcs.nAtomsInStringOx[i1];
			for (i2 = 0; i2<calcs.nAtomsInStringOx[i1]; i2++) {
				//memset(c,0,sizeof(c);
				S = calcs.szNameAtomOfOxides[i1][i2];
				ar << S;
				ar << calcs.factor_oxides[i1][i2];
			}
		}

		ar << calcs.nEditBoxesComb;
		for (i1 = 0; i1<calcs.nEditBoxesComb; i1++) {
			ar << calcs.percent_combust[i1];
			ar << calcs.nAtomsInStringComb[i1];
			for (i2 = 0; i2<calcs.nAtomsInStringComb[i1]; i2++) {
				S = calcs.szNameAtomOfCombust[i1][i2];
				ar << S;
				ar << calcs.factor_combust[i1][i2];
			}
		}

		ar << calcs.nEditBoxesAlfa;
		for (i1 = 0; i1<calcs.nEditBoxesAlfa; i1++) {
			ar << calcs.fAlfa[i1];
		}
	}
	else
	{
		// TODO: add loading code here
		ar >> calcs.nEditBoxesOx;
		if (calcs.nEditBoxesOx>MaxEditBoxesOx) {
			//MessageBox(m_hWnd,"Bad thermo file, load abort","Error",MB_OK);
			MessageBox(0,"Bad thermo file, invalid 'calcs.nEditBoxesOx', load abort","Load error",MB_OK);
			return;
		}

		for (i1 = 0; i1<calcs.nEditBoxesOx; i1++) {
			ar >> calcs.percent_oxides[i1];
			ar >> calcs.nAtomsInStringOx[i1];
			if (calcs.nAtomsInStringOx[i1]>MaxAtoms) {
				MessageBox(0,"Bad thermo file, invalid 'calcs.nAtomsInStringOx[i1]', load abort","Load error",MB_OK);
				return;
			}
			for (i2 = 0; i2<calcs.nAtomsInStringOx[i1]; i2++) {
				//memset(c,0,sizeof(c);
				ar >> S;
				if (S.GetLength()>MaxAtomNameLen) {
					MessageBox(0,"Bad thermo file, invalid 'calcs.szNameAtomOfOxides[i1][i2]', load abort","Load error",MB_OK);
					return;
				}
				strcpy(calcs.szNameAtomOfOxides[i1][i2],S);
				ar >> calcs.factor_oxides[i1][i2];
			}
		}

		ar >> calcs.nEditBoxesComb;
		if (calcs.nEditBoxesComb>MaxEditBoxesComb) {
			MessageBox(0,"Bad thermo file, invalid 'calcs.nEditBoxesComb', load abort","Error",MB_OK);
			return;
		}
		for (i1 = 0; i1<calcs.nEditBoxesComb; i1++) {
			ar >> calcs.percent_combust[i1];
			ar >> calcs.nAtomsInStringComb[i1];
			if (calcs.nAtomsInStringComb[i1]>MaxAtoms) {
				MessageBox(0,"Bad thermo file, invalid 'calcs.nAtomsInStringComb[i1]', load abort","Error",MB_OK);
				return;
			}
			for (i2 = 0; i2<calcs.nAtomsInStringComb[i1]; i2++) {
				ar >> S;
				if (S.GetLength()>MaxAtomNameLen) {
					MessageBox(0,"Bad thermo file, invalid 'calcs.szNameAtomOfCombust[i1][i2]', load abort","Load error",MB_OK);
					return;
				}
				strcpy(calcs.szNameAtomOfCombust[i1][i2],S);
				ar >> calcs.factor_combust[i1][i2];
			}
		}

		ar >> calcs.nEditBoxesAlfa;
		for (i1 = 0; i1<calcs.nEditBoxesAlfa; i1++) {
			ar >> calcs.fAlfa[i1];
		}

		Calc(0);
		bMaybeChangeCS = FALSE;
		bMaybeChangeUXF = FALSE;
		pDlgView -> ShowLoadView(calcs,uxf);
	}
}

/////////////////////////////////////////////////////////////////////////////
// CThermoCalcDoc diagnostics

#ifdef _DEBUG
void CThermoCalcDoc::AssertValid() const
{
	CDocument::AssertValid();
}

void CThermoCalcDoc::Dump(CDumpContext& dc) const
{
	CDocument::Dump(dc);
}
#endif //_DEBUG

/////////////////////////////////////////////////////////////////////////////
// CThermoCalcDoc commands

BOOL CThermoCalcDoc::CalcAndShow() {
	return CalcAndShow(0);
}

BOOL CThermoCalcDoc::CalcAndShow(int _nCurrentUXF) {
	if (!Calc(_nCurrentUXF)) {
		return FALSE;
	}

	//m_bModified = m_bModified;
	// Нужна поддержка опций вывода результатов
	pDlgView ->GetOptionsFromDlg(CThermoCalcDoc::os);
	// Активизировать окно вывода
	if ( ((CMDIFrameWnd *)AfxGetMainWnd()) ->MDIGetActive() != pFrameOutput ) {
		if(bInvisibleFlag) {
			// Show
			POSITION pos = GetFirstViewPosition();
			CViewOutput *pViewOutput = (CViewOutput *)GetNextView(pos);					// CView
			ASSERT (pos == 0);
			CFrameOutput *pFrameOutput = (CFrameOutput *)pViewOutput ->GetParentFrame();	// CFrameWnd
			pFrameOutput ->InitialUpdateFrame(this,TRUE);
			bInvisibleFlag = FALSE;
		}
		((CMDIFrameWnd *)AfxGetMainWnd()) ->MDIActivate(pFrameOutput);
	}

	CViewOutput *pVO = (CViewOutput *) pFrameOutput->GetActiveView();
	if (pVO ->Show(reportTextBuffer))
		SetModifiedFlag();
/*
	CViewOutput *pVO = (CViewOutput *) pFrameOutput->GetActiveView();
	if (pVO ->Show(CThermoCalcDoc::os,
			P,szNam,nPGas,nPCond,nPAll,nMaxNamesLen,
			calcs,uxf,ParamInCC,ParamInCriticalSection,ParamInSections,nSections))
		SetModifiedFlag();
*/
	//m_bModified = m_bModified;
	return TRUE;
}

void CThermoCalcDoc::ShowFull()
{
	char szOut[MAX_len];
	int  decimal, sign;
	int	precision = 8;
	char *buffer;
	//char c[80];
	CString S;
	float fSum;
	int cur_elem;

	SetModifiedFlag();
	CRichEditCtrl *pE = &(pViewOutput ->GetRichEditCtrl( ));
	pE ->ReplaceSel("Температура = ");
	ToDotString( T, S);		//_gcvt();- Но она вставляет экспоненту если превышает сотые доли
	pE ->ReplaceSel( S );
	pE ->ReplaceSel(" К;");
	pE ->ReplaceSel( cf );
	S.Empty();

	pE ->ReplaceSel("Энтальпия = ");
	ToDotString(((float)IS), S);
	pE ->ReplaceSel( S );
	pE ->ReplaceSel(" кДж/кг;");
	pE ->ReplaceSel( cf );
	S.Empty();

	// умножить на 1.03323 для Па
	pE ->ReplaceSel("Давление в КС = ");
	ToDotString(PP, S);
	pE ->ReplaceSel( S );
	pE ->ReplaceSel(" МПа;");
	pE ->ReplaceSel( cf );
	S.Empty();

	switch (os.nPressureOption) {
		case cnPrM:
			pE ->ReplaceSel("Мольные доли (моль/кг), Astra view, <PrM>:");
			pE ->ReplaceSel(cf);
			// Astra view мольные доли <PrM> моль/кг
			for (cur_elem=1;cur_elem<=nPAll;cur_elem++) {
				strncpy(szOut, szNam[cur_elem], nMaxNamesLen+1);
				if (P[cur_elem]>0)
					buffer = _fcvt(P[cur_elem],precision*2,&decimal,&sign);
				else
					buffer = _fcvt(exp(P[cur_elem]),precision*4,&decimal,&sign);

				if (abs(decimal)>precision) {
					continue;
					//for (int ii=0;ii<precision;ii++)
					//	szOut[nMaxNamesLen+1+ii]='0';
				}
				else
					strcpy(&szOut[nMaxNamesLen+1],buffer);
				szOut[nMaxNamesLen+1+precision] = 0;

				if (decimal>0)
					strcpy(&szOut[strlen(szOut)],"+O");
				else
					strcpy(&szOut[strlen(szOut)],"-O");

				gcvt(abs(decimal),3,&szOut[strlen(szOut)]);
				// Show
				pE ->ReplaceSel(szOut);	//CEdit
				pE ->ReplaceSel(cf);
			}
		break;

		case cnPrP:
			pE ->ReplaceSel("Парциальные давления (МПа), Astra view, <PrP>:");
			pE ->ReplaceSel(cf);
			fSum = 0;
			for (cur_elem=1;cur_elem<=nPAll;cur_elem++)
				if (P[cur_elem]>0)
					fSum += P[cur_elem];
			// Astra view Парциальное давление <PrP> МПа
			for (cur_elem=1;cur_elem<=nPAll;cur_elem++) {
				strncpy(szOut, szNam[cur_elem], nMaxNamesLen+1);
				if (P[cur_elem]>0)
					buffer = _fcvt(PP*P[cur_elem]/fSum,precision*2,&decimal,&sign);
				else
					buffer = _fcvt(PP*exp(P[cur_elem])/fSum,precision*4,&decimal,&sign);

				if (abs(decimal)>precision) {
					continue;
					//for (int ii=0;ii<precision;ii++)
					//	szOut[nMaxNamesLen+1+ii]='0';
				}
				else
					strcpy(&szOut[nMaxNamesLen+1],buffer);
				szOut[nMaxNamesLen+1+precision] = 0;

				if (decimal>0)
					strcpy(&szOut[strlen(szOut)],"+O");
				else
					strcpy(&szOut[strlen(szOut)],"-O");

				gcvt(abs(decimal),3,&szOut[strlen(szOut)]);
				// Show
				pE ->ReplaceSel(szOut);	//CEdit
				pE ->ReplaceSel(cf);
			}
		break;

	}
}

void CThermoCalcDoc::ShowShort()	{	}

BOOL CThermoCalcDoc::CalcUXF() {
	for (int i = 0; i<calcs.nEditBoxesAlfa; i++) {
		if (!CalcUXF(i))
			return FALSE;
	}

	bMaybeChangeCS = FALSE;
	bMaybeChangeUXF = FALSE;
	return TRUE;
}

BOOL CThermoCalcDoc::CalcUXF(int nCurrentAlfa) {
// I. Находим условную хим. формулу окислителя и горючего
	
	//"Условная хим. формула окислителя:";
	int cur_molecule;
	float fMolecWeightO[MaxEditBoxes];								// Молекулярные веса компонентов окислителя
	memset (fMolecWeightO,0,sizeof(fMolecWeightO));
	float fWeightPortionsO[MaxEditBoxes][MaxAtoms];			// Весовые доли
	memset (fWeightPortionsO,0,sizeof(fWeightPortionsO));

	for(cur_molecule=0;cur_molecule<calcs.nEditBoxesOx;cur_molecule++)
	{
		int index;
// а) Расчитываем молекулярные веса компонентов окислителя
		int cur_atom;
		for(cur_atom=0;cur_atom<calcs.nAtomsInStringOx[cur_molecule];cur_atom++)
		{	// Осутствует проверка GetSN
			index = GetSN(calcs.szNameAtomOfOxides[cur_molecule][cur_atom]);
			fMolecWeightO[cur_molecule] += (float)M[index]
				* calcs.factor_oxides[cur_molecule][cur_atom];
		}

// б) Расчитываем весовые доли элементов в компонентах окислителя
		for(cur_atom=0;cur_atom<calcs.nAtomsInStringOx[cur_molecule];cur_atom++)
		{	// Осутствует проверка GetSN
			index = GetSN(calcs.szNameAtomOfOxides[cur_molecule][cur_atom]);
			fWeightPortionsO[cur_molecule][cur_atom] = (float)(M[index] * calcs.factor_oxides[cur_molecule][cur_atom])
				/fMolecWeightO[cur_molecule];
		}
	}
	// Проверка
	//	for(cur_molecule=0;cur_molecule<calcs.nEditBoxesOx;cur_molecule++)
	//		for(int cur_atom=0;cur_atom<calcs.nAtomsInStringOx[cur_molecule];cur_atom++)
	//			float f = fWeightPortionsO[cur_molecule][cur_atom];

// в) Весовое содержание элементов в окислителе
	float fWeightContentO[MaxAtoms];
	memset (fWeightContentO,0,sizeof(fWeightContentO));
	int nCurWeightContentO;

	char szWasO[MaxAtoms][MaxAtomNameLen];
	memset (szWasO,0,sizeof(szWasO));
	int nWasO[MaxAtoms];
	memset (nWasO,0,sizeof(nWasO));
	int	nCurWasO=0;

	nCurWeightContentO = 0;
	for(cur_molecule=0;cur_molecule<calcs.nEditBoxesOx;cur_molecule++)
	{
		BOOL bWasFlag;
		for(int cur_atom=0;cur_atom<calcs.nAtomsInStringOx[cur_molecule];cur_atom++)
		{
			bWasFlag = FALSE;
			int f;
			for(f=0;f<nCurWasO;f++)
				if (!strcmp(szWasO[f], calcs.szNameAtomOfOxides[cur_molecule][cur_atom]) )
				{
					bWasFlag = TRUE;
					break;
				}
			
			if (bWasFlag)	// szWas[f] индекс повторяющегося элемента
			{
				fWeightContentO[nWasO[f]] += calcs.percent_oxides[cur_molecule]
					*fWeightPortionsO[cur_molecule][cur_atom];
			}
			else
			{
				strcpy (szWasO[nCurWasO],calcs.szNameAtomOfOxides[cur_molecule][cur_atom]);
				nWasO[nCurWasO] = nCurWeightContentO; 
				nCurWasO++;

				fWeightContentO[nCurWeightContentO] += calcs.percent_oxides[cur_molecule]
					*fWeightPortionsO[cur_molecule][cur_atom];
				nCurWeightContentO++;
			}
		}
	}

	int nQuantityElemO = nCurWeightContentO;

	float fConditChimFormO[MaxAtoms];			// Условн. хим. формула окислителя
	memset (fConditChimFormO,0,sizeof(fConditChimFormO));

// д) расчет условной химической формулы окислителя
// при условном молекулярном весе == 100
	int cur_elem;
	for(cur_elem=0;cur_elem<nQuantityElemO;cur_elem++)
	{
		int index;
		index = GetSN(szWasO[cur_elem]);
		fConditChimFormO[cur_elem] = 
			fWeightContentO[cur_elem]*fConditMolecularWeightO
			/(float)M[index];
	}


// "Условная хим. формула горючего";
	float fMolecWeightC[MaxEditBoxes];								// Молекулярные веса компонентов горючего
	memset (fMolecWeightC,0,sizeof(fMolecWeightC));
	float fWeightPortionsC[MaxEditBoxes][MaxAtoms];			// Весовые доли
	memset (fWeightPortionsC,0,sizeof(fWeightPortionsC));

	for(cur_molecule=0;cur_molecule<calcs.nEditBoxesComb;cur_molecule++)
	{
		int index;
// Расчитываем молекулярные веса компонентов горючего
		int cur_atom;
		for(cur_atom=0;cur_atom<calcs.nAtomsInStringComb[cur_molecule];cur_atom++)
		{	// Осутствует проверка GetSN
			index = GetSN(calcs.szNameAtomOfCombust[cur_molecule][cur_atom]);
			fMolecWeightC[cur_molecule] += (float)M[index]
				* calcs.factor_combust[cur_molecule][cur_atom];
		}

		//fWeightPortionsC[cur_molecule] = new float[calcs.nAtomsInStringComb[cur_molecule]];
// Расчитываем весовые доли элементов в компонентах горючего
		for(cur_atom=0;cur_atom<calcs.nAtomsInStringComb[cur_molecule];cur_atom++)
		{	// Осутствует проверка GetSN
			index = GetSN(calcs.szNameAtomOfCombust[cur_molecule][cur_atom]);
			fWeightPortionsC[cur_molecule][cur_atom] = (float)(M[index] * calcs.factor_combust[cur_molecule][cur_atom])
				/fMolecWeightC[cur_molecule];
		}
	}

// Весовое содержание элементов в горючем
	float fWeightContentC[MaxAtoms];
	memset (fWeightContentC,0,sizeof(fWeightContentC));
	int nCurWeightContentC;

	char szWasC[MaxAtoms][MaxAtomNameLen];
	memset (szWasC,0,sizeof(szWasC));
	int nWasC[MaxAtoms];
	memset (nWasC,0,sizeof(nWasC));
	int	nCurWasC = 0;

	nCurWeightContentC = 0;
	for(cur_molecule=0;cur_molecule<calcs.nEditBoxesComb;cur_molecule++)
	{
		//int nAtoms;
		//for(nAtoms=0;(int)szNameAtomOfCombust[cur_molecule][nAtoms]!=0;nAtoms++)
		//	;
		BOOL bWasFlag;
		for(int cur_atom=0;cur_atom<calcs.nAtomsInStringComb[cur_molecule];cur_atom++)
		{
			bWasFlag = FALSE;
			int f;
			for(f=0;f<nCurWasC;f++) 
			{
				if (!strcmp(szWasC[f],
					calcs.szNameAtomOfCombust[cur_molecule][cur_atom]) )
				{
					bWasFlag = TRUE;
					break;
				}
			}

			if (bWasFlag)	// szWas[f] индекс повторяющегося элемента
			{
				fWeightContentC[nWasC[f]] += calcs.percent_combust[cur_molecule]
					*fWeightPortionsC[cur_molecule][cur_atom];
			}
			else
			{
				strcpy(szWasC[nCurWasC], calcs.szNameAtomOfCombust[cur_molecule][cur_atom]);
				nWasC[nCurWasC] = nCurWeightContentC; 
				nCurWasC++;

				fWeightContentC[nCurWeightContentC] += calcs.percent_combust[cur_molecule]
					*fWeightPortionsC[cur_molecule][cur_atom];
				nCurWeightContentC++;
			}
		}
	}
	
	int nQuantityElemC = nCurWeightContentC;

	float fConditChimFormC[MaxAtoms];			// Условн. хим. формула горючего
	memset (fConditChimFormC,0,sizeof(fConditChimFormC));

// Расчет условной химической формулы горючего
// при условном молекулярном весе == ConditMolecularWeightC
	for(cur_elem=0;cur_elem<nQuantityElemC;cur_elem++)
	{
		int index;
		index = GetSN(szWasC[cur_elem]);
		fConditChimFormC[cur_elem] = 
			fWeightContentC[cur_elem] * fConditMolecularWeightC
			/(float)M[index];
	}

// 2. Определим стехиометрическое соотношение горючего и
// окислителя
// Kappa_штрих_0

	// в числителе
	float fNumerator = 0;
	for(cur_elem=0;cur_elem<nQuantityElemC;cur_elem++) {
		int index = GetSN(szWasC[cur_elem]);
		fNumerator += Valence[index] * fConditChimFormC[cur_elem];
	}

	// в знаменателе
	float fDenominator = 0;
	for(cur_elem=0;cur_elem<nQuantityElemO;cur_elem++) {
		int index = GetSN(szWasO[cur_elem]);
		fDenominator += Valence[index] * fConditChimFormO[cur_elem];
	}
	float fKappaStroke0 = -fNumerator/fDenominator;

	if(fKappaStroke0<0) {
		MessageBox(0,"Вероятно перепутаны формулы окислителя и горючего. Ошибка расчета УХФ, каппа_штрих_0.","Error - CThermoCalcDoc::CalcUXF(int nCurrentAlfa);",MB_OK);
		return FALSE;
	}

	calcs.fKappaStroke0[nCurrentAlfa] = fKappaStroke0;

// Весовое стехиометрическое соотношение
// Kappa 0
	float fKappa0 = (fConditMolecularWeightO
		/fConditMolecularWeightC) * fKappaStroke0;

// 3. Определим действительное соотношение компонентов
// Kappa
	float fKappa = calcs.fAlfa[nCurrentAlfa] * fKappa0;

	//fKappa = (float)2.65; //Ошибка в старой белой методичке

// 4. Определяем весовые доли элементов в составе топлива
	float fWeightPortionsF[MaxAtoms];

	BOOL bWasCalculated[MaxAtoms];
	memset(bWasCalculated,0,sizeof(bWasCalculated));

	// Возмем элемент горючего
	// и поищем во всех элементах окислителя такой же
	for(cur_elem=0;cur_elem<nQuantityElemC;cur_elem++)
	{
		fWeightPortionsF[cur_elem] = fWeightContentC[cur_elem];
		strcpy(uxf[nCurrentAlfa].szAtomsOfF[cur_elem],szWasC[cur_elem]);

		for(int elemO=0;elemO<nQuantityElemO;elemO++)
		{
			if(!strcmp(szWasC[cur_elem],szWasO[elemO]))
			{
				fWeightPortionsF[cur_elem] 
					+= fKappa*fWeightContentO[elemO];
				bWasCalculated[elemO]=TRUE;
				break;
			}
		}
		fWeightPortionsF[cur_elem] /= (1 + fKappa);
	}
	uxf[nCurrentAlfa].nQuantityElemF = cur_elem;

	// Поишем неучтенные элементы окислителя
	for(cur_elem=0;cur_elem<nQuantityElemO;cur_elem++)
	{
		if(!bWasCalculated[cur_elem])// Если нашли неучтенный элемент
		{
			fWeightPortionsF[uxf[nCurrentAlfa].nQuantityElemF] 
				= (fKappa*fWeightContentO[cur_elem]) / (1 + fKappa);
			strcpy(uxf[nCurrentAlfa].szAtomsOfF[uxf[nCurrentAlfa].nQuantityElemF],
				szWasO[cur_elem]);
			uxf[nCurrentAlfa].nQuantityElemF ++;
		}
	}

// 5. Определяем состав топлива
// (расчет условной химической формулы топлива)
	for(cur_elem=0;cur_elem<uxf[nCurrentAlfa].nQuantityElemF;cur_elem++)
	{
		int index;
		index = GetSN(uxf[nCurrentAlfa].szAtomsOfF[cur_elem]);
		uxf[nCurrentAlfa].fConditChimFormF[cur_elem] =
			fWeightPortionsF[cur_elem] * 1000 / (float)M[index];
	}

// II. Определяем полное теплосодержание топлива

//		а) Энтальпия горючего
	float fEnthalpyC = 0;
	for(cur_elem=0;cur_elem<calcs.nEditBoxesComb;cur_elem++)
		fEnthalpyC += calcs.percent_combust[cur_elem]*calcs.fEnthalpyComb[cur_elem];
//		б) Энтальпия окислителя
	float fEnthalpyO = 0;
	for(cur_elem=0;cur_elem<calcs.nEditBoxesOx;cur_elem++)
		fEnthalpyO += calcs.percent_oxides[cur_elem]*calcs.fEnthalpyOx[cur_elem];
//		в) Полное теплосодержание топлива
	uxf[nCurrentAlfa].fEnthalpyF = (fEnthalpyC + fKappa*fEnthalpyO)/(1 + fKappa);
	return TRUE;
}

BOOL CThermoCalcDoc::Calc(int _nCurrentUXF) {

	// Проверим основные параметры расчета на допустимость
//	if(calcs.nEditBoxesPressure<1)		return FALSE
//	if(calcs.nEditBoxesAlfa<1)		return FALSE

	// Если нужно расчитать УХФ
	if (bMaybeChangeCS||bMaybeChangeUXF)
		if (!CalcUXF())
			return FALSE;

// RegulateEntalpy
// RegulateEntropy
// термодинамический - thermodynamic
// equilibrium - равновесие, равновесный
// ThermodynamicEquilibriumCalc
// TherDynEqCalc
// TherDynEqCalcConstEntalpy
// TherDynEqCalcConstEntropy
// burning - горение
// flow - течение

	// Работа с опциями
	pDlgView ->GetOptionsFromDlg(CThermoCalcDoc::os);
	pViewOutput ->GetOptionsFromOutput(CThermoCalcDoc::os);

	db.Construct(uxf[0]);
	BOOL r = db.ReadDBs();
	if(!r) return FALSE;

	ProductsCombustion CurrPC;
	memset(&CurrPC, 0, sizeof(CurrPC));
	r = db.GetNamesPC(CurrPC);
	if(!r) return FALSE;

	tdec.Construct(&db);
	fa.Construct(&db, uxf, calcs);

	ccc.Construct(&tdec, uxf, calcs);
	ccc.SetOptionsReport(os);
	ccc.PrepareToFirstCalc();

	ccs.Construct(&tdec, uxf, calcs);
	ccs.SetOptionsReport(os);
	ccs.PrepareToFirstCalc();

// Запуск возможных констукторов
	// Если есть опция: расчитать выходное сечение при атм. давлении
	if(calcs.bIncludeAtmosphereSection) {
		float fRes = calcs.fPa[0];
		int	nRes = calcs.nPa;
		calcs.fPa[0] = fAtmospherePressure;
		calcs.nPa = 1;
		caswpa_atm.Construct(&tdec, uxf, calcs);
		calcs.fPa[0] = fRes;
		calcs.nPa = nRes;
		caswpa_atm.SetOptionsReport(os);
		caswpa_atm.PrepareToFirstCalc();
	}
	// Для различных вариантов, расчета течения
		//1) Для расчета течения по давлениям в сечениях
	if(calcs.bIncludePa) {
		caswpa.Construct(&tdec, uxf, calcs);
		caswpa.SetOptionsReport(os);
		caswpa.PrepareToFirstCalc();
	}
		//2) Для расчета течения, по степени расширения сечения
	if(calcs.bIncludeF) {
		caswf.Construct(&tdec, uxf, calcs);
		caswf.SetOptionsReport(os);
		caswf.PrepareToFirstCalc();
	}
		//3) Для расчета IS диаграммы
	if(calcs.bIncludeIS) {
		cisd.Construct(&tdec, uxf, calcs);
		cisd.SetOptionsReport(os);
		cisd.PrepareToFirstCalc();
	}

		//4) Для расчета горения при постоянном объеме
	if(calcs.bIncludeV) {
		ccv.Construct(&tdec, uxf, calcs);
		ccv.SetOptionsReport(os);
		ccv.PrepareToFirstCalc();
	}

	INTOCALCSTRUCT ics;
	reportTextBuffer.Empty();

	do {	// Цикл по альфа(УХФ)
		fa.PrepareToFirstCalc();
		do {	// Цикл по давлениям
			memset(&ics, 0, sizeof(ics));
			fa.Calc(CurrPC, ics);	// Первичное приближение
		// Расчет процесса горения (КС)
			ccc.Calc(CurrPC, ics);
		// Поиск и расчет кр. сечения
			ccs.Calc(CurrPC, ics);
		// Если есть опция: расчитать выходное сечение при атм. давлении
			if(calcs.bIncludeAtmosphereSection) {
				caswpa_atm.FirstParam();
				do {	// Цикл по давлениям в сечениях
					caswpa_atm.Calc(CurrPC, ics);
				} while(caswpa_atm.NextParam());
			}
		// Различные варианты, расчета течения
			//1) Расчет течения по давлениям в сечениях
			if(calcs.bIncludePa) {
				caswpa.FirstParam();
				do {	// Цикл по давлениям в сечениях
					caswpa.Calc(CurrPC, ics);
				} while(caswpa.NextParam());
			}

			//2) Расчет течения, по степени расширения сечения
			if(calcs.bIncludeF) {
				caswf.FirstParam();
				do {	// Цикл по степени расширения сечения
					caswf.Calc(CurrPC, ics);
				} while(caswf.NextParam());
			}

			//3) Для расчета IS диаграммы
			if(calcs.bIncludeIS) {
				cisd.FirstParam();
				do {	// Цикл по степени расширения сечения
					cisd.Calc(CurrPC, ics);
				} while(cisd.NextParam());
			}

			//4) Для расчета горения при постоянном объеме
			if(calcs.bIncludeV) {
				memset(&ics, 0, sizeof(ics));
				fa.Calc(CurrPC, ics);	// Первичное приближение
				ccv.FirstParam();
				do {	// Цикл по степени расширения сечения
					ccv.Calc(CurrPC, ics);
				} while(ccv.NextParam());
			}

			ccv.NextPressure();
			cisd.NextPressure();
			caswpa_atm.NextPressure();
			caswpa.NextPressure();
			caswf.NextPressure();
			ccs.NextPressure();
		} while(ccc.NextPressure());
		fa.NextAlfa();	// FirstApproximation пересчитывается только при изменении альфа
		ccv.NextAlfa();
		cisd.NextAlfa();
		caswpa_atm.NextAlfa();
		caswpa.NextAlfa();
		caswf.NextAlfa();
		ccs.NextAlfa();
	} while(ccc.NextAlfa());

//	int sss = sizeof(CameraCombustionSTRUCT);		// DEBUG
//	int sss = sizeof(AllPCSTRUCT);					// DEBUG
	reportTextBuffer += "Расчет КС.";
	reportTextBuffer += cf;
	ccc.GetReportA(reportTextBuffer);

	// Если есть опция: расчитать выходное сечение при атм. давлении
	if(calcs.bIncludeCritSect) {
		reportTextBuffer += cf;
		reportTextBuffer += cf;
		reportTextBuffer += "Расчет критического сечения.";
		reportTextBuffer += cf;
		ccs.GetReportA(reportTextBuffer);
	}
	// Если есть опция: расчитать выходное сечение при атм. давлении
	if(calcs.bIncludeAtmosphereSection) {
		reportTextBuffer += cf;
		reportTextBuffer += cf;
		reportTextBuffer += "Расчет выходного сечения при норм. атм. давлении.";
		reportTextBuffer += cf;
		caswpa_atm.GetReportA(reportTextBuffer);
	}
	// Различные варианты, расчета течения
		//1) Расчет течения по давлениям в сечениях
	if(calcs.bIncludePa) {
		reportTextBuffer += cf;
		reportTextBuffer += cf;
		reportTextBuffer += "Расчет выходных сечений по давлениям в них.";
		reportTextBuffer += cf;
		caswpa.GetReportA(reportTextBuffer);
	}

		//2) Расчет течения, по степени расширения сечения
	if(calcs.bIncludeF) {
		reportTextBuffer += cf;
		reportTextBuffer += cf;
		reportTextBuffer += "Расчет течения, по степени расширения сечения.";
		reportTextBuffer += cf;
		caswf.GetReportA(reportTextBuffer);
	}

		//3) Для расчета IS диаграммы
	if(calcs.bIncludeIS) {
		reportTextBuffer += cf;
		reportTextBuffer += cf;
		reportTextBuffer += "Расчет IS диаграммы.";
		reportTextBuffer += cf;
		cisd.GetReportA(reportTextBuffer);
	}

		//4) Для расчета горения при постоянном объеме
	if(calcs.bIncludeV) {
		reportTextBuffer += cf;
		reportTextBuffer += cf;
		reportTextBuffer += "Расчет горения при постоянном объеме.";
		reportTextBuffer += cf;
		ccv.GetReportA(reportTextBuffer);
	}

	return TRUE;
}

void CThermoCalcDoc::SetValidUXF() { bMaybeChangeUXF = FALSE; }

void CThermoCalcDoc::SetInvisibleFlag() {	bInvisibleFlag = TRUE;	}

BOOL CThermoCalcDoc::SaveAsText() {
	pViewOutput ->GetWindowText(sSaveAsExcel);
	int nLength = sSaveAsExcel.GetLength();
	if (nLength) {
		CFileDialog foSave(FALSE,"txt","Calc.txt");
		if (foSave.DoModal() == IDOK) {
			CFile fileExcel;
			if ( (fileExcel.Open(foSave.GetPathName(), CFile::modeCreate|CFile::modeWrite)) == NULL )
				return FALSE;
			char *p = sSaveAsExcel.GetBuffer(nLength);
			fileExcel.Write(p, nLength);
			sSaveAsExcel.ReleaseBuffer();
		}
	}
	return TRUE;
}

// rngLeftTopCorner - левая верхняя ячейка таблицы с которой начинается запись.
// return - 'Range' в котором поместились данные: Range *prngSquare
BOOL CThermoCalcDoc::WriteToExcelWorksheet(_Worksheet &ws, Range &rngLeftTopCorner, Range *prngSquare) {

	Range rngThroughOffset;
	LPDISPATCH pOffset;
	CString str1;
	CString str2;
	CString str3;

	int f1;
	int nFN2;
	int nFN1;

	if(!prngSquare) {
		prngSquare = new Range;
	}
	// Добавить бы еще имя пользователя и что-нить есчо
	MACROGetOffset(0,0);
	rngThroughOffset.SetValue2(COleVariant("Время расчета:"));
	MACROClearOffset();

	MACROGetOffset(0,1);
	COleDateTime odtCurrTime = COleDateTime::GetCurrentTime();
	rngThroughOffset.SetValue2(COleVariant(odtCurrTime));
	rngThroughOffset.SetNumberFormat(COleVariant("ДД.ММ.ГГГГ ч:мм"));	// ???Внимание если другой язык, то другая строка (или нет?)
	MACROClearOffset();
//	VARIANT v = rngLeftTopCorner.GetValue2();
//	COleVariant ov(v);
//	ov.ChangeType(VT_DATE);
//	MACROGetOffset(1,1);
//	rngThroughOffset.SetValue2(COleVariant(COleVariant((long)5)));

// Расположить отчеты на одном листе, с лева на право
	MACROGetOffset(1,0);
	rngThroughOffset.SetValue2(COleVariant("Расчет КС"));
	MACROClearOffset();

	MACROGetOffset(2,0);
	ccc.WriteToExcelWorksheet(ws, rngThroughOffset, prngSquare);
	MACROClearOffset();

	// Выбрать расположение следующего отчета
		// Преобразование правого верхнего угла прямоугольника в один Range (в одну ячейку)
	str1 = prngSquare ->GetAddress();			// str1 - адрес области где расположен отчет
	prngSquare ->ReleaseDispatch();
	f1 = str1.Find(':');
	str2 = str1.Left(f1);							// Левая часть
	str3 = str1.Mid(f1+1);							// Правая часть
	nFN2 = str3.FindOneOf("0123456789");	// Найти конец буквенных обозначений в str3
	str1 = str3.Left(nFN2);
	nFN1 = str2.FindOneOf("0123456789");	// Найти конец буквенных обозначений в str2
	str1 += str2.Mid(nFN1);
	pOffset = ws.GetRange(COleVariant(str1));
	rngLeftTopCorner.AttachDispatch(pOffset);
	str1 = rngLeftTopCorner.GetAddress();		// DEBUG
		// Следующий отчет сразу с права
	pOffset = rngLeftTopCorner.GetOffset(COleVariant((long)-1), COleVariant((long)cnIndentReports+1));//(-1,x) -1 на одну клетку выше
	rngLeftTopCorner.AttachDispatch(pOffset);

	MACROGetOffset(0,0);
	rngThroughOffset.SetValue2(COleVariant("Расчет кр. сечения"));
	MACROClearOffset();

	MACROGetOffset(1,0);
	ccs.WriteToExcelWorksheet(ws, rngThroughOffset, prngSquare);
	MACROClearOffset();
	// Выбрать расположение следующего отчета
		// Преобразование правого верхнего угла прямоугольника в один Range (в одну ячейку)
	str1 = prngSquare ->GetAddress();			// str1 - адрес области где расположен отчет
	prngSquare ->ReleaseDispatch();
	f1 = str1.Find(':');
	str2 = str1.Left(f1);							// Левая часть
	str3 = str1.Mid(f1+1);							// Правая часть
	nFN2 = str3.FindOneOf("0123456789");	// Найти конец буквенных обозначений в str3
	str1 = str3.Left(nFN2);
	nFN1 = str2.FindOneOf("0123456789");	// Найти конец буквенных обозначений в str2
	str1 += str2.Mid(nFN1);
	pOffset = ws.GetRange(COleVariant(str1));
	rngLeftTopCorner.AttachDispatch(pOffset);
	str1 = rngLeftTopCorner.GetAddress();		// DEBUG
		// Следующий отчет сразу с права
	pOffset = rngLeftTopCorner.GetOffset(COleVariant((long)-1), COleVariant((long)cnIndentReports+1));//(-1,x) -1 на одну клетку выше
	rngLeftTopCorner.AttachDispatch(pOffset);

	if(calcs.bIncludeAtmosphereSection) {
		MACROGetOffset(0,0);
		rngThroughOffset.SetValue2(COleVariant("Расчет выхода при атмосферном давлении"));
		MACROClearOffset();

		int nLenDescrProc;
		if (os.bIncludeUnitMeasOption)
			nLenDescrProc = 3;
		else
			nLenDescrProc = 2;
		MACROMergeRightToLeft(0,0,nLenDescrProc-1,str1,str2);

		MACROGetOffset(1,0);
		caswpa_atm.WriteToExcelWorksheet(ws, rngThroughOffset, prngSquare);
		MACROClearOffset();
		// Выбрать расположение следующего отчета
			// Преобразование правого верхнего угла прямоугольника в один Range (в одну ячейку)
		str1 = prngSquare ->GetAddress();			// str1 - адрес области где расположен отчет
		prngSquare ->ReleaseDispatch();
		f1 = str1.Find(':');
		str2 = str1.Left(f1);							// Левая часть
		str3 = str1.Mid(f1+1);							// Правая часть
		nFN2 = str3.FindOneOf("0123456789");	// Найти конец буквенных обозначений в str3
		str1 = str3.Left(nFN2);
		nFN1 = str2.FindOneOf("0123456789");	// Найти конец буквенных обозначений в str2
		str1 += str2.Mid(nFN1);
		pOffset = ws.GetRange(COleVariant(str1));
		rngLeftTopCorner.AttachDispatch(pOffset);
		str1 = rngLeftTopCorner.GetAddress();		// DEBUG
			// Следующий отчет сразу с права
		pOffset = rngLeftTopCorner.GetOffset(COleVariant((long)-1), COleVariant((long)cnIndentReports+1));//(-1,x) -1 на одну клетку выше
		rngLeftTopCorner.AttachDispatch(pOffset);
	}

	if(calcs.bIncludePa) {
		MACROGetOffset(0,0);
		rngThroughOffset.SetValue2(COleVariant("Расчет для давлений в сечениях"));
		MACROClearOffset();
		MACROGetOffset(1,0);
		caswpa.WriteToExcelWorksheet(ws, rngThroughOffset, prngSquare);
		MACROClearOffset();
		// Выбрать расположение следующего отчета
			// Преобразование правого верхнего угла прямоугольника в один Range (в одну ячейку)
		str1 = prngSquare ->GetAddress();			// str1 - адрес области где расположен отчет
		prngSquare ->ReleaseDispatch();
		f1 = str1.Find(':');
		str2 = str1.Left(f1);							// Левая часть
		str3 = str1.Mid(f1+1);							// Правая часть
		nFN2 = str3.FindOneOf("0123456789");	// Найти конец буквенных обозначений в str3
		str1 = str3.Left(nFN2);
		nFN1 = str2.FindOneOf("0123456789");	// Найти конец буквенных обозначений в str2
		str1 += str2.Mid(nFN1);
		pOffset = ws.GetRange(COleVariant(str1));
		rngLeftTopCorner.AttachDispatch(pOffset);
		str1 = rngLeftTopCorner.GetAddress();		// DEBUG
			// Следующий отчет сразу с права
		pOffset = rngLeftTopCorner.GetOffset(COleVariant((long)-1), COleVariant((long)cnIndentReports+1));//(-1,x) -1 на одну клетку выше
		rngLeftTopCorner.AttachDispatch(pOffset);
	}

	if(calcs.bIncludeF) {
		MACROGetOffset(0,0);
		rngThroughOffset.SetValue2(COleVariant("Расчет по степеням расш. сопла"));
		MACROClearOffset();
		MACROGetOffset(1,0);
		caswf.WriteToExcelWorksheet(ws, rngThroughOffset, prngSquare);
		MACROClearOffset();
		// Выбрать расположение следующего отчета
			// Преобразование правого верхнего угла прямоугольника в один Range (в одну ячейку)
		str1 = prngSquare ->GetAddress();			// str1 - адрес области где расположен отчет
		prngSquare ->ReleaseDispatch();
		f1 = str1.Find(':');
		str2 = str1.Left(f1);							// Левая часть
		str3 = str1.Mid(f1+1);							// Правая часть
		nFN2 = str3.FindOneOf("0123456789");	// Найти конец буквенных обозначений в str3
		str1 = str3.Left(nFN2);
		nFN1 = str2.FindOneOf("0123456789");	// Найти конец буквенных обозначений в str2
		str1 += str2.Mid(nFN1);
		pOffset = ws.GetRange(COleVariant(str1));
		rngLeftTopCorner.AttachDispatch(pOffset);
		str1 = rngLeftTopCorner.GetAddress();		// DEBUG
			// Следующий отчет сразу с права
		pOffset = rngLeftTopCorner.GetOffset(COleVariant((long)-1), COleVariant((long)cnIndentReports+1));//(-1,x) -1 на одну клетку выше
		rngLeftTopCorner.AttachDispatch(pOffset);
	}

	if(calcs.bIncludeIS) {
		MACROGetOffset(0,0);
		rngThroughOffset.SetValue2(COleVariant("Расчет IS диаграммы"));
		MACROClearOffset();
		MACROGetOffset(1,0);
		cisd.WriteToExcelWorksheet(ws, rngThroughOffset, prngSquare);
		MACROClearOffset();
		// Выбрать расположение следующего отчета
			// Преобразование правого верхнего угла прямоугольника в один Range (в одну ячейку)
		str1 = prngSquare ->GetAddress();			// str1 - адрес области где расположен отчет
		prngSquare ->ReleaseDispatch();
		f1 = str1.Find(':');
		str2 = str1.Left(f1);							// Левая часть
		str3 = str1.Mid(f1+1);							// Правая часть
		nFN2 = str3.FindOneOf("0123456789");	// Найти конец буквенных обозначений в str3
		str1 = str3.Left(nFN2);
		nFN1 = str2.FindOneOf("0123456789");	// Найти конец буквенных обозначений в str2
		str1 += str2.Mid(nFN1);
		pOffset = ws.GetRange(COleVariant(str1));
		rngLeftTopCorner.AttachDispatch(pOffset);
		str1 = rngLeftTopCorner.GetAddress();		// DEBUG
			// Следующий отчет сразу с права
		pOffset = rngLeftTopCorner.GetOffset(COleVariant((long)-1), COleVariant((long)cnIndentReports+1));//(-1,x) -1 на одну клетку выше
		rngLeftTopCorner.AttachDispatch(pOffset);
	}

//...
	if(prngSquare) {
		;
	}
//...

	return TRUE;
}

CALCSTRUCT * CThermoCalcDoc::GetpCS()	{ bMaybeChangeCS = TRUE;
 return &calcs;}
CALCSTRUCT & CThermoCalcDoc::GetCS()	{ bMaybeChangeCS = TRUE;
 return calcs;}
UXFSTRUCT * CThermoCalcDoc::GetpUXF()	{ bMaybeChangeUXF = TRUE;
 return uxf;}
//UXFSTRUCT *& CThermoCalcDoc::GetUXF() {return (struct UXFSTRUCT *&)uxf;}
void CThermoCalcDoc::SetFocus_Output() { pViewOutput->SetFocus(); }
