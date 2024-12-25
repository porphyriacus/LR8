#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "header.h"

RegCustomers *sort(RegCustomers *customers, const int size) // сортировка вставками по возрастаию(по значению поля скидка)

{ // О(n^2) в лучшем случае n
    for (int i = 1; i < size; ++i)
    {
        // выбираем опорный элемент
        float f_key = 0.0f;
        int i_key = 0;
        int is_float = customers[i].is_float;

        if (is_float)
        {
            f_key = customers[i].discount.floatV;
        }
        else
        {
            i_key = customers[i].discount.intV;
        }

        RegCustomers keyy = customers[i];
        int j = i - 1;
        // до тех пор пока элемент больше предыдущего меняем элементы местами
        // в рбщем каждый элемент "вставляется" туда где должен  находится
        while (j >= 0 && ((customers[j].is_float ? customers[j].discount.floatV : customers[j].discount.intV) > (is_float ? f_key : i_key)))
        {
            customers[j + 1] = customers[j];
            --j;
        }

        customers[j + 1] = keyy;

        if (is_float)
        {
            customers[j + 1].discount.floatV = f_key;
        }
        else
        {
            customers[j + 1].discount.intV = i_key;
        }
    }
    printf("\n\n");
    return customers;
}

void discount(RegCustomers *customers, const int size) // функция для вывода покупателей имеющих скидку 5%
{
    printf("\n\nСкидку 5%% имеют покупатели с фамилиями:\n");
    int check = 0;
    for (int i = 0; i < size; ++i)
    {
        int is_float = customers[i].is_float;

        if (!is_float) // если число с плавающей точкой то его можно не проверять
        {
            if (customers[i].discount.intV == 5)
            {
                ++check;
                printf("%s\n", customers[i].surname);
            }
            if (customers[i + 1].discount.intV != 5) // тк массив отсортирован то все покупатели со скидкой 5% находятся в одном месте
            {
                printf("\n");
                return;
            }
        }
    }
    if (!check)
    {
        printf("Никто из покупателей не имеет скидку 5%% :(\n");
    }
}

RegCustomers *addCustomer(FILE *file, RegCustomers *customers, int *size);
RegCustomers *removeCustomer(FILE *file, RegCustomers *customers, int *size, int index);
void modifyCustomer(FILE *file, RegCustomers *customers, int ind);

RegCustomers *createArray(FILE *file, int size) // функция создания списка покупателей
{

    RegCustomers *customers = (RegCustomers *)malloc(size * sizeof(RegCustomers)); // динамически выделяем память исходя из заданного пользователем размера
    short check = 0;
    for (int i = 0; i < size; i++)
    {
        if (check)
        {
            removeCustomer(file, customers, &size, i);
        }
        else
        {
            printf("Введите информацию про клиента номер %d\n", i + 1);
            printf("Введите имя клиента. Если Вы уважаете человека, введите имя с большой буквы. Впрочем, дело Ваше.\n");
            input_char(customers[i].name, sizeof(customers[i].name));
            if (strcmp(customers[i].name, "VaDiMoChKa") == 0)
            {
                printf("Вы ввели VaDiMoChKa! Ввод прекращается");
                removeCustomer(file, customers, &size, i);
                check = 1;
                continue;
            }
            else
            {
                printf("Введите фамилию клиента.\t");
                input_char(customers[i].surname, sizeof(customers[i].surname));
                printf("Введите отчество клиента.\t");
                input_char(customers[i].fathername, sizeof(customers[i].fathername));
                printf("Если ваш клиент женщина введите 1, если мужчина, введите 2\t");
                customers[i].sex = input(0, 3);
                printf("Введите возраст клиента.\t");
                customers[i].age = input(0, 150);
                printf("Введите адрес клиента.\t");
                input_address(customers[i].address, sizeof(customers[i].address));
                printf("Введите размер скидки клиента.\t");
                float s = Finput(0.0, 101.0);
                int ss = s; // если ss == s то число целое тк при приведении дробного к целому сохраняется только целая часть
                if (ss == s)
                {
                    customers[i].discount.intV = s;
                    customers[i].is_float = 0;
                }
                else
                {
                    customers[i].discount.floatV = s;
                    customers[i].is_float = 1;
                }
            }
        }
        // while(!check){  addCustomer(file, customers, &size, & check);}
    }
    // Закрытие файла после всех операций
    customers = sort(customers, size); // сортируем список
    File_Enter(file, customers, size); // сохраняем список в бинарный файл
    fclose(file);

    return customers;
}

