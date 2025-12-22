
// Kursach_View.cpp: реализация класса CKursachView
//

#include "pch.h"
#include "framework.h"
// SHARED_HANDLERS можно определить в обработчиках фильтров просмотра реализации проекта ATL, эскизов
// и поиска; позволяет совместно использовать код документа в данным проекте.
#ifndef SHARED_HANDLERS
#include "Kursach.h"
#endif
#include "Kursach_Doc.h"
#include "Kursach_View.h"
#include "MainFrm.h"
#include "afxdialogex.h"
#include "string"
#include "cmath"
#include "C:\Program Files\ASCON\KOMPAS-3D v23 Study\SDK\Include\ksConstants.h"
#include "C:\Program Files\ASCON\KOMPAS-3D v23 Study\SDK\Include\ksConstants3D.h"
#include <iostream>
#include <atlsafe.h>
#include <comutil.h>

#define PI 4*atan(1)
#ifdef _DEBUG
#define new DEBUG_NEW
#endif

#import "C:\Program Files\ASCON\KOMPAS-3D v23 Study\SDK\lib\kAPI5.tlb"

using namespace Kompas6API5;


KompasObjectPtr pKompasApp5;



ksPartPtr pPart;
ksDocument3DPtr pDoc;


// CKursachView

IMPLEMENT_DYNCREATE(CKursachView, CView)

BEGIN_MESSAGE_MAP(CKursachView, CView)
	// Стандартные команды печати
	ON_COMMAND(ID_FILE_PRINT, &CView::OnFilePrint)
	ON_COMMAND(ID_FILE_PRINT_DIRECT, &CView::OnFilePrint)
	ON_COMMAND(ID_FILE_PRINT_PREVIEW, &CView::OnFilePrintPreview)
	ON_BN_CLICKED(IDC_START, &CKursachView::StartKompas)
END_MESSAGE_MAP()

// Создание или уничтожение CKursachView

CKursachView::CKursachView() noexcept
{
	// TODO: добавьте код создания

}

CKursachView::~CKursachView()
{
}

BOOL CKursachView::PreCreateWindow(CREATESTRUCT& cs)
{
	// TODO: изменить класс Window или стили посредством изменения
	//  CREATESTRUCT cs

	return CView::PreCreateWindow(cs);
}

// Рисование CKursachView

void CKursachView::OnDraw(CDC* /*pDC*/)
{
	CKursachDoc* pDoc = GetDocument();
	ASSERT_VALID(pDoc);
	if (!pDoc)
		return;
	// TODO: добавьте здесь код отрисовки для собственных данных
}


// Печать CKursachView

BOOL CKursachView::OnPreparePrinting(CPrintInfo* pInfo)
{
	// подготовка по умолчанию
	return DoPreparePrinting(pInfo);
}

void CKursachView::OnBeginPrinting(CDC* /*pDC*/, CPrintInfo* /*pInfo*/)
{
	// TODO: добавьте дополнительную инициализацию перед печатью
}

void CKursachView::OnEndPrinting(CDC* /*pDC*/, CPrintInfo* /*pInfo*/)
{
	// TODO: добавьте очистку после печати
}


// Диагностика CKursachView

#ifdef _DEBUG
void CKursachView::AssertValid() const
{
	CView::AssertValid();
}

void CKursachView::Dump(CDumpContext& dc) const
{
	CView::Dump(dc);
}

CKursachDoc* CKursachView::GetDocument() const // встроена неотлаженная версия
{
	ASSERT(m_pDocument->IsKindOf(RUNTIME_CLASS(CKursachDoc)));
	return (CKursachDoc*)m_pDocument;
}
#endif //_DEBUG


// Обработчики сообщений CKursachView

void CKursachView::OnInitialUpdate()
{
	CView::OnInitialUpdate();

	Start.Create(_T("Построить"), WS_CHILD | WS_VISIBLE,
		CRect(0, 0, 120, 30), this, IDC_START);
	Start.CenterWindow();

	CView::OnInitialUpdate();
	// TODO: добавьте специализированный код или вызов базового класса
}


void CKursachView::StartKompas()
{
	CKursachApp* pApp = (CKursachApp*)AfxGetApp();
	CMainFrame* pMainFrame = (CMainFrame*)pApp->GetMainWnd();
	CKursachDoc* pDoc0 = (CKursachDoc*)pMainFrame->GetActiveDocument();

	BeginWaitCursor();

	// Получаем/создаём COM-объект Kompas.Application.5
	CComPtr<IUnknown> pKompasAppUnk = nullptr;
	if (!pKompasApp5)
	{
		CLSID InvAppClsid;
		HRESULT hRes = CLSIDFromProgID(L"Kompas.Application.5", &InvAppClsid);
		if (FAILED(hRes)) { EndWaitCursor(); return; }

		hRes = ::GetActiveObject(InvAppClsid, NULL, &pKompasAppUnk);
		if (FAILED(hRes)) {
			hRes = CoCreateInstance(InvAppClsid, NULL, CLSCTX_LOCAL_SERVER, __uuidof(IUnknown), (void**)&pKompasAppUnk);
			if (FAILED(hRes)) { EndWaitCursor(); return; }
		}

		hRes = pKompasAppUnk->QueryInterface(__uuidof(KompasObject), (void**)&pKompasApp5);
		if (FAILED(hRes))
		{
			EndWaitCursor();
			return;
		}
	}
	pKompasApp5->Visible = true;
	CreateDetails();
}

