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

#endif //PCH_H
