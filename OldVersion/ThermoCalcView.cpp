// ThermoCalcView.cpp : implementation of the CThermoCalcView class
//
#include "stdafx.h"
#include "ThermoCalc.h"

#include "ThermoCalcDoc.h"
#include "ThermoCalcView.h"
#include "ViewOutput.h"
#include "MainFrm.h"
#include "Calc.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

#include <io.h> 			//open, close
#include <fcntl.h>			//O_RDWR
#include <math.h>		// fabs();

// #pragma warning (disable : 4244)

char Mstr[256];//сообщение об ошибках
char cf[3]={13,10,0};// "\n" for CString
char szChemdataGas[]="dataGas_accuracy10.txt";
char szChemdataCond[]="dataCond_accuracy10.txt";
//char szChemdataGas[]="data_accuracy_short O H C).txt";
//char szChemdataGas[]="data_accuracy_short2 O H C).txt";
const CString CThermoCalcView :: cnstrUXF = "   УХФ топлива:";
const CString CThermoCalcView :: cnstrEnthalpyUXF = "Энтальпия    топлива:";

extern float P[MaxElemGasFromDB];

/////////////////////////////////////////////////////////////////////////////
// CThermoCalcView

IMPLEMENT_DYNCREATE(CThermoCalcView, CFormView)

BEGIN_MESSAGE_MAP(CThermoCalcView, CFormView)
	//{{AFX_MSG_MAP(CThermoCalcView)
	ON_BN_CLICKED(IDC_BUTTON2, OnButton2)
	ON_BN_CLICKED(IDC_BUTTON1, OnButtonCalc)
//	ON_COMMAND(ID_OUTPUT_FULL, OnOutputFull)
//	ON_UPDATE_COMMAND_UI(ID_OUTPUT_FULL, OnUpdateOutputFull)
//	ON_COMMAND(ID_OUTPUT_SHORT, OnOutputShort)
//	ON_UPDATE_COMMAND_UI(ID_OUTPUT_SHORT, OnUpdateOutputShort)
	ON_COMMAND(ID_VIEW_PARTIALPRESSURES, OnViewPartialpressures)
	ON_UPDATE_COMMAND_UI(ID_VIEW_PARTIALPRESSURES, OnUpdateViewPartialpressures)
	ON_COMMAND(ID_VIEW_MOLESFRACTION, OnViewMolesfraction)
	ON_UPDATE_COMMAND_UI(ID_VIEW_MOLESFRACTION, OnUpdateViewMolesfraction)
	ON_COMMAND(ID_FILE_SAVE_AS, OnFileSaveAs)
	ON_COMMAND(ID_FILE_SAVE, OnFileSave)
	ON_COMMAND(ID_FILE_CLOSE, OnFileClose)
	ON_BN_CLICKED(IDC_BUTTON_CALC_UXF, OnButtonCalcUxf)
	ON_BN_CLICKED(IDC_CHECK_CALC_UXF, OnCheckCalcOnlyWithUxf)
	ON_BN_CLICKED(IDC_ButtonAppendFormulO, OnButtonAppend_FormulO)
	ON_BN_CLICKED(IDC_ButtonDisFormulO, OnButtonDis_FormulO)
	ON_WM_VSCROLL()
	ON_BN_CLICKED(IDC_ButtonAppendAlfa, OnButtonAppend_Alfa)
	ON_BN_CLICKED(IDC_ButtonDisAlfa, OnButtonDis_Alfa)
	ON_BN_CLICKED(IDC_ButtonAppendPressure, OnButtonAppend_Pressure)
	ON_BN_CLICKED(IDC_ButtonDisPressure, OnButtonDis_Pressure)
	ON_BN_CLICKED(IDC_ButtonAppendFormulC, OnButtonAppend_FormulC)
	ON_BN_CLICKED(IDC_ButtonDisFormulC, OnButtonDis_FormulC)
	ON_BN_CLICKED(IDC_ButtonAppendPowerExpNozzle, OnButtonAppend_PowerExpNozzle)
	ON_BN_CLICKED(IDC_ButtonDisPowerExpNozzle, OnButtonDis_PowerExpNozzle)
	ON_BN_CLICKED(IDC_ButtonAppendPressureInSection, OnButtonAppend_PressureInSection)
	ON_BN_CLICKED(IDC_ButtonDisPressureInSection, OnButtonDis_PressureInSection)
	ON_BN_CLICKED(IDC_ButtonAppendIS, OnButtonAppend_IS)
	ON_BN_CLICKED(IDC_ButtonDisIS, OnButtonDis_IS)
	ON_BN_CLICKED(IDC_ButtonAppendV, OnButtonAppend_V)
	ON_BN_CLICKED(IDC_ButtonDisV, OnButtonDis_V)
	ON_BN_CLICKED(IDC_ButtonAppendEPS, OnButtonAppend_EPS)
	ON_BN_CLICKED(IDC_ButtonDisEPS, OnButtonDis_EPS)
	ON_BN_CLICKED(IDC_ButtonValueIS, OnButtonValue_IS)
	ON_BN_CLICKED(IDC_ButtonStepIS, OnButtonStep_IS)
	ON_WM_CREATE()
	ON_BN_CLICKED(IDC_CHECK_CRITIC_SECTION, OnCheckCriticSection)
	ON_BN_CLICKED(IDC_CHECK_ATMOSPHERE_SECTION, OnCheckAtmosphereSection)
	ON_COMMAND(ID_UNITMEASURE, OnIncudeUnitMeasure)
	ON_UPDATE_COMMAND_UI(ID_UNITMEASURE, OnUpdateIncudeUnitMeasure)
	ON_COMMAND(ID_MINNAMESINOUTPUT, OnMinNamesInOutput)
	ON_UPDATE_COMMAND_UI(ID_MINNAMESINOUTPUT, OnUpdateMinNamesInOutput)
	ON_COMMAND(ID_VIEW_ASTRAVIEW, OnViewAstraNumberView)
	ON_UPDATE_COMMAND_UI(ID_VIEW_ASTRAVIEW, OnUpdateViewAstraNumberView)
	ON_COMMAND(ID_VIEW_EXPONENTVIEW, OnViewExponentNumberView)
	ON_UPDATE_COMMAND_UI(ID_VIEW_EXPONENTVIEW, OnUpdateViewExponentNumberView)
	//}}AFX_MSG_MAP
	// Standard printing commands
	ON_COMMAND(ID_FILE_PRINT, CFormView::OnFilePrint)
	ON_COMMAND(ID_FILE_PRINT_DIRECT, CFormView::OnFilePrint)
	ON_COMMAND(ID_FILE_PRINT_PREVIEW, CFormView::OnFilePrintPreview)
END_MESSAGE_MAP()

/////////////////////////////////////////////////////////////////////////////
// CThermoCalcView construction/destruction

CThermoCalcView::CThermoCalcView()
	: CFormView(CThermoCalcView::IDD)
{
	//{{AFX_DATA_INIT(CThermoCalcView)
	strT = _T("");
	strMaxT = _T("");
	strMinT = _T("");
	//}}AFX_DATA_INIT
	// TODO: add construction code here

	// Нужна поддержка реестра, только сделать в PreCreate...()
	_nShowOption = cnFull;
	bEnableFull = TRUE;
	bEnableShort = FALSE;
	_nPressureOption = cnPrM;
	bMolesFraction = TRUE;
	bPartialPressures = FALSE;
	_nNumberView = cnExpNumV;
	bAstraNumberView = FALSE;
	bExponentNumberView = TRUE;
	os_nSizeTab = 12;
	_bIncludeUnitMeasOption = FALSE;
//	_bMinNamesInOutput = FALSE;!!!!!!!!!!!!!!!!
	_bMinNamesInOutput = TRUE;

	// Опции по умолчанию
	bCalcOnlyWithUxf = FALSE;
	bCalcCritSect = TRUE;
	bCalcAtmosphereSection = TRUE;
	fiFieldInputO.Coordinate(cnBoxFormulOLeft, cnBoxFormulOTop);
	fiFieldInputC.Coordinate(cnBoxFormulCLeft, cnBoxFormulCTop);
	fihAlfa.Coordinate(cnBoxAlfaLeft, cnBoxAlfaTop,0,4);
	fihPressures.Coordinate(cnBoxPressureLeft, cnBoxPressureTop, cnBoxPressureWidth,4);
//	nCurrentPageAlfa = 0;
}

CThermoCalcView::~CThermoCalcView() { }

void CThermoCalcView::DoDataExchange(CDataExchange* pDX) {
	CFormView::DoDataExchange(pDX);
	//{{AFX_DATA_MAP(CThermoCalcView)
	DDX_Control(pDX, IDC_ButtonDisFormulO, m_ButtonDisFormulO);
	DDX_Control(pDX, IDC_ButtonAppendFormulO, m_ButtonAppendFormulO);
	DDX_Control(pDX, IDC_ButtonDisFormulC, m_ButtonDisFormulC);
	DDX_Control(pDX, IDC_ButtonAppendFormulC, m_ButtonAppendFormulC);
	DDX_Control(pDX, IDC_ButtonDisPressure, m_ButtonDisPressure);
	DDX_Control(pDX, IDC_ButtonAppendPressure, m_ButtonAppendPressure);
	DDX_Control(pDX, IDC_ButtonDisAlfa, m_ButtonDisAlfa);
	DDX_Control(pDX, IDC_ButtonAppendAlfa, m_ButtonAppendAlfa);
	DDX_Control(pDX, IDC_BUTTON_CALC_UXF, m_ButtonCalcUxf);
	DDX_Text(pDX, IDC_Temperature, strT);
	DDX_Text(pDX, IDC_TemperatureMax, strMaxT);
	DDX_Text(pDX, IDC_TemperatureMin, strMinT);
	//}}AFX_DATA_MAP
}

BOOL CThermoCalcView::PreCreateWindow(CREATESTRUCT& cs) {
	// TODO: Modify the Window class or styles here by modifying
	//  the CREATESTRUCT cs
//	m_inputC1 = "CH";		//Edit box IDC_IO1
//	m_inputCP1 = "50";		//Edit box IDC_IOP1
//	m_inputC2 = "CH2";		//Edit box IDC_IO1
//	m_inputCP2 = "50";		//Edit box IDC_IOP1

/*
// O2+C
	sComb[0] = "C";
	sCombP[0] = "100";
	sCombENT[0] = "16";

	sOx[0] = "O2";
	sOxP[0] = "100";
	sOxENT[0] = "180.765";

	sAlfa[0] = "0.65";
	fAlfa[0] = (float)0.65;

	sPressures[0] = "0.1";
	fPressures[0] = (float)0.1;

	strT = "1200";
	strMaxT = "4000";
	strMinT = "0";
*/

//*
	sComb[0] = "H8N2C2";
	sCombP[0] = "50";
	sCombENT[0] = "774";
	sComb[1] = "N2H4";
	sCombP[1] = "50";
	sCombENT[1] = "1573";

	sOx[0] = "HNO3";
	sOxP[0] = "73";
	sOxENT[0] = "-2753";
	sOx[1] = "N2O4";
	sOxP[1] = "27";
	sOxENT[1] = "-209";
	sAlfa[0] = "0.85";
	fAlfa[0] = (float)0.85;
	sAlfa[1] = "0.9";
	fAlfa[1] = (float)0.9;

	// Горение
	sPressures[0] = "10";
	fPressures[0] = (float)10;
	sPressures[1] = "9";
	fPressures[1] = (float)9;
	sPressures[2] = "1";
	fPressures[2] = (float)1;

	strT = "2000";
	strMaxT = "4000";
	strMinT = "0";
//*/

	BOOL bR = CFormView::PreCreateWindow(cs);
//	cs.style = WS_CHILD;
//	cs.style |= WS_DLGFRAME;
//	cs.style = WS_CHILD | WS_BORDER;
//	cs.style = NULL;
//	cs.dwExStyle |= WS_EX_CLIENTEDGE;
//	cs.dwExStyle = WS_EX_CLIENTEDGE;
//	cs.dwExStyle = WS_EX_STATICEDGE;
//	cs.dwExStyle = WS_EX_STATICEDGE | WS_EX_CLIENTEDGE;
//	cs.dwExStyle = WS_EX_WINDOWEDGE;
	return  bR;
}

