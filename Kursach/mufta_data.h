/*
 * mufta_data.h
 * 
 * Структуры и функции для работы с параметрами муфты
 * Чистый C файл БЕЗ #include (всё в pch.h!)
 */

#ifndef MUFTA_DATA_H
#define MUFTA_DATA_H

#include <stdint.h>

/* ════════════════════════════════════════════════════════════════
 * СТРУКТУРЫ
 * ════════════════════════════════════════════════════════════════ */

/**
 * struct MuftaParameter
 * 
 * Один параметр муфты с его значениями
 * 
 * ПРИМЕР: параметр "d" с 18 значениями (9.0, 10.0, 11.0, ...)
 */
typedef struct MuftaParameter {
    char name[32];              // "d", "dt2", "b", и т.д. (макс 31 символ)
    char description[128];      // "Диаметр вала", "Ширина муфты", и т.д.
    char unit[16];              // "mm", "deg", и т.д.
    uint32_t index;             // 0-11 (индекс параметра в таблице)
    double* values;             // Указатель на массив значений
    uint32_t values_count;      // Количество значений (обычно 18)
} MuftaParameter;


/**
 * struct MuftaTable
 * 
 * Таблица со всеми параметрами муфты
 * 
 * ПРИМЕР: 12 параметров × 18 значений каждый
 */
typedef struct MuftaTable {
    MuftaParameter** parameters;    // Массив указателей на параметры
    uint32_t parameters_count;      // Количество параметров (обычно 12)
} MuftaTable;


/* ════════════════════════════════════════════════════════════════
 * ФУНКЦИИ API (8 функций)
 * ════════════════════════════════════════════════════════════════ */

/**
 * mufta_parameter_create
 * 
 * Создать один параметр
 * 
 * ПРИМЕР:
 *   MuftaParameter* param = mufta_parameter_create(
 *       "d", "Диаметр вала (мм)", "mm", 0
 *   );
 */
MuftaParameter* mufta_parameter_create(
    const char* name,
    const char* description,
    const char* unit,
    uint32_t index
);


/**
 * mufta_parameter_add_value
 * 
 * Добавить одно значение к параметру
 * 
 * ПРИМЕР:
 *   mufta_parameter_add_value(param, 9.0);
 *   mufta_parameter_add_value(param, 10.0);
 *   // ... добавить 16 остальных
 */
void mufta_parameter_add_value(MuftaParameter* param, double value);


/**
 * mufta_parameter_get_value
 * 
 * Получить значение из параметра по индексу
 * 
 * ПРИМЕР:
 *   double val = mufta_parameter_get_value(param, 5);  // = 16.0
 */
double mufta_parameter_get_value(MuftaParameter* param, uint32_t index);


/**
 * mufta_parameter_free
 * 
 * Освободить память параметра
 * 
 * ПРИМЕР:
 *   mufta_parameter_free(param);
 *   param = NULL;
 */
void mufta_parameter_free(MuftaParameter* param);


/**
 * mufta_table_create
 * 
 * Создать пустую таблицу
 * 
 * ПРИМЕР:
 *   MuftaTable* table = mufta_table_create();
 */
MuftaTable* mufta_table_create(void);


/**
 * mufta_table_add_parameter
 * 
 * Добавить параметр в таблицу
 * 
 * ПРИМЕР:
 *   mufta_table_add_parameter(table, param_d);
 *   mufta_table_add_parameter(table, param_dt2);
 */
void mufta_table_add_parameter(MuftaTable* table, MuftaParameter* param);


/**
 * mufta_table_get_parameter_by_name
 * 
 * Найти параметр по названию
 * 
 * ПРИМЕР:
 *   MuftaParameter* p = mufta_table_get_parameter_by_name(table, "d");
 *   if (p) printf("Найден: %s\n", p->description);
 */
MuftaParameter* mufta_table_get_parameter_by_name(
    MuftaTable* table,
    const char* name
);


/**
 * mufta_table_get_var_value
 * 
 * Получить значение Var[row][col] (ГЛАВНАЯ ФУНКЦИЯ!)
 * 
 * ПРИМЕР:
 *   double d = mufta_table_get_var_value(table, 0, 5);   // параметр 0, значение 5
 *   double b = mufta_table_get_var_value(table, 2, 10);  // параметр 2, значение 10
 * 
 * row = 0-11 (индекс параметра: 0=d, 1=dt2, 2=b, 3=b1, 4=d1, 5=D, 6=D1, 7=L, 8=l, 9=r, 10=c, 11=c1)
 * col = 0-17 (индекс значения)
 */
double mufta_table_get_var_value(MuftaTable* table, uint32_t row, uint32_t col);


/**
 * mufta_table_free
 * 
 * Освободить всю таблицу (все параметры и значения)
 * 
 * ПРИМЕР:
 *   mufta_table_free(table);
 *   table = NULL;
 */
void mufta_table_free(MuftaTable* table);


#endif // MUFTA_DATA_H
