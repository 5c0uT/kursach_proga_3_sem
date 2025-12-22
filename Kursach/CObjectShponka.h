#pragma once
#include "pch.h"

// Диалоговое окно CObjectShponka
class CObjectShponka : public CDialogEx
{
    DECLARE_DYNAMIC(CObjectShponka)

public:
    CObjectShponka(CWnd* pParent = nullptr);   // стандартный конструктор
    
    virtual ~CObjectShponka();

    void UpdateComboboxesFromB();
    void FillComboboxes();
    void FilterComboboxes(double znach, int index);
    void SetComboBoxValue(CComboBox& comboBox, double value);
    
    int m_currentRow = 0;
    double ShponkaVar[11][3]{
        {3.0,  5.0,  13},
        {3.0,  6.5,  16},
        {4.0,  6.5,  16},
        {4.0,  7.5,  19},
        {5.0,  6.5,  16},
        {5.0,  7.5,  19},
        {5.0,  9.0,  22},
        {6.0,  9.0,  22},
        {6.0,  10.0, 25},
        {8.0,  11.0, 28},
        {10.0, 13.0, 32}
    }; 

    // Данные диалогового окна
#ifdef AFX_DESIGN_TIME
    enum { IDD = IDD_Shponka };
#endif

protected:
    virtual void DoDataExchange(CDataExchange* pDX);    // поддержка DDX/DDV

    DECLARE_MESSAGE_MAP()

public:
    afx_msg void OnCbnSelchangeShponkaw();
    afx_msg void OnCbnSelchangeShponkah();
    afx_msg void OnBnClickedOk();

    CComboBox ShponkaH;
    CComboBox ShponkaW;

    CString ShponkaB;
};
