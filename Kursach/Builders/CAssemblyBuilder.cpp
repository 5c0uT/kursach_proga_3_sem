#include "pch.h"

CAssemblyBuilder::CAssemblyBuilder(KompasObjectPtr pKompasApp)
    : m_pKompasApp(pKompasApp), m_L(0), m_l(0), m_bHasKolco(false)
{
}

CAssemblyBuilder::~CAssemblyBuilder()
{
}

bool CAssemblyBuilder::Build(const CString& muftaPath, const CString& valPath,
    const CString& shponkaPath, const CString& vintPath,
    const CString& kolcoPath, double L, double l,
    double eps, const CString& savePath)
{
    try
    {
        // Сохраняем параметры
        this->muftaPath =   muftaPath;
        this->valPath =     valPath;
        this->shponkaPath = shponkaPath;
        this->vintPath =    vintPath;
        this->kolcoPath =   kolcoPath;
        m_L = L;
        m_l = l;
        m_bHasKolco = !kolcoPath.IsEmpty();

        // Создаем новый документ сборки
        m_pDoc = m_pKompasApp->Document3D();
        if (!m_pDoc)
            return false;

        m_pDoc->Create(false, false);
        m_pPart = m_pDoc->GetPart(pTop_Part);
        if (!m_pPart)
            return false;

        // Добавляем компоненты в сборку
        AddParts();

        // Создаем связи между деталями
        CreateConstraints();

        // Перестраиваем сборку
        m_pDoc->RebuildDocument();

        // Сохраняем сборку
        m_pDoc->SaveAs(_bstr_t(savePath));
        return true;
    }
    catch (...)
    {
        return false;
    }
}

void CAssemblyBuilder::AddParts()
{
    // Добавляем детали в сборку
    m_pDoc->SetPartFromFile(_bstr_t(muftaPath),     m_pPart, VARIANT_FALSE);
    m_pDoc->SetPartFromFile(_bstr_t(valPath),       m_pPart, VARIANT_FALSE);
    m_pDoc->SetPartFromFile(_bstr_t(valPath),       m_pPart, VARIANT_FALSE); // Второй вал
    m_pDoc->SetPartFromFile(_bstr_t(shponkaPath),   m_pPart, VARIANT_FALSE);
    m_pDoc->SetPartFromFile(_bstr_t(shponkaPath),   m_pPart, VARIANT_FALSE); // Вторая шпонка
    m_pDoc->SetPartFromFile(_bstr_t(vintPath),      m_pPart, VARIANT_FALSE);
    if (m_bHasKolco)
        m_pDoc->SetPartFromFile(_bstr_t(kolcoPath), m_pPart, VARIANT_FALSE);

    // Получаем добавленные части
    m_pMufta =     m_pDoc->GetPart(0); // Муфта
    m_pVal1 =      m_pDoc->GetPart(1); // Первый вал
    m_pVal2 =      m_pDoc->GetPart(2); // Второй вал
    m_pShponka1 =  m_pDoc->GetPart(3); // Первая шпонка
    m_pShponka2 =  m_pDoc->GetPart(4); // Вторая шпонка
    m_pVint =      m_pDoc->GetPart(5); // Винт
    if (m_bHasKolco)
        m_pKolco = m_pDoc->GetPart(6); // Кольцо
}

