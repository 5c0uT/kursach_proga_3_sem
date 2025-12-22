#pragma once
#include "afxdialogex.h"


// Диалоговое окно CObjectVal

class CObjectVal : public CDialogEx
{
	DECLARE_DYNAMIC(CObjectVal)

public:
	CObjectVal(CWnd* pParent = nullptr);   // стандартный конструктор
	virtual ~CObjectVal();

// Данные диалогового окна
#ifdef AFX_DESIGN_TIME
	enum { IDD = IDD_Val };
#endif

protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // поддержка DDX/DDV

	DECLARE_MESSAGE_MAP()

public:
	int ValL;

	afx_msg void OnBnClickedOk();
};
