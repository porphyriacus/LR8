#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <curl/curl.h>
#include <json-c/json.h>
#include "header.h"
#include <ctype.h>

int main()
{
    FILE *file = fopen("file.bin", "rb+");
    if (file == NULL)
    {
        printf("Ошибка открытия файла. Файл может не существовать или произошла другая ошибка.\n");
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

            // deleteAllCustomers();
            file = fopen("file.bin", "wb");
            if (file == NULL)
            {
                printf("Ошибка открытия файла для очистки.\n");
                return 1;
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
            char query[1024];
            printf("Введите поисковый запрос: ");
            scanf("%s", query);
            searchInElasticSearch(query);

            // Удаление всех клиентов с сервера
            deleteAllCustomers();
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
            list = removeCustomer(file, list, &numb, index - 1);
            fclose(file);

            /*
             FILE* file = fopen("file.bin", "rb+");
             char query[100];
             int field_choice;
             // Меню выбора поля для поиска
             printf("Выберите поле для поиска:\n");
             printf("1. Имя\n");
             printf("2. Фамилия\n");
             printf("3. Отчество\n");
             printf("4. Пол\n");
             printf("5. Возраст\n");
             printf("6. Адрес\n");
             printf("7. Скидка\n");
             printf("Введите номер поля: ");
             field_choice = input(0,8);
             deleteAllCustomers();
             // Назначение поля для поиска на основе выбора пользователя
             const char *field;
             switch (field_choice)
             {
             case 1:
                 field = "name";
                 break;
             case 2:
                 field = "surname";
                 break;
             case 3:
                 field = "fathername";
                 break;
             case 4:
                 field = "sex";
                 break;
             case 5:
                 field = "age";
                 break;
             case 6:
                 field = "address";
                 break;
             case 7:
                 field = "discount";
                 break;
             default:
                 printf("Неверный выбор поля.\n");
                 return 1;
             }

             printf("Введите значение для поиска: ");
             scanf("%99s", query);

             char uuids[10][37]; // Массив для хранения UUID (максимум 10 UUID)
             int found_uuids = searchDocumentUUIDs(field, query, uuids, 10);

             if (found_uuids > 0)
             {
                 printf("Найдено клиентов: %d\n", found_uuids);

                 // Удаление найденных клиентов из массива и файла
                 for (int i = 0; i < found_uuids; i++)
                 {
                     list = removeCustomerByUUID(file, list, &numb, uuids[i]);
                 }

                 printf("Клиенты удалены.\n");
             }
             else
             {
                 printf("Клиенты не найдены.\n");
             }
            */
             printf("Хотите завершить программу? Введите 1, если хотите завершить программу, 2, если хотите продолжить\t");
             int st = input(0,3);
             uploadAllCustomers(list, numb);
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
            // uploadAllCustomers(list, numb);
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