// #include <stddef.h> // включает библиотеку для использования макроса offsetof
// зачем библиотека если можно ручками
#define offsetof(type, member) ((size_t) & (((type *)0)->member))
// плотненький коммент
//  1. size_t
//  беззнаковый тип данных 8 байт на х64
// (type)0 значение указателя заданного типа равное 0 !!нее указатель нужна *
// ((type *)0)->member  ->member разыменовывает этот указатель и получает смещение члена member относительно начала структуры.
//((type *)0) создает указатель, указывающий на начало структуры. оператор -> используется для доступа к члену структуры member.

// функция для изменения поля структуры клиента в файле и массиве
void modifyCustomer(FILE *file, RegCustomers *customers, int ind)
{
    printf("выберите поле для изменения:\n");
    printf("1. имя\n");
    printf("2. фамилия\n");
    printf("3. отчество\n");
    printf("4. пол\n");
    printf("5. возраст\n");
    printf("6. адрес\n");
    printf("7. скидка\n");
    int choice = input(1, 7); // получает выбор пользователя

    RegCustomers *customer = &customers[ind - 1];
    long baseOffset = sizeof(RegCustomers) * (ind - 1); // вычисляет смещение до начала структуры

    switch (choice)
    { // выбирает поле для изменения на основе ввода пользователя
    case 1:
    {
        printf("введите новое имя: ");
        char name[50];
        input_char(name, sizeof(name));
        fseek(file, baseOffset + offsetof(RegCustomers, name), SEEK_SET); // перемещает указатель файла на нужное место
        int written = fwrite(name, sizeof(name), 1, file);                // записывает новое имя в файл
        if (written != 1)
        {
            perror("ошибка записи в файл");
        }
        strcpy(customer->name, name); // обновляет поле в массиве структур
        break;
    }
    case 2:
    {
        printf("введите новую фамилию: ");
        char surname[50];
        input_char(surname, sizeof(surname));
        fseek(file, baseOffset + offsetof(RegCustomers, surname), SEEK_SET); // перемещает указатель файла на нужное место
        int written = fwrite(surname, sizeof(surname), 1, file);             // записывает новую фамилию в файл
        if (written != 1)
        {
            perror("ошибка записи в файл");
        }
        strcpy(customer->surname, surname); // обновляет поле в массиве структур
        break;
    }
    case 3:
    {
        printf("введите новое отчество: ");
        char fathername[50];
        input_char(fathername, sizeof(fathername));
        fseek(file, baseOffset + offsetof(RegCustomers, fathername), SEEK_SET); // перемещает указатель файла на нужное место
        int written = fwrite(fathername, sizeof(fathername), 1, file);          // записывает новое отчество в файл
        if (written != 1)
        {
            perror("ошибка записи в файл");
        }
        strcpy(customer->fathername, fathername); // обновляет поле в массиве структур
        break;
    }
    case 4:
    {
        printf("введите новый пол (1 для женщины, 2 для мужчины): ");
        short sex = input(1, 2);
        fseek(file, baseOffset + offsetof(RegCustomers, sex), SEEK_SET); // перемещает указатель файла на нужное место
        int written = fwrite(&sex, sizeof(sex), 1, file);                // записывает новый пол в файл
        if (written != 1)
        {
            perror("ошибка записи в файл");
        }
        customer->sex = sex; // обновляет поле в массиве структур
        break;
    }
    case 5:
    {
        printf("введите новый возраст: ");
        int age = input(0, 150);
        fseek(file, baseOffset + offsetof(RegCustomers, age), SEEK_SET); // перемещает указатель файла на нужное место
        int written = fwrite(&age, sizeof(age), 1, file);                // записывает новый возраст в файл
        if (written != 1)
        {
            perror("ошибка записи в файл");
        }
        customer->age = age; // обновляет поле в массиве структур
        break;
    }
    case 6:
    {
        printf("введите новый адрес: ");
        char address[100];
        input_address(address, sizeof(address));
        fseek(file, baseOffset + offsetof(RegCustomers, address), SEEK_SET); // перемещает указатель файла на нужное место
        int written = fwrite(address, sizeof(address), 1, file);             // записывает новый адрес в файл
        if (written != 1)
        {
            perror("ошибка записи в файл");
        }
        strcpy(customer->address, address); // обновляет поле в массиве структур
        break;
    }
    case 7:
    {
        printf("введите новый размер скидки: ");
        float s = Finput(0.0, 100.0);
        int ss = (int)s;
        if (ss == s)
        {
            customer->discount.intV = ss;
            customer->is_float = 0;
            fseek(file, baseOffset + offsetof(RegCustomers, discount), SEEK_SET);                     // перемещает указатель файла на нужное место
            int written = fwrite(&customer->discount.intV, sizeof(customer->discount.intV), 1, file); // записывает новый размер скидки как int
            if (written != 1)
            {
                perror("ошибка записи в файл");
            }
        }
        else
        {
            customer->discount.floatV = s;
            customer->is_float = 1;
            fseek(file, baseOffset + offsetof(RegCustomers, discount), SEEK_SET);                         // перемещает указатель файла на нужное место
            int written = fwrite(&customer->discount.floatV, sizeof(customer->discount.floatV), 1, file); // записывает новый размер скидки как float
            if (written != 1)
            {
                perror("ошибка записи в файл");
            }
        }
        fseek(file, baseOffset + offsetof(RegCustomers, is_float), SEEK_SET);           // перемещает указатель файла на нужное место
        int written = fwrite(&customer->is_float, sizeof(customer->is_float), 1, file); // записывает флаг, указывающий на тип скидки
        if (written != 1)
        {
            perror("ошибка записи в файл");
        }
        break;
    }
    default:
        printf("некорректный выбор\n");
        break;
    }
}

