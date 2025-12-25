#pragma once

#ifdef __cplusplus
extern "C" {
#endif

#include <stddef.h>

    // Структура для хранения данных муфты
    typedef struct {
        const char** data;
        size_t       count;
    } MuftaNameArray;

    // Структура для хранения таблицы параметров муфты
    typedef struct {
        double** data;
        size_t   rows;
        size_t   cols;
    } MuftaVarTable;

    // Глобальные переменные для данных муфты
    extern MuftaVarTable  muftaVarTable;
    extern MuftaNameArray name_d;
    extern MuftaNameArray name_dt2;
    extern MuftaNameArray name_b;
    extern MuftaNameArray name_b1;
    extern MuftaNameArray name_d1;
    extern MuftaNameArray name_D;
    extern MuftaNameArray name_D1;
    extern MuftaNameArray name_L;
    extern MuftaNameArray name_l;
    extern MuftaNameArray name_r;
    extern MuftaNameArray name_c;
    extern MuftaNameArray name_c1;

    // Функция для загрузки данных из JSON файла
    int LoadMuftaDataFromJson(const char* filename);

    // Функция для освобождения памяти
    void FreeMuftaData(void);

#ifdef __cplusplus
}
#endif
