#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "header.h"

#define NUM_CALLS 100 // максимальное количество записей в массиве calls

calls *addCall(const char *filename, calls *, int *);
calls *removeCall(const char *filename, calls *, int *, int);
void modifyCalls(const char *filename, calls *, int);
calls *createArray(const char *filename, int);
void calculateTotals(calls *call, int);


#define MAX_UNIQUE_CITIES 100

typedef struct {
    char city[20];
    char code[20];
} CityCode;

CityCode uniqueCities[MAX_UNIQUE_CITIES];
int uniqueCityCount = 0;

const char* getCityCode(const char *city) {
    for (int i = 0; i < uniqueCityCount; i++) {
        if (strcmp(uniqueCities[i].city, city) == 0) {
            return uniqueCities[i].code;
        }
    }
    return NULL;
}

void addCityCode(const char *city, const char *code) {
    if (uniqueCityCount < MAX_UNIQUE_CITIES) {
        strcpy(uniqueCities[uniqueCityCount].city, city);
        strcpy(uniqueCities[uniqueCityCount].code, code);
        uniqueCityCount++;
    } else {
        printf("Error: Maximum number of unique cities reached.\n");
    }
}

void inputCityAndCode(calls *call) {
    printf("Введите город:\n");
    input_char(call->city, sizeof(call->city));

    const char *existingCode = getCityCode(call->city);
    if (existingCode) {
        strcpy(call->code, existingCode);
    } else {
        printf("Введите код города (состоит из чисел):\n");
        input_numb(call->code, sizeof(call->code));
        addCityCode(call->city, call->code);
    }
}



void createCityCodeArray(calls *callList, int size) {
    uniqueCityCount = 0;
    for (int i = 0; i < size; i++) {
        int cityExists = 0;
        for (int j = 0; j < uniqueCityCount; j++) {
            if (strcmp(callList[i].city, uniqueCities[j].city) == 0) {
                cityExists = 1;
                break;
            }
        }
        if (!cityExists && uniqueCityCount < MAX_UNIQUE_CITIES) {
            strcpy(uniqueCities[uniqueCityCount].city, callList[i].city);
            strcpy(uniqueCities[uniqueCityCount].code, callList[i].code);
            uniqueCityCount++;
        }
    }
}



calls* createArray(const char *filename, int size) {
    FILE *file = fopen(filename, "r+");
    if (file == NULL) {
        perror("Ошибка открытия файла");
        return NULL;
    }

    calls *call = (calls *)malloc(size * sizeof(calls));
    char uniqueCities[100][20]; // Массив для хранения уникальных городов
    char uniqueCodes[100][20]; // Массив для хранения кодов уникальных городов
    int uniqueCount = 0; // Количество уникальных городов

    for (int i = 0; i < size; i++) {
        printf("Введите информацию про звонок номер %d\n", i + 1);
        printf("Введите дату разговора:\n");
        input_date(call[i].date, sizeof(call[i].date));
        
        printf("Введите город:\n");
        input_char(call[i].city, sizeof(call[i].city));
        
        // Проверка на уникальность города
        int cityExists = 0;
        for (int j = 0; j < uniqueCount; j++) {
            if (strcmp(call[i].city, uniqueCities[j]) == 0) {
                strcpy(call[i].code, uniqueCodes[j]);
                cityExists = 1;
                break;
            }
        }

        // Если город не найден, запрос кода города и сохранение уникального города и кода
        if (!cityExists) {
            printf("Введите код города (состоит из чисел):\n");
            input_numb(call[i].code, sizeof(call[i].code));
            strcpy(uniqueCities[uniqueCount], call[i].city);
            strcpy(uniqueCodes[uniqueCount], call[i].code);
            uniqueCount++;
        }

        printf("Введите время разговора (в мин):\n");
        call[i].time = Finput(0.0, 10000.0);

        printf("Введите тариф (р./мин):\n");
        float s = Finput(0.0, 101.0);
        int ss = (int)s;
        if (ss == s) {
            call[i].tarif.intV = ss;
            call[i].is_float = 0;
        } else {
            call[i].tarif.floatV = s;
            call[i].is_float = 1;
        }

        printf("Номер содержит не более 12 чисел. Введите номер, на который звонили (номер города):\n");
        input_numb(call[i].PhoneNumber, sizeof(call[i].PhoneNumber));

        printf("Введите номер абонента:\n");
        input_numb(call[i].PhoneAbonents, sizeof(call[i].PhoneAbonents));
    }

    File_Enter(filename, call, size);
    fclose(file);
    return call;
}
calls* addCall(const char *filename, calls *call, int *size) {
    FILE *file = fopen(filename, "r+");
    if (file == NULL) {
        perror("Ошибка открытия файла");
        return NULL;
    }

    // Создаем структуру городов и кодов по имеющемуся массиву звонков
    createCityCodeArray(call, *size);

    *size += 1;
    call = (calls *)realloc(call, (*size) * sizeof(calls));
    if (call == NULL) {
        printf("Memory allocation failed!\n");
        fclose(file);
        exit(1);
    }

    int i = *size - 1;
    printf("Введите информацию про клиента номер %d\n", i + 1);
    printf("Введите информацию про звонок номер %d\n", i + 1);
    printf("Введите дату разговора:\n");
    input_date(call[i].date, sizeof(call[i].date));

    inputCityAndCode(&call[i]);

    printf("Введите время разговора (в мин):\n");
    call[i].time = Finput(0.0, 10000.0);

    printf("Введите тариф (р./мин):\n");
    float s = Finput(0.0, 101.0);
    int ss = (int)s;
    if (ss == s) {
        call[i].tarif.intV = ss;
        call[i].is_float = 0;
    } else {
        call[i].tarif.floatV = s;
        call[i].is_float = 1;
    }

    printf("Номер содержит не более 12 чисел. Введите номер, на который звонили (номер города):\n");
    input_numb(call[i].PhoneNumber, sizeof(call[i].PhoneNumber));

    printf("Введите номер абонента:\n");
    input_numb(call[i].PhoneAbonents, sizeof(call[i].PhoneAbonents));

    writeSingleCallToFile(file, call[i]);
    fclose(file);
    return call;
}


