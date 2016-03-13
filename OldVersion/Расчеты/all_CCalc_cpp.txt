STRICT
#define VC_EXTRALEAN		// speed up build process
#include "stdafx.h"
#include <math.h>			// fabs();
#include "Calc.h"
#include "all_CCalc.h"
#include "ThermoCalc.h"

#pragma warning (disable : 4244)								// Преобразование 'float' к 'double'

//#include "table.h"
extern int Valence[];
extern double M[];
extern char cf[3];
extern char tab[1];// "\t" for CString
const char tab_sym = 9;
char szCondSymbol[] = "k*";	// Последовательность символов обозначающая конденсат, добавляется в начале строки

// Макросы
	// Для работы с текстовым выводом (Нужны объявленные переменные - CString str1;	char	szOut[MAX_len];
	//																						int	ip,nCurrUXF,ipar,j; float	fTabLen)
		// дополнение строки табуляциями до длинны 'fTabLen'
#define MACROCyleRegulateTab(regul_str)							 {	\
	for (j = 0;j<MAX_TAB_STOPS;j++) {									\
		if (fTabLen<=LOWORD(GetTabbedTextExtent(hdc, (LPCTSTR)regul_str, regul_str.GetLength(), 1, rgxTabs)))\
			break;																\
		regul_str += tab;														\
	}																				\
}																					
		// вывод единиц измерения зависит от szPrecision;
#define MACROFormatToReport(szReport_x, pfAllx, szReport_unitx) {	\
	wsprintf(szOut,"%s%s", szReport_x, szPrecision);					\
	str1.Format(szOut, pfAllx, szReport_unitx);							\
	MACROCyleRegulateTab(str1);												\
	strReport += str1;															\
}
		// вывести в отчет форматированную строку сост. из назв., величины и единиц измерения
#define MACROMakeTab_Cycle_Format(szReport_x, pfAllx, szReport_unitx){\
	for(j=0;j<nStartTab;j++)														\
		strReport += tab;																\
	for (ip =0;ip<rep_nP;ip++)														\
		for (ipar =0;ipar<rep_nParam;ipar++)									\
			MACROFormatToReport(szReport_x, pfAllx[ip][nCurrUXF][ipar], szReport_unitx);		\
	strReport += cf;																	\
}
		// вывести в отчет форматированную строку сост. из назв., величины и единиц измерения
		// Учитывает флаг bMinNames. Нужно определить строку szMinPrecision
#define MACROMakeTab_Cycle_FormatMinNames(szReport_x, pfAllx, szReport_unitx){\
	for(j=0;j<nStartTab;j++)														\
		strReport += tab;																\
	for (ip =0;ip<rep_nP;ip++)														\
		for (ipar =0;ipar<rep_nParam;ipar++)									\
			if (bMinNames&&((ip!=0)||(ipar!=0))) {			/* Если стоит опция и это не самый левый столбец*/ \
				str1.Format(szMinPrecision, pfAllx[ip][nCurrUXF][ipar]); \
				MACROCyleRegulateTab(str1);										\
				strReport += str1;													\
			}																				\
			else {																		\
				MACROFormatToReport(szReport_x, pfAllx[ip][nCurrUXF][ipar], szReport_unitx);		\
			}																				\
	strReport += cf;																	\
}

#define MACROFormatReport MACROMakeTab_Cycle_FormatMinNames //Для удобства чтения

// DEBUG
char _szNam[MaxElemGasFromDB][MAX_len];
extern char szElemsFromDB[MaxAtoms][MaxAtomNameLen];	// Для разбора эл. из базы
extern float fT[MaxAtoms];										// Для разбора эл. из базы

// Для отладки нужны как внешние что бы видеть данные в отладчике как массив
// потом будут как указатели на переменные
//extern double P[MaxElemGasFromDB];
//extern float XF1[120];
//extern float XF2[120];

// DEBUG
float balanc_MC[PERIOD_TABLE_size];
// Debug
extern char Mstr[256];									//сообщение об ошибках

const float CTherDynEqCalc::R0 = (float)1.98719;// в калориях	//R0=1.98719*4.1868;// в джоулях

CTherDynEqCalc::CTherDynEqCalc() {
//	R0 = (float)1.98719;		// в калориях
	//R0=1.98719*4.1868;		// в джоулях
	E1 = (float)5E-5;
	E2 = (float)5E-4;
}

BOOL CTherDynEqCalc::Construct(JobWithDB *p) {
	if (!p) return FALSE;
	pDBs = p;

	FSH = 0;
	DR = 0;
	FRY = 0;
	tdecs.IS = 0;
	tdecs.SS = 0;
	tdecs.CPR = 0;
	T3AM = 0;
	F3C = 0;
	FP3 = 0;
	FHAC = 0;
	F = 0;
	PP = 0;
	HAC[0]=HAC[1]=HAC[2]=HAC[3]=HAC[4]=0;
	F1 = 0;
	J1 = 0;
	MIN = 0;
	tdecs.MS = 0;
	tdecs.MG = 0;
	tdecs.ZK = 0;
	FPP = 0;
	IS1 = 0;
	CP31 = 0;
	E = 0;
	BT = 0;
	tdecs.CP3 = 0;

	M = 0;
	N = 0;
	NG = 0;
	CK = 0;
	CKACH = 0;
	T2 = 0;
	T1 = 0;
	memset(S,0,sizeof(S));
	memset(ENT,0,sizeof(ENT));
	memset(CP,0,sizeof(CP));
	return TRUE;
}

//	BOOL Calc(double P[MaxElemGasFromDB], float XF1[120], float XF2[120]);
THERDYNEQCALCSTRUCT* CTherDynEqCalc::Calc (float ext_T, float ext_PP, 
		float P[MaxElemGasFromDB], INTOCALCSTRUCT &ics) 
{
	T = ext_T;
	PP = ext_PP;
	memset(XF1,0,sizeof(XF1));
	memset(XF2,0,sizeof(XF2));
	if (!pDBs->GetTERMO_XF1andXF2(XF1, XF2))
		return FALSE;

	pDBs->GetAllchem_data(nPElementaryElem, nPGas, cdG, nPCond, cdC);
//	pDBs->GetNamesP(NULL, nMaxNamesLen);
	pDBs->GetNamesP(_szNam, nMaxNamesLen);

	// Потом выбрать что-нибудь одно
	M = nPElementaryElem;
	NG = nPGas;
	N = nPGas + nPCond;
	nPAll = nPGas + nPCond;

//	float fSum, fSum2 = 0;
	int cur_elem, i;

	CALFN();
// My code
	for (cur_elem=1;cur_elem<=nPAll;cur_elem++)
		gcvt((S[cur_elem]*4.1868),8,&_szNam[cur_elem][nMaxNamesLen+1]);
	for (cur_elem=1;cur_elem<=nPAll;cur_elem++)
		gcvt((ENT[cur_elem]*4.1868),8,&_szNam[cur_elem][nMaxNamesLen+1]);
	for (cur_elem=1;cur_elem<=nPAll;cur_elem++)
		gcvt((CP[cur_elem]*4.1868),8,&_szNam[cur_elem][nMaxNamesLen+1]);
// My code end

	CK = 0;
	CKACH = FALSE;

/*
	for(int i1=0;i1<10;i1++)
		for(int i2=0;i2<10;i2++)
			B[i1][i2] = ics.B[i1][i2];
	for(int i3=0;i3<10;i3++)
		A5[i3] = ics.A5[i3];
*/

	//PABHPAC();
	PABHPAC(P, ics);
	tdecs.IS = tdecs.IS * (float)4.1868;
	tdecs.SS = tdecs.SS * (float)4.1868;
	tdecs.CPR = tdecs.CPR * (float)4.1868;

// My code
	// балланс - по уравнениям сохранения элементов
/*
	memset(balanc_MC,0,sizeof(balanc_MC));
	for(cur_elem=1;cur_elem<=uxf[nCurrUXF].nQuantityElemF;cur_elem++)
		balanc_MC[GetSN(uxf[nCurrUXF].szAtomsOfF[cur_elem-1])]=
				uxf[nCurrUXF].fConditChimFormF[cur_elem-1];
*/
	char cRes[MAX_len];
 	float fResSum;
	for (i=0;i<M;i++) {
		fResSum = 0;
		TransNameMolecToAandF(szElemsFromDB,fT,cdG[i].view);
		strcpy(cRes,szElemsFromDB[0]);
		for (int i2=M;i2<NG;i2++)
			if (P[i2+1]>0) {
				int nQuantAtomsFromDB = TransNameMolecToAandF(szElemsFromDB,fT,cdG[i2].view);
				for (int i3=0;i3<nQuantAtomsFromDB;i3++)
					if (!strcmp(cRes,szElemsFromDB[i3]))
						fResSum += P[i2+1]*fT[i3];
			}
		gcvt(fResSum,6,&_szNam[i+1][nMaxNamesLen+1]);
		int sn = GetSN(cRes);
		balanc_MC[sn] = fResSum;
	}
	for (cur_elem=1;cur_elem<=nPAll;cur_elem++)
		gcvt(P[cur_elem],6,&_szNam[cur_elem][nMaxNamesLen+1]);
// My code end
	return &tdecs;
}

void CTherDynEqCalc::CALFN() {
	//DCL AT(5) DEC FLOAT(10);
	double IS=0,SS=0,CPR=0;
	int RK = N-NG;
	CK = 0;

// Объявленные компилятором по умолчанию
	float R = 0;
	float W = 0;
	float C = 0;

	// DEBUG // T = 288;	// const double fTrans = 4.1868;
	// Газы
	const float fTrans = 1;
	double tt = T/1000;
	double ResEnt,ResEntr,Entr,power,fff,ResCP;
	for (int i=0;i<NG;i++) {
		ResEnt = ResEntr = Entr = ResCP = 0;
		//
		ResEntr = cdG[i].factors[2]*log(tt);
		ResEnt = cdG[i].factors[1] + cdG[i].factors[2]*tt;//pow(tt,1);
		ResCP = cdG[i].factors[2];
		for (int i2=2;i2<NumFactorsGas;i2++) {
			power = pow(tt,i2-1);
			ResEnt += cdG[i].factors[i2+1]*power*tt;
			fff = i2;
			ResEntr += fff/(fff-1)*cdG[i].factors[i2+1]*power;
			ResCP += i2*cdG[i].factors[i2]*power;
		}
		Entr = (cdG[i].factors[0]+ResEntr*0.001);
		ResCP = 0.001*ResCP;

		ENT[i+1] = ResEnt;
		S[i+1] = Entr;
		CP[i+1] = ResCP;
	}

	// Конденсаты
	R = T/1000;
	C = log(R)/1000;
	float fi;
	if (RK != 0)
	{
		I = NG+1;
		while (I<=N)
		{
			IS = 0;
			SS = 0;
			CPR = 0;
			int nT = cdC[I-NG-1].nIntervals;
			for (int iT=0;iT<nT;iT++) { // Цикл по каждому температурному интервалу
				if (cdC[I-NG-1].dTemperature[iT] >= T) {
					for(int ii=0;ii<2;ii++) {
						W = cdC[I-NG-1].factors[iT][4-ii];
						IS = (IS + W)*R;
						fi = ii;	// Во время деления получаются дробные числа
						SS = (SS+(3-fi)/(2-fi)*W)*R;
						CPR = (CPR+(3-fi)*W)*R;
					}
					W = cdC[I-NG-1].factors[iT][2];
					ENT[I] = (IS+W)*R + cdC[I-NG-1].factors[iT][1];
					S[I] = SS/1000+W*C + cdC[I-NG-1].factors[iT][0];
					CP[I] = (CPR+W)/1000;
					break;	// Расчет для этого элемента закончен
				}
				// Значит текущая температура не входит в диапазон интерполяции для данного элемента
				ENT[I] = 1E9;
				S[I] = 0;
				CP[I] = 0;
			}
			I ++;
		}
	}
	return;
}

//	BOOL PABHPAC(double P[MaxElemGasFromDB], float XF1[120], float XF2[120]);
BOOL CTherDynEqCalc::PABHPAC (float P[MaxElemGasFromDB], INTOCALCSTRUCT &ics) {

	float _E;
// DCL (K,D,A,Q,PI,LPI,A1,A2,AA,FL,MM,R,YI,C,DK) DEC (6);
	double K=0,D,A,Q,PI,LPI,A1,A2,AA=0,FL,MM,R,YI,C,DK;

	FL=1;
	YI=1;
	// Объявленные компилятором по умолчанию
	int IT=0;

	if (T>T3AM) {
		F3C = FALSE;
		FP3 = FALSE;
	}

BEGIN:

	if (FHAC)
		ics.FB = FALSE;

	DR = 1;
	I1 = 1;
	F = TRUE;
	YI = -YI;
	A = 0;

	// 'A' Просумируем количество молей в УХФ
	for (I=1;I<=N;I++) {
		if ((XF1[I]>0)&&(P[I]>0))
			A=A+P[I];		
	}

	//	if (A<=PP*(1E-18))
	//		goto LA;
	if (A>PP*(1E-18)) {

		AA=log(PP/A);	// 'AA' Давление на один моль
		if (F3C)
			goto TD;

		F1 = TRUE;
		for (I=1;I<=N;I++) {
			A2=XF2[I];						// ??? сложно -XF2[]
			if (A2<0) {
				if ((I<=NG)&&(P[I]>1E-17))
					F1=FALSE;
				//goto LK;	//My code
				if (I==HAC[DR]) DR=DR+1;
				continue;
			}

			A1=XF1[I];
			PI=P[I];
			if ((A1>0)&&((PI/A)<0.1E-5)&&(ics.FB)){//'A'кол-во молей в УХФ
				//goto LK;	//My code
				if (I==HAC[DR]) DR=DR+1;
				continue;
			}

			if (A1<0)
				LPI=0;
			else
				if (PI<0)
					LPI=PI;	// значит уже логарифм
				else
					LPI=log(PI);

			if (A1<0)
				Q=0;
			else
				Q=-(AA+LPI);			// Q=-(log(PP/A)+log(PI))
			K=(S[I]-FL*ENT[I]/T)/R0;// K= расчет правой части уравнения диссоциации

			//A6 = 0
			memset(A6,0,sizeof(A6));

			X=1;	Y=1;
			int nIndexInElementarSubst;
			for (I1=1;I1<=4;I1++) {
				J = floor(fabs(A1)/X)-floor(fabs(A1)/(X*100))*100;
				if (J==0)	break;
				J1 = floor(A2/Y)-floor(A2/(Y*10))*10;
				nIndexInElementarSubst=1;
				while (XF1[nIndexInElementarSubst]!=J)
					nIndexInElementarSubst++;
				// Что бы 'nIndexSubst' содержал индекс элементарн.хим.эл.(атома) из 'XF1[]'
				// атома который был найден в 'I'-том соединении
				// Цикл по атомам
				for (I2=1;I2<=M;I2++) {//'J1'с каким коэффициентом присутствует эл. в соединении
					A6[I2]=A6[I2]+J1*ics.B[I2][nIndexInElementarSubst];//'J1' коэффициент при атоме 'I1' в соединении 'I'
					if (fabs(A6[I2])<0.1E-7)  // Если 'I2' равно 'nIndex', то 'B[I2][nIndexSubst]' равно 1 на первой итерации
						A6[I2]=0;
				}
			// B[1..M][] цикл по строкам - I2 Э [1..M],
			// B[][J] в столбцах nIndexSubst
			// вначале A6[]=0, потом в A6[0..M](по I2) суммируется
			// для каждого коэфф.'J1'при атоме 'I1'(0..4) в соединении
			// A6[I2] += J1_f(I1)*B[I2][nIndexSubst_f(I1)]
			// каждую 'I1' итерацию 'nIndexSubst' будет разный, 
			// т.к. соединение состоит из разных веществ
				X=X*100;
				Y=Y*10;
			}
			MM=1;
			for (I1=1;I1<=M;I1++) {
				_E=A6[I1];
				if (_E==0)	continue;
				//J=A5[I1];
				if ((A1<0)||(PI>=0.01*P[(ics.A5[I1])]))	//if ((A1<0)||(PI>=0.01*P[J]))
					MM=0;
				K=K+_E*(ENT[(ics.A5[I1])]*FL/T-S[(ics.A5[I1])])/R0;	//K = K1[I] + _E*K2[(A5[1..M])]
				if (XF1[(ics.A5[I1])]<0)
					continue;
				Q=Q+_E*(log(P[ics.A5[I1]])+AA);	// Q=Q+_E*(LP[I1]+AA);
				//Q=_E*(log(PP/A)+log(P[A5[I1]]))-(log(PP/A)+log(P[I]))
			}

			C=K+Q;
			if (I!=HAC[DR])
				if ((A1<0)&&(PI<0.1E-18)&&(C<0)) {
					//goto L2;	//My code
					P[I]=PI; if (I==HAC[DR]) DR=DR+1;
					continue;
				}

			if (MM==0)
				_E=E1;	//E1=5E-5;
			else
				_E=E2;	//E2=5E-4;
			if ((fabs(C))<_E) {
			//goto L2;	//My code
				P[I]=PI; if (I==HAC[DR]) DR=DR+1;
				continue;	
			}

			F = FALSE;
			//	if (Нечитаемый символ) ok
			if ((I!=HAC[DR])&&(C<=0))
				MIN=PI;
			else
				MIN=0.1E5;

			if (A1<0)
				D=0;
			else
				D=1/PI;

			for (I1=1;I1<=M;I1++) {
				R=A6[I1];
				if (R==0)
					continue;
				//J=A5[I1];
				if ((R*C)>=0) {
					_E=P[(ics.A5[I1])]/fabs(R);		//_E=P[J]/fabs(R);давление соединения с мин.парц.давл. и имеющем в своем составе вещ. с индексом I1/кол-во коэффициентов
					if (_E<MIN)
						MIN=_E;
				}
				if ((MM==0)&&(XF1[(ics.A5[I1])]>0))	//if ((MM==0)&&(XF1[J]>0))
					D=D+R*R/P[(ics.A5[I1])];		//D=D+R*R/P[J];
			}

			float reduce_MIN =(float) 0.9;
			//if (MM==0)//	goto L5;
			if (MM!=0) {
				//if (YI>0)	//	goto L2;
// YI Единица, инвертируется при прохожлении метки BEGIN - при неудачном просчете всех соединений
				if (YI <=0) {	
					LPI=LPI+C;
					//if ((PI<0)&&(C<0)) { //PI=LPI; //goto L2; //}
					if (!((PI<0)&&(C<0))) {
						if (LPI>40)
							_E=2*MIN;
						else
							_E=exp(LPI);

						if (PI<0)
							DK=_E;
						else
							DK=_E-PI;
						if (DK>MIN) {
							//	goto L8;
							//L8:
							if (fabs(DK)>=MIN)
								DK=reduce_MIN*MIN*SIGN(C);
								//if (C>=0)	DK=0.9*MIN*1;
								//else		DK=0.9*MIN*(-1);

							PI=PI+DK;						// Нечитааемый + или *
						}
						else {
							if (LPI<-42)
								PI=LPI;		// здесь встречается отрицательное LPI
							else
								PI=_E;
							//goto L3;
						}
					}
					else {
						PI=LPI;
						//goto L2;	//My code
						P[I]=PI; if (I==HAC[DR]) DR=DR+1;
						continue;
					}
				}
				else {
					//goto L2;	//My code
					P[I]=PI; if (I==HAC[DR]) DR=DR+1;
					continue;
				}
			}
			else {
				//L5:
				if (D==0)	DK=MIN/reduce_MIN;	// DK=10*MIN/9;
				else		DK=C/D;
				//L8:
				if (fabs(DK)>=MIN)
					//if (C>=0)	DK=0.9*MIN*1;
					//else		DK=0.9*MIN*(-1);
					DK=reduce_MIN*MIN*SIGN(C);
				PI=PI+DK;						// Нечитааемый + или *
			}

	//L3: 
	// - один из концов операции расчета давления одного соединения
	// изменения давлений всех макс.соед.(А5[]) выбор мин. и возможно замена его на текущ. соед.
	// если давл. текущ. соед. больше
			MIN=(float)0.1E18;
			for (I1=1;I1<=M;I1++) {
				R=A6[I1];
				if (R==0)
					continue;
				//J=A5[I1];
				P[(ics.A5[I1])]=P[(ics.A5[I1])]-R*DK;
				if (XF1[(ics.A5[I1])]>0)
					LP[I1]=log(P[(ics.A5[I1])]);
				if (P[(ics.A5[I1])]<MIN){
					I2=I1;
					MIN=P[(ics.A5[I1])];
				}
			}

			if (PI>=MIN) {
				//J=A5[I2];
				XF2[(ics.A5[I2])]=fabs(XF2[(ics.A5[I2])]);
				ics.A5[I2]=I;
				XF2[I]=-fabs(XF2[I]);
				LP[I2]=log(PI);		//natural
				F1 = FALSE;

				MIN=1/A6[I2];	// 1<=I2<=M
				for (I1=1;I1<=M;I1++) {
					_E=ics.B[I2][I1];
					if (_E==0)	//goto L6;
						continue;
					for (J1=1;J1<=M;J1++) {
						ics.B[J1][I1]+=_E*A6[J1]*(-MIN);
						if (fabs(ics.B[J1][I1])<0.1E-7)
							ics.B[J1][I1]=0;
					}
					ics.B[I2][I1]=_E*MIN;//A6[I2]
					if (fabs(ics.B[I2][I1])<0.1E-7)
						ics.B[I2][I1]=0;
					//L6:
				}
			}

			P[I]=PI;		//L2:
			if (I==HAC[DR])	//LK:
				DR=DR+1;
		}
	}

	if (F1&&(A<1E-14)) {
		tdecs.MS=0;
		tdecs.MG=0;
		tdecs.ZK=1;
		FPP = TRUE;
		//goto LZ;
	}
	else {

		//if (Нечитаемый символ) ?F ok
		if (!F) {
			IT=IT+1;
			goto BEGIN;
		}

		if (ics.FB) {
			ics.FB = FALSE;
			goto BEGIN;
		}
		else
			ics.FB = TRUE;
TD:// Переход сюда если F3C==TRUE;
		//	if(F1)
		//		goto LZ;
		if(!F1) {
			//if (Нечитаемый символ) FL? ok
			//	if(FL!=1)
			//		goto LZ;
			if(FL==1) {
				tdecs.ZK=0;
				// Выполним расчет доли конденсата	
				for (I=NG+1;I<=N;I++) {
					tdecs.ZK=tdecs.ZK+MK[I-NG]*P[I];//'ZK'доля КОНДЕНСТАТА,'MK' молекулярная масса конд. вещ.
				}

				tdecs.MS=1000/A;			// молекулярная масса смеси
				tdecs.ZK=tdecs.ZK/1000;
				tdecs.MG=tdecs.MS*(1-tdecs.ZK);		// молекулярная масса газа
			}
		}
	}
	//LZ:

	if (FL==1)
		tdecs.SS=-R0*A*AA;
	IS1=0;
	CP31=0;

	for (I=1;I<=N;I++) {
		PI=P[I];
		if (PI<=0)
			//goto LI;
			continue;
		IS1=IS1+ENT[I]*PI;
		CP31=CP31+CP[I]*PI;
		//if (Нечитаемый символ) FL? ok
		if(FL!=1)
			//goto LI;
			continue;
		//значит FL==1
		if(XF1[I]>0)
			tdecs.SS=tdecs.SS+PI*(S[I]-R0*log(PI));
		else
			tdecs.SS=tdecs.SS+PI*S[I];
		//LI:
	}

	if (FHAC) {
		E=0;
		for (I=1;I<=DR-1;I++) {
			I1=HAC[I];
			if (P[I1]>0) {
				IS1=IS1-ENT[I1]*P[I1];
				CP31=CP31-CP[I1]*P[I1];
				if (FL==1)
					tdecs.SS=tdecs.SS-S[I1]*P[I1];
			}
			E=E+MK[I1-NG]*P[I1];
		}
		E=1000/(1000-E);
		IS1=IS1*E;
		CP31=CP31*E;
		if (FL==1)
			tdecs.SS=tdecs.SS*E;
		tdecs.MS=tdecs.MS/E;
		tdecs.ZK=1-(1-tdecs.ZK)*E;
		tdecs.MG=tdecs.MS*(1-tdecs.ZK);
	}

	IS1=IS1/1000;
	CP31=CP31/1000;

	if (FL==1) {
		//	if (Нечитаемый символ) CK? ok
		if (CK!=0)
			BT=CK*P[IC]/1000;
		else
			BT=0;
	}

	if (FL==1) {
		tdecs.IS=IS1;
		tdecs.SS=tdecs.SS/1000;
		tdecs.CP3=CP31;
		if (F3C||FHAC) {
			tdecs.CPR=tdecs.CP3;
			//goto MEN;
			//MEN:
			E=BT;
			return TRUE;
		}

		//	if (Нечитаемый символ) ?F1 ok
		if (!F1) {
			FL=1-(0.1/T);
			goto BEGIN;
		}
	}

	tdecs.CPR=CP31+(IS1-tdecs.IS)/0.1;
	E=BT;
	return TRUE;
}

