#include "pch.h"

IMPLEMENT_DYNAMIC(CObjectShponka, CDialogEx)

CObjectShponka::CObjectShponka(CWnd* pParent)
	: CDialogEx(IDD_Shponka, pParent)
	, ShponkaB(_T(""))
{
}

CObjectShponka::~CObjectShponka()
{
}

void CObjectShponka::DoDataExchange(CDataExchange* pDX)
{
	CDialogEx::DoDataExchange(pDX);
	DDX_Control(pDX, IDC_ShponkaH, ShponkaH);
	DDX_Control(pDX, IDC_ShponkaW, ShponkaW);
	DDX_Text(pDX,    IDC_ShponkaB, ShponkaB);

	UpdateComboboxesFromB();
}

BEGIN_MESSAGE_MAP(CObjectShponka, CDialogEx)
	ON_CBN_SELCHANGE(IDC_ShponkaW, &CObjectShponka::OnCbnSelchangeShponkaw)
	ON_CBN_SELCHANGE(IDC_ShponkaH, &CObjectShponka::OnCbnSelchangeShponkah)
	ON_BN_CLICKED(IDOK,            &CObjectShponka::OnBnClickedOk)
END_MESSAGE_MAP()

void CObjectShponka::OnCbnSelchangeShponkaw()
{
	int selectedIndex = ShponkaW.GetCurSel();
	if (selectedIndex != CB_ERR) {
		CString selectedText;
		ShponkaW.GetLBText(selectedIndex, selectedText);
		double dValue = _ttof(selectedText);
		FilterComboboxes(dValue, 2);
	}
}

void CObjectShponka::OnCbnSelchangeShponkah()
{
	int selectedIndex = ShponkaH.GetCurSel();
	if (selectedIndex != CB_ERR) {
		CString selectedText;
		ShponkaH.GetLBText(selectedIndex, selectedText);
		double dValue = _ttof(selectedText);
		FilterComboboxes(dValue, 1);
	}
}

void CObjectShponka::UpdateComboboxesFromB()
{
	CKursachApp* pApp = (CKursachApp*)AfxGetApp();
	CMainFrame* pMainFrame = (CMainFrame*)pApp->GetMainWnd();
	CKursachDoc* pDoc = (CKursachDoc*)pMainFrame->GetActiveDocument();

	if (pDoc == nullptr) return;

	double bValue = pDoc->b;

	for (int row = 0; row < 11; row++) {
		if (abs(ShponkaVar[row][0] - bValue) < 0.001) {
			CString hValue, wValue;

			hValue.Format(_T("%.1f мм"), ShponkaVar[row][1]);
			ShponkaH.AddString(hValue);

			wValue.Format(_T("%.1f мм"), ShponkaVar[row][2]);
			ShponkaW.AddString(wValue);
		}
	}

	if (ShponkaH.GetCount() > 0) ShponkaH.SetCurSel(0);
	if (ShponkaW.GetCount() > 0) ShponkaW.SetCurSel(0);

	CStatic* pStatic = (CStatic*)GetDlgItem(IDC_ShponkaB);
	if (pStatic != nullptr) {
		CString newText;
		newText.Format(_T("%.1f мм"), bValue);
		pStatic->SetWindowText(newText);
	}
}

void CObjectShponka::FilterComboboxes(double znach, int index)
{
	int foundRow = 0;
	for (int row = 0; row < 11; row++) {
		if (abs(ShponkaVar[row][index] - znach) < 0.001) {
			foundRow = row;
			break;
		}
	}

	SetComboBoxValue(ShponkaH, ShponkaVar[foundRow][1]);
	SetComboBoxValue(ShponkaW, ShponkaVar[foundRow][2]);

	m_currentRow = foundRow;
}

void CObjectShponka::SetComboBoxValue(CComboBox& comboBox, double value)
{
	CString searchText;
	searchText.Format(_T("%.1f мм"), value);
	int index = comboBox.FindStringExact(-1, searchText);
	if (index != CB_ERR) {
		comboBox.SetCurSel(index);
		return;
	}
}

void CObjectShponka::OnBnClickedOk()
{
	CDialogEx::OnOK();

	CKursachApp* pApp =      (CKursachApp*)AfxGetApp();
	CMainFrame* pMainFrame = (CMainFrame*)pApp->GetMainWnd();
	CKursachDoc* pDoc =      (CKursachDoc*)pMainFrame->GetActiveDocument();

	if (pDoc) {
		pDoc->ShponkaH = ShponkaVar[m_currentRow][1];
		pDoc->ShponkaW = ShponkaVar[m_currentRow][2];

		pDoc->Shponka_Check = true;
	}
}
