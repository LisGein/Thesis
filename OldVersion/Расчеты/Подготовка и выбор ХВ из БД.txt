#include "stdafx.h"
#include "ThermoCalc.h"
#include "Calc.h"
#include "table.h"
#include <math.h>		// fabs();

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

int FindTrueAtomInStr(char **,char *, int*);
int TransNameMolecToAandF(char **,float*,char*);

extern char Mstr[256];//сообщение об ошибках
extern char szChemdataGas[];
extern char szChemdataCond[];

char sz[MAX_len];	// ToDotString();
char *buffer;
int  decimal, sign;

CThermoCalcDoc *GetCurrentCalcDoc()
{
	CString S;
	enum CDocTemplate::DocStringIndex i;
	i = CDocTemplate::fileNewName;		// Ищем по имени файлов документа

	CWinApp * pCWA = AfxGetApp();
	POSITION pos = pCWA-> GetFirstDocTemplatePosition();
	do
	{
		CDocTemplate* pCDT = pCWA -> GetNextDocTemplate(pos);
		POSITION pos2 = pCDT->GetFirstDocPosition();
		// Если есть шаблон но нет открытых документов
		if (!pos2) continue;
		do
		{
			CDocument* pD = pCDT->GetNextDoc(pos2);
			CThermoCalcDoc * pMyDoc = (CThermoCalcDoc *)pD;
			BOOL bR = pCDT->GetDocString(S,i);
			if (bR) {
				if (S == "Output")
					//pMyDoc ->Up();
					return pMyDoc;
			}
		}
		while(pos2);
	}
	while (pos);

	return NULL;
}

/*void ToDotString(double &rdValue, CString &rS) {
	char *buffer;
	int  decimal, sign;
	buffer = _fcvt( rdValue, constToDotString , &decimal, &sign);
	if (sign)
		rS += "-";
	if (decimal<=0) {
		rS += "0.";
		for (int i = decimal;i<0;i++)
			rS += "0";
		strcpy(c,buffer);
		rS += c;
	}
	else {
		strncpy(c,buffer,decimal);
		c[decimal] = 0;
		rS += c;
		rS += ".";
		rS += &buffer[decimal];
	}
}*/

// Замечание: Именно прибавляет к строке число. не менять.
void ToDotString(const float &fValue, CString &rS) {
	buffer = _fcvt( fValue, constToDotString , &decimal, &sign);
	if (sign)
		rS += "-";
	if (decimal<=0) {
		rS += "0.";
		if( fabs((float)decimal)>fabs((float)constToDotString) )
			decimal=constToDotString*SIGN(decimal);
		for (int i = decimal;i<0;i++)
			rS += "0";
		strcpy(sz,buffer);
		rS += sz;
	}
	else {
		strncpy(sz,buffer,decimal);
		sz[decimal] = 0;
		rS += sz;
		rS += ".";
		rS += &buffer[decimal];
	}
}

// Число в строку сплавающей точкой без нулей на конце
void ToDotStringWOZ(const float &fValue, CString &rS) {
	buffer = _fcvt( fValue, constToDotString , &decimal, &sign);
	if (sign)
		rS += "-";
	if (decimal<=0) {
		rS += "0.";
		if( fabs((float)decimal)>fabs((float)constToDotString) )
			decimal=constToDotString*SIGN(decimal);
		for (int i = decimal;i<0;i++)
			rS += "0";
		strcpy(sz,buffer);
		rS += sz;
	}
	else {
		strncpy(sz,buffer,decimal);
		sz[decimal] = 0;
		rS += sz;
		rS += ".";
		rS += &buffer[decimal];
	}

	int nLen = rS.GetLength();
	int nDecPart = nLen - 1 - rS.Find('.'); // -1: надо учесть точку
	for (int i=0; i<nDecPart; i++) {
		if (rS[nLen-i-1] == '0') {	// -1: отчет с нуля
			rS.Delete(nLen-i-1);		// -1: отчет с нуля
		}
	}
}

// В стоку число без десятичных разрядов. Если число без десятичных разрядов то вернуть TRUE.
BOOL ToWholeNumberString(const float &fValue, CString &rS) {
	ChangeCommaToDot(rS);
	buffer = _fcvt( fValue, constToDotString , &decimal, &sign);
	unsigned int len = strlen(buffer);
	unsigned int uiFullSize = 0;
	for (int i = (len-1); i>=0; i--)
		if (buffer[i] != '0')
			uiFullSize ++;
	//unsigned int uiFullSize = atoi(buffer)
	if ( (decimal<=0) || (uiFullSize > ( (unsigned int)decimal )) )
		return FALSE;
	if (sign)
		rS += "-";
	strncpy(sz,buffer,decimal);
	sz[decimal] = 0;
	rS += sz;
	return TRUE;
}

