// CObjectMufta.cpp: файл реализации
//

#include "pch.h"
#include "Kursach.h"
#include "afxdialogex.h"
#include "CObjectMufta.h"
#include "MainFrm.h"
#include "Kursach_Doc.h" 

// Диалоговое окно CObjectMufta

IMPLEMENT_DYNAMIC(CObjectMufta, CDialogEx)

CObjectMufta::CObjectMufta(CWnd* pParent /*=nullptr*/)
	: CDialogEx(IDD_Mufta, pParent)
{
	
}

CObjectMufta::~CObjectMufta()
{
}

void CObjectMufta::DoDataExchange(CDataExchange* pDX)
{
	CDialogEx::DoDataExchange(pDX);

	DDX_Control(pDX, IDC_b1, Mufta_b1);
	DDX_Control(pDX, IDC_b, Mufta_b);
	DDX_Control(pDX, IDC_c, Mufta_c);
	DDX_Control(pDX, IDC_c1, Mufta_c1);
	DDX_Control(pDX, IDC_d, Mufta_d);
	DDX_Control(pDX, IDC_d1, Mufta_d1);
	DDX_Control(pDX, IDC_DD, Mufta_D);
	DDX_Control(pDX, IDC_DD1, Mufta_D1);
	DDX_Control(pDX, IDC_dt2, Mufta_dt2);
	DDX_Control(pDX, IDC_L, Mufta_L);
	DDX_Control(pDX, IDC_ll, Mufta_l);
	DDX_Control(pDX, IDC_r, Mufta_r);

    FillComboboxes();
}

BEGIN_MESSAGE_MAP(CObjectMufta, CDialogEx)
	ON_CBN_SELCHANGE(IDC_d, &CObjectMufta::OnCbnSelchanged)
	ON_CBN_SELCHANGE(IDC_dt2, &CObjectMufta::OnCbnSelchangedt2)
	ON_CBN_SELCHANGE(IDC_b, &CObjectMufta::OnCbnSelchangeb)
	ON_CBN_SELCHANGE(IDC_b1, &CObjectMufta::OnCbnSelchangeb1)
	ON_CBN_SELCHANGE(IDC_d1, &CObjectMufta::OnCbnSelchanged1)
	ON_CBN_SELCHANGE(IDC_DD, &CObjectMufta::OnCbnSelchangeDd)
	ON_CBN_SELCHANGE(IDC_DD1, &CObjectMufta::OnCbnSelchangeDd1)
	ON_CBN_SELCHANGE(IDC_L, &CObjectMufta::OnCbnSelchangeL)
	ON_CBN_SELCHANGE(IDC_ll, &CObjectMufta::OnCbnSelchangell)
	ON_CBN_SELCHANGE(IDC_r, &CObjectMufta::OnCbnSelchanger)
	ON_CBN_SELCHANGE(IDC_c, &CObjectMufta::OnCbnSelchangec)
	ON_CBN_SELCHANGE(IDC_c1, &CObjectMufta::OnCbnSelchangec1)
    ON_BN_CLICKED(IDOK, &CObjectMufta::OnBnClickedOk)
END_MESSAGE_MAP()


// Обработчики сообщений CObjectMufta

void CObjectMufta::OnCbnSelchanged()
{
	// TODO: добавьте свой код обработчика уведомлений
    int selectedIndex = Mufta_d.GetCurSel();
    if (selectedIndex != CB_ERR) {
        CString selectedText;
        Mufta_d.GetLBText(selectedIndex, selectedText);

        // Преобразование текста в double
        double dValue = _ttof(selectedText);
        FilterComboboxes(dValue, 0);
    }
}

void CObjectMufta::OnCbnSelchangedt2()
{
	// TODO: добавьте свой код обработчика уведомлений
    int selectedIndex = Mufta_dt2.GetCurSel();
    if (selectedIndex != CB_ERR) {
        CString selectedText;
        Mufta_dt2.GetLBText(selectedIndex, selectedText);

        double dValue = _ttof(selectedText);
        FilterComboboxes(dValue, 1);
    }
}

void CObjectMufta::OnCbnSelchangeb()
{
	// TODO: добавьте свой код обработчика уведомлений
    int selectedIndex = Mufta_b.GetCurSel();
    if (selectedIndex != CB_ERR) {
        CString selectedText;
        Mufta_b.GetLBText(selectedIndex, selectedText);

        double dValue = _ttof(selectedText);
        FilterComboboxes(dValue, 2);
    }

}

void CObjectMufta::OnCbnSelchangeb1()
{
	// TODO: добавьте свой код обработчика уведомлений
    int selectedIndex = Mufta_b1.GetCurSel();
    if (selectedIndex != CB_ERR) {
        CString selectedText;
        Mufta_b1.GetLBText(selectedIndex, selectedText);

        double dValue = _ttof(selectedText);
        FilterComboboxes(dValue, 3);
    }
}

