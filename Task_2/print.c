#include <stdio.h>
#include <stdlib.h>
#include "header.h"

// Функция для вывода данных о звонке
void printCall(const calls *call)
{
    printf("Дата разговора: %02d.%02d.%04d\n", call->day, call->month, call->year);
    printf("Код города: %s\n", call->code);
    printf("Город: %s\n", call->city);
    printf("Время разговора: %.2f\n", (call->time));
    if (call->is_float)
    {
        printf("Тариф: %.2f\n", call->tarif.floatV);
    }
    else
    {
        printf("Тариф: %d\n", call->tarif.intV);
    }
    printf("Номер города: %s\n", call->PhoneNumber);
    printf("Номер абонента: %s\n", call->PhoneAbonents);
}

// Функция для вывода списка абонентов и получения подробной информации
void list(calls *call, int n)
{
    printf("Список покупателей по номеру абонента:\n");
    for (int i = 0; i < n; ++i)
    {
        printf("%d. %s\n", i + 1, call[i].PhoneAbonents);
    }

    printf("Введите номер абонента, про звонок которого вы хотите узнать больше информации; введите 0, если хотите выйти:\n");

    int choice = input(0, n + 1);
    if (choice == 0){
        return;
    }
    printCall(&call[choice - 1]);
}