//Преобразование строки в массив атомов и коэф.(обратная функция ListMixtureToStr)
int TransNameMolecToAandF(char szAtoms[][MaxAtomNameLen], float *factor, char *szMolecule)
{
	unsigned int index=0;
	int count=0;
	int Len_atoms=0;				//Len_atom длинна распознанной строки с хим. обозн. атома
	char reserv[MAX_len];

	do
	{
		memset(reserv,0,sizeof(reserv));
//		if (!strcmp(szMolecule,"OD"))		{			szMolecule[0] = 'O';		}
//		if (!strcmp(szMolecule,"D"))		{			szMolecule[0] = 'D';		}
		// Переписать в 'reserv' литерную строку
		int gs = GetStr(reserv,&szMolecule[index]);//gs длинна строки с хим. формулой
		int number_elem = FindTrueAtomInStr(&szAtoms[count],reserv,&Len_atoms);

		if (number_elem == -1) return 0;
		if (number_elem==0) {
			MessageBox(0,"TransNameMolecToAandF(): Unknown chemisry element in Molecule.","Error",0);
			return 0; 
		}
		if ((count+number_elem) > MaxAtoms)
			{ MessageBox(0,"TransNameMolecToAandF(): Количество элементов в молекуле больше 'MaxAtoms'.","Error",0); return 0; }

		//проверить есть ли у тестируемой строки лишние символы после распознанных
		if (GetStr(reserv,&szMolecule[index+Len_atoms]) != 0)
		{ MessageBox(0,"TransNameMolecToAandF(): Встретился некорректный символ после элемента.","Error",0); return 0; }

		index+=gs;
		int gf;
		for (int i=count;i<count+number_elem;i++) {
			factor[i] = 0;
		}
		gf = GetUnsFactorFromStr(&factor[count+number_elem-1],&szMolecule[index]);//for LAST finded element
		if((gf==0)&(strlen(szMolecule)>index))//не последний ?
		{ MessageBox(0,"TransNameMolecToAandF(): Встретился некорректный коэффициент элемента.","Error",0); return 0; }

/*	//factor[count-1+number_elem]=0;
	// Если number_elem больше одного то читаем коэффициент у последнего 
	int gf= GetUnsFactorFromStr(&factor[count-1+number_elem],&szMolecule[index]);//for last finded element
//у последнего атома последний коэффициент можно не вводить, он по умолчанию = 0(превращяясь потом в 1)
	if((gf==0)&(strlen(szMolecule)>index))//не последний ?
		{MessageBox(0,"Неверный коэффициент","Error",0); return 0;}*/
		index+=gf;
		count+=number_elem;
	}
	while( GetStr(reserv,&szMolecule[index]) );

	// trans factor 0 to factor 1//
	for(int i1=0;i1<count;i1++) {
		if(!factor[i1])
			factor[i1]=1;
	}
	//calc and //show:
//	for(int i2=0;pszAtoms[i2]!=0;i2++);
	return count;
}

// Распознавание хим. периодических элементов из текстовой строки и
// запись их в массив
int FindTrueAtomInStr(char dest[][MaxAtomNameLen], char *src_, int* size_a)//USE global: char *table[]
{
	*size_a=0;									//init
	int len=strlen(src_);
	if (!len) {
		//MessageBox(0,"FindTrueAtomInStr(): Пустая строка переданна в функцию.","Error",0);
		return 0;
	}
	if ( (len-1)>MAX_len ) {
		MessageBox(0,"FindTrueAtomInStr(): Длинна строки переданной в функцию больше 'MAX_len'.","Error",0);
		return -1;
	}

	int count=0,in_count=0;
	char src[MAX_len];
	char s1[MAX_len];		// Алгоритм основан на двух символьном имени хим эл.
	char s2[MAX_len];		//

//Init
	memset(src,0,sizeof(src));
	memset(s1,0,sizeof(s1));
	memset(s2,0,sizeof(s2));

	for(int ext_i=0;ext_i<len;ext_i++)//По всей строке
	{
	//!cout<<"\nbegin with symbol="<<ext_i+1;
		for(int in_i=1;in_i<=(len-ext_i);in_i++)//Счетчик кол-ва символов
		{				       //которые будут взяты для распознавания
			strncpy(src,&src_[ext_i],strlen(&src_[ext_i]));
			src[in_i]='\0';
			for(int i=1;i<PERIOD_TABLE_size;i++) { //scan in table
				if(!strcmp(src,table[i])) { //найдено соответствие
					//!cout<<"\nfinded element: "<<src<<" serial number="<<i;
					//getch();
					//если s1 заполнено
					if(in_count==1) {
						strcpy(s2,src);
						goto end_scan;
					}
					else {            	// s1 не заполнено
						strcpy(s1,src);
						in_count++;
						break;	//закончить поиск в таблице
					}
				} //scan next symbol in src
			}
			//Основано на том что текстовое представление любого хим. эл-та не больше двух
			//Если число только что проверенных символов больше 1 и найдено соответствие
			if((in_i>1)&&(in_count==1))goto end_scan;
			//Если число только что проверенных символов больше 1 и пока не найдено соответствие
			if((in_i>1)&&(in_count==0)){//MessageBox(0,"Bad chimestry element. Exit.\n","Fatal error",0);
				return 0;}
			//попробуй ввести Fee
		}
//test and write
end_scan:
		if(!in_count) {	// Не найдено ни одного соответствия в хим. таблице
			goto fail;//fail
		}

		if(strlen(s1)>strlen(s2)) {//Выбирает самый длинный элемент
			//!cout<<"\nResult:"<<s1;
			//dest[count]=new char[strlen(s1)+1];
			if ((strlen(s1)+1)>MaxAtomNameLen)
				{ MessageBox(0,"FindTrueAtomInStr(): Длинна хим. эл-та в молекуле больше 'MaxAtomNameLen'.","Error",0); return 0; }
			strcpy(dest[count],s1);
			if(strlen(s1)==2)
				{ext_i++;*size_a+=2;}
			else			
				*size_a+=1;

			count++;   //?
		}
		else {
		//!cout<<"\nResult:"<<s2;
			//dest[count]=new char[strlen(s2)+1];
			if ((strlen(s2)+1)>MaxAtomNameLen)
				{ MessageBox(0,"FindTrueAtomInStr(): Длинна хим. эл-та в молекуле больше 'MaxAtomNameLen'.","Error",0); return 0; }
			strcpy(dest[count],s2);
			if(strlen(s2)==2)
				{ext_i++;*size_a+=2;}
			else			
				*size_a+=1;
			count++;   //?
		}
//Clear
fail:
		memset(src,0,len+1);
		memset(s1,0,len+1);
		memset(s2,0,len+1);
		in_count=0;
		//getch();
	}
//	delete src;
//	delete s1;
//	delete s2;
	return count;//Возвращает кол-во найденных хим. элементов
}