CalcCameraCombustion::CalcCameraCombustion ()												
: CalcBase(){}	// С начала запуск базового конструктора по умолчанию

BOOL CalcCameraCombustion::Construct(CTherDynEqCalc *p, 
		UXFSTRUCT _uxf[MaxQuantityFIHBox], CALCSTRUCT _calcs) 
{
	return CalcBase::ConstrShortRep(p, _uxf, _calcs);
}

BOOL CalcCameraCombustion::Calc(ProductsCombustion &PC, INTOCALCSTRUCT &ics)
{
	T = fAssumedT;
	T1 = fMinT;
	T2 = fMaxT;
	PP = fAllP[nCurrP];
	if(!PP) return FALSE;
LB3_CALFN:
	//BOOL bRV = pTherDynEqCalc->Calc(PP,T);
	THERDYNEQCALCSTRUCT *ptdecs = pTherDynEqCalc->Calc(T,PP,PC.P,ics);
	if ( ((void *)ptdecs)==((void *)-1) ) return FALSE;
	if (!ptdecs) return FALSE;

	memset(&tdecs,0,sizeof(tdecs));
	tdecs.IS = ptdecs->IS;
	tdecs.SS = ptdecs->SS;
	tdecs.CPR= ptdecs->CPR;
	tdecs.MS = ptdecs->MS;
	tdecs.CP3 = ptdecs->CP3;
	tdecs.ZK = ptdecs->ZK;
	tdecs.MG = ptdecs->MG;

	float H = fAllEnthalpyF[nCurrUXF];
	if(!H) return FALSE;
	// постоянное давление
	T = T - (float(0.001))*SIGN(tdecs.IS-H);// SIGN(x)-функция нужна что бы сохранить знак 'x'
	float DT=(tdecs.IS-H)/tdecs.CPR;
	if (fabs(DT)>T*(float(0.12)))
		DT=T*(float(0.12))*SIGN(DT);// SIGN(x)-функция нужна что бы сохранить знак 'x'
	if (DT>0)
		T2=T;
	else
		T1=T;
	if ((T2-T1 - fabs(DT)) < (float(0.1)))
		DT=(T2-T1)/2*SIGN(DT);// SIGN(x)-функция нужна что бы сохранить знак 'x'
	if (fabs(DT)>0.3) {
		T = T - DT;
		ics.FB = TRUE;
		goto LB3_CALFN;
	}
	fAllProcessP[nCurrP][nCurrUXF][nCurrParam] = fAllP[nCurrP];
	fAllT[nCurrP][nCurrUXF][0] = T;
	fAllI[nCurrP][nCurrUXF][0] = tdecs.IS;
	fAllS[nCurrP][nCurrUXF][0] = tdecs.SS;
	fAllMS[nCurrP][nCurrUXF][0] = tdecs.MS;
	fAllCP3[nCurrP][nCurrUXF][0] = tdecs.CP3;
	fAllZK[nCurrP][nCurrUXF][0] = tdecs.ZK;

// Для поиска и расчета кр. сечения нужны некоторые данные из КС
	// Сохраним их в структуре передаваемой из расчета в расчет
	// Подготовка к след. стадии расчета
	ics.cc.T = T;
	ics.cc.IS = tdecs.IS;
	ics.cc.SS = tdecs.SS;
	if (tdecs.MS!=0)
		ics.cc.RR = 847.9/tdecs.MS;
	ics.cc.ZK = tdecs.ZK;
	ics.cc.MG = tdecs.MG;

	// Сохраним P для расчета течения в сечениях
	ics.cc.P = PP;

	// Добавление значений давления ПС для будущего формирования отчета Excel
	CalcBase::pRep ->SaveSomePC(nCurrP, nCurrUXF, nCurrParam, PC);
	return TRUE;
}

// Во время использования объекта нужно передать ему данные расчета КС - INTOCALCSTRUCT &ics
CalcCriticalSection::CalcCriticalSection() 
: CalcBase(){}	// С начала запуск базового конструктора по умолчанию

BOOL CalcCriticalSection::Construct(CTherDynEqCalc *p, 
		UXFSTRUCT _uxf[MaxQuantityFIHBox], CALCSTRUCT _calcs) 
{
	return CalcBase::ConstrLongRep(p, _uxf, _calcs);
}

BOOL CalcCriticalSection::Calc(ProductsCombustion &PC, INTOCALCSTRUCT &ics) {

	PP = fAllP[nCurrP];
	if(!PP) return FALSE;// ??? !!! Пока не разработано средство от зацикливания надо проверять

// Для поиска и расчета кр. сечения нужны некоторые данные из КС
	// Получим их
	T = ics.cc.T;
	T2 = ics.cc.T;
	T1 = 0;
	PPK = PP;
	PPA=0.61*PP;//0.61: на сколько обычно давление в критике меньше чем в КС для ЖРД и ТТ
		// 0.6^0.12 = 0,9405	// Вот откуда взялось 0.6: 0.6 =	0.12/(1.2-1)
	T = T*pow(0.6,0.12);
	BTMIN = 1E6;
	PPA = PPA-0.01*PPK;
	PP = PPA;

LB3_CALFN2:
	THERDYNEQCALCSTRUCT *ptdecs = pTherDynEqCalc->Calc(T,PP,PC.P,ics);
	if ( ((void *)ptdecs)==((void *)-1) ) return FALSE;
	if (!ptdecs) return FALSE;

	// Внутреняя структура для хранения данных текущего расчета
	memset(&tdecs,0,sizeof(tdecs));
	tdecs.IS = ptdecs->IS;
	tdecs.SS = ptdecs->SS;
	tdecs.CPR= ptdecs->CPR;
	tdecs.MS = ptdecs->MS;
	tdecs.CP3 = ptdecs->CP3;
	tdecs.ZK = ptdecs->ZK;
	tdecs.MG = ptdecs->MG;

	float H = ics.cc.SS;	// энтропия из камеры
	float C = tdecs.CPR/T;
	if(!H) return FALSE;
	// Расчет течения - постоянная энтропия
	T = T - (float(0.001))*SIGN(tdecs.SS-H);// SIGN(x)-функция нужна что бы сохранить знак 'x'
	float DT=(tdecs.SS-H)/C;
	if (fabs(DT)>T*(float(0.12)))
		DT=T*(float(0.12))*SIGN(DT);// SIGN(x)-функция нужна что бы сохранить знак 'x'
	if (DT>0)
		T2=T;
	else
		T1=T;
	if ((T2-T1 - fabs(DT)) < (float(0.1)))
		DT=(T2-T1)/2*SIGN(DT);// SIGN(x)-функция нужна что бы сохранить знак 'x'
	if (fabs(DT)>0.01) {			// Внимание: увеличенная точность
		T = T - DT;
		ics.FB = TRUE;
		goto LB3_CALFN2;
	}

	if (tdecs.MS!=0)
		ics.cc.RR = 847.9/tdecs.MS;
	W = sqrt(ics.cc.IS-tdecs.IS);
	PYD = 9.328*W;
	W = 91.48*W;
	BT = ics.cc.RR*T/W;
	ics.cs.RR = (ics.cs.RR+ics.cc.RR)/2;			// RC это RR в камере, и по мнемонике видно что RR на входе в сопло
	PYDP = PYD+BT;
	BT = BT * PPK/PP;			// BT = ((847.9/MS)*T/(91.48*sqrt(HK-IS)))*PPK/PP

	if (BTMIN>BT)	{
		BTMIN = BT; 
		// Сохранить
		_ST[1] = PP; _ST[2] = tdecs.IS; _ST[3] = T; _ST[4] = tdecs.MS; _ST[5] = tdecs.CP3;
		_ST[8] = PYD; _ST[9] = PYDP; _ST[10] = BT; _ST[11] = tdecs.ZK;

		PPA=PPA-0.01*PPK;
		T1=0;
		PP=PPA;	
		goto LB3_CALFN2;
	}

	// Загрузить
	PP = _ST[1]; tdecs.IS = _ST[2]; T = _ST[3]; tdecs.MS = _ST[4]; tdecs.CP3 = _ST[5];
	PYD = _ST[8]; PYDP = _ST[9]; BT = _ST[10]; tdecs.ZK = _ST[11];

	// Сохраним BTMIN,P и Т для расчета течения в сечениях
	ics.cs.BTMIN = BTMIN;
	ics.cs.T = T;
	ics.cs.P = PP;

	// ZKK это ZK в камере
	// ZK доля КОНДЕНСАТА, ZK == 1 когда в PABHPAC(); неудачный расчет
	if (ics.cc.ZK!=1)
		ics.cs.RR = log(PPK/PP) / log(PPK*T*ics.cc.MG*(1-tdecs.ZK)/(PP*ics.cc.T*tdecs.MG*(1-ics.cc.ZK)));
	fAllT[nCurrP][nCurrUXF][0] = T;
	fAllI[nCurrP][nCurrUXF][0] = tdecs.IS;
	fAllS[nCurrP][nCurrUXF][0] = tdecs.SS;
	fAllMS[nCurrP][nCurrUXF][0] = tdecs.MS;
	fAllCP3[nCurrP][nCurrUXF][0] = tdecs.CP3;
	fAllK3[nCurrP][nCurrUXF][0] = ics.cs.RR;
	fAllB[nCurrP][nCurrUXF][0] = BT;							// Расходный комплекс
	fAllPYD[nCurrP][nCurrUXF][0] = PYD;
	fAllPYDP[nCurrP][nCurrUXF][0] = PYDP;
	fAllF[nCurrP][nCurrUXF][0] = BT/PPK;					// Удельная площадь сечения
	fAllF_div_FKP[nCurrP][nCurrUXF][0] = BT/BTMIN;
	fAllZK[nCurrP][nCurrUXF][0] = tdecs.ZK;

	// Добавление значений давления ПС для будущего формирования отчета Excel
	CalcBase::pRep ->SaveSomePC(nCurrP, nCurrUXF, nCurrParam, PC);
	return TRUE;
}

// Во время использования объекта нужно передать ему данные расчета кр. сеч-INTOCALCSTRUCT &ics
CalcAnySectionWithPa::CalcAnySectionWithPa () 
: CalcBase()	// С начала запуск базового конструктора по умолчанию
{}

BOOL CalcAnySectionWithPa::Construct(CTherDynEqCalc *p, 
		UXFSTRUCT _uxf[MaxQuantityFIHBox], CALCSTRUCT _calcs) 
{
	CalcBase::nParam = _calcs.nPa;
	if (!CalcBase::ConstrLongParamRep(p, _uxf, _calcs))
		return FALSE;
	CalcBase::pRep->SetWayWrite(Report::showPa);
	for (int i1=0;i1<CalcBase::nParam;i1++)
		CalcBase::fAllParam[i1] = _calcs.fPa[i1]/1.03323;
	return TRUE;
}

