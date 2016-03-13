#if !defined(CALC_INCLUDED_)
#define CALC_INCLUDED_

#include "ThermoCalc.h"
#include "ThermoCalcDoc.h"
//void ToDotString(double &rdValue, CString &rS);
void ToDotString(const float &rdValue, CString &rS);
void ToDotStringWOZ(const float &rdValue, CString &rS);
BOOL ToWholeNumberString(const float &rdValue, CString &rS);
CThermoCalcDoc * GetCurrentCalcDoc();
int FindTrueAtomInStr(char dest[][MaxAtomNameLen],char *source, int*);
int GetStr(char *dest,char *source);
int GetUnsFactorFromStr(float *,char *);
int GetFactorFromStr(float *,char *);
BOOL ChangeCommaToDot(char *p);
BOOL ChangeCommaToDot(CString &str);
int ffind_elem(int, char *, int, 
			   char _molecul[], char _name_elem[], double __factor[], int nCoef);
int GetSN(char *str);//Получить номер элемента по его символьному обозначению
int GetPossibleChemCombinGas(chem_dataGas [MaxElemGasFromDB], char pszNamesOfAtom[][MaxAtomNameLen],int nAtoms);
int GetPossibleChemCombinCond(chem_dataCond [MaxElemGasFromDB], char pszNamesOfAtom[][MaxAtomNameLen],int nAtoms);
int TransNameMolecToAandF(char str_dest[][MaxAtomNameLen],float *f_dest,char*);

#endif // !defined(CALC_INCLUDED_)
