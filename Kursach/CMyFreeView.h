#pragma once
#include "afxcview.h"

// Просмотр CMyFreeView
class CKursachDoc;

class CMyFreeView : public CTreeView
{
	DECLARE_DYNCREATE(CMyFreeView)

protected:

	CMyFreeView();           // защищенный конструктор, используемый при динамическом создании
	virtual ~CMyFreeView();

public:

	CKursachDoc* m_pDoc;

	HTREEITEM m_hSborka, m_hMufta, m_hVal, m_hShponka, m_hKolco, m_hVint;

	void FillTree();

#ifdef _DEBUG
	virtual void AssertValid() const;
#ifndef _WIN32_WCE
	virtual void Dump(CDumpContext& dc) const;
#endif
#endif

protected:
	DECLARE_MESSAGE_MAP()

public:
	afx_msg int OnCreate(LPCREATESTRUCT lpCreateStruct);
	afx_msg void OnLButtonUp(UINT nFlags, CPoint point);
	afx_msg void OnLButtonDblClk(UINT nFlags, CPoint point);
};