int GetStr(char *sdest_b,char *ssrc_b)//Копирует в /sdest_b/ только
				      //алфавитные символы из /ssrc_b/
				      //до первого не алфавитного символа
{				      //возвращает длинну /sdest_b/
	int l = strlen(ssrc_b);
	// Число не должно заканчиваться с разделителя раз.
	if( (ssrc_b[l] == '.')||(ssrc_b[l] == ',') )
		return 0;
	char c = ssrc_b[0];
	// Число не должно начинаться с разделителя раз.
	if ( (c=='.')||(c==',') )
		return 0;
	int i = 0;
   while(Letter)
    {
	i++;
	c=ssrc_b[i];//for macro
    }
   if(!i){sdest_b[0] = 0; return 0;}
	//Ok 'atof' будет работать
   strncpy(sdest_b,ssrc_b,i);
	sdest_b[i]=0;
   return i;
}

// Преобразовать строку с числом (без знака) в float и вернуть длинну строки с числом
// Число не должно начинаться и заканчиваться с разделителя разрядов
int GetUnsFactorFromStr(float *fdest_b,char *fsrc_b) //else!
{
	int l = strlen(fsrc_b);
	// Число не должно заканчиваться с разделителя раз.
	if( (fsrc_b[l-1] == '.')||(fsrc_b[l-1] == ',') )
		return 0;
	char c = fsrc_b[0];
	// Число не должно начинаться с разделителя раз.
	if ( (c=='.')||(c==',') )
		return 0;
	int i = 0;
	while(Numeral||(c=='.')||(c==','))//Пока не будет найден символ отличный от числа и "."
	{
		i++;
		c=fsrc_b[i];
	}

   if(!i){fdest_b[0] = 0; return 0;}
	//Ok 'atof' будет работать
	*fdest_b=(float)atof(fsrc_b);//may be returned: overflow!!!
//	if (!(*fdest_b))// Если передать строку с числом нуль, то функция вернет 0
//		return 0;
	return i;
}

// Преобразовать строку с числом (со знаком) в float и вернуть длинну строки с числом
// Число не должно начинаться и заканчиваться с разделителя разрядов
int GetFactorFromStr(float *fdest_b,char *fsrc_b) //else!
{
	int i=0;
	char c=fsrc_b[0];
	while(Numeral||(c=='.')||(c==',')||(c=='-')||(c=='+'))//Пока не будет найден символ отличный от числа и "."
	{
		i++;
		c=fsrc_b[i];
	}

	if(!i){return 0;}
	//Ok 'atof' будет работать
	*fdest_b=(float)atof(fsrc_b);//may be returned: overflow!!!
//	if (!(*fdest_b))// Если передать строку с числом нуль, то функция вернет 0
//		return 0;
	return i;
}

BOOL ChangeCommaToDot(CString &str) {
	int i=0;
	char *p = str.GetBuffer(MAX_len);
	char *pLastOccurrence = strrchr(p, ',');
	str.ReleaseBuffer();
	if (pLastOccurrence) {
		*pLastOccurrence = '.';
		return TRUE;
	}
	return FALSE;
}
BOOL ChangeCommaToDot(char *psz) {
	int i=0;
	char *pLastOccurrence = strrchr(psz, ',');
	if (pLastOccurrence) {
		*pLastOccurrence = '.';
		return TRUE;
	}
	return FALSE;
}

