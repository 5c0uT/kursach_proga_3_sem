#include "pch.h"

IMPLEMENT_DYNCREATE(CMyFreeView, CTreeView)

CMyFreeView::CMyFreeView()
{
}

CMyFreeView::~CMyFreeView()
{
}

BEGIN_MESSAGE_MAP(CMyFreeView, CTreeView)
	ON_WM_CREATE()
	ON_WM_LBUTTONUP()
	ON_WM_LBUTTONDBLCLK()
END_MESSAGE_MAP()

#ifdef _DEBUG
void CMyFreeView::AssertValid() const
{
	CTreeView::AssertValid();
}

void CMyFreeView::Dump(CDumpContext& dc) const
{
	CTreeView::Dump(dc);
}
#endif

int CMyFreeView::OnCreate(LPCREATESTRUCT lpCreateStruct)
{
	lpCreateStruct->style |= TVS_HASLINES | TVS_HASBUTTONS | TVS_LINESATROOT | TVS_SHOWSELALWAYS;
	if (CTreeView::OnCreate(lpCreateStruct) == -1)
		return -1;

	return 0;
}

void CMyFreeView::FillTree()
{
	CTreeCtrl& tree = GetTreeCtrl();
	tree.DeleteAllItems();

	m_hSborka =  tree.InsertItem(L"Cборка", -1, -1, NULL,      TVI_FIRST);
	m_hMufta =   tree.InsertItem(L"Муфта",  -1, -1, m_hSborka, TVI_FIRST);
	m_hVal =     tree.InsertItem(L"Вал",    -1, -1, m_hSborka, TVI_FIRST);
	m_hShponka = tree.InsertItem(L"Шпонка", -1, -1, m_hSborka, TVI_FIRST);
	m_hVint =    tree.InsertItem(L"Винт",   -1, -1, m_hSborka, TVI_FIRST);
	m_hKolco =   tree.InsertItem(L"Кольцо", -1, -1, m_hSborka, TVI_FIRST);
}

void CMyFreeView::OnLButtonUp(UINT nFlags, CPoint point)
{
	CTreeView::OnLButtonUp(nFlags, point);

	CTreeCtrl& tree = GetTreeCtrl();
	CRect rc;

	tree.GetItemRect(m_hMufta, &rc, false);
	if (rc.PtInRect(point))
	{
		tree.SelectItem(m_hMufta);
	}
	tree.GetItemRect(m_hVal, &rc, false);
	if (rc.PtInRect(point))
	{
		tree.SelectItem(m_hVal);
	}
	tree.GetItemRect(m_hShponka, &rc, false);
	if (rc.PtInRect(point))
	{
		tree.SelectItem(m_hShponka);
	}
	tree.GetItemRect(m_hVint, &rc, false);
	if (rc.PtInRect(point))
	{
		tree.SelectItem(m_hVint);
	}
	tree.GetItemRect(m_hKolco, &rc, false);
	if (rc.PtInRect(point))
	{
		tree.SelectItem(m_hKolco);
	}

	if (tree.GetSelectedItem() == m_hSborka)
	{
		m_pDoc->m_bSborka =  TRUE;
		m_pDoc->m_bMufta =   FALSE;
		m_pDoc->m_bVal =     FALSE;
		m_pDoc->m_bShponka = FALSE;
		m_pDoc->m_bVint =    FALSE;
		m_pDoc->m_bKolco =   FALSE;
	}
	if (tree.GetSelectedItem() == m_hMufta)
	{
		m_pDoc->m_bSborka =  FALSE;
		m_pDoc->m_bMufta =   TRUE;
		m_pDoc->m_bVal =     FALSE;
		m_pDoc->m_bShponka = FALSE;
		m_pDoc->m_bVint =    FALSE;
		m_pDoc->m_bKolco =   FALSE;
	}
	if (tree.GetSelectedItem() == m_hVal)
	{
		m_pDoc->m_bSborka =  FALSE;
		m_pDoc->m_bMufta =   FALSE;
		m_pDoc->m_bVal =     TRUE;
		m_pDoc->m_bShponka = FALSE;
		m_pDoc->m_bVint =    FALSE;
		m_pDoc->m_bKolco =   FALSE;
	}
	if (tree.GetSelectedItem() == m_hShponka)
	{
		m_pDoc->m_bSborka =  FALSE;
		m_pDoc->m_bMufta =   FALSE;
		m_pDoc->m_bVal =     FALSE;
		m_pDoc->m_bShponka = TRUE;
		m_pDoc->m_bVint =    FALSE;
		m_pDoc->m_bKolco =   FALSE;
	}
	if (tree.GetSelectedItem() == m_hVint)
	{
		m_pDoc->m_bSborka =  FALSE;
		m_pDoc->m_bMufta =   FALSE;
		m_pDoc->m_bVal =     FALSE;
		m_pDoc->m_bShponka = FALSE;
		m_pDoc->m_bVint =    TRUE;
		m_pDoc->m_bKolco =   FALSE;
	}
	if (tree.GetSelectedItem() == m_hKolco)
	{
		m_pDoc->m_bSborka =  FALSE;
		m_pDoc->m_bMufta =   FALSE;
		m_pDoc->m_bVal =     FALSE;
		m_pDoc->m_bShponka = FALSE;
		m_pDoc->m_bVint =    FALSE;
		m_pDoc->m_bKolco =   TRUE;
	}

	m_pDoc->pView->Invalidate();
}

void CMyFreeView::OnLButtonDblClk(UINT nFlags, CPoint point)
{
	CTreeView::OnLButtonDblClk(nFlags, point);

	CKursachApp* pApp =      (CKursachApp*)AfxGetApp();
	CMainFrame* pMainFrame = (CMainFrame*)pApp->GetMainWnd();
	CKursachDoc* pDoc =      (CKursachDoc*)pMainFrame->GetActiveDocument();

	CTreeCtrl& tree = GetTreeCtrl();
	CRect rc, rc1, rc2, rc3;

	tree.GetItemRect(m_hMufta,   &rc,  false);
	tree.GetItemRect(m_hVal,     &rc1, false);
	tree.GetItemRect(m_hShponka, &rc2, false);
	tree.GetItemRect(m_hSborka,  &rc3, false);

	if (rc.PtInRect(point)) {
		tree.SelectItem(m_hMufta);
		CObjectMufta dlg;
		dlg.DoModal();
	}
	else if (rc1.PtInRect(point) && pDoc->Mufta_Check) {
		tree.SelectItem(m_hVal);
		CObjectVal dlg;
		dlg.DoModal();
	}
	else if (rc2.PtInRect(point) && pDoc->Mufta_Check) {
		tree.SelectItem(m_hShponka);
		CObjectShponka dlg;
		dlg.DoModal();
	}
	else if (rc3.PtInRect(point))
	{
	}
	else
	{
		AfxMessageBox(L"Для начала укажите параметры муфты");
	}
}
