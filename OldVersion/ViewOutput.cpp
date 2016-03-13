// ViewOutput.cpp : implementation file
//

#include "stdafx.h"
#include <math.h>
#include "ThermoCalc.h"
#include "ViewOutput.h"
#include "MainFrm.h"
#include "Calc.h"
#include "all_excel.h"// _Application, Workbooks, Worksheets, _Worksheet, Range

extern BOOL bNotSelectedFont;
extern CHARFORMAT charf;
extern char cf[3];
char tab[1]={9};// "\t" for CString

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

/////////////////////////////////////////////////////////////////////////////
// CViewOutput

IMPLEMENT_DYNCREATE(CViewOutput, CRichEditView)

CViewOutput::CViewOutput(){}
CViewOutput::~CViewOutput(){}

BEGIN_MESSAGE_MAP(CViewOutput, CRichEditView)
	//{{AFX_MSG_MAP(CViewOutput)
	ON_COMMAND(ID_OUTPUT_SHORT, OnOutputShort)
	ON_UPDATE_COMMAND_UI(ID_OUTPUT_SHORT, OnUpdateOutputShort)
	ON_COMMAND(ID_OUTPUT_FULL, OnOutputFull)
	ON_UPDATE_COMMAND_UI(ID_OUTPUT_FULL, OnUpdateOutputFull)
	ON_COMMAND(ID_VIEW_PARTIALPRESSURES, OnViewPartialpressures)
	ON_UPDATE_COMMAND_UI(ID_VIEW_PARTIALPRESSURES, OnUpdateViewPartialpressures)
	ON_COMMAND(ID_VIEW_MOLESFRACTION, OnViewMolesfraction)
	ON_UPDATE_COMMAND_UI(ID_VIEW_MOLESFRACTION, OnUpdateViewMolesfraction)
	ON_COMMAND(ID_FILE_OPEN_IN_EXCEL, OnFileOpenInExcel)
	ON_COMMAND(ID_FILE_SAVE_TEXT, OnFileSaveText)
	//}}AFX_MSG_MAP
END_MESSAGE_MAP()

/////////////////////////////////////////////////////////////////////////////
// CViewOutput drawing

void CViewOutput::OnDraw(CDC* pDC)
{
	// TODO: add draw code here
//	CEdit* pCE = &GetEditCtrl( );
//	pCE->ReplaceSel(" Hello OnDraw()!");
}

/////////////////////////////////////////////////////////////////////////////
// CViewOutput diagnostics

#ifdef _DEBUG
void CViewOutput::AssertValid() const
{
	CRichEditView::AssertValid();
}

void CViewOutput::Dump(CDumpContext& dc) const
{
	CRichEditView::Dump(dc);
}
#endif //_DEBUG

/////////////////////////////////////////////////////////////////////////////
// CViewOutput message handlers

void CViewOutput::OnInitialUpdate()
{
	// TODO: Add your specialized code here and/or call the base class
//	CEdit* pCE = &GetEditCtrl( );
//	pCE->ReplaceSel("Hello Initial ! ");
	CRichEditCtrl *pE = &(GetRichEditCtrl( ));
	char szFaceName[MAX_len];
	CDC *dc = pE ->GetWindowDC();
	BOOL rv = dc ->GetTextFace(MAX_len-1,szFaceName);
	if(!bNotSelectedFont)
		pE ->SetDefaultCharFormat(charf);
	pE = &(GetRichEditCtrl( ));
	dc = pE ->GetWindowDC();
	rv = dc ->GetTextFace(MAX_len-1,szFaceName);
	dc = pE ->GetDC();
	rv = dc ->GetTextFace(MAX_len-1,szFaceName);
//wrap

	PARAFORMAT2 pf;
	pE ->GetParaFormat(pf);
//#define MAX_TAB_STOPS 32
//#define lDefaultTab 720
	pf.dwMask |= PFM_TABSTOPS;
	pf.cTabCount = MAX_TAB_STOPS;
	for(int i=0;i<MAX_TAB_STOPS;i++)
		pf.rgxTabs[i] = lDefaultTab*(i+1);	//lDefaultTab/3 = 4 пробела в табе

	CClientDC cdc(pE);
	rv = cdc.GetTextFace(MAX_len-1,szFaceName);
	float fTwipsPerInch = 1440;
	int nPixelsPerInch = cdc.GetDeviceCaps(LOGPIXELSX);
	pE->ReleaseDC(&cdc);
	float fConvertTwipsToPixels = (lDefaultTab / fTwipsPerInch) * nPixelsPerInch;

//	pf.dwMask |= PFM_OFFSET;
//	pf.dxOffset = 1440;
//	pf.cTabCount = 3;
//	pf.rgxTabs[0] = 1440/2;
//	pf.rgxTabs[0] = lDefaultTab;
//	pf.rgxTabs[0] = 150;
//	pf.rgxTabs[1] = 200;
//	pf.rgxTabs[2] = 200+150;
//	pf.rgxTabs[1] = 600;
//	pf.rgxTabs[2] = 300;
//	pf.rgxTabs[3] = 700;
//	pf.rgxTabs[4] = 300;
	SetParaFormat(pf);
	pE ->SetTargetDevice((HDC)NULL,1); // Что бы не заворачивать строки в окне (из REitp)
}

