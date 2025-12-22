// MainFrm.h: интерфейс класса CMainFrame
//

#pragma once

// Добавляем необходимые заголовочные файлы
#include "pch.h"

class CMainFrame : public CFrameWnd
{

protected: // создать только из сериализации
	CMainFrame() noexcept;
	DECLARE_DYNCREATE(CMainFrame)

	// Атрибуты
public:

	// Операции
public:

	// Переопределение
public:
	virtual BOOL PreCreateWindow(CREATESTRUCT& cs);

	// Реализация
public:
	virtual ~CMainFrame();
#ifdef _DEBUG
	virtual void AssertValid() const;
	virtual void Dump(CDumpContext& dc) const;
#endif

protected:  // встроенные члены панели элементов управления
	CToolBar          m_wndToolBar;
	CStatusBar        m_wndStatusBar;
	CSplitterWnd	  m_wndSplitter;

	// Созданные функции схемы сообщений
protected:
	afx_msg int OnCreate(LPCREATESTRUCT lpCreateStruct);
	DECLARE_MESSAGE_MAP()

	virtual BOOL OnCreateClient(LPCREATESTRUCT lpcs, CCreateContext* pContext)
	{
		// TODO: добавьте специализированный код или вызов базового класса
		m_wndSplitter.CreateStatic(this, 1, 2);
		m_wndSplitter.CreateView(0, 0, RUNTIME_CLASS(CMyFreeView),  CSize(200, 0), pContext);
		m_wndSplitter.CreateView(0, 1, RUNTIME_CLASS(CKursachView), CSize(0,   0), pContext);

		SetActiveView((CView*)m_wndSplitter.GetPane(0, 1));

		((CKursachDoc*)GetActiveDocument())->pTree = (CMyFreeView*)m_wndSplitter.GetPane( 0, 0);
		((CKursachDoc*)GetActiveDocument())->pView = (CKursachView*)m_wndSplitter.GetPane(0, 1);

		((CMyFreeView*)m_wndSplitter.GetPane(0, 0))->m_pDoc = ((CKursachDoc*)GetActiveDocument());
		return TRUE;
	}
};