BOOL CalcAnySectionWithPa::Calc(ProductsCombustion &PC, INTOCALCSTRUCT &ics) {

	PP = fAllP[nCurrP];
	if(!PP) return FALSE;// ??? !!! Пока не разработано средство от зацикливания надо проверять

// Для поиска сечения нужны некоторые данные из ???
	// Получим их
	T = ics.cs.T;
	PPK = ics.cc.P;
	PPA=fAllParam[nCurrParam];
	if (PPA<PP)
		T2=T;
	else
		T2=ics.cc.T;
	T2=T2+5;
	if (PPA>PPK)
		PPA=ics.cc.P;
	T = T*pow((float)(PPA/PP), (float)0.15);
	T1=0;
	PP=PPA;

LB3_CALFN2:
	THERDYNEQCALCSTRUCT *ptdecs = pTherDynEqCalc->Calc(T,PP,PC.P,ics);
	if ( ((void *)ptdecs)==((void *)-1) ) return FALSE;
	if (!ptdecs) return FALSE;

	// Внутреняя структура для хранения данных текущего расчета
	memset(&tdecs,0,sizeof(tdecs));
	tdecs.IS = ptdecs->IS;
	tdecs.SS = ptdecs->SS;
	tdecs.CPR= ptdecs->CPR;
	tdecs.MS = ptdecs->MS;
	tdecs.CP3 = ptdecs->CP3;
	tdecs.ZK = ptdecs->ZK;
	tdecs.MG = ptdecs->MG;

	float H = ics.cc.SS;	// энтропия из камеры
	float C = tdecs.CPR/T;
	if(!H) return FALSE;
	// Расчет течения - постоянная энтропия
	T = T - (float(0.001))*SIGN(tdecs.SS-H);// SIGN(x)-функция нужна что бы сохранить знак 'x'
	float DT=(tdecs.SS-H)/C;
	if (fabs(DT)>T*(float(0.12)))
		DT=T*(float(0.12))*SIGN(DT);// SIGN(x)-функция нужна что бы сохранить знак 'x'
	if (DT>0)
		T2=T;
	else
		T1=T;
	if ((T2-T1 - fabs(DT)) < (float(0.1)))
		DT=(T2-T1)/2*SIGN(DT);// SIGN(x)-функция нужна что бы сохранить знак 'x'
	if (fabs(DT)>0.3) {
		T = T - DT;
		ics.FB = TRUE;
		goto LB3_CALFN2;
	}

	if (tdecs.MS!=0)
		ics.as.RR = 847.9/tdecs.MS;
	W = sqrt(ics.cc.IS-tdecs.IS);
	PYD = 9.328*W;
	W = 91.48*W;
	BT = ics.as.RR*T/W;
	ics.as.RR = (ics.as.RR+ics.cc.RR)/2;
	PYDP = PYD+BT;
	E = (ics.cc.IS-tdecs.IS)/(ics.cc.T-T);
	E = E/(E-0.023485*ics.as.RR);
	BT = BT * PPK/PP;			// BT = ((847.9/MS)*T/(91.48*sqrt(HK-IS)))*PPK/PP
	// ZKK это ZK в камере
	// ZK доля КОНДЕНСАТА, ZK == 1 когда в PABHPAC(); неудачный расчет
	if (ics.cc.ZK!=1)
		ics.as.RR = log(PPK/PP) / log(PPK*T*ics.cc.MG*(1-tdecs.ZK)/(PP*ics.cc.T*tdecs.MG*(1-ics.cc.ZK)));

	fAllProcessP[nCurrP][nCurrUXF][nCurrParam] = fAllParam[nCurrParam];
	fAllT[nCurrP][nCurrUXF][nCurrParam] = T;
	fAllI[nCurrP][nCurrUXF][nCurrParam] = tdecs.IS;
	fAllS[nCurrP][nCurrUXF][nCurrParam] = tdecs.SS;
	fAllMS[nCurrP][nCurrUXF][nCurrParam] = tdecs.MS;
	fAllCP3[nCurrP][nCurrUXF][nCurrParam] = tdecs.CP3;
	fAllK3[nCurrP][nCurrUXF][nCurrParam] = ics.as.RR;
	fAllB[nCurrP][nCurrUXF][nCurrParam] = BT;							// Расходный комплекс
	fAllPYD[nCurrP][nCurrUXF][nCurrParam] = PYD;
	fAllPYDP[nCurrP][nCurrUXF][nCurrParam] = PYDP;
	fAllF[nCurrP][nCurrUXF][nCurrParam] = BT/PPK;					// Удельная площадь сечения
	fAllF_div_FKP[nCurrP][nCurrUXF][nCurrParam] = BT/ics.cs.BTMIN;
	fAllZK[nCurrP][nCurrUXF][nCurrParam] = tdecs.ZK;

	// Добавление значений давления ПС для будущего формирования отчета Excel
	CalcBase::pRep ->SaveSomePC(nCurrP, nCurrUXF, nCurrParam, PC);
	return TRUE;
}


CalcAnySectionWithF::CalcAnySectionWithF ()
: CalcBase(){}	// С начала запуск базового конструктора по умолчанию

BOOL CalcAnySectionWithF::Construct(CTherDynEqCalc *p, 
		UXFSTRUCT _uxf[MaxQuantityFIHBox], CALCSTRUCT _calcs) 
{
	CalcBase::nParam = _calcs.nF;
	if (!CalcBase::ConstrLongParamRep(p, _uxf, _calcs))
		return FALSE;
	CalcBase::pRep->SetWayWrite(Report::showPEN);
	for (int i1=0;i1<CalcBase::nParam;i1++)
		CalcBase::fAllParam[i1] = _calcs.fF[i1];
	return TRUE;
}

BOOL CalcAnySectionWithF::Calc(ProductsCombustion &PC, INTOCALCSTRUCT &ics) {

	PP = fAllP[nCurrP];
	if(!PP) return FALSE;// ??? !!! Пока не разработано средство от зацикливания надо проверять

// Для поиска сечения нужны некоторые данные из ???
	// Получим их
	T = ics.cs.T;
	PPK = PP;
	FA=fAllParam[nCurrParam];

	if(FA<=1) {	// Степень расширения сопла должна быть больше 1
		MessageBox(0,"Расчет по степеням расширения сопла не возможен. Неправильная степень расширения.","Calc error",MB_OK);
		return FALSE;
	}

	PX=(float)0.1E-18;
	PY=0;
	FX=0;
	FY=0;
	PPA=PPK*0.06/(FA-1);
	DA=1;
	if ( ((nParam-1)!=nCurrParam) &&(FA>=fAllParam[nCurrParam+1]) )
		DA=-1;
//старая метка LL:	_Func__T2_PPA_PP_P1_P2_T_FP3_T1(FT,F3C,FP3,T3AM,P1,T,T2,T1,PP,P2);
	if (PPA<PP)
		T2=T;
	else
		T2=ics.cc.T;
	T2=T2+5;
//	if ((FF)&&((DA*PPA)>(DA*PKP)))	// FF - старый флаг расчета степени расширения сопла
	if ( (DA*PPA)>(DA*ics.cs.P) )
		PPA=ics.cs.P;
//	if (!FT) {								// Расчет расширения по соответств. ??? темперетурам
		if (PPA>PPK)
			PPA=PPK;
//	}
	T = T*pow((float)(PPA/PP), (float)0.15);	// T=T*(PPA/PP)**0.15;	Неизвестный оператор **
	T1=0;
	PP=PPA;

LB3_CALFN2:
	THERDYNEQCALCSTRUCT *ptdecs = pTherDynEqCalc->Calc(T,PP,PC.P,ics);
	if ( ((void *)ptdecs)==((void *)-1) ) return FALSE;
	if (!ptdecs) return FALSE;

	// Внутреняя структура для хранения данных текущего расчета
	memset(&tdecs,0,sizeof(tdecs));
	tdecs.IS = ptdecs->IS;
	tdecs.SS = ptdecs->SS;
	tdecs.CPR= ptdecs->CPR;
	tdecs.MS = ptdecs->MS;
	tdecs.CP3 = ptdecs->CP3;
	tdecs.ZK = ptdecs->ZK;
	tdecs.MG = ptdecs->MG;

	float H = ics.cc.SS;	// энтропия из камеры
	float C = tdecs.CPR/T;
	if(!H) return FALSE;
	// Расчет течения - постоянная энтропия
	T = T - (float(0.001))*SIGN(tdecs.SS-H);// SIGN(x)-функция нужна что бы сохранить знак 'x'
	float DT=(tdecs.SS-H)/C;
	if (fabs(DT)>T*(float(0.12)))
		DT=T*(float(0.12))*SIGN(DT);// SIGN(x)-функция нужна что бы сохранить знак 'x'
	if (DT>0)
		T2=T;
	else
		T1=T;
	if ((T2-T1 - fabs(DT)) < (float(0.1)))
		DT=(T2-T1)/2*SIGN(DT);// SIGN(x)-функция нужна что бы сохранить знак 'x'
	if (fabs(DT)>0.01) {		// Внимание: увеличенная точность
		T = T - DT;
		ics.FB = TRUE;
		goto LB3_CALFN2;
	}

	if (tdecs.MS!=0)
		ics.as.RR = 847.9/tdecs.MS;
	W = sqrt(ics.cc.IS-tdecs.IS);
	PYD = 9.328*W;
	W = 91.48*W;
	BT = ics.as.RR*T/W;
	ics.as.RR = (ics.as.RR+ics.cc.RR)/2;
	PYDP = PYD+BT;
	E = (ics.cc.IS-tdecs.IS)/(ics.cc.T-T);
	E = E/(E-0.023485*ics.as.RR);
	BT = BT * PPK/PP;			// BT = ((847.9/MS)*T/(91.48*sqrt(HK-IS)))*PPK/PP
	// ZKK это ZK в камере
	// ZK доля КОНДЕНСАТА, ZK == 1 когда в PABHPAC(); неудачный расчет
	if (ics.cc.ZK!=1)
		ics.as.RR = log(PPK/PP) / log(PPK*T*ics.cc.MG*(1-tdecs.ZK)/(PP*ics.cc.T*tdecs.MG*(1-ics.cc.ZK)));

	fDifference = FA-BT/ics.cs.BTMIN;	// BT/BTMIN - Геом. степень расширения сопла
	fDifference = fDifference*DA;			// DA - направление
	if ( (fabs(fDifference) > (0.0005*FA)) && ((fabs(PY-PX)) >= (0.01*PY))) {
		//F0T=CKACH=FALSE;
//старая метка LR: _Func_if_FF__PX_PY_FX_FY_PPA(PP);// Обработка различных сечений сопла
		if (fDifference>0)
			{ PY=PP; FY=fDifference; }	// Если текущая площадь меньше искомой
		else
			{ PX=PP; FX=fDifference; }	// Если текущая площадь превышает искомую
		if ((FX*FY)==0) {	
			// Сюда будет вход пока текущая площадь 
			// не превысит искомую и потом не станет меньше искомой
			if (fDifference<0)
				PPA=PP*2;		// Если текущая площадь превышает искомую, увеличим давление
			else
				PPA=PP/2;
		} 
		else
			PPA=PX+FX*(PY-PX)/(FX-FY);	// В ранней версии была ошибка:PX -_ FX*(PY-PX)/(FX-FY)

		if (PPA<=0)
			 return FALSE; // Расчет не удался
//старая метка LL: _Func__T2_PPA_PP_P1_P2_T_FP3_T1(FT,F3C,FP3,T3AM,P1,T,T2,T1,PP,P2);
		if (PPA<PP)
			T2=T;
		else
			T2=ics.cc.T;
		T2=T2+5;
//		if ((FF)&&((DA*PPA)>(DA*PKP)))	// FF - старый флаг расчета степени расширения сопла
		if ( (DA*PPA)>(DA*ics.cs.P) )
			PPA=ics.cs.P;
//		if (!FT) {
			if (PPA>PPK)
				PPA=PPK;
//		}
		T = T*pow((float)(PPA/PP), (float)0.15);	// T=T*(PPA/PP)**0.15;	Неизвестный оператор **
		T1=0;
		PP=PPA;
		goto LB3_CALFN2;	// продолжать расчет при новой температуре и давлении
	}
	PX = PY;

//Высчитать давление и записать его в массив
	int i;
	float fCalcP = 0, fCalcP2 = 0;
	for (i=1;i<=PC.nPGas;i++)								// Сумма мольных долей газа
		if (PC.P[i] > 0) 
			fCalcP += PC.P[i];
		else
			fCalcP += exp(PC.P[i]);
	for (i=1;i<=PC.nPGas;i++)								// Сумма парциальных давлений газа
		if (PC.P[i] > 0) 
			fCalcP2 += fAllP[nCurrP]*PC.P[i]/fCalcP;	// Из мольных долей в парциальные давления
		else
			fCalcP2 += fAllP[nCurrP]*exp(PC.P[i])/fCalcP;
	fAllProcessP[nCurrP][nCurrUXF][nCurrParam] = fCalcP2;
	fAllT[nCurrP][nCurrUXF][nCurrParam] = T;
	fAllI[nCurrP][nCurrUXF][nCurrParam] = tdecs.IS;
	fAllS[nCurrP][nCurrUXF][nCurrParam] = tdecs.SS;
	fAllMS[nCurrP][nCurrUXF][nCurrParam] = tdecs.MS;
	fAllCP3[nCurrP][nCurrUXF][nCurrParam] = tdecs.CP3;
	fAllK3[nCurrP][nCurrUXF][nCurrParam] = ics.as.RR;
	fAllB[nCurrP][nCurrUXF][nCurrParam] = BT;							// Расходный комплекс
	fAllPYD[nCurrP][nCurrUXF][nCurrParam] = PYD;
	fAllPYDP[nCurrP][nCurrUXF][nCurrParam] = PYDP;
	fAllF[nCurrP][nCurrUXF][nCurrParam] = BT/PPK;					// Удельная площадь сечения
	fAllF_div_FKP[nCurrP][nCurrUXF][nCurrParam] = BT/ics.cs.BTMIN;
	fAllZK[nCurrP][nCurrUXF][nCurrParam] = tdecs.ZK;

	// Добавление значений давления ПС для будущего формирования отчета Excel
	CalcBase::pRep ->SaveSomePC(nCurrP, nCurrUXF, nCurrParam, PC);
	return TRUE;
}

CalcISDiagram::CalcISDiagram() 
: CalcBase()	// С начала запуск базового конструктора по умолчанию
{}

BOOL CalcISDiagram::Construct(CTherDynEqCalc *p, 
		UXFSTRUCT _uxf[MaxQuantityFIHBox], CALCSTRUCT _calcs) 
{
	CalcBase::nParam = _calcs.nIS;
	if (!CalcBase::ConstrLongParamRep(p, _uxf, _calcs))
		return FALSE;
	CalcBase::pRep->SetWayWrite(Report::showIS);
	for (int i1=0;i1<CalcBase::nParam;i1++)
		CalcBase::fAllParam[i1] = _calcs.fIS[i1];
	return TRUE;
}

BOOL CalcISDiagram::Calc(ProductsCombustion &PC, INTOCALCSTRUCT &ics) {

	PP = fAllP[nCurrP];
	if(!PP) return FALSE;// ??? !!! Пока не разработано средство от зацикливания надо проверять

// Для расчета нужны некоторые данные из ???
	// Получим их
	T = fAllParam[nCurrParam];
	PPK = ics.cc.P;

	THERDYNEQCALCSTRUCT *ptdecs = pTherDynEqCalc->Calc(T,PP,PC.P,ics);
	if ( ((void *)ptdecs)==((void *)-1) ) return FALSE;
	if (!ptdecs) return FALSE;

	// Внутреняя структура для хранения данных текущего расчета
	memset(&tdecs,0,sizeof(tdecs));
	tdecs.IS = ptdecs->IS;
	tdecs.SS = ptdecs->SS;
	tdecs.CPR= ptdecs->CPR;
	tdecs.MS = ptdecs->MS;
	tdecs.CP3 = ptdecs->CP3;
	tdecs.ZK = ptdecs->ZK;
	tdecs.MG = ptdecs->MG;

	if (tdecs.MS!=0)
		ics.as.RR = 847.9/tdecs.MS;
	W = sqrt(ics.cc.IS-tdecs.IS);
	PYD = 9.328*W;
	W = 91.48*W;
	BT = ics.as.RR*T/W;
	ics.as.RR = (ics.as.RR+ics.cc.RR)/2;
	PYDP = PYD+BT;
	E = (ics.cc.IS-tdecs.IS)/(ics.cc.T-T);
	E = E/(E-0.023485*ics.as.RR);
	BT = BT * PPK/PP;			// BT = ((847.9/MS)*T/(91.48*sqrt(HK-IS)))*PPK/PP
	// ZKK это ZK в камере
	// ZK доля КОНДЕНСАТА, ZK == 1 когда в PABHPAC(); неудачный расчет
	if (ics.cc.ZK!=1)
		ics.as.RR = log(PPK/PP) / log(PPK*T*ics.cc.MG*(1-tdecs.ZK)/(PP*ics.cc.T*tdecs.MG*(1-ics.cc.ZK)));

//Высчитать давление и записать его в массив
	int i;
	float fCalcP = 0, fCalcP2 = 0;
	for (i=1;i<=PC.nPGas;i++)								// Сумма мольных долей газа
		if (PC.P[i] > 0) 
			fCalcP += PC.P[i];
		else
			fCalcP += exp(PC.P[i]);
	for (i=1;i<=PC.nPGas;i++)								// Сумма парциальных давлений газа
		if (PC.P[i] > 0) 
			fCalcP2 += fAllP[nCurrP]*PC.P[i]/fCalcP;	// Из мольных долей в парциальные давления
		else
			fCalcP2 += fAllP[nCurrP]*exp(PC.P[i])/fCalcP;
	fAllProcessP[nCurrP][nCurrUXF][nCurrParam] = fCalcP2;
	fAllT[nCurrP][nCurrUXF][nCurrParam] = T;
	fAllI[nCurrP][nCurrUXF][nCurrParam] = tdecs.IS;
	fAllS[nCurrP][nCurrUXF][nCurrParam] = tdecs.SS;
	fAllMS[nCurrP][nCurrUXF][nCurrParam] = tdecs.MS;
	fAllCP3[nCurrP][nCurrUXF][nCurrParam] = tdecs.CP3;
	fAllK3[nCurrP][nCurrUXF][nCurrParam] = ics.as.RR;
	fAllB[nCurrP][nCurrUXF][nCurrParam] = BT;							// Расходный комплекс
	fAllPYD[nCurrP][nCurrUXF][nCurrParam] = PYD;
	fAllPYDP[nCurrP][nCurrUXF][nCurrParam] = PYDP;
	fAllF[nCurrP][nCurrUXF][nCurrParam] = BT/PPK;					// Удельная площадь сечения
	fAllF_div_FKP[nCurrP][nCurrUXF][nCurrParam] = BT/ics.cs.BTMIN;
	fAllZK[nCurrP][nCurrUXF][nCurrParam] = tdecs.ZK;

	// Добавление значений давления ПС для будущего формирования отчета Excel
	CalcBase::pRep ->SaveSomePC(nCurrP, nCurrUXF, nCurrParam, PC);
	return TRUE;
}

CalcConstV::CalcConstV() 
: CalcBase()	// С начала запуск базового конструктора по умолчанию
{}

BOOL CalcConstV::Construct(CTherDynEqCalc *p, 
		UXFSTRUCT _uxf[MaxQuantityFIHBox], CALCSTRUCT _calcs) 
{
	CalcBase::nParam = _calcs.nV;
	if (!CalcBase::ConstrShortRep(p, _uxf, _calcs))
		return FALSE;
	CalcBase::pRep->SetWayWrite(Report::showV);
	for (int i1=0;i1<CalcBase::nParam;i1++)
		CalcBase::fAllParam[i1] = _calcs.fV[i1];
	return TRUE;
}

