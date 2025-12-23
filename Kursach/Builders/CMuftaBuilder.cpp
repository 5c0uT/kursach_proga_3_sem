#include "pch.h"

CMuftaBuilder::CMuftaBuilder(KompasObjectPtr pKompasApp)
	: m_pKompasApp(pKompasApp)
{
}

CMuftaBuilder::~CMuftaBuilder()
{
}

bool CMuftaBuilder::Build(double d, double dt2, double b, double b1, double d1,
	double D, double D1, double L, double l, double r,
	double c, double c1, double eps, const CString& savePath)
{
	try
	{
		// Сохраняем параметры
		m_d = d; m_dt2 = dt2; m_b = b; m_b1 = b1; m_d1 = d1;
		m_D = D; m_D1 =  D1;  m_L = L; m_l =  l;  m_r =  r;
		m_c = c; m_c1 =  c1;

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
		CreateKeyway();
		CreateThreadedHole();

		if (b1 != 0)
			CreateRingGroove();

		CreateChamfers();
		CreateFillets();
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

void CMuftaBuilder::CreateMainBody()
{
	// Создаем эскиз профиля муфты
	ksEntityPtr pSketch = m_pPart->NewEntity(o3d_sketch);
	ksSketchDefinitionPtr pSketchDef = pSketch->GetDefinition();
	pSketchDef->SetPlane(m_pPart->GetDefaultEntity(o3d_planeXOY));
	pSketch->Create();
	ksDocument2DPtr p2DDoc = pSketchDef->BeginEdit();

	// Точки профиля муфты
	double point[4][2];

	// Внешний контур
	point[0][0] = 0;
	point[0][1] = m_D / 2.0;

	point[1][0] = m_L;
	point[1][1] = m_D / 2.0;

	// Внутренний контур
	point[2][0] = m_L;
	point[2][1] = m_d / 2.0;

	point[3][0] = 0;
	point[3][1] = m_d / 2.0;

	//внешний контур
	p2DDoc->ksLineSeg(point[0][0], point[0][1], point[1][0], point[1][1], 1);
	p2DDoc->ksLineSeg(point[1][0], point[1][1], point[2][0], point[2][1], 1);
	p2DDoc->ksLineSeg(point[2][0], point[2][1], point[3][0], point[3][1], 1);
	p2DDoc->ksLineSeg(point[3][0], point[3][1], point[0][0], point[0][1], 1);

	// Ось вращения
	p2DDoc->ksLineSeg(-10, 0, 10, 0, 3);

	pSketchDef->EndEdit();

	// Вращение для создания вал
	ksEntityPtr pRotate = m_pPart->NewEntity(o3d_bossRotated);
	ksBossRotatedDefinitionPtr pRotDef = pRotate->GetDefinition();
	pRotDef->SetSketch(pSketch);
	pRotDef->SetSideParam(TRUE, 360);
	pRotate->Create();
}

void CMuftaBuilder::CreateKeyway()
{
	// Создаем эскиз для шпоночного паза на плоскости XOY
	ksEntityPtr pSketchKeyway = m_pPart->NewEntity(o3d_sketch);
	ksSketchDefinitionPtr pSketchKeywayDef = pSketchKeyway->GetDefinition();
	pSketchKeywayDef->SetPlane(m_pPart->GetDefaultEntity(o3d_planeXOY));
	pSketchKeyway->Create();
	ksDocument2DPtr p2DDocKeyway = pSketchKeywayDef->BeginEdit();

	// Координаты прямоугольника по всей длине вала
	double leftX = -m_b / 2.0;              // левая граница
	double rightX = m_b / 2.0;               // правая граница
	double startY = 0;                       // начинаем от центра
	double endY = m_L;                       // до конца трубы

	// Создание прямоугольника вдоль вала
	p2DDocKeyway->ksLineSeg(startY, leftX,  endY,   leftX,  1);    // левая грань
	p2DDocKeyway->ksLineSeg(endY,   leftX,  endY,   rightX, 1);    // торцевая грань
	p2DDocKeyway->ksLineSeg(endY,   rightX, startY, rightX, 1);    // правая грань
	p2DDocKeyway->ksLineSeg(startY, rightX, startY, leftX,  1);    // начальная грань

	pSketchKeywayDef->EndEdit();

	// Выдавливание шпоночного паза
	ksEntityPtr pExtrudeKeyway = m_pPart->NewEntity(o3d_cutExtrusion);
	ksCutExtrusionDefinitionPtr pExtrudeKeywayDef = pExtrudeKeyway->GetDefinition();
	pExtrudeKeywayDef->SetSketch(pSketchKeyway);
	pExtrudeKeywayDef->SetSideParam(FALSE, etBlind, m_dt2 - (m_d / 2), 0, FALSE);
	pExtrudeKeyway->Create();
}

void CMuftaBuilder::CreateThreadedHole()
{
	// Смещение плоскости
	ksEntityPtr pPlane1 = m_pPart->NewEntity(o3d_planeOffset);
	ksPlaneOffsetDefinitionPtr pPlaneDef1 = pPlane1->GetDefinition();

	pPlaneDef1->direction = false;
	pPlaneDef1->offset = m_D / 2;
	pPlaneDef1->SetPlane(m_pPart->GetDefaultEntity(o3d_planeXOY));
	pPlane1->Create();

	// Создание отверстия с резьбой
	ksEntityPtr pSketchThreadedHole = m_pPart->NewEntity(o3d_sketch);
	ksSketchDefinitionPtr pSketchDefThreadedHole = pSketchThreadedHole->GetDefinition();
	pSketchDefThreadedHole->SetPlane(pPlane1); // Используем вторую плоскость
	pSketchThreadedHole->Create();
	ksDocument2DPtr p2DDocThreadedHole = pSketchDefThreadedHole->BeginEdit();

	p2DDocThreadedHole->ksCircle(m_l, 0, (m_d1 - 1) / 2.0, 1);// вычитание 1 т.к отверстие м6 имеет радиус 5

	pSketchDefThreadedHole->EndEdit();

	ksEntityPtr pExtrudeThreadedHole = m_pPart->NewEntity(o3d_cutExtrusion);
	ksCutExtrusionDefinitionPtr pExtrudeDefThreadedHole = pExtrudeThreadedHole->GetDefinition();
	pExtrudeDefThreadedHole->SetSketch(pSketchThreadedHole);
	pExtrudeDefThreadedHole->SetSideParam(FALSE, etBlind, m_D / 2, 0, FALSE); //указываем проверка выреза до центра
	pExtrudeThreadedHole->Create();

	// Создание резьбы на отверстии
	ksEntityPtr Thread1 = m_pPart->NewEntity(o3d_thread);
	ksThreadDefinitionPtr ThreadDef1 = Thread1->GetDefinition();

	// Настройка параметров резьбы
	double p = (m_d1 == 4) ? 0.7 : 1.0;   // шаг резьбы
	ThreadDef1->allLength =   true;       // признак полной длины
	ThreadDef1->autoDefinDr = true;       // признак автоопределения диаметра
	ThreadDef1->dr =          m_d1;       // номинальный диаметр резьбы
	ThreadDef1->length =      m_D / 2;    // длина резьбы
	ThreadDef1->faceValue =   true;       // направление построения резьбы (true - внутренняя)
	ThreadDef1->p =           p;          // шаг резьбы

	ksEntityCollectionPtr faceCollection = m_pPart->EntityCollection(o3d_face);
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
}

void CMuftaBuilder::CreateRingGroove()
{
	// Создаем эскиз для выреза под кольцо
	ksEntityPtr pSketchRectCut = m_pPart->NewEntity(o3d_sketch);
	ksSketchDefinitionPtr pSketchRectCutDef = pSketchRectCut->GetDefinition();
	pSketchRectCutDef->SetPlane(m_pPart->GetDefaultEntity(o3d_planeXOY));
	pSketchRectCut->Create();
	ksDocument2DPtr p2DDocRectCut = pSketchRectCutDef->BeginEdit();

	double rectPoint[4][2];

	rectPoint[0][0] = m_l - (m_b1 / 2);
	rectPoint[0][1] = m_D / 2.0;

	rectPoint[1][0] = m_l + (m_b1 / 2);
	rectPoint[1][1] = m_D / 2.0;

	rectPoint[2][0] = m_l + (m_b1 / 2);
	rectPoint[2][1] = m_D1 / 2.0;

	rectPoint[3][0] = m_l - (m_b1 / 2);
	rectPoint[3][1] = m_D1 / 2.0;

	p2DDocRectCut->ksLineSeg(rectPoint[0][0], rectPoint[0][1], rectPoint[1][0], rectPoint[1][1], 1);
	p2DDocRectCut->ksLineSeg(rectPoint[1][0], rectPoint[1][1], rectPoint[2][0], rectPoint[2][1], 1);
	p2DDocRectCut->ksLineSeg(rectPoint[2][0], rectPoint[2][1], rectPoint[3][0], rectPoint[3][1], 1);
	p2DDocRectCut->ksLineSeg(rectPoint[3][0], rectPoint[3][1], rectPoint[0][0], rectPoint[0][1], 1);

	p2DDocRectCut->ksLineSeg(-10, 0, m_L + 10, 0, 3);

	pSketchRectCutDef->EndEdit();

	ksEntityPtr pRotateCut = m_pPart->NewEntity(o3d_cutRotated);
	ksCutRotatedDefinitionPtr pRotateCutDef = pRotateCut->GetDefinition();
	pRotateCutDef->SetSketch(pSketchRectCut);
	pRotateCutDef->SetSideParam(TRUE, 360);
	pRotateCut->Create();
}

void CMuftaBuilder::CreateChamfers()
{
	// Получаем коллекцию всех ребер детали
	ksEntityCollectionPtr allEdges = m_pPart->EntityCollection(o3d_edge);

	// Создаем фаску на всех круговых ребрах
	ksEntityPtr pChamfer = m_pPart->NewEntity(o3d_chamfer);
	ksChamferDefinitionPtr pChamferDef = pChamfer->GetDefinition();
	pChamferDef->SetChamferParam(true, m_c, m_c);

	ksEntityCollectionPtr chamferEdges = pChamferDef->array();
	chamferEdges->Clear();

	// Перебираем все ребра
	for (int i = 0; i < allEdges->GetCount(); i++)
	{
		ksEntityPtr edge = allEdges->GetByIndex(i);
		ksEdgeDefinitionPtr edgeDef = edge->GetDefinition();

		// Проверяем, является ли ребро круговым
		if (edgeDef->IsCircle())
		{
			chamferEdges->Add(edge);
		}
	}

	if (chamferEdges->GetCount() > 0)
		pChamfer->Create();
}

void CMuftaBuilder::CreateFillets()
{
	// Получаем обновленную коллекцию ребер после создания шпоночного паза
	ksEntityCollectionPtr updatedEdges = m_pPart->EntityCollection(o3d_edge);
	ksEntityPtr pFillet = m_pPart->NewEntity(o3d_fillet);
	ksFilletDefinitionPtr pFilletDef = pFillet->GetDefinition();
	pFilletDef->radius = m_r;
	ksEntityCollectionPtr fl = pFilletDef->array();
	fl->Clear();

	for (int i = 0; i < updatedEdges->GetCount(); i++)
	{
		ksEntityPtr ed = updatedEdges->GetByIndex(i);
		ksEdgeDefinitionPtr def = ed->GetDefinition();

		// Проверяем, что ребро принадлежит операции выдавливания шпоночного паза
		if (def->GetOwnerEntity() == m_pPart->EntityCollection(o3d_edge))
		{
			ksVertexDefinitionPtr p1 = def->GetVertex(true);
			ksVertexDefinitionPtr p2 = def->GetVertex(false);
			double x1, y1, z1, x2, y2, z2;
			p1->GetPoint(&x1, &y1, &z1);
			p2->GetPoint(&x2, &y2, &z2);

			if (x1 == 0 && x2 == m_L && z1 == m_dt2 - (m_d / 2) && y1 == y2)
			{
				fl->Add(ed);
			}
			else if (x2 == 0 && x1 == m_L && z2 == m_dt2 - (m_d / 2) && y1 == y2)
			{
				fl->Add(ed);
			}
		}
	}

	if (fl->GetCount() > 0) {
		pFillet->Create();
	}
}

void CMuftaBuilder::SetFaceMarkers()
{
	ksEntityCollectionPtr allFaces00 = (ksEntityCollectionPtr)m_pPart->EntityCollection(o3d_face);
	// 0. НАЧАЛЬНАЯ ГРАНЬ
	allFaces00->SelectByPoint(0, m_D / 2 - m_c, 0);
	for (int i = 0; i < allFaces00->GetCount(); i++)
	{
		ksEntityPtr face = allFaces00->GetByIndex(i);
		face->Putname("MuftaNachalo");
		face->Update();
	}

	ksEntityCollectionPtr allFaces0 = (ksEntityCollectionPtr)m_pPart->EntityCollection(o3d_face);
	// 1. ВЕРХНЯЯ ГРАНЬ
	allFaces0->SelectByPoint(m_L / 2, 0, (m_d / 2) * (-1));
	for (int i = 0; i < allFaces0->GetCount(); i++)
	{
		ksEntityPtr face = allFaces0->GetByIndex(i);
		face->Putname("MuftaCilinder");
		face->Update();
	}

	ksEntityCollectionPtr allFaces1 = (ksEntityCollectionPtr)m_pPart->EntityCollection(o3d_face);
	// 2. НИЖНЯЯ ГРАНЬ ВЫРЕЗА
	allFaces1->SelectByPoint(m_L / 2, 0, m_dt2 - (m_d / 2));
	for (int i = 0; i < allFaces1->GetCount(); i++)
	{
		ksEntityPtr face = allFaces1->GetByIndex(i);
		face->Putname("MuftaVirezBottom");
		face->Update();
	}

	ksEntityCollectionPtr allFaces2 = (ksEntityCollectionPtr)m_pPart->EntityCollection(o3d_face);
	// 3. БОКОВАЯ ГРАНЬ ВЫРЕЗА
	allFaces2->SelectByPoint(m_L / 2, m_b / 2, m_dt2 - (m_d / 2) - m_r);
	for (int i = 0; i < allFaces2->GetCount(); i++)
	{
		ksEntityPtr face = allFaces2->GetByIndex(i);
		face->Putname("MuftaVirezBok");
		face->Update();
	}

	ksEntityCollectionPtr allFaces3 = (ksEntityCollectionPtr)m_pPart->EntityCollection(o3d_face);
	// 4. ГРАНЬ ОТВЕРСТИЯ
	allFaces3->SelectByPoint(m_l + ((m_d1 - 1) / 2), 0, (m_D / 2 - m_c1) * (-1));
	for (int i = 0; i < allFaces3->GetCount(); i++)
	{
		ksEntityPtr face = allFaces3->GetByIndex(i);
		face->Putname("MuftaOtverstie");
		face->Update();
	}

	if (m_b1 != 0)
	{
		// 5. ГРАНЬ ВЫРЕЗА ПОД КОЛЬЦО (НИЖНЯЯ)
		ksEntityCollectionPtr allFaces87 = (ksEntityCollectionPtr)m_pPart->EntityCollection(o3d_face);
		allFaces87->SelectByPoint(m_l, 0, m_D1 / 2);
		for (int i = 0; i < allFaces87->GetCount(); i++)
		{
			ksEntityPtr face = allFaces87->GetByIndex(i);
			face->Putname("MuftaVirezKolcoNiz");
			face->Update();
		}

		// 6. ГРАНЬ ВЫРЕЗА ПОД КОЛЬЦО (БОКОВАЯ)
		ksEntityCollectionPtr allFaces88 = (ksEntityCollectionPtr)m_pPart->EntityCollection(o3d_face);
		allFaces88->SelectByPoint(m_l + (m_b1 / 2), 0, (m_D1 / 2) + (m_D - m_D1) / 2 - 1);
		for (int i = 0; i < allFaces88->GetCount(); i++)
		{
			ksEntityPtr face = allFaces88->GetByIndex(i);
			face->Putname("MuftaVirezKolcoBok");
			face->Update();
		}
	}
}
