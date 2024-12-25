#include <stdio.h>
#include <stdlib.h>
#include "header.h"

void printCustomer(const RegCustomers *customer)//выводит всю информацию о выбранном покупателе
{
    printf("Name: %s\n", customer->name);
    printf("Surname: %s\n", customer->surname);
    printf("Fathername: %s\n", customer->fathername);
    printf("Sex: %s\n", (customer->sex == 2) ? "Male" : "Female");
    printf("Age: %d\n", customer->age);
    printf("Address: %s\n", customer->address);
    if (customer->is_float)
    {
        printf("Discount: %.2f\n", customer->discount.floatV);
    }
    else
    {
        printf("Discount: %d\n", customer->discount.intV);
    }
}

void List(RegCustomers *list, int n) //выводит список всех покупателей
{
    
    printf("Список покупателей по фамилиям:\n");
    for (int i = 0; i < n; ++i)
    {
        printf("%d. %s\n", i + 1, list[i].surname);
    }

    printf("Введите номер покупателя о котором хотите узнать больше информации, введите 0, если хотите выйти:\n");

    int choice = input(-1, n + 1);
    if (choice == 0){
        return;
    }
    printCustomer(&list[choice - 1]);
}