BOOL CalcConstV::Calc(ProductsCombustion &PC, INTOCALCSTRUCT &ics) {

	T = fAssumedT;
	T1 = fMinT;
	T2 = fMaxT;

	PP = fAllParam[nCurrParam];
	if(!PP) return FALSE;// ??? !!! Пока не разработано средство от зацикливания надо проверять

LB3_CALFN2:
	THERDYNEQCALCSTRUCT *ptdecs = pTherDynEqCalc->Calc(T,PP,PC.P,ics);
	if ( ((void *)ptdecs)==((void *)-1) ) return FALSE;
	if (!ptdecs) return FALSE;

	// Внутреняя структура для хранения данных текущего расчета
	memset(&tdecs,0,sizeof(tdecs));
	tdecs.IS = ptdecs->IS;
	tdecs.SS = ptdecs->SS;
	tdecs.CPR = ptdecs->CPR;
	tdecs.MS = ptdecs->MS;
	tdecs.CP3 = ptdecs->CP3;
	tdecs.ZK = ptdecs->ZK;
	tdecs.MG = ptdecs->MG;

	float H = fAllEnthalpyF[nCurrUXF];
	float W = tdecs.IS-CTherDynEqCalc::R0*T/tdecs.MS;
	float C = tdecs.CPR;
	if(!H) return FALSE;
	// Расчет горения - постоянная энтальпия
	T = T - (float(0.001))*SIGN(W-H);// SIGN(x)-функция нужна что бы сохранить знак 'x'
	float DT=(W - H)/C;
	if (fabs(DT)>T*(float(0.12)))
		DT=T*(float(0.12))*SIGN(DT);// SIGN(x)-функция нужна что бы сохранить знак 'x'
	if (DT>0)
		T2=T;
	else
		T1=T;
	if ((T2-T1 - fabs(DT)) < (float(0.1)))
		DT=(T2-T1)/2*SIGN(DT);// SIGN(x)-функция нужна что бы сохранить знак 'x'
	if (fabs(DT)>0.3) {
		T = T - DT;
		ics.FB = TRUE;
		goto LB3_CALFN2;
	}

//Высчитать давление и записать его в массив
	int i;
	float fCalcP = 0, fCalcP2 = 0;
	for (i=1;i<=PC.nPGas;i++)								// Сумма мольных долей газа
		if (PC.P[i] > 0) 
			fCalcP += PC.P[i];
		else
			fCalcP += exp(PC.P[i]);
	for (i=1;i<=PC.nPGas;i++)								// Сумма парциальных давлений газа
		if (PC.P[i] > 0) 
			fCalcP2 += fAllP[nCurrP]*PC.P[i]/fCalcP;	// Из мольных долей в парциальные давления
		else
			fCalcP2 += fAllP[nCurrP]*exp(PC.P[i])/fCalcP;
	fAllProcessP[nCurrP][nCurrUXF][nCurrParam] = fCalcP2;
	fAllT[nCurrP][nCurrUXF][0] = T;
	fAllI[nCurrP][nCurrUXF][0] = tdecs.IS;
	fAllS[nCurrP][nCurrUXF][0] = tdecs.SS;
	fAllMS[nCurrP][nCurrUXF][0] = tdecs.MS;
	fAllCP3[nCurrP][nCurrUXF][0] = tdecs.CP3;
	fAllZK[nCurrP][nCurrUXF][0] = tdecs.ZK;

	// Добавление значений давления ПС для будущего формирования отчета Excel
	CalcBase::pRep ->SaveSomePC(nCurrP, nCurrUXF, nCurrParam, PC);
	return TRUE;
}

// Во время использования объекта нужно передать ему данные расчета КС - INTOCALCSTRUCT &ics
CalcEPS::CalcEPS() 
: CalcBase()	// С начала запуск базового конструктора по умолчанию
{}

BOOL CalcEPS::Construct(CTherDynEqCalc *p, 
		UXFSTRUCT _uxf[MaxQuantityFIHBox], CALCSTRUCT _calcs) 
{
	CalcBase::nParam = _calcs.nEPS;
	if (!CalcBase::ConstrLongParamRep(p, _uxf, _calcs))
		return FALSE;
	CalcBase::pRep->SetWayWrite(Report::showEPS);
	for (int i1=0;i1<CalcBase::nParam;i1++)
		CalcBase::fAllParam[i1] = _calcs.fEPS[i1];
	return TRUE;
}

BOOL CalcEPS::Calc(ProductsCombustion &PC, INTOCALCSTRUCT &ics) {
	PP = fAllP[nCurrP];
	if(!PP) return FALSE;// ??? !!! Пока не разработано средство от зацикливания надо проверять

// Для расчета нужны некоторые данные из ???
	// Получим их
	T = ics.cs.T;
	PPK = ics.cc.P;
	PPA = PPK/fAllParam[nCurrParam];	// Степень расширения газа в сопле

	if (PPA<PP)
		T2=T;
	else
		T2=ics.cc.T;
	T2=T2+5;
	if (PPA>PPK)
		PPA=ics.cc.P;
	T = T*pow((float)(PPA/PP), (float)0.15);
	T1=0;
	PP=PPA;

LB3_CALFN2:
	THERDYNEQCALCSTRUCT *ptdecs = pTherDynEqCalc->Calc(T,PP,PC.P,ics);
	if ( ((void *)ptdecs)==((void *)-1) ) return FALSE;
	if (!ptdecs) return FALSE;

	// Внутреняя структура для хранения данных текущего расчета
	memset(&tdecs,0,sizeof(tdecs));
	tdecs.IS = ptdecs->IS;
	tdecs.SS = ptdecs->SS;
	tdecs.CPR= ptdecs->CPR;
	tdecs.MS = ptdecs->MS;
	tdecs.CP3 = ptdecs->CP3;
	tdecs.ZK = ptdecs->ZK;
	tdecs.MG = ptdecs->MG;

	float H = ics.cc.SS;	// энтропия из камеры
	float C = tdecs.CPR/T;
	if(!H) return FALSE;
	// Расчет течения - постоянная энтропия
	T = T - (float(0.001))*SIGN(tdecs.SS-H);// SIGN(x)-функция нужна что бы сохранить знак 'x'
	float DT=(tdecs.SS-H)/C;
	if (fabs(DT)>T*(float(0.12)))
		DT=T*(float(0.12))*SIGN(DT);// SIGN(x)-функция нужна что бы сохранить знак 'x'
	if (DT>0)
		T2=T;
	else
		T1=T;
	if ((T2-T1 - fabs(DT)) < (float(0.1)))
		DT=(T2-T1)/2*SIGN(DT);// SIGN(x)-функция нужна что бы сохранить знак 'x'
	if (fabs(DT)>0.3) {
		T = T - DT;
		ics.FB = TRUE;
		goto LB3_CALFN2;
	}

	if (tdecs.MS!=0)
		ics.as.RR = 847.9/tdecs.MS;
	W = sqrt(ics.cc.IS-tdecs.IS);
	PYD = 9.328*W;
	W = 91.48*W;
	BT = ics.as.RR*T/W;
	ics.as.RR = (ics.as.RR+ics.cc.RR)/2;
	PYDP = PYD+BT;
	E = (ics.cc.IS-tdecs.IS)/(ics.cc.T-T);
	E = E/(E-0.023485*ics.as.RR);
	BT = BT * PPK/PP;			// BT = ((847.9/MS)*T/(91.48*sqrt(HK-IS)))*PPK/PP
	// ZKK это ZK в камере
	// ZK доля КОНДЕНСАТА, ZK == 1 когда в PABHPAC(); неудачный расчет
	if (ics.cc.ZK!=1)
		ics.as.RR = log(PPK/PP) / log(PPK*T*ics.cc.MG*(1-tdecs.ZK)/(PP*ics.cc.T*tdecs.MG*(1-ics.cc.ZK)));

	fAllT[nCurrP][nCurrUXF][nCurrParam] = T;
	fAllI[nCurrP][nCurrUXF][nCurrParam] = tdecs.IS;
	fAllS[nCurrP][nCurrUXF][nCurrParam] = tdecs.SS;
	fAllMS[nCurrP][nCurrUXF][nCurrParam] = tdecs.MS;
	fAllCP3[nCurrP][nCurrUXF][nCurrParam] = tdecs.CP3;
	fAllK3[nCurrP][nCurrUXF][nCurrParam] = ics.as.RR;
	fAllB[nCurrP][nCurrUXF][nCurrParam] = BT;							// Расходный комплекс
	fAllPYD[nCurrP][nCurrUXF][nCurrParam] = PYD;
	fAllPYDP[nCurrP][nCurrUXF][nCurrParam] = PYDP;
	fAllF[nCurrP][nCurrUXF][nCurrParam] = BT/PPK;					// Удельная площадь сечения
	fAllF_div_FKP[nCurrP][nCurrUXF][nCurrParam] = BT/ics.cs.BTMIN;
	fAllZK[nCurrP][nCurrUXF][nCurrParam] = tdecs.ZK;

	// Добавление значений давления ПС для будущего формирования отчета Excel
	CalcBase::pRep ->SaveSomePC(nCurrP, nCurrUXF, nCurrParam, PC);
	return TRUE;
}


FirstApproximation::FirstApproximation() {}

BOOL FirstApproximation::Construct(JobWithDB *p, UXFSTRUCT _uxf[MaxQuantityFIHBox], 
		CALCSTRUCT _calcs) 
{
	if (!p) return FALSE;
	pDBs = p;
	nCurrUXF = 0;
	for (int i = 0;i<MaxQuantityFIHBox;i++)
		fa_uxf[i] = _uxf[i];
	nAlfa = _calcs.nEditBoxesAlfa;
	return TRUE;
}

void FirstApproximation::PrepareToFirstCalc() {nCurrUXF = 0;}
BOOL FirstApproximation::NextAlfa() {
	if ( (nCurrUXF+1)>=nAlfa )
		return FALSE;
	nCurrUXF++;
	return TRUE;
}

BOOL FirstApproximation::Calc(ProductsCombustion &PC, INTOCALCSTRUCT &ics) {
	int I;
	int I1;
	int I2;
	int J;
	int J1;
	int X;
	int Y;

	memset(XF1,0,sizeof(XF1));
	memset(XF2,0,sizeof(XF2));
	if (!pDBs->GetTERMO_XF1andXF2(XF1, XF2))
		return FALSE;

//	pDBs->GetAllchem_data(nPElementaryElem, nPGas, NULL, nPCond, NULL);
//	nPAll = nPGas + nPCond;

	memset(MC,0,sizeof(MC));
	for(int cur_elem=1;cur_elem<=fa_uxf[nCurrUXF].nQuantityElemF;cur_elem++)
		MC[GetSN(fa_uxf[nCurrUXF].szAtomsOfF[cur_elem-1])]=
				fa_uxf[nCurrUXF].fConditChimFormF[cur_elem-1];

	long A1,A2;
	memset(ics.A5,0,sizeof(ics.A5));
	float MIN=0;
	for(I=PC.nPElementaryElem+1;I<=PC.nPAll;I++){
		A1=fabs(XF1[I]);
		XF2[I]=fabs(XF2[I]);
		A2=XF2[I];
		MIN=0;
		X=1;
		Y=1;
		for (I1=1;I1<=4;I1++){
			J=floor((float)A1/X)-floor((float)A1/(X*100))*100;
			if (J==0) 
				goto LL;
			J1=floor((float)A2/Y)-floor((float)A2/(Y*10))*10;

			for (I2=1;I2<=PC.nPElementaryElem;I2++){
				if (J == XF1[I2])
					ics.A5[I2] = ics.A5[I2]+J1;
			}

			if (I>PC.nPGas)
				MIN += M[J]*J1;
			X=X*100;
			Y=Y*10;
		}
LL:
		if (I>PC.nPGas)
			ics.MK[I-PC.nPGas]=MIN;
	}
	// Было подсчитано кол-во атомов в соединениях

	MIN=(float)1E17;
	for (I=1;I<=PC.nPElementaryElem;I++) {
		J=XF1[I];
		if ( (MC[J]/(ics.A5[I]+1))<MIN )
			MIN=MC[J]/(ics.A5[I]+1);
	}
	for (I=PC.nPElementaryElem+1;I<=PC.nPAll;I++)
		PC.P[I]=MIN;
// Был проведен поиск какого-то минимального значения 
// и далнейшее присвоения его сложным соединениям

	for (I=1;I<=PC.nPElementaryElem;I++) {
		J=XF1[I];
		PC.P[I]=MC[J]-MIN*ics.A5[I];
		//P[I]=MC[XF1[I]]-MIN*A5[I];
		XF2[I]=-fabs(XF2[I]);
		ics.A5[I]=I;
//		LP[I]=log(P[I]);	// Натуральный логарифм
	}

	// формируется матрица состава атомарных веществ
	for (I=1;I<=PC.nPElementaryElem;I++) {
		for (J=1;J<=PC.nPElementaryElem;J++) {
			if (I==J)
				ics.B[I][J]=1;
			else
				ics.B[I][J]=0;
		}
	}

	return TRUE;
}

JobWithDB::JobWithDB() {
	memset(&uxf_with_common_atoms,0,sizeof(uxf_with_common_atoms));
	memset(&cdG,0,sizeof(cdG));
	memset(&cdG,0,sizeof(cdC));
	bWasReadDBs = FALSE;
}

BOOL JobWithDB::Construct(UXFSTRUCT _uxf) {
	if (!&_uxf) return FALSE;
	uxf_with_common_atoms = _uxf;
	nPElementaryElem = nPGas = nPCond = nPAll = 0;
	return TRUE;
}

BOOL	JobWithDB::OpenAndReadGasDB() {
#ifdef YELLOW	// Желтая методичка
	strcpy(szAtomsOfF[0],"H");	fConditChimFormF[0] = 0.0484*1000/M[GetSN(szAtomsOfF[0])];
	strcpy(szAtomsOfF[1],"C");	fConditChimFormF[1] = 0.1371*1000/M[GetSN(szAtomsOfF[1])];
	strcpy(szAtomsOfF[2],"O");	fConditChimFormF[2] = 0.4032*1000/M[GetSN(szAtomsOfF[2])];
	strcpy(szAtomsOfF[3],"Cl");fConditChimFormF[3] = 0.2233*1000/M[GetSN(szAtomsOfF[3])];
	strcpy(szAtomsOfF[4],"N");	fConditChimFormF[4] = 0.1371*1000/M[GetSN(szAtomsOfF[4])];
	strcpy(szAtomsOfF[5],"Al");float sss = 1-(0.0484+0.1371+0.4032+0.2233+0.1371);
	fConditChimFormF[5] = sss*1000/M[GetSN(szAtomsOfF[5])];	nQuantityElemF = 6;
#endif
	nPGas = GetPossibleChemCombinGas(cdG,uxf_with_common_atoms.szAtomsOfF,
			uxf_with_common_atoms.nQuantityElemF);
	if (nPGas == -1) {
		MessageBox(0,"Не могу начать работу с базой данных по газообразным соединениям!","Fatal calculation error",0);
		return FALSE;
	}
	return TRUE;
}

BOOL	JobWithDB::OpenAndReadCondensateDB() {
#ifdef YELLOW	// Желтая методичка
	strcpy(szAtomsOfF[0],"H");	fConditChimFormF[0] = 0.0484*1000/M[GetSN(szAtomsOfF[0])];
	strcpy(szAtomsOfF[1],"C");	fConditChimFormF[1] = 0.1371*1000/M[GetSN(szAtomsOfF[1])];
	strcpy(szAtomsOfF[2],"O");	fConditChimFormF[2] = 0.4032*1000/M[GetSN(szAtomsOfF[2])];
	strcpy(szAtomsOfF[3],"Cl");fConditChimFormF[3] = 0.2233*1000/M[GetSN(szAtomsOfF[3])];
	strcpy(szAtomsOfF[4],"N");	fConditChimFormF[4] = 0.1371*1000/M[GetSN(szAtomsOfF[4])];
	strcpy(szAtomsOfF[5],"Al");float sss = 1-(0.0484+0.1371+0.4032+0.2233+0.1371);
	fConditChimFormF[5] = sss*1000/M[GetSN(szAtomsOfF[5])];	nQuantityElemF = 6;
#endif
	nPCond = GetPossibleChemCombinCond(cdC,uxf_with_common_atoms.szAtomsOfF,
			uxf_with_common_atoms.nQuantityElemF);
	if (nPCond == -1) {
		MessageBox(0,"Не могу начать работу с базой данных по конденсированным соединениям!","Fatal calculation error",0);
		return FALSE;
	}
	return TRUE;
}

BOOL	JobWithDB::ReadDBs() {
	if (!OpenAndReadGasDB())
		return FALSE;
	if (!OpenAndReadCondensateDB())
		return FALSE;
	nPAll = nPGas + nPCond;
	if (!nPAll) {
		MessageBox(0,"Расчет невозможен, так как в базах данных \
не найдено ни одного нужного соединения!","Fatal calculation error",0);
		return FALSE;
	}
	bWasReadDBs = TRUE;
	return TRUE;
}

BOOL	JobWithDB::GetNamesPC(ProductsCombustion &PC) {
	if (!bWasReadDBs) return FALSE;

// Перенести в начало газообр. хим. соединения являющиеся прост. хим. эл.
	int _M = 0;
	chem_dataGas cdr;
	BOOL bWasFound;
	for (int s=0;s<nPGas;s++) {
		int nQuantAtomsFromDB =	TransNameMolecToAandF(szElemsFromDB,fT,cdG[s].view);

		if ((nQuantAtomsFromDB!=1)||(fT[0]!=1)) {
			bWasFound=FALSE;
			for (int s2=s+1;s2<nPGas;s2++) {
				int nQuantAtomsFromDB =	TransNameMolecToAandF(szElemsFromDB,fT,cdG[s2].view);			
				if ((nQuantAtomsFromDB==1)&&(fT[0]==1)) {
					cdr=cdG[s];
					cdG[s]=cdG[s2];
					cdG[s2]=cdr;
					bWasFound=TRUE;
					_M++;
					break;
				}
			}
			if(!bWasFound) break; // Конец
		}
		else
			_M++;
	}
	nPElementaryElem = _M;

// Названия компонентов топлива и отступ для записи их количества
	nMaxNamesLen = 0;
	int cur_elem;
	for (cur_elem=1;cur_elem<=nPGas;cur_elem++) {
		strcpy(PC.szNam[cur_elem],cdG[cur_elem-1].view);
		if (strlen(PC.szNam[cur_elem])>nMaxNamesLen)
			nMaxNamesLen=strlen(PC.szNam[cur_elem]);
	}

	for (cur_elem=1;cur_elem<=nPCond;cur_elem++) {
		strcpy(PC.szNam[nPGas+cur_elem],szCondSymbol);
		strcpy(&PC.szNam[nPGas+cur_elem][2],cdC[cur_elem-1].view);
		if (strlen(PC.szNam[cur_elem])>nMaxNamesLen)
			nMaxNamesLen=strlen(PC.szNam[cur_elem]);
	}
	for (cur_elem=1;cur_elem<=nPAll;cur_elem++) {
		unsigned int s1 = strlen(PC.szNam[cur_elem]);
		PC.szNam[cur_elem][s1] = ':'; 
		s1++;
		for (s1;s1<nMaxNamesLen+1;s1++)
			PC.szNam[cur_elem][s1] = ' ';
		PC.szNam[cur_elem][s1] = 0; 
	}

	PC.nPElementaryElem = nPElementaryElem;
	PC.nPGas = nPGas;
	PC.nPCond = nPCond;
	PC.nPAll = nPAll;
	PC.nMaxNamesLen = nMaxNamesLen;

	return TRUE;
}

