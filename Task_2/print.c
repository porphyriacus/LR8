#include <stdio.h>
#include <stdlib.h>
#include "header.h"

void printCall(const calls *call)
{
    printf("Дата разговора: %s\n", call->date);
    printf("Код города: %s\n", call->code);
    printf("Город: %s\n", call->city);
    printf("Время разгоаора: %.2f\n", (call->time));
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

void list(calls *call, int n)
{
    
    printf("Список покупателей по номеру абонента:\n");
    for (int i = 0; i < n; ++i)
    {
        printf("%d. %s\n", i + 1, call[i].PhoneAbonents);
    }

    printf("Введите номер абонента, про звонок которого вы хотите узнать больше информации; введите 0, если хотите выйти:\n");

    int choice = input(-1, n + 1);
    if (choice == 0){
        return;
    }
    printCall(&call[choice - 1]);
}
