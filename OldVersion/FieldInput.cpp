// FieldInput.cpp : implementation file
//

#include "stdafx.h"
#include "ThermoCalc.h"
#include "FieldInput.h"
#include "Calc.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

BOOL bNotSelectedFont = TRUE;
CHARFORMAT charf;
/////////////////////////////////////////////////////////////////////////////
// CRichEditCtrlChem

CRichEditCtrlChem::CRichEditCtrlChem() {
	memset(&cfSimple, 0, sizeof(cfSimple));
	cfSimple.dwMask = CFM_ITALIC|CFM_BOLD|CFM_UNDERLINE;
	cfSimple.dwEffects &= ~CFE_BOLD;
	memset(&cfITALIC, 0, sizeof(cfITALIC));
	cfITALIC.dwMask = CFM_ITALIC;
	cfITALIC.dwEffects = CFE_ITALIC;

	memset(&cfBOLD, 0, sizeof(cfBOLD));
	cfBOLD.dwMask = CFM_ITALIC|CFM_BOLD|CFM_UNDERLINE;
	cfBOLD.dwEffects = CFE_BOLD;

	memset(&cfITALIC_BOLD, 0, sizeof(cfITALIC_BOLD));
	cfITALIC_BOLD.dwMask = CFM_ITALIC|CFM_BOLD|CFM_UNDERLINE;
	cfITALIC_BOLD.dwEffects = CFE_ITALIC|CFE_BOLD;

	memset(&cfINVALID, 0, sizeof(cfINVALID));
	cfINVALID.dwMask = CFM_ITALIC|CFM_BOLD|CFM_UNDERLINE;
	cfINVALID.dwEffects = CFE_UNDERLINE;

	strObserveStr.Empty();
//	memset(szObserveStr, 0, sizeof(szObserveStr));
	// Ввод пользователя должен начинаться только с букв
	bMustBeLetter = TRUE;
	bWasDot = FALSE;
	lIndexDot = 0;
	memset(szTemp, 0, sizeof(szTemp));
}

CRichEditCtrlChem::~CRichEditCtrlChem() {	}

BOOL CRichEditCtrlChem::Create(DWORD dwStyle, const RECT& rect, CWnd* pParentWnd, UINT nID) {

	if(!CRichEditCtrl::Create(dwStyle, rect, pParentWnd, nID))
		return FALSE;

	if (bNotSelectedFont) {
		// !!! ??? EnumFontFamiliesEx();
		memset(&charf, 0, sizeof(charf));
		charf.dwMask |= CFM_BOLD | CFM_ITALIC;
		charf.dwEffects = CFE_BOLD;
		charf.dwMask |= CFM_SIZE;
		charf.yHeight = cnFontSizeRichEditCtrlChem *10*2;
		charf.dwMask |= CFM_FACE;
		charf.bPitchAndFamily = VARIABLE_PITCH|FF_SWISS;	//m_cf.lpLogFont->lfPitchAndFamily;
		strcpy(charf.szFaceName, szDefFontFaceName);	//		strcpy(charf.szFaceName, "Arial");
		charf.dwMask |= CFM_UNDERLINE | CFM_STRIKEOUT | CFM_COLOR;
		charf.dwEffects |= CFE_AUTOCOLOR;		//GetSysColor();
		charf.dwMask |= CFM_CHARSET;
		charf.bCharSet = RUSSIAN_CHARSET;
		bNotSelectedFont = FALSE;
	}
	SetDefaultCharFormat(charf);
	pParentWnd ->InvalidateRect(&rect);

	return TRUE;
}

void CRichEditCtrlChem::SetWindowTextChem(LPCTSTR lpszString) {
//	CHARFORMAT cf;
//	GetDefaultCharFormat(cf);
	if (!strlen(lpszString)) return;
	if (!strcmp(lpszString,"0")) {
		CWnd::SetWindowText(""); 
		ReplaceSel("0");
		SetSel(1, 1);
		SetSel(0, 1);
		SetSelectionCharFormat(cfITALIC_BOLD);
		return;
	}
	nQuantity = TransNameMolecToAandF(szAtoms, fFormF, (char *)lpszString);
	if (!nQuantity) return;
	CWnd::SetWindowText("");	//Clear
	int nCurPos = 0;
	int nLA = 0;
	int i;
	for (i=0;i<nQuantity;i++) {
		SetSel(nCurPos, nCurPos);
		nLA = strlen(szAtoms[i]);
		nLA += nCurPos;
		ReplaceSel(szAtoms[i]);
		nCurPos = nLA;

		strTemp.Empty();
		// Если коэффициент единица - не выводим
		if(fFormF[i] !=1) {
			if (!ToWholeNumberString(fFormF[i], strTemp))
				ToDotString(fFormF[i], strTemp);

			nLA += strTemp.GetLength();
			SetSel(nCurPos, nCurPos);
			ReplaceSel((LPCTSTR)strTemp);
		}
		nCurPos = nLA;
	}

	nCurPos = 0;
	nLA = 0;
	for (i=0;i<nQuantity;i++) {
		nLA = strlen(szAtoms[i]);
		nLA += nCurPos;
		// Выделить символы с именем хим. элемента
		SetSel(nCurPos, nLA);
		// Изменить шрифт выделенных символов
		SetSelectionCharFormat(cfSimple);
//		SetSelectionCharFormat(cfITALIC_BOLD);	//(cfSimple);
		nCurPos = nLA;

		strTemp.Empty();
		// Если коэффициент единица - не выводим
		if(fFormF[i] !=1) {
			if (!ToWholeNumberString(fFormF[i], strTemp))
				ToDotString(fFormF[i], strTemp);

			nLA += strTemp.GetLength();
			// Выделить символы с коэффициентом  хим. элемента
			SetSel(nCurPos, nLA);
			// Изменить шрифт выделенных символов
			SetSelectionCharFormat(cfBOLD);
//			SetSelectionCharFormat(cfITALIC);
			nCurPos = nLA;
		}
	}
}

