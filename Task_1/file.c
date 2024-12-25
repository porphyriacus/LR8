#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "header.h"

// Функция для записи клиента в файл
void File_Enter(FILE *file, RegCustomers *customer, int size)
{

    if (file != NULL)
    {
        for (int i = 0; i < size; ++i)
        {
            fwrite(&customer[i], sizeof(RegCustomers), 1, file); // 1 это количество элементов для записи
        }
    }
    else
    {
        perror("Ошибка записи в файл");
    }
}

void ChangeBin(FILE *file, RegCustomers customer, int a)
{
    // добавление в нужное место
    // int а это позиция куда нужно добавить структуру
    // если элементов было 5, а мы хотим поставить на место 4, то a = 4

    if (file != NULL)
    {
        // Вычисляем смещение для указателя
        long bit = sizeof(RegCustomers) * (a);
        fseek(file, bit, SEEK_SET); // Устанавливаем указатель на нужное место в файле
        size_t written = fwrite(&customer, sizeof(RegCustomers), 1, file);
        if (written != 1)
        {
            perror("Ошибка записи в файл"); // выводит последнюю ошибку после вывода заданной строки
        }
    }
    else
    {
        perror("Ошибка открытия файла");
    }
}

// функция для чтения всех структур из бинарного файла
RegCustomers *readAllCustomersFromFile(const char *filename, int *size)
{
    FILE *file = fopen(filename, "rb");
    if (file == NULL)
    {
        // printf("Ошибка открытия файла. Возможно списка пока не существует./n");
        return NULL;
    }

    // определение размер файла
    fseek(file, 0, SEEK_END);    // позиция на последний бит
    long fileSize = ftell(file); //
    fseek(file, 0, SEEK_SET);    // указатель на начало файла

    // определение количества структур (размер данных файла на размер одной структуры)
    *size = fileSize / sizeof(RegCustomers);
    if (!size)
    {
        return NULL;
    }
    // выделение памяти для хранения всех структур
    RegCustomers *customers = (RegCustomers *)malloc(fileSize);
    if (customers == NULL)
    {
        printf("Ошибка выделения памяти");
        fclose(file);
        return NULL;
    }

    // читаем все структуры из файла

    long read = fread(customers, sizeof(RegCustomers), *size, file); // функция возвращвет количество прочитанных элементов в нашем случае структур
    // про функцию fread(customers, sizeof(RegCustomers), *size, file)
    // customers указатель на область памяти в которое будет производиться чтение из файла
    // sizeof(RegCustomers) размер данных считываемых за раз
    // *size количество элементов(в нашем случае структур) которые нужно прочитать
    //  филе это файл из которого собственно происходит чтение
    if (read != *size)
    // если количество прочитанных структур не равно предполагаемому размеру нашего динамического массива(т.у реальному количеству структур) то произошла ошибка чтения файла
    {
        perror("Ошибка чтения из файла");
        free(customers);
        fclose(file);
        return NULL;
    }

    // закрываем файл
    fclose(file);
    sort(customers, *size);
    return customers;
}

void deleteCustomerByIndex(FILE *file, int index)
{

    const char *filename = "file.bin";
    // Открытие временного файла для записи
    FILE *tempFile = fopen("temp.bin", "wb");
    if (tempFile == NULL)
    {
        perror("Ошибка открытия временного файла для записи");
        return;
    }

    RegCustomers customer;
    int currentIndex = 0;
    fseek(file, 0, SEEK_SET); // указатель файла в начало

    while (fread(&customer, sizeof(RegCustomers), 1, file)) // fread возвращает количество проситанных элементов если ниче не прочитал выходим из цикла
    {
        if (currentIndex != index)//записываем во вспомогательный файл данные исключая элемент заданного индекса
        {
            fwrite(&customer, sizeof(RegCustomers), 1, tempFile);
        }
        currentIndex++;
    }

    fclose(tempFile); 

    // Замена исходного файла новым
    if (remove(filename) != 0) // удаляет файл
    {
        perror("Ошибка удаления исходного файла");
        return;
    }
    if (rename("temp.bin", filename) != 0)
    { // переименовывает файл
        perror("Ошибка переименования временного файла");
    }
}