void CObjectMufta::OnCbnSelchanged1()
{
	// TODO: добавьте свой код обработчика уведомлений
    int selectedIndex = Mufta_d1.GetCurSel();
    if (selectedIndex != CB_ERR) {
        CString selectedText;
        Mufta_d1.GetLBText(selectedIndex, selectedText);

        double dValue = _ttof(selectedText);
        FilterComboboxes(dValue, 4);
    }
}

void CObjectMufta::OnCbnSelchangeDd()
{
	// TODO: добавьте свой код обработчика уведомлений
    int selectedIndex = Mufta_D.GetCurSel();
    if (selectedIndex != CB_ERR) {
        CString selectedText;
        Mufta_D.GetLBText(selectedIndex, selectedText);

        double dValue = _ttof(selectedText);
        FilterComboboxes(dValue, 5);
    }
}

void CObjectMufta::OnCbnSelchangeDd1()
{
	// TODO: добавьте свой код обработчика уведомлений
    int selectedIndex = Mufta_D1.GetCurSel();
    if (selectedIndex != CB_ERR) {
        CString selectedText;
        Mufta_D1.GetLBText(selectedIndex, selectedText);

        double dValue = _ttof(selectedText);
        FilterComboboxes(dValue, 6);
    }
}

void CObjectMufta::OnCbnSelchangeL()
{
	// TODO: добавьте свой код обработчика уведомлений
    int selectedIndex = Mufta_L.GetCurSel();
    if (selectedIndex != CB_ERR) {
        CString selectedText;
        Mufta_L.GetLBText(selectedIndex, selectedText);

        double dValue = _ttof(selectedText);
        FilterComboboxes(dValue, 7);
    }
}

void CObjectMufta::OnCbnSelchangell()
{
	// TODO: добавьте свой код обработчика уведомлений
    int selectedIndex = Mufta_l.GetCurSel();
    if (selectedIndex != CB_ERR) {
        CString selectedText;
        Mufta_l.GetLBText(selectedIndex, selectedText);

        double dValue = _ttof(selectedText);
        FilterComboboxes(dValue, 8);
    }
}

void CObjectMufta::OnCbnSelchanger()
{
	// TODO: добавьте свой код обработчика уведомлений
    int selectedIndex = Mufta_r.GetCurSel();
    if (selectedIndex != CB_ERR) {
        CString selectedText;
        Mufta_r.GetLBText(selectedIndex, selectedText);
        double dValue = _ttof(selectedText);
        FilterComboboxes(dValue, 9);
    }
}

void CObjectMufta::OnCbnSelchangec()
{
	// TODO: добавьте свой код обработчика уведомлений
    int selectedIndex = Mufta_c.GetCurSel();
    if (selectedIndex != CB_ERR) {
        CString selectedText;
        Mufta_c.GetLBText(selectedIndex, selectedText);

        double dValue = _ttof(selectedText);
        FilterComboboxes(dValue, 10);
    }
}

void CObjectMufta::OnCbnSelchangec1()
{
	// TODO: добавьте свой код обработчика уведомлений
    int selectedIndex = Mufta_c1.GetCurSel();
    if (selectedIndex != CB_ERR) {
        CString selectedText;
        Mufta_c1.GetLBText(selectedIndex, selectedText);

        double dValue = _ttof(selectedText);
        FilterComboboxes(dValue, 11);
    }
}