void CRichEditCtrlChem::OnChar(UINT nChar, UINT nRepCnt, UINT nFlags) 
{
// Начинать ввод только с букв.
// после разделителя целых и десятичных разрядов должна быть цифра
// Если коэффициент единица - не выводим или выводим ??? !!!

	LPTSTR p;
	// Пропустить к выводу полученный символ ?
	char szForReplaceSel[2];
	char c = nChar;
	szForReplaceSel[0] = c;
	szForReplaceSel[1] = NULL;
	long lStartChar, lEndChar;
	long lStChDot, lEndChDot;

	if (Letter||Numeral||c=='.'||c==','||c == VK_BACK||c == VK_DELETE)
	{
		// Пропустить к выводу управляющий символ ?
		if (c == VK_BACK) {
		// Может стерется буква, число, или разделитель целых и десятичных разрядов
		// Если прошел управляющий символ то определить его влияние на
		//		bMustBeLetter		//		bWasDot
			CRichEditCtrl::OnChar(nChar, nRepCnt, nFlags);
			szForReplaceSel[0] = 0;	// Просто перекрасим шрифт
		}

		CWnd::GetWindowText(strObserveStr);
		if (bMustBeLetter && (!Letter) && !strObserveStr.GetLength()) {
			// Фильтруем числа, не нужные символы и не найденные в период. таблице символы
			// Работает только в начале ввода
			MessageBeep(MB_OK);
			return;
		}

		CWnd::GetWindowText(strTempObserveStr);
		GetSel(lStartChar, lEndChar);				// Сохраним положение курсора
		// Что бы самому не учитывать управляющие символы
		ReplaceSel(szForReplaceSel);				// Курсор стоит справа от только что выведенного символа

		// Работа с точкой
		if (bWasDot&&(Letter)) {
			GetSel(lStChDot, lEndChDot);
			if ( ((lEndChDot-1)==lIndexDot)&&(lStChDot==lEndChDot) ) {
				// Вернем все на место
				CWnd::SetWindowText(strTempObserveStr);
				SetSel(lStartChar, lEndChar);		// Востановим положение курсора
				MessageBeep(MB_OK);
				return;
			}
		}

		// Чтобы длина строки на превышала MAX_len
		CWnd::GetWindowText(strObserveStr);
		nLenAllStr = strObserveStr.GetLength();
		if (nLenAllStr >= MAX_len-1) {
			// Вернем все на место
			CWnd::SetWindowText(strTempObserveStr);
			SetSel(lStartChar, lEndChar);		// Востановим положение курсора
			MessageBeep(MB_OK);
			MessageBox("Длина вводимой строки превышает допустый предел!", "Error - CRichEditCtrlChem::OnChar()");
			return;
		}

		GetSel(lStartChar, lEndChar);		// Сохраним положение курсора
		if (nLenAllStr)
			bMustBeLetter = FALSE;
		else
			bMustBeLetter = TRUE;
		p = strObserveStr.GetBuffer(MAX_len);
		// Изменим запятую на точку
		if (ChangeCommaToDot(p)) {
			// Вернем все на место
			CWnd::SetWindowText(strTempObserveStr);
			SetSel(lStartChar, lEndChar);		// Востановим положение курсора
		}
		strcpy(szTempObserveStr,p);
		strObserveStr.ReleaseBuffer();

		nCurPos = nLA = 0;
		nLenCurrStr = 0;
		nLenCurrStrLetter = 0;
		memset(szTemp,0,sizeof(szTemp));
		do {
			// Переписать в 'reserv' литерную строку
			GetStr(szTemp,&szTempObserveStr[nLenCurrStr]);
			int number_elem = FindTrueAtomInStr(szAtoms, szTemp, &nLenCurrStrLetter);
			if (number_elem == -1) return;
/*			if (number_elem==0) {
				for (int i=nLenAllStr;i>(nLenCurrStr+1);i--) {	// укоратим строку с конца
					szTempObserveStr[i-1] = 0;
					GetStr(szTemp,&szTempObserveStr[nLenCurrStr]);
					number_elem = FindTrueAtomInStr(szAtoms, szTemp, &nLenCurrStrLetter);
				}
				// Востановим
				strObserveStr.GetBuffer(MAX_len);
				strcpy(szTempObserveStr,p);
				strObserveStr.ReleaseBuffer();//MessageBox(0,"TransNameMolecToAandF(): Unknown chemisry element in Molecule.","Error",0);
			}*/

			nCurPos = nLenCurrStr;
			for (int i2=0;i2<number_elem ;i2++) {
				nLA = strlen(szAtoms[i2]) + nCurPos;
				// Выделить символы с именем хим. элемента
				SetSel(nCurPos, nLA);
				// Изменить шрифт выделенных символов
				SetSelectionCharFormat(cfSimple);	//SetSelectionCharFormat(cfINVALID);
				nCurPos = nLA;
			}
			SetSel(lStartChar, lEndChar);		// Востановим положение курсора
			nLenCurrStr += nLenCurrStrLetter;

			// Если в конце числа стоит запятая то это нужно все таки пометить
			int uf = GetUnsFactorFromStr(&fFormF[0], &szTempObserveStr[nLenCurrStr]);
			if (!uf) {
				if (nLenCurrStr == nLenAllStr )
					return;								// Это конец строки
				else {
				// Значит после букв идет не число - а это нужно выделить
					// Найдем первый распознаваемый символ
					int nFirstValidSymbol = 0;
					number_elem = 0;
					uf = 0;
					for (int i=nLenCurrStr;i<(nLenAllStr);i++) {
						// Может найдем буквенную строку атомов
						GetStr(szTemp,&szTempObserveStr[i]);
						number_elem = FindTrueAtomInStr(szAtoms, szTemp, &nLenCurrStrLetter);
						if (number_elem) {
							nFirstValidSymbol = i;
							break;
						}
						// Может найдем число
						uf = GetUnsFactorFromStr(&fFormF[0], &szTempObserveStr[i]);
						if (uf) {
							nFirstValidSymbol = i;
							break;
						}
					}
					if (!nFirstValidSymbol) {
						// Выделить символы до конца строки
						SetSel(nLenCurrStr, -1);
						// Изменить шрифт выделенных символов
						SetSelectionCharFormat(cfINVALID);
						SetSel(lStartChar, lEndChar);		// Востановим положение курсора
						nLenCurrStr = nLenAllStr ;
						break;									// Дошли до конца строки
					}

					// Выделить символы
					SetSel(nLenCurrStr, nFirstValidSymbol );
					// Изменить шрифт выделенных символов
					SetSelectionCharFormat(cfINVALID);
					SetSel(lStartChar, lEndChar);		// Востановим положение курсора
					nLenCurrStr = nFirstValidSymbol;
					continue;
				}
			}

			nLA = nLenCurrStr;
			strTemp.Empty();
			if (!ToWholeNumberString(fFormF[0], strTemp))
				ToDotStringWOZ(fFormF[0], strTemp);
			nLA += strTemp.GetLength();
			// Выделить символы с коэффициентом  хим. элемента
			SetSel(nLenCurrStr, nLA);
			// Изменить шрифт выделенных символов
			SetSelectionCharFormat(cfBOLD);
			SetSel(lStartChar, lEndChar);		// Востановим положение курсора
			nLenCurrStr += uf;
		}
		while(nLenCurrStr < nLenAllStr );
		return;
	}
	MessageBeep(MB_OK);
	return;
}