//,_MBCS
void CViewOutput::OnUpdate(CView* pSender, LPARAM lHint, CObject* pHint) 
{
	// TODO: Add your specialized code here and/or call the base class
//	CRichEditCtrl *pCE = &GetRichEditCtrl( );
//	CDocument* pDoc = GetDocument();
//	pCE->ReplaceSel(" Hello OnUpdate! + ");
//	pCE->ReplaceSel(pDoc ->GetTitle());
}

void CViewOutput::OnActivateView(BOOL bActivate, CView* pActivateView, CView* pDeactiveView) 
{
	// TODO: Add your specialized code here and/or call the base class	
	CRichEditView::OnActivateView(bActivate, pActivateView, pDeactiveView);
}

void CViewOutput::OnOutputFull() 
{
	// TODO: Add your command handler code here
	GetCurrentCalcDoc() ->pDlgView ->OnOutputFull();
}

void CViewOutput::OnUpdateOutputFull(CCmdUI* pCmdUI) 
{
	// TODO: Add your command update UI handler code here
	GetCurrentCalcDoc() ->pDlgView ->OnUpdateOutputFull(pCmdUI);
}

void CViewOutput::OnOutputShort() 
{
	// TODO: Add your command handler code here
	GetCurrentCalcDoc() ->pDlgView ->OnOutputShort();
}

void CViewOutput::OnUpdateOutputShort(CCmdUI* pCmdUI) 
{
	// TODO: Add your command update UI handler code here
	GetCurrentCalcDoc() ->pDlgView ->OnUpdateOutputShort(pCmdUI);
}

void CViewOutput::OnViewPartialpressures() {
	// TODO: Add your command handler code here
	GetCurrentCalcDoc() ->pDlgView ->OnViewPartialpressures();
}

void CViewOutput::OnUpdateViewPartialpressures(CCmdUI* pCmdUI) {
	// TODO: Add your command update UI handler code here
	GetCurrentCalcDoc() ->pDlgView ->OnUpdateViewPartialpressures(pCmdUI);
}

void CViewOutput::OnViewMolesfraction() {
	// TODO: Add your command handler code here
	GetCurrentCalcDoc() ->pDlgView ->OnViewMolesfraction();
}

void CViewOutput::OnUpdateViewMolesfraction(CCmdUI* pCmdUI) {
	// TODO: Add your command update UI handler code here
	GetCurrentCalcDoc() ->pDlgView ->OnUpdateViewMolesfraction(pCmdUI);
}

