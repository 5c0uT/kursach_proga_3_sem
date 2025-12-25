#include "pch.h"
// Kursach_Doc.h: интерфейс класса CKursachDoc 
//


#pragma once
class CMyFreeView;
class CKursachView;

class CKursachDoc : public CDocument
{
protected: // создать только из сериализации
	CKursachDoc() noexcept;
	DECLARE_DYNCREATE(CKursachDoc)

// Атрибуты
public:
	bool m_bSborka, m_bMufta, m_bVal, m_bShponka, m_bKolco, m_bVint;

	CKursachView* pView;
	CMyFreeView*  pTree;

	bool Mufta_Check =   false;
	bool Shponka_Check = false;
	bool Val_Check =     false;

	double d =           0;
	double dt2 =         0;
	double b =           0;
	double b1 =          0;
	double d1 =          0;
	double D =           0;
	double D1 =          0;
	double L =           0;
	double l =           0;
	double r =           0;
	double c =           0;
	double c1 =          0;
	double lV =          0; // длина вала
	double eps =         1; // точность для маркеров  думай
	double ShponkaH =    0; // высота шпонки
	double ShponkaW =    0; // длина шпонки
	double ShponkaR =    0; // скругление
	double p =           0; // шаг резьбы
	double VintLenght =  0; // длина винта
	double VintH =       0; // ширина шлица
	double VintW =       0; // глибина шлица

// Операции
public:

// Переопределение
public:
	virtual BOOL OnNewDocument();
	virtual void Serialize(CArchive& ar);

#ifdef SHARED_HANDLERS
	virtual void InitializeSearchContent();
	virtual void OnDrawThumbnail(CDC& dc, LPRECT lprcBounds);
#endif // SHARED_HANDLERS

// Реализация
public:
	virtual ~CKursachDoc();
#ifdef _DEBUG
	virtual void AssertValid() const;
	virtual void Dump(CDumpContext& dc) const;
#endif

protected:

// Созданные функции схемы сообщений
protected:
	DECLARE_MESSAGE_MAP()

#ifdef SHARED_HANDLERS
	// Вспомогательная функция, задающая содержимое поиска для обработчика поиска
	void SetSearchContent(const CString& value);
#endif // SHARED_HANDLERS
};
