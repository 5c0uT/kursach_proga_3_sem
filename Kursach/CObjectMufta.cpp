#include "pch.h"

using json = nlohmann::json;

IMPLEMENT_DYNAMIC(CObjectMufta, CDialogEx)

CObjectMufta::CObjectMufta(CWnd* pParent)
    : CDialogEx(IDD_Mufta, pParent)
{
    LoadMuftaDataFromJson("mufta_data.json");
}

int CObjectMufta::LoadMuftaDataFromJson(const char* filename)
{
    try {
        std::ifstream file(filename);
        if (!file.is_open()) 
        {
            return -1;
        }

        json j;
        file >> j;
        auto& muftaData = j["MuftaData"];
        FreeMuftaData();

        auto& varArray = muftaData["Var"];
        muftaVarTable.rows = varArray.size();
        if (muftaVarTable.rows == 0) 
        {
            return -2;
        }

        muftaVarTable.cols = varArray[0].size();
        muftaVarTable.data = (double**)malloc(muftaVarTable.rows * sizeof(double*));

        for (size_t i = 0; i < muftaVarTable.rows; i++)
        {
            muftaVarTable.data[i] = (double*)malloc(muftaVarTable.cols * sizeof(double));
            for (size_t k = 0; k < muftaVarTable.cols; k++) 
            {
                muftaVarTable.data[i][k] = varArray[i][k].get<double>();
            }
        }

        auto& nameArrays = muftaData["NameArrays"];

        {
            auto& dArray = nameArrays["d"];
            name_d.count = dArray.size();
            name_d.data = (const char**)malloc(name_d.count * sizeof(const char*));
            for (size_t i = 0; i < name_d.count; i++)
            {
                std::string str = dArray[i].get<std::string>();
                name_d.data[i] = _strdup(str.c_str());
            }
        }

        {
            auto& dt2Array = nameArrays["dt2"];
            name_dt2.count = dt2Array.size();
            name_dt2.data = (const char**)malloc(name_dt2.count * sizeof(const char*));
            for (size_t i = 0; i < name_dt2.count; i++)
            {
                std::string str = dt2Array[i].get<std::string>();
                name_dt2.data[i] = _strdup(str.c_str());
            }
        }

        {
            auto& bArray = nameArrays["b"];
            name_b.count = bArray.size();
            name_b.data = (const char**)malloc(name_b.count * sizeof(const char*));
            for (size_t i = 0; i < name_b.count; i++)
            {
                std::string str = bArray[i].get<std::string>();
                name_b.data[i] = _strdup(str.c_str());
            }
        }

        {
            auto& b1Array = nameArrays["b1"];
            name_b1.count = b1Array.size();
            name_b1.data = (const char**)malloc(name_b1.count * sizeof(const char*));
            for (size_t i = 0; i < name_b1.count; i++)
            {
                std::string str = b1Array[i].get<std::string>();
                name_b1.data[i] = _strdup(str.c_str());
            }
        }

        {
            auto& d1Array = nameArrays["d1"];
            name_d1.count = d1Array.size();
            name_d1.data = (const char**)malloc(name_d1.count * sizeof(const char*));
            for (size_t i = 0; i < name_d1.count; i++)
            {
                std::string str = d1Array[i].get<std::string>();
                name_d1.data[i] = _strdup(str.c_str());
            }
        }

        {
            auto& DArray = nameArrays["D"];
            name_D.count = DArray.size();
            name_D.data = (const char**)malloc(name_D.count * sizeof(const char*));
            for (size_t i = 0; i < name_D.count; i++) 
            {
                std::string str = DArray[i].get<std::string>();
                name_D.data[i] = _strdup(str.c_str());
            }
        }

        {
            auto& D1Array = nameArrays["D1"];
            name_D1.count = D1Array.size();
            name_D1.data = (const char**)malloc(name_D1.count * sizeof(const char*));
            for (size_t i = 0; i < name_D1.count; i++) 
            {
                std::string str = D1Array[i].get<std::string>();
                name_D1.data[i] = _strdup(str.c_str());
            }
        }

        {
            auto& LArray = nameArrays["L"];
            name_L.count = LArray.size();
            name_L.data = (const char**)malloc(name_L.count * sizeof(const char*));
            for (size_t i = 0; i < name_L.count; i++) 
            {
                std::string str = LArray[i].get<std::string>();
                name_L.data[i] = _strdup(str.c_str());
            }
        }

        {
            auto& lArray = nameArrays["l"];
            name_l.count = lArray.size();
            name_l.data = (const char**)malloc(name_l.count * sizeof(const char*));
            for (size_t i = 0; i < name_l.count; i++)
            {
                std::string str = lArray[i].get<std::string>();
                name_l.data[i] = _strdup(str.c_str());
            }
        }

        {
            auto& rArray = nameArrays["r"];
            name_r.count = rArray.size();
            name_r.data = (const char**)malloc(name_r.count * sizeof(const char*));
            for (size_t i = 0; i < name_r.count; i++) 
            {
                std::string str = rArray[i].get<std::string>();
                name_r.data[i] = _strdup(str.c_str());
            }
        }

        {
            auto& cArray = nameArrays["c"];
            name_c.count = cArray.size();
            name_c.data = (const char**)malloc(name_c.count * sizeof(const char*));
            for (size_t i = 0; i < name_c.count; i++) 
            {
                std::string str = cArray[i].get<std::string>();
                name_c.data[i] = _strdup(str.c_str());
            }
        }

        {
            auto& c1Array = nameArrays["c1"];
            name_c1.count = c1Array.size();
            name_c1.data = (const char**)malloc(name_c1.count * sizeof(const char*));
            for (size_t i = 0; i < name_c1.count; i++) 
            {
                std::string str = c1Array[i].get<std::string>();
                name_c1.data[i] = _strdup(str.c_str());
            }
        }

        file.close();
        return 0;

    }
    catch (const std::exception& e) 
    {
#ifdef _DEBUG
        OutputDebugStringA(e.what());
#endif
        return -3;
    }
}

