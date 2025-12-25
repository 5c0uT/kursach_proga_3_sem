#pragma once

#include "../pch.h"

// Используем типы из KOMPAS API
using Kompas6API5::KompasObjectPtr;
using Kompas6API5::ksDocument3DPtr;
using Kompas6API5::ksPartPtr;

class CValBuilder
{
public:
    CValBuilder(KompasObjectPtr pKompasApp);
    ~CValBuilder();

    bool Build(double d, double dt2, double b, double l, double lV,
        double ShponkaH, double ShponkaW, double c, double eps,
        const CString& savePath);

private:
    KompasObjectPtr m_pKompasApp;
    ksDocument3DPtr m_pDoc;
    ksPartPtr       m_pPart;

    void CreateMainBody();
    void CreateKeyway();
    void CreateChamfer();
    void SetFaceMarkers();

    // Параметры
    double          m_d, m_dt2, m_b, m_l, m_lV, m_ShponkaH, m_ShponkaW, m_c;
};
