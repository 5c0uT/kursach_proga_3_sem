#include "pch.h"

using json = nlohmann::json;

// Инициализация глобальных переменных
MuftaVarTable  muftaVarTable = { nullptr, 0, 0 };
MuftaNameArray name_d =        { nullptr, 0 };
MuftaNameArray name_dt2 =      { nullptr, 0 };
MuftaNameArray name_b =        { nullptr, 0 };
MuftaNameArray name_b1 =       { nullptr, 0 };
MuftaNameArray name_d1 =       { nullptr, 0 };
MuftaNameArray name_D =        { nullptr, 0 };
MuftaNameArray name_D1 =       { nullptr, 0 };
MuftaNameArray name_L =        { nullptr, 0 };
MuftaNameArray name_l =        { nullptr, 0 };
MuftaNameArray name_r =        { nullptr, 0 };
MuftaNameArray name_c =        { nullptr, 0 };
MuftaNameArray name_c1 =       { nullptr, 0 };

// Вспомогательная функция для освобождения массива строк
void FreeNameArray(MuftaNameArray& arr) 
{
    if (arr.data) 
    {
        for (size_t i = 0; i < arr.count; i++) 
        {
            free((void*)arr.data[i]);
        }
        free(arr.data);
        arr.data = nullptr;
        arr.count = 0;
    }
}

// Функция для загрузки данных из JSON файла
int LoadMuftaDataFromJson(const char* filename) 
{
    try {
        // Открываем файл
        std::ifstream file(filename);
        if (!file.is_open()) 
        {
            return -1; // Ошибка открытия файла
        }

        // Парсим JSON
        json j;
        file >> j;

        // Получаем доступ к данным
        auto& muftaData = j["MuftaData"];

        // Освобождаем старые данные, если они есть
        FreeMuftaData();

        // Парсим таблицу Var
        auto& varArray = muftaData["Var"];
        muftaVarTable.rows = varArray.size();
        if (muftaVarTable.rows == 0) 
        {
            return -2; // Нет данных в таблице
        }

        muftaVarTable.cols = varArray[0].size();

        // Выделяем память для таблицы
        muftaVarTable.data = (double**)malloc(muftaVarTable.rows * sizeof(double*));
        for (size_t i = 0; i < muftaVarTable.rows; i++) 
        {
            muftaVarTable.data[i] = (double*)malloc(muftaVarTable.cols * sizeof(double));

            // Заполняем строку данными
            for (size_t k = 0; k < muftaVarTable.cols; k++) 
            {
                muftaVarTable.data[i][k] = varArray[i][k].get<double>();
            }
        }

        // Парсим массивы имен
        auto& nameArrays = muftaData["NameArrays"];

        // Вспомогательная лямбда для загрузки массива имен
        auto loadNameArray = [](MuftaNameArray& arr, const json& jArray) {
            arr.count = jArray.size();
            arr.data = (const char**)malloc(arr.count * sizeof(const char*));
            for (size_t i = 0; i < arr.count; i++) {
                std::string str = jArray[i].get<std::string>();
                arr.data[i] = _strdup(str.c_str());
            }
            };

        // Загружаем все массивы
        loadNameArray(name_d,   nameArrays["d"]);
        loadNameArray(name_dt2, nameArrays["dt2"]);
        loadNameArray(name_b,   nameArrays["b"]);
        loadNameArray(name_b1,  nameArrays["b1"]);
        loadNameArray(name_d1,  nameArrays["d1"]);
        loadNameArray(name_D,   nameArrays["D"]);
        loadNameArray(name_D1,  nameArrays["D1"]);
        loadNameArray(name_L,   nameArrays["L"]);
        loadNameArray(name_l,   nameArrays["l"]);
        loadNameArray(name_r,   nameArrays["r"]);
        loadNameArray(name_c,   nameArrays["c"]);
        loadNameArray(name_c1,  nameArrays["c1"]);

        file.close();
        return 0;
    }
    catch (const std::exception& e) {
        // Обработка ошибок парсинга
        #ifdef _DEBUG
            OutputDebugStringA(e.what());
        #endif
        return -3;; // Ошибка парсинга JSON
    }
}

// Функция для освобождения памяти
void FreeMuftaData(void) {
    // Освобождаем таблицу Var
    if (muftaVarTable.data) {
        for (size_t i = 0; i < muftaVarTable.rows; i++) {
            free(muftaVarTable.data[i]);
        }
        free(muftaVarTable.data);
        muftaVarTable.data = nullptr;
        muftaVarTable.rows = 0;
        muftaVarTable.cols = 0;
    }

    // Освобождаем массивы имен
    FreeNameArray(name_d);
    FreeNameArray(name_dt2);
    FreeNameArray(name_b);
    FreeNameArray(name_b1);
    FreeNameArray(name_d1);
    FreeNameArray(name_D);
    FreeNameArray(name_D1);
    FreeNameArray(name_L);
    FreeNameArray(name_l);
    FreeNameArray(name_r);
    FreeNameArray(name_c);
    FreeNameArray(name_c1);
}