RegCustomers *addCustomer(FILE *file, RegCustomers *customers, int *size) // функция добавления элемента в списка покупателей
{
    /*
    printf("Если вы хотите заполнить какую-либо информацию о клиенте, данными другого клиента введите 1, если нет введите 2.\n");
    int gg = input(0, 3);
    if (input)
    {
        printf("Список покупателей по фамилиям:\n");
        for (int i = 0; i < size; ++i)
        {
            printf("%d. %s\n", i + 1, list[i].surname);
        }
        printf("введите номер клиента из списка, чьи данные вы хотите использовать");
        int number = input(0, size + 1);

        printf("Введите номер данных, которые вы хотите заменить.\t");
        printf("1.Имя/n2.Фамилия/n3.Отчество/n4.Пол/n5.Адресс/n");

        int check = input(0; 6);
    }
    */
    int check = 0;
    *size += 1;
    RegCustomers *help = (RegCustomers *)realloc(customers, (*size) * sizeof(RegCustomers));

    if (customers == NULL && *size > 0)
    {
        printf("Memory allocation failed!\n");
    }
    else
    {
        customers = help;
        help = NULL;
        customers = sort(customers, *size);
    }

    int i = *size - 1;
    printf("Введите информацию про клиента номер %d\n", i + 1);
    // if(check != 1){}
    printf("Введите имя клиента. Если Вы уважаете человека, введите имя с большой буквы. Впрочем, дело Ваше.\n");
    // getchar(); // Убираем '\n' после предыдущего ввода
    input_char(customers[i].name, sizeof(customers[i].name));
    if (strcmp(customers[i].name, "VaDiMoChKa") == 0)
    {
        printf("Вы ввели VaDiMoChKa! Ввод прекращается");
        removeCustomer(file, customers, size, i);
        check = 1;
        return customers;
    }
    else
    {
        check = 0;
    }
    printf("Введите фамилию клиента.\n");
    input_char(customers[i].surname, sizeof(customers[i].surname));
    printf("Введите отчество клиента.\n");
    input_char(customers[i].fathername, sizeof(customers[i].fathername));
    printf("Если ваш клиент женщина введите 1, если мужчина, введите 2\n");
    customers[i].sex = input(0, 3);
    printf("Введите возраст клиента.\n");
    customers[i].age = input(0, 150);
    printf("Введите адрес клиента.\n");
    input_address(customers[i].address, sizeof(customers[i].address));
    printf("Введите размер скидки клиента.\n");
    float s = Finput(0.0, 100.0);
    int ss = s;
    if (ss == s)
    {
        customers[i].discount.intV = s;
        customers[i].is_float = 0;
    }
    else
    {
        customers[i].discount.floatV = s;
        customers[i].is_float = 1;
    }
    customers = sort(customers, *size);
    File_Enter(file, customers, *size);
    return customers;
}

RegCustomers *removeCustomer(FILE *file, RegCustomers *customers, int *size, int index) // удаление элемента из списка
{
    deleteCustomerByIndex(file, index);
    for (int i = index; i < *size - 1; i++)
    {
        customers[i] = customers[i + 1];
    }
    *size -= 1;
    RegCustomers *help = (RegCustomers *)realloc(customers, (*size) * sizeof(RegCustomers));

    if (customers == NULL && *size > 0)
    {
        printf("Memory allocation failed!\n");
    }
    else
    {
        customers = help;
        help = NULL;
        customers = sort(customers, *size);
    }

    return customers;
}