/////////////////////////////////////////////////////////////////////////////
// CRichEditCtrlChem

BEGIN_MESSAGE_MAP(CRichEditCtrlChem, CRichEditCtrl)
	//{{AFX_MSG_MAP(CRichEditCtrlChem)
	ON_WM_CHAR()
	//}}AFX_MSG_MAP
END_MESSAGE_MAP()

/////////////////////////////////////////////////////////////////////////////
// CRichEditCtrlChem message handlers


/////////////////////////////////////////////////////////////////////////////
// FieldInput

FieldInput::FieldInput() { nItems = 0; nLeft =0; nTop = 0; }

FieldInput::~FieldInput() {
	for (int i=0;i<nItems;i++) {
		BOOL bR = EO[i].DestroyWindow();
		// Percent
		bR = EOP[i].DestroyWindow();
		// Percent spin
		bR = SpinO[i].DestroyWindow();
		// Enthalpy
		bR = EENT[i].DestroyWindow();
	}
}

BOOL FieldInput::Add(CWnd *pTCV, char *szFormul, char *szPercent, char *szEnthalpy) {
	if (nItems < MaxEditBoxesOx) {
		// Formul
		int top = nTop + nItems*(cnBoxCommonHigh + cnBoxCommonVSpace);
		RECT rect = {nLeft, top};
		rect.right = rect.left + cnBoxFormulWidth;
		rect.bottom = rect.top + cnBoxCommonHigh;
		DWORD dwS = WS_CHILD|WS_VISIBLE|WS_BORDER|ES_AUTOHSCROLL;
		BOOL bEC = EO[nItems].Create(dwS, rect, pTCV, ID_START_ID_O+nItems);
			// Font
/*		CFont fontTestl. fontTest2, fontTest3, fontTest4;
		fontTest1.CreateFont(50, 0, 0, 0, 400. FALSE, FALSE, 0, ANSI_CHARSET, OUT_DEFAULT_PRECIS,
			CLIP_DEFAULT_PRECIS. DEFAULT_QUALITY, DEFAULT_PITCH | FF_SWISS, "Arial");
		CFont* pOldFont = pDC->SelectObject(&fontTest1);		pDC->TextOut(0, 0, "Это Arial, ширина по умолчанию");
		fontTest2.CreateFont(50, 0, 0, 0, 400, FALSE, FALSE, 0, ANSI_CHARSET, OUT_DEFAULT_PRECIS,
			CLIP_DEFAULT_PRECIS, DEFAULT_QUALITY, OEFAULT_PITCH | FF_MODERN, "Courier");
		// не TrueType		pDC->TextOut(0, 100, "Это Courier, ширина по умолчанию");
		fontTest3.CreateFont(50, 10, 0, 0, 400, FALSE, FALSE, 0, ANSI.CHARSET, OUT_DEFAULT_PRECIS,
			CLIP_DEFAULT_PRECIS, DEFAULT_QUALITY, DEFAULT.PITCH | FF_ROHAN. NULL);
		pDC->TextOut(0, 200, "Это типовой Roman, переменная ширина"};
		fontTest4.CreateFont(50, 0, 0, 0, 400, FALSE, FALSE, 0, ANSI_CHARSET, OUT_DEFAULT_PRECIS,
			CLIP_DEFAULT_PRECIS, DEFAULTJJUALUY, DEFAULT_PITCH | FF_MOOERM, "LinePrinter");
		pDC->TextOut{0, 300, "Это LinePrinter, ширина по умолчанию");*/
/*		CHARFORMAT m_charformat;
		memset(&m_charformat, 0, sizeof(m_charformat));
		m_charformat.dwMask = CFM_ITALIC;					// CFM_BOLD;
		m_charformat.dwEffects = CFE_ITALIC;
		EO[nItems].SetDefaultCharFormat(m_charformat);*/

		EO[nItems].SetWindowTextChem(szFormul);
		pTCV ->InvalidateRect(&rect);

		// Percent
		rect.left = nLeft + cnBoxFormulWidth + cnBoxCommonHSpace;
		rect.top = nTop + nItems*(cnBoxCommonHigh + cnBoxCommonVSpace);
		rect.right = rect.left + cnBoxPercentWidth;
		rect.bottom = rect.top + cnBoxCommonHigh;
		bEC = EOP[nItems].Create(dwS|ES_NUMBER, rect, pTCV, ID_START_ID_OP+nItems);
		if(!bNotSelectedFont)
			EOP[nItems].SetDefaultCharFormat(charf);
		int nPercent = atoi(szPercent);
		if ((nPercent>100)||(nPercent<0))
			nPercent = 0;
		if (nPercent)
			EOP[nItems].SetWindowText(szPercent);
		else
			EOP[nItems].SetWindowText("0");

		pTCV ->InvalidateRect(&rect);

		// Percent spin
		rect.left = nLeft + cnBoxFormulWidth + cnBoxCommonHSpace + cnBoxPercentWidth;
		rect.top = nTop + nItems*(cnBoxCommonHigh + cnBoxCommonVSpace);
		rect.right = rect.left + cnSpinWidth;
		rect.bottom = rect.top + cnSpinHigh;
		bEC = SpinO[nItems].Create(WS_CHILD|WS_VISIBLE, rect, pTCV, ID_START_ID_OSPIN+nItems);
		SpinO[nItems].SetRange(0,100);
		SpinO[nItems].SetBase(1);
		if (nPercent)
			SpinO[nItems].SetPos(nPercent);
		else
			SpinO[nItems].SetPos(0);
		SpinO[nItems].SetBuddy(pTCV);
		pTCV ->InvalidateRect(&rect);

		// Enthalpy
		rect.left = nLeft + cnBoxFormulWidth + cnBoxPercentWidth + cnSpinWidth + cnBoxCommonHSpace*2 + 4;
		rect.top = nTop + nItems*(cnBoxCommonHigh + cnBoxCommonVSpace);
		rect.right = rect.left + cnBoxEnthalpyWidth;
		rect.bottom = rect.top + cnBoxCommonHigh;
		bEC = EENT[nItems].Create(dwS|ES_NUMBER, rect, pTCV, ID_START_ID_OENT+nItems);
		if(!bNotSelectedFont)
			EENT[nItems].SetDefaultCharFormat(charf);
		EENT[nItems].SetWindowText(szEnthalpy);
		pTCV ->InvalidateRect(&rect);

		nItems++;
		return TRUE;
	}
	else {
		MessageBeep(MB_OK);	//MB_ICONEXCLAMATION
		return FALSE;
	}
}