void CViewOutput::OnFileSaveText() {	GetCurrentCalcDoc() ->SaveAsText();}
void CViewOutput::OnFileOpenInExcel() {
// При присоединии к запущенному приложению могут возникнуть проблемы: делается нивидимой 
// клиентская область (авторы одной книги по DELPHI это знают, но не знают причины).
// Предполагаемые действия:
// Поиск запущенного Excel`я , если он найден то запрашивается его закрытие - 
// сначало программно, а потом с помощью пользователя (окно с сообщением 
// или диалоговое с возможности отмены работы с Excel).
// Потом следует ожидание завершения процесса Excel - 
// показывается диалоговое окно (может с прошедшим временем ожидания) 
// и кнопка отмены работы с расчетом в Excel`е. Ожидание так же может быть связано с 
// сохранением пользователем документа с которым ранее щла работа.
// Потом запуск Excel`я и присоединение.
// В принципе если открыть Excel сразу(очень быстро) после закрытия его пользователем,
// то пользователь может не заметить что только-что был открыт документ Excel с расчетом 
// и закрыть его так же как предыдущий документ.

	_Application m_app;
	Workbooks m_workbooks;
	Worksheets m_worksheets;
	_Worksheet m_worksheet;
	Range m_range[5];
	// если Excel уже запущен, подключаемся к нему; нет - запускаем его
	LPDISPATCH pDisp;
	LPUNKNOWN pUnk;
	CLSID clsid;
	TRACE("Входим в CEx25dView::OnExceloleLoad\n");
	BeginWaitCursor();
	//HRESULT r = ::CLSIDFromProgID(L"Excel.Application.10", &clsid); //из Реестра
	HRESULT r = ::CLSIDFromProgID(L"Excel.Application", &clsid); //из Реестра
// Ключ VersionIndependentProgID и его значение Excel.Application - для всех версий Excel.
	HRESULT r1 = S_OK;
	HRESULT r2 = CO_E_CLASSSTRING;
	HRESULT r3 = REGDB_E_WRITEREGDB;
	if(r1!=S_OK) {
		AfxMessageBox("Excel не найден");
		EndWaitCursor();
	}

	WCHAR ostr [30];
	HRESULT r4 = StringFromCLSID(clsid,(LPOLESTR *)&ostr);

	if(::GetActiveObject(clsid, NULL, &pUnk) == S_OK) {
		VERIFY(pUnk->QueryInterface(IID_IDispatch, (void**) &pDisp) == S_OK);
		m_app.AttachDispatch(pDisp);
		pUnk->Release();
		TRACE(" подсоединение завершено\n");
	}
	else {
		if(!m_app.CreateDispatch(clsid)) {
			AfxMessageBox("Ошибка создания объекта IDistpatch для Excel");
			EndWaitCursor();
			return;
		}
		TRACE("создание завершено\n");
	}
	EndWaitCursor();

	LPDISPATCH pRange;
	LPDISPATCH pWorkbooks;
	CWnd* pWnd = CWnd::FindWindow("XLMAIN",NULL);
	if (pWnd != NULL) {
		TRACE("OKHO Excel найдено\n");
		pWnd->ShowWindow(SW_SHOWNORMAL);
		pWnd->UpdateWindow( ) ;
		pWnd->BringWindowToTop();
	}

	COleVariant ov1((short)1);
	m_app.SetSheetsInNewWorkbook(1);
	VERIFY( pWorkbooks = m_app.GetWorkbooks( ));
	m_workbooks.AttachDispatch(pWorkbooks);	
	LPDISPATCH pWorkbook = NULL;
	if (m_workbooks.GetCount() == 0) {
		// Add возвращает указатель на Workbook, но у нас
		// нет класса Workbook
		VARIANT vaEmpty;
		vaEmpty.vt = VT_EMPTY;
		pWorkbook = m_workbooks.Add(vaEmpty); // Сохраняем указатель, чтобы освободить позже
	}

	LPDISPATCH pWorksheets = m_app.GetWorksheets();
	ASSERT(pWorksheets != NULL);
	m_worksheets.AttachDispatch(pWorksheets);
	LPDISPATCH pWorksheet = m_worksheets.GetItem(COleVariant((short) 1));
	m_worksheet.AttachDispatch(pWorksheet);
	m_worksheet.Select();

	VERIFY(pRange = m_worksheet.GetRange(COleVariant("A1")));
	Range rngTargetCorner;
	rngTargetCorner.AttachDispatch(pRange);
	Range rngReport;
	GetCurrentCalcDoc() ->WriteToExcelWorksheet(m_worksheet, rngTargetCorner,&rngReport);
/*
	VERIFY(pRange = m_worksheet.GetRange(COleVariant("A1")));
	m_range[0].AttachDispatch(pRange);
	VERIFY(pRange = m_worksheet.GetRange(COleVariant("A2")));
	m_range[1].AttachDispatch(pRange);
	VERIFY(pRange = m_worksheet.GetRange(COleVariant("A3")));
	m_range[2].AttachDispatch(pRange);
	VERIFY(pRange = m_worksheet.GetRange(COleVariant("A3"), COleVariant("C5")));
	m_range[3].AttachDispatch(pRange);
	VERIFY(pRange = m_worksheet.GetRange(COleVariant("A6")));
	m_range[4].AttachDispatch(pRange);

	m_range[4].SetValue(COleVariant(COleDateTime(1998, 4, 24, 15, 47, 8)));
	// получаем сохраненную дату и выводим ее как строку
	COleVariant vaTimeDate = m_range[4].GetValue();
	TRACE("возвращенный тип данных = Kd\n", vaTimeDate.vt);
	COleVariant vaTemp;
	vaTemp.ChangeType(VT_BSTR, &vaTimeDate);
	CString str = vaTemp.bstrVal;
	TRACE("дата = %s\n", (const char*) str);

	m_range[0].SetValue(COleVariant("Teкcтовая строка"));
	COleVariant vaResultO = m_range[0].GetValue();
	if (vaResultO.vt == VT_BSTR) {
		CString str = vaResultO.bstrVal;
		TRACE("vaBesultO = %s\n", (const char*) str);
	}

	m_range[1].SetValue(COleVariant(3.14159));
	COleVariant vaResultl = m_range[1].GetValue();
	if (vaResultl.vt == VT_R8) {
		TRACE("vaResultl = *f\n", vaResultl.dblVal);
	}

	m_range[2].SetFormula(COleVariant("=$A2*2.0"));
	COleVariant vaResult2 = m_range[2].GetValue();
	if (vaResult2.vt == VT_R8) {
		TRACE("vaResult2 = %f\n", vaResult2.dblVal);
	}

	COleVariant vaResult2a = m_range[2].GetFormula();
	if (vaResult2a.vt == VT_BSTR) {
		CString str = vaResult2a.bstrVal;
		TRACE("vaResult2a = %s\n", (const char*) str);
	}
	m_range[3].FillRight();
	m_range[3].FillDown();
*/

	// очистка
/*
	if (pWorksheet != NULL) {
		pWorksheet->Release( ) ;
	}

	if (pWorksheets != NULL) {
		pWorksheets->Release( ) ;
	}

	if (pWorkbook != NULL) {
		pWorkbook->Release( ) ;
	}
*/
	m_worksheet.ReleaseDispatch();
	m_worksheets.ReleaseDispatch();
	m_workbooks.ReleaseDispatch();
	m_app.ReleaseDispatch();
}

