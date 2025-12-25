#include "pch.h"

IMPLEMENT_DYNAMIC(CObjectVal, CDialogEx)

CObjectVal::CObjectVal(CWnd* pParent)
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

void CObjectVal::OnBnClickedOk()
{
    // Сначала получаем данные с контролей
    if (!UpdateData(TRUE))
        return;

    CKursachApp* pApp = (CKursachApp*)AfxGetApp();
    CMainFrame* pMainFrame = (CMainFrame*)pApp->GetMainWnd();
    CKursachDoc* pDoc = (CKursachDoc*)pMainFrame->GetActiveDocument();

    // Проверка: Длина вала должна быть >= размера муфты/2 + 10
    if (pDoc->L / 2 + 10 > ValL)
    {
        AfxMessageBox(L"Длина вала не должна быть меньше размера муфты");
        return;
    }

    // Проверка: Минимальное значение L = 28
    const double MIN_L_VALUE = 28.0;
    if (ValL < MIN_L_VALUE)
    {
        CString errorMsg;
        errorMsg.Format(L"Значение L не может быть меньше %.0f!", MIN_L_VALUE);
        AfxMessageBox(errorMsg, MB_ICONEXCLAMATION | MB_OK);
        return;
    }

    pDoc->lV =        ValL;
    pDoc->Val_Check = true;
    CDialogEx::OnOK();  // Теперь закрываем только если все ОК
}

BOOL CObjectVal::OnInitDialog()
{
	CDialogEx::OnInitDialog();

	// TODO:  Добавить дополнительную инициализацию
	CKursachApp* pApp =      (CKursachApp*)AfxGetApp();
	CMainFrame* pMainFrame = (CMainFrame*)pApp->GetMainWnd();
	CKursachDoc* pDoc =      (CKursachDoc*)pMainFrame->GetActiveDocument();
	if (pDoc->lV != 0)
	{
		ValL = pDoc->lV;
		UpdateData(FALSE);
		CEdit* pEdit = (CEdit*)GetDlgItem(IDC_ValL);
		CString newText;
		newText.Format(_T("%d"), ValL);
		pEdit->SetWindowText(newText);

	}
	return TRUE;  // return TRUE unless you set the focus to a control
	// Исключение: страница свойств OCX должна возвращать значение FALSE
}
