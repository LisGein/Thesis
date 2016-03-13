// StaticChem.cpp : implementation file
//

#include "stdafx.h"
#include "ThermoCalc.h"
#include "StaticChem.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

/////////////////////////////////////////////////////////////////////////////
// CStaticChem

CStaticChem::CStaticChem() {	pFont = 0;	}

CStaticChem::~CStaticChem() {
	if (pFont) {
		pFont->DeleteObject();
		delete pFont;
		pFont = 0;
	}
}

BEGIN_MESSAGE_MAP(CStaticChem, CStatic)
	//{{AFX_MSG_MAP(CStaticChem)
	ON_WM_CTLCOLOR_REFLECT()
	//}}AFX_MSG_MAP
END_MESSAGE_MAP()

/////////////////////////////////////////////////////////////////////////////
// CStaticChem message handlers
HBRUSH CStaticChem::CtlColor(CDC* pDC, UINT nCtlColor) {
	// TODO: Change any attributes of the DC here
	if (!pFont) {
// Declare the variables needed
#ifdef _DEBUG
    CMemoryState oldMemState, newMemState, diffMemState;
    oldMemState.Checkpoint();
#endif
		pFont = new CFont;
#ifdef _DEBUG
    newMemState.Checkpoint();
    if( diffMemState.Difference( oldMemState, newMemState ) )
    {
        TRACE( "Memory leaked!\n" );
    }
#endif
	
		LOGFONT lf;
		memset(&lf,0,sizeof(LOGFONT));
	/*	CFontDialog fdFont( NULL, CF_SCREENFONTS, NULL, this);
		if (fdFont.DoModal() == IDOK) {
			fdFont.GetCurrentFont(&lf);
			pFont ->CreateFontIndirect(&lf);
			pDC->SelectObject(pFont);
		}//*/
		lf.lfHeight = -cnFontSizeStaticChem;
		lf.lfWeight = FW_NORMAL;
		lf.lfCharSet = RUSSIAN_CHARSET;
		lf.lfOutPrecision = OUT_STRING_PRECIS;
		lf.lfClipPrecision = CLIP_STROKE_PRECIS;
		lf.lfQuality = DRAFT_QUALITY;
		lf.lfPitchAndFamily = VARIABLE_PITCH|FF_SWISS;
		strcpy(lf.lfFaceName, "MS Sans Serif");
		pFont ->CreateFontIndirect(&lf);
	}
	pDC->SelectObject(pFont);
	CGdiObject * pGO_oldBrush = pDC ->SelectStockObject(COLOR_WINDOW);
	CGdiObject * pGO_oldPen = pDC ->SelectStockObject(BLACK_PEN);

// TODO: Return a non-NULL brush if the parent's handler should not be called
//	return GetSysColorBrush(COLOR_3DFACE);
	return NULL;
}