BOOL CViewOutput::PreCreateWindow(CREATESTRUCT& cs) {
	// TODO: Add your specialized code here and/or call the base class
//	CRichEditCtrl::LimitText(cnSizeExcelOutputText);
	return CRichEditView::PreCreateWindow(cs);
}

void CViewOutput::GetOptionsFromOutput(OPTIONSTRUCT &os) 
{
	CRichEditCtrl *pE = &(GetRichEditCtrl());
//	os.hdcOutput = pE ->GetDC()->m_hDC;
	os.hdcOutput = pE ->GetWindowDC() ->m_hDC;
//	os.hdcOutput = GetDC()->m_hDC;
//	os.hdcOutput = pE ->GetDCEx(DCX_CACHE);	//CS_CLASSDC
}

BOOL CViewOutput::Show(CString &str) 
{
	CRichEditCtrl *pE = &(GetRichEditCtrl( ));
	PARAFORMAT pf;
	pE ->GetParaFormat(pf);
//#define MAX_TAB_STOPS 32
//#define lDefaultTab 720
//	pf.dwMask |= PFM_TABSTOPS;
//	pf.dwMask |= PFM_OFFSET;
//	pf.dxOffset = 1440;
//	pf.cTabCount = 3;
//	pf.rgxTabs[0] = 1440/2;
//	pf.rgxTabs[0] = lDefaultTab;
//	pf.rgxTabs[0] = 150;
//	pf.rgxTabs[1] = 200;
//	pf.rgxTabs[2] = 200+150;
//	pf.rgxTabs[1] = 600;
//	pf.rgxTabs[2] = 300;
//	pf.rgxTabs[3] = 700;
//	pf.rgxTabs[4] = 300;
//	pE ->SetParaFormat(pf);

// Найти самую длинную строку
	int nMax = 0;
	int nCurr = 0;
	int nI;
	int nMaxI = 0;
	while((nI = str.Find(cf, nCurr))!= -1) {
		if(nMax < nI-nCurr) {
			nMax = nI-nCurr;
			nMaxI = nCurr;
		}
		nCurr = nI+1;
	}

	RECT r;
//	pE ->GetRect(&r);
//	r.right += 200;
//	pE ->SetRect(&r);

//	CWnd *pW = GetParent();
//	pW ->GetWindowRect(&r);
//	r.right += 100;
//	pW ->MoveWindow(&r);

	pE ->GetWindowRect(&r);
//	r.right += 200;
//	pE ->MoveWindow(&r);

	//if(bOneCalcOnDocument)// Добавить ???
	pE ->CWnd::SetWindowText("");	//Clear
	pE ->ReplaceSel(str);

	pE ->RequestResize();
	pE ->GetParaFormat(pf);
//	pE ->GetRect(&r);
//	r.right += 200;
//	pE ->SetRect(&r);
	return TRUE;
}