int GetSN(char *str)//Получить номер элемента по его символьному обозначению
{
 for(int i=1;i<=PERIOD_TABLE_size;i++)//scan in table
   if(!strcmp(str,table[i]))return i;
 return 0;
}

#define szMAX_elem 256	//рамер в байтах максималной ячейки из Б.Д.
#define szMAX_name 20
#define szMAX_molecul 20
#define szMAX_factor 32

#define nErrorMsgLength 80
char	cErrorMsg[nErrorMsgLength];

// Найти все химические соединения в базе, которые содержат заданный атом.
// Для всех атомов переданных в функцию,исключая совпадения соединений.
// Заполнить структуры с названием и коэффициентами найденных соединений. /strFV/
// (необходимо, обраничить максимальное возможное количество структур
// исключая ошибку переполнения)
int GetPossibleChemCombinGas(chem_dataGas data[MaxElemGasFromDB], char pszNamesOfAtom[][MaxAtomNameLen],int nAtoms)
{
	char name_elem[szMAX_name+1];
	char molecule[szMAX_molecul];
	double 	factor[NumFactorsGas];	//a0,a1,...a7

	char strFV[MAX_len];
	char strFVLetter[MAX_len];
	memset(strFV,0,sizeof(strFV));
	memset(strFVLetter,0,sizeof(strFVLetter));

	int	nValidElemsFromDB=0;

	//Поиск по всей базе
//открытие файла
	HANDLE in=CreateFile(szChemdataGas,
			GENERIC_READ,FILE_SHARE_READ,0,OPEN_EXISTING,0,0);
	if(in==INVALID_HANDLE_VALUE)
	{	if ((strlen(szChemdataGas)+strlen("Unable to open file: "))>=nErrorMsgLength) return 0;
		wsprintf(cErrorMsg,"Unable to open file: %s",szChemdataGas);
		MessageBox(0,cErrorMsg,"Error",0);
		return -1;	}

	DWORD hi_fsize=0;
	const DWORD fsize=GetFileSize(in,&hi_fsize);
	if(hi_fsize)
		MessageBox(0,"Слишком велик размер Б.Д. (более 4 Гб :о ).","Warning!",0);

	HANDLE hFM=CreateFileMapping(in,0,PAGE_READONLY|SEC_COMMIT,0,fsize,0);
	if(!hFM)
	{	MessageBox(0,"Не могу получить идентификатор объекта-отображения файла Б.Д. на память (CreateFileMapping()).","Error. Exit.",0);
		return -1;	}

	char * lpFileMap=(char *)MapViewOfFile(hFM,FILE_MAP_READ,0,0,0);
	if(!lpFileMap)
	{	MessageBox(0,"Не могу сделать проекцию файла Б.Д. на память (MapViewOfFile()).","Error. Exit.",0);
		return -1;	}


//Поиск по всей базе

	// Для перезаписи молекулы из БД в список по атомам
	char pszElemsFromDB[MaxAtoms][MaxAtomNameLen];
	float fT[MaxAtoms];


	int i_elem = -1;
	do
	{
		i_elem++;
		// Возмем элемент номер i_elem
		if(!ffind_elem(i_elem,&lpFileMap[0],fsize, molecule, name_elem, factor,NumFactorsGas))
			return nValidElemsFromDB;

		// Вырежем не литерные символы
		int	f = 0;
		int	j = 0;
		int	curlenS = 0;
		int	curlenF = 0;
		float res;
		while(strlen(&molecule[f]))
		{
			curlenS = GetStr(&strFVLetter[j],&molecule[f]);
			j += curlenS;
			strFVLetter[j] = 0;
			f += curlenS;
			curlenF = GetUnsFactorFromStr(&res,&molecule[f]);
			f += curlenF;
			if((!curlenS)&&(!curlenF))
				break;					// Уже нет распознаваемых знаков
		}
		// Если встретились не литерные символы в молекуле из БД
		if((!curlenS)&&(!curlenF))
			continue;	// То взять новый элемент из БД

		// Перепишем молекулу из БД в список по атомам
		int nQuantAtomsFromDB =
			TransNameMolecToAandF(pszElemsFromDB,fT,molecule);
		// Если встретились нераспознаваемые символы в молекуле из БД
		if(!nQuantAtomsFromDB)
			continue;	// То взять новый элемент из БД

		//цикл по всем элементам хим. соединения из БД
		int nEq = 0;
		for (int k=0;k<nQuantAtomsFromDB;k++)
			for (int ext_i=0;ext_i<nAtoms;ext_i++)
				if (!strcmp(pszElemsFromDB[k],pszNamesOfAtom[ext_i]))
					nEq ++;

		if (nEq==nQuantAtomsFromDB) {
			strcpy(data[nValidElemsFromDB].view,molecule);
			strcpy(data[nValidElemsFromDB].name,name_elem);
			for(int i2=0;i2<NumFactorsGas;i2++)
				data[nValidElemsFromDB].factors[i2]=factor[i2];
			nValidElemsFromDB++;
			if(nValidElemsFromDB>=MaxElemGasFromDB) {
				wsprintf(Mstr,"Поиск в БД окончен.\
				\n Соединение <%s>, не умещается в памяти, более %d.",molecule,MaxElemGasFromDB);
				MessageBox(0,Mstr,"Message",0);
				return nValidElemsFromDB;
			}
		}
	}while(1);

	UnmapViewOfFile(lpFileMap);
	CloseHandle(in);
	return nValidElemsFromDB;
}