void CObjectMufta::FreeMuftaData()
{
    if (muftaVarTable.data) 
    {
        for (size_t i = 0; i < muftaVarTable.rows; i++) 
        {
            if (muftaVarTable.data[i]) 
            {
                free(muftaVarTable.data[i]);
            }
        }
        free(muftaVarTable.data);
        muftaVarTable.data = nullptr;
    }

    auto freeNameArray = [](MuftaNameArray& arr) 
        {
            if (arr.data) 
            {
                for (size_t i = 0; i < arr.count; i++) 
                {
                    if (arr.data[i]) 
                    {
                        free((void*)arr.data[i]);
                    }
                }
                free(arr.data);
                arr.data = nullptr;
                arr.count = 0;
            }
        };

    freeNameArray(name_d);
    freeNameArray(name_dt2);
    freeNameArray(name_b);
    freeNameArray(name_b1);
    freeNameArray(name_d1);
    freeNameArray(name_D);
    freeNameArray(name_D1);
    freeNameArray(name_L);
    freeNameArray(name_l);
    freeNameArray(name_r);
    freeNameArray(name_c);
    freeNameArray(name_c1);
}

CObjectMufta::~CObjectMufta()
{
    FreeMuftaData();
}

void CObjectMufta::DoDataExchange(CDataExchange* pDX)
{
    CDialogEx::DoDataExchange(pDX);
    DDX_Control(pDX, IDC_b1,  Mufta_b1);
    DDX_Control(pDX, IDC_b,   Mufta_b);
    DDX_Control(pDX, IDC_c,   Mufta_c);
    DDX_Control(pDX, IDC_c1,  Mufta_c1);
    DDX_Control(pDX, IDC_d,   Mufta_d);
    DDX_Control(pDX, IDC_d1,  Mufta_d1);
    DDX_Control(pDX, IDC_DD,  Mufta_D);
    DDX_Control(pDX, IDC_DD1, Mufta_D1);
    DDX_Control(pDX, IDC_dt2, Mufta_dt2);
    DDX_Control(pDX, IDC_L,   Mufta_L);
    DDX_Control(pDX, IDC_ll,  Mufta_l);
    DDX_Control(pDX, IDC_r,   Mufta_r);
    FillComboboxes();
}

