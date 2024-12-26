#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "header.h"


// Функция для записи клиента в файл
void File_Enter(const char *filename, calls *call, int size) {
    FILE* file = fopen(filename, "w+");
    if (file != NULL) {
        for (int i = 0; i < size; i++) {
            fprintf(file, "Дата разговора: %02d.%02d.%04d\n", call[i].day, call[i].month, call[i].year);
            fprintf(file, "Код города: %s\n", call[i].code);
            fprintf(file, "Город: %s\n", call[i].city);
            fprintf(file, "Время разговора: %.2f\n", call[i].time);
            if (call[i].is_float) {
                fprintf(file, "Тариф: %.2f\n", call[i].tarif.floatV);
            } else {
                fprintf(file, "Тариф: %d\n", call[i].tarif.intV);
            }
            fprintf(file, "Номер города: %s\n", call[i].PhoneNumber);
            fprintf(file, "Номер абонента: %s\n", call[i].PhoneAbonents);
            fprintf(file, "\n"); // Разделение записей пустой строкой
        }
        fclose(file);
    } else {
        perror("Ошибка записи в файл");
    }
}

// Функция для подсчета количества записей в файле
static int countRecordsInFile(const char *filename) {
    FILE *file = fopen(filename, "r");
    if (file == NULL) {
        perror("Ошибка открытия файла для чтения");
        return -1;
    }

    int count = 0;
    char line[256];
    while (fgets(line, sizeof(line), file) != NULL) {
        if (strncmp(line, "Дата разговора:", 15) == 0) {
            count++;
        }
    }

    fclose(file);
    return count;
}

// Функция для чтения данных из файла и создания массива структур
calls* readCallsFromFile(calls *call, const char *filename, int *size) {
    FILE *file = fopen(filename, "r+");
    if (file == NULL) {
        perror("Ошибка открытия файла для чтения");
        return NULL;
    }
    int count = countRecordsInFile(filename);
    call = malloc(count * sizeof(calls));
    if (call == NULL) {
        perror("Ошибка выделения памяти");
        fclose(file);
        return NULL;
    }

    for (int i = 0; i < count; i++) {
        fscanf(file, "Дата разговора: %d.%d.%d\n", &call[i].day, &call[i].month, &call[i].year);
        fscanf(file, "Код города: %s\n", call[i].code);
        fscanf(file, "Город: %s\n", call[i].city);
        fscanf(file, "Время разговора: %f\n", &call[i].time);
        char tarifType[10];
        fscanf(file, "Тариф: %s\n", tarifType);
        if (strchr(tarifType, '.') != NULL) {
            call[i].is_float = 1;
            call[i].tarif.floatV = atof(tarifType);
        } else {
            call[i].is_float = 0;
            call[i].tarif.intV = atoi(tarifType);
        }
        fscanf(file, "Номер города: %s\n", call[i].PhoneNumber);
        fscanf(file, "Номер абонента: %s\n", call[i].PhoneAbonents);
        fscanf(file, "\n"); // Пропуск пустой строки между записями
    }
    *size = count;
    fclose(file);
    return call;
}