//Поиск элемента в файле б.д. с порядковым номером /num_elem/ (через /in/)
int ffind_elem(int num_elem, char * in_buf, int size_buf,
		char _molecul[],char _name_elem[],double __factor[],int nCoef)//и заполнение глобальных буферов
{
 char  	_elem[szMAX_elem];
 unsigned long lLast_elem,lNext_elem;
 long i;
 int size=0;
 DWORD dword;
 LPDWORD lpNumberOfBytesRead=& dword;
 lNext_elem=0;//отсчет файлового указателя идет с 0
 lLast_elem=0;


 //Поиск элемента б.д. с порядковым номером /num_elem/
 int cur_elem;
 for(cur_elem=0;cur_elem<num_elem+1;cur_elem++)//
	{
		lLast_elem=lNext_elem;
		for (i=lNext_elem;i<size_buf;i++)//Поиск конца элемента б.д.
			{
				if (in_buf[i]==13) 
					{
						i++;//Коррекция размера
						if (in_buf[i]==10)					
							break; //Набор символов с кодами 13,10 конец ячейки
					}
			}
		i++;
		lNext_elem=i;//файловый адрес байта следующего за точкой
		//Конец буффера?
		if(i>=size_buf) {
			// конец базы
			if (cur_elem == num_elem) {
				// может это как раз конец  элемента б.д.
				lNext_elem = size_buf;
			}
			else
				return 0;
		}
	 }

 size=lNext_elem-lLast_elem;//Размер элем. От начала до точки(если первый эл.)
			    //Или от точки до точки
 if(size>szMAX_elem){MessageBox(0,"\nСлишком большой элемент! Exit.\n","Message",0);exit(0);}


	//Read
	memset(_elem,0,szMAX_elem);
	memcpy(_elem,&in_buf[lLast_elem],size);
	_elem[size]=0;
	size++;//для 0

//Filter
//Find and cut bad symbol - cf,lf
	char reserv_elem[szMAX_elem];
	for (int f=0;f<size;f++)
	{
		if ((_elem[f]==13)|(_elem[f]==10))
		{
			memcpy(reserv_elem,&_elem[f+1],size-f);
			_elem[f]=0;
			size--;
			strncat(&_elem[f],reserv_elem,size);
			f--;//Проверить еще раз байт по текущему индексу (из-за сдвига)
		}
	}

	//Вырезание символов пробела и табуляции в начале буфера _elem   
	while( (_elem[0]==0x20)|(_elem[0]==9) )
	{
		size--;
		memcpy(reserv_elem,&_elem[1],size);
		memcpy(_elem,reserv_elem,size);
	}

	//CURENT BUFFER FIELD
//от начала файла или от конца ячейки Б.Д.(расположенной перед искомой)-
//первое поле - хим. формула молекулы

//начало поля - любой символ отличный от пробела и табуляции
//конец поля символ пробела или табуляции
	char *search0x9;
	char *search0x20;
	char *search;
 
	if((search0x9=strchr(_elem,0x9))!=0)
	{
		search=search0x9;					//найдена табуляция
		if((search0x20=strchr(_elem,0x20))!=0)	//найден символ пробела?
			if(search0x9>search0x20)		//табуляция позже пробела?
				search=search0x20;
	}
	else
	{	//ненайдена табуляция
		//if(!(search0x20=strchr(_elem,0x20)))//ненайден символ пробела?
		if((search0x20=strchr(_elem,0x20))==0)//ненайден символ пробела?
		{
			wsprintf(Mstr,"Неверно записана хим. формула у элемента %u! из Б.Д.",cur_elem);
			MessageBox(0,Mstr,"Error",0);exit(0);
		}
		else	//найден символ пробела ненайдена табуляция
			search=search0x20;
	}


	int nMoleculNameLength = search - &_elem[0];
	memset(_molecul,0,szMAX_name);
	strncpy(_molecul,_elem,nMoleculNameLength);

	//мы работаем с _elem[]
	size -= nMoleculNameLength;
	memcpy(reserv_elem,&_elem[nMoleculNameLength],size);
	memcpy(_elem,reserv_elem,size);			//перезапись


   //Вырезание символов пробела и табуляции в начале буфера _elem   
	while( (_elem[0]==0x20)|(_elem[0]==9) )
	{
		size--;
		memcpy(reserv_elem,&_elem[1],size);
		memcpy(_elem,reserv_elem,size);
	}
	search=&_elem[0];//предполагаемый адрес поля коэффициентов


	char conv_factor[szMAX_factor];//буфер для конвертирования
	char *pFactor;

//Второе поле - коэффициенты
	for(int cur_f=0;cur_f<nCoef;cur_f++)//Цикл по всем коэффициентами текущего
	{                               //элемемента
   		search0x9=0;
		search0x20=0;
		//поиск конца текущего коэффициента

//		if(search0x9=strchr(search,0x9))
		if((search0x9=strchr(search,0x9))!=0)
		{
			pFactor=search0x9;	//найдена табуляция
//			if((search0x20=strchr(search,0x20)))//найден символ пробела?
			if((search0x20=strchr(search,0x20))!=0)//найден символ пробела?
				if(search0x9>search0x20)//табуляция позже пробела?
					pFactor=search0x20;							
		}
		else
		{					//ненайдена табуляция
//			if(!(search0x20=strchr(search,0x20)))
			if((search0x20=strchr(search,0x20))==0)//ненайден символ пробела?
			{
				if(cur_f==(nCoef-1))//Если последний
				{
					if(_elem[size-1]==0)
					{
						pFactor=&_elem[size-1];
						goto last_f;
					}
				}
				wsprintf(Mstr,"Невозможно прочитать коэффициент №%u у конденс. элемента с именем %s!",cur_f+1,_molecul);
				MessageBox(0,Mstr,"Error",0);exit(0);
			}
			else	//найден символ пробела ненайдена табуляция
			{
				pFactor=search0x20;
			}
		}

last_f:
		int nFactorStrLength = pFactor - search;	//размер

		memset(conv_factor,0,szMAX_factor);
		strncpy(conv_factor,search,nFactorStrLength);
		if (strchr(conv_factor,','))
		{
			wsprintf(Mstr,"Разделитель целой и дробной части должен быть 'точкой'. Смотря коэффициент %u у элемента Б.Д. c именем '%u' ", cur_f+1 ,_molecul);
			MessageBox(0,Mstr,"Error",0);exit(0);
		}//ERROR

		__factor[cur_f]=atof(conv_factor);
		if(!__factor[cur_f])
		{
			wsprintf(Mstr,"Неверный коэффициент %u у элемента Б.Д. %u ", cur_f+1 ,cur_elem);
			MessageBox(0,Mstr,"Error",0);exit(0);
		}//ERROR

		//мы работаем с _elem[]
		size -= nFactorStrLength;
		memcpy(reserv_elem,&_elem[nFactorStrLength],size);
		memcpy(_elem,reserv_elem,size);			//перезапись

		//Вырезание символов пробела и табуляции в начале буфера _elem   
		while( (_elem[0]==0x20)|(_elem[0]==9) )
		{
			size--;
			memcpy(reserv_elem,&_elem[1],size);
			memcpy(_elem,reserv_elem,size);
		}
		search=&_elem[0];	//предполагаемый адрес поля коэффициентов
	}

//После последнего 9-ого коэф. может располаготься любая инфа
//но!: после символа пробела и до символов перевода строки и возврата каретки (0x0D,0x0A)
	int s1=strlen(search);

	if((s1!=0)&(s1<80))
		strcpy(_name_elem,search);
	else
		strcpy(_name_elem,"no name");

	return 1;				// success
}