BEGIN_MESSAGE_MAP(CObjectMufta, CDialogEx)
    ON_CBN_SELCHANGE(IDC_d,   &CObjectMufta::OnCbnSelchanged)
    ON_CBN_SELCHANGE(IDC_dt2, &CObjectMufta::OnCbnSelchangedt2)
    ON_CBN_SELCHANGE(IDC_b,   &CObjectMufta::OnCbnSelchangeb)
    ON_CBN_SELCHANGE(IDC_b1,  &CObjectMufta::OnCbnSelchangeb1)
    ON_CBN_SELCHANGE(IDC_d1,  &CObjectMufta::OnCbnSelchanged1)
    ON_CBN_SELCHANGE(IDC_DD,  &CObjectMufta::OnCbnSelchangeDd)
    ON_CBN_SELCHANGE(IDC_DD1, &CObjectMufta::OnCbnSelchangeDd1)
    ON_CBN_SELCHANGE(IDC_L,   &CObjectMufta::OnCbnSelchangeL)
    ON_CBN_SELCHANGE(IDC_ll,  &CObjectMufta::OnCbnSelchangell)
    ON_CBN_SELCHANGE(IDC_r,   &CObjectMufta::OnCbnSelchanger)
    ON_CBN_SELCHANGE(IDC_c,   &CObjectMufta::OnCbnSelchangec)
    ON_CBN_SELCHANGE(IDC_c1,  &CObjectMufta::OnCbnSelchangec1)
    ON_BN_CLICKED(IDOK,       &CObjectMufta::OnBnClickedOk)
END_MESSAGE_MAP()

void CObjectMufta::OnCbnSelchanged()
{
    int selectedIndex = Mufta_d.GetCurSel();
    if (selectedIndex != CB_ERR) 
    {
        CString selectedText;
        Mufta_d.GetLBText(selectedIndex, selectedText);
        double dValue = _ttof(selectedText);
        FilterComboboxes(dValue, 0);
    }
}

void CObjectMufta::OnCbnSelchangedt2()
{
    int selectedIndex = Mufta_dt2.GetCurSel();
    if (selectedIndex != CB_ERR) 
    {
        CString selectedText;
        Mufta_dt2.GetLBText(selectedIndex, selectedText);
        double dValue = _ttof(selectedText);
        FilterComboboxes(dValue, 1);
    }
}

void CObjectMufta::OnCbnSelchangeb()
{
    int selectedIndex = Mufta_b.GetCurSel();
    if (selectedIndex != CB_ERR) 
    {
        CString selectedText;
        Mufta_b.GetLBText(selectedIndex, selectedText);
        double dValue = _ttof(selectedText);
        FilterComboboxes(dValue, 2);
    }
}

void CObjectMufta::OnCbnSelchangeb1()
{
    int selectedIndex = Mufta_b1.GetCurSel();
    if (selectedIndex != CB_ERR) 
    {
        CString selectedText;
        Mufta_b1.GetLBText(selectedIndex, selectedText);
        double dValue = _ttof(selectedText);
        FilterComboboxes(dValue, 3);
    }
}

void CObjectMufta::OnCbnSelchanged1()
{
    int selectedIndex = Mufta_d1.GetCurSel();
    if (selectedIndex != CB_ERR) 
    {
        CString selectedText;
        Mufta_d1.GetLBText(selectedIndex, selectedText);
        double dValue = _ttof(selectedText);
        FilterComboboxes(dValue, 4);
    }
}

void CObjectMufta::OnCbnSelchangeDd()
{
    int selectedIndex = Mufta_D.GetCurSel();
    if (selectedIndex != CB_ERR) 
    {
        CString selectedText;
        Mufta_D.GetLBText(selectedIndex, selectedText);
        double dValue = _ttof(selectedText);
        FilterComboboxes(dValue, 5);
    }
}

void CObjectMufta::OnCbnSelchangeDd1()
{
    int selectedIndex = Mufta_D1.GetCurSel();
    if (selectedIndex != CB_ERR) 
    {
        CString selectedText;
        Mufta_D1.GetLBText(selectedIndex, selectedText);
        double dValue = _ttof(selectedText);
        FilterComboboxes(dValue, 6);
    }
}

