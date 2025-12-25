#include "pch.h"

CShponkaBuilder::CShponkaBuilder(KompasObjectPtr pKompasApp)
	: m_pKompasApp(pKompasApp)
{
}

CShponkaBuilder::~CShponkaBuilder()
{
}

bool CShponkaBuilder::Build(double b, double ShponkaH, double ShponkaW,
	double ShponkaR, const CString& savePath)
{
	try
	{
		// Сохраняем параметры
		m_b = b; m_ShponkaH = ShponkaH; m_ShponkaW = ShponkaW; m_ShponkaR = ShponkaR;

		// Создаем новый документ
		m_pDoc = m_pKompasApp->Document3D();
		if (!m_pDoc)
			return false;

		m_pDoc->Create(false, true);
		m_pPart = m_pDoc->GetPart(pTop_Part);
		if (!m_pPart)
			return false;

		// Создаем основные элементы
		CreateMainBody();
		CreateChamfers();
		SetFaceMarkers();

		// Сохраняем
		m_pDoc->SaveAs((LPCTSTR)savePath);

		return true;
	}
	catch (...)
	{
		return false;
	}
}

void CShponkaBuilder::CreateMainBody()
{
	// Создаем эскиз профиля шпонки
	ksEntityPtr pSketch = m_pPart->NewEntity(o3d_sketch);
	ksSketchDefinitionPtr pSketchDef = pSketch->GetDefinition();
	pSketchDef->SetPlane(m_pPart->GetDefaultEntity(o3d_planeXOZ));
	pSketch->Create();
	ksDocument2DPtr p2DDoc = pSketchDef->BeginEdit();

	// 1. Создаем линию длиной ShponkaW
	double lineStartX = -m_ShponkaW / 2;  // Начало линии
	double lineEndX =   m_ShponkaW / 2;     // Конец линии
	double lineY =      0;                     // Позиция по Y
	p2DDoc->ksLineSeg(lineStartX, lineY, lineEndX, lineY, 1);

	// 2. Создаем точку по центру отрезка на высоте ShponkaH
	double midX =       0;                      // Центр по X
	double midY =       m_ShponkaH;             // Центр по Y

	// 3. Создаем дугу по трем точкам
	p2DDoc->ksArcBy3Points(lineStartX, lineY, midX, midY, lineEndX, lineY, 1);

	pSketchDef->EndEdit();

	// Выдавливаем шпонку на ширину b
	ksEntityPtr pExtrude = m_pPart->NewEntity(o3d_bossExtrusion);
	ksBossExtrusionDefinitionPtr pExtrudeDef = pExtrude->GetDefinition();
	pExtrudeDef->SetSketch(pSketch);
	pExtrudeDef->SetSideParam(TRUE, etBlind, m_b, 0, FALSE);
	pExtrude->Create();
}

void CShponkaBuilder::CreateChamfers()
{
	ksEntityCollectionPtr allEdges =     m_pPart->EntityCollection(o3d_edge);
	ksEntityPtr pChamfer =               m_pPart->NewEntity(o3d_chamfer);
	ksChamferDefinitionPtr pChamferDef = pChamfer->GetDefinition();
	pChamferDef->SetChamferParam(true, m_ShponkaR, m_ShponkaR);

	ksEntityCollectionPtr chamferEdges = pChamferDef->array();
	chamferEdges->Clear();

	// Добавляем торцевые ребра для фасок
	for (int i = 0; i < allEdges->GetCount(); i++)
	{
		ksEntityPtr edge = allEdges->GetByIndex(i);
		ksEdgeDefinitionPtr edgeDef = edge->GetDefinition();

		ksEntityCollectionPtr edges = m_pPart->EntityCollection(o3d_edge);
		if (edgeDef->GetOwnerEntity() == edges->GetByIndex(0))
		{
			// Проверяем координаты торцов
			ksVertexDefinitionPtr v1 = edgeDef->GetVertex(true);
			ksVertexDefinitionPtr v2 = edgeDef->GetVertex(false);
			double x1, y1, z1, x2, y2, z2;
			v1->GetPoint(&x1, &y1, &z1);
			v2->GetPoint(&x2, &y2, &z2);

			// Торцевые ребра (z=0 или z=ширина)
			if ((abs(z1) < 0.001 && abs(z2) < 0.001) ||
				(abs(z1 - m_b) < 0.001 && abs(z2 - m_b) < 0.001))
			{
				chamferEdges->Add(edge);
			}
		}
	}

	if (chamferEdges->GetCount() > 0) {
		pChamfer->Create();
	}
}

void CShponkaBuilder::SetFaceMarkers()
{
	ksEntityCollectionPtr allFaces8 = (ksEntityCollectionPtr)m_pPart->EntityCollection(o3d_face);
	// 1. НИЖНЯЯ ГРАНЬ
	allFaces8->SelectByPoint(0, m_b / 2, 0);
	for (int i = 0; i < allFaces8->GetCount(); i++)
	{
		ksEntityPtr face = allFaces8->GetByIndex(i);
		face->Putname("ShponkaBottom");
		face->Update();
	}

	ksEntityCollectionPtr allFaces9 = (ksEntityCollectionPtr)m_pPart->EntityCollection(o3d_face);
	// 2. БОКОВАЯ ГРАНЬ
	allFaces9->SelectByPoint(0, m_b, -m_ShponkaH / 2);
	for (int i = 0; i < allFaces9->GetCount(); i++)
	{
		ksEntityPtr face = allFaces9->GetByIndex(i);
		face->Putname("ShponkaBok");
		face->Update();
	}

	ksEntityCollectionPtr allFaces10 = (ksEntityCollectionPtr)m_pPart->EntityCollection(o3d_face);
	// 3. ВЕРХНЯЯ ГРАНЬ
	allFaces10->SelectByPoint(0, m_b / 2, -m_ShponkaH);
	for (int i = 0; i < allFaces10->GetCount(); i++)
	{
		ksEntityPtr face = allFaces10->GetByIndex(i);
		face->Putname("ShponkaTop");
		face->Update();
	}
}