void CThermoCalcView::OnInitialUpdate() {
	CFormView::OnInitialUpdate();							// отобразим

	CHARFORMAT charf;
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
	CRichEditCtrl *pREC = (CRichEditCtrl *)GetDlgItem(IDC_Temperature);
	ASSERT(pREC);
	pREC ->SetDefaultCharFormat(charf);
	pREC = (CRichEditCtrl *)GetDlgItem(IDC_TemperatureMax);
	ASSERT(pREC);
	pREC ->SetDefaultCharFormat(charf);
	pREC = (CRichEditCtrl *)GetDlgItem(IDC_TemperatureMin);
	ASSERT(pREC);
	pREC ->SetDefaultCharFormat(charf);

	CFrameWnd* pCFWd = GetParentFrame();
	pCFWd -> RecalcLayout();
	ResizeParentToFit(FALSE);								// ScrollView
	//CWnd* pWCheck = GetDlgItem(IDC_CHECK_CALC_UXF);
	//pWCheck ->EnableWindow(FALSE);

	// Выставить опции по умолчанию
	CheckDlgButton(IDC_CHECK_CRITIC_SECTION, 1);
	CheckDlgButton(IDC_CHECK_ATMOSPHERE_SECTION, 1);
//	CWnd* pWCheck = GetDlgItem(IDC_CHECK_CRITIC_SECTION);
//	pWCheck ->EnableWindow(FALSE);

	LOGFONT lf;
	memset(&lf,0,sizeof(LOGFONT));
	strcpy(lf.lfFaceName, szDefFontFaceName);

/*
	CFontDialog fdFont( NULL, CF_SCREENFONTS, NULL, this);
	if (fdFont.DoModal() == IDOK) {
		fdFont.GetCurrentFont(&lf);
		CFont font;
		font.CreateFontIndirect(&lf);
		fihAlfa.SetFont(&font,FALSE);
		fihPowerExpNozzle.SetFont(&font,FALSE);
		fihPressures.SetFont(&font,FALSE);
	}
	else {
//*/
	HFONT hFont = (HFONT)::GetStockObject(DEFAULT_GUI_FONT);
//	HFONT hFont = (HFONT)::GetStockObject(SYSTEM_FONT);
	if (hFont == NULL)	hFont = (HFONT)::GetStockObject(SYSTEM_FONT);
	if ((hFont != NULL) && (::GetObject(hFont, sizeof(LOGFONT), &lf) != 0)) {
		CFont *pF = CFont::FromHandle(hFont);
		ASSERT(pF);
//		SetFont(pF,TRUE);
		fihAlfa.SetFont(pF,FALSE);
		fihPowerExpNozzle.SetFont(pF,FALSE);
		fihPressures.SetFont(pF,FALSE);
		fihPressureInSection.SetFont(pF,FALSE);
		fihV.SetFont(pF,FALSE);
		fihEPS.SetFont(pF,FALSE);
	} else {
		CFont *pCurrentF = GetFont( );
		ASSERT(pCurrentF);
//		SetFont(pCurrentF,TRUE);
		fihAlfa.SetFont(pCurrentF,FALSE);
		fihPowerExpNozzle.SetFont(pCurrentF,FALSE);
		fihPressures.SetFont(pCurrentF,FALSE);
		fihPressureInSection.SetFont(pCurrentF,FALSE);
		fihV.SetFont(pCurrentF,FALSE);
		fihEPS.SetFont(pCurrentF,FALSE);
	}
//	}

	if (!sOx[0].IsEmpty()) {
/*fiFieldInputO.Add(this, "H1", "51", "-1"); fiFieldInputO.Add(this, "H2", 52, "-2"); sOx[0] = "H3"; sOxENT[0] = "-3";
fiFieldInputO.Add(this,(CString &)sOx[0],53,sOxENT[0]);sOx[0] = "H4";sOxP[0] = "54";sOxENT[0] = "-4";fiFieldInputO.Add(this, sOx[0], sOxP[0], sOxENT[0]);*/
		fiFieldInputO.Add(this, sOx[0], sOxP[0], sOxENT[0]);
	}
	else
		fiFieldInputO.Add(this);

	int i;
	for (i=1;i<MaxEditBoxesOx;i++)
		if (!sOx[i].IsEmpty())
			fiFieldInputO.Add(this, sOx[i], sOxP[i], sOxENT[i]);


	if (!sComb[0].IsEmpty())
		fiFieldInputC.Add(this, sComb[0], sCombP[0], sCombENT[0]);
	else
		fiFieldInputC.Add(this);

	for (i=1;i<MaxEditBoxesOx;i++)
		if (!sComb[i].IsEmpty())
			fiFieldInputC.Add(this, sComb[i], sCombP[i], sCombENT[i]);

	fihAlfa.Add(this);
	if (!sAlfa[0].IsEmpty()) {
		fihAlfa.SetText(0,sAlfa[0]);
	}
	if (!sAlfa[1].IsEmpty()) {
		fihAlfa.Add(this);
		fihAlfa.SetText(1,sAlfa[1]);
	}

	fihPressures.Add(this);
	if (!sPressures[0].IsEmpty()) {
		fihPressures.SetText(0,sPressures[0]);
	}

	if (!sPressures[1].IsEmpty()) {
		fihPressures.Add(this);
		fihPressures.SetText(1,sPressures[1]);
	}

	if (!sPressures[2].IsEmpty()) {
		fihPressures.Add(this);
		fihPressures.SetText(2,sPressures[2]);
	}

	DrawEmptyPSUXF();
	CreatePSCalcFlow();

	SetFocus();
}

/////////////////////////////////////////////////////////////////////////////
// CThermoCalcView printing

BOOL CThermoCalcView::OnPreparePrinting(CPrintInfo* pInfo)
{
	// default preparation
	return DoPreparePrinting(pInfo);
}

void CThermoCalcView::OnBeginPrinting(CDC* /*pDC*/, CPrintInfo* /*pInfo*/)
{
	// TODO: add extra initialization before printing
}

void CThermoCalcView::OnEndPrinting(CDC* /*pDC*/, CPrintInfo* /*pInfo*/)
{
	// TODO: add cleanup after printing
}

void CThermoCalcView::OnPrint(CDC* pDC, CPrintInfo* /*pInfo*/)
{
	// TODO: add customized printing code here
}

/////////////////////////////////////////////////////////////////////////////
// CThermoCalcView diagnostics

#ifdef _DEBUG
void CThermoCalcView::AssertValid() const
{
	CFormView::AssertValid();
}

void CThermoCalcView::Dump(CDumpContext& dc) const
{
	CFormView::Dump(dc);
}

DlgEmptyDoc* CThermoCalcView::GetDocument() // non-debug version is inline
{
	ASSERT(m_pDocument->IsKindOf(RUNTIME_CLASS(DlgEmptyDoc)));
	return (DlgEmptyDoc*)m_pDocument;
}
#endif //_DEBUG

/////////////////////////////////////////////////////////////////////////////
// CThermoCalcView message handlers

void CThermoCalcView::OnButton2() 
{
	// TODO: Add your control notification handler code here
//	CWnd * pCW = AfxGetMainWnd();
//	pCW -> DestroyWindow ( );
	PostQuitMessage(0);
}

void CThermoCalcView::OnButtonCalc() {
/*	CThermoCalcDoc *pTCD = GetCurrentCalcDoc();
	if (!pTCD) {
		pTCD = (CThermoCalcDoc *)( ((CMainFrame*)AfxGetMainWnd()) ->
			GetOutputDocTemplate() ->OpenDocumentFile(NULL) );
		if (!pTCD) return;
	}*/

// Объект - создание или использование текущего
	CThermoCalcDoc *pTCD = GetCurrentCalcDoc();
	if (!pTCD) {
		// Так как низвестно удачно ли пойдет подготовка данных к расчету,
		// а все нужные методы находятся в объекте документа
		// Открыть новый невидимый документ
		// Внимание: m_bEmbedded == TRUE - document is being created by OLE
		pTCD = (CThermoCalcDoc *)( ((CMainFrame*)AfxGetMainWnd()) ->
			GetOutputDocTemplate() ->OpenDocumentFile(NULL,FALSE) );
		if (!pTCD) return;
		pTCD ->SetInvisibleFlag();
		// Вывести на передний план окно с диалогом
		CChildFrame * pCF =(CChildFrame *) GetParentFrame();
		if ( ((CMDIFrameWnd *)AfxGetMainWnd()) ->MDIGetActive() != pCF)
			((CMDIFrameWnd *)AfxGetMainWnd()) ->MDIActivate(pCF);
	}

// Подготовка данных о топливе к расчету
	UXFSTRUCT * pUXF = pTCD ->GetpUXF();
	CALCSTRUCT &cs = pTCD ->GetCS();
	// Если нужно считать формулы ок. и гор. и расч УХФ
	if (!bCalcOnlyWithUxf) {
		if (!FillCalcsStruct(cs)) {
			CWnd* pWCheck = GetDlgItem(IDC_CHECK_CALC_UXF);
			pWCheck ->EnableWindow(FALSE);
			return;
		}

		// Расчитать УХФ
		if (!pTCD ->CalcUXF())
			return;

		ShowUXF(pUXF);
		CWnd* pWCheck = GetDlgItem(IDC_CHECK_CALC_UXF);
		pWCheck ->EnableWindow(TRUE);
	}
	else {
	// Считаем из окна УХФ и передадим ее в документ
		if(!GetUXFFromBox(pUXF))
			return;
		pTCD ->SetValidUXF();
	}

// Подготовка данных для процесса горения
	// ...
// Подготовка данных для процесса течения
	// ...
	if(bCalcCritSect)
		cs.bIncludeCritSect = TRUE;
	else
		cs.bIncludeCritSect = FALSE;
	if(bCalcAtmosphereSection)
		cs.bIncludeAtmosphereSection = TRUE;
	else
		cs.bIncludeAtmosphereSection = FALSE;

// Вызов метода объекта
	if (! (pTCD -> CalcAndShow()) ) {
		pTCD ->OnCloseDocument();	// CDocument
		return;
	}
	pTCD ->SetFocus_Output();
}

void CThermoCalcView::OnButtonCalcUxf() {
	// TODO: Add your control notification handler code here

	CThermoCalcDoc *pTCD = GetCurrentCalcDoc();
	if (!pTCD) {
		// Открыть новый невидимый документ
		// Внимание: m_bEmbedded == TRUE - document is being created by OLE
		pTCD = (CThermoCalcDoc *)( ((CMainFrame*)AfxGetMainWnd()) ->
			GetOutputDocTemplate() ->OpenDocumentFile(NULL,FALSE) );
		if (!pTCD) return;
		pTCD ->SetInvisibleFlag();
		// Вывести на передний план окно с диалогом
		CChildFrame * pCF =(CChildFrame *) GetParentFrame();
		if ( ((CMDIFrameWnd *)AfxGetMainWnd()) ->MDIGetActive() != pCF)
			((CMDIFrameWnd *)AfxGetMainWnd()) ->MDIActivate(pCF);
	}
	CALCSTRUCT &cs = pTCD ->GetCS();

	if (!FillCalcsStruct(cs)) {
		CWnd* pWCheck = GetDlgItem(IDC_CHECK_CALC_UXF);
		pWCheck ->EnableWindow(FALSE);
		return;
	}


	if (!pTCD ->CalcUXF())
		return;

	UXFSTRUCT * pUXF = pTCD ->GetpUXF();
	ShowUXF(pUXF);
	CWnd* pWCheck = GetDlgItem(IDC_CHECK_CALC_UXF);
	pWCheck ->EnableWindow(TRUE);

	return;
}