BOOL FieldInput::Add(CWnd *pTCV, CString &sFormul, CString &sPercent, CString &sEnthalpy) {
	char *pszF = sFormul.GetBuffer(MAX_len);
	char *pszP = sPercent.GetBuffer(MAX_len);
	char *pszE = sEnthalpy.GetBuffer(MAX_len);
	BOOL bR = Add(pTCV, pszF, pszP, pszE);
	sFormul.ReleaseBuffer();
	sPercent.ReleaseBuffer();
	sEnthalpy.ReleaseBuffer();
	return bR;
}

BOOL FieldInput::Add(CWnd *pTCV, CString &sFormul, int nPercent, CString &sEnthalpy) {
	char *pszF = sFormul.GetBuffer(MAX_len);
	char *pszE = sEnthalpy.GetBuffer(MAX_len);
	gcvt(nPercent, 3, szTemp);
	if(szTemp[strlen(szTemp) - 1] == '.')
		szTemp[strlen(szTemp) - 1] = 0;
	BOOL bR = Add(pTCV, pszF, szTemp, pszE);
	sFormul.ReleaseBuffer();
	sEnthalpy.ReleaseBuffer();
	return bR;
}

BOOL FieldInput::Add(CWnd *pTCV, char *pszFormul, int nPercent, char *pszEnthalpy) {
	gcvt(nPercent, 3, szTemp);
	if(szTemp[strlen(szTemp) - 1] == '.')
		szTemp[strlen(szTemp) - 1] = 0;
	BOOL bR = Add(pTCV, pszFormul, szTemp, pszEnthalpy);
	return bR;
}

BOOL FieldInput::Add(CWnd *pTCV) {
	char szFormul[] = "0";
	char szPercent[] = "0";
	char szEnthalpy[] = "0";
	return Add(pTCV, szFormul, 0, szEnthalpy);
}

/////////////////////////////////////////////////////////////////////////////
// FieldInput message handlers

BOOL FieldInput::Remove() {
	if (nItems > 1) {
		nItems--;
		BOOL bR = EO[nItems].DestroyWindow();
		// Percent
		bR = EOP[nItems].DestroyWindow();

		// Percent spin
		bR = SpinO[nItems].DestroyWindow();

		// Enthalpy
		bR = EENT[nItems].DestroyWindow();
		return TRUE;
	}
	else {
		MessageBeep(MB_OK);								// MB_ICONEXCLAMATION
		return FALSE;
	}
}

BOOL FieldInput::Scroll(UINT nSBCode, UINT nPos, CScrollBar *pScrollBar) {
	for (int i1=0;i1<nItems;i1++)
		if ((&SpinO[i1]) == (CSpinButtonCtrl *) pScrollBar) {
			switch(nSBCode)
			{
				// Scroll to the absolute position. The current position is provided in nPos.
				case SB_THUMBPOSITION:
					//nOxP[i1] = nPos;
					//SpinO[i1].SetPos(nOxP[i1]);
					gcvt((int)nPos,3,szTemp);
					if(szTemp[strlen(szTemp) - 1] == '.')
						szTemp[strlen(szTemp) - 1] = 0;
					EOP[i1].SetWindowText(szTemp);
					return TRUE;
				break;
			}
			break; // для for()
		}
	return FALSE;
}

BOOL FieldInput::SetFormulText(int nCurrentItem, char *psz) {
	if (nCurrentItem < nItems) {
		EO[nCurrentItem].SetWindowTextChem(psz);
		return TRUE;
	}
	return FALSE;
}

BOOL FieldInput::SetFormulText(int nCurrentItem, CString &sOx) {
	if (nCurrentItem < nItems) {
		EO[nCurrentItem].SetWindowText((LPCTSTR)sOx);
		return TRUE;
	}
	return FALSE;
}

BOOL FieldInput::SetPercentText(int nCurrentItem, char *psz) {
	if (nCurrentItem < nItems) {
		EOP[nCurrentItem].SetWindowText(psz);
		SpinO[nCurrentItem].SetPos(atoi(psz));
		return TRUE;
	}
	return FALSE;
}

BOOL FieldInput::SetPercentText(int nCurrentItem, CString &sOxP) {
	if (nCurrentItem < nItems) {
		EOP[nCurrentItem].SetWindowText((LPCTSTR)sOxP);
		char * p = sOxP.GetBuffer(10);
		SpinO[nCurrentItem].SetPos(atoi(p));
		sOxP.ReleaseBuffer();
		return TRUE;
	}
	return FALSE;
}