typedef int (*cmp_func)(const calls *, const void *);
calls *removeCall(const char *filename, calls *call, int *size, int index)
{
    for (int i = index; i < *size - 1; i++)
    {
        call[i] = call[i + 1];
    }
    *size -= 1;
    call = (calls *)realloc(call, (*size) * sizeof(calls));
    if (call == NULL && *size > 0)
    {
        printf("Memory allocation failed!\n");
        exit(1);
    }
    File_Enter(filename, call, *size);

    return call;
}

// Функция для изменения поля структуры клиента в файле и массиве
void modifyCalls(const char *filename, calls *call, int ind)
{
    FILE *file = fopen(filename, "r+");
    printf("выберите поле для изменения:\n");
    printf("1. дата\n");
    printf("2. код города\n");
    printf("3. город\n");
    printf("4. время разговора\n");
    printf("5. тариф\n");
    printf("6. номер города\n");
    printf("7. номер абонента\n");
    int choice = input(1, 7); // получает выбор пользователя

    calls *ca = &call[ind - 1];

    switch (choice)
    { // выбирает поле для изменения на основе ввода пользователя
    case 1:
    {
        printf("введите другую дату: ");
        char data[12];
        input_date(data, sizeof(data));
        strcpy(call->date, data); // обновляет поле в массиве структур
        break;
    }
    case 2:
    {
        printf("введите новый код города: ");
        char citycode[20];
        input_numb(citycode, sizeof(citycode));

        strcpy(call->code, citycode); // обновляет поле в массиве структур
        break;
    }
    case 3:
    {
        printf("введите другой город: ");
        char town[20];
        input_char(town, sizeof(town));

        strcpy(call->city, town); // обновляет поле в массиве структур
        break;
    }
    case 4:
    {
        printf("введите другое время разговора: ");
        float min = Finput(0, 10000.0);

        call->time = min; // обновляет поле в массиве структур
        break;
    }
    case 5:
    {
        printf("введите новый тариф: ");
        float s = Finput(0.0, 100.0);
        int ss = (int)s;
        if (ss == s)
        {
            call->tarif.intV = ss;
            call->is_float = 0;
        }
        else
        {
            call->tarif.floatV = s;
            call->is_float = 1;
        }

        break;
    }
    case 6:
    {
        printf("введите новый номер горрода: ");
        char numb[13];
        input_numb(numb, sizeof(numb));

        strcpy(call->PhoneNumber, numb); // обновляет поле в массиве структур
        break;
    }
    case 7:
    {
        printf("введите новый номер горрода: ");
        char number[13];
        input_numb(number, sizeof(number));

        strcpy(call->PhoneAbonents, number); // обновляет поле в массиве структур
        break;
    }
    default:
        printf("некорректный выбор\n");
        break;
    }
    writeSingleCallToFile(file, *ca);
    fclose(file);
}

