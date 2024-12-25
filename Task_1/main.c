#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <curl/curl.h>
#include <json-c/json.h>
#include "header.h"

int main()
{
    FILE *file = fopen("file.bin", "rb+");
    if (file == NULL)
    {
        printf("Ошибка открытия файла. Файл может не существовать или произошла другая ошибка.\n");
        return 1;
    }

    int numb = 0;
    RegCustomers *list = readAllCustomersFromFile("file.bin", &numb);
        if (list == NULL)
    {
        printf("Oшибка открытия файла.\n");
    }
    else
    {
        if (numb == 0)
        {
            printf("Вероятно, у вас нет созданного списка. Создайте новый список для дальнейшей работы.\n");
        }
        else
        {
            printf("У вас уже имеется список клиентов. Можете использовать его или создать новый список.\n");
        }
    }
    fclose(file);

    while (1)
    {
        discount(list, numb);
        menu();
        int status = input(0, 8);
        int st;
        switch (status)
        {
        case 1:
            if (remove("file.bin") != 0)
            {
                printf("Ошибка удаления исходного файла.\n");
            }
            else
            {
                printf("Сколько клиентов будет в вашем списке? Введите целое число от 1 до 100.\n");
                numb = input(0, 101);
                file = fopen("file.bin", "wb+");
                if (file == NULL)
                {
                    printf("Ошибка создания файла.\n");
                    return 1;
                }
                list = createArray(file, numb);
            }

            printf("Хотите завершить программу? Введите 1, если хотите завершить программу, 2, если хотите продолжить\t");
            st = input(0, 3);
            if (st == 1)
            {
                printf("Всего хорошего!:3\n");
                free(list);
                list = NULL;
                return 0;
            }
            break;

        case 2:
            List(list, numb);
            printf("Хотите завершить программу? Введите 1, если хотите завершить программу, 2, если хотите продолжить\t");
            st = input(0, 3);
            if (st == 1)
            {
                printf("Всего хорошего!:3\n");
                free(list);
                list = NULL;
                return 0;
            }
            break;

        case 3:
            file = fopen("file.bin", "rb+");
            if (file == NULL)
            {
                printf("Ошибка открытия файла.\n");
                return 1;
            }
            list = addCustomer(file, list, &numb);
            fclose(file);
            printf("Хотите завершить программу? Введите 1, если хотите завершить программу, 2, если хотите продолжить\t");
            st = input(0, 3);
            if (st == 1)
            {
                printf("Всего хорошего!:3\n");
                free(list);
                list = NULL;
                return 0;
            }
            break;

        case 4:
            // Загрузка всех клиентов на сервер
            uploadAllCustomers(list, numb);
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
            deleteAllCustomers();

            printf("Хотите завершить программу? Введите 1, если хотите завершить программу, 2, если хотите продолжить\t");
            st = input(0, 3);
            
            if (st == 1)
            {
                printf("Всего хорошего!:3\n");
                free(list);
                list = NULL;
                return 0;
            }
            break;

        case 5:
            printf("Список покупателей по фамилиям:\n");
            for (int i = 0; i < numb; ++i)
            {
                printf("%d. %s\n", i + 1, list[i].surname);
            }
            printf("Введите индекс клиента для удаления: ");
            int index = input(0, numb + 1);
            file = fopen("file.bin", "rb+");
            if (file == NULL)
            {
                printf("Ошибка открытия файла.\n");
                return 1;
            }
            list = removeCustomer(file, list, &numb, index - 1);
            fclose(file);
            printf("Хотите завершить программу? Введите 1, если хотите завершить программу, 2, если хотите продолжить\t");
            st = input(0, 3);
            if (st == 1)
            {
                printf("Всего хорошего!:3\n");
                free(list);
                list = NULL;
                return 0;
            }
            break;

        case 6:
            file = fopen("file.bin", "rb+");
            if (file == NULL)
            {
                printf("Ошибка открытия файла.\n");
                return 1;
            }
            printf("Список покупателей по фамилиям:\n");
            for (int i = 0; i < numb; ++i)
            {
                printf("%d. %s\n", i + 1, list[i].surname);
            }
            printf("Введите индекс клиента для изменения: ");
            int ind = input(0, numb + 1);
            modifyCustomer(file, &list[ind - 1], ind);
            fclose(file);
            printf("Хотите завершить программу? Введите 1, если хотите завершить программу, 2, если хотите продолжить\t");
            st = input(0, 3);
            if (st == 1)
            {
                printf("Всего хорошего!:3\n");
                free(list);
                list = NULL;
                return 0;
            }
            break;

        case 7:
            printf("Всего хорошего!:3\n");
            free(list);
            list = NULL;
            return 0;
        }
    }
}