// Найти все химические соединения в базе, которые содержат заданный атом.
// Для всех атомов переданных в функцию,исключая совпадения соединений.
// Заполнить структуры с названием и коэффициентами найденных соединений. /strFV/
// (необходимо, обраничить максимальное возможное количество структур
// исключая ошибку переполнения)
int GetPossibleChemCombinCond(chem_dataCond data[MaxElemCondFromDB], char pszNamesOfAtom[][MaxAtomNameLen],int nAtoms)
{
	char name_elem[szMAX_name+1];
	char molecule[szMAX_molecul];
	double 	factor[NumFactorsCond+2];	// intervals,t,as,ai,a1,a2,a3

	char strFV[MAX_len];
	char strFVLetter[MAX_len];
	memset(strFV,0,sizeof(strFV));
	memset(strFVLetter,0,sizeof(strFVLetter));

	int	nValidElemsFromDB=0;

	//Поиск по всей базе
//открытие файла
	HANDLE in=CreateFile(szChemdataCond,
			GENERIC_READ,FILE_SHARE_READ,0,OPEN_EXISTING,0,0);
	if(in==INVALID_HANDLE_VALUE)
	{	if ((strlen(szChemdataCond)+strlen("Unable to open file: "))>=nErrorMsgLength) return 0;
		wsprintf(cErrorMsg,"Unable to open file: %s",szChemdataCond);
		MessageBox(0,cErrorMsg,"Error",0);
		return -1;	}

	DWORD hi_fsize=0;
	const DWORD fsize=GetFileSize(in,&hi_fsize);
	if(hi_fsize)
		MessageBox(0,"Слишком велик размер Б.Д. (более 4 Гб :о ).","Warning!",0);

	HANDLE hFM=CreateFileMapping(in,0,PAGE_READONLY|SEC_COMMIT,0,fsize,0);
	if(!hFM)
	{	MessageBox(0,"Не могу получить идентификатор объекта-отображения файла Б.Д. на память (CreateFileMapping()).","Error. Exit.",0);
		return -1;	}

	char * lpFileMap=(char *)MapViewOfFile(hFM,FILE_MAP_READ,0,0,0);
	if(!lpFileMap)
	{	MessageBox(0,"Не могу сделать проекцию файла Б.Д. на память (MapViewOfFile()).","Error. Exit.",0);
		return -1;	}


//Поиск по всей базе

	// Для перезаписи молекулы из БД в список по атомам
	char pszElemsFromDB[MaxAtoms][MaxAtomNameLen];
	float fT[MaxAtoms];

	int nQuantityIntervals = 0;	// Still unknown
	BOOL bIsIntervals = TRUE;
	BOOL	bWasAnyValid;
	int nInterval = 0;
	int i_elem = -1;
	do
	{
		i_elem++;
		// Возмем элемент номер i_elem
		if(!ffind_elem(i_elem,&lpFileMap[0],fsize, molecule, name_elem,
				factor,NumFactorsCond+1+bIsIntervals))
			return nValidElemsFromDB;

		// Вырежем не литерные символы из формулы соединения
		int	f = 0;
		int	j = 0;
		int	curlenS = 0;
		int	curlenF = 0;
		float res;
		while(strlen(&molecule[f]))
		{
			curlenS = GetStr(&strFVLetter[j],&molecule[f]);
			j += curlenS;
			strFVLetter[j] = 0;
			f += curlenS;
			curlenF = GetUnsFactorFromStr(&res,&molecule[f]);
			f += curlenF;
			if((!curlenS)&&(!curlenF))
				break;					// Уже нет распознаваемых знаков
		}
		// Если встретились не литерные символы в молекуле из БД
		if((!curlenS)&&(!curlenF))
			continue;	// То взять новый элемент из БД

		// Перепишем молекулу из БД в список по атомам
		int nQuantAtomsFromDB =
			TransNameMolecToAandF(pszElemsFromDB,fT,molecule);
		// Если встретились нераспознаваемые символы в молекуле из БД
		if(!nQuantAtomsFromDB)
			continue;	// То взять новый элемент из БД

		bWasAnyValid = FALSE;
		//цикл по всем элементам хим. соединения из БД
		int nEq = 0;
		for (int k=0;k<nQuantAtomsFromDB;k++)
			for (int ext_i=0;ext_i<nAtoms;ext_i++)
				if (!strcmp(pszElemsFromDB[k],pszNamesOfAtom[ext_i]))
					nEq ++;

		if (nEq==nQuantAtomsFromDB) {	// Оно самое
			bWasAnyValid = TRUE;
			strcpy(data[nValidElemsFromDB].view,molecule);
			//strcpy(data[nValidElemsFromDB].name,name_elem);
			for(int i2=bIsIntervals;i2<NumFactorsCond+bIsIntervals;i2++)
				data[nValidElemsFromDB].factors[nInterval][i2-bIsIntervals]=factor[i2+1];
			data[nValidElemsFromDB].dTemperature[nInterval]=factor[bIsIntervals];
		}
			if (bIsIntervals) {
				nQuantityIntervals = (int)factor[0];
				if (nQuantityIntervals >= MaxTempIntervals) {
					wsprintf(Mstr,"Ошибка поиска в БД. \
						\n Конденс. соединение <%s> имеет более %d темпер. интервалов.",molecule,MaxTempIntervals);
					MessageBox(0,Mstr,"Message",0);
					return nValidElemsFromDB;
				}
				bIsIntervals = FALSE;
			}
			nInterval	++;
			if (nInterval>=nQuantityIntervals) {
				if (bWasAnyValid) {
					data[nValidElemsFromDB].nIntervals = nQuantityIntervals;
					nValidElemsFromDB++;
					bWasAnyValid = FALSE;
				}
				bIsIntervals = TRUE;
				nInterval = 0;
			}

			if(nValidElemsFromDB>=MaxElemCondFromDB) {
				wsprintf(Mstr,"Поиск в БД окончен.\
				\n Конденсированное соединение <%s>, не умещается в памяти, более %d.",molecule,MaxElemCondFromDB);
				MessageBox(0,Mstr,"Message",0);
				return nValidElemsFromDB;
			}

	}while(1);

	UnmapViewOfFile(lpFileMap);
	CloseHandle(in);
	return nValidElemsFromDB;
}


