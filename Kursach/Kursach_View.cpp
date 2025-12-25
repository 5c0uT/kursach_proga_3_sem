#include "pch.h"

#define PI (4*atan(1.0))

#ifdef _DEBUG
#define new DEBUG_NEW
#endif



using namespace Kompas6API5;

KompasObjectPtr pKompasApp5;

// CKursachView

IMPLEMENT_DYNCREATE(CKursachView, CView)

BEGIN_MESSAGE_MAP(CKursachView, CView)
	ON_COMMAND(ID_FILE_PRINT,         &CView::OnFilePrint)
	ON_COMMAND(ID_FILE_PRINT_DIRECT,  &CView::OnFilePrint)
	ON_COMMAND(ID_FILE_PRINT_PREVIEW, &CView::OnFilePrintPreview)
	ON_BN_CLICKED(IDC_START,          &CKursachView::StartKompas)
END_MESSAGE_MAP()

// Создание или уничтожение CKursachView

CKursachView::CKursachView() noexcept
{
}

CKursachView::~CKursachView()
{
}

BOOL CKursachView::PreCreateWindow(CREATESTRUCT& cs)
{
	return CView::PreCreateWindow(cs);
}

// Рисование CKursachView

void CKursachView::OnDraw(CDC* pDC)
{
	CKursachDoc* pDoc = GetDocument();
	ASSERT_VALID(pDoc);
	if (!pDoc)
		return;
}

// Печать CKursachView

BOOL CKursachView::OnPreparePrinting(CPrintInfo* pInfo)
{
	return DoPreparePrinting(pInfo);
}

void CKursachView::OnBeginPrinting(CDC* pDC, CPrintInfo* pInfo)
{
}

void CKursachView::OnEndPrinting(CDC* pDC, CPrintInfo* pInfo)
{
}

// Диагностика CKursachView

#ifdef _DEBUG
void CKursachView::AssertValid() const
{
	CView::AssertValid();
}

void CKursachView::Dump(CDumpContext& dc) const
{
	CView::Dump(dc);
}

CKursachDoc* CKursachView::GetDocument() const
{
	ASSERT(m_pDocument->IsKindOf(RUNTIME_CLASS(CKursachDoc)));
	return (CKursachDoc*)m_pDocument;
}
#endif

// Обработчики сообщений CKursachView

void CKursachView::OnInitialUpdate()
{
	CView::OnInitialUpdate();

	Start.Create(_T("Построить"), WS_CHILD | WS_VISIBLE,
		         CRect(0, 0, 120, 30), this, IDC_START);
	Start.CenterWindow();
}
CString CKursachView::GetProjectRootPath()
{
    TCHAR szModulePath[MAX_PATH];
    GetModuleFileName(NULL, szModulePath, MAX_PATH);

    CString strPath = szModulePath;

    // Удаляем имя файла (Debug/Kursach.exe > Debug)
    int nLastSlash = strPath.ReverseFind(_T('\\'));
    if (nLastSlash != -1)
        strPath = strPath.Left(nLastSlash);

    // Удаляем Debug/Release папку (Debug > x64)
    nLastSlash = strPath.ReverseFind(_T('\\'));
    if (nLastSlash != -1)
        strPath = strPath.Left(nLastSlash);

    // Удаляем x64/Win32 папку (x64 > Kursach)
    nLastSlash = strPath.ReverseFind(_T('\\'));
    if (nLastSlash != -1)
        strPath = strPath.Left(nLastSlash);

    // Добавляем финальный слеш
    if (strPath[strPath.GetLength() - 1] != _T('\\'))
        strPath += _T("\\");

    return strPath;
}

// Проверка существования директории
BOOL CKursachView::DirectoryExists(const CString& dirPath)
{
    DWORD dwAttrib = GetFileAttributes(dirPath);
    return (dwAttrib != INVALID_FILE_ATTRIBUTES && (dwAttrib & FILE_ATTRIBUTE_DIRECTORY));
}

// Валидация и создание необходимых директорий
BOOL CKursachView::ValidateAndCreateDirectories(const CString& rootPath)
{
    // Список необходимых поддиректорий
    CStringArray requiredDirs;
    requiredDirs.Add(rootPath + _T("KompasAssembly"));
    requiredDirs.Add(rootPath + _T("Kursach"));
    requiredDirs.Add(rootPath + _T("nlohmann"));
    requiredDirs.Add(rootPath + _T("x64"));

    // Проверяем и создаём директории
    for (int i = 0; i < requiredDirs.GetSize(); i++)
    {
        CString dirPath = requiredDirs[i];

        if (!DirectoryExists(dirPath))
        {
            if (!CreateDirectory(dirPath, NULL))
            {
                DWORD dwError = GetLastError();
                if (dwError != ERROR_ALREADY_EXISTS)
                {
                    CString errMsg;
                    errMsg.Format(_T("Ошибка при создании директории:\n%s\nОшибка: %d"), dirPath, dwError);
                    AfxMessageBox(errMsg);
                    return FALSE;
                }
            }
        }
    }

    // Проверяем наличие файла .sln
    CString slnPath = rootPath + _T("Kursach.sln");
    if (!PathFileExists(slnPath))
    {
        CString warnMsg;
        warnMsg.Format(_T("Внимание: файл проекта не найден:\n%s"), slnPath);
        AfxMessageBox(warnMsg);
    }

    return TRUE;
}