BOOL	JobWithDB::GetTERMO_XF1andXF2(float _XF1[120], float _XF2[120]) {
// Заполнение массивов с именами(номерами) и коэффициентами компонентов
	int X,Y;
	int i;
	for (i=1;i<=nPGas;i++) {
		// Перепишем молекулу из БД в список по атомам
		int nQuantAtomsFromDB =
			TransNameMolecToAandF(szElemsFromDB,fT,cdG[i-1].view);
		if (nQuantAtomsFromDB>MaxAtoms) {
			wsprintf(Mstr,"Соединение <%s> из БД,\
				\n состоит из более 'MaxAtoms' элементов.",cdG[i-1].view);
			MessageBox(0,Mstr,"Message",0);
			return FALSE;
		}
		X=Y=1;
		for (int j=0;j<nQuantAtomsFromDB;j++) {
			_XF1[i]+=GetSN(szElemsFromDB[j])*X;
			_XF2[i]+=fT[j]*Y;
			fT[j]=0;
			X=X*100;
			Y=Y*10;
		}
	}
	for (i=1;i<=nPCond;i++) {
		// Перепишем молекулу из БД в список по атомам
		int nQuantAtomsFromDB =
			TransNameMolecToAandF(szElemsFromDB,fT,cdC[i-1].view);
		if (nQuantAtomsFromDB>4) {
			wsprintf(Mstr,"Конденс. соединение <%s> из БД,\
				\n состоит из более 4 элементов.",cdC[i-1].view);
			MessageBox(0,Mstr,"Message",0);
			return FALSE;
		}
		X=Y=1;
		for (int j=0;j<nQuantAtomsFromDB;j++) {
			_XF1[nPGas+i]+=GetSN(szElemsFromDB[j])*X;
			_XF2[nPGas+i]+=fT[j]*Y;
			fT[j]=0;
			X=X*100;
			Y=Y*10;
		}
		_XF1[nPGas+i] = -_XF1[nPGas+i];
	}
	return TRUE;
}

void JobWithDB::GetAllchem_data(int &ext_nPElementaryElem, 
		int &ext_nPGas,chem_dataGas ext_cdG[MaxElemGasFromDB], 
		int &ext_nPCond,chem_dataCond ext_cdC[MaxElemCondFromDB])
{
	if (ext_cdG)
		for (int i=0;i<=nPGas;i++)
			ext_cdG[i] = cdG[i];

	if (ext_cdC)
		for (int i=0;i<=nPCond;i++)
			ext_cdC[i] = cdC[i];

	ext_nPElementaryElem = nPElementaryElem;
	ext_nPGas = nPGas;
	ext_nPCond = nPCond;
}

void JobWithDB::GetNamesP(char ext_szNam[MaxElemGasFromDB][MAX_len], unsigned int &ext_nMaxNamesLen) {
	ext_nMaxNamesLen = nMaxNamesLen;
	if (_szNam) {

		int cur_elem;
		for (cur_elem=1;cur_elem<=nPGas;cur_elem++) {
			strcpy(ext_szNam[cur_elem],cdG[cur_elem-1].view);
		}
		for (cur_elem=1;cur_elem<=nPCond;cur_elem++) {
			strcpy(ext_szNam[nPGas+cur_elem],szCondSymbol);
			strcpy(&ext_szNam[nPGas+cur_elem][2],cdC[cur_elem-1].view);
		}
		for (cur_elem=1;cur_elem<=nPAll;cur_elem++) {
			unsigned int s1 = strlen(ext_szNam[cur_elem]);
			ext_szNam[cur_elem][s1] = ':'; 
			s1++;
			for (s1;s1<nMaxNamesLen+1;s1++)
				ext_szNam[cur_elem][s1] = ' ';
			ext_szNam[cur_elem][s1] = 0; 
		}
	}
}


Report::Report(
	int _nP,
	int _nAlfa,
	int _nParam,
	UXFSTRUCT _rep_uxf[MaxQuantityFIHBox],
	float _pfAllAlfa[MaxQuantityFIHBox],
	float _pfKappaStroke0[MaxQuantityFIHBox],
	float _pfAllParam[MaxQuantityFIHBox],
	float _pfAllP[MaxQuantityFIHBox],
	float _fAllProcessP[MaxQuantityFIHBox][MaxQuantityFIHBox][MaxQuantityFIHBox],
	float _pfAllT[MaxQuantityFIHBox][MaxQuantityFIHBox][MaxQuantityFIHBox],
	float _pfAllI[MaxQuantityFIHBox][MaxQuantityFIHBox][MaxQuantityFIHBox],
	float _pfAllS[MaxQuantityFIHBox][MaxQuantityFIHBox][MaxQuantityFIHBox],
	float _pfAllMS[MaxQuantityFIHBox][MaxQuantityFIHBox][MaxQuantityFIHBox],
	float _pfAllCP3[MaxQuantityFIHBox][MaxQuantityFIHBox][MaxQuantityFIHBox],
	float _pfAllK3[MaxQuantityFIHBox][MaxQuantityFIHBox][MaxQuantityFIHBox],
	float _pfAllB[MaxQuantityFIHBox][MaxQuantityFIHBox][MaxQuantityFIHBox],
	float _pfAllPYD[MaxQuantityFIHBox][MaxQuantityFIHBox][MaxQuantityFIHBox],
	float _pfAllPYDP[MaxQuantityFIHBox][MaxQuantityFIHBox][MaxQuantityFIHBox],
	float _pfAllF[MaxQuantityFIHBox][MaxQuantityFIHBox][MaxQuantityFIHBox],
	float _pfAllF_div_FKP[MaxQuantityFIHBox][MaxQuantityFIHBox][MaxQuantityFIHBox],
	float _pfAllZK[MaxQuantityFIHBox][MaxQuantityFIHBox][MaxQuantityFIHBox]
	)
{
	rep_nP = _nP;
	rep_nAlfa = _nAlfa;
	rep_nParam = _nParam;
	for (int i3=0;i3<rep_nAlfa;i3++)
		rep_uxf[i3] = _rep_uxf[i3];
	pfAllAlfa = _pfAllAlfa;
	pfKappaStroke0 = _pfKappaStroke0;
	pfAllParam = _pfAllParam;
	pfAllP = _pfAllP;
	pfAllProcessP = _fAllProcessP;
	pfAllT = _pfAllT;
	pfAllI = _pfAllI;
	pfAllS = _pfAllS;
	pfAllMS = _pfAllMS;
	pfAllCP3 = _pfAllCP3;
	pfAllK3 = _pfAllK3;
	pfAllB = _pfAllB;
	pfAllPYD = _pfAllPYD;
	pfAllPYDP = _pfAllPYDP;
	pfAllF = _pfAllF;
	pfAllF_div_FKP = _pfAllF_div_FKP;
	pfAllZK = _pfAllZK;
	bPa = FALSE;
	bPEN = FALSE;
	bIS = FALSE;
	bV = FALSE;
	bEPS = FALSE;
	memset(&rep_os,0,sizeof(rep_os));	// Должно значить опции по умолчанию
	nCurrUXF = 0;
	nCurrP = 0;
	strReport.Empty();
// ПГ
	memset(&apc,0,sizeof(apc));
	bWasInitAllPCSTRUCT = FALSE;
	precision = 8; // точность значений для ПС
}

BOOL Report::PrepareToFirstCalc() {	nCurrUXF = 0;	nCurrP = 0;	return TRUE;}

BOOL Report::NextAlfa() {	// и установка начального давления в КС
	if ( (nCurrUXF+1)>=rep_nAlfa )
		return FALSE;
	nCurrUXF++;
	nCurrP = 0;
	return TRUE;
}

BOOL Report::NextPressure() {
	if ( (nCurrP+1)>=rep_nP )
		return FALSE;
	nCurrP++;
	return TRUE;
}

void Report::ClearReportStr() {strReport.Empty();}

void Report::SetOptionsReport(OPTIONSTRUCT &ext_os) {	rep_os = ext_os;}

void Report::SetWayWrite(unsigned int uiWayWrite) {
	if (uiWayWrite==showPa) {
		bPa = TRUE;
	}
	else 
		if (uiWayWrite==showPEN) {
			bPEN = TRUE;
		}
		else 
			if (uiWayWrite==showIS) {
				bIS = TRUE;
			}
			else 
				if (uiWayWrite==showV) {
					bV = TRUE;
				}
				else 
					if (uiWayWrite==showEPS) {
						bEPS = TRUE;
					}
}

BOOL Report::SaveSomePC(int nP, int nAlfa, int nParam, ProductsCombustion &PC) {
	if ( (nP>MaxQuantityFIHBox) && (nAlfa>MaxQuantityFIHBox) ) 
		return FALSE;
	if (!bWasInitAllPCSTRUCT) {
		// Нужно ли записывать имена?
		for (int n=0;n<MaxElemGasFromDB;n++)
			strcpy(apc.szNam[n], PC.szNam[n]);
		apc.nMaxNamesLen = PC.nMaxNamesLen;
		apc.nPElementaryElem = PC.nPElementaryElem;
		apc.nPGas = PC.nPGas;
		apc.nPCond = PC.nPCond;
		apc.nPAll = PC.nPAll;
		bWasInitAllPCSTRUCT = TRUE;
	}
	for (int i=0;i<MaxElemGasFromDB;i++)
		apc.P[nP][nAlfa][nParam][i] = PC.P[i];
	return TRUE;
}

CString Report::GetReportA() {	return strReport;}

int Report::WriteAllCalcsInStr() {
	int nCommonLen = 0;
	if ( (!bPa)&&(!bPEN)&&(!bIS)&&(!bV)&&(!bEPS) )//если расчет без F, Pa, IS диагр. то один цикл для Ркам
		rep_nParam = 1;

	int n_save_CurrUXF = nCurrUXF;
	for (nCurrUXF = 0;nCurrUXF<rep_nAlfa;nCurrUXF++) {
		nCommonLen += WriteCurrentAlfaCalcsInStr();
	}
	nCurrUXF = n_save_CurrUXF;
	return nCommonLen;
}

