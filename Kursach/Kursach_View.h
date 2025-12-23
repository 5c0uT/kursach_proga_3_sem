#pragma once
#include "pch.h"

class CKursachView : public CView
{
protected: // создать только из сериализации
	CKursachView() noexcept;
	DECLARE_DYNCREATE(CKursachView)

	// Атрибуты
public:
	CKursachDoc* GetDocument() const;
	CButton Start;

	// Операции
public:
	void StartKompas();
	void CreateDetails();

	// Переопределение
public:
	virtual void OnDraw(CDC* pDC);  // переопределено для отрисовки этого представления
	virtual BOOL PreCreateWindow(CREATESTRUCT& cs);

protected:
	virtual BOOL OnPreparePrinting(CPrintInfo* pInfo);
	virtual void OnBeginPrinting(CDC* pDC, CPrintInfo* pInfo);
	virtual void OnEndPrinting(CDC* pDC, CPrintInfo* pInfo);

	// Реализация
public:
	virtual ~CKursachView();
#ifdef _DEBUG
	virtual void AssertValid() const;
	virtual void Dump(CDumpContext& dc) const;
#endif

protected:
	// Созданные функции схемы сообщений
protected:
	DECLARE_MESSAGE_MAP()

public:
	virtual void OnInitialUpdate();
};

#ifndef _DEBUG  // версия отладки в Kursach_View.cpp
inline CKursachDoc* CKursachView::GetDocument() const
{
	return reinterpret_cast<CKursachDoc*>(m_pDocument);
}
#endif