void CThermoCalcView::GetOptionsFromDlg(OPTIONSTRUCT &os) {
	os.nShowOption = _nShowOption;
	os.nPressureOption = _nPressureOption;
	os.nNumberView = _nNumberView;
	os.nSizeTab = os_nSizeTab;
	os.bIncludeUnitMeasOption = _bIncludeUnitMeasOption;
	os.bMinNamesInOutput = _bMinNamesInOutput;
}

void CThermoCalcView::OnOutputFull() {
	// TODO: Add your command handler code here
	if (!bEnableFull) {
		_nShowOption = cnFull;
		bEnableFull = bEnableFull^1;
		bEnableShort = bEnableShort^1;
	}
}

void CThermoCalcView::OnUpdateOutputFull(CCmdUI* pCmdUI) {
	// TODO: Add your command update UI handler code here
	if (bEnableFull)
		pCmdUI ->SetCheck(1);
	else
		pCmdUI ->SetCheck(0);
}

void CThermoCalcView::OnOutputShort() {
	// TODO: Add your command handler code here
	if (!bEnableShort) {
		_nShowOption = cnShort;
		bEnableShort = bEnableShort^1;
		bEnableFull = bEnableFull^1;
	}
}

void CThermoCalcView::OnUpdateOutputShort(CCmdUI* pCmdUI) {
	// TODO: Add your command update UI handler code here
	if (bEnableShort)
		pCmdUI ->SetCheck(1);
	else
		pCmdUI ->SetCheck(0);
}

void CThermoCalcView::OnIncudeUnitMeasure() {
	// TODO: Add your command handler code here
	_bIncludeUnitMeasOption = _bIncludeUnitMeasOption^1;
}

void CThermoCalcView::OnUpdateIncudeUnitMeasure(CCmdUI* pCmdUI) {
	// TODO: Add your command update UI handler code here
	if (_bIncludeUnitMeasOption)
		pCmdUI ->SetCheck(1);
	else
		pCmdUI ->SetCheck(0);	
}


void CThermoCalcView::OnViewPartialpressures() {
	// TODO: Add your command handler code here
	if (!bPartialPressures) {
		_nPressureOption = cnPrP;
		bPartialPressures = bPartialPressures^1;
		bMolesFraction = bMolesFraction^1;
	}
}

void CThermoCalcView::OnUpdateViewPartialpressures(CCmdUI* pCmdUI) {
	// TODO: Add your command update UI handler code here
	if (bPartialPressures)
		pCmdUI ->SetCheck(1);
	else
		pCmdUI ->SetCheck(0);	
}

void CThermoCalcView::OnViewMolesfraction() {
	// TODO: Add your command handler code here
	if (!bMolesFraction) {
		_nPressureOption = cnPrM;
		bMolesFraction = bMolesFraction^1;
		bPartialPressures = bPartialPressures^1;
	}
}

void CThermoCalcView::OnUpdateViewMolesfraction(CCmdUI* pCmdUI) {
	// TODO: Add your command update UI handler code here
	if (bMolesFraction)
		pCmdUI ->SetCheck(1);
	else
		pCmdUI ->SetCheck(0);
}

void CThermoCalcView::OnViewAstraNumberView() {
	// TODO: Add your command handler code here
	if (!bAstraNumberView) {
		_nNumberView = cnASTRANumV;
		bAstraNumberView = bAstraNumberView^1;
		bExponentNumberView = bExponentNumberView^1;
	}	
}

void CThermoCalcView::OnUpdateViewAstraNumberView(CCmdUI* pCmdUI) {
	// TODO: Add your command update UI handler code here
	if (bAstraNumberView)
		pCmdUI ->SetCheck(1);
	else
		pCmdUI ->SetCheck(0);	
}

void CThermoCalcView::OnViewExponentNumberView() {
	// TODO: Add your command handler code here	
	if (!bExponentNumberView) {
		_nNumberView = cnExpNumV;
		bAstraNumberView = bAstraNumberView^1;
		bExponentNumberView = bExponentNumberView^1;
	}	
}

void CThermoCalcView::OnUpdateViewExponentNumberView(CCmdUI* pCmdUI) {
	// TODO: Add your command update UI handler code here	
	if (bExponentNumberView)
		pCmdUI ->SetCheck(1);
	else
		pCmdUI ->SetCheck(0);	
}

void CThermoCalcView::OnMinNamesInOutput() {
	// TODO: Add your command handler code here
	_bMinNamesInOutput = _bMinNamesInOutput^1;	
}

void CThermoCalcView::OnUpdateMinNamesInOutput(CCmdUI* pCmdUI) {
	// TODO: Add your command update UI handler code here
	if (_bMinNamesInOutput)
		pCmdUI ->SetCheck(1);
	else
		pCmdUI ->SetCheck(0);
}

void CThermoCalcView::OnFileSaveAs() {
	// TODO: Add your command handler code here
	CThermoCalcDoc * pTCD = GetCurrentCalcDoc();
	if (!pTCD) {
		MessageBox("There are no open documents.\n");
		return;
	}
	if (!( pTCD ->CDocument::DoSave(NULL)))
//	if (!( pTCD ->CThermoCalcDoc::DoSave(NULL)))
//	if (!( pTCD ->DoSave(NULL)))
		MessageBox("Warning: File save-as failed.\n");
}

void CThermoCalcView::OnFileSave() {
	// TODO: Add your command handler code here	
	CThermoCalcDoc * pTCD = GetCurrentCalcDoc();
	if (!pTCD) {
		MessageBox("There are no open documents.\n");
		return;
	}
	 pTCD ->CDocument::DoFileSave();
}

void CThermoCalcView::OnFileClose() {
	// TODO: Add your command handler code here
	CThermoCalcDoc * pTCD = GetCurrentCalcDoc();
	if (!pTCD) {
		MessageBox("There are no open documents.\n");
		return;
	}
	if (!(pTCD ->CDocument::SaveModified()))
		return;

	// shut it down
	pTCD ->OnCloseDocument();
		// this should destroy the document
}