BOOL FieldInput::SetPercentText(int nCurrentItem, int &_nOxP) {
	if (nCurrentItem < nItems) {
		SpinO[nCurrentItem].SetPos(_nOxP);
		gcvt((int)_nOxP, 3, szTemp);
		if(szTemp[strlen(szTemp) - 1] == '.')
			szTemp[strlen(szTemp) - 1] = 0;
		EOP[nCurrentItem].SetWindowText(szTemp);
		return TRUE;
	}
	return FALSE;
}

BOOL FieldInput::SetEnthalpyText(int nCurrentItem, char *psz) {
	if (nCurrentItem < nItems) {
		EENT[nCurrentItem].SetWindowText(psz);
		return TRUE;
	}
	return FALSE;
}

BOOL FieldInput::SetEnthalpyText(int nCurrentItem, CString &_sEnth) {
	if (nCurrentItem < nItems) {
		EENT[nCurrentItem].SetWindowText((LPCTSTR)_sEnth);
		return TRUE;
	}
	return FALSE;
}

BOOL FieldInput::SetEnthalpyText(int nCurrentItem, int &_sEnth) {
	if (nCurrentItem < nItems) {
		gcvt((int)_sEnth, 3, szTemp);
		if(szTemp[strlen(szTemp) - 1] == '.')
			szTemp[strlen(szTemp) - 1] = 0;
		EENT[nCurrentItem].SetWindowText(szTemp);
		return TRUE;
	}
	return FALSE;
}

int FieldInput::GetCountItems()	{	return nItems;	}

BOOL FieldInput::GetFormul(int nCurrentItem, CString &_sOx) {
	if (nCurrentItem < nItems) {
		EO[nCurrentItem].GetWindowText(_sOx);
		return TRUE;
	}
	return FALSE;
}

BOOL FieldInput::GetPercent(int nCurrentItem, CString &_sOxP) {
	if (nCurrentItem < nItems) {
		EOP[nCurrentItem].GetWindowText(_sOxP);
		return TRUE;
	}
	return FALSE;	
}

BOOL FieldInput::GetEnthalpy(int nCurrentItem, CString &_sOxENT) {
	if (nCurrentItem < nItems) {
		EENT[nCurrentItem].GetWindowText(_sOxENT);
		return TRUE;
	}
	return FALSE;	
}

void FieldInput::Coordinate(int &_nLeft, int &_nTop)	{	nLeft = _nLeft;	nTop = _nTop;	}
void FieldInput::Coordinate(const int _nLeft, const int _nTop)	{	nLeft = _nLeft;	nTop = _nTop;	}

void FieldInput::EnableWindow(BOOL bEnable)
{
	for (int i =0; i<nItems; i++) {	
		EO[i].EnableWindow(bEnable);
		EOP[i].EnableWindow(bEnable);
		SpinO[i].EnableWindow(bEnable);
		EENT[i].EnableWindow(bEnable);
	}
}


/////////////////////////////////////////////////////////////////////////////
// FieldInputH
// По умолчанию создаются для ввода ряда значений
FieldInputH::FieldInputH() 
{	nItems = 0; nLeft =0; nTop = 0; bDefaultCharFormat = FALSE; 
	nMaxQuantityItems = MaxQuantityFIHBox;
	bMaybeZeroItems = FALSE;

	int i;
	for (i=0;i<MaxQuantityFIHBox;i++) {
		fSomeValueOfSteps[i] = 0.0;
		strSaveValue[i].Empty();
	}
	for (i=0;i<3;i++)
		strSaveSteps[i].Empty();
	uiTransformState = FieldInputH::TransStateIsValue;
	bStepsWasCalc = FALSE;
	nAmountOfSteps = 0;
}

const CString FieldInputH::strStep = "Шаг";
const CString FieldInputH::strStartValue = "Нач. знач.";
const CString FieldInputH::strEndValue = "Кон. знач.";
const int FieldInputH::nIndexStep = 0;
const int FieldInputH::nIndexStartValue = 1;
const int FieldInputH::nIndexEndValue = 2;

FieldInputH::~FieldInputH() {
	if (uiTransformState == FieldInputH::TransStateIsStep) 
		for (int i=0;i<3;i++)
			BOOL bR = FIH[i].DestroyWindow();
	else
		for (int i=0;i<nItems;i++)
			BOOL bR = FIH[i].DestroyWindow();
}

void FieldInputH::Coordinate(int &_nLeft, int &_nTop)	{
nLeft = _nLeft; nTop = _nTop; nWidth = cnBoxFIHWidth;
}
void FieldInputH::Coordinate(const int _nLeft, const int _nTop)	{
nLeft = _nLeft; nTop = _nTop; nWidth = cnBoxFIHWidth;
}
void FieldInputH::Coordinate(const int _nLeft, const int _nTop, const int _nWidth) 
{	nLeft = _nLeft;	nTop = _nTop;
	if (_nWidth)
		nWidth = _nWidth;	
	else
		nWidth = cnBoxFIHWidth;
}

void FieldInputH::Coordinate(const int _nLeft, const int _nTop, const int _nWidth,
		const int _MaxQuantityItems) 
{	nLeft = _nLeft;	nTop = _nTop;
	if (_nWidth)
		nWidth = _nWidth;	
	else
		nWidth = cnBoxFIHWidth;
	if (_MaxQuantityItems<=MaxQuantityFIHBox)
		nMaxQuantityItems = _MaxQuantityItems;
}

void FieldInputH::MaybeZeroItems(BOOL b) {	bMaybeZeroItems = b;	}

