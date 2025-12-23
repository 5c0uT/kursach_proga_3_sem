#pragma once

#include "../pch.h"

// Используем типы из KOMPAS API
using Kompas6API5::KompasObjectPtr;
using Kompas6API5::ksDocument3DPtr;
using Kompas6API5::ksPartPtr;

class CMuftaBuilder
{
public:
    CMuftaBuilder(KompasObjectPtr pKompasApp);
    ~CMuftaBuilder();

    bool Build(double d, double dt2, double b, double b1, double d1,
        double D, double D1, double L, double l, double r,
        double c, double c1, double eps, const CString& savePath);

private:
    KompasObjectPtr m_pKompasApp;
    ksDocument3DPtr m_pDoc;
    ksPartPtr m_pPart;

    void CreateMainBody();
    void CreateKeyway();
    void CreateThreadedHole();
    void CreateRingGroove();
    void CreateChamfers();
    void CreateFillets();
    void SetFaceMarkers();

    // Параметры
    double m_d, m_dt2, m_b, m_b1, m_d1, m_D, m_D1, m_L, m_l, m_r, m_c, m_c1;
};