BOOL CThermoCalcView::FillCalcsStruct(CALCSTRUCT &cs) {
	char szNameAtomOfOxides [MaxEditBoxesOx][MaxAtoms][MaxAtomNameLen];
	float factor_oxides [MaxEditBoxesOx][MaxAtoms];
	int nEditBoxesOx = 0;
	int nAtomsOx[MaxEditBoxesOx];
	float percent_oxides[MaxEditBoxesOx]; 	//процентное отношение молекулы окислителя ко всей смеси окислителей
	char szNameAtomOfCombust [MaxEditBoxesComb][MaxAtoms][MaxAtomNameLen];
	float factor_combust [MaxEditBoxesComb][MaxAtoms];
	int nEditBoxesComb = 0;
	int nComb[MaxEditBoxesComb];
	float percent_combust[MAX_elem];

	float Tbuf_factor[MaxAtoms];
	char	Tbuf_elem[MaxAtoms][MaxAtomNameLen];
	int	num_atoms;
	char	*p;

	UpdateData(TRUE);	// Из окон редактирования в связаные переменные с помощью CDataExchange
	int ssss = sizeof(CALCSTRUCT);//DEBUG
//	ssss = sizeof(UXFSTRUCT);//DEBUG
//	ssss = sizeof(ProductsCombustion);//DEBUG
//	ssss = sizeof(INTOCALCSTRUCT);//DEBUG
	ssss = 
		sizeof(JobWithDB)+
		sizeof(CTherDynEqCalc)+
		sizeof(FirstApproximation)+
		sizeof(CalcCameraCombustion)+
		sizeof(CalcCriticalSection)+
		sizeof(CalcAnySectionWithF)+
		sizeof(CalcAnySectionWithPa)+
		sizeof(CalcAnySectionWithPa )+	// atmosphere
		sizeof(CalcISDiagram)+
		sizeof(CalcConstV)+
		sizeof(CalcEPS);//DEBUG

	//m_output+="Окислитель: ";
	int nBoxO = fiFieldInputO.GetCountItems();
	int i1;
	for (i1=0;i1<nBoxO;i1++) {
		fiFieldInputO.GetFormul(i1,sOx[i1]);
		if (sOx[i1].GetLength()) {					//окно не пусто
			num_atoms=TransNameMolecToAandF(Tbuf_elem,Tbuf_factor,(char *)(LPCTSTR)sOx[i1]);
			nAtomsOx[i1] = num_atoms;
			if (!num_atoms) {
				wsprintf(Mstr,"Встретился нераспознаваемый хим. элемент в элементе ОКИСЛИТЕЛЯ номер %u.",i1+1);
				MessageBox (Mstr);
				//UpdateData(0);
				return FALSE;
			}
// Процентное соотношение
			fiFieldInputO.GetPercent(i1,sOxP[i1]);
			if (!(sOxP[i1].GetLength())) {
				wsprintf(Mstr,"Введите процентное отношение для элемента ОКИСЛИТЕЛЬ-%u",i1+1);		
				MessageBox (Mstr);
				//UpdateData(0);
				return FALSE;	
			}

			unsigned int gf=GetUnsFactorFromStr(&percent_oxides[i1],(char *)(LPCTSTR)sOxP[i1]);
			if (!gf) {
				wsprintf(Mstr,"Неверно введено процентное отношение у элемента ОКИСЛИТЕЛЬ-%u",i1+1);
				MessageBox(Mstr);return FALSE;
			}

			if ( gf<strlen((char *)(LPCTSTR)sOxP[i1]) ) {
				wsprintf(Mstr,"В введеном процентном отношении у элемента ОКИСЛИТЕЛЬ-%u лишние символы.",i1+1);
				MessageBox(Mstr); return FALSE;
			}	
			percent_oxides[i1]=percent_oxides[i1]/100;

// Энтальпия компонента окислителя
			fiFieldInputO.GetEnthalpy(i1,sOxENT[i1]);
			int nEL = sOxENT[i1].GetLength();
			if (nEL) {
				p = sOxENT[i1].GetBuffer(MAX_len);
				if (ChangeCommaToDot(p))
					fiFieldInputO.SetEnthalpyText(i1,sOxENT[i1]);
				int nFL = GetFactorFromStr(&cs.fEnthalpyOx[i1], p);
				sOxENT[i1].ReleaseBuffer();
				if (!nFL) {
					wsprintf(Mstr,"Неверно введена энтальпия у элемента ОКИСЛИТЕЛЬ-%u",i1+1);
					MessageBox(Mstr);return FALSE;
				}
				if (nEL > nFL) {
					wsprintf(Mstr,"В введеной энтальпии у элемента ОКИСЛИТЕЛЬ-%u лишние символы.",i1+1);
					MessageBox(Mstr); return FALSE;
				}
				if (cs.fEnthalpyOx[i1] == 0) {
					wsprintf(Mstr,"Энтальпия не может быть равна нулю. Элемент ОКИСЛИТЕЛЬ-%u",i1+1);
					MessageBox(Mstr);return FALSE;
				}
			}
			else
			{
				wsprintf(Mstr,"Введите энтальпию элемента ОКИСЛИТЕЛЬ-%u",i1+1);		
				MessageBox (Mstr);
				//UpdateData(0);
				return FALSE;	
			}

// Названия атомов
			for (int i2=0;i2<num_atoms;i2++) {
				strcpy(szNameAtomOfOxides[i1][i2],Tbuf_elem[i2]);
				factor_oxides[i1][i2]=Tbuf_factor[i2];
			}
			nEditBoxesOx++;
		}
		else {
			if (nEditBoxesOx == 0) {
				MessageBox ("Введите данные об окислителе или используйте УХФ топлива!");
				//UpdateData(0);
				return FALSE;
			}
		}
	}

	//m_output+="Горючее: ";
	int nBoxC = fiFieldInputC.GetCountItems();
	for (i1=0;i1<nBoxC;i1++) {
		fiFieldInputC.GetFormul(i1,sComb[i1]);
		if (sComb[i1].GetLength()) {					//окно не пусто
			num_atoms=TransNameMolecToAandF(Tbuf_elem,Tbuf_factor,(char *)(LPCTSTR)sComb[i1]);
			nComb[i1] = num_atoms;
			if (!num_atoms) {
				wsprintf(Mstr,"Встретился нераспознаваемый хим. элемент в элементе ГОРЮЧЕГО номер %u.",i1+1);
				MessageBox (Mstr);
				//UpdateData(0);
				return FALSE;
			}
// Процентное соотношение
			fiFieldInputC.GetPercent(i1,sCombP[i1]);
			if (!(sCombP[i1].GetLength())) {
				wsprintf(Mstr,"Введите процентное отношение для элемента ГОРЮЧЕЕ-%u",i1+1);		
				MessageBox (Mstr);
				//UpdateData(0);
				return FALSE;	
			}
			unsigned int gf=GetUnsFactorFromStr(&percent_combust[i1],(char *)(LPCTSTR)sCombP[i1]);
			if (!gf) {
				wsprintf(Mstr,"Неверное процентное отношение у элемента ГОРЮЧЕЕ-%u",i1+1);
				MessageBox(Mstr);return FALSE;
			}

			if ( gf<strlen((char *)(LPCTSTR)sCombP[i1]) ) {
				wsprintf(Mstr,"В введеном процентном отношении у элемента ГОРЮЧЕЕ-%u лишние символы.",i1+1);
				MessageBox(Mstr);return FALSE;
			}
			percent_combust[i1]=percent_combust[i1]/100;

// Энтальпия компонента горючего
			fiFieldInputC.GetEnthalpy(i1,sCombENT[i1]);
			int nEL = sCombENT[i1].GetLength();
			if (nEL) {
				p = sCombENT[i1].GetBuffer(MAX_len);
				if (ChangeCommaToDot(p))
					fiFieldInputC.SetEnthalpyText(i1,sCombENT[i1]);
				int nFL = GetFactorFromStr(&cs.fEnthalpyComb[i1], p);
				sCombENT[i1].ReleaseBuffer();
				if (!nFL) {
					wsprintf(Mstr,"Неверно введена энтальпия у элемента ГОРЮЧЕЕ-%u",i1+1);
					MessageBox(Mstr);return FALSE;
				}
				if (nEL > nFL) {
					wsprintf(Mstr,"В введеной энтальпии у элемента ГОРЮЧЕЕ-%u лишние символы.",i1+1);
					MessageBox(Mstr); return FALSE;
				}
				if (cs.fEnthalpyComb[i1] == 0) {
					wsprintf(Mstr,"Энтальпия не может быть равна нулю. Элемент ГОРЮЧЕЕ-%u",i1+1);
					MessageBox(Mstr);return FALSE;
				}
			}
			else
			{
				wsprintf(Mstr,"Введите энтальпию элемента ГОРЮЧЕЕ-%u",i1+1);		
				MessageBox (Mstr);
				//UpdateData(0);
				return FALSE;	
			}

// Названия атомов
			for (int i2=0;i2<num_atoms;i2++) {
				strcpy(szNameAtomOfCombust[i1][i2],Tbuf_elem[i2]);
				factor_combust[i1][i2]=Tbuf_factor[i2];
			}
			nEditBoxesComb++;
		}
		else {
			if (nEditBoxesComb==0) {
				MessageBox ("Введите данные о горючем или используйте УХФ топлива!");
				//UpdateData(0);
				return FALSE;
			}
		}
	}

	// Коэффициент избытка окислителя
	int nBoxAlfa = fihAlfa.GetCountItems();
	for (i1=0;i1<nBoxAlfa;i1++) {
		fihAlfa.GetText(i1,sAlfa[i1]);
		int nAL = sAlfa[i1].GetLength();
		if (nAL) {
			p = sAlfa[i1].GetBuffer(MAX_len);
			if (ChangeCommaToDot(p))
				fihAlfa.SetText(i1,sAlfa[i1]);
			int nFL = GetUnsFactorFromStr(&fAlfa[i1], p);
			sAlfa[i1].ReleaseBuffer();
			if (!nFL) {
				MessageBox ("Bad alfa field!");
				//UpdateData(0);
				return FALSE;
			}
			if (nAL > nFL) {
				MessageBox ("Alfa field have bad symbols!");
				return FALSE;
			}
			if (fAlfa[i1] == 0) {
				MessageBox ("Alfa field must be not 0.");
				return FALSE;
			}
			//if (fAlfa[i1] >= 1) {
			//	MessageBox ("Alfa field must be low than 1.");
			//	return FALSE;
			//}
		}
		else {
			MessageBox ("Please input alfa!");
			return FALSE;
		}
	}

	// Параметы расчета горения
		// Давления в КС
	int nBoxPressure = fihPressures.GetCountItems();
	for (i1=0;i1<nBoxPressure;i1++) {
		fihPressures.GetText(i1,sPressures[i1]);
		int nPL = sPressures[i1].GetLength();
		if (nPL) {
			p = sPressures[i1].GetBuffer(MAX_len);
			if (ChangeCommaToDot(p))
				fihPressures.SetText(i1,p);
			int nFL = GetUnsFactorFromStr(&fPressures[i1],p);
			sPressures[i1].ReleaseBuffer();
			if (!nFL) {
				MessageBox ("Bad pressure field!");
				return FALSE;
			}
			if (nPL > nFL) {
				MessageBox ("Pressure field have bad symbols!");
				return FALSE;
			}
			if (fPressures[i1] == 0) {
				MessageBox ("Pressure field must be not 0.");
				return FALSE;
			}
			//if (fAlfa[i1] >= 1) {
			//	MessageBox ("Pressure field must be more than ???.");
			//	return FALSE;
			//}
		}
		else {
			MessageBox ("Please input pressure !");
			return FALSE;
		}
	}

	// Предполагаемая температура
	int nTL = strT.GetLength();
	if (nTL) {
		p = strT.GetBuffer(MAX_len);
		ChangeCommaToDot(p);
		int nFL = GetUnsFactorFromStr(&fT,p);
		strT.ReleaseBuffer();
		if (!nFL) {
			MessageBox ("Bad temperature field!");
			return FALSE;
		}
		if (nTL > nFL) {
			MessageBox ("Temperature field have bad symbols!");
			return FALSE;
		}
	}
	else {
		MessageBox ("Please input temperature !");
		return FALSE;
	}

	// Максимальная температура
	nTL = strMaxT.GetLength();
	if (nTL) {
		p = strMaxT.GetBuffer(MAX_len);
		ChangeCommaToDot(p);
		int nFL = GetUnsFactorFromStr(&fMaxT,p);
		strMaxT.ReleaseBuffer();
		if (!nFL) {
			MessageBox ("Bad MAX temperature field!");
			return FALSE;
		}
		if (nTL > nFL) {
			MessageBox ("MAX temperature field have bad symbols!");
			return FALSE;
		}
	}
	else {
		MessageBox ("Please input MAX temperature !");
		return FALSE;
	}

	// Минимальная температура
	nTL = strMinT.GetLength();
	if (nTL) {
		p = strMinT.GetBuffer(MAX_len);
		ChangeCommaToDot(p);
		int nFL = GetUnsFactorFromStr(&fMinT,p);
		strMinT.ReleaseBuffer();
		if (!nFL) {
			MessageBox ("Bad MIN temperature field!");
			return FALSE;
		}
		if (nTL > nFL) {
			MessageBox ("MIN temperature field have bad symbols!");
			return FALSE;
		}
	}
	else {
		MessageBox ("Please input MIN temperature !");
		return FALSE;
	}

	// Параметры расчета течения
		//	"Pа - давление в сечении";

		int nBoxPa = fihPressureInSection.GetCountItems();
		for (i1=0;i1<nBoxPa;i1++) {
			fihPressureInSection.GetText(i1,sPressureInSection[i1]);
			int nPaL = sPressureInSection[i1].GetLength();
			if (nPaL) {
				p = sPressureInSection[i1].GetBuffer(MAX_len);
				if (ChangeCommaToDot(p))
					fihPressureInSection.SetText(i1,p);
				int nFL = GetUnsFactorFromStr(&fPressureInSection[i1],p);
				sPressureInSection[i1].ReleaseBuffer();
				if (!nFL) {
					MessageBox ("Bad 'Pa' field!", "Calculation flow error");
					return FALSE;
				}
				if (nPaL > nFL) {
					MessageBox ("'Pa' field have bad symbols!", "Calculation flow error");
					return FALSE;
				}
				if (fPressureInSection[i1] == 0) {
					MessageBox ("'Pa' field must be not 0.", "Calculation flow error");
					return FALSE;
				}
			}
			else {
				MessageBox ("Please input 'Pa' !", "Calculation flow error");
				return FALSE;
			}
		}

		//	"F/Fkp - степень расширения сопла";
	int nBoxF = fihPowerExpNozzle.GetCountItems();
	for (i1=0;i1<nBoxF;i1++) {
		fihPowerExpNozzle.GetText(i1,sPowerExpNozzle[i1]);
		int nPaL = sPowerExpNozzle[i1].GetLength();
		if (nPaL) {
			p = sPowerExpNozzle[i1].GetBuffer(MAX_len);
			if (ChangeCommaToDot(p))
				fihPowerExpNozzle.SetText(i1,p);
			int nFL = GetUnsFactorFromStr(&fPowerExpNozzle[i1],p);
			sPowerExpNozzle[i1].ReleaseBuffer();
			if (!nFL) {
				MessageBox ("Bad 'F/Fкр' field!", "Calculation flow error");
				return FALSE;
			}
			if (nPaL > nFL) {
				MessageBox ("'F/Fкр' field have bad symbols!", "Calculation flow error");
				return FALSE;
			}
			if (fPowerExpNozzle[i1] == 0) {
				MessageBox ("'F/Fкр' field must be not 0.", "Calculation flow error");
				return FALSE;
			}
		}
		else {
			MessageBox ("Please input 'F/Fкр' !", "Calculation flow error");
			return FALSE;
		}
	}

		//	"IS диаграмма";
	int nBoxIS = 0;
	unsigned int uiFIHState = fihIS.GetTransformState();
	if (uiFIHState == FieldInputH::TransStateIsValue) {
		nBoxIS = fihIS.GetCountItems();
		for (i1=0;i1<nBoxIS;i1++) {
			fihIS.GetText(i1,sIS[i1]);
			int nPaL = sIS[i1].GetLength();
			if (nPaL) {
				p = sIS[i1].GetBuffer(MAX_len);
				if (ChangeCommaToDot(p))
					fihIS.SetText(i1,p);
				int nFL = GetUnsFactorFromStr(&fIS[i1],p);
				sIS[i1].ReleaseBuffer();
				if (!nFL) {
					MessageBox ("Bad 'IS' field!", "Calculation flow error");
					return FALSE;
				}
				if (nPaL > nFL) {
					MessageBox ("'IS' field have bad symbols!", "Calculation flow error");
					return FALSE;
				}
				if (fIS[i1] == 0) {
					MessageBox ("'IS' field must be not 0.", "Calculation flow error");
					return FALSE;
				}
			}
			else {
				MessageBox ("Please input 'IS'!", "Calculation flow error");
				return FALSE;
			}
		}
	}
	else {
			// Шаг
		if (!fihIS.CalcSteps())	// Просчет шагов объекта перед использованием
			return FALSE;
		nBoxIS = fihIS.GetAmountSteps();
		for (int i=0; i<nBoxIS; i++) {
			fihIS.GetSomeValueOfSteps(i,&fIS[i]);
		}
	}


		//	"При постоянном объеме";
	int nBoxV = fihV.GetCountItems();
	for (i1=0;i1<nBoxV;i1++) {
		fihV.GetText(i1,sV[i1]);
		int nPaL = sV[i1].GetLength();
		if (nPaL) {
			p = sV[i1].GetBuffer(MAX_len);
			if (ChangeCommaToDot(p))
				fihV.SetText(i1,p);
			int nFL = GetUnsFactorFromStr(&fV[i1],p);
			sV[i1].ReleaseBuffer();
			if (!nFL) {
				MessageBox ("Bad 'const V' field!", "Calculation flow error");
				return FALSE;
			}
			if (nPaL > nFL) {
				MessageBox ("'const V' field have bad symbols!", "Calculation flow error");
				return FALSE;
			}
			if (fV[i1] == 0) {
				MessageBox ("'const V' field must be not 0.", "Calculation flow error");
				return FALSE;
			}
		}
		else {
			MessageBox ("Please input 'const V' !", "Calculation flow error");
			return FALSE;
		}
	}

		//	"EPS - плотность в сечении";
	int nBoxEPS = fihEPS.GetCountItems();
	for (i1=0;i1<nBoxEPS;i1++) {
		fihEPS.GetText(i1,sEPS[i1]);
		int nPaL = sEPS[i1].GetLength();
		if (nPaL) {
			p = sEPS[i1].GetBuffer(MAX_len);
			if (ChangeCommaToDot(p))
				fihEPS.SetText(i1,p);
			int nFL = GetUnsFactorFromStr(&fEPS[i1],p);
			sEPS[i1].ReleaseBuffer();
			if (!nFL) {
				MessageBox ("Bad 'EPS' field!", "Calculation flow error");
				return FALSE;
			}
			if (nPaL > nFL) {
				MessageBox ("'EPS' field have bad symbols!", "Calculation flow error");
				return FALSE;
			}
			if (fEPS[i1] == 0) {
				MessageBox ("'EPS' field must be not 0.", "Calculation flow error");
				return FALSE;
			}
		}
		else {
			MessageBox ("Please input 'EPS' !", "Calculation flow error");
			return FALSE;
		}
	}


	for (i1=0;i1<nEditBoxesOx;i1++) {
		for (int i2=0;i2<nAtomsOx[i1];i2++) {
			strcpy(cs.szNameAtomOfOxides[i1][i2],szNameAtomOfOxides[i1][i2]);
			cs.factor_oxides[i1][i2] = factor_oxides[i1][i2];
		}
		cs.percent_oxides[i1] = percent_oxides[i1];
		cs.nAtomsInStringOx[i1] = nAtomsOx[i1];
	}
	cs.nEditBoxesOx = nEditBoxesOx;

	for (i1=0;i1<nEditBoxesComb;i1++) {
		for (int i2=0;i2<nComb[i1];i2++) {
			strcpy(cs.szNameAtomOfCombust[i1][i2],szNameAtomOfCombust[i1][i2]);
			cs.factor_combust[i1][i2] = factor_combust[i1][i2];
		}
		cs.percent_combust[i1] = percent_combust[i1];
		cs.nAtomsInStringComb[i1] = nComb[i1];
	}
	cs.nEditBoxesComb = nEditBoxesComb;

	for (i1=0;i1<nBoxAlfa;i1++) {
		cs.fAlfa[i1] = fAlfa[i1];
	}
	cs.nEditBoxesAlfa = nBoxAlfa;

	for (i1=0;i1<nBoxPressure;i1++) {
		cs.fPressure[i1] = fPressures[i1];
	}
	cs.nEditBoxesPressure = nBoxPressure;

	cs.fAssumedT = fT;
	cs.fMaxT = fMaxT;
	cs.fMinT = fMinT;

		//	"Pа - давление в сечении";
	if(nBoxPa) {
		cs.bIncludePa = TRUE;
		for (i1=0;i1<nBoxPa;i1++)
			cs.fPa[i1] = fPressureInSection[i1];
		cs.nPa = nBoxPa;
	}
	else
		cs.bIncludePa = FALSE;

		//	"F/Fkp - степень расширения сопла";
	if(nBoxF) {
		cs.bIncludeF = TRUE;
		for (i1=0;i1<nBoxF;i1++)
			cs.fF[i1] = fPowerExpNozzle[i1];
		cs.nF = nBoxF;
	}
	else
		cs.bIncludeF = FALSE;

		//	"IS диаграмма";
	if(nBoxIS) {
		cs.bIncludeIS = TRUE;
		for (i1=0;i1<nBoxIS;i1++)
			cs.fIS[i1] = fIS[i1];
		cs.nIS = nBoxIS;
	}
	else
		cs.bIncludeIS = FALSE;

		//	"EPS - плотность в сечении";
	cs.bIncludeEPS = FALSE;
	if(nBoxEPS) {
		cs.bIncludeEPS = TRUE;
		for (i1=0;i1<nBoxEPS;i1++)
			cs.fEPS[i1] = fEPS[i1];
		cs.nEPS = nBoxEPS;
	}
	else
		cs.bIncludeEPS = FALSE;

		//	"При постоянном объеме";
	if(nBoxV) {
		cs.bIncludeV = TRUE;
		for (i1=0;i1<nBoxV;i1++)
			cs.fV[i1] = fV[i1];
		cs.nV = nBoxV;
	}
	else
		cs.bIncludeV = FALSE;

	return TRUE;
}