void CAssemblyBuilder::CreateConstraints()
{
    // Получаем грани деталей
    ksEntityCollectionPtr col = m_pMufta->EntityCollection(o3d_face);
    
    // Грани муфты
    ksEntityPtr MuftaNachalo =     col->GetByName(L"MuftaNachalo", true, true);
    ksEntityPtr MuftaCylinder =    col->GetByName(L"MuftaCilinder", true, true);
    ksEntityPtr MuftaVbot =        col->GetByName(L"MuftaVirezBottom", true, true);
    ksEntityPtr MufttaVbok =       col->GetByName(L"MuftaVirezBok", true, true);
    ksEntityPtr MuftaOtverstie =   col->GetByName(L"MuftaOtverstie", true, true);
    ksEntityPtr MuftaKolVirezBok = nullptr;
    ksEntityPtr MuftaKolVirezNiz = nullptr;
    if (m_bHasKolco)
    {
        MuftaKolVirezBok = col->GetByName(L"MuftaVirezKolcoBok", true, true);
        MuftaKolVirezNiz = col->GetByName(L"MuftaVirezKolcoNiz", true, true);
    }

    col = m_pVal1->EntityCollection(o3d_face);
    // Грани первого вала
    ksEntityPtr Val1Nach =     col->GetByName(L"NachaloVal", true, true);
    ksEntityPtr Val1Cylinder = col->GetByName(L"CilindrVal", true, true);
    ksEntityPtr Val1VirezBok = col->GetByName(L"BokSponkaVirezVal", true, true);
    ksEntityPtr Val1VirezBot = col->GetByName(L"SponkaVirezVal", true, true);

    col = m_pVal2->EntityCollection(o3d_face);
    // Грани второго вала
    ksEntityPtr Val2Nach =     col->GetByName(L"NachaloVal", true, true);
    ksEntityPtr Val2Cylinder = col->GetByName(L"CilindrVal", true, true);
    ksEntityPtr Val2VirezBok = col->GetByName(L"BokSponkaVirezVal", true, true);
    ksEntityPtr Val2VirezBot = col->GetByName(L"SponkaVirezVal", true, true);

    col = m_pShponka1->EntityCollection(o3d_face);
    // Грани первой шпонки
    ksEntityPtr Shponka1Bok =    col->GetByName(L"ShponkaBok", true, true);
    ksEntityPtr Shponka1Bottom = col->GetByName(L"ShponkaBottom", true, true);
    ksEntityPtr Shponka1Top =    col->GetByName(L"ShponkaTop", true, true);

    col = m_pShponka2->EntityCollection(o3d_face);
    // Грани второй шпонки
    ksEntityPtr Shponka2Bok =    col->GetByName(L"ShponkaBok", true, true);
    ksEntityPtr Shponka2Bottom = col->GetByName(L"ShponkaBottom", true, true);
    ksEntityPtr Shponka2Top =    col->GetByName(L"ShponkaTop", true, true);

    col = m_pVint->EntityCollection(o3d_face);
    // Грани Винта
    ksEntityPtr VintCilindr = col->GetByName(L"VintVnesh", true, true);
    ksEntityPtr VintKonchik = col->GetByName(L"VintKon", true, true);

    // Связи для кольца (если есть)
    if (m_bHasKolco)
    {
        col = m_pKolco->EntityCollection(o3d_face);
        // Грани Кольца
        ksEntityPtr KolcoRazre =    col->GetByName(L"KolcoRazrez", true, true);
        ksEntityPtr KolcoVnutr =    col->GetByName(L"KolcoInnerCylinder", true, true);
        ksEntityPtr KolcoLStorona = col->GetByName(L"KolcoLeftStorona", true, true);

        // Связи кольца
        m_pDoc->AddMateConstraint(mc_Coincidence, KolcoLStorona, MuftaKolVirezBok, -1, 1, 0);
        m_pDoc->AddMateConstraint(mc_Concentric,  KolcoVnutr,    MuftaKolVirezNiz, 1,  1, 0);
    }

    // Связи для валов
    m_pDoc->AddMateConstraint(mc_Concentric, MuftaCylinder, Val1Cylinder, 1,  1, 0);
    m_pDoc->AddMateConstraint(mc_Concentric, MuftaCylinder, Val2Cylinder, -1, 1, 0);
    m_pDoc->AddMateConstraint(mc_Distance,   Val1Nach,      Val2Nach,     -1, 1, 2);

    // Связи для шпоночных отверстий
    m_pDoc->AddMateConstraint(mc_Parallel, Val1VirezBok, Val2VirezBok, -1, 1, 0);
    m_pDoc->AddMateConstraint(mc_Parallel, MufttaVbok,   Val1VirezBok, -1, 1, 0);

    // Связи для шпонок
    m_pDoc->AddMateConstraint(mc_Coincidence, Shponka1Bottom, MuftaVbot,   -1,  1, 0);
    m_pDoc->AddMateConstraint(mc_Coincidence, Shponka2Bottom, MuftaVbot,   -1,  1, 0);
    m_pDoc->AddMateConstraint(mc_Coincidence, Shponka1Bok,    MufttaVbok,   -1, 1, 0);
    m_pDoc->AddMateConstraint(mc_Coincidence, Shponka2Bok,    MufttaVbok,   -1, 1, 0);
    m_pDoc->AddMateConstraint(mc_Concentric,  Shponka1Top,    Val1VirezBot, -1, 1, 0);
    m_pDoc->AddMateConstraint(mc_Concentric,  Shponka2Top,    Val2VirezBot, 1,  1, 0);

    // Связи для муфты
    m_pDoc->AddMateConstraint(mc_Distance, Val2Nach, MuftaNachalo, 1, 1, (m_L / 2) + 2);

    // Связи для винта
    m_pDoc->AddMateConstraint(mc_Concentric, MuftaOtverstie, VintCilindr, 1, 1, 0);
    m_pDoc->AddMateConstraint(5, VintKonchik, Val1Cylinder, 1, 1, 0);
}