BOOL FieldInputH::Add(CWnd *pTCV) {
	if (uiTransformState == FieldInputH::TransStateIsStep) 
	{	MessageBeep(MB_ICONEXCLAMATION);	return FALSE;}

	if (nItems < nMaxQuantityItems) {
		int left = nLeft + nItems*(nWidth + cnBoxCommonHSpace);
		RECT rect = {left, nTop};
		rect.right = rect.left + nWidth;
		rect.bottom = rect.top + cnBoxCommonHigh;
		DWORD dwS = WS_CHILD|WS_VISIBLE|WS_BORDER|ES_AUTOHSCROLL|ES_NUMBER;
		BOOL bEC = FIH[nItems].Create(dwS, rect, pTCV, ID_START_ID_O+nItems);
		if (bDefaultCharFormat)
			FIH[nItems].SetDefaultCharFormat(cfDefaultCharFormat);
		FIH[nItems].SetWindowText("0");
		pTCV ->InvalidateRect(&rect);
		nItems++;
		return TRUE;
	}
	else {
		MessageBeep(MB_OK);	//MB_ICONEXCLAMATION
		return FALSE;
	}
}

BOOL FieldInputH::Remove() {
	if (uiTransformState == FieldInputH::TransStateIsStep) 
	{	MessageBeep(MB_ICONEXCLAMATION);	return FALSE;}

	if ((nItems > 0)&&( (nItems > 1)||(bMaybeZeroItems) )) {
		nItems--;
		BOOL bR = FIH[nItems].DestroyWindow();
		return TRUE;
	}
	else {
		MessageBeep(MB_OK);								// MB_ICONEXCLAMATION
		return FALSE;
	}
}

BOOL FieldInputH::SetText(int nCurrentItem, char *psz) {
	if (nCurrentItem < nItems) {
		FIH[nCurrentItem].SetWindowText(psz);
		return TRUE;
	}
	return FALSE;
}

BOOL FieldInputH::SetText(int nCurrentItem, CString &_s) {
	if (nCurrentItem < nItems) {
		//FIH[nCurrentItem].GetDefaultCharFormat(cfDefaultCharFormat);
		FIH[nCurrentItem].SetWindowText((LPCTSTR)_s);
		return TRUE;
	}
	return FALSE;
}

BOOL FieldInputH::SetText(int nCurrentItem, double &_d) {
	if (nCurrentItem < nItems) {
		gcvt(_d, 3, szTemp);
		if(szTemp[strlen(szTemp) - 1] == '.')
			szTemp[strlen(szTemp) - 1] = 0;
		FIH[nCurrentItem].SetWindowText(szTemp);
		return TRUE;
	}
	return FALSE;
}
BOOL FieldInputH::SetText(int nCurrentItem, const double &_d) {
	if (nCurrentItem < nItems) {
		gcvt(_d, 3, szTemp);
		if(szTemp[strlen(szTemp) - 1] == '.')
			szTemp[strlen(szTemp) - 1] = 0;
		FIH[nCurrentItem].SetWindowText(szTemp);
		return TRUE;
	}
	return FALSE;
}

int FieldInputH::GetCountItems()	{	return nItems;	}

BOOL FieldInputH::GetText(int nCurrentItem, CString &_s) {
	if (nCurrentItem < nItems) {
		FIH[nCurrentItem].GetWindowText(_s);
		return TRUE;
	}
	return FALSE;
}

BOOL FieldInputH::GetText(int nCurrentItem, char * _ps) {
	if (nCurrentItem < nItems) {
		FIH[nCurrentItem].GetWindowText(_ps, MAX_len);
		return TRUE;
	}
	return FALSE;
}

void FieldInputH::SetFont( CFont* pFont, BOOL bRedraw) {

	memset(&cfDefaultCharFormat, 0, sizeof(cfDefaultCharFormat));
	LOGFONT lf;
	memset(&lf,0,sizeof(LOGFONT));
	pFont ->GetLogFont(&lf);
	cfDefaultCharFormat.dwMask |= CFM_COLOR;
	cfDefaultCharFormat.dwEffects |= CFE_AUTOCOLOR;
	cfDefaultCharFormat.dwMask |= CFM_FACE;
	if (strlen(lf.lfFaceName))
//		strcpy(cfDefaultCharFormat.szFaceName, "System");
		strcpy(cfDefaultCharFormat.szFaceName, szDefFontFaceName);
//		strcpy(cfDefaultCharFormat.szFaceName, lf.lfFaceName);
	else
		strcpy(cfDefaultCharFormat.szFaceName, "System");

	cfDefaultCharFormat.dwMask |= CFM_CHARSET;
	if (lf.lfCharSet)
		cfDefaultCharFormat.bCharSet = lf.lfCharSet;
	else
		cfDefaultCharFormat.bCharSet = RUSSIAN_CHARSET;

	cfDefaultCharFormat.dwMask |= CFM_SIZE;
	if (lf.lfHeight) {//'1440 Twips=1 Inch
							// "Point" is a unit of measurement used in typography that is equal to 1/72 inch. 
							// "Twip" is a unit of measurement equal to 1/20th
							//	120 / Points = Pitch; For example: 10 point = 120 / 10 = 12 Pitch
//MM_TEXT//The default mapping mode is MM_TEXT. One logical unit equals one pixel.
//GetDeviceCaps(.. , LOGPIXELSY);
//LOGPIXELSY Number of pixels per logical inch along the screen height. 
//In a system with multiple display monitors, this value is the same for all monitors 
/*
    Function ConvertTwipsToPixels (lngTwips as Long, lngDirection _
    as long) As Long
    Dim lngDC as long                        'Handle to device
    Dim lngPixelsPerInch as Long
    Const nTwipsPerInch = 1440
    lngDC = GetDC(0)
    If (lngDirection = 0) Then       'Horizontal
        lngPixelsPerInch = GetDeviceCaps(lngDC, WU_LOGPIXELSX)
    Else                            'Vertical
        lngPixelsPerInch = GetDeviceCaps(lngDC, WU_LOGPIXELSY)
    End If
    lngDC = ReleaseDC(0, lngDC)
    ConvertTwipsToPixels = (lngTwips / nTwipsPerInch) * lngPixelsPerInch
    End Function
*/
		HDC hDC = ::GetDC(NULL);
		if (lf.lfHeight < 0)
			lf.lfHeight = -lf.lfHeight;
		int nnn = GetDeviceCaps(hDC, LOGPIXELSY);
		WORD wSize = (WORD)MulDiv(lf.lfHeight, 72, GetDeviceCaps(hDC, LOGPIXELSY));
		::ReleaseDC(NULL, hDC);
		//cfDefaultCharFormat.yHeight = lf.lfHeight;
		//wSize += 2;
		if (wSize<=cnFontSizeRichEditCtrlChem)
			cfDefaultCharFormat.yHeight = cnFontSizeRichEditCtrlChem *10*2;
		else
			cfDefaultCharFormat.yHeight = wSize *10*2;
	} else
		cfDefaultCharFormat.yHeight = cnDefFontSize *10*2;

	if (lf.lfPitchAndFamily)
		cfDefaultCharFormat.bPitchAndFamily = lf.lfPitchAndFamily;
	else
		cfDefaultCharFormat.bPitchAndFamily = VARIABLE_PITCH|FF_SWISS;

	if (lf.lfItalic) {
		cfDefaultCharFormat.dwMask |= CFM_ITALIC;
		cfDefaultCharFormat.dwEffects |= CFE_ITALIC;
	}

	if (lf.lfWeight>=FW_SEMIBOLD) {
		cfDefaultCharFormat.dwMask |= CFM_BOLD;
		cfDefaultCharFormat.dwEffects |= CFE_BOLD;
	}

	bDefaultCharFormat = TRUE;

	if (bRedraw) {
		for (int i=0;i<nItems;i++) {
			FIH[i].GetWindowText(strTemp);
			FIH[i].SetDefaultCharFormat(cfDefaultCharFormat);
			FIH[i].SetWindowText(strTemp);
		}
	}
	else
		for (int i=0;i<nItems;i++)
			FIH[i].SetDefaultCharFormat(cfDefaultCharFormat);
}