void CThermoCalcView::ShowUXF(UXFSTRUCT * pUXF) {
	RedrawPSUXF();
	int nA =fihAlfa.GetCountItems();
	for (int i = 0; i<nA; i++) {
		sUXF[i].Empty();
		for (int i2=0;i2<pUXF[i].nQuantityElemF;i2++) {
			sUXF[i] += pUXF[i].szAtomsOfF[i2];
			ToDotString(pUXF[i].fConditChimFormF[i2], sUXF[i]);
		}
		sEnthalpyUXF[i].Empty();
		ToDotString(pUXF[i].fEnthalpyF, sEnthalpyUXF[i]);
		recEnthalpyUXF[i].SetWindowText((LPCTSTR)sEnthalpyUXF[i]);
		reccUXF[i].SetWindowTextChem((LPCTSTR)sUXF[i]);
	}
}

BOOL CThermoCalcView::GetUXFFromBox(UXFSTRUCT * pUXF) {

	int nP = ps.GetPageCount();
	if (!nP) {
		MessageBox ("Ни одна УХФ топлива еще не расчитана!");
		return FALSE;
	}

	for (int i = 0; i<nP; i++) {
		reccUXF[i].GetWindowText(sUXF[i]);
		if (sUXF[i].IsEmpty()) {
			wsprintf(Mstr,"Введите УХФ в закладку номер %u, или уберите окно с одним из альфа!",i+1);
			MessageBox(Mstr); return FALSE;
		}

		char *p = sUXF[i].GetBuffer(MAX_len);
		//int a = strlen(p);
		int nQuantity = TransNameMolecToAandF(pUXF[i].szAtomsOfF, 
				pUXF[i].fConditChimFormF, p);
		sUXF[i].ReleaseBuffer();
		if (!nQuantity) {	pUXF[i].nQuantityElemF = 0;	
			wsprintf(Mstr,"Ошибка в УХФ в закладке номер %u!",i+1);
			MessageBox(Mstr); return FALSE;
		}
		pUXF[i].nQuantityElemF = nQuantity;
		//Еще нужно взять энтальпию !!!???
		recEnthalpyUXF[i].GetWindowText(sEnthalpyUXF[i]);
		int nEL = sEnthalpyUXF[i].GetLength();
		if (nEL) {
			p = sEnthalpyUXF[i].GetBuffer(MAX_len);
			if (ChangeCommaToDot(p))
				recEnthalpyUXF[i].SetWindowText(sEnthalpyUXF[i]);
			int nFL = GetFactorFromStr(&pUXF[i].fEnthalpyF, p);
			sEnthalpyUXF[i].ReleaseBuffer();
			if (!nFL) {
				wsprintf(Mstr,"Неверно введена энтальпия УХФ в закладке номер %u!",i+1);
				MessageBox(Mstr);return FALSE;
			}
			if (nEL > nFL) {
				wsprintf(Mstr,"В введеной энтальпии УХФ в закладке номер %u лишние символы.",i+1);
				MessageBox(Mstr); return FALSE;
			}
			if (pUXF[i].fEnthalpyF == 0) {
				wsprintf(Mstr,"Энтальпия не может быть равна нулю. УХФ в закладке номер %u.",i+1);
				MessageBox(Mstr);return FALSE;
			}
		}
		else
		{
			wsprintf(Mstr,"Введите энтальпию УХФ в закладку номер %u!",i+1);
			MessageBox(Mstr); return FALSE;
		}
	}
	return TRUE;
}

void CThermoCalcView::OnCheckCalcOnlyWithUxf() {
	// TODO: Add your control notification handler code here
	bCalcOnlyWithUxf = bCalcOnlyWithUxf^1;
	if (bCalcOnlyWithUxf) {
		// Disable
		fiFieldInputO.EnableWindow (FALSE);
		fiFieldInputC.EnableWindow (FALSE);
	}
	else	{
		// To be able
		fiFieldInputO.EnableWindow ();
		fiFieldInputC.EnableWindow ();
	}
	SetFocus();
}

void CThermoCalcView::OnCheckCriticSection() {	bCalcCritSect ^= TRUE;	SetFocus();}
void CThermoCalcView::OnCheckAtmosphereSection() { bCalcAtmosphereSection ^= TRUE; SetFocus();}

void CThermoCalcView::ShowLoadView(CALCSTRUCT &cs,	UXFSTRUCT uxf[MaxQuantityFIHBox])	{	}

void CThermoCalcView::OnDraw(CDC* pDC) {	// TODO: Add your specialized code here and/or call the base class
/*		RECT r;//		GetClientRect(&r);//		GetWindowRect(&r);
		pDC ->SetMapMode(MM_HIENGLISH);//		pDC ->SetMapMode(MM_LOENGLISH);//		pDC ->SetMapMode(MM_TEXT);
//		pDC ->SetMapMode(MM_HIMETRIC);//		pDC ->SetMapMode(MM_LOMETRIC);//		pDC ->SetMapMode(MM_TWIPS);*/
		CGdiObject * pGO_oldBrush = pDC ->SelectStockObject(COLOR_WINDOW);
		CGdiObject * pGO_oldPen = pDC ->SelectStockObject(BLACK_PEN);
		RECT rect = {8, 377};
		rect.right = rect.left + 200;
		rect.bottom = rect.top + 64;
//		pDC ->Rectangle(&rect);//		DrawNozzle(&rect, pDC);//		ValidateRect(&rect);
}