int Report::WriteCurrentAlfaCalcsInStr() {
	int nCommonLen = 0;
	char szOut[MAX_len];
	int	precision = 8;

	strReport += tab;
	wsprintf(szOut,"Альфа%u = ",nCurrUXF+1);
	strReport += szOut;
	str1.Empty();
	ToDotString(pfAllAlfa[nCurrUXF], str1);
	strReport += str1;
	strReport += ";";
	strReport += cf ;

	strReport += tab;
	wsprintf(szOut,"k%u = ",nCurrUXF+1);	// или действительное стехиометрическое соотношение или что-то еще
	strReport += szOut;
	str1.Empty();
	ToDotString(pfAllAlfa[nCurrUXF]*pfKappaStroke0[nCurrUXF], str1);
	strReport += str1;
	strReport += ";";
	strReport += cf ;

	strReport += tab;
	strReport += "УХФ: ";
	str1.Empty();
	for (int i2=0;i2<rep_uxf[nCurrUXF].nQuantityElemF;i2++) {
		str1 += rep_uxf[nCurrUXF].szAtomsOfF[i2];
		ToDotString(rep_uxf[nCurrUXF].fConditChimFormF[i2], str1);
	}
	strReport += str1;
	strReport += ";";
	strReport += cf;

	// Опция вывода единиц измерения
	BOOL bIncludeUMO = rep_os.bIncludeUnitMeasOption;
	strReport += tab;
	strReport += "энтальпия УХФ: ";
	str1.Empty();
	ToDotString(rep_uxf[nCurrUXF].fEnthalpyF, str1);
	strReport += str1;
	if (bIncludeUMO) {
		strReport += " ";
		strReport += szReport_unitI;
	}
	strReport += ";";
	strReport += cf;
	strReport += cf;
	if(!rep_os.hdcOutput) {
		return NULL;
	}

	// Создать контекст устройства клиентской области окна (NULL в констр.-окно рабочего стола)
	HDC hdc = rep_os.hdcOutput;// old
	CClientDC cdc(NULL);
	hdc = cdc.m_hDC;
	// и присоединить к нему шрифт
	LOGFONT lf;
	memset(&lf,0,sizeof(lf));
	lf.lfHeight = -::MulDiv(cnFontSizeRichEditCtrlChem, GetDeviceCaps(hdc,LOGPIXELSY), 72);
	lf.lfWeight = FW_BOLD;	//700;
	lf.lfCharSet = RUSSIAN_CHARSET;
	lf.lfOutPrecision = 3;
	lf.lfClipPrecision = 2;
	lf.lfQuality = 1;
	lf.lfPitchAndFamily = VARIABLE_PITCH|FF_SWISS;
	strcpy(lf.lfFaceName,szDefFontFaceName);
	CFont font;
	font.CreateFontIndirect(&lf);
	cdc.SelectObject(&font);
	char szFaceName[MAX_len];	//debug
	BOOL rv = GetTextFace(hdc,MAX_len-1,szFaceName);	//debug

	int nStartTab = 2;// Должна быть < (MAX_len-1)
	SIZE sT;
	//Подсчет максимальной длины надписи характеристики процесса
	GetTextExtentPoint32(hdc,szReport_T,strlen(szReport_T),&sT);
	int nLenMax = sT.cx;
	GetTextExtentPoint32(hdc,szReport_I,strlen(szReport_I),&sT);
	int nLen = sT.cx;
	if(nLen>nLenMax)
		nLenMax = nLen;
	GetTextExtentPoint32(hdc,szReport_S,strlen(szReport_S),&sT);
	nLen = sT.cx;
	if(nLen>nLenMax)
		nLenMax = nLen;
	GetTextExtentPoint32(hdc,szReport_P,strlen(szReport_P),&sT);
	nLen = sT.cx;
	if(nLen>nLenMax)
		nLenMax = nLen;
	GetTextExtentPoint32(hdc,szReport_MS,strlen(szReport_MS),&sT);
	nLen = sT.cx;
	if(nLen>nLenMax)
		nLenMax = nLen;
	GetTextExtentPoint32(hdc,szReport_CP3,strlen(szReport_CP3),&sT);
	nLen = sT.cx;
	if(nLen>nLenMax)
		nLenMax = nLen;
	GetTextExtentPoint32(hdc,szReport_K3,strlen(szReport_K3),&sT);
	nLen = sT.cx;
	if(nLen>nLenMax)
		nLenMax = nLen;
	GetTextExtentPoint32(hdc,szReport_B,strlen(szReport_B),&sT);
	nLen = sT.cx;
	if(nLen>nLenMax)
		nLenMax = nLen;
	GetTextExtentPoint32(hdc,szReport_PYD,strlen(szReport_PYD),&sT);
	nLen = sT.cx;
	if(nLen>nLenMax)
		nLenMax = nLen;
	GetTextExtentPoint32(hdc,szReport_PYDP,strlen(szReport_PYDP),&sT);
	nLen = sT.cx;
	if(nLen>nLenMax)
		nLenMax = nLen;
	GetTextExtentPoint32(hdc,szReport_F,strlen(szReport_F),&sT);
	nLen = sT.cx;
	if(nLen>nLenMax)
		nLenMax = nLen;
	GetTextExtentPoint32(hdc,szReport_F_div_FKP,strlen(szReport_F_div_FKP),&sT);
	nLen = sT.cx;
	if(nLen>nLenMax)
		nLenMax = nLen;
	GetTextExtentPoint32(hdc,szReport_ZK,strlen(szReport_ZK),&sT);
	nLen = sT.cx;
	if(nLen>nLenMax)
		nLenMax = nLen;

	// Длина знака равенства
	char szEqu[] = " = ";
	GetTextExtentPoint32(hdc,szEqu,strlen(szEqu),&sT);
	nLenMax += sT.cx;

	//Подсчет длины значения характеристики процесса
	char szValue[MAX_len];
	memset(szValue,0,sizeof(szValue));
	int z;
	for(z=0;z<constToDotString;z++)
		szValue[z] = '0';
	szValue[z] = '.';
	z++;
	for(z=constToDotString+1;z<5+constToDotString+1;z++)
		szValue[z] = '0';
	//Подсчет длины значов характеристики процесса
	szValue[z] = '-';
	z++;
	szValue[z] = ';';
	z++;
	szValue[z] = ' ';
	z++;
	GetTextExtentPoint32(hdc,szValue,strlen(szValue),&sT);
	int nLenVal = sT.cx;

	int nLenUnitDimens = 0;
	if (bIncludeUMO) {
		//Подсчет максимальной длины надписи единиц измерения характеристики процесса
		GetTextExtentPoint32(hdc,szReport_unitF,strlen(szReport_unitF),&sT);
		nLenUnitDimens = sT.cx;
	}

// Научиться правильно переводить структуру LOGFONT в CHARFORMAT и обратно.
// Подсчитать длинну пробела черес класс CDC с присвоенным шрифтом созданным через LOGFONT,
// длинну tab установить в RichEditCtrl по умолчанию (узнать в каких единицах она имеряется).
// Узнать сколько space`ов помещаются в один tab и потом высчитывать длинну tab`а по space`ам.

	// Заполним массив табов постоянной длинны, в ячейке длинна таба в пикселах
	int rgxTabs[MAX_TAB_STOPS]; 
	const float cfTwipsPerInch = 1440;
	int nPixelsPerInch = GetDeviceCaps(hdc,LOGPIXELSX);
	float fConvertTwipsToPixels = (lDefaultTab / cfTwipsPerInch) * nPixelsPerInch;
	for(int k=0;k<MAX_TAB_STOPS;k++)
		rgxTabs[k] = fConvertTwipsToPixels*(k+1);	//lDefaultTab/3 = 4 пробела в табе
	// Подогнать количество табов к максимальной длинне строки
	float fTuneTab = nLenMax+nLenVal+nLenUnitDimens;//Длина сначала в пробелах потом в табах(на этапе отладки предположим что в табе 12 пробелов)
	CString strTabs;
	strTabs += tab;
	int nCountTabStops = 1;
	WORD w = LOWORD(GetTabbedTextExtent(hdc, (LPCTSTR)strTabs, strlen((LPCTSTR)strTabs), 1, rgxTabs));
	while(fTuneTab>w)
	{
		nCountTabStops++;	
		if(nCountTabStops>MAX_TAB_STOPS) break;
		strTabs += tab;
		w = LOWORD(GetTabbedTextExtent(hdc, (LPCTSTR)strTabs, strlen((LPCTSTR)strTabs), 1, rgxTabs));
	}
	float fTabLen = w;
	int nLenCurr = 0;
	float fA = 0;
	int nA = 0;
	int sss = sizeof(CThermoCalcView);	//debug
	sss += sizeof(CThermoCalcDoc);	//debug

	// Строка задания точности, для фунции форматирования
	char szPrecision[MAX_len];
	if (bIncludeUMO)
		wsprintf(szPrecision," = %%.%if %%s;",constToDotString); // Если нужно выводить единицы измерения
	else
		wsprintf(szPrecision," = %%.%if ;",constToDotString);

	// Строка задания точности, для фунции форматирования
	char szMinPrecision[MAX_len];
	if (bIncludeUMO)
		wsprintf(szMinPrecision,"%%.%if %%s;",constToDotString); // Если нужно выводить единицы измерения
	else
		wsprintf(szMinPrecision,"%%.%if ;",constToDotString);

	int nElseSpace = 0;
	if ( (bPa)||(bPEN)||(bIS)||(bV)||(bEPS) )	// если расчет с F, Pa, IS диагр. то учесть их длинну
		nElseSpace = fTabLen*(rep_nParam-1);
	else
		nElseSpace = 0;
	int j;
	int ip;
	int ipar;

	// Начать вывод - за давление
	for(j=0;j<nStartTab;j++)
		strReport += tab;
	for (ip =0;ip<rep_nP;ip++) {
/*		wsprintf(szOut,"P кс%u",ip+1);
		str1 = szOut;
		int o = 1;
		WORD w = LOWORD(GetTabbedTextExtent(hdc, (LPCTSTR)str1, strlen((LPCTSTR)str1), 1, rgxTabs));
		while(fTabLen+nElseSpace>w)
		{
			o++;
			if(o>MAX_TAB_STOPS) break;
			str1 += tab;
			w = LOWORD(GetTabbedTextExtent(hdc, (LPCTSTR)str1, strlen((LPCTSTR)str1), 1, rgxTabs));
		}*/
		str1.Format("P кс%u", ip+1);
		for (j = 0;j<MAX_TAB_STOPS;j++) {
			if (fTabLen+nElseSpace<=LOWORD(GetTabbedTextExtent(hdc, (LPCTSTR)str1, str1.GetLength(), 1, rgxTabs)))
				break;
			str1 += tab;
		}
		strReport += str1;
	}
	strReport += cf;

	for(j=0;j<nStartTab;j++)
		strReport += tab;
	for (ip =0;ip<rep_nP;ip++) {
		str1.Format(szPrecision, pfAllP[ip]*1.03323, szReport_unitP);
		for (j = 0;j<MAX_TAB_STOPS;j++) {
			if (fTabLen+nElseSpace<=LOWORD(GetTabbedTextExtent(hdc, (LPCTSTR)str1, str1.GetLength(), 1, rgxTabs)))
				break;
			str1 += tab;
		}
		strReport += str1;
	}
	strReport += cf;

	if ( (bPa)||(bPEN)||(bIS)||(bV)||(bEPS) ) {	// если расчет с F, Pa, IS диагр.
		if (bPa) {
			for(j=0;j<nStartTab-1;j++)
				strReport += tab;
			for (ip =0;ip<rep_nP;ip++) {
				for (ipar =0;ipar<rep_nParam;ipar++) {
					wsprintf(szOut,"Для давления%u в сеч.%s", ipar+1, szPrecision);
					str1.Format(szOut, pfAllParam[ipar]*1.03323, "МПа"); 
					MACROCyleRegulateTab(str1);
					strReport += str1;
				}
			}
			strReport += cf ;
		}
		else 
			if (bPEN) {
				for(j=0;j<nStartTab-1;j++)
					strReport += tab;
				for (ip =0;ip<rep_nP;ip++) {
					for (ipar =0;ipar<rep_nParam;ipar++) {
/*						str1.Format("Для степени%u расш. сопла",ipar+1);
						for (j = 0;j<MAX_TAB_STOPS;j++) {
							if (fTabLen<=LOWORD(GetTabbedTextExtent(hdc, (LPCTSTR)str1, str1.GetLength(), 1, rgxTabs)))
								break;
							str1 += tab;
						}
						strReport += str1;*/
						wsprintf(szOut,"Для степени%u расш. сопла%s", ipar+1, szPrecision);
						str1.Format(szOut, pfAllParam[ipar], " "); 
						MACROCyleRegulateTab(str1);
						strReport += str1;
					}
				}
				strReport += cf ;
			}
			else 
				if (bIS) {
					for(j=0;j<nStartTab-1;j++)
						strReport += tab;
					for (ip =0;ip<rep_nP;ip++) {
						for (ipar =0;ipar<rep_nParam;ipar++) {
/*
							str1.Empty();
							str1.Format("Для температуры%u IS-диаграммы",ipar+1);
							ToDotString(pfAllParam[ipar], str1);
							int o = 1;
							WORD w = LOWORD(GetTabbedTextExtent(hdc, (LPCTSTR)str1, strlen((LPCTSTR)str1), 1, rgxTabs));
							while(fTabLen>w)
							{
								o++;
								if(o>MAX_TAB_STOPS) break;
								str1 += tab;
								w = LOWORD(GetTabbedTextExtent(hdc, (LPCTSTR)str1, strlen((LPCTSTR)str1), 1, rgxTabs));
							}
*/
							str1.Format("Для температуры%u IS-диаграммы",ipar+1);
							for (j = 0;j<MAX_TAB_STOPS;j++) {
								if (fTabLen<=LOWORD(GetTabbedTextExtent(hdc, (LPCTSTR)str1, str1.GetLength(), 1, rgxTabs)))
									break;
								str1 += tab;
							}
							strReport += str1;
						}
					}
					strReport += cf ;
				}
				else 
					if (bV) {
						for(j=0;j<nStartTab-1;j++)
							strReport += tab;
						for (ip =0;ip<rep_nP;ip++) {
							for (ipar =0;ipar<rep_nParam;ipar++) {

/*
								str1.Empty();
								str1.Format("Для горения при постоянном объеме%u.",ipar+1);
								ToDotString(pfAllParam[ipar]*1.03323, str1);
								int o = 1;
								WORD w = LOWORD(GetTabbedTextExtent(hdc, (LPCTSTR)str1, strlen((LPCTSTR)str1), 1, rgxTabs));
								while(fTabLen>w)
								{
									o++;
									if(o>MAX_TAB_STOPS) break;
									str1 += tab;
									w = LOWORD(GetTabbedTextExtent(hdc, (LPCTSTR)str1, strlen((LPCTSTR)str1), 1, rgxTabs));
								}
*/
								str1.Format("Для горения при постоянном объеме%u.",ipar+1);
								for (j = 0;j<MAX_TAB_STOPS;j++) {
									if (fTabLen<=LOWORD(GetTabbedTextExtent(hdc, (LPCTSTR)str1, str1.GetLength(), 1, rgxTabs)))
										break;
									str1 += tab;
								}
								strReport += str1;
							}
						}
						strReport += cf ;
					}
	}
	else {
		for(j=0;j<nStartTab-1;j++)
			strReport += tab;
		strReport += "Параметры процесса";
		strReport += cf ;
	}

/* // Вывод этой строки нужно обдумать1
	for(j=0;j<nStartTab;j++)
		strReport += tab;
	for (ip =0;ip<rep_nP;ip++) {
		str1.Empty();
		str1 += "Давление в КС = ";
		ToDotString(pfAllP[ip]*1.03323, str1);		//_gcvt();- Но она вставляет экспоненту если превышает сотые доли
		str1 += " К;";
		int o = 1;
		WORD w = LOWORD(GetTabbedTextExtent(hdc, (LPCTSTR)str1, strlen((LPCTSTR)str1), 1, rgxTabs));
		while(fTabLen>w)
		{
			o++;
			if(o>MAX_TAB_STOPS) break;
			str1 += tab;
			w = LOWORD(GetTabbedTextExtent(hdc, (LPCTSTR)str1, strlen((LPCTSTR)str1), 1, rgxTabs));
		}
		strReport += str1;
	}
	strReport += cf;
*/
	// Опция минимума названий
	BOOL bMinNames = rep_os.bMinNamesInOutput;

// Вывести в одну строку одну характеристику о процессе для всех Ркс и параметре (F, Pa, IS диагр.)
	for(j=0;j<nStartTab;j++)
		strReport += tab;
	for (ip =0;ip<rep_nP;ip++) {
		for (ipar =0;ipar<rep_nParam;ipar++) {
			if (bMinNames&&((ip!=0)||(ipar!=0))) {			// Если стоит опция и это не самый левый столбец
				str1.Format(szMinPrecision, pfAllT[ip][nCurrUXF][ipar]); 
				MACROCyleRegulateTab(str1);
				strReport += str1;
			}
			else {
				MACROFormatToReport(szReport_T, pfAllT[ip][nCurrUXF][ipar], szReport_unitT);
			}
		}
	}
	strReport += cf;

	MACROFormatReport(szReport_I, pfAllI, szReport_unitI);

	MACROFormatReport(szReport_S, pfAllS, szReport_unitS);

 // Вывод этой строки нужно обдумать2
	for(j=0;j<nStartTab;j++)
		strReport += tab;
	for (ip =0;ip<rep_nP;ip++) {
		for (ipar =0;ipar<rep_nParam;ipar++) {
			if (pfAllParam) {
				if (bMinNames&&((ip!=0)||(ipar!=0))) {			// Если стоит опция и это не самый левый столбец
					str1.Format(szMinPrecision, pfAllParam[ipar]*1.03323); 
					MACROCyleRegulateTab(str1);
					strReport += str1;
				}
				else {
					MACROFormatToReport("Давление в сеч.", pfAllParam[ipar]*1.03323, szReport_unitP);
				}
			}
			else {
				if (bMinNames&&((ip!=0)||(ipar!=0))) {			// Если стоит опция и это не самый левый столбец
					str1.Format(szMinPrecision, pfAllP[ipar]*1.03323); 
					MACROCyleRegulateTab(str1);
					strReport += str1;
				}
				else {
					MACROFormatToReport("Давление процесса", pfAllP[ipar]*1.03323, szReport_unitP);
				}
			}
/*			if (pfAllParam) {
				MACROFormatToReport("Давление в сеч.= ", pfAllProcessP[ip][nCurrUXF][ipar]*1.03323, szReport_unitP);
			}
			else {
				MACROFormatToReport("Давление процесса = ", pfAllProcessP[ip][nCurrUXF][ipar]*1.03323, szReport_unitP);
			}//*/
		}
	}
	strReport += cf;

	MACROFormatReport(szReport_MS, pfAllMS, szReport_unitMS);

	MACROFormatReport(szReport_CP3, pfAllCP3, szReport_unitCP3);

	if (pfAllK3) {
		MACROFormatReport(szReport_K3, pfAllK3, szReport_unitK3);
	}

	if (pfAllB) {
		MACROFormatReport(szReport_B, pfAllB, szReport_unitB);
	}

	if (pfAllPYD) {
		MACROFormatReport(szReport_PYD, pfAllPYD, szReport_unitPYD);
	}

	if (pfAllPYDP) {
		MACROFormatReport(szReport_PYDP, pfAllPYDP, szReport_unitPYDP);
	}

	if (pfAllF) {
		MACROFormatReport(szReport_F, pfAllF, szReport_unitF);
	}

	if (pfAllF_div_FKP) {
		MACROFormatReport(szReport_F_div_FKP, pfAllF_div_FKP, szReport_unitF_div_FKP);
	}

	MACROFormatReport(szReport_ZK, pfAllZK, szReport_unitZK);

	for (ip =0;ip<rep_nP;ip++) {
		for (ipar =0;ipar<rep_nParam;ipar++) {
			// Формирование текстового отчета с результатами расчета ПГ
			str1.Empty();
			ToDotString(pfAllAlfa[nCurrUXF], str1);
			str2.Empty();
			ToDotString(pfAllP[ip]*1.03323, str2);

			if ( (bPa)||(bPEN)||(bIS)||(bV)||(bEPS) )	// если расчет с F, Pa, IS диагр.
				strHeader.Format("\r\n\t\tСостав ПГ при Альфа%u=%s, Pкс%u=%s, параметр%u\r\n",
						nCurrUXF+1,(LPCTSTR)str1,nCurrP+1,(LPCTSTR)str2, ipar+1);
			else
				strHeader.Format("\r\n\t\tСостав ПГ при Альфа%u=%s, Pкс%u=%s\r\n",
						nCurrUXF+1,(LPCTSTR)str1,nCurrP+1,(LPCTSTR)str2);
			strReport += strHeader;
			WriteToReportOnePCA(ip, nCurrUXF, ipar);
			strReport += cf;
		}
	}
	return nCommonLen;
}

