#pragma once
#include "pch.h"

// Диалоговое окно CObjectMufta

class CObjectMufta : public CDialogEx
{
	DECLARE_DYNAMIC(CObjectMufta)

public:
	CObjectMufta(CWnd* pParent = nullptr);
	virtual ~CObjectMufta();

	// Добавьте эти объявления:
	static int LoadMuftaDataFromJson(const char* filename);
	static void FreeMuftaData();

	// Данные диалогового окна
#ifdef AFX_DESIGN_TIME
	enum { IDD = IDD_Mufta };
#endif

protected:
	virtual void DoDataExchange(CDataExchange* pDX);

	void FillComboboxes();
	void FilterComboboxes(double, int);
	void SetComboBoxValue(CComboBox& comboBox, double value);

	DECLARE_MESSAGE_MAP()

public:
	CComboBox Mufta_b1;
	CComboBox Mufta_b;
	CComboBox Mufta_c;
	CComboBox Mufta_c1;
	CComboBox Mufta_d;
	CComboBox Mufta_d1;
	CComboBox Mufta_D;
	CComboBox Mufta_D1;
	CComboBox Mufta_dt2;
	CComboBox Mufta_L;
	CComboBox Mufta_l;
	CComboBox Mufta_r;

	double d, dt2, b, b1, d1, D, D1, L, l, r, c, c1;
	int m_currentRow = 0;
	// Данные таблицы параметров муфты
	// Теперь загружаются из JSON файла через структуры mufta_data.h
	// Эти данные являются примером того, что хранится и записывается вовнутрь
	// Реальные данные загружаются из JSON файла при инициализации
	//double Var[12][18]{
	//	//     1        2       3       4       5       6       7       8       9       10      11      12      13      14      15      16      17      18
	//	/*d*/  {9.0,    10.0,   11.0,   12.0,   14.0,   16.0,   18.0,   19.0,   20.0,   20.0,   22.0,   24.0,   25.0,   28.0,   28.0,   30.0,   32.0,   32.0},
	//	/*dt2*/{10.4,   11.4,   12.8,   13.8,   16.3,   18.8,   20.8,   21.8,   22.8,   22.8,   24.8,   27.3,   28.3,   31.3,   31.3,   33.3,   35.3,   35.8},
	//	/*b*/  {3.0,    3.0,    4.0,    4.0,    5.0,    5.0,    6.0,    6.0,    6.0,    6.0,    6.0,    8.0,    8.0,    8.0,    8.0,    8.0,    10.0,   10.0},
	//	/*b1*/ {0.0,    0.0,    0.0,    0.0,    1.0,    1.0,    1.0,    1.0,    1.0,    1.0,    1.0,    1.0,    1.2,    1.2,    1.2,    1.2,    1.2,    1.2},
	//	/*d1*/ {4.0,    4.0,    4.0,    4.0,    6.0,    6.0,    6.0,    6.0,    6.0,    6.0,    6.0,    6.0,    6.0,    6.0,    6.0,    6.0,    6.0,    6.0},
	//	/*D*/  {16.0,   16.0,   18.0,   18.0,   28.0,   28.0,   32.0,   32.0,   32.0,   38.0,   38.0,   38.0,   42.0,   42.0,   48.0,   48.0,   48.0,   48.0},
	//	/*D1*/ {0.0,    0.0,    0.0,    0.0,    24.0,   24.0,   28.0,   28.0,   28.0,   34.0,   34.0,   34.0,   37.0,   37.0,   43.0,   43.0,   43.0,   43.0},
	//	/L**/  {35.0,   35.0,   40.0,   40.0,   45.0,   45.0,   45.0,   45.0,   55.0,   65.0,   65.0,   65.0,   65.0,   65.0,   90.0,   90.0,   90.0,   60.0},
	//	/*l*/  {8.0,    8.0,    8.0,    8.0,    10.0,   10.0,   12.0,   12.0,   12.0,   15.0,   15.0,   15.0,   20.0,   20.0,   20.0,   20.0,   20.0,   20.0},
	//	/*r*/  {0.1,    0.1,    0.1,    0.1,    0.2,    0.2,    0.2,    0.2,    0.2,    0.2,    0.2,    0.2,    0.2,    0.2,    0.2,    0.2,    0.3,    0.3},
	//	/*c*/  {0.6,    0.6,    0.6,    0.6,    1.0,    1.0,    1.0,    1.0,    1.0,    1.0,    1.0,    1.0,    1.0,    1.0,    1.0,    1.0,    1.0,    1.0},
	//	/*c1*/ {0.5,    0.5,    0.5,    0.5,    1.0,    1.0,    1.0,    1.0,    1.0,    1.0,    1.0,    1.0,    1.0,    1.0,    1.0,    1.0,    1.2,    1.2}
	//};

	/*std::string Name_d[15]{  
		"9.0",  "10.0", "11.0", "12.0", "14.0", "16.0", "18.0", "19.0", "20.0", "22.0", "24.0", "25.0", "28.0", "30.0", "32.0"
	};
	std::string Name_dt2[16]{
		"10.4", "11.4", "12.8", "13.8", "16.3", "18.8", "20.8", "21.8", "22.8", "24.8", "27.3", "28.3", "31.3", "33.3", "35.3", "35.8"
	};
	std::string Name_b[6]{
		"3.0",  "4.0",  "5.0",  "6.0",  "8.0",  "10.0" 
	};
	std::string Name_b1[3]{
		"0.0",  "1.0",  "1.2" 
	};
	std::string Name_d1[2]{
		"4.0",  "6.0" 
	};
	std::string Name_D[7]{
		"16.0", "18.0", "28.0", "32.0", "38.0", "42.0", "48.0" 
	};
	std::string Name_D1[6]{
		"0.0",  "24.0", "28.0", "34.0", "37.0", "43.0" 
	};
	std::string Name_L[7]{
		"35.0", "40.0", "45.0", "55.0", "65.0", "90.0", "60.0"
	};
	std::string Name_l[5]{
		"8.0",  "10.0", "12.0", "15.0", "20.0" 
	};
	std::string Name_r[3]{
		"0.1",  "0.2",  "0.3"
	};
	std::string Name_c[2]{
		"0.6",  "1.0" 
	};
	std::string Name_c1[3]{
		"0.5",  "1.0",  "1.2"
	};*/

	afx_msg void OnCbnSelchanged();
	afx_msg void OnCbnSelchangedt2();
	afx_msg void OnCbnSelchangeb();
	afx_msg void OnCbnSelchangeb1();
	afx_msg void OnCbnSelchanged1();
	afx_msg void OnCbnSelchangeDd();
	afx_msg void OnCbnSelchangeDd1();
	afx_msg void OnCbnSelchangeL();
	afx_msg void OnCbnSelchangell();
	afx_msg void OnCbnSelchanger();
	afx_msg void OnCbnSelchangec();
	afx_msg void OnCbnSelchangec1();
	afx_msg void OnBnClickedOk();
};