void CThermoCalcView::OnVScroll(UINT nSBCode, UINT nPos, CScrollBar* pScrollBar) {
	// TODO: Add your message handler code here and/or call default
	if (fiFieldInputO.Scroll(nSBCode, nPos, pScrollBar))
		return;
	if (fiFieldInputC.Scroll(nSBCode, nPos, pScrollBar))
		return;
	CFormView::OnVScroll(nSBCode, nPos, pScrollBar);
}

void CThermoCalcView::OnButtonAppend_FormulO() {	fiFieldInputO.Add(this);}
void CThermoCalcView::OnButtonDis_FormulO() {	fiFieldInputO.Remove();}
void CThermoCalcView::OnButtonAppend_FormulC() {	fiFieldInputC.Add(this);}
void CThermoCalcView::OnButtonDis_FormulC() {	fiFieldInputC.Remove();}
void CThermoCalcView::OnButtonAppend_Alfa() {	fihAlfa.Add(this);}
void CThermoCalcView::OnButtonDis_Alfa() {	fihAlfa.Remove();}
void CThermoCalcView::OnButtonAppend_Pressure() {	fihPressures.Add(this);}
void CThermoCalcView::OnButtonDis_Pressure() {	fihPressures.Remove();}
void CThermoCalcView::OnButtonAppend_PressureInSection() {	fihPressureInSection.Add(&ppCalcFlow_Pa);}
void CThermoCalcView::OnButtonDis_PressureInSection() {	fihPressureInSection.Remove(); }
void CThermoCalcView::OnButtonAppend_PowerExpNozzle() {	fihPowerExpNozzle.Add(&ppCalcFlow_F); }
void CThermoCalcView::OnButtonDis_PowerExpNozzle() {	fihPowerExpNozzle.Remove(); }
void CThermoCalcView::OnButtonAppend_IS() {	fihIS.Add(&ppCalcFlow_IS); }
void CThermoCalcView::OnButtonDis_IS() {	fihIS.Remove(); }
void CThermoCalcView::OnButtonAppend_V() {	fihV.Add(&ppCalcFlow_V); }
void CThermoCalcView::OnButtonDis_V() {	fihV.Remove(); }
void CThermoCalcView::OnButtonAppend_EPS() {	fihEPS.Add(&ppCalcFlow_EPS); }
void CThermoCalcView::OnButtonDis_EPS() {	fihEPS.Remove(); }
void CThermoCalcView::OnButtonValue_IS() 
{	fihIS.Transform(&ppCalcFlow_IS, FieldInputH::TransStateIsValue); }
void CThermoCalcView::OnButtonStep_IS() 
{	fihIS.Transform(&ppCalcFlow_IS, FieldInputH::TransStateIsStep); }

void CThermoCalcView::RedrawPSUXF() {
	//CDialog
	// Clear
	int nP = ps.GetPageCount();
	int i;
	for (i = nP-1; i>=0; i--) {	// с конца
		BOOL bD = reccUXF[i].DestroyWindow();
		bD = recEnthalpyUXF[i].DestroyWindow();
		bD = stUXF[i].DestroyWindow();
		bD = stEnthalpyUXF[i].DestroyWindow();
		ps.RemovePage(i);
	}
	BOOL bD = ps.DestroyWindow();

	//New
	int nA =fihAlfa.GetCountItems();
	if (!nA) {
		DrawEmptyPSUXF();
		return;
	}

	for (i = 0; i<nA; i++) {
		fihAlfa.GetText(i,sAlfa[i]);
		if (sAlfa[i].GetLength()) {
			if (!GetFactorFromStr(&fAlfa[i],(char *)(LPCTSTR)sAlfa[i])) {
				MessageBox ("Bad alfa field!?");
				DrawEmptyPSUXF();
				return;
			}
		}
		strcpy(szTitilePP[i], "Alfa = ");
		fihAlfa.GetText( i, &szTitilePP[i][ strlen(szTitilePP[i]) ]);
	}

	ps.Construct("Caption PSUXF", this);
	ps.m_psh.dwFlags |= PSH_DEFAULT;

	for (i = 0; i<nA; i++) {
		pp[i].Construct(IDD_PP_TAMP1);
		pp[i].m_psp.dwFlags |= PSP_PREMATURE|PSP_USETITLE;
		pp[i].m_psp.pszTitle = szTitilePP[i];
		ps.AddPage(&pp[i]);
	}

	ps.Create(this, WS_CHILD|WS_VISIBLE|WM_NOTIFY);//, WS_EX_DLGMODALFRAME);
	RECT rectPS = {cnPSUXFLeft, cnPSUXFTop};
	rectPS.right = rectPS.left + cnPSUXFWidth;
	rectPS.bottom = rectPS.top + cnPSUXFHigh;
	ps.MoveWindow(&rectPS);

	RECT rect = {cnBoxUXFLeft, cnBoxUXFTop};
	rect.right = rect.left + cnBoxUXFWidth;
	rect.bottom = rect.top + cnBoxUXFHigh;
	RECT rectE = {cnBoxEnthalpyUXFLeft, cnBoxEnthalpyUXFTop};
	rectE.right = rectE.left + cnBoxEnthalpyUXFWidth;
	rectE.bottom = rectE.top + cnBoxEnthalpyUXFHigh;
	RECT rectStUXF = {cnStUXFLeft, cnStUXFTop};
	rectStUXF.right = rectStUXF.left + cnStUXFWidth;
	rectStUXF.bottom = rectStUXF.top + cnStUXFHigh;
	RECT rectStEUXF = {cnStEnthalpyUXFLeft, cnStEnthalpyUXFTop};
	rectStEUXF.right = rectStEUXF.left + cnStEnthalpyUXFWidth;
	rectStEUXF.bottom = rectStEUXF.top + cnStEnthalpyUXFHigh;
	for (i = 0; i<nA; i++) {
		// Окошко с УХФ
		DWORD dwS = WS_CHILD|WS_BORDER|ES_AUTOHSCROLL|WS_VISIBLE;
		BOOL bEC = reccUXF[i].Create(dwS, rect, ps.GetPage(i), ID_UFX + i);

		// Окошко с энтальпией УХФ
		DWORD dwSE = WS_CHILD|WS_VISIBLE|WS_BORDER|ES_AUTOHSCROLL|ES_NUMBER;
		BOOL bECE = recEnthalpyUXF[i].Create(dwSE, rectE, ps.GetPage(i), ID_ENT_UFX + i);
		CHARFORMAT _cf;
		fihAlfa.GetDefaultCharFormat(_cf);
		//recEnthalpyUXF[i].GetDefaultCharFormat(_cf);
		recEnthalpyUXF[i].SetDefaultCharFormat(_cf);
		ps.GetPage(i) ->InvalidateRect(&rectE);

		// Static
		stUXF[i].Create(cnstrUXF, WS_CHILD|WS_VISIBLE|SS_RIGHT|SS_NOTIFY, rectStUXF, ps.GetPage(i));

		// Static
		stEnthalpyUXF[i].Create(cnstrEnthalpyUXF, WS_CHILD|WS_VISIBLE|SS_RIGHT|SS_NOTIFY, rectStEUXF, ps.GetPage(i));
	}
	SetFocus();
	SetActiveWindow( );

//	_AfxCbtFilterHook(int code, WPARAM wParam, LPARAM lParam)
//	HWND hWnd = (HWND)wParam;
//	LRESULT lResult = CallNextHookEx(pThreadState->m_hHookOldCbtFilter, code, wParam, lParam);
// CBTProc
//Value							wParam 
// HCBT_CREATEWND	Specifies the handle to the new window. 
// lParam
// Specifies a long pointer to a CBT_CREATEWND structure containing initialization parameters for the window.
// The parameters include the coordinates and dimensions of the window.
// By changing these parameters, a CBTProc hook procedure can set the initial size and position of the window.
}

int CThermoCalcView::OnCreate(LPCREATESTRUCT lpCreateStruct) {
	if (CFormView::OnCreate(lpCreateStruct) == -1)
		return -1;
	// TODO: Add your specialized creation code here
	return 0;
}