void calculateTotals(calls *call, int num_calls)
{

    // Создание массива уникальных городов
    char cities[NUM_CALLS][20];
    int city_count = 0;

    // Временные массивы для хранения общего времени и суммы
    float total_time[NUM_CALLS] = {0};
    float total_sum[NUM_CALLS] = {0};

    for (int i = 0; i < num_calls; i++)
    {
        int city_index = -1;

        // Поиск индекса города в массиве уникальных городов
        for (int j = 0; j < city_count; j++)
        {
            if (strcmp(cities[j], call[i].city) == 0)
            {
                city_index = j;
                break;
            }
        }

        // Если город не найден, добавляем его в массив уникальных городов
        if (city_index == -1)
        {
            strcpy(cities[city_count], call[i].city);
            city_index = city_count;
            city_count++;
        }

        // Суммируем время и стоимость звонков для города
        total_time[city_index] += call[i].time;
        if (call[i].is_float)
        {
            total_sum[city_index] += call[i].tarif.floatV * call[i].time;
        }
        else
        {
            total_sum[city_index] += call[i].tarif.intV * call[i].time;
        }
    }

    // Вывод результатов
    for (int i = 0; i < city_count; i++)
    {
        printf("Город: %s, Общее время разговоров: %.2f, Сумма: %.2f\n", cities[i], total_time[i], total_sum[i]);
    }
}

typedef int (*cmp_func)(const calls *, const void *);

// Функции сравнения для различных полей
int cmp_date(const calls *call, const void *value)
{
    return strcmp(call->date, (const char *)value);
}

int cmp_code(const calls *call, const void *value)
{
    return strcmp(call->code, (const char *)value);
}

int cmp_city(const calls *call, const void *value)
{
    return strcmp(call->city, (const char *)value);
}

int cmp_time(const calls *call, const void *value)
{
    return call->time == *(const float *)value ? 0 : 1;
}

int cmp_tarif(const calls *call, const void *value)
{
    if (call->is_float)
    {
        return call->tarif.floatV == *(const float *)value ? 0 : 1;
    }
    else
    {
        return call->tarif.intV == *(const int *)value ? 0 : 1;
    }
}

int cmp_phone_number(const calls *call, const void *value)
{
    return strcmp(call->PhoneNumber, (const char *)value);
}

int cmp_phone_abonents(const calls *call, const void *value)
{
    return strcmp(call->PhoneAbonents, (const char *)value);
}

calls *removeCallsByField(calls *callList, int *size, cmp_func cmp, const void *value)
{
    int j = 0;
    for (int i = 0; i < *size; i++)
    {
        if (cmp(&callList[i], value) != 0)
        {
            callList[j++] = callList[i];
        }
    }

    *size = j;
    callList = realloc(callList, (*size) * sizeof(calls));
    if (callList == NULL && *size > 0)
    {
        perror("Ошибка перераспределения памяти");
        exit(1);
    }
    const char filename[] = "/home/porphyriacus/Documents/453502/ОАиП/LR8/Task_2/call.txt";
    File_Enter(filename, callList, *size);
    return callList;
}
void removeByField(calls **callList, int *size)
{
    if (*callList != NULL)
    {
        printf("Выберите поле для удаления:\n");
        printf("1. Дата\n");
        printf("2. Код города\n");
        printf("3. Город\n");
        printf("4. Время разговора\n");
        printf("5. Тариф\n");
        printf("6. Номер города\n");
        printf("7. Номер абонента\n");

        int choice = input(1, 7); // Функция для ввода выбора пользователя

        void *value;
        char strValue[20];
        float floatValue;
        int intValue;
        cmp_func cmp;

        switch (choice)
        {
        case 1:
            printf("Введите дату: ");
            input_date(strValue, sizeof(strValue));
            value = strValue;
            cmp = cmp_date;
            break;
        case 2:
            printf("Введите код города: ");
            input_numb(strValue, sizeof(strValue));
            value = strValue;
            cmp = cmp_code;
            break;
        case 3:
            printf("Введите город: ");
            input_char(strValue, sizeof(strValue));
            value = strValue;
            cmp = cmp_city;
            break;
        case 4:
            printf("Введите время разговора: ");
            floatValue = Finput(0.0, 10000.0);
            value = &floatValue;
            cmp = cmp_time;
            break;
        case 5:
            printf("Введите тариф: ");
            floatValue = Finput(0.0, 100.0);
            value = &floatValue;
            cmp = cmp_tarif;
            break;
        case 6:
            printf("Введите номер города: ");
            input_numb(strValue, sizeof(strValue));
            value = strValue;
            cmp = cmp_phone_number;
            break;
        case 7:
            printf("Введите номер абонента: ");
            input_numb(strValue, sizeof(strValue));
            value = strValue;
            cmp = cmp_phone_abonents;
            break;
        default:
            printf("Некорректный выбор.\n");
            return;
        }

        *callList = removeCallsByField(*callList, size, cmp, value);

        printf("Записи успешно удалены.\n");
    }
    else
    {
        printf("Список звонков пуст.\n");
    }
}