//(OPTIONSTRUCT &os,double P[],	UXFSTRUCT uxf[],type_ParamInCC &ParamInCC, type_CriticalSection &ParamInCriticalSection,type_Section ParamInSections[MAX_Sections],int nSections);
BOOL CViewOutput::Show(OPTIONSTRUCT &os, float P[], char szNam[MaxElemGasFromDB][MAX_len], int nPGas, int nPCond, int nPAll, unsigned int nMaxNamesLen, CALCSTRUCT &calcs,UXFSTRUCT uxf[], type_ParamInCC &ParamInCC, type_CriticalSection &ParamInCriticalSection, type_Section ParamInSections[], int nSections) 
{
	char szOut[MAX_len];
	int  decimal, sign;
	int	precision = 8;
	char *buffer;
	//char c[80];
	CString S;
	CString S2;
	float fSum;
	int cur_elem;
	CRichEditCtrl *pE = &(GetRichEditCtrl( ));

	pE ->ReplaceSel("Расчет КС");
	pE ->ReplaceSel( cf );

	for (int i =0;i<calcs.nEditBoxesAlfa;i++) {
		pE ->ReplaceSel(tab);
		wsprintf(szOut,"Альфа%u = ",i+1);
		pE ->ReplaceSel(szOut);
		S.Empty();
		ToDotString(ParamInCC.Alfa, S);
		pE ->ReplaceSel( S );
		pE ->ReplaceSel(";");
		pE ->ReplaceSel( cf );

		pE ->ReplaceSel(tab);
		pE ->ReplaceSel("УХФ: ");
		S.Empty();
		for (int i2=0;i2<uxf[i].nQuantityElemF;i2++) {
			S += uxf[i].szAtomsOfF[i2];
			ToDotString(uxf[i].fConditChimFormF[i2], S);
		}
		pE ->ReplaceSel( S );
		pE ->ReplaceSel(";");
		pE ->ReplaceSel( cf );

		pE ->ReplaceSel(tab);
		pE ->ReplaceSel("энтальпия УХФ: ");
		S.Empty();
		ToDotString(uxf[i].fEnthalpyF, S);
		pE ->ReplaceSel( S );
		pE ->ReplaceSel(";");
		pE ->ReplaceSel( cf );

		pE ->ReplaceSel( cf );

		pE ->ReplaceSel(tab);
		pE ->ReplaceSel(tab);
		pE ->ReplaceSel(tab);
		pE ->ReplaceSel(tab);
		for (int ip =0;ip<calcs.nEditBoxesPressure;ip++) {
			wsprintf(szOut,"P кс%u",ip+1);
			pE ->ReplaceSel(szOut);
			pE ->ReplaceSel(tab);
			pE ->ReplaceSel(tab);
			pE ->ReplaceSel(tab);
			pE ->ReplaceSel(tab);
			pE ->ReplaceSel(tab);
		}
		pE ->ReplaceSel( cf );

		pE ->ReplaceSel(tab);
		pE ->ReplaceSel(tab);
		pE ->ReplaceSel(tab);
		pE ->ReplaceSel(tab);
		S.Empty();
		ToDotString(ParamInCC.P_KAM, S);
		pE ->ReplaceSel( S );
		pE ->ReplaceSel(";");
		pE ->ReplaceSel( cf );

		pE ->ReplaceSel(tab);
		pE ->ReplaceSel(tab);
		pE ->ReplaceSel("Параметры КС");
		pE ->ReplaceSel( cf );

		pE ->ReplaceSel(tab);
		pE ->ReplaceSel(tab);
		pE ->ReplaceSel(tab);
		pE ->ReplaceSel(tab);
		pE ->ReplaceSel("Температура = ");
		S.Empty();
		ToDotString( ParamInCC.T, S);		//_gcvt();- Но она вставляет экспоненту если превышает сотые доли
		pE ->ReplaceSel( S );
		pE ->ReplaceSel(" К;");
		pE ->ReplaceSel( cf );
		S.Empty();

		pE ->ReplaceSel(tab);
		pE ->ReplaceSel(tab);
		pE ->ReplaceSel(tab);
		pE ->ReplaceSel(tab);
		pE ->ReplaceSel("Энтальпия = ");
		S.Empty();
		ToDotString(((float)ParamInCC.I), S);
		pE ->ReplaceSel( S );
		pE ->ReplaceSel(" кДж/кг;");
		pE ->ReplaceSel( cf );

		pE ->ReplaceSel(tab);
		pE ->ReplaceSel(tab);
		pE ->ReplaceSel(tab);
		pE ->ReplaceSel(tab);
		pE ->ReplaceSel("Давление = ");
		S.Empty();
		ToDotString(ParamInCC.P_KAM, S);
		pE ->ReplaceSel( S );
		pE ->ReplaceSel(" МПа;");
		pE ->ReplaceSel( cf );
	}





	for (int ia =0;ia<calcs.nEditBoxesAlfa;ia++) {
		for (int ip =0;ip<calcs.nEditBoxesPressure;ip++) {
	if ( os.nShowOption==cnFull ) {
//			ShowFull();
		switch (os.nPressureOption) {
			case cnPrM:
				pE ->ReplaceSel(cf);
				pE ->ReplaceSel(tab);
				pE ->ReplaceSel(tab);
				S.Empty();
				ToDotString(ParamInCC.Alfa, S);
				S2.Empty();
				ToDotString(ParamInCC.P_KAM, S2);
				wsprintf(szOut,"Состав ПГ	 при Альфа%u = %s и P кс%u = %s",
						ia+1,(LPCTSTR)S,ip+1,(LPCTSTR)S2);
				pE ->ReplaceSel(szOut);
				pE ->ReplaceSel(cf);
				pE ->ReplaceSel(tab);
				pE ->ReplaceSel(tab);
				pE ->ReplaceSel("Мольные доли (моль/кг), Astra view, <PrM>:");
				pE ->ReplaceSel(cf);
				// Astra view мольные доли <PrM> моль/кг
				for (cur_elem=1;cur_elem<=nPAll;cur_elem++) {
					strncpy(szOut, szNam[cur_elem], nMaxNamesLen+1);
					strncpy(&szOut[nMaxNamesLen+1], tab, 1);
//					char pc *strrchr(szOut,':');
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
						strcpy(&szOut[nMaxNamesLen+1+1],buffer);
					szOut[nMaxNamesLen+1+1+precision] = 0;
	
					if (decimal>0)
						strcpy(&szOut[strlen(szOut)],"+O");
					else
					strcpy(&szOut[strlen(szOut)],"-O");
	
					gcvt(abs(decimal),3,&szOut[strlen(szOut)]);
					// Show
					pE ->ReplaceSel(tab);
					pE ->ReplaceSel(tab);
					pE ->ReplaceSel(szOut);	//CEdit
					pE ->ReplaceSel(cf);
				}
			break;

			case cnPrP:
				pE ->ReplaceSel(cf);
				pE ->ReplaceSel(tab);
				pE ->ReplaceSel(tab);
				wsprintf(szOut,"Состав ПГ при Альфа%u = и P кс%u",ia+1,ip+1);
				pE ->ReplaceSel(szOut);
				pE ->ReplaceSel(cf);
				pE ->ReplaceSel(tab);
				pE ->ReplaceSel(tab);
				pE ->ReplaceSel("Парциальные давления (МПа), Astra view, <PrP>:");
				pE ->ReplaceSel(cf);
				fSum = 0;
				for (cur_elem=1;cur_elem<=nPAll;cur_elem++)
					if (P[cur_elem]>0)
						fSum += (float)P[cur_elem];
				// Astra view Парциальное давление <PrP> МПа
				for (cur_elem=1;cur_elem<=nPAll;cur_elem++) {
						strncpy(szOut, szNam[cur_elem], nMaxNamesLen+1);
					if (P[cur_elem]>0)
						buffer = _fcvt(ParamInCC.P_KAM*P[cur_elem]/fSum,precision*2,&decimal,&sign);
					else
						buffer = _fcvt(ParamInCC.P_KAM*exp(P[cur_elem])/fSum,precision*4,&decimal,&sign);
	
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
//		case nShort://			ShowShort();//			break;
	}
	}}
	return TRUE;
}

//BOOL CViewOutput::Show(OPTIONSTRUCT &os, double P[], char szNam[][MAX_len], int nPGas, int nPCond, int nPAll, unsigned int nMaxNamesLen, UXFSTRUCT uxf[], type_ParamInCC &ParamInCC, type_CriticalSection &ParamInCriticalSection, type_Section ParamInSections[], int nSections)
//{}