void CObjectMufta::OnCbnSelchangeL()
{
    int selectedIndex = Mufta_L.GetCurSel();
    if (selectedIndex != CB_ERR) 
    {
        CString selectedText;
        Mufta_L.GetLBText(selectedIndex, selectedText);
        double dValue = _ttof(selectedText);
        FilterComboboxes(dValue, 7);
    }
}

void CObjectMufta::OnCbnSelchangell()
{
    int selectedIndex = Mufta_l.GetCurSel();
    if (selectedIndex != CB_ERR) 
    {
        CString selectedText;
        Mufta_l.GetLBText(selectedIndex, selectedText);
        double dValue = _ttof(selectedText);
        FilterComboboxes(dValue, 8);
    }
}

void CObjectMufta::OnCbnSelchanger()
{
    int selectedIndex = Mufta_r.GetCurSel();
    if (selectedIndex != CB_ERR) 
    {
        CString selectedText;
        Mufta_r.GetLBText(selectedIndex, selectedText);
        double dValue = _ttof(selectedText);
        FilterComboboxes(dValue, 9);
    }
}

void CObjectMufta::OnCbnSelchangec()
{
    int selectedIndex = Mufta_c.GetCurSel();
    if (selectedIndex != CB_ERR) 
    {
        CString selectedText;
        Mufta_c.GetLBText(selectedIndex, selectedText);
        double dValue = _ttof(selectedText);
        FilterComboboxes(dValue, 10);
    }
}

void CObjectMufta::OnCbnSelchangec1()
{
    int selectedIndex = Mufta_c1.GetCurSel();
    if (selectedIndex != CB_ERR) 
    {
        CString selectedText;
        Mufta_c1.GetLBText(selectedIndex, selectedText);
        double dValue = _ttof(selectedText);
        FilterComboboxes(dValue, 11);
    }
}

void CObjectMufta::FillComboboxes()
{
    for (int i = 0; i < (int)name_d.count; i++) 
    {
        CString strValue = CString(name_d.data[i]);
        strValue += _T(" мм");
        Mufta_d.InsertString(i, strValue);
    }

    for (int i = 0; i < (int)name_dt2.count; i++) 
    {
        CString strValue = CString(name_dt2.data[i]);
        strValue += _T(" мм");
        Mufta_dt2.InsertString(i, strValue);
    }

    for (int i = 0; i < (int)name_b.count; i++) 
    {
        CString strValue = CString(name_b.data[i]);
        strValue += _T(" мм");
        Mufta_b.InsertString(i, strValue);
    }

    for (int i = 0; i < (int)name_b1.count; i++) 
    {
        CString strValue = CString(name_b1.data[i]);
        strValue += _T(" мм");
        Mufta_b1.InsertString(i, strValue);
    }

    for (int i = 0; i < (int)name_d1.count; i++) 
    {
        CString strValue = CString(name_d1.data[i]);
        strValue += _T(" мм");
        Mufta_d1.InsertString(i, strValue);
    }

    for (int i = 0; i < (int)name_D.count; i++) 
    {
        CString strValue = CString(name_D.data[i]);
        strValue += _T(" мм");
        Mufta_D.InsertString(i, strValue);
    }

    for (int i = 0; i < (int)name_D1.count; i++) 
    {
        CString strValue = CString(name_D1.data[i]);
        strValue += _T(" мм");
        Mufta_D1.InsertString(i, strValue);
    }

    for (int i = 0; i < (int)name_L.count; i++) 
    {
        CString strValue = CString(name_L.data[i]);
        strValue += _T(" мм");
        Mufta_L.InsertString(i, strValue);
    }

    for (int i = 0; i < (int)name_l.count; i++) 
    {
        CString strValue = CString(name_l.data[i]);
        strValue += _T(" мм");
        Mufta_l.InsertString(i, strValue);
    }

    for (int i = 0; i < (int)name_r.count; i++) 
    {
        CString strValue = CString(name_r.data[i]);
        strValue += _T(" мм");
        Mufta_r.InsertString(i, strValue);
    }

    for (int i = 0; i < (int)name_c.count; i++) 
    {
        CString strValue = CString(name_c.data[i]);
        strValue += _T(" мм");
        Mufta_c.InsertString(i, strValue);
    }

    for (int i = 0; i < (int)name_c1.count; i++) 
    {
        CString strValue = CString(name_c1.data[i]);
        strValue += _T(" мм");
        Mufta_c1.InsertString(i, strValue);
    }

    Mufta_d.SetCurSel(  0);
    Mufta_dt2.SetCurSel(0);
    Mufta_b.SetCurSel(  0);
    Mufta_b1.SetCurSel( 0);
    Mufta_d1.SetCurSel( 0);
    Mufta_D.SetCurSel(  0);
    Mufta_D1.SetCurSel( 0);
    Mufta_L.SetCurSel(  0);
    Mufta_l.SetCurSel(  0);
    Mufta_r.SetCurSel(  0);
    Mufta_c.SetCurSel(  0);
    Mufta_c1.SetCurSel( 0);
}

