// DlgEmptyDoc.cpp : implementation file
//

#include "stdafx.h"
#include "ThermoCalc.h"
#include "DlgEmptyDoc.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

/////////////////////////////////////////////////////////////////////////////
// DlgEmptyDoc

IMPLEMENT_DYNCREATE(DlgEmptyDoc, CDocument)

DlgEmptyDoc::DlgEmptyDoc()
{
}

BOOL DlgEmptyDoc::OnNewDocument()
{
	if (!CDocument::OnNewDocument())
		return FALSE;
	return TRUE;
}

DlgEmptyDoc::~DlgEmptyDoc()
{
}


BEGIN_MESSAGE_MAP(DlgEmptyDoc, CDocument)
	//{{AFX_MSG_MAP(DlgEmptyDoc)
		// NOTE - the ClassWizard will add and remove mapping macros here.
	//}}AFX_MSG_MAP
END_MESSAGE_MAP()

/////////////////////////////////////////////////////////////////////////////
// DlgEmptyDoc diagnostics

#ifdef _DEBUG
void DlgEmptyDoc::AssertValid() const
{
	CDocument::AssertValid();
}

void DlgEmptyDoc::Dump(CDumpContext& dc) const
{
	CDocument::Dump(dc);
}
#endif //_DEBUG

/////////////////////////////////////////////////////////////////////////////
// DlgEmptyDoc serialization

void DlgEmptyDoc::Serialize(CArchive& ar)
{
	if (ar.IsStoring())
	{
		// TODO: add storing code here
	}
	else
	{
		// TODO: add loading code here
	}
}

/////////////////////////////////////////////////////////////////////////////
// DlgEmptyDoc commands