DWORD FieldInputH::GetDefaultCharFormat(CHARFORMAT &_cf) {
	if (!nItems) return 0;
	return FIH[0].GetDefaultCharFormat(_cf);
}

void FieldInputH::Transform(CWnd *pTCV, unsigned int _uiTransVariant) {
	if ( _uiTransVariant == uiTransformState )
		return;

	if (_uiTransVariant == FieldInputH::TransStateIsStep) {
// Создать три поля "ШАГ"
		// Сохранить строки значений
		int i;
		for (i=0;i<nItems;i++)
			FIH[i].GetWindowText(strSaveValue[i]);
		// Уничтожить старые поля ввода ряда значений
		for (i=0;i<nItems;i++)
			BOOL bR = FIH[i].DestroyWindow();
		// Создать три поля "ШАГ"
		int left;
		DWORD dwS;
		BOOL bEC;
		RECT rect;
		for (i=0;i<3;i++) {
			left = nLeft + (1+i*2)*(nWidth + cnBoxCommonHSpace);
			rect.left = left;
			rect.top = nTop;
			rect.right = rect.left + nWidth;
			rect.bottom = rect.top + cnBoxCommonHigh;
			dwS = WS_CHILD|WS_VISIBLE|WS_BORDER|ES_AUTOHSCROLL|ES_NUMBER;
			bEC = FIH[i].Create(dwS, rect, pTCV, ID_START_ID_O_STEP+i);
			if (bDefaultCharFormat)
				// Установить шрифт
				FIH[i].SetDefaultCharFormat(cfDefaultCharFormat);
			// Восстановить значения полей "ШАГ"
			FIH[i].SetWindowText(strSaveSteps[i]);
			pTCV ->InvalidateRect(&rect);
		}
		// Текст static text
		left = nLeft + 0*(nWidth + cnBoxCommonHSpace);
		rect.left = left;
		rect.top = nTop + long(cnBoxCommonHigh/(cnFontSizeStaticChem/1.6)); // в центр по вертикали
		rect.right = rect.left + nWidth;
		rect.bottom = rect.top + cnBoxCommonHigh;
		bEC = stStep.Create((LPCTSTR)strStep, SS_CENTER|WS_CHILD|WS_VISIBLE, rect, pTCV, NULL);
		pTCV ->InvalidateRect(&rect);
		left = nLeft + 2*(nWidth + cnBoxCommonHSpace);
		rect.left = left;
		rect.top = nTop;
		rect.right = rect.left + nWidth;
		rect.bottom = rect.top + cnBoxCommonHigh;
		bEC = stStartValue.Create((LPCTSTR)strStartValue, SS_CENTER|WS_CHILD|WS_VISIBLE, rect, pTCV, NULL);
		pTCV ->InvalidateRect(&rect);
		left = nLeft + 4*(nWidth + cnBoxCommonHSpace);
		rect.left = left;
		rect.top = nTop;
		rect.right = rect.left + nWidth;
		rect.bottom = rect.top + cnBoxCommonHigh;
		bEC = stEndValue.Create((LPCTSTR)strEndValue, SS_CENTER|WS_CHILD|WS_VISIBLE, rect, pTCV, NULL);
		pTCV ->InvalidateRect(&rect);

		uiTransformState = TransStateIsStep;
		bStepsWasCalc = FALSE;
	}
	else
	{
		// Сохранить строки "ШАГ"
		int i;
		for (i=0;i<3;i++)
			FIH[i].GetWindowText(strSaveSteps[i]);
		// Уничтожить старые поля ввода "ШАГ"
		for (i=0;i<3;i++)
			BOOL bR = FIH[i].DestroyWindow();
		stStep.DestroyWindow();
		stStartValue.DestroyWindow();
		stEndValue.DestroyWindow();

		// Создать поля ввода ряда значений, если они были созданы ранее
		int left;
		DWORD dwS;
		BOOL bEC;
		for (i=0;i<nItems;i++) {
			left = nLeft + i*(nWidth + cnBoxCommonHSpace);
			RECT rect = {left, nTop};
			rect.right = rect.left + nWidth;
			rect.bottom = rect.top + cnBoxCommonHigh;
			dwS = WS_CHILD|WS_VISIBLE|WS_BORDER|ES_AUTOHSCROLL|ES_NUMBER;
			bEC = FIH[i].Create(dwS, rect, pTCV, ID_START_ID_O+i);
			if (bDefaultCharFormat)
				// Установить шрифт
				FIH[i].SetDefaultCharFormat(cfDefaultCharFormat);
			// Восстановить значения полей
			FIH[i].SetWindowText(strSaveValue[i]);
			pTCV ->InvalidateRect(&rect);
		}
		uiTransformState = TransStateIsValue;
		bStepsWasCalc = FALSE;
	}
}

