#ifndef PCH_H
#define PCH_H

// Добавляем сюда заголовочные файлы для предварительной компиляции
#include "framework.h"
#include "targetver.h"
#include <afxwin.h>
#include <afxext.h>
#include <afxdisp.h>
#include <afxdialogex.h>
#include <afxcview.h>
#include <afxcmn.h>
#include <afxdtctl.h>
#include <afxcontrolbars.h>
#include <afxrich.h>
#include <string>
#include <cmath>
#include <iostream>
#include <memory>
#include <vector>
#include <map>
#include <algorithm>
#include <functional>
#include <atlbase.h>
#include <atlcom.h>
#include <atlstr.h>
#include <atlpath.h>
#include <atlcoll.h>
#include <atlsafe.h>
#include <comutil.h>
#include <comdef.h>
#include <fstream>
#include <sstream>
#include "nlohmann/json.hpp"

// KOMPAS SDK includes
#include "C:\Program Files\ASCON\KOMPAS-3D v23 Study\SDK\Include\ksConstants.h"
#include "C:\Program Files\ASCON\KOMPAS-3D v23 Study\SDK\Include\ksConstants3D.h"
#import "C:\Program Files\ASCON\KOMPAS-3D v23 Study\SDK\lib\kAPI5.tlb" \
    rename("GetObject", "KompasGetObject") \
    rename("min",       "KompasMin") \
    rename("max",       "KompasMax")

// Проектные заголовки
#include "resource.h"
#include "Kursach.h"
#include "Kursach_Doc.h"
#include "Kursach_View.h"
#include "CMyFreeView.h"
#include "MainFrm.h"
#include "CObjectMufta.h"
#include "CObjectVal.h"
#include "CObjectShponka.h"
#include "mufta_data.h"
#include "Config.h"

// Билдеры деталей
#include "Builders/CMuftaBuilder.h"
#include "Builders/CValBuilder.h"
#include "Builders/CShponkaBuilder.h"
#include "Builders/CVintBuilder.h"
#include "Builders/CKolcoBuilder.h"
#include "Builders/CAssemblyBuilder.h"

#include <shlwapi.h>
#pragma comment(lib, "shlwapi.lib")

using Kompas6API5::ksEntityPtr;
using Kompas6API5::ksSketchDefinitionPtr;
using Kompas6API5::ksDocument2DPtr;
using Kompas6API5::ksBossExtrusionDefinitionPtr;
using Kompas6API5::ksThreadDefinitionPtr;
using Kompas6API5::ksEntityCollectionPtr;
using Kompas6API5::ksFaceDefinitionPtr;
using Kompas6API5::ksEdgeDefinitionPtr;
using Kompas6API5::ksVertexDefinitionPtr;
using Kompas6API5::ksPlaneOffsetDefinitionPtr;
using Kompas6API5::ksChamferDefinitionPtr;
using Kompas6API5::ksCutExtrusionDefinitionPtr;
using Kompas6API5::ksBossRotatedDefinitionPtr;
using Kompas6API5::ksFilletDefinitionPtr;
using Kompas6API5::ksCutRotatedDefinitionPtr;

#endif //PCH_H