// Взять размеры характеристики динамически-создаваемых эл. уравления
// из графически создаваемых ресурсов
// Похожее есть у класса CDialogTemplate
int CThermoCalcView::TakeDlg (WORD Id, DLGTEMPLATE *pdt, WORD *pwFontDlgSize, char *pszFontDlgName, 
	DLGITEMTEMPLATE pdit[], char szItemInitText[][MAX_len], char szItemClassName[][MAX_len], int nItemSysClass[])
{
	HMODULE hRMH =AfxGetResourceHandle( );
	HRSRC hRS = ::FindResource(hRMH, MAKEINTRESOURCE(Id), RT_DIALOG);
	HGLOBAL hG = ::LoadResource(hRMH, hRS);
	BYTE *pFirstByteRes = (BYTE *)LockResource(hG);
	DWORD size = SizeofResource(hRMH, hRS);	
	WORD w;
	LPCWSTR pws;
	WORD nFlagMenu;
	WORD nFlagClass;
	WORD nFlagTitle;
	char szTitleDlg[MAX_len];
	int nTitleDlgLenght;
	int nFontDlgNameLenght;
	int nQuantityItem;
	int nCurrentItem;

	BYTE *pb = pFirstByteRes;
	BYTE * dwLastByteRes = pFirstByteRes+size-1;
	// Проверим версию ресурса
	WORD wVerDTE;										// Версия DLGTEMPLATEEX 
	WORD wSignatureDTE;								// Обозначает что эта структура - DLGTEMPLATEEX
	memcpy(&wVerDTE, pb,sizeof(wVerDTE));
	pb += sizeof(wVerDTE);
	memcpy(&wSignatureDTE, pb,sizeof(wSignatureDTE));
	pb += sizeof(wSignatureDTE);
	if (wSignatureDTE == 0xFFFF)
		if (wVerDTE == 1) {
			// пусть работает другая функция
			MessageBox("Для ресурса DLGTEMPLATEEX код еще не написан!");
			PostQuitMessage(0);
			return -1;
		}else{
			// неизвестно что
			MessageBox("Неизвестная версия ресурса");
			PostQuitMessage(0);
			return -1;
		}

	//  Значит версия DLGTEMPLATE 
	pb = pFirstByteRes;
	memcpy(pdt, pb,sizeof(DLGTEMPLATE));
	pb += sizeof(DLGTEMPLATE);

	// Ресурс не должен иметь меню, использовать не предопределенный диалоговый класс
	memcpy(&nFlagMenu, pb,sizeof(WORD));
	pb += sizeof(WORD);
	if (nFlagMenu != 0x0000) {
		MessageBox("Ресурс не должен иметь меню");
		PostQuitMessage(0);
		return -1;
	}

	memcpy(&nFlagClass, pb,sizeof(WORD));
	pb += sizeof(WORD);
	if (nFlagClass!= 0x0000) {
		MessageBox("Ресурс не должен использовать не предопределенный диалоговый класс");
		PostQuitMessage(0);
		return -1;
	}

	memcpy(&nFlagTitle, pb,sizeof(WORD));
	pb += sizeof(WORD);
	nTitleDlgLenght = 0;
	if (nFlagTitle!= 0x0000) {
		pws = (LPCWSTR)pb;
		nTitleDlgLenght = WideCharToMultiByte(CP_ACP,WC_COMPOSITECHECK, pws, -1,//wcslen(pws), 
			szTitleDlg, MAX_len-1, NULL, NULL);
		//MessageBox("Ресурс не должен иметь заголовок");		AfxGetMainWnd() -> DestroyWindow ( );
		pb += nTitleDlgLenght *sizeof(WCHAR);
	}

	nFontDlgNameLenght = 0;
	if (pdt ->style & DS_SETFONT) {
		memcpy(pwFontDlgSize, pb,sizeof(WORD));	// Point size of font
		pb += sizeof(WORD);
		pws = (LPCWSTR)pb;
		nFontDlgNameLenght = WideCharToMultiByte(CP_ACP,WC_COMPOSITECHECK, pws, -1,
			pszFontDlgName, MAX_len-1, NULL, NULL);
		pb += nFontDlgNameLenght*sizeof(WCHAR);
	}


	nQuantityItem = pdt ->cdit;
	if (nQuantityItem >MAX_Items) {
		MessageBox("Ресурс содержит слишком много элементов","Fatal error",MB_ICONERROR);
		PostQuitMessage(0);
		return -1;
	}

	// Выравнивание на DWORD
	if ( ((DWORD)pb) & (sizeof(DWORD) - 1) ) {
		pb += sizeof(WORD);
		if ( ((DWORD)pb)& (sizeof(DWORD) - 1) )
			pb += sizeof(WORD);
	}

	for (nCurrentItem=0; (nCurrentItem < nQuantityItem) || (pb < dwLastByteRes); nCurrentItem++) {
		memcpy(&pdit[nCurrentItem], pb, sizeof(DLGITEMTEMPLATE));
		pb += sizeof(DLGITEMTEMPLATE);

// class, title, and creation data for the control
		memcpy(&w, pb,sizeof(w));										// class
		if (w !=0xFFFF) {															// Unicode string that specifies the name of a registered window class
			pws = (LPCWSTR)pb;
			int nC = WideCharToMultiByte(CP_ACP,WC_COMPOSITECHECK, pws, -1,
				szItemClassName[nCurrentItem], MAX_len-1, NULL, NULL);
			pb += nC*sizeof(WCHAR);
			if (!strcmp("RICHEDIT", szItemClassName[nCurrentItem])) {
				nItemSysClass[nCurrentItem] = Item_Edit;
				szItemInitText[nCurrentItem][0] = 0;
			}
			else {
				MessageBox("Ресурс не должен содержать элементы использующие не предопределенный диалоговый класс",0,MB_ICONERROR);
				//AfxGetMainWnd() -> DestroyWindow ( ); 
				PostQuitMessage(0);
				return -1;
			}
		}
		else {
			pb += sizeof(WORD);

			// ordinal value of a predefined system class
			memcpy(&w, pb,sizeof(w));									// title
			pb += sizeof(WORD);
			nItemSysClass[nCurrentItem] = w;
			szItemClassName[nCurrentItem][0] = 0;
			switch (w) {
				case Item_Edit:
				case Item_Button:
				case Item_Static:
					memcpy(&w, pb,sizeof(w));	// flag initial text
					//pb += sizeof(WORD);
					if (w != 0xFFFF) {	// null-terminated Unicode string that specifies the initial text
						pws = (LPCWSTR)pb;
						int n1 = WideCharToMultiByte(CP_ACP,WC_COMPOSITECHECK, pws, -1,
							szItemInitText[nCurrentItem], MAX_len-1, NULL, NULL);
						pb += n1*sizeof(WCHAR);
					}
					else {
						MessageBox("Ресурс не должен содержать элементы ссылающиеся на идентификаторы ресурсов-контролов, таких как 'icon' и т.д.");
						PostQuitMessage(0);
						return -1;
						// ordinal value of a resource
						//pb += sizeof(WORD);
					}
				break;

				//case ItemUXF_Edit:
				//break;
				default:
					MessageBox("Ресурс содержит неизвестный ординал предопределенного диалогового класса");
					PostQuitMessage(0);
					return -1;
				break;
			}
		}

		// Выравнивание на WORD
		if ( ((DWORD)pb) & (sizeof(BYTE) - 1) )
			pb += sizeof(BYTE);
		memcpy(&w, pb,sizeof(w));										// The creation data array
		pb += sizeof(WORD);

		if (w)
			// When the system creates the control, 
			// it passes a pointer to this data in the lParam parameter of the WM_CREATE message
			// that it sends to the control. 
			pb += w*sizeof(BYTE);

		// Выравнивание на DWORD
		if ( ((DWORD)pb) & (sizeof(DWORD) - 1) ) {
			pb += sizeof(WORD);
			if ( ((DWORD)pb)& (sizeof(DWORD) - 1) )
				pb += sizeof(WORD);
		}
	}
//	BOOL b = ::DeleteObject(hG);
/*
LPWORD lpwAlign ( LPWORD lpIn) { 
  ULONG ul;
  ul = (ULONG) lpIn;  ul +=3;  ul >>=2;  ul <<=2;
  return (LPWORD) ul;
} 
int nCopyAnsiToWideChar (LPWORD lpWCStr, LPSTR lpAnsiIn) { 
  int nChar = 0;
  do { *lpWCStr++ = (WORD) *lpAnsiIn;    nChar++;  } while (*lpAnsiIn++);
  return nChar;
}*/
	return nQuantityItem;
};

WORD CThermoCalcView::SetCountItemInDlg (WORD Id, WORD wNewCount) {
	HMODULE hRMH =AfxGetResourceHandle( );
	HRSRC hRS = ::FindResource(hRMH, MAKEINTRESOURCE(Id), RT_DIALOG);
	if (!hRS)
		return -1;
	HGLOBAL hG = ::LoadResource(hRMH, hRS);
	if (!hG)
		return -1;
	BYTE *pFirstByteRes = (BYTE *)LockResource(hG);
	if (!pFirstByteRes)
		return -1;
	DWORD size = SizeofResource(hRMH, hRS);	
	if (!size)
		return -1;
//	WORD nFlagMenu;
//	WORD nFlagClass;
//	WORD nFlagTitle;
//	char szTitleDlg[MAX_len];
//	int nTitleDlgLenght;
//	int nFontDlgNameLenght;
//	int nQuantityItem;
//	int nCurrentItem;
	WORD wOldCount;

	BYTE *pb = pFirstByteRes;
	BYTE * dwLastByteRes = pFirstByteRes+size-1;
	// Проверим версию ресурса
	WORD wVerDTE;										// Версия DLGTEMPLATEEX 
	WORD wSignatureDTE;								// Обозначает что эта структура - DLGTEMPLATEEX
	memcpy(&wVerDTE, pb,sizeof(wVerDTE));
	pb += sizeof(wVerDTE);
	memcpy(&wSignatureDTE, pb,sizeof(wSignatureDTE));
	pb += sizeof(wSignatureDTE);
	if (wSignatureDTE == 0xFFFF)
		if (wVerDTE == 1) {
			// пусть работает другая функция
			wVerDTE = wVerDTE;
			MessageBox("Для ресурса типа DLGTEMPLATEEX код еще не написан!");
			PostQuitMessage(0);
			return -1;
		}else{
			// неизвестно что
			MessageBox("Неизвестная версия ресурса");
			PostQuitMessage(0);
			return -1;
		}

	//  Значит версия DLGTEMPLATE 
	pb = pFirstByteRes;
	pb += 2*sizeof(DWORD);
	WORD *pw = (WORD *)pb;
	memcpy(&wOldCount, pb,sizeof(WORD));
	*pb = LOBYTE(wNewCount);
	pb ++;
	*pb = HIBYTE(wNewCount);

	return wOldCount;
};


void CThermoCalcView::DrawEmptyPSUXF() {
	ps.Construct("Caption PSUXF", this);
	ps.m_psh.dwFlags |= PSH_DEFAULT;
	pp[0].Construct(IDD_PP_TAMP1);
	pp[0].m_psp.dwFlags |= PSP_PREMATURE|PSP_USETITLE;
	strcpy(szTitilePP[0],"Нужно задать альфа");
	pp[0].m_psp.pszTitle = szTitilePP[0];
	ps.AddPage(&pp[0]);								// Должна быть хотя бы одна страница
	ps.Create(this, WS_CHILD|WS_VISIBLE|WM_NOTIFY);//, WS_EX_DLGMODALFRAME);
	RECT rectPS = {cnPSUXFLeft, cnPSUXFTop};
	rectPS.right = rectPS.left + cnPSUXFWidth;
	rectPS.bottom = rectPS.top + cnPSUXFHigh;
	ps.MoveWindow(&rectPS);
	SetFocus();
	SetActiveWindow( );

	// Окошко с УХФ
	RECT rect = {cnBoxUXFLeft, cnBoxUXFTop};
	rect.right = rect.left + cnBoxUXFWidth;
	rect.bottom = rect.top + cnBoxUXFHigh;
	DWORD dwS = WS_CHILD|WS_BORDER|ES_AUTOHSCROLL|WS_VISIBLE;
	BOOL bEC = reccUXF[0].Create(dwS, rect, ps.GetPage(0), ID_UFX + 0);
	InvalidateRect(&rect);

	// Окошко с энтальпией УХФ
	RECT rectE = {cnBoxEnthalpyUXFLeft, cnBoxEnthalpyUXFTop};
	rectE.right = rectE.left + cnBoxEnthalpyUXFWidth;
	rectE.bottom = rectE.top + cnBoxEnthalpyUXFHigh;
	DWORD dwSE = WS_CHILD|WS_BORDER|ES_AUTOHSCROLL|WS_VISIBLE;
	BOOL bECE = recEnthalpyUXF[0].Create(dwSE, rectE, ps.GetPage(0), ID_ENT_UFX + 0);
	InvalidateRect(&rectE);

	// Static
	RECT rectStUXF = {cnStUXFLeft, cnStUXFTop};
	rectStUXF.right = rectStUXF.left + cnStUXFWidth;
	rectStUXF.bottom = rectStUXF.top + cnStUXFHigh;
	stUXF[0].Create(cnstrUXF, WS_CHILD|WS_VISIBLE|SS_RIGHT|SS_NOTIFY,
		rectStUXF, ps.GetPage(0));

	// Static
	RECT rectStEUXF = {cnStEnthalpyUXFLeft, cnStEnthalpyUXFTop};
	rectStEUXF.right = rectStEUXF.left + cnStEnthalpyUXFWidth;
	rectStEUXF.bottom = rectStEUXF.top + cnStEnthalpyUXFHigh;
	stEnthalpyUXF[0].Create(cnstrEnthalpyUXF, WS_CHILD|WS_VISIBLE|SS_RIGHT|SS_NOTIFY,
		rectStEUXF, ps.GetPage(0));

	reccUXF[0].EnableWindow(FALSE);
	recEnthalpyUXF[0].EnableWindow(FALSE);
	stUXF[0].EnableWindow(FALSE);
	stEnthalpyUXF[0].EnableWindow(FALSE);
	ps.EnableWindow(FALSE);
}

void CThermoCalcView::DrawNozzle(RECT *pMainRect, CDC* pDC) {
}

