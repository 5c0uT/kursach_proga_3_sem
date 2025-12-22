// CObjectVal.cpp: файл реализации
//

#include "pch.h"
#include "Kursach.h"
#include "afxdialogex.h"
#include "CObjectVal.h"
#include "MainFrm.h"
#include "Kursach_Doc.h" 

// Диалоговое окно CObjectVal

IMPLEMENT_DYNAMIC(CObjectVal, CDialogEx)

CObjectVal::CObjectVal(CWnd* pParent /*=nullptr*/)
	: CDialogEx(IDD_Val, pParent)
	, ValL(0)
{

}

CObjectVal::~CObjectVal()
{
}

void CObjectVal::DoDataExchange(CDataExchange* pDX)
{
	CDialogEx::DoDataExchange(pDX);
	DDX_Text(pDX, IDC_ValL, ValL);
}


BEGIN_MESSAGE_MAP(CObjectVal, CDialogEx)
	ON_BN_CLICKED(IDOK, &CObjectVal::OnBnClickedOk)
END_MESSAGE_MAP()


// Обработчики сообщений CObjectVal

void CObjectVal::OnBnClickedOk()
{
	CDialogEx::OnOK();
	CKursachApp* pApp = (CKursachApp*)AfxGetApp();
	CMainFrame* pMainFrame = (CMainFrame*)pApp->GetMainWnd();
	CKursachDoc* pDoc = (CKursachDoc*)pMainFrame->GetActiveDocument();
	if (pDoc->L / 2 + 10 > ValL)
	{
		AfxMessageBox(L"Длина вала не должна быть меньше размера муфты");
		return;
	}
	else
	{
		pDoc->lV = ValL;
		pDoc->Val_Check = true;
		CDialogEx::OnOK();
	}
}
