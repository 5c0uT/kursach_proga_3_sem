#pragma once

#include "../pch.h"

// Используем типы из KOMPAS API
using Kompas6API5::KompasObjectPtr;
using Kompas6API5::ksDocument3DPtr;
using Kompas6API5::ksPartPtr;

class CShponkaBuilder
{
public:
    CShponkaBuilder(KompasObjectPtr pKompasApp);
    ~CShponkaBuilder();

    bool Build(double b, double ShponkaH, double ShponkaW,
        double ShponkaR, const CString& savePath);

private:
    KompasObjectPtr m_pKompasApp;
    ksDocument3DPtr m_pDoc;
    ksPartPtr       m_pPart;

    void CreateMainBody();
    void CreateChamfers();
    void SetFaceMarkers();

    // Параметры
    double          m_b, m_ShponkaH, m_ShponkaW, m_ShponkaR;
};
