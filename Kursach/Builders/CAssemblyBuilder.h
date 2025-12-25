#pragma once

#include "../pch.h"

class CAssemblyBuilder
{
public:
    CAssemblyBuilder(KompasObjectPtr pKompasApp);
    ~CAssemblyBuilder();

    bool Build(const CString& muftaPath, const CString& valPath,
        const CString& shponkaPath, const CString& vintPath,
        const CString& kolcoPath, double L, double l,
        double eps, const CString& savePath);

private:
    void AddParts();
    void CreateConstraints();

    KompasObjectPtr m_pKompasApp;
    ksDocument3DPtr m_pDoc;
    ksPartPtr       m_pPart;
    ksPartPtr       m_pMufta;
    ksPartPtr       m_pVal1;
    ksPartPtr       m_pVal2;
    ksPartPtr       m_pShponka1;
    ksPartPtr       m_pShponka2;
    ksPartPtr       m_pVint;
    ksPartPtr       m_pKolco;

    CString         muftaPath;
    CString         valPath;
    CString         shponkaPath;
    CString         vintPath;
    CString         kolcoPath;

    double          m_L;
    double          m_l;
    bool            m_bHasKolco;
};
