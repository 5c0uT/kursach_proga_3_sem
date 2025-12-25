#include "pch.h"

CKolcoBuilder::CKolcoBuilder(KompasObjectPtr pKompasApp)
	: m_pKompasApp(pKompasApp)
{
}

CKolcoBuilder::~CKolcoBuilder()
{
}

bool CKolcoBuilder::Build(double b1, double D, double D1, const CString& savePath)
{
	try
	{
		// Сохраняем параметры
		m_b1 = b1; m_D = D; m_D1 = D1;

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

void CKolcoBuilder::CreateMainBody()
{
	// Создаем эскиз профиля кольца
	ksEntityPtr pSketchRing =              m_pPart->NewEntity(o3d_sketch);
	ksSketchDefinitionPtr pSketchRingDef = pSketchRing->GetDefinition();
	pSketchRingDef->SetPlane(m_pPart->GetDefaultEntity(o3d_planeXOY));
	pSketchRing->Create();
	ksDocument2DPtr p2DDocRing =           pSketchRingDef->BeginEdit();

	double rect[4][2];

	rect[0][0] = 0;
	rect[0][1] = (m_D1 / 2.0);

	rect[1][0] = m_b1;
	rect[1][1] = (m_D1 / 2.0);

	rect[2][0] = m_b1;
	rect[2][1] = (m_D1 / 2.0) + ((m_D - m_D1) / 2);

	rect[3][0] = 0;
	rect[3][1] = (m_D1 / 2.0) + ((m_D - m_D1) / 2);

	p2DDocRing->ksLineSeg(rect[0][0], rect[0][1], rect[1][0], rect[1][1], 1);
	p2DDocRing->ksLineSeg(rect[1][0], rect[1][1], rect[2][0], rect[2][1], 1);
	p2DDocRing->ksLineSeg(rect[2][0], rect[2][1], rect[3][0], rect[3][1], 1);
	p2DDocRing->ksLineSeg(rect[3][0], rect[3][1], rect[0][0], rect[0][1], 1);

	p2DDocRing->ksLineSeg(-10, 0, (m_D1 / 2.0) + (m_D1 / 2.0) + 10, 0, 3);

	pSketchRingDef->EndEdit();

	// Вращение для создания кольца
	ksEntityPtr pRotateRing = m_pPart->NewEntity(o3d_bossRotated);
	ksBossRotatedDefinitionPtr pRotDefRing = pRotateRing->GetDefinition();
	pRotDefRing->SetSketch(pSketchRing);
	pRotDefRing->SetSideParam(TRUE, 340);  // угол 340 градусов
	pRotateRing->Create();
}

void CKolcoBuilder::CreateFillets()
{
	ksEntityCollectionPtr allEdgesRing = m_pPart->EntityCollection(o3d_edge);

	ksEntityPtr pFilletRing = m_pPart->NewEntity(o3d_fillet);
	ksFilletDefinitionPtr pFilletRingDef = pFilletRing->GetDefinition();
	pFilletRingDef->radius = 0.25;

	ksEntityCollectionPtr filletEdges = pFilletRingDef->array();
	filletEdges->Clear();

	for (int i = 0; i < allEdgesRing->GetCount(); i++)
	{
		ksEntityPtr edge = allEdgesRing->GetByIndex(i);
		ksEdgeDefinitionPtr edgeDef = edge->GetDefinition();

		if (edgeDef->GetOwnerEntity() == m_pPart->EntityCollection(o3d_edge))
		{
			filletEdges->Add(edge);
		}
	}

	if (filletEdges->GetCount() > 0)
		pFilletRing->Create();
}

void CKolcoBuilder::SetFaceMarkers()
{
	ksEntityCollectionPtr allFacesRing = (ksEntityCollectionPtr)m_pPart->EntityCollection(o3d_face);
	// 1. ПЛОСКОСТЬ РАЗРЕЗА
	allFacesRing->SelectByPoint(m_b1 / 2, m_D1 / 2 + 1, 0);
	for (int i = 0; i < allFacesRing->GetCount(); i++)
	{
		ksEntityPtr face = allFacesRing->GetByIndex(i);
		face->Putname("KolcoRazrez");
		face->Update();
	}

	ksEntityCollectionPtr allFacesRing1 = (ksEntityCollectionPtr)m_pPart->EntityCollection(o3d_face);
	// 2. ВНУТРЕННЯЯ ЦИЛИНДРИЧЕСКАЯ ПОВЕРХНОСТЬ
	allFacesRing1->SelectByPoint(m_b1 / 2, 0, m_D1 / 2);
	for (int i = 0; i < allFacesRing1->GetCount(); i++)
	{
		ksEntityPtr face = allFacesRing1->GetByIndex(i);
		face->Putname("KolcoInnerCylinder");
		face->Update();
	}

	ksEntityCollectionPtr allFacesRing2 = (ksEntityCollectionPtr)m_pPart->EntityCollection(o3d_face);
	// 3. ЛЕВАЯ БОКОВАЯ ПОВЕРХНОСТЬ
	allFacesRing2->SelectByPoint(m_b1, 0, m_D1 / 2 + m_b1 / 2);
	for (int i = 0; i < allFacesRing2->GetCount(); i++)
	{
		ksEntityPtr face = allFacesRing2->GetByIndex(i);
		face->Putname("KolcoLeftStorona");
		face->Update();
	}
}
