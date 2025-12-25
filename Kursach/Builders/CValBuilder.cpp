#include "pch.h"

CValBuilder::CValBuilder(KompasObjectPtr pKompasApp)
	: m_pKompasApp(pKompasApp)
{
}

CValBuilder::~CValBuilder()
{
}

bool CValBuilder::Build(double d, double dt2, double b, double l, double lV,
	double ShponkaH, double ShponkaW, double c, double eps,
	const CString& savePath)
{
	try
	{
		// Сохраняем параметры
		m_d = d; m_dt2 = dt2; m_b = b; m_l = l; m_lV = lV;
		m_ShponkaH = ShponkaH; m_ShponkaW = ShponkaW; m_c = c;

		// Создаем новый документ
		m_pDoc =  m_pKompasApp->Document3D();
		if (!m_pDoc)
			return false;

		m_pDoc->Create(false, true);
		m_pPart = m_pDoc->GetPart(pTop_Part);
		if (!m_pPart)
			return false;

		// Создаем основные элементы
		CreateMainBody();
		CreateChamfer();
		CreateKeyway();
		SetFaceMarkers();

		// Сохраняем
		m_pDoc->SaveAs(_bstr_t(savePath));

		return true;
	}
	catch (...)
	{
		return false;
	}
}

void CValBuilder::CreateMainBody()
{
	// Создаем эскиз профиля вала
	ksEntityPtr pSketch =              m_pPart->NewEntity(o3d_sketch);
	ksSketchDefinitionPtr pSketchDef = pSketch->GetDefinition();
	pSketchDef->SetPlane(m_pPart->GetDefaultEntity(o3d_planeXOY));
	pSketch->Create();
	ksDocument2DPtr p2DDoc =           pSketchDef->BeginEdit();

	// Точки профиля вала (прямоугольник для вращения)
	double point[4][2];

	// Внешний контур (больший диаметр)
	point[0][0] = 0;
	point[0][1] = m_d / 2;

	point[1][0] = m_lV;
	point[1][1] = m_d / 2;

	point[2][0] = m_lV;
	point[2][1] = 0;

	point[3][0] = 0;
	point[3][1] = 0;

	// Рисуем внешний контур
	p2DDoc->ksLineSeg(point[0][0], point[0][1], point[1][0], point[1][1], 1);
	p2DDoc->ksLineSeg(point[1][0], point[1][1], point[2][0], point[2][1], 1);
	p2DDoc->ksLineSeg(point[2][0], point[2][1], point[3][0], point[3][1], 1);
	p2DDoc->ksLineSeg(point[3][0], point[3][1], point[0][0], point[0][1], 1);

	// Ось вращения (ось Y)
	p2DDoc->ksLineSeg(-m_d / 2, 0, m_d / 2, 0, 3);

	pSketchDef->EndEdit();

	// Вращение для создания вала
	ksEntityPtr pRotate =                m_pPart->NewEntity(o3d_bossRotated);
	ksBossRotatedDefinitionPtr pRotDef = pRotate->GetDefinition();
	pRotDef->SetSketch(pSketch);
	pRotDef->SetSideParam(TRUE, 360);  // полное вращение на 360 градусов
	pRotate->Create();
}

void CValBuilder::CreateChamfer()
{
	ksEntityCollectionPtr allEdges =     m_pPart->EntityCollection(o3d_edge);
	ksEntityPtr pChamfer =               m_pPart->NewEntity(o3d_chamfer);
	ksChamferDefinitionPtr pChamferDef = pChamfer->GetDefinition();
	pChamferDef->SetChamferParam(true, m_c, m_c);

	ksEntityCollectionPtr chamferEdges = pChamferDef->array();
	chamferEdges->Clear();

	// Перебираем все ребра
	for (int i = 0; i < allEdges->GetCount(); i++)
	{
		ksEntityPtr edge =             allEdges->GetByIndex(i);
		ksEdgeDefinitionPtr edgeDef =  edge->GetDefinition();

		if (edgeDef->IsCircle()) {
			ksVertexDefinitionPtr p1 = edgeDef->GetVertex(true);
			double x1, y1, z1;
			p1->GetPoint(&x1, &y1, &z1);
			if (x1 == 0 && y1 == m_d / 2)
			{
				chamferEdges->Add(edge);
				break;
			}
		}
	}

	if (chamferEdges->GetCount() > 0)
		pChamfer->Create();
}

