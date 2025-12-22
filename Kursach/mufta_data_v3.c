/*
 * mufta_data.c
 * 
 * Реализация функций для работы с параметрами муфты
 * БЕЗ #include в этом файле (всё подключается через pch.h)
 */

// ВНИМАНИЕ: #include "mufta_data.h" должен быть в pch.h!
// В этом файле не добавляем #include, так как используется precompiled header

#include "mufta_data.h"  // Даже так можно - pch.h уже включил!
#include <stdlib.h>
#include <string.h>
#include <stdio.h>


/* ════════════════════════════════════════════════════════════════
 * ФУНКЦИИ ПАРАМЕТРА
 * ════════════════════════════════════════════════════════════════ */

MuftaParameter* mufta_parameter_create(
    const char* name,
    const char* description,
    const char* unit,
    uint32_t index
)
{
    MuftaParameter* param = (MuftaParameter*)malloc(sizeof(MuftaParameter));
    
    if (!param) {
        fprintf(stderr, "Error: malloc failed in mufta_parameter_create\n");
        return NULL;
    }
    
    // Копируем строки
    strncpy_s(param->name, sizeof(param->name), name, _TRUNCATE);
    strncpy_s(param->description, sizeof(param->description), description, _TRUNCATE);
    strncpy_s(param->unit, sizeof(param->unit), unit, _TRUNCATE);
    
    param->index = index;
    param->values = NULL;
    param->values_count = 0;
    
    return param;
}


void mufta_parameter_add_value(MuftaParameter* param, double value)
{
    if (!param) return;
    
    // Увеличиваем массив на одно значение
    double* new_values = (double*)realloc(
        param->values,
        (param->values_count + 1) * sizeof(double)
    );
    
    if (!new_values) {
        fprintf(stderr, "Error: realloc failed in mufta_parameter_add_value\n");
        return;
    }
    
    param->values = new_values;
    param->values[param->values_count] = value;
    param->values_count++;
}


double mufta_parameter_get_value(MuftaParameter* param, uint32_t index)
{
    if (!param || index >= param->values_count) {
        return 0.0;
    }
    
    return param->values[index];
}


void mufta_parameter_free(MuftaParameter* param)
{
    if (!param) return;
    
    if (param->values) {
        free(param->values);
        param->values = NULL;
    }
    
    free(param);
}


/* ════════════════════════════════════════════════════════════════
 * ФУНКЦИИ ТАБЛИЦЫ
 * ════════════════════════════════════════════════════════════════ */

MuftaTable* mufta_table_create(void)
{
    MuftaTable* table = (MuftaTable*)malloc(sizeof(MuftaTable));
    
    if (!table) {
        fprintf(stderr, "Error: malloc failed in mufta_table_create\n");
        return NULL;
    }
    
    table->parameters = NULL;
    table->parameters_count = 0;
    
    return table;
}


void mufta_table_add_parameter(MuftaTable* table, MuftaParameter* param)
{
    if (!table || !param) return;
    
    // Увеличиваем массив на один параметр
    MuftaParameter** new_params = (MuftaParameter**)realloc(
        table->parameters,
        (table->parameters_count + 1) * sizeof(MuftaParameter*)
    );
    
    if (!new_params) {
        fprintf(stderr, "Error: realloc failed in mufta_table_add_parameter\n");
        return;
    }
    
    table->parameters = new_params;
    table->parameters[table->parameters_count] = param;
    table->parameters_count++;
}


MuftaParameter* mufta_table_get_parameter_by_name(
    MuftaTable* table,
    const char* name
)
{
    if (!table || !name) return NULL;
    
    for (uint32_t i = 0; i < table->parameters_count; i++) {
        if (table->parameters[i] && 
            strcmp(table->parameters[i]->name, name) == 0) {
            return table->parameters[i];
        }
    }
    
    return NULL;
}


double mufta_table_get_var_value(MuftaTable* table, uint32_t row, uint32_t col)
{
    if (!table || row >= table->parameters_count) {
        return 0.0;
    }
    
    MuftaParameter* param = table->parameters[row];
    if (!param || col >= param->values_count) {
        return 0.0;
    }
    
    return param->values[col];
}


void mufta_table_free(MuftaTable* table)
{
    if (!table) return;
    
    // Освобождаем все параметры
    for (uint32_t i = 0; i < table->parameters_count; i++) {
        if (table->parameters[i]) {
            mufta_parameter_free(table->parameters[i]);
        }
    }
    
    // Освобождаем массив параметров
    if (table->parameters) {
        free(table->parameters);
        table->parameters = NULL;
    }
    
    // Освобождаем саму таблицу
    free(table);
}