BOOL FieldInputH::CalcSteps() {	// Просчет полей "ШАГ" перед использованием

	char *p;
	int nFL;
	int nL;

	if (uiTransformState == FieldInputH::TransStateIsValue) 
		return FALSE;

// Сбор данных из полей "ШАГ"
	FIH[nIndexStep].GetWindowText(strTemp);
	nL = strTemp.GetLength();
	p = strTemp.GetBuffer(MAX_len);
	if (ChangeCommaToDot(p))
		FIH[nIndexStep].SetWindowText(p);
	nFL = GetFactorFromStr(&fStep,p);
	strTemp.ReleaseBuffer();
	if (!nFL) {
		MessageBox(FIH[nIndexStep].m_hWnd, "Bad 'Step' field!", "FieldInputH::CalcSteps() error",MB_ICONSTOP);
		bStepsWasCalc = FALSE;
		return FALSE;
	}
	if (nL > nFL) {
		MessageBox (FIH[nIndexStep].m_hWnd, "'Step' field have bad symbols!", "FieldInputH::CalcSteps() error",MB_ICONSTOP);
		bStepsWasCalc = FALSE;
		return FALSE;
	}
	if (fStep == 0) {
		MessageBox (FIH[nIndexStep].m_hWnd, "'Step' field must be not 0.", "FieldInputH::CalcSteps() error",MB_ICONSTOP);
		bStepsWasCalc = FALSE;
		return FALSE;
	}
	if (!nL) {
		MessageBox (FIH[nIndexStep].m_hWnd, "Please input 'Step' !", "FieldInputH::CalcSteps() error",MB_ICONSTOP);
		bStepsWasCalc = FALSE;
		return FALSE;
	}

	FIH[nIndexStartValue].GetWindowText(strTemp);
	nL = strTemp.GetLength();
	p = strTemp.GetBuffer(MAX_len);
	if (ChangeCommaToDot(p))
		FIH[nIndexStartValue].SetWindowText(p);
	nFL = GetUnsFactorFromStr(&fStartValue,p);
	strTemp.ReleaseBuffer();
	if (!nFL) {
		MessageBox(FIH[nIndexStartValue].m_hWnd, "Bad 'StartValue' field!", "FieldInputH::CalcSteps() error",MB_ICONSTOP);
		bStepsWasCalc = FALSE;
		return FALSE;
	}
	if (nL > nFL) {
		MessageBox (FIH[nIndexStartValue].m_hWnd, "'StartValue' field have bad symbols!", "FieldInputH::CalcSteps() error",MB_ICONSTOP);
		bStepsWasCalc = FALSE;
		return FALSE;
	}
	if (fStartValue == 0) {
		MessageBox (FIH[nIndexStartValue].m_hWnd, "'StartValue' field must be not 0.", "FieldInputH::CalcSteps() error",MB_ICONSTOP);
		bStepsWasCalc = FALSE;
		return FALSE;
	}
	if (!nL) {
		MessageBox (FIH[nIndexStartValue].m_hWnd, "Please input 'StartValue' !", "FieldInputH::CalcSteps() error",MB_ICONSTOP);
		bStepsWasCalc = FALSE;
		return FALSE;
	}

	FIH[nIndexEndValue].GetWindowText(strTemp);
	nL = strTemp.GetLength();
	p = strTemp.GetBuffer(MAX_len);
	if (ChangeCommaToDot(p))
		FIH[nIndexEndValue].SetWindowText(p);
	nFL = GetUnsFactorFromStr(&fEndValue,p);
	strTemp.ReleaseBuffer();
	if (!nFL) {
		MessageBox(FIH[nIndexEndValue].m_hWnd, "Bad 'EndValue' field!", "FieldInputH::CalcSteps() error",MB_ICONSTOP);
		bStepsWasCalc = FALSE;
		return FALSE;
	}
	if (nL > nFL) {
		MessageBox (FIH[nIndexEndValue].m_hWnd, "'EndValue' field have bad symbols!", "FieldInputH::CalcSteps() error",MB_ICONSTOP);
		bStepsWasCalc = FALSE;
		return FALSE;
	}
	if (fEndValue == 0) {
		MessageBox (FIH[nIndexEndValue].m_hWnd, "'EndValue' field must be not 0.", "FieldInputH::CalcSteps() error",MB_ICONSTOP);
		bStepsWasCalc = FALSE;
		return FALSE;
	}
	if (!nL) {
		MessageBox (FIH[nIndexEndValue].m_hWnd, "Please input 'EndValue' !", "FieldInputH::CalcSteps() error",MB_ICONSTOP);
		bStepsWasCalc = FALSE;
		return FALSE;
	}

// Подсчет
	float fCurrentV = fStartValue;
	int i;
	for (i=0;i<MaxQuantityFIHBox;i++) {
		fSomeValueOfSteps[i] = fCurrentV;
		fCurrentV += fStep;
		if (fCurrentV>fEndValue) break;
	}
	nAmountOfSteps = i;
	if (nAmountOfSteps == 0)
		return FALSE;
	bStepsWasCalc = TRUE;
	return TRUE;
}

unsigned int FieldInputH::GetTransformState() {return uiTransformState;}

int FieldInputH::GetAmountSteps() {
	if (!bStepsWasCalc) return 0;
	return nAmountOfSteps;
}

BOOL FieldInputH::GetSomeValueOfSteps(int nNumValue, float *pfV) {
	if (!bStepsWasCalc) return FALSE;
	if (nNumValue < nAmountOfSteps) {
		*pfV = fSomeValueOfSteps[nNumValue];
		return TRUE;
	}
	return FALSE;
}