BOOL CThermoCalcView::CreatePSCalcFlow() {
	DLGITEMTEMPLATE dit[MAX_Items];
	char szFontDlgName[MAX_len];
	WORD wFontDlgSize;
	char szItemInitText[MAX_Items][MAX_len];
	char szItemClassName[MAX_Items][MAX_len];
	int nItemSysClass[MAX_Items];

	WORD old_Pa;
	WORD old_F;
	WORD old_IS;
	WORD old_V;
	WORD old_EPS;

	// Создание листа закладок и присоединие страниц
	psCalcFlow.Construct("Caption PSCalcFlow", this);
	psCalcFlow.m_psh.dwFlags |= PSH_DEFAULT;

	// Обнулим кол-во элементов в шаблоне диалога
	old_Pa = SetCountItemInDlg(IDD_ForCalcFlow_Pa,0);
	ppCalcFlow_Pa.Construct(IDD_ForCalcFlow_Pa);//	ppCalcFlow_Pa.Construct(IDD_PP_TAMP1);
	ppCalcFlow_Pa.m_psp.dwFlags |= PSP_PREMATURE|PSP_USETITLE;
	ppCalcFlow_Pa.m_psp.pszTitle = "Pа - давление в сечении";
	psCalcFlow.AddPage(&ppCalcFlow_Pa);

	old_F = SetCountItemInDlg(IDD_ForCalcFlow_F,0);
	ppCalcFlow_F.Construct(IDD_ForCalcFlow_F);//	ppCalcFlow_F.Construct(IDD_PP_TAMP1);
	ppCalcFlow_F.m_psp.dwFlags |= PSP_PREMATURE|PSP_USETITLE;
	ppCalcFlow_F.m_psp.pszTitle = "F/Fkp - степень расширения сопла";
	psCalcFlow.AddPage(&ppCalcFlow_F);

	old_IS = SetCountItemInDlg(IDD_ForCalcFlow_IS,0);
	ppCalcFlow_IS.Construct(IDD_ForCalcFlow_IS);//	ppCalcFlow_IS.Construct(IDD_PP_TAMP1);
	ppCalcFlow_IS.m_psp.dwFlags |= PSP_PREMATURE|PSP_USETITLE;
	ppCalcFlow_IS.m_psp.pszTitle = "IS диаграмма";
	psCalcFlow.AddPage(&ppCalcFlow_IS);

	old_V = SetCountItemInDlg(IDD_ForCalcFlow_V,0);
	ppCalcFlow_V.Construct(IDD_ForCalcFlow_V);//	ppCalcFlow_V.Construct(IDD_PP_TAMP1);
	ppCalcFlow_V.m_psp.dwFlags |= PSP_PREMATURE|PSP_USETITLE;
	ppCalcFlow_V.m_psp.pszTitle = "При постоянном объеме";
	psCalcFlow.AddPage(&ppCalcFlow_V);

	old_EPS = SetCountItemInDlg(IDD_ForCalcFlow_EPS,0);
	ppCalcFlow_EPS.Construct(IDD_ForCalcFlow_EPS);//	ppCalcFlow_EPS.Construct(IDD_PP_TAMP1);
	ppCalcFlow_EPS.m_psp.dwFlags |= PSP_PREMATURE|PSP_USETITLE;
	ppCalcFlow_EPS.m_psp.pszTitle = "EPS - степень расширения газа в сопле";
	psCalcFlow.AddPage(&ppCalcFlow_EPS);

	psCalcFlow.Create(this, WS_CHILD|WS_VISIBLE|WM_NOTIFY);//, WS_EX_DLGMODALFRAME);
	SetCountItemInDlg(IDD_ForCalcFlow_Pa,old_Pa);
	SetCountItemInDlg(IDD_ForCalcFlow_F,old_F);
	SetCountItemInDlg(IDD_ForCalcFlow_IS,old_IS);
	SetCountItemInDlg(IDD_ForCalcFlow_V,old_V);
	SetCountItemInDlg(IDD_ForCalcFlow_EPS,old_EPS);

	CPropertyPageChem *arrayP[cnQuantityPagesCalcFlow];
	arrayP[0] = &ppCalcFlow_Pa;
	arrayP[1] = &ppCalcFlow_F;
	arrayP[2] = &ppCalcFlow_IS;
	arrayP[3] = &ppCalcFlow_V;
	arrayP[4] = &ppCalcFlow_EPS;

	FieldInputH *arrayFIH[cnQuantityPagesCalcFlow];;
	arrayFIH[0] = &fihPressureInSection;
	arrayFIH[1] = &fihPowerExpNozzle;
	arrayFIH[2] = &fihIS;
	arrayFIH[3] = &fihV;
	arrayFIH[4] = &fihEPS;

	int arrayMaxI[cnQuantityPagesCalcFlow];
	arrayMaxI[0] = MAX_Sections;
	arrayMaxI[1] = MaxQuantityFIHBox;
	arrayMaxI[2] = MaxQuantityFIHBox;
	arrayMaxI[3] = MaxQuantityFIHBox;
	arrayMaxI[4] = MaxQuantityFIHBox;

	int arrayID[cnQuantityPagesCalcFlow];;
	arrayID[0] = IDD_ForCalcFlow_Pa;
	arrayID[1] = IDD_ForCalcFlow_F;
	arrayID[2] = IDD_ForCalcFlow_IS;
	arrayID[3] = IDD_ForCalcFlow_V;
	arrayID[4] = IDD_ForCalcFlow_EPS;

	CStaticChem	*arraySt[cnQuantityPagesCalcFlow];
	arraySt[0] = &st_Pa;
	arraySt[1] = &st_F;
	arraySt[2] = &st_IS;
	arraySt[3] = &st_V;
	arraySt[4] = &st_EPS;

	CButtonChem *arrayBa[cnQuantityPagesCalcFlow];
	arrayBa[0] = &m_ButtonAppendPressureInSection;
	arrayBa[1] = &m_ButtonAppendPowerExpNozzle;
	arrayBa[2] = &m_ButtonAppendIS;
	arrayBa[3] = &m_ButtonAppendV;
	arrayBa[4] = &m_ButtonAppendEPS;

	CButtonChem *arrayBd[cnQuantityPagesCalcFlow];
	arrayBd[0] = &m_ButtonDisPressureInSection;
	arrayBd[1] = &m_ButtonDisPowerExpNozzle;
	arrayBd[2] = &m_ButtonDisIS;
	arrayBd[3] = &m_ButtonDisV;
	arrayBd[4] = &m_ButtonDisEPS;

	int arrayIDBa[cnQuantityPagesCalcFlow];;
	arrayIDBa[0] = IDC_ButtonAppendPressureInSection;
	arrayIDBa[1] = IDC_ButtonAppendPowerExpNozzle;
	arrayIDBa[2] = IDC_ButtonAppendIS;
	arrayIDBa[3] = IDC_ButtonAppendV;
	arrayIDBa[4] = IDC_ButtonAppendEPS;

	int arrayIDBd[cnQuantityPagesCalcFlow];;
	arrayIDBd[0] = IDC_ButtonDisPressureInSection;
	arrayIDBd[1] = IDC_ButtonDisPowerExpNozzle;
	arrayIDBd[2] = IDC_ButtonDisIS;
	arrayIDBd[3] = IDC_ButtonDisV;
	arrayIDBd[4] = IDC_ButtonDisEPS;

	// Radio button
	CButtonChem *arrayRBV[cnQuantityPagesCalcFlow];
	arrayRBV[0] = &m_ButtonDisPressureInSection;
	arrayRBV[1] = &m_ButtonDisPowerExpNozzle;
	arrayRBV[2] = &m_RButtonValueIS;
	arrayRBV[3] = &m_ButtonDisV;
	arrayRBV[4] = &m_ButtonDisEPS;

	CButtonChem *arrayRBS[cnQuantityPagesCalcFlow];
	arrayRBS[0] = &m_ButtonAppendPressureInSection;
	arrayRBS[1] = &m_ButtonAppendPowerExpNozzle;
	arrayRBS[2] = &m_RButtonStepIS;
	arrayRBS[3] = &m_ButtonAppendV;
	arrayRBS[4] = &m_ButtonAppendEPS;


	RECT rect;
// Раскидаем кнопки и текст по закладкам
	for (int i1 = 0; i1<cnQuantityPagesCalcFlow;i1++) {
		if (!arrayP[i1]) 
			continue;
		memset(nItemSysClass, 0, sizeof(nItemSysClass));
		int nItems = TakeDlg (arrayID[i1], &dt, &wFontDlgSize, szFontDlgName, 
				dit, szItemInitText, szItemClassName, nItemSysClass);

		for (int i = 0; i<nItems; i++)
			switch (nItemSysClass[i]){
				case Item_Button:
					if (!arrayFIH[i1]) 
						continue;
					rect.left = dit[i].x;
					rect.top = dit[i].y;
					rect.right = rect.left + dit[i].cx;
					rect.bottom = rect.top + dit[i].cy;
					arrayP[i1] ->MapDialogRect(&rect);
					// Если кнопка кружок RADIOBUTTON
					// две кнопки в одной групе для автоматического выделения
					if (dit[i].style & BS_AUTORADIOBUTTON) {
						if (!strcmp(szItemInitText[i],"Значения")) {
							arrayRBV[i1] ->Create(szItemInitText[i], BS_RADIOBUTTON|WS_CHILD|WS_VISIBLE|BS_NOTIFY
									,rect , arrayP[i1], IDC_ButtonValueIS);
							arrayRBV[i1] ->SetCheck(1);
						}
						else
							arrayRBS[i1] ->Create(szItemInitText[i], BS_RADIOBUTTON|WS_CHILD|WS_VISIBLE|BS_NOTIFY
									,rect , arrayP[i1], IDC_ButtonStepIS);
					}
					// Обычная кнопка PUSHBUTTON
					else {
						if (!strcmp(szItemInitText[i],"+"))
							arrayBa[i1] ->Create(szItemInitText[i], WS_CHILD|WS_VISIBLE|BS_NOTIFY
									,rect , arrayP[i1], arrayIDBa[i1]);
						else
							arrayBd[i1] ->Create(szItemInitText[i], WS_CHILD|WS_VISIBLE|BS_NOTIFY
									,rect , arrayP[i1], arrayIDBd[i1]);
					}

//					m_ButtonAppendPowerExpNozzle.SetFont(&F);
//					pF = m_ButtonAppendPowerExpNozzle.GetFont();
					break;
				case Item_Static:
					if (!arraySt[i1]) 
						continue;
					rect.left = dit[i].x;
					rect.top = dit[i].y;
					rect.right = rect.left + dit[i].cx;
					rect.bottom = rect.top + dit[i].cy;
					arrayP[i1] ->MapDialogRect(&rect);
					// Static
					arraySt[i1] ->Create(szItemInitText[i], dit[i].style,//WS_CHILD|WS_VISIBLE|SS_RIGHT|SS_NOTIFY,
							rect, arrayP[i1]);
//					pF = st_F.GetFont();
					break;
				case Item_Edit:	// Объекты, указ. на которые хранятся в массиве arrayFIH[] 
										// уже созданы, нужно только правильно раскидать их по закладке
					if (!arrayFIH[i1]) 
						continue;
					rect.left = dit[i].x;
					rect.top = dit[i].y;
					rect.right = rect.left + dit[i].cx;
					rect.bottom = rect.top + dit[i].cy;
					arrayP[i1] ->MapDialogRect(&rect);
					arrayFIH[i1] ->Coordinate(rect.left, rect.top, rect.right-rect.left,arrayMaxI[i1]);
					arrayFIH[i1] ->MaybeZeroItems(TRUE);
					break;
			}
	}
/*	for (i1 = 0; i1<cnQuantityPagesCalcFlow;i1++) {		if (!arrayP[i1]) continue;
		arrayP[i1] ->GetWindowRect(&rect);		arrayP[i1] ->MoveWindow(rect.left, rect.top, rect.right-rect.left,rect.bottom-rect.top);
	}*/
	psCalcFlow.SetActivePage(0);

	// Передвинем на заданное место
	psCalcFlow.GetWindowRect(&rect);
	psCalcFlow.MoveWindow(cnPSCalcFlowLeft, cnPSCalcFlowTop, 
			rect.right-rect.left-6,rect.bottom-rect.top-6);

	return TRUE;
}
