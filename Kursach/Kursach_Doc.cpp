#include "pch.h"

IMPLEMENT_DYNCREATE(CKursachDoc, CDocument)

BEGIN_MESSAGE_MAP(CKursachDoc, CDocument)
END_MESSAGE_MAP()

CKursachDoc::CKursachDoc() noexcept
{
	m_bSborka = m_bMufta = m_bVal = m_bShponka = m_bKolco = m_bVint = false;
	pView =     nullptr;
	pTree =     nullptr;
}

CKursachDoc::~CKursachDoc()
{
}

BOOL CKursachDoc::OnNewDocument()
{
	if (!CDocument::OnNewDocument())
		return FALSE;

	if (pTree)
		pTree->FillTree();

	return TRUE;
}

void CKursachDoc::Serialize(CArchive& ar)
{
	if (ar.IsStoring())
	{
	}
	else
	{
	}
}

#ifdef SHARED_HANDLERS

void CKursachDoc::OnDrawThumbnail(CDC& dc, LPRECT lprcBounds)
{
	dc.FillSolidRect(lprcBounds, RGB(255, 255, 255));
	CString strText = _T("TODO: implement thumbnail drawing here");
	LOGFONT lf;

	CFont* pDefaultGUIFont = CFont::FromHandle((HFONT)GetStockObject(DEFAULT_GUI_FONT));
	pDefaultGUIFont->GetLogFont(&lf);
	lf.lfHeight = 36;

	CFont fontDraw;
	fontDraw.CreateFontIndirect(&lf);

	CFont* pOldFont = dc.SelectObject(&fontDraw);
	dc.DrawText(strText, lprcBounds, DT_CENTER | DT_WORDBREAK);
	dc.SelectObject(pOldFont);
}

void CKursachDoc::InitializeSearchContent()
{
	CString strSearchContent;
	SetSearchContent(strSearchContent);
}

void CKursachDoc::SetSearchContent(const CString& value)
{
	if (value.IsEmpty())
	{
		RemoveChunk(PKEY_Search_Contents.fmtid, PKEY_Search_Contents.pid);
	}
	else
	{
		CMFCFilterChunkValueImpl* pChunk = nullptr;
		ATLTRY(pChunk = new CMFCFilterChunkValueImpl);
		if (pChunk != nullptr)
		{
			pChunk->SetTextValue(PKEY_Search_Contents, value, CHUNK_TEXT);
			SetChunkValue(pChunk);
		}
	}
}

#endif // SHARED_HANDLERS

#ifdef _DEBUG
void CKursachDoc::AssertValid() const
{
	CDocument::AssertValid();
}

void CKursachDoc::Dump(CDumpContext& dc) const
{
	CDocument::Dump(dc);
}
#endif