//Преобразование строки в массив атомов и коэф.(обратная функция ListMixtureToStr)
int TransNameMolecToAandF(char **pszAtoms,float *factor,char *szMolecule)
{
	unsigned int index=0;
	int count=0;
	int Len_atoms=0;				//Len_atom длинна распознанной строки с хим. обозн. атома
	char reserv[MAX_len];

	do
	{
		memset(&reserv,0,sizeof(reserv));
		// Переписать в 'reserv' литерную строку
		int gs = GetStr(reserv,&szMolecule[index]);//gs длинна строки с хим. формулой
		int number_elem = FindTrueAtomInStr(&pszAtoms[count],reserv,&Len_atoms);

		if (number_elem==0) {/*MessageBox(0,"\nBad element in szMolecule.","Error",0);*/return 0;}

		//проверить есть ли у тестируемой строки лишние символы после распознанных
		if (GetStr(reserv,&szMolecule[index+Len_atoms])!=0)
			{/*MessageBox(0,"Bad element in szMolecule.","Error",0);*/return 0;}

		index+=gs;
		int gf;
		for (int i=count;i<count+number_elem;i++) {
			factor[i] = 0;
		}
		gf = GetUnsFactorFromStr(&factor[count+number_elem-1],&szMolecule[index]);//for last finded element
		if((gf==0)&(strlen(szMolecule)>index))//не последний ?
				{MessageBox(0,"Неверный коэффициент","Error",0); return 0;}

/*	//factor[count-1+number_elem]=0;
	// Если number_elem больше одного то читаем коэффициент у последнего 
	int gf=GetUnsFactorFromStr(&factor[count-1+number_elem],&szMolecule[index]);//for last finded element
//у последнего атома последний коэффициент можно не вводить, он по умолчанию = 0(превращяясь потом в 1)
	if((gf==0)&(strlen(szMolecule)>index))//не последний ?
		{MessageBox(0,"Неверный коэффициент","Error",0); return 0;}*/
		index+=gf;
		count+=number_elem;
	}
	while( GetStr(reserv,&szMolecule[index]) );

	// factor 0 in factor 1//
	for(int i1=0;i1<count;i1++) {
		if(!factor[i1])
			factor[i1]=1;
	}
	//calc and //show:
	return count;
}