void CKursachView::CreateDetails()
{
	CKursachApp* pApp = (CKursachApp*)AfxGetApp();
	CMainFrame* pMainFrame = (CMainFrame*)pApp->GetMainWnd();
	CKursachDoc* pDoc0 = (CKursachDoc*)pMainFrame->GetActiveDocument();
	if (pDoc0->Mufta_Check != false && pDoc0->Shponka_Check != false && pDoc0->Val_Check)
	{

		double d = pDoc0->d;
		double dt2 = pDoc0->dt2;
		double b = pDoc0->b;
		double b1 = pDoc0->b1;
		double d1 = pDoc0->d1;
		double D = pDoc0->D;
		double D1 = pDoc0->D1;
		double L = pDoc0->L;
		double l = pDoc0->l;
		double r = pDoc0->r;
		double c = pDoc0->c;
		double c1 = pDoc0->c1;
		double lV = pDoc0->lV; // длина вала
		double eps = 1; //точность для маркеров  думай
		double ShponkaH = pDoc0->ShponkaH; // высота шпонки
		double ShponkaW = pDoc0->ShponkaW; //длина шпонки
		double ShponkaR = 0.2; //скругление
		double p = 0; //шаг резьбы
		double VintH = 0; // ширина шлица
		double VintW = 0; // глибина шлица
		if (pDoc0->d1 == 4)
		{
			p = 0.7;
			VintH = 0.6;
			VintW = 1.42;
		}
		else
		{
			p = 1.0;
			VintH = 1.0;
			VintW = 2.0;
		}
		double VintLenght = pDoc0->D/2 - pDoc0->d/2; // длина винта

		if ((D != 0) && (ShponkaH != 0) && (lV != 0)) {
			{
				pDoc = pKompasApp5->Document3D();
				pDoc->Create(false, true);
				pPart = pDoc->GetPart(pTop_Part);

				//Муфта
				//-------------------------------------------------------------------------------
				ksEntityPtr pSketch = pPart->NewEntity(o3d_sketch);
				ksSketchDefinitionPtr pSketchDef = pSketch->GetDefinition();
				pSketchDef->SetPlane(pPart->GetDefaultEntity(o3d_planeXOY));
				pSketch->Create();
				ksDocument2DPtr p2DDoc = pSketchDef->BeginEdit();

				// Точки профиля муфты
				double point[4][2];

				// Внешний контур
				point[0][0] = 0;
				point[0][1] = D / 2.0;

				point[1][0] = L;
				point[1][1] = D / 2.0;

				// Внутренний контур
				point[2][0] = L;
				point[2][1] = d / 2.0;

				point[3][0] = 0;
				point[3][1] = d / 2.0;

				//внешний контур
				p2DDoc->ksLineSeg(point[0][0], point[0][1], point[1][0], point[1][1], 1);
				p2DDoc->ksLineSeg(point[1][0], point[1][1], point[2][0], point[2][1], 1);
				p2DDoc->ksLineSeg(point[2][0], point[2][1], point[3][0], point[3][1], 1);
				p2DDoc->ksLineSeg(point[3][0], point[3][1], point[0][0], point[0][1], 1);

				// Ось вращения
				p2DDoc->ksLineSeg(-10, 0, 10, 0, 3);

				pSketchDef->EndEdit();

				// Вращение для создания вал
				ksEntityPtr pRotate = pPart->NewEntity(o3d_bossRotated);
				ksBossRotatedDefinitionPtr pRotDef = pRotate->GetDefinition();
				pRotDef->SetSketch(pSketch);
				pRotDef->SetSideParam(TRUE, 360);
				pRotate->Create();

				//фаска
				//-------------------------------------------------------------------------------
				// Получаем коллекцию всех ребер детали
				ksEntityCollectionPtr allEdges = pPart->EntityCollection(o3d_edge);
				// Создаем фаску
				ksEntityPtr pChamfer = pPart->NewEntity(o3d_chamfer);
				ksChamferDefinitionPtr pChamferDef = pChamfer->GetDefinition();
				pChamferDef->SetChamferParam(true, c, c); // фаска 1x45 градусов

				// Получаем коллекцию ребер для фаски
				ksEntityCollectionPtr chamferEdges = pChamferDef->array();
				chamferEdges->Clear();

				// Перебираем все ребра
				for (int i = 0; i < allEdges->GetCount(); i++) {
					ksEntityPtr edge = allEdges->GetByIndex(i);
					ksEdgeDefinitionPtr edgeDef = edge->GetDefinition();

					// Проверяем, является ли ребро круговым
					if (edgeDef->IsCircle()) {
						chamferEdges->Add(edge);
					}
				}
				pChamfer->Create();

				//вырез под шпонку
				//-------------------------------------------------------------------------------
				// Создаем эскиз для шпоночного паза на плоскости XOY
				ksEntityPtr pSketchKeyway = pPart->NewEntity(o3d_sketch);
				ksSketchDefinitionPtr pSketchKeywayDef = pSketchKeyway->GetDefinition();
				pSketchKeywayDef->SetPlane(pPart->GetDefaultEntity(o3d_planeXOY));
				pSketchKeyway->Create();
				ksDocument2DPtr p2DDocKeyway = pSketchKeywayDef->BeginEdit();

				// Координаты прямоугольника по всей длине вала
				double leftX = -b / 2.0;              // левая граница
				double rightX = b / 2.0;              // правая граница
				double startY = 0;                    // начинаем от центра
				double endY = L;                      // до конца трубы

				// Создание прямоугольника вдоль вала
				p2DDocKeyway->ksLineSeg(startY, leftX, endY, leftX, 1);    // левая грань
				p2DDocKeyway->ksLineSeg(endY, leftX, endY, rightX, 1);     // торцевая грань
				p2DDocKeyway->ksLineSeg(endY, rightX, startY, rightX, 1);  // правая грань
				p2DDocKeyway->ksLineSeg(startY, rightX, startY, leftX, 1); // начальная грань

				pSketchKeywayDef->EndEdit();

				// Выдавливание шпоночного паза на глубину 10.8 мм
				ksEntityPtr pExtrudeKeyway = pPart->NewEntity(o3d_cutExtrusion);
				ksCutExtrusionDefinitionPtr pExtrudeKeywayDef = pExtrudeKeyway->GetDefinition();
				pExtrudeKeywayDef->SetSketch(pSketchKeyway);
				pExtrudeKeywayDef->SetSideParam(FALSE, etBlind, dt2 - (d / 2), 0, FALSE);
				pExtrudeKeyway->Create();

				//СКРУГЛЕНИЕ!!!!!!!!!!!!!!!!!!!!!!
				//-------------------------------------------------------------------------------
				// Получаем обновленную коллекцию ребер после создания шпоночного паза
				ksEntityCollectionPtr updatedEdges = pPart->EntityCollection(o3d_edge);
				ksEntityPtr pFillet = pPart->NewEntity(o3d_fillet);
				ksFilletDefinitionPtr pFilletDef = pFillet->GetDefinition();
				pFilletDef->radius = r;
				ksEntityCollectionPtr fl = pFilletDef->array();
				fl->Clear();
				for (int i = 0; i < updatedEdges->GetCount(); i++)
				{
					ksEntityPtr ed = updatedEdges->GetByIndex(i);
					ksEdgeDefinitionPtr def = ed->GetDefinition();

					// Проверяем, что ребро принадлежит операции выдавливания шпоночного паза
					if (def->GetOwnerEntity() == pExtrudeKeyway)
					{
						ksVertexDefinitionPtr p1 = def->GetVertex(true);
						ksVertexDefinitionPtr p2 = def->GetVertex(false);
						double x1, y1, z1, x2, y2, z2;
						p1->GetPoint(&x1, &y1, &z1);
						p2->GetPoint(&x2, &y2, &z2);

						if (x1 == 0 && x2 == L && z1 == dt2 - (d / 2) && y1 == y2)
						{
							fl->Add(ed);
						}
						else if (x2 == 0 && x1 == L && z2 == dt2 - (d / 2) && y1 == y2)
						{
							fl->Add(ed);
						}
					}
				}

				if (fl->GetCount() > 0) {
					pFillet->Create();
				}

				//СМЕЩЕНИЕ ПЛОСКОСТИ
				//-------------------------------------------------------------------------------
				ksEntityPtr pPlane1 = pPart->NewEntity(o3d_planeOffset);
				ksPlaneOffsetDefinitionPtr pPlaneDef1 = pPlane1->GetDefinition();

				pPlaneDef1->direction = false;
				pPlaneDef1->offset = D / 2;
				pPlaneDef1->SetPlane(pPart->GetDefaultEntity(o3d_planeXOY));
				pPlane1->Create();


				//СОЗДАНИЕ ОТВЕРСТИЯ С РЕЗЬБОЙ M6-7H НА ВТОРОЙ ПЛОСКОСТИ
				//-------------------------------------------------------------------------------
				ksEntityPtr pSketchThreadedHole = pPart->NewEntity(o3d_sketch);
				ksSketchDefinitionPtr pSketchDefThreadedHole = pSketchThreadedHole->GetDefinition();
				pSketchDefThreadedHole->SetPlane(pPlane1); // Используем вторую плоскость
				pSketchThreadedHole->Create();
				ksDocument2DPtr p2DDocThreadedHole = pSketchDefThreadedHole->BeginEdit();

				p2DDocThreadedHole->ksCircle(l, 0, (d1 - 1) / 2.0, 1);// вычитание 1 т.к отверстие м6 имеет радиус 5

				pSketchDefThreadedHole->EndEdit();

				ksEntityPtr pExtrudeThreadedHole = pPart->NewEntity(o3d_cutExtrusion);
				ksCutExtrusionDefinitionPtr pExtrudeDefThreadedHole = pExtrudeThreadedHole->GetDefinition();
				pExtrudeDefThreadedHole->SetSketch(pSketchThreadedHole);
				pExtrudeDefThreadedHole->SetSideParam(FALSE, etBlind, D / 2, 0, FALSE); //указываем проверка выреза до центра
				pExtrudeThreadedHole->Create();

				//СОЗДАНИЕ ФАСКИ ОТВЕРСТИЯ С РЕЗЬБОЙ НА ВТОРОЙ ПЛОСКОСТИ
				//-------------------------------------------------------------------------------
				// Получаем коллекцию всех ребер детали
				ksEntityCollectionPtr allEdgesAfterThread = pPart->EntityCollection(o3d_edge);

				ksEntityPtr pChamferThread = pPart->NewEntity(o3d_chamfer);
				ksChamferDefinitionPtr pChamferThreadDef = pChamferThread->GetDefinition();
				pChamferThreadDef->SetChamferParam(false, c1, c1);

				ksEntityCollectionPtr chamferThreadEdges = pChamferThreadDef->array();
				chamferThreadEdges->Clear();
				for (int i = 0; i < allEdgesAfterThread->GetCount(); i++)
				{
					ksEntityPtr edge = allEdgesAfterThread->GetByIndex(i);
					ksEdgeDefinitionPtr edgeDef = edge->GetDefinition();

					if (edgeDef->GetOwnerEntity() == pExtrudeThreadedHole)
					{
						chamferThreadEdges->Add(edge);
						break;
					}
				}

				if (chamferThreadEdges->GetCount() > 0) {
					pChamferThread->Create();
				}

				// СОЗДАНИЕ РЕЗЬБЫ НА ОТВЕРСТИИ
				//-------------------------------------------------------------------------------
				ksEntityPtr Thread1 = pPart->NewEntity(o3d_thread);
				ksThreadDefinitionPtr ThreadDef1 = Thread1->GetDefinition();

				// Настройка параметров резьбы M6
				ThreadDef1->allLength = true; // признак полной длины
				ThreadDef1->autoDefinDr = true; // признак автоопределения диаметра
				ThreadDef1->dr = d1; // номинальный диаметр резьбы M6
				ThreadDef1->length = D / 2; // длина резьбы (должна соответствовать глубине отверстия) //указываем проверка выреза до центра
				ThreadDef1->faceValue = true; // направление построения резьбы (true - внутренняя)
				ThreadDef1->p = p; // шаг резьбы M6 составляет 1 мм //указываем

				ksEntityCollectionPtr faceCollection = pPart->EntityCollection(o3d_face);
				ksEntityPtr threadFace = nullptr;

				for (int i = 0; i < faceCollection->GetCount(); i++)
				{
					ksEntityPtr face = faceCollection->GetByIndex(i);
					ksFaceDefinitionPtr faceDef = face->GetDefinition();

					// Проверяем, что грань принадлежит нашему выдавливанию отверстия
					if (faceDef->GetOwnerEntity() == pExtrudeThreadedHole)
					{
						threadFace = face;
						break;
					}
				}
				if (threadFace != nullptr)
				{
					ThreadDef1->SetBaseObject(threadFace);
					Thread1->Create();
				}

				if (b1 != 0)
				{
					// СОЗДАНИЕ ВЫРЕЗА ПОД КОЛЬЦО
					//-------------------------------------------------------------------------------
					ksEntityPtr pSketchRectCut = pPart->NewEntity(o3d_sketch);
					ksSketchDefinitionPtr pSketchRectCutDef = pSketchRectCut->GetDefinition();
					pSketchRectCutDef->SetPlane(pPart->GetDefaultEntity(o3d_planeXOY));
					pSketchRectCut->Create();
					ksDocument2DPtr p2DDocRectCut = pSketchRectCutDef->BeginEdit();

					double rectPoint[4][2];

					rectPoint[0][0] = l - (b1 / 2);
					rectPoint[0][1] = D / 2.0;

					rectPoint[1][0] = l + (b1 / 2);
					rectPoint[1][1] = D / 2.0;

					rectPoint[2][0] = l + (b1 / 2);
					rectPoint[2][1] = D1 / 2.0;

					rectPoint[3][0] = l - (b1 / 2);
					rectPoint[3][1] = D1 / 2.0;

					p2DDocRectCut->ksLineSeg(rectPoint[0][0], rectPoint[0][1], rectPoint[1][0], rectPoint[1][1], 1);
					p2DDocRectCut->ksLineSeg(rectPoint[1][0], rectPoint[1][1], rectPoint[2][0], rectPoint[2][1], 1);
					p2DDocRectCut->ksLineSeg(rectPoint[2][0], rectPoint[2][1], rectPoint[3][0], rectPoint[3][1], 1);
					p2DDocRectCut->ksLineSeg(rectPoint[3][0], rectPoint[3][1], rectPoint[0][0], rectPoint[0][1], 1);

					p2DDocRectCut->ksLineSeg(-10, 0, L + 10, 0, 3);

					pSketchRectCutDef->EndEdit();

					ksEntityPtr pRotateCut = pPart->NewEntity(o3d_cutRotated);
					ksCutRotatedDefinitionPtr pRotateCutDef = pRotateCut->GetDefinition();
					pRotateCutDef->SetSketch(pSketchRectCut);
					pRotateCutDef->SetSideParam(TRUE, 360);
					pRotateCut->Create();
				}

				//-------------------------------------------------------------------------------
				// СОЗДАНИЕ МАРКЕРОВ
				//-------------------------------------------------------------------------------
				ksEntityCollectionPtr allFaces00 = (ksEntityCollectionPtr)pPart->EntityCollection(o3d_face);
				// 0. НАЧАЛЬНАЯ ГРАНЬ
				allFaces00->SelectByPoint(0, D / 2 - c, 0);
				for (int i = 0; i < allFaces00->GetCount(); i++) {
					ksEntityPtr face = allFaces00->GetByIndex(i);
					face->Putname("MuftaNachalo");
					face->Update();
				}

				ksEntityCollectionPtr allFaces0 = (ksEntityCollectionPtr)pPart->EntityCollection(o3d_face);
				// 1. ВЕРХНЯЯ ГРАНЬ
				allFaces0->SelectByPoint(L / 2, 0, (d / 2) * (-1));
				for (int i = 0; i < allFaces0->GetCount(); i++) {
					ksEntityPtr face = allFaces0->GetByIndex(i);
					face->Putname("MuftaCilinder");
					face->Update();
				}
				ksEntityCollectionPtr allFaces1 = (ksEntityCollectionPtr)pPart->EntityCollection(o3d_face);
				// 2. НИЖНЯЯ ГРАНЬ ВЫРЕЗА
				allFaces1->SelectByPoint(L / 2, 0, dt2 - (d / 2));
				for (int i = 0; i < allFaces1->GetCount(); i++) {
					ksEntityPtr face = allFaces1->GetByIndex(i);
					face->Putname("MuftaVirezBottom");
					face->Update();
				}

				ksEntityCollectionPtr allFaces2 = (ksEntityCollectionPtr)pPart->EntityCollection(o3d_face);
				// 3. БОКОВАЯ ГРАНЬ ВЫРЕЗА
				allFaces2->SelectByPoint(L / 2, b / 2, dt2 - (d / 2) - r);
				for (int i = 0; i < allFaces2->GetCount(); i++) {
					ksEntityPtr face = allFaces2->GetByIndex(i);
					face->Putname("MuftaVirezBok");
					face->Update();
				}

				ksEntityCollectionPtr allFaces3 = (ksEntityCollectionPtr)pPart->EntityCollection(o3d_face);
				// 3. ГРАНЬ ОТВЕРСТИЯ
				allFaces3->SelectByPoint(l + ((d1 - 1) / 2), 0, (D / 2 - c1) * (-1));//вычитаем тут жопа
				for (int i = 0; i < allFaces3->GetCount(); i++) {
					ksEntityPtr face = allFaces3->GetByIndex(i);
					face->Putname("MuftaOtverstie");
					face->Update();
				}
				//3. Грань выреза под кольцо
				ksEntityCollectionPtr allFaces87 = (ksEntityCollectionPtr)pPart->EntityCollection(o3d_face);
				allFaces87->SelectByPoint(l, 0, D1 / 2);
				for (int i = 0; i < allFaces87->GetCount(); i++) {
					ksEntityPtr face = allFaces87->GetByIndex(i);
					face->Putname("MuftaVirezKolcoNiz");
					face->Update();
				}
				//3. Грань выреза под кольцо

				ksEntityCollectionPtr allFaces88 = (ksEntityCollectionPtr)pPart->EntityCollection(o3d_face);
				allFaces88->SelectByPoint(l + (b1 / 2), 0, (D1 / 2) + (D - D1) / 2 - eps);
				for (int i = 0; i < allFaces88->GetCount(); i++) {
					ksEntityPtr face = allFaces88->GetByIndex(i);
					face->Putname("MuftaVirezKolcoBok");
					face->Update();
				}
				pDoc->SaveAs("C:\\KompasAssembly\\Mufta.m3d");
			}


			/*-------------------------------------------------------------------------------
			 СОЗДАНИЕ НОВОГО ОБЪЕКТА КОЛЬЦО
			-------------------------------------------------------------------------------*/
			if (b1 != 0)
			{
				pDoc = pKompasApp5->Document3D();
				pDoc->Create(false, true);
				pPart = pDoc->GetPart(pTop_Part);

				ksEntityPtr pSketchRing = pPart->NewEntity(o3d_sketch);
				ksSketchDefinitionPtr pSketchRingDef = pSketchRing->GetDefinition();
				pSketchRingDef->SetPlane(pPart->GetDefaultEntity(o3d_planeXOY));
				pSketchRing->Create();
				ksDocument2DPtr p2DDocRing = pSketchRingDef->BeginEdit();

				double rect[4][2];

				rect[0][0] = 0;
				rect[0][1] = (D1 / 2.0);

				rect[1][0] = b1;
				rect[1][1] = (D1 / 2.0);

				rect[2][0] = b1;
				rect[2][1] = (D1 / 2.0) + ((D - D1) / 2);

				rect[3][0] = 0;
				rect[3][1] = (D1 / 2.0) + ((D - D1) / 2);

				p2DDocRing->ksLineSeg(rect[0][0], rect[0][1], rect[1][0], rect[1][1], 1);
				p2DDocRing->ksLineSeg(rect[1][0], rect[1][1], rect[2][0], rect[2][1], 1);
				p2DDocRing->ksLineSeg(rect[2][0], rect[2][1], rect[3][0], rect[3][1], 1);
				p2DDocRing->ksLineSeg(rect[3][0], rect[3][1], rect[0][0], rect[0][1], 1);

				p2DDocRing->ksLineSeg(-10, 0, (D1 / 2.0) + (D1 / 2.0) + 10, 0, 3);

				pSketchRingDef->EndEdit();

				ksEntityPtr pRotateRing = pPart->NewEntity(o3d_bossRotated);
				ksBossRotatedDefinitionPtr pRotDefRing = pRotateRing->GetDefinition();
				pRotDefRing->SetSketch(pSketchRing);
				pRotDefRing->SetSideParam(TRUE, 340);  // угол 340 градусов
				pRotateRing->Create();

				/* ДОБАВЛЕНИЕ СКРУГЛЕНИЙ НА ВСЕ ГРАНИ КОЛЬЦА
				-------------------------------------------------------------------------------*/
				ksEntityCollectionPtr allEdgesRing = pPart->EntityCollection(o3d_edge);

				ksEntityPtr pFilletRing = pPart->NewEntity(o3d_fillet);
				ksFilletDefinitionPtr pFilletRingDef = pFilletRing->GetDefinition();
				pFilletRingDef->radius = 0.25;

				ksEntityCollectionPtr filletEdges = pFilletRingDef->array();
				filletEdges->Clear();

				for (int i = 0; i < allEdgesRing->GetCount(); i++) {
					ksEntityPtr edge = allEdgesRing->GetByIndex(i);
					ksEdgeDefinitionPtr edgeDef = edge->GetDefinition();

					if (edgeDef->GetOwnerEntity() == pRotateRing) {
						filletEdges->Add(edge);
					}
				}

				if (filletEdges->GetCount() > 0) {
					pFilletRing->Create();
				}

				/*СОЗДАНИЕ МАРКЕРОВ ДЛЯ КОЛЬЦА
			   -------------------------------------------------------------------------------*/
				ksEntityCollectionPtr allFacesRing = (ksEntityCollectionPtr)pPart->EntityCollection(o3d_face);
				/*1. ПЛОСКОСТЬ РАЗРЕЗА */
				allFacesRing->SelectByPoint(b1 / 2, D1 / 2 + 1, 0);
				for (int i = 0; i < allFacesRing->GetCount(); i++) {
					ksEntityPtr face = allFacesRing->GetByIndex(i);
					face->Putname("KolcoRazrez");
					face->Update();
				}
				ksEntityCollectionPtr allFacesRing1 = (ksEntityCollectionPtr)pPart->EntityCollection(o3d_face);
				/* 2. ВНУТРЕННЯЯ ЦИЛИНДРИЧЕСКАЯ ПОВЕРХНОСТЬ*/
				allFacesRing1->SelectByPoint(b1 / 2, 0, D1 / 2);
				for (int i = 0; i < allFacesRing1->GetCount(); i++) {
					ksEntityPtr face = allFacesRing1->GetByIndex(i);
					face->Putname("KolcoInnerCylinder");
					face->Update();
				}
				ksEntityCollectionPtr allFacesRing2 = (ksEntityCollectionPtr)pPart->EntityCollection(o3d_face);
				/* 2. ВНУТРЕННЯЯ ЦИЛИНДРИЧЕСКАЯ ПОВЕРХНОСТЬ*/
				allFacesRing2->SelectByPoint(b1, 0, D1 / 2 + b1 / 2);
				for (int i = 0; i < allFacesRing2->GetCount(); i++) {
					ksEntityPtr face = allFacesRing2->GetByIndex(i);
					face->Putname("KolcoLeftStorona");
					face->Update();
				}

				pDoc->SaveAs("C:\\KompasAssembly\\Kolco.m3d");
			}
			//СОЗДАНИЕ НОВОГО ОБЪЕКТА ВАЛ
			//-------------------------------------------------------------------------------
			{
				pDoc = pKompasApp5->Document3D();
				pDoc->Create(false, true);
				pPart = pDoc->GetPart(pTop_Part);

				// Создаем эскиз профиля вала
				ksEntityPtr pSketch = pPart->NewEntity(o3d_sketch);
				ksSketchDefinitionPtr pSketchDef = pSketch->GetDefinition();
				pSketchDef->SetPlane(pPart->GetDefaultEntity(o3d_planeXOY));
				pSketch->Create();
				ksDocument2DPtr p2DDoc = pSketchDef->BeginEdit();

				// Точки профиля вала (прямоугольник для вращения)
				double point[4][2];

				// Внешний контур (больший диаметр)
				point[0][0] = 0;
				point[0][1] = d / 2;

				point[1][0] = lV;
				point[1][1] = d / 2;


				point[2][0] = lV;
				point[2][1] = 0;

				point[3][0] = 0;
				point[3][1] = 0;

				// Рисуем внешний контур
				p2DDoc->ksLineSeg(point[0][0], point[0][1], point[1][0], point[1][1], 1);
				p2DDoc->ksLineSeg(point[1][0], point[1][1], point[2][0], point[2][1], 1);
				p2DDoc->ksLineSeg(point[2][0], point[2][1], point[3][0], point[3][1], 1);
				p2DDoc->ksLineSeg(point[3][0], point[3][1], point[0][0], point[0][1], 1);

				// Ось вращения (ось Y)
				p2DDoc->ksLineSeg(-d / 2, 0, d / 2, 0, 3);

				pSketchDef->EndEdit();

				// Вращение для создания вала
				ksEntityPtr pRotate = pPart->NewEntity(o3d_bossRotated);
				ksBossRotatedDefinitionPtr pRotDef = pRotate->GetDefinition();
				pRotDef->SetSketch(pSketch);
				pRotDef->SetSideParam(TRUE, 360);  // полное вращение на 360 градусов
				pRotate->Create();

				//СОЗДАНИЕ ФАСКИ ВАЛА
				//-------------------------------------------------------------------------------
				ksEntityCollectionPtr allEdges = pPart->EntityCollection(o3d_edge);
				ksEntityPtr pChamfer = pPart->NewEntity(o3d_chamfer);
				ksChamferDefinitionPtr pChamferDef = pChamfer->GetDefinition();
				pChamferDef->SetChamferParam(true, c, c);

				ksEntityCollectionPtr chamferEdges = pChamferDef->array();
				chamferEdges->Clear();


				// Перебираем все ребра
				for (int i = 0; i < allEdges->GetCount(); i++) {
					ksEntityPtr edge = allEdges->GetByIndex(i);
					ksEdgeDefinitionPtr edgeDef = edge->GetDefinition();

					if (edgeDef->IsCircle()) {
						ksVertexDefinitionPtr p1 = edgeDef->GetVertex(true);
						double x1, y1, z1;
						p1->GetPoint(&x1, &y1, &z1);
						if (x1 == 0 && y1 == d / 2)
						{
							chamferEdges->Add(edge);
							break;
						}
					}
				}
				pChamfer->Create();


				//СОЗДАНИЕ ВЫРЕЗА ПОД ШПОНКУ НА ВАЛУ
				//-------------------------------------------------------------------------------
				ksEntityPtr pPlane1 = pPart->NewEntity(o3d_planeOffset);
				ksPlaneOffsetDefinitionPtr pPlaneDef1 = pPlane1->GetDefinition();

				pPlaneDef1->direction = false;
				pPlaneDef1->offset = b / 2;
				pPlaneDef1->SetPlane(pPart->GetDefaultEntity(o3d_planeXOZ));
				pPlane1->Create();

				// Создаем эскиз для шпоночного паза
				ksEntityPtr pSketchKeyway = pPart->NewEntity(o3d_sketch);
				ksSketchDefinitionPtr pSketchKeywayDef = pSketchKeyway->GetDefinition();
				pSketchKeywayDef->SetPlane(pPlane1);
				pSketchKeyway->Create();
				ksDocument2DPtr p2DDocKeyway = pSketchKeywayDef->BeginEdit();

				// Параметры для создания паза
				double centerX = l;           // Центр по оси X
				double startY = -d / 2 - (dt2 - d);     // Начальная позиция по Y (на поверхности вала)//вычитаем

				// 1. Создаем линию длиной 16 мм
				double lineStartX = centerX - ShponkaW / 2;       // Начало линии (смещение на половину длины)
				double lineEndX = centerX + ShponkaW / 2;         // Конец линии
				p2DDocKeyway->ksLineSeg(lineStartX, startY, lineEndX, startY, 1);

				// 2. Создаем точку по центру отрезка на высоте 6.5 мм
				double midX = centerX;                   // Центр по X (середина линии)
				double midY = startY + ShponkaH;     // Центр по Y (на высоте 6.5 мм от линии)

				// 3. Создаем дугу по трем точкам:
				//    - начало линии (левая точка)
				//    - средняя точка на высоте 6.5 мм  
				//    - конец линии (правая точка)
				p2DDocKeyway->ksArcBy3Points(lineStartX, startY, midX, midY, lineEndX, startY, 1);

				pSketchKeywayDef->EndEdit();

				// Выдавливание шпоночного паза (вырез) на всю ширину
				ksEntityPtr pExtrudeKeyway = pPart->NewEntity(o3d_cutExtrusion);
				ksCutExtrusionDefinitionPtr pExtrudeKeywayDef = pExtrudeKeyway->GetDefinition();
				pExtrudeKeywayDef->SetSketch(pSketchKeyway);
				pExtrudeKeywayDef->SetSideParam(FALSE, etBlind, b, 0, FALSE);
				pExtrudeKeyway->Create();

				//МАРКЕРЫ ДЛЯ ШПОНОЧНОГО ПАЗА
				//-------------------------------------------------------------------------------
				ksEntityCollectionPtr allFaces4 = (ksEntityCollectionPtr)pPart->EntityCollection(o3d_face);
				// 1. нАЧАЛЬНАЯ ГРАНЬ
				allFaces4->SelectByPoint(0, 0, 0);
				for (int i = 0; i < allFaces4->GetCount(); i++) {
					ksEntityPtr face = allFaces4->GetByIndex(i);
					face->Putname("NachaloVal");
					face->Update();
				}
				ksEntityCollectionPtr allFaces5 = (ksEntityCollectionPtr)pPart->EntityCollection(o3d_face);
				// 2. ВНЕШНЯЯ ГРАНЬ
				allFaces5->SelectByPoint(lV - eps, 0, d / 2);
				for (int i = 0; i < allFaces5->GetCount(); i++) {
					ksEntityPtr face = allFaces5->GetByIndex(i);
					face->Putname("CilindrVal");
					face->Update();
				}
				ksEntityCollectionPtr allFaces6 = (ksEntityCollectionPtr)pPart->EntityCollection(o3d_face);
				// 3. БОКОВАЯ ГРАНЬ ШПОНКИ
				allFaces6->SelectByPoint(l, b / 2, d / 2 - eps);
				for (int i = 0; i < allFaces6->GetCount(); i++) {
					ksEntityPtr face = allFaces6->GetByIndex(i);
					face->Putname("BokSponkaVirezVal");
					face->Update();
				}
				ksEntityCollectionPtr allFaces7 = (ksEntityCollectionPtr)pPart->EntityCollection(o3d_face);
				// 4. ГРАНЬ ВЫРЕЗА ШПОНКИ
				allFaces7->SelectByPoint(l, 0, d / 2 - (ShponkaH - (dt2 - d)));//высота дуги - высота выреза под шпонку //вычитаем
				for (int i = 0; i < allFaces7->GetCount(); i++) {
					ksEntityPtr face = allFaces7->GetByIndex(i);
					face->Putname("SponkaVirezVal");
					face->Update();
				}

				//threadedHoleX
				pDoc->SaveAs("C:\\KompasAssembly\\Val.m3d");
			}

			// СОЗДАНИЕ НОВОГО ОБЪЕКТА ШПОНКА
			//-------------------------------------------------------------------------------
			{
				pDoc = pKompasApp5->Document3D();
				pDoc->Create(false, true);
				pPart = pDoc->GetPart(pTop_Part);

				// Создаем эскиз профиля шпонки
				ksEntityPtr pSketch = pPart->NewEntity(o3d_sketch);
				ksSketchDefinitionPtr pSketchDef = pSketch->GetDefinition();
				pSketchDef->SetPlane(pPart->GetDefaultEntity(o3d_planeXOZ));
				pSketch->Create();
				ksDocument2DPtr p2DDoc = pSketchDef->BeginEdit();

				// 1. Создаем линию длиной shponkaLength
				double lineStartX = -ShponkaW / 2;  // Начало линии (смещение на половину длины)
				double lineEndX = ShponkaW / 2;     // Конец линии
				double lineY = 0;                   // Позиция по Y
				p2DDoc->ksLineSeg(lineStartX, lineY, lineEndX, lineY, 1);

				// 2. Создаем точку по центру отрезка на высоте shponkaHeight (6.5 мм)
				double midX = 0;                         // Центр по X (середина линии)
				double midY = ShponkaH;             // Центр по Y (на высоте 6.5 мм от линии)

				// 3. Создаем дугу по трем точкам:
				p2DDoc->ksArcBy3Points(lineStartX, lineY, midX, midY, lineEndX, lineY, 1);

				pSketchDef->EndEdit();

				// Выдавливаем шпонку на ширину shponkaWidth (5 мм)
				ksEntityPtr pExtrude = pPart->NewEntity(o3d_bossExtrusion);
				ksBossExtrusionDefinitionPtr pExtrudeDef = pExtrude->GetDefinition();
				pExtrudeDef->SetSketch(pSketch);
				pExtrudeDef->SetSideParam(TRUE, etBlind, b, 0, FALSE);
				pExtrude->Create();

				// Создаем фаски на торцах шпонки
				ksEntityCollectionPtr allEdges = pPart->EntityCollection(o3d_edge);
				ksEntityPtr pChamfer = pPart->NewEntity(o3d_chamfer);
				ksChamferDefinitionPtr pChamferDef = pChamfer->GetDefinition();
				pChamferDef->SetChamferParam(true, ShponkaR, ShponkaR);

				ksEntityCollectionPtr chamferEdges = pChamferDef->array();
				chamferEdges->Clear();

				// Добавляем торцевые ребра для фасок
				for (int i = 0; i < allEdges->GetCount(); i++) {
					ksEntityPtr edge = allEdges->GetByIndex(i);
					ksEdgeDefinitionPtr edgeDef = edge->GetDefinition();

					if (edgeDef->GetOwnerEntity() == pExtrude) {
						// Проверяем координаты торцов
						ksVertexDefinitionPtr v1 = edgeDef->GetVertex(true);
						ksVertexDefinitionPtr v2 = edgeDef->GetVertex(false);
						double x1, y1, z1, x2, y2, z2;
						v1->GetPoint(&x1, &y1, &z1);
						v2->GetPoint(&x2, &y2, &z2);

						// Торцевые ребра (z=0 или z=ширина)
						if ((abs(z1) < 0.001 && abs(z2) < 0.001) ||
							(abs(z1 - b) < 0.001 && abs(z2 - b) < 0.001)) {
							chamferEdges->Add(edge);
						}
					}
				}

				if (chamferEdges->GetCount() > 0) {
					pChamfer->Create();
				}

				//-------------------------------------------------------------------------------
				// СОЗДАЕМ МАРКЕР ШПОНКИ
				//-------------------------------------------------------------------------------
				ksEntityCollectionPtr allFaces8 = (ksEntityCollectionPtr)pPart->EntityCollection(o3d_face);
				// 1. НИЖНЯЯ  ГРАНЬ
				allFaces8->SelectByPoint(0, b / 2, 0);
				for (int i = 0; i < allFaces8->GetCount(); i++) {
					ksEntityPtr face = allFaces8->GetByIndex(i);
					face->Putname("ShponkaBottom");
					face->Update();
				}
				ksEntityCollectionPtr allFaces9 = (ksEntityCollectionPtr)pPart->EntityCollection(o3d_face);
				// 2. БОКОВАЯ ГРАНЬ
				allFaces9->SelectByPoint(0, b, -ShponkaH / 2);
				for (int i = 0; i < allFaces9->GetCount(); i++) {
					ksEntityPtr face = allFaces9->GetByIndex(i);
					face->Putname("ShponkaBok");
					face->Update();
				}
				ksEntityCollectionPtr allFaces10 = (ksEntityCollectionPtr)pPart->EntityCollection(o3d_face);
				// 3.ВЕРХНЯЯ ГРАНЬ
				allFaces10->SelectByPoint(0, b / 2, -ShponkaH);
				for (int i = 0; i < allFaces10->GetCount(); i++) {
					ksEntityPtr face = allFaces10->GetByIndex(i);
					face->Putname("ShponkaTop");
					face->Update();
				}
				pDoc->SaveAs(L"C:\\KompasAssembly\\Shponka.m3d");
			}



			// СОЗДАНИЕ НОВОГО ОБЪЕКТА ВИНТ
			//-------------------------------------------------------------------------------
			{
				pDoc = pKompasApp5->Document3D();
				pDoc->Create(false, true);
				pPart = pDoc->GetPart(pTop_Part);

				// Эскиз для стержня винта
				ksEntityPtr pRodSketch = pPart->NewEntity(o3d_sketch);
				ksSketchDefinitionPtr pRodSketchDef = pRodSketch->GetDefinition();
				pRodSketchDef->SetPlane(pPart->GetDefaultEntity(o3d_planeXOY));
				pRodSketch->Create();
				ksDocument2DPtr p2DRod = pRodSketchDef->BeginEdit();

				// Создаем круг для стержня
				p2DRod->ksCircle(0, 0, d1 / 2, 1);

				pRodSketchDef->EndEdit();

				// Выдавливаем стержень
				ksEntityPtr pRodExtrude = pPart->NewEntity(o3d_bossExtrusion);
				ksBossExtrusionDefinitionPtr pRodExtrudeDef = pRodExtrude->GetDefinition();
				pRodExtrudeDef->SetSketch(pRodSketch);
				pRodExtrudeDef->SetSideParam(TRUE, etBlind, VintLenght, 0, FALSE);
				pRodExtrude->Create();


				// СОЗДАЕМ ФАСКИ НА ВИНТЕ
				//-------------------------------------------------------------------------------
				ksEntityCollectionPtr allEdges = pPart->EntityCollection(o3d_edge);

				// ПЕРВАЯ ФАСКА 0.3 мм (один конец)
				ksEntityPtr pChamfer1 = pPart->NewEntity(o3d_chamfer);
				ksChamferDefinitionPtr pChamferDef1 = pChamfer1->GetDefinition();
				pChamferDef1->SetChamferParam(true, 0.3, 0.3);

				ksEntityCollectionPtr chamferEdges1 = pChamferDef1->array();
				chamferEdges1->Clear();

				// ВТОРАЯ ФАСКА 2.25 мм (противоположный конец)
				ksEntityPtr pChamfer2 = pPart->NewEntity(o3d_chamfer);
				ksChamferDefinitionPtr pChamferDef2 = pChamfer2->GetDefinition();
				pChamferDef2->SetChamferParam(true, 1.75, 1.75);

				ksEntityCollectionPtr chamferEdges2 = pChamferDef2->array();
				chamferEdges2->Clear();

				// Ищем все ребра стержня и распределяем по концам
				for (int i = 0; i < allEdges->GetCount(); i++) {
					ksEntityPtr edge = allEdges->GetByIndex(i);
					ksEdgeDefinitionPtr edgeDef = edge->GetDefinition();

					if (edgeDef->GetOwnerEntity() == pRodExtrude && edgeDef->IsCircle()) {
						ksVertexDefinitionPtr v1 = edgeDef->GetVertex(true);
						double x, y, z;
						v1->GetPoint(&x, &y, &z);

						// Определяем к какому концу принадлежит ребро
						if (abs(z) < 0.001) {
							// Ребро на начале стержня (z = 0)
							chamferEdges1->Add(edge);
						}
						else if (abs(z - VintLenght) < 0.001) {
							// Ребро на конце стержня (z = rodLength)
							chamferEdges2->Add(edge);
						}
					}
				}

				// Создаем фаски
				if (chamferEdges1->GetCount() > 0) {
					pChamfer1->Create();
				}

				if (chamferEdges2->GetCount() > 0) {
					pChamfer2->Create();
				}

				// СОЗДАЕМ ШЛИЦ ВНУТРИ ВИНТА
				//-------------------------------------------------------------------------------
				// Смещаем плоскость для создания шлица
				ksEntityPtr pPlaneOffset = pPart->NewEntity(o3d_planeOffset);
				ksPlaneOffsetDefinitionPtr pPlaneOffsetDef = pPlaneOffset->GetDefinition();
				pPlaneOffsetDef->direction = false;
				pPlaneOffsetDef->offset = d1 / 2;
				pPlaneOffsetDef->SetPlane(pPart->GetDefaultEntity(o3d_planeXOZ));
				pPlaneOffset->Create();

				// Эскиз для шлица
				ksEntityPtr pSlotSketch = pPart->NewEntity(o3d_sketch);
				ksSketchDefinitionPtr pSlotSketchDef = pSlotSketch->GetDefinition();
				pSlotSketchDef->SetPlane(pPlaneOffset);
				pSlotSketch->Create();
				ksDocument2DPtr p2DSlot = pSlotSketchDef->BeginEdit();

				// Создаем прямоугольник для шлица
				double slotHalfWidth = VintH / 2;
				p2DSlot->ksLineSeg(-slotHalfWidth, 0, slotHalfWidth, 0, 1);
				p2DSlot->ksLineSeg(slotHalfWidth, 0, slotHalfWidth, -VintW, 1);
				p2DSlot->ksLineSeg(slotHalfWidth, -VintW, -slotHalfWidth, -VintW, 1);
				p2DSlot->ksLineSeg(-slotHalfWidth, -VintW, -slotHalfWidth, 0, 1);

				pSlotSketchDef->EndEdit();

				// Вырезаем шлиц
				ksEntityPtr pSlotCut = pPart->NewEntity(o3d_cutExtrusion);
				ksCutExtrusionDefinitionPtr pSlotCutDef = pSlotCut->GetDefinition();
				pSlotCutDef->SetSketch(pSlotSketch);
				pSlotCutDef->SetSideParam(FALSE, etBlind, d1, 0, FALSE);
				pSlotCut->Create();

				//fdsfsdfsdfdsfds
				ksEntityPtr pThread = pPart->NewEntity(o3d_thread);
				ksThreadDefinitionPtr pThreadDef = pThread->GetDefinition();

				// Настройка параметров резьбы M6
				pThreadDef->allLength = true; // признак полной длины
				pThreadDef->autoDefinDr = true; // признак автоопределения диаметра
				pThreadDef->dr = d1; // номинальный диаметр резьбы M6
				pThreadDef->length = VintLenght; // длина резьбы (по всей длине стержня)
				pThreadDef->faceValue = false; // направление построения резьбы (false - наружная резьба)
				pThreadDef->p = p; // шаг резьбы M6 составляет 1 мм

				// Ищем цилиндрическую грань стержня для нанесения резьбы
				ksEntityCollectionPtr faceCollection = pPart->EntityCollection(o3d_face);
				ksEntityPtr threadFace = nullptr;

				for (int i = 0; i < faceCollection->GetCount(); i++)
				{
					ksEntityPtr face = faceCollection->GetByIndex(i);
					ksFaceDefinitionPtr faceDef = face->GetDefinition();

					// Проверяем, что грань принадлежит нашему выдавливанию стержня и является цилиндрической
					if (faceDef->GetOwnerEntity() == pRodExtrude && faceDef->IsCylinder())
					{
						threadFace = face;
						break;
					}
				}

				if (threadFace != nullptr)
				{
					pThreadDef->SetBaseObject(threadFace);
					pThread->Create();
				}

				//-------------------------------------------------------------------------------
				// СОЗДАЕМ МАРКЕРЫ ГРАНЕЙ ВИНТА
				//-------------------------------------------------------------------------------
				ksEntityCollectionPtr allFaces11 = (ksEntityCollectionPtr)pPart->EntityCollection(o3d_face);
				// 1. НИЖНЯЯ  ГРАНЬ
				allFaces11->SelectByPoint(0, 0, VintLenght);
				for (int i = 0; i < allFaces11->GetCount(); i++) {
					ksEntityPtr face = allFaces11->GetByIndex(i);
					face->Putname("VintKon");
					face->Update();
				}
				ksEntityCollectionPtr allFaces12 = (ksEntityCollectionPtr)pPart->EntityCollection(o3d_face);
				// 2. БОКОВАЯ ГРАНЬ
				allFaces12->SelectByPoint(d1 / 2, 0, VintLenght / 2);
				for (int i = 0; i < allFaces12->GetCount(); i++) {
					ksEntityPtr face = allFaces12->GetByIndex(i);
					face->Putname("VintVnesh");
					face->Update();
				}

				// Сохраняем деталь винта
				pDoc->SaveAs(L"C:\\KompasAssembly\\Vint.m3d");
			}

			// СОЗДАНИЕ СБОРКИ
			//-------------------------------------------------------------------------------
			{
				pDoc = pKompasApp5->Document3D();
				pDoc->Create(false, false);
				pPart = pDoc->GetPart(pTop_Part);
				// Добавляем компоненты в сборку
				pDoc->SetPartFromFile(L"C:\\KompasAssembly\\Mufta.m3d", pPart, VARIANT_FALSE);
				pDoc->SetPartFromFile(L"C:\\KompasAssembly\\Val.m3d", pPart, VARIANT_FALSE);
				pDoc->SetPartFromFile(L"C:\\KompasAssembly\\Val.m3d", pPart, VARIANT_FALSE);
				pDoc->SetPartFromFile(L"C:\\KompasAssembly\\Shponka.m3d", pPart, VARIANT_FALSE);
				pDoc->SetPartFromFile(L"C:\\KompasAssembly\\Shponka.m3d", pPart, VARIANT_FALSE);
				pDoc->SetPartFromFile(L"C:\\KompasAssembly\\Vint.m3d", pPart, VARIANT_FALSE);
				if (b1 != 0)
				{
					pDoc->SetPartFromFile(L"C:\\KompasAssembly\\Kolco.m3d", pPart, VARIANT_FALSE);
				}

				// Получаем добавленные части
				ksPartPtr pMufta = pDoc->GetPart(0);  // Муфта
				ksPartPtr pVal1 = pDoc->GetPart(1);   // Первый вал
				ksPartPtr pVal2 = pDoc->GetPart(2);   // Второй вал
				ksPartPtr pShponka1 = pDoc->GetPart(3); // Первая шпонка
				ksPartPtr pShponka2 = pDoc->GetPart(4); // Вторая шпонка
				ksPartPtr pVint = pDoc->GetPart(5); // Винт
				ksPartPtr pKolco = pDoc->GetPart(6); // Кольцо

				ksEntityCollectionPtr col = pMufta->EntityCollection(o3d_face);
				// Грани муфты  
				ksEntityPtr MuftaNachalo = col->GetByName(L"MuftaNachalo", true, true);
				ksEntityPtr MuftaCylinder = col->GetByName(L"MuftaCilinder", true, true);
				ksEntityPtr MuftaVbot = col->GetByName(L"MuftaVirezBottom", true, true);
				ksEntityPtr MufttaVbok = col->GetByName(L"MuftaVirezBok", true, true);
				ksEntityPtr MuftaOtverstie = col->GetByName(L"MuftaOtverstie", true, true);
				ksEntityPtr MuftaKolVirezBok = col->GetByName(L"MuftaVirezKolcoBok", true, true);
				ksEntityPtr MuftaKolVirezNiz = col->GetByName(L"MuftaVirezKolcoNiz", true, true);

				col = pVal1->EntityCollection(o3d_face);
				// Грани первого вала
				ksEntityPtr Val1Nach = col->GetByName(L"NachaloVal", true, true);
				ksEntityPtr Val1Cylinder = col->GetByName(L"CilindrVal", true, true);
				ksEntityPtr Val1VirezBok = col->GetByName(L"BokSponkaVirezVal", true, true);
				ksEntityPtr Val1VirezBot = col->GetByName(L"SponkaVirezVal", true, true);

				col = pVal2->EntityCollection(o3d_face);
				// Грани второго вала
				ksEntityPtr Val2Nach = col->GetByName(L"NachaloVal", true, true);
				ksEntityPtr Val2Cylinder = col->GetByName(L"CilindrVal", true, true);
				ksEntityPtr Val2VirezBok = col->GetByName(L"BokSponkaVirezVal", true, true);
				ksEntityPtr Val2VirezBot = col->GetByName(L"SponkaVirezVal", true, true);

				col = pShponka1->EntityCollection(o3d_face);
				// Грани первой шпонки
				ksEntityPtr Shponka1Bok = col->GetByName(L"ShponkaBok", true, true);
				ksEntityPtr Shponka1Bottom = col->GetByName(L"ShponkaBottom", true, true);
				ksEntityPtr Shponka1Top = col->GetByName(L"ShponkaTop", true, true);

				col = pShponka2->EntityCollection(o3d_face);
				// Грани второй шпонки
				ksEntityPtr Shponka2Bok = col->GetByName(L"ShponkaBok", true, true);
				ksEntityPtr Shponka2Bottom = col->GetByName(L"ShponkaBottom", true, true);
				ksEntityPtr Shponka2Top = col->GetByName(L"ShponkaTop", true, true);

				col = pVint->EntityCollection(o3d_face);
				// Грани Винта
				ksEntityPtr VintCilindr = col->GetByName(L"VintVnesh", true, true);
				ksEntityPtr VintKonchik = col->GetByName(L"VintKon", true, true);
				if (b1 != 0)
				{
					col = pKolco->EntityCollection(o3d_face);
					// Грани Кольца
					ksEntityPtr KolcoRazre = col->GetByName(L"KolcoRazrez", true, true);
					ksEntityPtr KolcoVnutr = col->GetByName(L"KolcoInnerCylinder", true, true);
					ksEntityPtr KolcoLStorona = col->GetByName(L"KolcoLeftStorona", true, true);

					//кольца
					pDoc->AddMateConstraint(mc_Coincidence, KolcoLStorona, MuftaKolVirezBok, -1, 1, 0);
					pDoc->AddMateConstraint(mc_Concentric, KolcoVnutr, MuftaKolVirezNiz, 1, 1, 0);
				}
				//валы
				pDoc->AddMateConstraint(mc_Concentric, MuftaCylinder, Val1Cylinder, 1, 1, 0);
				pDoc->AddMateConstraint(mc_Concentric, MuftaCylinder, Val2Cylinder, -1, 1, 0);

				pDoc->AddMateConstraint(mc_Distance, Val1Nach, Val2Nach, -1, 1, 2);

				//шпоночные отверстия 
				pDoc->AddMateConstraint(mc_Parallel, Val1VirezBok, Val2VirezBok, -1, 1, 0);
				pDoc->AddMateConstraint(mc_Parallel, MufttaVbok, Val1VirezBok, -1, 1, 0);

				//шпонка
				pDoc->AddMateConstraint(mc_Coincidence, Shponka1Bottom, MuftaVbot, -1, 1, 0);
				pDoc->AddMateConstraint(mc_Coincidence, Shponka2Bottom, MuftaVbot, -1, 1, 0);

				pDoc->AddMateConstraint(mc_Coincidence, Shponka1Bok, MufttaVbok, -1, 1, 0);
				pDoc->AddMateConstraint(mc_Coincidence, Shponka2Bok, MufttaVbok, -1, 1, 0);

				pDoc->AddMateConstraint(mc_Concentric, Shponka1Top, Val1VirezBot, -1, 1, 0);
				pDoc->AddMateConstraint(mc_Concentric, Shponka2Top, Val2VirezBot, 1, 1, 0);

				//муфта
				pDoc->AddMateConstraint(mc_Distance, Val2Nach, MuftaNachalo, 1, 1, (L / 2) + 2);

				//винт
				pDoc->AddMateConstraint(mc_Concentric, MuftaOtverstie, VintCilindr, 1, 1, 0);
				pDoc->AddMateConstraint(5, VintKonchik, Val1Cylinder, 1, 1, 0);



				// Перестраиваем сборку
				pDoc->RebuildDocument();

				// Сохраняем сборку
				pDoc->SaveAs(L"C:\\KompasAssembly\\ItogProdgect.a3d");
			}
		}

		else 
		{
			AfxMessageBox(L"Укажите все параметры сборки...");
		}
	}
}