BOOL Report::WriteAllCalcsInWS(_Worksheet &ws,Range &rngLeftTopCorner,Range *prngSquare) {

	Range rng1;
	Range rng2;
	str1 = rngLeftTopCorner.GetAddress();
	if(str1.Find(':')!=-1) {
		return FALSE;			// rngLeftTopCorner должен указывать на одну ячейку
	}
	int nCurrStr = 0;
	int nCurrStrParam = 0;
	int nCurrStrP = 0;
	int nCurrStrUXF = 0;
	if ( (!bPa)&&(!bPEN)&&(!bIS)&&(!bV)&&(!bEPS) )//если расчет без F, Pa, IS диагр. то один цикл для Ркам
		rep_nParam = 1;

	// Опция вывода единиц измерения
	int nLenDescrProc;
	BOOL bInclUMO = rep_os.bIncludeUnitMeasOption;
	if (bInclUMO)
		nLenDescrProc = cnColWithUnitMeasOption;
	else
		nLenDescrProc = cnColWithoutUnitMeasOption;

	// Опция минимума названий
	BOOL bMinNames = rep_os.bMinNamesInOutput;//писать названия перед началом раздела расчета

	nCurrStrUXF = nCurrStr;			// Сохраним номер строки с альфа
	// Цикл по альфа
	for (int nCurrUXF = 0;nCurrUXF<rep_nAlfa;nCurrUXF++) {
		nCurrStr = nCurrStrUXF;		// Начнем со строки где выводилась первая альфа


		if (bInclUMO)
			nLenDescrProc = cnColWithUnitMeasOption;
		else
			nLenDescrProc = cnColWithoutUnitMeasOption;

		int nColumnHeadAlfa = nCurrUXF*rep_nP*rep_nParam*nLenDescrProc;
		if (bMinNames/*&&(nCurrUXF!=0)*/)	// Минимум названий величин
			nColumnHeadAlfa = nCurrUXF*(rep_nP*rep_nParam)*cnColWithMinNames+(nCurrUXF?(nLenDescrProc-cnColWithMinNames):0);

		MACROSetFormatnValue(nCurrStr,nColumnHeadAlfa,str1,"Альфа%u",nCurrUXF+1);
		MACROSetfValue(nCurrStr,nColumnHeadAlfa+1,str1,pfAllAlfa[nCurrUXF]);

		nCurrStr ++;
		str1 = "УХФ: ";
		for (int i2=0;i2<rep_uxf[nCurrUXF].nQuantityElemF;i2++) {
			str1 += rep_uxf[nCurrUXF].szAtomsOfF[i2];
			ToDotString(rep_uxf[nCurrUXF].fConditChimFormF[i2], str1);
		}
		MACROSetStrValue(nCurrStr,nColumnHeadAlfa,str1);
		// Объединить столбцы, что бы уместилась УХФ
		MACROMergeRightToLeft(nCurrStr,nColumnHeadAlfa, 1+cnIndentReports, str1, str2);

		nCurrStr ++;
		MACROSetStrValue(nCurrStr,nColumnHeadAlfa,"Энтальпия УХФ");
		MACROSetfValue(nCurrStr,nColumnHeadAlfa+1,str1,rep_uxf[nCurrUXF].fEnthalpyF);
		if (bInclUMO)
			MACROSetStrValue(nCurrStr,nColumnHeadAlfa+2,szReport_unitI);

		nCurrStr ++;
		nCurrStr ++;
		nCurrStrP = nCurrStr;		// Сохраним номер строки с давлением
		for (int _nCurrP =0;_nCurrP<rep_nP;_nCurrP++) {
			// Заголовок для давления
			nCurrStr = nCurrStrP;	// Начнем со строки где выводилось первое давление

			if (bInclUMO)
				nLenDescrProc = cnColWithUnitMeasOption;
			else
				nLenDescrProc = cnColWithoutUnitMeasOption;
			int nColumnHeadP = nCurrUXF*rep_nP*rep_nParam*nLenDescrProc + _nCurrP*rep_nParam*nLenDescrProc;
//*			
			if (bMinNames) {				// Минимум названий величин
				//nLenDescrProc = cnColWithMinNames;
				//nColumnHeadP = nCurrUXF*rep_nP*rep_nParam*nLenDescrProc + _nCurrP*rep_nParam*nLenDescrProc;
/*				nColumnHeadP = nCurrUXF*((rep_nP*rep_nParam)-1)*cnColWithMinNames
						+ _nCurrP*rep_nParam*cnColWithMinNames + nCurrUXF*nLenDescrProc
						+ (_nCurrP?(nLenDescrProc-cnColWithMinNames):0)
						;
*/

		nColumnHeadP=nCurrUXF*(rep_nP*rep_nParam)*cnColWithMinNames+nCurrUXF*(nLenDescrProc-cnColWithMinNames)
			// Выравнивание по УХФ

				+_nCurrP*rep_nParam*cnColWithMinNames+(_nCurrP!=0&&nCurrUXF==0?(nLenDescrProc-cnColWithMinNames):0)
			// Выравнивание по текущему давлению
				;
			}

			if (bInclUMO) {			// вывести "[МПа]"
				str1.Format("P кс%u [%s]",_nCurrP+1,szReport_unitP);
				MACROSetStrValue(nCurrStr-1,nColumnHeadP,str1);
			}
			else
				MACROSetFormatnValue(nCurrStr-1,nColumnHeadP,str1,"P кс%u",_nCurrP+1);
			MACROSetfValue(nCurrStr,nColumnHeadP,str1,pfAllP[_nCurrP]*1.03323);

			// Объединить столбцы первой строки, до всех параметров расчета для данного давления
			if (bMinNames) {
				MACROMergeRightToLeft(nCurrStr-1,nColumnHeadP, 
						((_nCurrP==0&&nCurrUXF==0)?((rep_nParam-1)*cnColWithMinNames)+nLenDescrProc-1:(rep_nParam*cnColWithMinNames)-1), str1, str2);
			}
			else
				MACROMergeRightToLeft(nCurrStr-1,nColumnHeadP, rep_nParam*nLenDescrProc-1, str1, str2);
			// Объединить столбцы второй строки
			if (bMinNames) {
				MACROMergeRightToLeft(nCurrStr,nColumnHeadP, 
						((_nCurrP==0&&nCurrUXF==0)?((rep_nParam-1)*cnColWithMinNames)+nLenDescrProc-1:(rep_nParam*cnColWithMinNames)-1), str1, str2);
			}
			else
				MACROMergeRightToLeft(nCurrStr,nColumnHeadP, rep_nParam*nLenDescrProc-1, str1, str2);

			// "Параметры процесса" и
			// расчет с F, Pa, IS диагр.
			nCurrStrParam = nCurrStr;
			for (int nCurrParam=0;nCurrParam<rep_nParam;nCurrParam++) {
				nCurrStr = nCurrStrParam;
				nCurrStr ++;
				nCurrStr ++;

				if (bInclUMO)
					nLenDescrProc = cnColWithUnitMeasOption;
				else
					nLenDescrProc = cnColWithoutUnitMeasOption;

				int nColumnDescrProcN = nCurrUXF*rep_nP*rep_nParam*nLenDescrProc + 
						_nCurrP*rep_nParam*nLenDescrProc+nCurrParam*nLenDescrProc;
				int nColumnDescrProcV = nCurrUXF*rep_nP*rep_nParam*nLenDescrProc + 
						_nCurrP*rep_nParam*nLenDescrProc+nCurrParam*nLenDescrProc+1;
				int nColumnDescrProcUM = nCurrUXF*rep_nP*rep_nParam*nLenDescrProc + 
						_nCurrP*rep_nParam*nLenDescrProc+nCurrParam*nLenDescrProc+2;

				if (bMinNames) {				// Минимум названий величин
/*					nColumnDescrProcN = nCurrUXF*((rep_nP*rep_nParam)-1)*cnColWithMinNames
							+ _nCurrP*rep_nParam*cnColWithMinNames + nCurrUXF*nLenDescrProc
							+ nCurrParam*cnColWithMinNames
							+ (nCurrParam?(nLenDescrProc-cnColWithMinNames):0)
							;
*/

		nColumnDescrProcV=nCurrUXF*(rep_nP*rep_nParam)*cnColWithMinNames+nCurrUXF*(nLenDescrProc-cnColWithMinNames)
			// Выравнивание по УХФ

				+_nCurrP*rep_nParam*cnColWithMinNames+(_nCurrP!=0&&nCurrUXF==0?(nLenDescrProc-cnColWithMinNames):0)
			// Выравнивание по текущему давлению

				+nCurrParam*cnColWithMinNames+(nCurrParam!=0&&_nCurrP==0&&nCurrUXF==0?(nLenDescrProc-cnColWithMinNames):0)
			// Выравнивание по текущему параметру
			;

					nColumnDescrProcN = nColumnDescrProcV;
					if ((nCurrUXF==0)&&(_nCurrP==0)&&(nCurrParam==0))
						nColumnDescrProcV++;
				}

/*
				if ((bMinNames)&&(!((_nCurrP==0)&&(nCurrParam==0)))) {// Если стоит опция и это не самый левый столбец
					nLenDescrProc = cnColWithMinNames;
					nColumnDescrProcV = nCurrUXF*rep_nP*rep_nParam*nLenDescrProc + 
							_nCurrP*rep_nParam*nLenDescrProc+nCurrParam*nLenDescrProc+
							cnColWithMinNames-2;
					if (bInclUMO)
						nColumnDescrProcV += cnColWithUnitMeasOption;
					else
						nColumnDescrProcV += cnColWithoutUnitMeasOption;
					nColumnDescrProcN = nColumnDescrProcV;
				}
*/
				if ( (bPa)||(bPEN)||(bIS)||(bV)||(bEPS) ) {	// если расчет с F, Pa, IS диагр.
					if (bPa) {
/*						MACROGetOffset(long(nCurrStr-1),((long)nCurrUXF*rep_nP*rep_nParam*2 + _nCurrP*rep_nParam*2+nCurrParam*2));
						str1.Empty();
						str1.Format("Для давления%u в сечении",nCurrParam+1);
						rngThroughOffset.SetValue2(COleVariant(str1));
						MACROClearOffset();*/
						MACROSetFormatnValue(nCurrStr-1,nColumnDescrProcN,str1,"Для давления%u в сечении",nCurrParam+1);
/*						MACROGetOffset(nCurrStr,((long)nCurrUXF*rep_nP*rep_nParam*2 + _nCurrP*rep_nParam*2+nCurrParam*2));
						str1.Empty();
						ToDotString(pfAllParam[nCurrParam]*1.03323, str1);
						rngThroughOffset.SetValue2(COleVariant(str1));
						MACROClearOffset();*/
						MACROSetfValue(nCurrStr,nColumnDescrProcN,str1,pfAllParam[nCurrParam]*1.03323);
					}
					else 
						if (bPEN) {
							MACROSetFormatnValue(nCurrStr-1,nColumnDescrProcN,str1,"Для степени%u расш. сопла",nCurrParam+1);
							MACROSetfValue(nCurrStr,nColumnDescrProcN,str1,pfAllParam[nCurrParam]);
						}
						else 
							if (bIS) {
								MACROSetFormatnValue(nCurrStr-1,nColumnDescrProcN,str1,"Для температуры%u IS-диаграммы",nCurrParam+1);
								MACROSetfValue(nCurrStr,nColumnDescrProcN,str1,pfAllParam[nCurrParam]);
							}
							else 
								if (bV) {
									MACROSetFormatnValue(nCurrStr-1,nColumnDescrProcN,str1,"Для горения при постоянном объеме%u.",nCurrParam+1);
									MACROSetfValue(nCurrStr,nColumnDescrProcN,str1,pfAllParam[nCurrParam]);
								}

					// Объединить столбцы первой строки
					if (bMinNames) {
						MACROMergeRightToLeft(nCurrStr-1,nColumnDescrProcN,
								((nCurrParam==0&&_nCurrP==0&&nCurrUXF==0)?nLenDescrProc-1:cnColWithMinNames-1),
								str1, str2);
					}
					else
						MACROMergeRightToLeft(nCurrStr-1,nColumnDescrProcN, nLenDescrProc-1, str1, str2);

					// Объединить столбцы второй строки
					if (bMinNames) {
						MACROMergeRightToLeft(nCurrStr,nColumnDescrProcN,
								cnColWithMinNames-1, str1, str2);
					}
					else
						MACROMergeRightToLeft(nCurrStr,nColumnDescrProcN, nLenDescrProc-1, str1, str2);
				}

				// Заголовок для данных текущего параметра(Pa, F, IS или просто Pкам)
				nCurrStr ++;
				MACROSetStrValue(nCurrStr, nColumnDescrProcN ,"Параметры процесса");
				// Объединить столбцы для заголовка "Параметры процесса"
				MACROMergeRightToLeft(nCurrStr, nColumnDescrProcN , nLenDescrProc-1, str1, str2);

				nCurrStr ++;
				if ((!bMinNames)||(bMinNames&&((_nCurrP==0)&&(nCurrParam==0)))) {/* Если стоит опция и это не самый левый столбец*/
					MACROSetStrValue(nCurrStr, nColumnDescrProcN, szReport_T);
					if (bInclUMO)
						MACROSetStrValue(nCurrStr, nColumnDescrProcUM, szReport_unitT);
				}
				MACROSetfValue(nCurrStr, nColumnDescrProcV, str1, pfAllT[_nCurrP][nCurrUXF][nCurrParam]);

				nCurrStr ++;
				if ((!bMinNames)||(bMinNames&&((_nCurrP==0)&&(nCurrParam==0)))) {
					MACROSetStrValue(nCurrStr, nColumnDescrProcN, szReport_I);
					if (bInclUMO)
						MACROSetStrValue(nCurrStr, nColumnDescrProcUM, szReport_unitI);
				}
				MACROSetfValue(nCurrStr, nColumnDescrProcV, str1, pfAllI[_nCurrP][nCurrUXF][nCurrParam]);

				nCurrStr ++;
				if ((!bMinNames)||(bMinNames&&((_nCurrP==0)&&(nCurrParam==0)))) {
					MACROSetStrValue(nCurrStr, nColumnDescrProcN, szReport_S);
					if (bInclUMO)
						MACROSetStrValue(nCurrStr, nColumnDescrProcUM, szReport_unitS);
				}
				MACROSetfValue(nCurrStr, nColumnDescrProcV, str1, pfAllS[_nCurrP][nCurrUXF][nCurrParam]);

				nCurrStr ++;
				if ((!bMinNames)||(bMinNames&&((_nCurrP==0)&&(nCurrParam==0)))) {
					MACROSetStrValue(nCurrStr, nColumnDescrProcN, szReport_P);
					if (bInclUMO)
						MACROSetStrValue(nCurrStr, nColumnDescrProcUM, szReport_unitP);
				}
				if (bPa) {
					MACROSetfValue(nCurrStr, nColumnDescrProcV, str1, pfAllParam[nCurrParam]*1.03323);
				}
				else {
					MACROSetfValue(nCurrStr, nColumnDescrProcV, str1, pfAllP[_nCurrP]*1.03323);
				}

				nCurrStr ++;
				if ((!bMinNames)||(bMinNames&&((_nCurrP==0)&&(nCurrParam==0)))) {
					MACROSetStrValue(nCurrStr, nColumnDescrProcN, , szReport_MS);
					if (bInclUMO)
						MACROSetStrValue(nCurrStr, nColumnDescrProcUM, szReport_unitMS);
				}
				MACROSetfValue(nCurrStr, nColumnDescrProcV, str1, pfAllMS[_nCurrP][nCurrUXF][nCurrParam]);

				nCurrStr ++;
				if ((!bMinNames)||(bMinNames&&((_nCurrP==0)&&(nCurrParam==0)))) {
					MACROSetStrValue(nCurrStr, nColumnDescrProcN, , szReport_CP3);
					if (bInclUMO)
						MACROSetStrValue(nCurrStr, nColumnDescrProcUM, szReport_unitCP3);
				}
				MACROSetfValue(nCurrStr, nColumnDescrProcV, str1, pfAllCP3[_nCurrP][nCurrUXF][nCurrParam]);

				nCurrStr ++;
				if (pfAllK3) {
					if ((!bMinNames)||(bMinNames&&((_nCurrP==0)&&(nCurrParam==0)))) {
						MACROSetStrValue(nCurrStr, nColumnDescrProcN, , szReport_K3);
						if (bInclUMO)
							MACROSetStrValue(nCurrStr, nColumnDescrProcUM, szReport_unitK3);
					}
					MACROSetfValue(nCurrStr, nColumnDescrProcV, str1, pfAllK3[_nCurrP][nCurrUXF][nCurrParam]);
				}

				nCurrStr ++;
				if (pfAllB) {
					if ((!bMinNames)||(bMinNames&&((_nCurrP==0)&&(nCurrParam==0)))) {
						MACROSetStrValue(nCurrStr, nColumnDescrProcN, , szReport_B);
						if (bInclUMO)
							MACROSetStrValue(nCurrStr, nColumnDescrProcUM, szReport_unitB);
					}
					MACROSetfValue(nCurrStr, nColumnDescrProcV, str1, pfAllB[_nCurrP][nCurrUXF][nCurrParam]);
				}

				nCurrStr ++;
				if (pfAllPYD) {
					if ((!bMinNames)||(bMinNames&&((_nCurrP==0)&&(nCurrParam==0)))) {
						MACROSetStrValue(nCurrStr, nColumnDescrProcN, szReport_PYD);
						if (bInclUMO)
							MACROSetStrValue(nCurrStr, nColumnDescrProcUM, szReport_unitPYD);
					}
					MACROSetfValue(nCurrStr, nColumnDescrProcV, str1, pfAllPYD[_nCurrP][nCurrUXF][nCurrParam]);
				}

				nCurrStr ++;
				if (pfAllPYDP) {
					if ((!bMinNames)||(bMinNames&&((_nCurrP==0)&&(nCurrParam==0)))) {
						MACROSetStrValue(nCurrStr, nColumnDescrProcN, szReport_PYDP);
						if (bInclUMO)
							MACROSetStrValue(nCurrStr, nColumnDescrProcUM, szReport_unitPYDP);
					}
					MACROSetfValue(nCurrStr, nColumnDescrProcV, str1, pfAllPYDP[_nCurrP][nCurrUXF][nCurrParam]);
				}

				nCurrStr ++;
				if (pfAllF) {
					if ((!bMinNames)||(bMinNames&&((_nCurrP==0)&&(nCurrParam==0)))) {
						MACROSetStrValue(nCurrStr, nColumnDescrProcN, , szReport_F);
						if (bInclUMO)
							MACROSetStrValue(nCurrStr, nColumnDescrProcUM, szReport_unitF);
					}
					MACROSetfValue(nCurrStr, nColumnDescrProcV, str1, pfAllF[_nCurrP][nCurrUXF][nCurrParam]);
				}

				nCurrStr ++;
				if (pfAllF_div_FKP) {
					if ((!bMinNames)||(bMinNames&&((_nCurrP==0)&&(nCurrParam==0)))) {
						MACROSetStrValue(nCurrStr, nColumnDescrProcN, szReport_F_div_FKP);
						if (bInclUMO)
							MACROSetStrValue(nCurrStr, nColumnDescrProcUM, szReport_unitF_div_FKP);
					}
					MACROSetfValue(nCurrStr, nColumnDescrProcV, str1, pfAllF_div_FKP[_nCurrP][nCurrUXF][nCurrParam]);
				}

				nCurrStr ++;
				if ((!bMinNames)||(bMinNames&&((_nCurrP==0)&&(nCurrParam==0)))) {
					MACROSetStrValue(nCurrStr, nColumnDescrProcN, szReport_ZK);
					if (bInclUMO)
						MACROSetStrValue(nCurrStr, nColumnDescrProcUM, szReport_unitZK);
				}
				MACROSetfValue(nCurrStr, nColumnDescrProcV, str1, pfAllZK[_nCurrP][nCurrUXF][nCurrParam]);
			}	// cycle nCurrParam
		}		// cycle _nCurrP
	}			// cycle nCurrUXF

	// Авто определение длинны столбца
	for (int ia=0;ia<rep_nAlfa;ia++)
		for (int ip=0;ip<rep_nP;ip++)
			for (int ipar=0;ipar<rep_nParam;ipar++) {
				for (int idescr=0;idescr<nLenDescrProc;idescr++) {
					MACROGetOffset(0,long(ia*rep_nP*rep_nParam*nLenDescrProc + ip*rep_nParam*nLenDescrProc+ipar*nLenDescrProc+idescr));
					pOffset = rngThroughOffset.GetEntireColumn();
					rng1.AttachDispatch(pOffset);
					rng1.AutoFit();
					rng1.ReleaseDispatch();
					MACROClearOffset();
				}
			}

//	if (!praapc) {
//		str1 = rngLeftTopCorner.GetAddress(); // Левая верхняя ячейка области отчета
//	}
//	else 
	{
		// Формирование заголовка отчета с результатами расчета ПГ
		nCurrStr ++;
		Range rngNewLeftTopCorner;
		Range rngNewSquare;
		for (int ia=0;ia<rep_nAlfa;ia++) {
			for (int ip=0;ip<rep_nP;ip++) {
				for (int ipar=0;ipar<rep_nParam;ipar++) {

					if (bInclUMO)
						nLenDescrProc = cnColWithUnitMeasOption;
					else
						nLenDescrProc = cnColWithoutUnitMeasOption;
//					if ((!bMinNames)||(bMinNames&&((ip==0)&&(ipar==0))))// Если нужно выводить имена
//						nLenDescrProc = cnColWithMinNames;

					long lCol;
					if (bMinNames) {
						lCol = ia*rep_nP*rep_nParam*cnColWithMinNames 
								+ ip*rep_nParam*cnColWithMinNames 
								+ ipar*cnColWithMinNames;
						if ((ipar!=0)||(ip!=0))
							lCol += nLenDescrProc-1;
					}
					else
						lCol = ia*rep_nP*rep_nParam*nLenDescrProc 
								+ ip*rep_nParam*nLenDescrProc 
								+ ipar*nLenDescrProc;

					pOffset = rngLeftTopCorner.GetOffset(COleVariant(long(nCurrStr+1)),COleVariant(lCol));

					rngNewLeftTopCorner.AttachDispatch(pOffset);

					str1.Empty();
					ToDotString(pfAllAlfa[ia], str1);
					str2.Empty();
					ToDotString(pfAllP[ip]*1.03323, str2);

					if ( (bPa)||(bPEN)||(bIS)||(bV)||(bEPS) )	// если расчет с F, Pa, IS диагр.
						strHeader.Format("Состав ПГ Альфа%u=%s, Pкс%u=%s, параметр%u",
								ia+1,(LPCTSTR)str1,ip+1,(LPCTSTR)str2,ipar+1);
					else
						strHeader.Format("Состав ПГ Альфа%u=%s, Pкс%u=%s",
								ia+1,(LPCTSTR)str1,ip+1,(LPCTSTR)str2);
					// Заголовок столбца

	if (bMinNames) {
		lCol = ia*rep_nP*rep_nParam*cnColWithMinNames + ip*rep_nParam*cnColWithMinNames + ipar*cnColWithMinNames;
		if ((ipar!=0)||(ip!=0))
			lCol += nLenDescrProc-1;
		MACROGetOffset(nCurrStr, lCol);
	}
	else {
		MACROGetOffset(nCurrStr,long(ia*rep_nP*rep_nParam*nLenDescrProc 
				+ ip*rep_nParam*nLenDescrProc +ipar*nLenDescrProc ));
	}
					rngThroughOffset.SetValue2(COleVariant(strHeader));
					MACROClearOffset();

					// Запись отчета с результатами расчета ПГ
					WriteToWSOnePC(ip, ia, ipar, 
							ws, rngNewLeftTopCorner, &rngNewSquare, bMinNames);
					str1 = rngNewSquare.GetAddress();	// str1 - адрес области где расположен отчет
				}
			}
		}
		rngNewSquare.ReleaseDispatch();
		rngNewLeftTopCorner.ReleaseDispatch();
	}

	// Возврат области где расположен отчет (Учесть область где располагаются ПС 'str1')
	if(prngSquare) {
		// Возьмем из строки 'str1' адрес нижнего угла прямоугольника с ПС и поместим в 'str2'
		int fr = str1.Find(':');
		str2 = str1.Mid(fr+1);			// Правая часть (Правая нижняя ячейка) адреса отчета о ПС
		str1 = rngLeftTopCorner.GetAddress(); // Левая верхняя ячейка области отчета
		pOffset = ws.GetRange(COleVariant(str1), COleVariant(str2));
		prngSquare ->AttachDispatch(pOffset);//		str1 = prngSquare ->GetAddress(); // DEBUG
	}

	return TRUE; // DEBUG
}

