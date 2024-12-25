#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <curl/curl.h>
#include <json-c/json.h>
#include "header.h"

int main()
{
    const char filename[] = "/home/porphyriacus/Documents/453502/ОАиП/LR8/Task_2/call.txt";
    int size;
    calls *call = NULL;
    call = readCallsFromFile(call, filename, &size);
    if (call == NULL)
    {
        printf("Oшибка открытия файла.\n");
    }
    else
    {
        if (size == 0)
        {
            printf("Вероятно, у вас нет созданного списка звонков. Создайте новый список для дальнейшей работы.\n");
        }
        else
        {
            printf("У вас уже имеется список звонков. Можете использовать его или создать новый список.\n");
        }
    }

    while (1)
    {
        menu();
        int status = input(0, 9);
        int st;
        switch (status)
        {
        case 1:

            printf("Введите количество звонков которые Вы хотите добавить(натуральное число от 1 до 100)\n");
            size = input(0, 101);
            call = createArray(filename, size);

            printf("Хотите завершить программу? Введите 1, если хотите завершить программу, 2, если хотите продолжить\t");
            st = input(0, 3);
            if (st == 1)
            {
                printf("Всего хорошего!:3\n");
                free(call);
                call = NULL;
                return 0;
            }
            break;

        case 2:
            list(call, size);
            printf("Хотите завершить программу? Введите 1, если хотите завершить программу, 2, если хотите продолжить\t");
            st = input(0, 3);
            if (st == 1)
            {
                printf("Всего хорошего!:3\n");
                free(call);
                call = NULL;
                return 0;
            }
            break;

        case 3:
            call = addCall(filename, call, &size);
            printf("Хотите завершить программу? Введите 1, если хотите завершить программу, 2, если хотите продолжить\t");
            st = input(0, 3);
            if (st == 1)
            {
                printf("Всего хорошего!:3\n");
                free(call);
                call = NULL;
                return 0;
            }
            break;

        case 4:

            // Загрузка всех клиентов на сервер
            uploadAllcalls(call, size);
            const char *field = chooseField();
            // Пример использования обновленной функции поиска
            printf("Введите запрос для поиска клиентов: ");
            char query[256];
            scanf("%s", query);
            if (field != NULL)
            {
                searchInElasticSearch(field, query);
            }

            // Удаление всех клиентов с сервера
            deleteAllcalls();

            printf("Хотите завершить программу? Введите 1, если хотите завершить программу, 2, если хотите продолжить\t");
            st = input(0, 3);

            if (st == 1)
            {
                printf("Всего хорошего!:3\n");
                free(call);
                call = NULL;
                return 0;
            }
            break;

        case 5:

            printf("Список звонков по номеру абонента:\n");
            for (int i = 0; i < size; ++i)
            {
                printf("%d. %s\n", i + 1, call[i].PhoneAbonents);
            }
            printf("Введите индекс клиента для удаления: ");
            int index = input(0, size + 1);
            call = removeCall(filename, call, &size, index);

            // removeByField(&call, & size);
            printf("Хотите завершить программу? Введите 1, если хотите завершить программу, 2, если хотите продолжить\t");
            st = input(0, 3);
            if (st == 1)
            {
                printf("Всего хорошего!:3\n");
                free(call);
                call = NULL;
                return 0;
            }
            break;

        case 6:
            modifyCalls(filename, call, size);
            printf("Хотите завершить программу? Введите 1, если хотите завершить программу, 2, если хотите продолжить\t");
            st = input(0, 3);
            if (st == 1)
            {
                printf("Всего хорошего!:3\n");
                free(call);
                call = NULL;
                return 0;
            }
            break;

        case 7:
            calculateTotals(call, size);
            printf("Хотите завершить программу? Введите 1, если хотите завершить программу, 2, если хотите продолжить\t");
            st = input(0, 3);
            if (st == 1)
            {
                printf("Всего хорошего!:3\n");
                free(call);
                call = NULL;
                return 0;
            }
            break;
        case 8:
            printf("Всего хорошего!:3\n");
            free(call);
            call = NULL;
            return 0;
        }
    }
}