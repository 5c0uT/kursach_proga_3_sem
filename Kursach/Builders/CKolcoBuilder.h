#pragma once

#include "../pch.h"

// Используем типы из KOMPAS API
using Kompas6API5::KompasObjectPtr;
using Kompas6API5::ksDocument3DPtr;
using Kompas6API5::ksPartPtr;

class CKolcoBuilder
{
public:
    CKolcoBuilder(KompasObjectPtr pKompasApp);
    ~CKolcoBuilder();

    bool Build(double b1, double D, double D1, const CString& savePath);

private:
    KompasObjectPtr m_pKompasApp;
    ksDocument3DPtr m_pDoc;
    ksPartPtr m_pPart;

    void CreateMainBody();
    void CreateFillets();
    void SetFaceMarkers();

    // Параметры
    double m_b1, m_D, m_D1;
};