int Report::WriteToReportOnePCA(int nP, int nAlfa, int nParam)
{
	int	nCommonLen = 0;
	memset(szOut,0,sizeof(szOut));

	if (!bWasInitAllPCSTRUCT) 
		return NULL;

//	strReport += strHeader;
	if ( rep_os.nShowOption==cnFull ) {
		switch (rep_os.nPressureOption) {
			case cnPrM:
				strReport += tab;
				strReport += tab;
				if (rep_os.nNumberView == cnASTRANumV)
					strReport += "Мольные доли (моль/кг), Astra view, <PrM>:";
				else
					if (rep_os.nNumberView == cnExpNumV)
						strReport += "Мольные доли (моль/кг), Exponent view, <PrM>:";
				strReport += cf;

				// Astra view мольные доли <PrM> моль/кг
				for (cur_elem=1;cur_elem<=apc.nPAll;cur_elem++) {
					strncpy(szOut, apc.szNam[cur_elem], apc.nMaxNamesLen+1);
					strncpy(&szOut[apc.nMaxNamesLen+1], tab, 1);
					if (rep_os.nNumberView == cnASTRANumV) {
						if (apc.P[nP][nAlfa][nParam][cur_elem]>0)
							buffer = _fcvt(apc.P[nP][nAlfa][nParam][cur_elem],precision*2,&decimal,&sign);
						else
							buffer = _fcvt(exp(apc.P[nP][nAlfa][nParam][cur_elem]),precision*4,&decimal,&sign);

						if (abs(decimal)>precision) {
							continue;
							//for (int ii=0;ii<precision;ii++)
							//	szOut[nMaxNamesLen+1+ii]='0';
						}
						else
							strcpy(&szOut[apc.nMaxNamesLen+1+1],buffer);
						szOut[apc.nMaxNamesLen+1+1+precision] = 0;	

						if (decimal>0)
							strcpy(&szOut[strlen(szOut)],"+O");
						else
							strcpy(&szOut[strlen(szOut)],"-O");

						gcvt(abs(decimal),3,&szOut[strlen(szOut)]);
						// Удалить точку в конце
						if (szOut[strlen(szOut)-1] == '.')
							szOut[strlen(szOut)-1] = '\0';
					}
					else
						if (rep_os.nNumberView == cnExpNumV) {
							if (apc.P[nP][nAlfa][nParam][cur_elem]>0)
								sprintf(&szOut[strlen(szOut)], " %e", apc.P[nP][nAlfa][nParam][cur_elem]);
							else
								sprintf(&szOut[strlen(szOut)], " %e", exp(apc.P[nP][nAlfa][nParam][cur_elem]));
						}
					// Show
					strReport += tab;
					strReport += tab;
					strReport += szOut;	//CEdit
					strReport += cf;
					memset(szOut,0,sizeof(szOut));
				}
			break;

			case cnPrP:
				strReport += cf;
				strReport += tab;
				strReport += tab;
				if (rep_os.nNumberView == cnASTRANumV)
					strReport += "Парциальные давления (МПа), Astra view, <PrP>:";
				else
					if (rep_os.nNumberView == cnExpNumV)
						strReport += "Парциальные давления (МПа), Exponent view, <PrP>:";
				strReport += cf;
				fSum = 0;
				for (cur_elem=1;cur_elem<=apc.nPAll;cur_elem++)
					if (apc.P[nP][nAlfa][nParam][cur_elem]>0)
						fSum += (float)apc.P[nP][nAlfa][nParam][cur_elem];
				for (cur_elem=1;cur_elem<=apc.nPAll;cur_elem++) {
					strncpy(szOut, apc.szNam[cur_elem], apc.nMaxNamesLen+1);
					strncpy(&szOut[apc.nMaxNamesLen+1], tab, 1);
					if (rep_os.nNumberView == cnASTRANumV) {
						if (apc.P[nP][nAlfa][nParam][cur_elem]>0)
							buffer = _fcvt(pfAllP[nP]*apc.P[nP][nAlfa][nParam][cur_elem]/fSum,precision*2,&decimal,&sign);
						else
							buffer = _fcvt(pfAllP[nP]*exp(apc.P[nP][nAlfa][nParam][cur_elem])/fSum,precision*4,&decimal,&sign);

						if (abs(decimal)>precision) {
							continue;
						//for (int ii=0;ii<precision;ii++)
							//	szOut[nMaxNamesLen+1+ii]='0';
						}
						else
							strcpy(&szOut[apc.nMaxNamesLen+1],buffer);
						szOut[apc.nMaxNamesLen+1+precision] = 0;

						if (decimal>0)
							strcpy(&szOut[strlen(szOut)],"+O");
						else
							strcpy(&szOut[strlen(szOut)],"-O");
		
						gcvt(abs(decimal),3,&szOut[strlen(szOut)]);
						// Show
						// Удалить точку в конце
						if (szOut[strlen(szOut)-1] == '.')
							szOut[strlen(szOut)-1] = '\0';
					}
					else
						if (rep_os.nNumberView == cnExpNumV) {
							if (apc.P[nP][nAlfa][nParam][cur_elem]>0)
								sprintf(&szOut[strlen(szOut)], " %e", pfAllP[nP]*apc.P[nP][nAlfa][nParam][cur_elem]/fSum);
							else
								sprintf(&szOut[strlen(szOut)], " %e", pfAllP[nP]*exp(apc.P[nP][nAlfa][nParam][cur_elem]/fSum));
						}

					strReport += tab;
					strReport += tab;
					strReport += szOut;	//CEdit
					strReport += cf;
					memset(szOut,0,sizeof(szOut));
				}
				break;
		}
	}
	return nCommonLen;
}

BOOL Report::WriteToWSOnePC(int nCurrP, int nCurrUXF, int nParam, 
		_Worksheet &ws, Range &rngLeftTopCorner, Range *prngSquare, BOOL _bMinNames)
{
	if (!prngSquare)	return FALSE;
	memset(szOut,0,sizeof(szOut));
	if (!bWasInitAllPCSTRUCT) {
		MACROGetOffset(0,0);
		rngThroughOffset.SetValue2(COleVariant("Ошибка инициализации объекта ПГ"));
		MACROClearOffset();
		str1 = rngLeftTopCorner.GetAddress();
		pOffset = ws.GetRange(COleVariant(str1));
		prngSquare ->AttachDispatch(pOffset);
		return FALSE;
	}

//	int nCol_MinNames = 0;

	if ( rep_os.nShowOption==cnFull ) {
		switch (rep_os.nPressureOption) {
			case cnPrM:
				MACROGetOffset(0,0);
				if (rep_os.nNumberView == cnASTRANumV)
					rngThroughOffset.SetValue2(COleVariant("Мольные доли (моль/кг), Astra view, <PrM>:"));
				else
					if (rep_os.nNumberView == cnExpNumV)
						rngThroughOffset.SetValue2(COleVariant("Мольные доли (моль/кг), Exponent view, <PrM>:"));
				MACROClearOffset();
				for (cur_elem=1;cur_elem<=apc.nPAll;cur_elem++) {
					if ((!_bMinNames)||((_bMinNames)&&nCurrP==0&&nParam==0)) {
						strncpy(szOut, apc.szNam[cur_elem], apc.nMaxNamesLen+1);
						szOut[apc.nMaxNamesLen+1] = 0;
						MACROGetOffset(long(cur_elem),(long)0);
						rngThroughOffset.SetValue2(COleVariant(szOut));
						MACROClearOffset();
					}
					if (rep_os.nNumberView == cnASTRANumV) {
						if (apc.P[nCurrP][nCurrUXF][cur_elem]>0)
							buffer = _fcvt(apc.P[nCurrP][nCurrUXF][nParam][cur_elem],precision*2,&decimal,&sign);
						else
							buffer = _fcvt(exp(apc.P[nCurrP][nCurrUXF][nParam][cur_elem]),precision*4,&decimal,&sign);

						if (abs(decimal)>precision) {
							if (_bMinNames&&(nCurrP!=0||nParam!=0)) {

								MACROGetOffset(long(cur_elem),0);
							}
							else {
								MACROGetOffset(long(cur_elem),1);
							}
							rngThroughOffset.SetValue2(COleVariant("0"));
							MACROClearOffset();
							continue;
							//for (int ii=0;ii<precision;ii++)
							//	szOut[nMaxNamesLen+1+ii]='0';
						}
						else
							strcpy(szOut,buffer);
						szOut[precision] = 0;

						if (decimal>0)
							strcpy(&szOut[strlen(szOut)],"+O");
						else
						strcpy(&szOut[strlen(szOut)],"-O");

						gcvt(abs(decimal),3,&szOut[strlen(szOut)]);
						// Удалить точку в конце
						if (szOut[strlen(szOut)-1] == '.')
							szOut[strlen(szOut)-1] = '\0';
					}
					else
						if (rep_os.nNumberView == cnExpNumV) {
							if (apc.P[nCurrP][nCurrUXF][nParam][cur_elem]>0)
								sprintf(szOut, " %e", apc.P[nCurrP][nCurrUXF][nParam][cur_elem]);
							else
								sprintf(szOut, " %e", exp(apc.P[nCurrP][nCurrUXF][nParam][cur_elem]));
						}

					if (_bMinNames&&(nCurrP!=0||nParam!=0)) {

						MACROGetOffset(long(cur_elem),0);
					}
					else {
						MACROGetOffset(long(cur_elem),1);
					}

					rngThroughOffset.SetValue2(COleVariant(szOut));
					MACROClearOffset();
					memset(szOut,0,sizeof(szOut));
				}
			break;

			case cnPrP:
				MACROGetOffset(0,0);
				rngThroughOffset.SetValue2(COleVariant("Парциальные давления (МПа), Astra view, <PrP>:"));
				MACROClearOffset();
				fSum = 0;
				for (cur_elem=1;cur_elem<=apc.nPAll;cur_elem++)
					if (apc.P[nCurrP][nCurrUXF][cur_elem]>0)
						fSum += (float)apc.P[nCurrP][nCurrUXF][nParam][cur_elem];
				// Astra view Парциальное давление <PrP> МПа
				for (cur_elem=1;cur_elem<=apc.nPAll;cur_elem++) {
					if ((!_bMinNames)||((_bMinNames)&&nCurrP==0&&nParam==0)) {
						strncpy(szOut, apc.szNam[cur_elem], apc.nMaxNamesLen+1);
						szOut[apc.nMaxNamesLen+1] = 0;
						MACROGetOffset(long(cur_elem),(long)0);
						rngThroughOffset.SetValue2(COleVariant(szOut));
						MACROClearOffset();
					}
					if (rep_os.nNumberView == cnASTRANumV) {
						if (apc.P[nCurrP][nCurrUXF][cur_elem]>0)
							buffer = _fcvt(pfAllP[nCurrP]*1.03323*apc.P[nCurrP][nCurrUXF][nParam][cur_elem]/fSum,
									precision*2,&decimal,&sign);
						else
							buffer = _fcvt(pfAllP[nCurrP]*1.03323*exp(apc.P[nCurrP][nCurrUXF][nParam][cur_elem])/fSum,
									precision*4,&decimal,&sign);
		
						if (abs(decimal)>precision) {
							if (_bMinNames&&(nCurrP!=0||nParam!=0)) {

								MACROGetOffset(long(cur_elem),0);
							}
							else {
								MACROGetOffset(long(cur_elem),1);
							}
							rngThroughOffset.SetValue2(COleVariant("0"));
							MACROClearOffset();
							continue;
							//for (int ii=0;ii<precision;ii++)
							//	szOut[nMaxNamesLen+1+ii]='0';
						}
						else
							strcpy(&szOut[apc.nMaxNamesLen+1],buffer);
						szOut[apc.nMaxNamesLen+1+precision] = 0;

						if (decimal>0)
							strcpy(&szOut[strlen(szOut)],"+O");
						else
							strcpy(&szOut[strlen(szOut)],"-O");

						gcvt(abs(decimal),3,&szOut[strlen(szOut)]);
						// Удалить точку в конце
						if (szOut[strlen(szOut)-1] == '.')
							szOut[strlen(szOut)-1] = '\0';
					}
					else
						if (rep_os.nNumberView == cnExpNumV) {
							if (apc.P[nCurrP][nCurrUXF][nParam][cur_elem]>0)
								sprintf(&szOut[strlen(szOut)], " %e", pfAllP[nCurrP]*apc.P[nCurrP][nCurrUXF][nParam][cur_elem]/fSum);
							else
								sprintf(&szOut[strlen(szOut)], " %e", pfAllP[nCurrP]*exp(apc.P[nCurrP][nCurrUXF][nParam][cur_elem])/fSum);
						}

					if (_bMinNames&&(nCurrP!=0||nParam!=0)) {
						MACROGetOffset(long(cur_elem),0);
					}
					else {
						MACROGetOffset(long(cur_elem),1);
					}
					rngThroughOffset.SetValue2(COleVariant(szOut));
					MACROClearOffset();
					memset(szOut,0,sizeof(szOut));
				}
				break;
		}
	}

	int nLenDP;
	if (rep_os.bIncludeUnitMeasOption)
		nLenDP = cnColWithUnitMeasOption;
	else
		nLenDP = cnColWithoutUnitMeasOption;

	// Возврат области где расположен отчет
	if(prngSquare) {
		str1 = rngLeftTopCorner.GetAddress();
		MACROGetOffset(long(apc.nPAll),long(nLenDP-1));
		str2 = rngThroughOffset.GetAddress();
		MACROClearOffset();
		pOffset = ws.GetRange(COleVariant(str1), COleVariant(str2));
		prngSquare ->AttachDispatch(pOffset);//		str1 = prngSquare ->GetAddress(); // DEBUG
	}
	return TRUE;
}


CalcBase::CalcBase() 
{
	nCurrP = 0;
	nCurrUXF = 0;
	nCurrParam = 0;
	nP = 0;
	nAlfa = 0;
	nParam = 0;
	pTherDynEqCalc = NULL;
	pRep = NULL;
}

BOOL CalcBase::CommonConstr(CTherDynEqCalc *p, 
		UXFSTRUCT _uxf[MaxQuantityFIHBox], CALCSTRUCT _calcs) 
{
	if (!p) return FALSE;
	pTherDynEqCalc = p;

	nEnthalpyF = _calcs.nEditBoxesAlfa;
	for (int i=0;i<nEnthalpyF;i++)
		fAllEnthalpyF[i] = _uxf[i].fEnthalpyF;

	nP = _calcs.nEditBoxesPressure;
	for (int i1=0;i1<nP;i1++)
		fAllP[i1] = _calcs.fPressure[i1]/1.03323;

	nAlfa = _calcs.nEditBoxesAlfa;
	for (int i2=0;i2<nAlfa;i2++) {
		fAllAlfa[i2] = _calcs.fAlfa[i2];
		fKappaStroke0[i2] = _calcs.fKappaStroke0[i2];
	}
//	for (int i3=0;i3<_calcs.nEditBoxesAlfa;i3++)
//		bc_uxf[i3] = _uxf[i3];

	fAssumedT = _calcs.fAssumedT;
	fMinT = _calcs.fMinT;
	fMaxT = _calcs.fMaxT;

	nCurrP = 0;
	nCurrUXF = 0;
	nCurrParam = 0;
	memset(fAllProcessP,0,sizeof(fAllProcessP));
	memset(fAllT,0,sizeof(fAllT));
	memset(fAllI,0,sizeof(fAllI));
	memset(fAllS,0,sizeof(fAllS));
	memset(fAllMS,0,sizeof(fAllMS));
	memset(fAllCP3,0,sizeof(fAllCP3));
	memset(fAllK3,0,sizeof(fAllK3));
	memset(fAllB,0,sizeof(fAllB));
	memset(fAllPYD,0,sizeof(fAllPYD));
	memset(fAllPYDP,0,sizeof(fAllPYDP));
	memset(fAllF,0,sizeof(fAllF));
	memset(fAllF_div_FKP,0,sizeof(fAllF_div_FKP));
	memset(fAllZK,0,sizeof(fAllZK));
	bWasNextAlfa = FALSE;
	return TRUE;
}

BOOL CalcBase::ConstrShortRep(CTherDynEqCalc *p, 
		UXFSTRUCT _uxf[MaxQuantityFIHBox], CALCSTRUCT _calcs) 
{	
	if(!CommonConstr(p, _uxf, _calcs))
		return FALSE;
	if(pRep)
		delete pRep;
	pRep = new ::Report(
			nP,
			nAlfa,
			NULL,
			_uxf,
			fAllAlfa,
			fKappaStroke0,
			NULL,
			fAllP,
			fAllProcessP,
			fAllT,
			fAllI,
			fAllS,
			fAllMS,
			fAllCP3,
			NULL,
			NULL,
			NULL,
			NULL,
			NULL,
			NULL,
			fAllZK
			);
	return TRUE;
}

BOOL CalcBase::ConstrLongRep(CTherDynEqCalc *p, 
		UXFSTRUCT _uxf[MaxQuantityFIHBox], CALCSTRUCT _calcs) 
{
	if(!CommonConstr(p, _uxf, _calcs))
		return FALSE;
	if(pRep)
		delete pRep;
	pRep = new ::Report(
			nP,
			nAlfa,
			NULL,
			_uxf,
			fAllAlfa,
			fKappaStroke0,
			NULL,
			fAllP,
			fAllProcessP,
			fAllT,
			fAllI,
			fAllS,
			fAllMS,
			fAllCP3,
			fAllK3,
			fAllB,
			fAllPYD,
			fAllPYDP,
			fAllF,
			fAllF_div_FKP,
			fAllZK
			);
	return TRUE;
}

BOOL CalcBase::ConstrLongParamRep(CTherDynEqCalc *p, 
		UXFSTRUCT _uxf[MaxQuantityFIHBox], CALCSTRUCT _calcs) 
{
	if(!CommonConstr(p, _uxf, _calcs))
		return FALSE;
	if(pRep)
		delete pRep;
	pRep = new ::Report(
			nP,
			nAlfa,
			nParam,
			_uxf,
			fAllAlfa,
			fKappaStroke0,
			fAllParam,
			fAllP,
			fAllProcessP,
			fAllT,
			fAllI,
			fAllS,
			fAllMS,
			fAllCP3,
			fAllK3,
			fAllB,
			fAllPYD,
			fAllPYDP,
			fAllF,
			fAllF_div_FKP,
			fAllZK
			);
	return TRUE;
}

CalcBase::~CalcBase() {
	if(pRep)
		delete pRep;
}

BOOL CalcBase::PrepareToFirstCalc() 
{nCurrUXF = 0;	nCurrP = 0;	nCurrParam=0; pRep ->PrepareToFirstCalc(); return TRUE;}

BOOL CalcBase::NextAlfa() {	// и установка начального давления в КС
	bWasNextAlfa = TRUE;
	if ( (nCurrUXF+1)>=nAlfa )
		return FALSE;
	nCurrUXF++;
	nCurrP = 0;
	pRep ->NextAlfa();
	return TRUE;
}

BOOL CalcBase::NextPressure() {
	if ( (nCurrP+1)>=nP )
		return FALSE;
	nCurrP++;
	pRep ->NextPressure();
	return TRUE;
}

void CalcBase::FirstParam() {nCurrParam = 0;}
BOOL CalcBase::NextParam() {
	if ( (nCurrParam+1)>=nParam )
		return FALSE;
	nCurrParam++;
	return TRUE;
}

void CalcBase::SetOptionsReport(OPTIONSTRUCT &ext_os) {
	pRep ->SetOptionsReport(ext_os);
}

BOOL CalcBase::Calc(ProductsCombustion &PC, INTOCALCSTRUCT &ics) {
	PP = fAllP[nCurrP];
	if(!PP) return FALSE;// ??? !!! Пока не разработано средство от зацикливания надо проверять
	return TRUE;
}

int CalcBase::GetReportA(CString &ext_strReport, int nMaxLenReport) {
	// Формирование текстового отчета с результатами расчета
	pRep ->WriteAllCalcsInStr();
	ext_strReport += pRep ->GetReportA();
	return 1;
}

BOOL CalcBase::WriteToExcelWorksheet(_Worksheet &ws,Range &rngLeftTopCorner,
		Range *prngSquare) {
	return pRep ->WriteAllCalcsInWS(ws, rngLeftTopCorner, prngSquare);
}

/*
P/V=MRT

Vi = V/m ; V i-тое

выбирается объем для единичной массы
P = RT/Vi - должно быть выполнено

Iтоплива = Iпс

Uт = Uпс внутренняя энергия
---------------------------
Задается давление и температура
ход процесса до Uт = U пс

корректируется температура???

  задаются удельные объемы
  температура Uт = U пс
  давление   P = RT/Vi - должно быть выполнено
*/
