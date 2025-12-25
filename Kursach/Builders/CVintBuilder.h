#pragma once

#include "../pch.h"

// Используем типы из KOMPAS API
using Kompas6API5::KompasObjectPtr;
using Kompas6API5::ksDocument3DPtr;
using Kompas6API5::ksPartPtr;

class CVintBuilder
{
public:
    CVintBuilder(KompasObjectPtr pKompasApp);
    ~CVintBuilder();

    bool Build(double d1, double p, double VintH, double VintW,
        double VintLenght, double eps, const CString& savePath);

private:
    KompasObjectPtr m_pKompasApp;
    ksDocument3DPtr m_pDoc;
    ksPartPtr       m_pPart;

    void CreateMainBody();
    void CreateThread();
    void CreateSlot();
    void CreateChamfers();
    void SetFaceMarkers();

    // Параметры
    double          m_d1, m_p, m_VintH, m_VintW, m_VintLenght;
};