// Распознавание хим. периодических элементов из текстовой строки и
// запись их в массив
int FindTrueAtomInStr(char **dest,char *src_,int* size_a)//USE global: char *table[]
{
	*size_a=0;//init
	int len=strlen(src_);//!!!
	if (!len) {
		MessageBox(0,"FindTrueAtomInStr(): Пустая строка переданна в функцию.","Error",0);
		return 0;
	}
	if ( (len-1)>MAX_len ) {
		MessageBox(0,"FindTrueAtomInStr(): Длинна строки переданной в функцию больше 'MAX_len'.","Error",0);
		return 0;
	}

	int count=0,in_count=0;
	char *src=new char[len+1];
	char *s1=new char[len+1];
	char *s2=new char[len+1];
//Init
	memset(src,0,len+1);
	memset(s1,0,len+1);
	memset(s2,0,len+1);
	//!cout<<"\nInput data: "<<src_;

	for(int ext_i=0;ext_i<len;ext_i++)//По всей строке
	{
	//!cout<<"\nbegin with symbol="<<ext_i+1;
		for(int in_i=1;in_i<=(len-ext_i);in_i++)//Счетчик кол-ва символов
		{				       //которые будут взяты для распознавания
			strncpy(src,&src_[ext_i],strlen(&src_[ext_i]));
			src[in_i]='\0';
			for(int i=1;i<PERIOD_TABLE_size;i++) { //scan in table
				if(!strcmp(src,table[i])) { //найдено соответствие
					//!cout<<"\nfinded element: "<<src<<" serial number="<<i;
					//getch();
					//если s1 заполнено
					if(in_count==1) {
						strcpy(s2,src);
						goto end_scan;
					}
					else {            	// s1 не заполнено
						strcpy(s1,src);
						in_count++;
						break;	//закончить поиск в таблице
					}
				} //scan next symbol in src
			}
			//Основано на том что текстовое представление любого хим. эл-та не больше двух
			//Если число только что проверенных символов больше 1 и найдено соответствие
			if((in_i>1)&(in_count==1))goto end_scan;
			//Если число только что проверенных символов больше 1 и пока не найдено соответствие
			if((in_i>1)&(in_count==0)){//MessageBox(0,"Bad chimestry element. Exit.\n","Fatal error",0);
				return 0;}
			//попробуй ввести Fee
		}
//test and write
end_scan:
		if(!in_count)goto fail;//fail
		if(strlen(s1)>strlen(s2)) {//Выбирает самый длинный элемент
			//!cout<<"\nResult:"<<s1;
			dest[count]=new char[strlen(s1)+1];
			strcpy(dest[count],s1);
			if(strlen(s1)==2)
				{ext_i++;*size_a+=2;}
			else			
				*size_a+=1;

			count++;   //?
		}
		else {
		//!cout<<"\nResult:"<<s2;
			dest[count]=new char[strlen(s2)+1];
			strcpy(dest[count],s2);
			if(strlen(s2)==2)
				{ext_i++;*size_a+=2;}
			else			
				*size_a+=1;
			count++;   //?
		}
//Clear
fail:
		memset(src,0,len+1);
		memset(s1,0,len+1);
		memset(s2,0,len+1);
		in_count=0;
		//getch();
	}
	delete src;
	delete s1;
	delete s2;
	return count;//Возвращает кол-во найденных хим. элементов
}