void CValBuilder::CreateKeyway()
{
	// Смещаем плоскость для создания шпоночного паза
	ksEntityPtr pPlane1 =                          m_pPart->NewEntity(o3d_planeOffset);
	ksPlaneOffsetDefinitionPtr pPlaneDef1 =        pPlane1->GetDefinition();

	pPlaneDef1->direction =                        false;
	pPlaneDef1->offset =                           m_b / 2;
	pPlaneDef1->SetPlane(m_pPart->GetDefaultEntity(o3d_planeXOZ));
	pPlane1->Create();

	// Создаем эскиз для шпоночного паза
	ksEntityPtr pSketchKeyway =                     m_pPart->NewEntity(o3d_sketch);
	ksSketchDefinitionPtr pSketchKeywayDef =        pSketchKeyway->GetDefinition();
	pSketchKeywayDef->SetPlane(pPlane1);
	pSketchKeyway->Create();
	ksDocument2DPtr p2DDocKeyway =                  pSketchKeywayDef->BeginEdit();

	// Параметры для создания паза
	double centerX =                                m_l;           // Центр по оси X
	double startY =                                 -m_d / 2 - (m_dt2 - m_d); // Начальная позиция по Y

	// 1. Создаем линию длиной ShponkaW
	double lineStartX =                             centerX - m_ShponkaW / 2;       // Начало линии
	double lineEndX =                               centerX + m_ShponkaW / 2;         // Конец линии
	p2DDocKeyway->ksLineSeg(lineStartX, startY, lineEndX, startY, 1);

	// 2. Создаем точку по центру отрезка на высоте ShponkaH
	double midX =                                   centerX;                   // Центр по X
	double midY =                                   startY + m_ShponkaH;       // Центр по Y

	// 3. Создаем дугу по трем точкам
	p2DDocKeyway->ksArcBy3Points(lineStartX, startY, midX, midY, lineEndX, startY, 1);

	pSketchKeywayDef->EndEdit();

	// Выдавливание шпоночного паза (вырез) на всю ширину
	ksEntityPtr pExtrudeKeyway =                    m_pPart->NewEntity(o3d_cutExtrusion);
	ksCutExtrusionDefinitionPtr pExtrudeKeywayDef = pExtrudeKeyway->GetDefinition();
	pExtrudeKeywayDef->SetSketch(pSketchKeyway);
	pExtrudeKeywayDef->SetSideParam(FALSE, etBlind, m_b, 0, FALSE);
	pExtrudeKeyway->Create();
}

void CValBuilder::SetFaceMarkers()
{
	ksEntityCollectionPtr allFaces4 =               (ksEntityCollectionPtr)m_pPart->EntityCollection(o3d_face);
	// 1. НАЧАЛЬНАЯ ГРАНЬ
	allFaces4->SelectByPoint(0, 0, 0);
	for (int i = 0; i < allFaces4->GetCount(); i++)
	{
		ksEntityPtr face =                           allFaces4->GetByIndex(i);
		face->Putname("NachaloVal");
		face->Update();
	}

	ksEntityCollectionPtr allFaces5 =                (ksEntityCollectionPtr)m_pPart->EntityCollection(o3d_face);
	// 2. ВНЕШНЯЯ ГРАНЬ (ЦИЛИНДР)
	allFaces5->SelectByPoint(m_lV - 1, 0, m_d / 2);
	for (int i = 0; i < allFaces5->GetCount(); i++)
	{
		ksEntityPtr face =                           allFaces5->GetByIndex(i);
		face->Putname("CilindrVal");
		face->Update();
	}

	ksEntityCollectionPtr allFaces6 =                (ksEntityCollectionPtr)m_pPart->EntityCollection(o3d_face);
	// 3. БОКОВАЯ ГРАНЬ ШПОНКИ
	allFaces6->SelectByPoint(m_l, m_b / 2, m_d / 2 - 1);
	for (int i = 0; i < allFaces6->GetCount(); i++)
	{
		ksEntityPtr face =                            allFaces6->GetByIndex(i);
		face->Putname("BokSponkaVirezVal");
		face->Update();
	}

	ksEntityCollectionPtr allFaces7 =                (ksEntityCollectionPtr)m_pPart->EntityCollection(o3d_face);
	// 4. ГРАНЬ ВЫРЕЗА ШПОНКИ (НИЖНЯЯ)
	allFaces7->SelectByPoint(m_l, 0, m_d / 2 - (m_ShponkaH - (m_dt2 - m_d)));
	for (int i = 0; i < allFaces7->GetCount(); i++)
	{
		ksEntityPtr face =                           allFaces7->GetByIndex(i);
		face->Putname("SponkaVirezVal");
		face->Update();
	}
}