void CKursachView::StartKompas()
{
	CKursachApp* pApp =      (CKursachApp*)AfxGetApp();
	CMainFrame* pMainFrame = (CMainFrame*)pApp->GetMainWnd();
	CKursachDoc* pDoc0 =     (CKursachDoc*)pMainFrame->GetActiveDocument();

    // Получаем корневую папку проекта
    CString rootPath = GetProjectRootPath();

    // Проверяем и создаём необходимые директории
    if (!ValidateAndCreateDirectories(rootPath))
    {
        AfxMessageBox(_T("Не удалось инициализировать рабочие директории!"));
        return;
    }

    BeginWaitCursor();

    // Получаем/создаём COM-объект Kompas.Application.5
    CComPtr<IUnknown> pKompasAppUnk = nullptr;
	if (!pKompasApp5)
	{
		CLSID InvAppClsid;
		HRESULT hRes = CLSIDFromProgID(L"Kompas.Application.5", &InvAppClsid);
		if (FAILED(hRes))
		{
			EndWaitCursor(); return;
		}

		hRes = ::GetActiveObject(InvAppClsid, NULL, &pKompasAppUnk);
		if (FAILED(hRes))
		{
			hRes = CoCreateInstance(InvAppClsid, NULL, CLSCTX_LOCAL_SERVER, __uuidof(IUnknown), (void**)&pKompasAppUnk);
			if (FAILED(hRes))
			{
				EndWaitCursor(); return;
			}
		}

		hRes = pKompasAppUnk->QueryInterface(__uuidof(KompasObject), (void**)&pKompasApp5);
		if (FAILED(hRes))
		{
			EndWaitCursor();
			return;
		}
	}

	pKompasApp5->Visible = true;
	CreateDetails();
}

void CKursachView::CreateDetails()
{
    CKursachApp* pApp =      (CKursachApp*)AfxGetApp();
    CMainFrame* pMainFrame = (CMainFrame*)pApp->GetMainWnd();
    CKursachDoc* pDoc0 =     (CKursachDoc*)pMainFrame->GetActiveDocument();

    if (pDoc0->Mufta_Check != false && pDoc0->Shponka_Check != false && pDoc0->Val_Check)
    {
        double d =          pDoc0->d;
        double dt2 =        pDoc0->dt2;
        double b =          pDoc0->b;
        double b1 =         pDoc0->b1;
        double d1 =         pDoc0->d1;
        double D =          pDoc0->D;
        double D1 =         pDoc0->D1;
        double L =          pDoc0->L;
        double l =          pDoc0->l;
        double r =          pDoc0->r;
        double c =          pDoc0->c;
        double c1 =         pDoc0->c1;
        double lV =         pDoc0->lV;
        double eps =        1.0;
        double ShponkaH =   pDoc0->ShponkaH;
        double ShponkaW =   pDoc0->ShponkaW;
        double ShponkaR =   0.2;
        double p =          0;
        double VintH =      0;
        double VintW =      0;
        double VintLenght = pDoc0->D / 2 - pDoc0->d / 2;

        if (pDoc0->d1 == 4)
        {
            p =     0.7;
            VintH = 0.6;
            VintW = 1.42;
        }
        else
        {
            p =     1.0;
            VintH = 1.0;
            VintW = 2.0;
        }

        if ((D != 0) && (ShponkaH != 0) && (lV != 0))
        {
            // Получаем корневой путь
            CString basePath = GetProjectRootPath() + _T("KompasAssembly\\");

            // 1. Создаем муфту
            CMuftaBuilder muftaBuilder(pKompasApp5);
            if (!muftaBuilder.Build(d, dt2, b, b1, d1, D, D1, L, l, r, c, c1, eps, basePath + _T("Mufta.m3d")))
            {
                AfxMessageBox(L"Ошибка при создании муфты!");
                return;
            }

            // 2. Создаем вал
            CValBuilder valBuilder(pKompasApp5);
            if (!valBuilder.Build(d, dt2, b, l, lV, ShponkaH, ShponkaW, c, eps, basePath + _T("Val.m3d")))
            {
                AfxMessageBox(L"Ошибка при создании вала!");
                return;
            }

            // 3. Создаем шпонку
            CShponkaBuilder shponkaBuilder(pKompasApp5);
            if (!shponkaBuilder.Build(b, ShponkaH, ShponkaW, ShponkaR, basePath + _T("Shponka.m3d")))
            {
                AfxMessageBox(L"Ошибка при создании шпонки!");
                return;
            }

            // 4. Создаем винт
            CVintBuilder vintBuilder(pKompasApp5);
            if (!vintBuilder.Build(d1, p, VintH, VintW, VintLenght, eps, basePath + _T("Vint.m3d")))
            {
                AfxMessageBox(L"Ошибка при создании винта!");
                return;
            }

            // 5. Создаем кольцо
            CString kolcoPath = _T("");
            if (b1 != 0)
            {
                CKolcoBuilder kolcoBuilder(pKompasApp5);
                if (!kolcoBuilder.Build(b1, D, D1, basePath + _T("Kolco.m3d")))
                {
                    AfxMessageBox(L"Ошибка при создании кольца!");
                    return;
                }
                kolcoPath = basePath + _T("Kolco.m3d");
            }

            // 6. Создаем сборку
            CAssemblyBuilder assemblyBuilder(pKompasApp5);
            if (!assemblyBuilder.Build(
                basePath + _T("Mufta.m3d"),
                basePath + _T("Val.m3d"),
                basePath + _T("Shponka.m3d"),
                basePath + _T("Vint.m3d"),
                kolcoPath,
                L,
                l,
                eps,
                basePath + _T("ItogProdgect.a3d")))
            {
                AfxMessageBox(L"Ошибка при создании сборки!");
                return;
            }

            MessageBox(L"Все детали и сборка успешно созданы!", MB_OK);
        }
        else
        {
            AfxMessageBox(L"Укажите все параметры сборки...");
        }
    }
    else
    {
        AfxMessageBox(L"Не все компоненты выбраны для построения!");
    }
}