void CObjectMufta::FilterComboboxes(double znach, int index)
{
    int foundRow = -1;
    for (int row = 0; row < (int)muftaVarTable.rows; row++) {
        if (abs(muftaVarTable.data[row][index] - znach) < 0.001) {
            foundRow = row;
            break;
        }
    }

    if (foundRow != -1) {
        SetComboBoxValue(Mufta_d,   muftaVarTable.data[foundRow][0]);
        SetComboBoxValue(Mufta_dt2, muftaVarTable.data[foundRow][1]);
        SetComboBoxValue(Mufta_b,   muftaVarTable.data[foundRow][2]);
        SetComboBoxValue(Mufta_b1,  muftaVarTable.data[foundRow][3]);
        SetComboBoxValue(Mufta_d1,  muftaVarTable.data[foundRow][4]);
        SetComboBoxValue(Mufta_D,   muftaVarTable.data[foundRow][5]);
        SetComboBoxValue(Mufta_D1,  muftaVarTable.data[foundRow][6]);
        SetComboBoxValue(Mufta_L,   muftaVarTable.data[foundRow][7]);
        SetComboBoxValue(Mufta_l,   muftaVarTable.data[foundRow][8]);
        SetComboBoxValue(Mufta_r,   muftaVarTable.data[foundRow][9]);
        SetComboBoxValue(Mufta_c,   muftaVarTable.data[foundRow][10]);
        SetComboBoxValue(Mufta_c1,  muftaVarTable.data[foundRow][11]);
        m_currentRow = foundRow;
    }
}

void CObjectMufta::SetComboBoxValue(CComboBox& comboBox, double value)
{
    CString searchText;
    searchText.Format(_T("%.1f мм"), value);
    int index = comboBox.FindStringExact(-1, searchText);
    if (index != CB_ERR) {
        comboBox.SetCurSel(index);
    }
}

void CObjectMufta::OnBnClickedOk()
{
    CDialogEx::OnOK();
    CKursachApp* pApp = (CKursachApp*)AfxGetApp();
    CMainFrame* pMainFrame = (CMainFrame*)pApp->GetMainWnd();
    CKursachDoc* pDoc = (CKursachDoc*)pMainFrame->GetActiveDocument();

    if (pDoc) {
        if (m_currentRow >= 0 && m_currentRow < (int)muftaVarTable.rows) {
            pDoc->d =           muftaVarTable.data[m_currentRow][0];
            pDoc->dt2 =         muftaVarTable.data[m_currentRow][1];
            pDoc->b =           muftaVarTable.data[m_currentRow][2];
            pDoc->b1 =          muftaVarTable.data[m_currentRow][3];
            pDoc->d1 =          muftaVarTable.data[m_currentRow][4];
            pDoc->D =           muftaVarTable.data[m_currentRow][5];
            pDoc->D1 =          muftaVarTable.data[m_currentRow][6];
            pDoc->L =           muftaVarTable.data[m_currentRow][7];
            pDoc->l =           muftaVarTable.data[m_currentRow][8];
            pDoc->r =           muftaVarTable.data[m_currentRow][9];
            pDoc->c =           muftaVarTable.data[m_currentRow][10];
            pDoc->c1 =          muftaVarTable.data[m_currentRow][11];
            pDoc->Mufta_Check = true;
        }
    }
}
