// this file is "_from_termo.h"
#include "ThermoCalc.h"
void init_var_of_TERMO1 (const double M[],int &JE,BOOL &FT,BOOL &FV,double &R0,int HAC[]);
void init_var_of_TERMO2 (void);
void Init_CALBR_STREAM (void);
void init_var_of_TERMO3 (BOOL &FB);
void init_var_of_TERMO4 (int &N,int &NG);
//void CALBR (void);
BOOL CALBR (BOOL &FIS,BOOL &FT,BOOL &FV,int HAC[]);
//void MENJE (float MC[],int &M,int &JE);
BOOL MENJE (float MC[],int &M,int &JE,BOOL &F0T,BOOL &FPP,BOOL &CKACH,
			BOOL &FIS,BOOL &KAMEPA,BOOL &FT,BOOL &F3C,
			BOOL &FV,BOOL &FP3,float &H,float &P1,
			double &IS,float &T,double &SS,float &T2,float &T1,
			float &PP,float &P2);
void MENJE_CameraCombustion(float MC[],int &M,int &JE,BOOL &F0T,BOOL &FPP,BOOL &CKACH,
		BOOL &FIS,BOOL &KAMEPA,BOOL &F3C,BOOL &FP3,float &H,
		float &T,float &T2,float &T1,float &PP);

void MENJE_CameraCombustion_IS(float MC[],int &M,int &JE,BOOL &F0T,BOOL &FPP,BOOL &CKACH,
		BOOL &FIS,BOOL &KAMEPA,BOOL &FT,BOOL &F3C,BOOL &FV,BOOL &FP3,float &H,float &P1,
		float &T,float &T2,float &T1,float &PP,float &P2);

void MENJE_TransitionToFindCriticalSection(BOOL &KAMEPA,BOOL &F3C,
		float &H,double &IS,float &T,double &SS,float &T2,float &T1,float &PP);
void MENJE_NextStepToFindCriticalSection(float &T1,float &PP);

void MENJE_TransitionToFindSection(BOOL &F0T,BOOL &CKACH,BOOL &FIS,BOOL &KAMEPA,BOOL &FT,
		BOOL &F3C,BOOL &FP3,float &P1,float &T,float &T2,float &T1,float &PP,float &P2);
BOOL MENJE_NextStepToFindSection(float &MS,double &W,float &E,double &IS,
		BOOL &F0T,BOOL &CKACH,BOOL &FIS,BOOL &KAMEPA,BOOL &FT,
		BOOL &F3C,BOOL &FP3,float &P1,float &T,float &T2,float &T1,float &PP,float &P2);

void FIRST (float _XF1[],float _XF2[],float _MC[], int M, int N, int NG, BOOL &FPP,float P[]);
//void PABHPAC (float _XF1[],float _XF2[], int M, int N, int NG);
void PABHPAC (float XF1[],float XF2[], int M, int N, int NG,
	BOOL &FPP,BOOL &F3C,BOOL &FB,BOOL &FP3,float &CK,double &R0,
	float &MS,double &CPR,double &IS,float &T,double &SS,
	float &E,float &PP,int HAC[]);

void CALFN(const int N,const int NG,float &CK,float &T,
		BOOL &CKACH,float &T2,float &T1,chem_dataGas cdG[],chem_dataCond cdC[]);

BOOL PRINT_TestBTMIN(float &MS,BOOL &KAMEPA,BOOL &FIS,float &PYD,float &PYDP,
		double &IS,float &T,float &PP);
void PRINT_CameraCombustion(BOOL &KAMEPA,BOOL &FIS,float &PP,double &IS,float &T,
		float &MS,type_ParamInCC &_ParamInCC);
void PRINT_CriticalSection(float &MS,BOOL &KAMEPA,BOOL &FIS,double &W,float &PYD,float &PYDP,
	float &E,double &IS,float &T,float &PP,type_CriticalSection &CriticalSection);
void PRINT_Section(float &MS,BOOL &KAMEPA,BOOL &FIS,double &W,float &PYD,float &PYDP,
	float &E,double &IS,float &T,float &PP,type_Section &Section);
