#include "pch.h"

CVintBuilder::CVintBuilder(KompasObjectPtr pKompasApp)
	: m_pKompasApp(pKompasApp)
	, m_d1(0.0), m_p(0.0), m_VintH(0.0), m_VintW(0.0), m_VintLenght(0.0)
{
}

CVintBuilder::~CVintBuilder()
{
}

bool CVintBuilder::Build(double d1, double p, double VintH, double VintW,
	double VintLenght, double eps, const CString& savePath)
{
	try
	{
		// Сохраняем параметры
		m_d1 = d1; m_p = p; m_VintH = VintH; m_VintW = VintW; m_VintLenght = VintLenght;

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
		CreateThread();
		CreateSlot();
		CreateChamfers();
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

void CVintBuilder::CreateMainBody()
{
	// Эскиз для стержня винта
	ksEntityPtr pRodSketch =                      m_pPart->NewEntity(o3d_sketch);
	ksSketchDefinitionPtr pRodSketchDef = pRodSketch->GetDefinition();
	pRodSketchDef->SetPlane(m_pPart->GetDefaultEntity(o3d_planeXOY));
	pRodSketch->Create();
	ksDocument2DPtr p2DRod =                      pRodSketchDef->BeginEdit();

	// Создаем круг для стержня
	p2DRod->ksCircle(0, 0, m_d1 / 2, 1);

	pRodSketchDef->EndEdit();

	// Выдавливаем стержень
	ksEntityPtr pRodExtrude =                     m_pPart->NewEntity(o3d_bossExtrusion);
	ksBossExtrusionDefinitionPtr pRodExtrudeDef = pRodExtrude->GetDefinition();
	pRodExtrudeDef->SetSketch(pRodSketch);
	pRodExtrudeDef->SetSideParam(TRUE, etBlind, m_VintLenght, 0, FALSE);
	pRodExtrude->Create();

	ksEntityCollectionPtr allEdges =              m_pPart->EntityCollection(o3d_edge);

	// ПЕРВАЯ ФАСКА 0.3 мм (один конец)
	ksEntityPtr pChamfer1 =                       m_pPart->NewEntity(o3d_chamfer);
	ksChamferDefinitionPtr pChamferDef1 =         pChamfer1->GetDefinition();
	pChamferDef1->SetChamferParam(true, 0.3, 0.3);

	ksEntityCollectionPtr chamferEdges1 =         pChamferDef1->array();
	chamferEdges1->Clear();

	// ВТОРАЯ ФАСКА 1.75 мм (противоположный конец)
	ksEntityPtr pChamfer2 =                       m_pPart->NewEntity(o3d_chamfer);
	ksChamferDefinitionPtr pChamferDef2 =         pChamfer2->GetDefinition();
	pChamferDef2->SetChamferParam(true, 1.75, 1.75);

	ksEntityCollectionPtr chamferEdges2 =         pChamferDef2->array();
	chamferEdges2->Clear();

	// Ищем все ребра стержня и распределяем по концам
	for (int i = 0; i < allEdges->GetCount(); i++) {
		ksEntityPtr edge =                        allEdges->GetByIndex(i);
		ksEdgeDefinitionPtr edgeDef =             edge->GetDefinition();

		if (edgeDef->GetOwnerEntity() == pRodExtrude && edgeDef->IsCircle())
		{
			ksVertexDefinitionPtr v1 =            edgeDef->GetVertex(true);
			double x, y, z;
			v1->GetPoint(&x, &y, &z);

			// Определяем к какому концу принадлежит ребро
			if (abs(z) < 0.001)
			{
				// Ребро на начале стержня (z = 0)
				chamferEdges1->Add(edge);
			}
			else if (abs(z - m_VintLenght) < 0.001)
			{
				// Ребро на конце стержня (z = rodLength)
				chamferEdges2->Add(edge);
			}
		}
	}

	// Создаем фаски
	if (chamferEdges1->GetCount() > 0)
		pChamfer1->Create();

	if (chamferEdges2->GetCount() > 0)
		pChamfer2->Create();
}

void CVintBuilder::CreateThread()
{
	ksEntityPtr pThread =              m_pPart->NewEntity(o3d_thread);
	ksThreadDefinitionPtr pThreadDef = pThread->GetDefinition();

	// Настройка параметров резьбы
	pThreadDef->allLength =   true;         // признак полной длины
	pThreadDef->autoDefinDr = true;         // признак автоопределения диаметра
	pThreadDef->dr =          m_d1;         // номинальный диаметр резьбы
	pThreadDef->length =      m_VintLenght; // длина резьбы
	pThreadDef->faceValue =   false;        // направление построения резьбы (false - наружная резьба)
	pThreadDef->p =           m_p;          // шаг резьбы

	// Ищем цилиндрическую грань стержня для нанесения резьбы
	ksEntityCollectionPtr faceCollection = m_pPart->EntityCollection(o3d_face);
	ksEntityPtr threadFace =               nullptr;

	for (int i = 0; i < faceCollection->GetCount(); i++)
	{
		ksEntityPtr face =            faceCollection->GetByIndex(i);
		ksFaceDefinitionPtr faceDef = face->GetDefinition();

		// Проверяем, что грань принадлежит нашему выдавливанию стержня и является цилиндрической
		if (faceDef->IsCylinder())
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
}

void CVintBuilder::CreateSlot()
{
	// Смещаем плоскость для создания шлица
	ksEntityPtr pPlaneOffset =                   m_pPart->NewEntity(o3d_planeOffset);
	ksPlaneOffsetDefinitionPtr pPlaneOffsetDef = pPlaneOffset->GetDefinition();
	pPlaneOffsetDef->direction =                 false;
	pPlaneOffsetDef->offset =                    m_d1 / 2;
	pPlaneOffsetDef->SetPlane(m_pPart->GetDefaultEntity(o3d_planeXOZ));
	pPlaneOffset->Create();

	// Эскиз для шлица
	ksEntityPtr pSlotSketch =                    m_pPart->NewEntity(o3d_sketch);
	ksSketchDefinitionPtr pSlotSketchDef =       pSlotSketch->GetDefinition();
	pSlotSketchDef->SetPlane(pPlaneOffset);
	pSlotSketch->Create();
	ksDocument2DPtr p2DSlot =                    pSlotSketchDef->BeginEdit();

	// Создаем прямоугольник для шлица
	double slotHalfWidth = m_VintH / 2;
	p2DSlot->ksLineSeg(-slotHalfWidth, 0,        slotHalfWidth,  0,        1);
	p2DSlot->ksLineSeg(slotHalfWidth,  0,        slotHalfWidth,  -m_VintW, 1);
	p2DSlot->ksLineSeg(slotHalfWidth,  -m_VintW, -slotHalfWidth, -m_VintW, 1);
	p2DSlot->ksLineSeg(-slotHalfWidth, -m_VintW, -slotHalfWidth, 0,        1);

	pSlotSketchDef->EndEdit();

	// Вырезаем шлиц
	ksEntityPtr pSlotCut =                    m_pPart->NewEntity(o3d_cutExtrusion);
	ksCutExtrusionDefinitionPtr pSlotCutDef = pSlotCut->GetDefinition();
	pSlotCutDef->SetSketch(pSlotSketch);
	pSlotCutDef->SetSideParam(FALSE, etBlind, m_d1, 0, FALSE);
	pSlotCut->Create();
}

void CVintBuilder::CreateChamfers()
{
	
}

void CVintBuilder::SetFaceMarkers()
{
	ksEntityCollectionPtr allFaces11 = (ksEntityCollectionPtr)m_pPart->EntityCollection(o3d_face);
	// 1. НИЖНЯЯ ГРАНЬ (ТОРЦЕВАЯ)
	allFaces11->SelectByPoint(0, 0, m_VintLenght);
	for (int i = 0; i < allFaces11->GetCount(); i++)
	{
		ksEntityPtr face = allFaces11->GetByIndex(i);
		face->Putname("VintKon");
		face->Update();
	}

	ksEntityCollectionPtr allFaces12 = (ksEntityCollectionPtr)m_pPart->EntityCollection(o3d_face);
	// 2. БОКОВАЯ ГРАНЬ (ЦИЛИНДР)
	allFaces12->SelectByPoint(m_d1 / 2, 0, m_VintLenght / 2);
	for (int i = 0; i < allFaces12->GetCount(); i++)
	{
		ksEntityPtr face = allFaces12->GetByIndex(i);
		face->Putname("VintVnesh");
		face->Update();
	}
}