void CObjectMufta::FillComboboxes()
{
    //d
    for (int i = 0; i < 15; i++) {
        CString strValue = CString(Name_d[i].c_str());
        strValue += _T(" мм");
        Mufta_d.InsertString(i, strValue);
    }
    //dt2
    for (int i = 0; i < 16; i++) {
        CString strValue = CString(Name_dt2[i].c_str());
        strValue += _T(" мм");
        Mufta_dt2.InsertString(i, strValue);
    }
    // b
    for (int i = 0; i < 6; i++) {
        CString strValue = CString(Name_b[i].c_str());
        strValue += _T(" мм");
        Mufta_b.InsertString(i, strValue);
    }
    // b1
    for (int i = 0; i < 3; i++) {
        CString strValue = CString(Name_b1[i].c_str());
        strValue += _T(" мм");
        Mufta_b1.InsertString(i, strValue);
    }
    for (int i = 0; i < 2; i++) {
        CString strValue = CString(Name_d1[i].c_str());
        strValue += _T(" мм");
        Mufta_d1.InsertString(i, strValue);
    }
    // D
    for (int i = 0; i < 7; i++) {
        CString strValue = CString(Name_D[i].c_str());
        strValue += _T(" мм");
        Mufta_D.InsertString(i, strValue);
    }
    // D1
    for (int i = 0; i < 6; i++) {
        CString strValue = CString(Name_D1[i].c_str());
        strValue += _T(" мм");
        Mufta_D1.InsertString(i, strValue);
    }
    // L
    for (int i = 0; i < 7; i++) {
        CString strValue = CString(Name_L[i].c_str());
        strValue += _T(" мм");
        Mufta_L.InsertString(i, strValue);
    }
    // l
    for (int i = 0; i < 5; i++) {
        CString strValue = CString(Name_l[i].c_str());
        strValue += _T(" мм");
        Mufta_l.InsertString(i, strValue);
    }
    // r
    for (int i = 0; i < 3; i++) {
        CString strValue = CString(Name_r[i].c_str());
        strValue += _T(" мм");
        Mufta_r.InsertString(i, strValue);
    }
    // c
    for (int i = 0; i < 2; i++) {
        CString strValue = CString(Name_c[i].c_str());
        strValue += _T(" мм");
        Mufta_c.InsertString(i, strValue);
    }
    // c1
    for (int i = 0; i < 3; i++) {
        CString strValue = CString(Name_c1[i].c_str());
        strValue += _T(" мм");
        Mufta_c1.InsertString(i, strValue);
    }
    Mufta_d.SetCurSel(0);
    Mufta_dt2.SetCurSel(0);
    Mufta_b.SetCurSel(0);
    Mufta_b1.SetCurSel(0);
    Mufta_d1.SetCurSel(0);
    Mufta_D.SetCurSel(0);
    Mufta_D1.SetCurSel(0);
    Mufta_L.SetCurSel(0);
    Mufta_l.SetCurSel(0);
    Mufta_r.SetCurSel(0);
    Mufta_c.SetCurSel(0);
    Mufta_c1.SetCurSel(0);
}

void CObjectMufta::FilterComboboxes(double znach, int index)
{
    int foundRow = -1;
    for (int row = 0; row < 18; row++) {
        if (abs(Var[index][row] - znach) < 0.001) {
            foundRow = row;
            break;
        }
    }
    if (foundRow != -1)
    {
        SetComboBoxValue(Mufta_d, Var[0][foundRow]);
        SetComboBoxValue(Mufta_dt2, Var[1][foundRow]);
        SetComboBoxValue(Mufta_b, Var[2][foundRow]);
        SetComboBoxValue(Mufta_b1, Var[3][foundRow]);
        SetComboBoxValue(Mufta_d1, Var[4][foundRow]);
        SetComboBoxValue(Mufta_D, Var[5][foundRow]);
        SetComboBoxValue(Mufta_D1, Var[6][foundRow]);
        SetComboBoxValue(Mufta_L, Var[7][foundRow]);
        SetComboBoxValue(Mufta_l, Var[8][foundRow]);
        SetComboBoxValue(Mufta_r, Var[9][foundRow]);
        SetComboBoxValue(Mufta_c, Var[10][foundRow]);
        SetComboBoxValue(Mufta_c1, Var[11][foundRow]);

        m_currentRow = foundRow;
    }
}

void CObjectMufta::SetComboBoxValue(CComboBox& comboBox, double value)
{
    CString searchText;
    {
        searchText.Format(_T("%.1f мм"), value);
    }
    int index = comboBox.FindStringExact(-1, searchText);
    if (index != CB_ERR) {
        comboBox.SetCurSel(index);
        return;
    }
}

void CObjectMufta::OnBnClickedOk()
{
    CDialogEx::OnOK();
    CKursachApp* pApp = (CKursachApp*)AfxGetApp();
    CMainFrame* pMainFrame = (CMainFrame*)pApp->GetMainWnd();
    CKursachDoc* pDoc = (CKursachDoc*)pMainFrame->GetActiveDocument();

    if (pDoc)
    {

        if (m_currentRow >= 0 && m_currentRow < 18)
        {
            pDoc->d = Var[0][m_currentRow];
            pDoc->dt2 = Var[1][m_currentRow];
            pDoc->b = Var[2][m_currentRow];
            pDoc->b1 = Var[3][m_currentRow];
            pDoc->d1 = Var[4][m_currentRow];
            pDoc->D = Var[5][m_currentRow];
            pDoc->D1 = Var[6][m_currentRow];
            pDoc->L = Var[7][m_currentRow];
            pDoc->l = Var[8][m_currentRow];
            pDoc->r = Var[9][m_currentRow];
            pDoc->c = Var[10][m_currentRow];
            pDoc->c1 = Var[11][m_currentRow];
            pDoc->Mufta_Check = true;
        }
    }
}
