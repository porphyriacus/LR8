#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "header.h"
#include <curl/curl.h>
#include <json-c/json.h>
#include <ctype.h>
#define BUFFER_SIZE 1024

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
#include <uuid/uuid.h> // Заголовочный файл для libuuid

RegCustomers *createArray(FILE *file, int size) {
    RegCustomers *customers = (RegCustomers *)malloc(size * sizeof(RegCustomers)); // динамически выделяем память
    short check = 0;
    for (int i = 0; i < size; i++) {
        if (check) {
            removeCustomer(file, customers, &size, i);
        } else {
            // Генерация UUID для нового клиента
            uuid_t uuid;
            uuid_generate(uuid);
            uuid_unparse(uuid, customers[i].uuid);

            //printf("Введите информацию про клиента с UUID %s\n", customers[i].uuid);
            printf("Введите имя клиента. Если Вы уважаете человека, введите имя с большой буквы. Впрочем, дело Ваше.\n");
            input_char(customers[i].name, sizeof(customers[i].name));
            if (strcmp(customers[i].name, "VaDiMoChKa") == 0) {
                printf("Вы ввели VaDiMoChKa! Ввод прекращается");
                removeCustomer(file, customers, &size, i);
                check = 1;
                continue;
            } else {
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
                int ss = s;
                if (ss == s) {
                    customers[i].discount.intV = s;
                    customers[i].is_float = 0;
                } else {
                    customers[i].discount.floatV = s;
                    customers[i].is_float = 1;
                }
            }
        }

    }

    customers = sort(customers, size); // сортируем список
    File_Enter(file, customers, size); // сохраняем список в бинарный файл
    fclose(file); // закрытие файла после всех операций

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
void modifyCustomer(FILE *file, RegCustomers *customers, int ind) {
    deleteAllCustomers();
    printf("выберите поле для изменения:\n");
    printf("1. имя\n");
    printf("2. фамилия\n");
    printf("3. отчество\n");
    printf("4. пол\n");
    printf("5. возраст\n");
    printf("6. адрес\n");
    printf("7. скидка\n");
    int choice = input(0, 8); // получает выбор пользователя
    RegCustomers *customer = &customers[ind - 1];
    long baseOffset = sizeof(RegCustomers) * (ind - 1); // вычисляет смещение до начала структуры

    switch (choice) { // выбирает поле для изменения на основе ввода пользователя
    case 1: {
        printf("введите новое имя: ");
        char name[50];
        input_char(name, sizeof(name));
        fseek(file, baseOffset + offsetof(RegCustomers, name), SEEK_SET); // перемещает указатель файла на нужное место
        int written = fwrite(name, sizeof(name), 1, file);                // записывает новое имя в файл
        if (written != 1) {
            perror("ошибка записи в файл");
        }
        strcpy(customer->name, name); // обновляет поле в массиве структур
        break;
    }
    case 2: {
        printf("введите новую фамилию: ");
        char surname[50];
        input_char(surname, sizeof(surname));
        fseek(file, baseOffset + offsetof(RegCustomers, surname), SEEK_SET); // перемещает указатель файла на нужное место
        int written = fwrite(surname, sizeof(surname), 1, file);             // записывает новую фамилию в файл
        if (written != 1) {
            perror("ошибка записи в файл");
        }
        strcpy(customer->surname, surname); // обновляет поле в массиве структур
        break;
    }
    case 3: {
        printf("введите новое отчество: ");
        char fathername[50];
        input_char(fathername, sizeof(fathername));
        fseek(file, baseOffset + offsetof(RegCustomers, fathername), SEEK_SET); // перемещает указатель файла на нужное место
        int written = fwrite(fathername, sizeof(fathername), 1, file);          // записывает новое отчество в файл
        if (written != 1) {
            perror("ошибка записи в файл");
        }
        strcpy(customer->fathername, fathername); // обновляет поле в массиве структур
        break;
    }
    case 4: {
        printf("введите новый пол (1 для женщины, 2 для мужчины): ");
        short sex = input(1, 2);
        fseek(file, baseOffset + offsetof(RegCustomers, sex), SEEK_SET); // перемещает указатель файла на нужное место
        int written = fwrite(&sex, sizeof(sex), 1, file);                // записывает новый пол в файл
        if (written != 1) {
            perror("ошибка записи в файл");
        }
        customer->sex = sex; // обновляет поле в массиве структур
        break;
    }
    case 5: {
        printf("введите новый возраст: ");
        int age = input(0, 150);
        fseek(file, baseOffset + offsetof(RegCustomers, age), SEEK_SET); // перемещает указатель файла на нужное место
        int written = fwrite(&age, sizeof(age), 1, file);                // записывает новый возраст в файл
        if (written != 1) {
            perror("ошибка записи в файл");
        }
        customer->age = age; // обновляет поле в массиве структур
        break;
    }
    case 6: {
        printf("введите новый адрес: ");
        char address[100];
        input_address(address, sizeof(address));
        fseek(file, baseOffset + offsetof(RegCustomers, address), SEEK_SET); // перемещает указатель файла на нужное место
        int written = fwrite(address, sizeof(address), 1, file);             // записывает новый адрес в файл
        if (written != 1) {
            perror("ошибка записи в файл");
        }
        strcpy(customer->address, address); // обновляет поле в массиве структур
        break;
    }
    case 7: {
        printf("введите новый размер скидки: ");
        float s = Finput(0.0, 100.0);
        int ss = (int)s;
        if (ss == s) {
            customer->discount.intV = ss;
            customer->is_float = 0;
            fseek(file, baseOffset + offsetof(RegCustomers, discount), SEEK_SET);                     // перемещает указатель файла на нужное место
            int written = fwrite(&customer->discount.intV, sizeof(customer->discount.intV), 1, file); // записывает новый размер скидки как int
            if (written != 1) {
                perror("ошибка записи в файл");
            }
        } else {
            customer->discount.floatV = s;
            customer->is_float = 1;
            fseek(file, baseOffset + offsetof(RegCustomers, discount), SEEK_SET);                         // перемещает указатель файла на нужное место
            int written = fwrite(&customer->discount.floatV, sizeof(customer->discount.floatV), 1, file); // записывает новый размер скидки как float
            if (written != 1) {
                perror("ошибка записи в файл");
            }
        }
        fseek(file, baseOffset + offsetof(RegCustomers, is_float), SEEK_SET);           // перемещает указатель файла на нужное место
        int written = fwrite(&customer->is_float, sizeof(customer->is_float), 1, file); // записывает флаг, указывающий на тип скидки
        if (written != 1) {
            perror("ошибка записи в файл");
        }
        break;
    }
    default:
        printf("некорректный выбор\n");
        return;
    }
}

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
RegCustomers *addCustomer(FILE *file, RegCustomers *customers, int *size) {
    *size += 1;
    RegCustomers *help = (RegCustomers *)realloc(customers, (*size) * sizeof(RegCustomers));

    if (help == NULL && *size > 0) {
        printf("Memory allocation failed!\n");
        return customers;
    } else {
        customers = help;
        help = NULL;
    }

    int i = *size - 1;

    // Генерация UUID для нового клиента
    uuid_t uuid;
    uuid_generate(uuid);
    uuid_unparse(uuid, customers[i].uuid);

    printf("Введите информацию про клиента с UUID %s\n", customers[i].uuid);

    printf("Введите имя клиента. Если Вы уважаете человека, введите имя с большой буквы. Впрочем, дело Ваше.\n");
    input_char(customers[i].name, sizeof(customers[i].name));
    if (strcmp(customers[i].name, "VaDiMoChKa") == 0) {
        printf("Вы ввели VaDiMoChKa! Ввод прекращается");
        removeCustomer(file, customers, size, i);
        return customers;
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
    if (ss == s) {
        customers[i].discount.intV = s;
        customers[i].is_float = 0;
    } else {
        customers[i].discount.floatV = s;
        customers[i].is_float = 1;
    }

    customers = sort(customers, *size);
    File_Enter(file, customers, *size);
    return customers;
}

RegCustomers *removeCustomer(FILE *file, RegCustomers *customers, int *size, int index) {
    if (index < 0 || index >= *size) {
        printf("Invalid index!\n");
        return customers;
    }
    deleteCustomerByIndex(file, index);
    deleteAllCustomers();
    // Сдвиг оставшихся элементов в массиве
    for (int i = index; i < *size - 1; i++) {
        customers[i] = customers[i + 1];
    }

    *size -= 1;
    RegCustomers *help = (RegCustomers *)realloc(customers, (*size) * sizeof(RegCustomers));

    if (help == NULL && *size > 0) {
        printf("Memory allocation failed!\n");
    } else {
        customers = help;
        help = NULL;
    }
    return customers;
}

RegCustomers *removeCustomerByUUID(FILE* file, RegCustomers *customers, int *size, const char *uuid)
{
    
    // Поиск клиента по UUID и удаление
    for (int i = 0; i < *size; i++)
    {
        if (strcmp(customers[i].uuid, uuid) == 0)
        {   
            deleteCustomerByIndex(file,  i);
            // Сдвиг оставшихся элементов в массиве
            for (int j = i; j < *size - 1; j++)
            {
                customers[j] = customers[j + 1];
            }

            *size -= 1;
            RegCustomers *help = (RegCustomers *)realloc(customers, (*size) * sizeof(RegCustomers));

            if (help == NULL && *size > 0)
            {
                printf("Memory allocation failed!\n");
                return customers;
            }
            else
            {
                customers = help;
            }

            break;
        }
    }

    return customers;
}

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

// функция для ввода целого числа в заданном диапазоне
int input(int a, int b) {
    int value;
    int result = scanf("%d", &value);

    if (result != 1) {
        printf("Ошибка ввода. Пожалуйста, введите число.\n");
        // Очистка потока ввода при ошибке
        while (getchar() != '\n');
        return input(a, b);  // Повторный ввод
    }

    // Проверка, что введенное значение в диапазоне
    if (value < a || value > b) {
        printf("Число должно быть между %d и %d. Попробуйте снова.\n", a, b);
        return input(a, b);  // Повторный ввод
    }

    return value;
}

// функция для ввода числа с плавающей запятой в заданном диапазоне
float Finput(float a, float b)
{
    while (1)
    {
        float n;
        char control;
        while (scanf("%f%c", &n, &control) != 2 || control != '\n')
        {
            printf("Неверный формат ввода.\n");
            while (getchar() != '\n')
                ;
        }
        if ((n > a) && (n < b))
            return n;
        printf("Неверный формат ввода. Попробуйте ещё раз\n");
    }
}

// функция для проверки, является ли символ частью utf-8
static int utf_8(int ch)
{
    int s;
    s = ((ch & 0xC0) != 0x80) ? 1 : 0;
    return s;
}

void input_char(char *name, int size)
{
    int input = 0;
    int check = 0;

    while (!input)
    {
        int ch;
        input = 1;
        check = 0;
        printf("Введите слово: ");

        while (((ch = getchar()) != 10))
        {

            if (((ch >= 65) && (ch <= 90)) || ((ch >= 97) && (ch <= 122)) )
            {
                input = 1;
            }
            else if ((ch >= 0xD0 && ch <= 0xD1))
            {
                int next_ch = getchar();
                if (((ch == 0xD0) && (next_ch >= 0x90 && next_ch <= 0xBF)) || ((ch == 0xD1) && (next_ch >= 0x80 && next_ch <= 0x8F)))
                {
                    name[check] = ch;
                    ++check;
                    name[check] = next_ch;
                    ++check;
                    continue;
                }
                else
                {
                    input = 0;
                    while ((ch = getchar()) != '\n')
                        ;
                    break;
                }
            }
            else
            {
                input = 0;
                while ((ch = getchar()) != '\n')
                    ;
                break;
            }

            name[check] = ch;
            ++check;
        }
      
        if(!((check == 0) && ((ch) == '\n'))){
        name[check] = '\0';}
        else{input = 0;}

        if (!input) // выводим сообщение о некорректном вводе и повторяем ввод
        {
            printf("ФИО могут содержать только буквы. Попробуйте еще раз.\n");
        }
        else // если значение переменной не было изменено, то заканчиваем цикл
        {
            printf("Введенное слово: %s\n", name);
            break;
        }
    }
}

// функция для ввода адреса
void input_address(char *address, int size)
{
    char street[70];           // массив для хранения названия улицы
    char house_number[20];     // массив для хранения номера дома
    char apartment_number[10]; // массив для хранения номера квартиры

    // ввод названия улицы
    int input = 0;
    int check = 0;

    while (!input)
    {
        int ch;
        input = 1;
        check = 0;
        printf("Введите слово: ");
        ;
        while ((ch = getchar()) != '\n')
        {
            if (((ch >= 65) && (ch <= 90)) || ((ch >= 97) && (ch <= 122)) || ((ch == '\n') && (check != 0)))
            {
                input = 1;
            }
            else if ((ch >= 0xD0 && ch <= 0xD1))
            {
                int next_ch = getchar();
                if (((ch == 0xD0) && (next_ch >= 0x90 && next_ch <= 0xBF)) || ((ch == 0xD1) && (next_ch >= 0x80 && next_ch <= 0x8F)))
                {
                    street[check] = ch;
                    ++check;
                    street[check] = next_ch;
                    ++check;
                    continue;
                }
                else
                {
                    input = 0;
                    while ((ch = getchar()) != '\n')
                        ;
                    break;
                }
            }
            else
            {
                input = 0;
                while ((ch = getchar()) != '\n')
                    ;
                break;
            }

            street[check] = ch;
            ++check;
        }
         if(!((check == 0) && ((ch) == '\n'))){
        street[check] = '\0';}
        else{input = 0;}


        if (!input) // выводим сообщение о некорректном вводе и повторяем ввод
        {
            printf("Название улицы может содержать только буквы. Попробуйте еще раз\n");
        }
        else // если значение переменной не было изменено, то заканчиваем цикл
        {

            break;
        }
    }
 input = 0;
   check = 0;
    // ввод номера дома
    while (1)
    {
        printf("Введите номер дома: ");
       

        // удаление символа новой строки, если он есть
        int len = strlen(house_number);
        if (len > 0 && house_number[len - 1] == '\n')
        {
            house_number[len - 1] = '\0';
        }
        int ch;
        // проверка номера дома цифры
        while ((ch = getchar()) != '\n')
        {
            //|| (ch == '/')|| ((ch >= 97) && (ch <= 122))  || ((ch >= 65) && (ch <= 90)
            if (((ch >= 47) && (ch <= 57) ) || ((ch == '\n') && (check != 0)))
            {
                input = 1;
            }
            /*
            else if ((ch >= 0xD0 && ch <= 0xD1))
            {
                int next_ch = getchar();
                if (((ch == 0xD0) && (next_ch >= 0x90 && next_ch <= 0xBF)) || ((ch == 0xD1) && (next_ch >= 0x80 && next_ch <= 0x8F)))
                {
                   house_number[check] = ch;
                    ++check;
                    house_number[check] = next_ch;
                    ++check;
                    continue;
                }
                else
                {
                    input = 0;
                    while ((ch = getchar()) != '\n')
                        ;
                    break;
                }
            }
            */
            else
            {
                input = 0;
                while ((ch = getchar()) != '\n')
                    ;
                break;
            }

            house_number[check] = ch;
            ++check;
        }
         if(!((check == 0) && ((ch) == '\n'))){
        house_number[check] = '\0';}
        else{input = 0;}


        if (!input) // выводим сообщение о некорректном вводе и повторяем ввод
        {
            printf("Номер дома может содержать только цифры. Попробуйте еще раз\n");
        }
        else // если значение переменной не было изменено, то заканчиваем цикл
        {

            break;
        }
    }
    input = 0;
    check = 0;
    while(1){
           printf("Введите номер квартиры: ");
       
        // удаление символа новой строки, если он есть
        int len = strlen(house_number);
        if (len > 0 && house_number[len - 1] == '\n')
        {
            apartment_number[len - 1] = '\0';
        }
        int ch;
        // проверка номера дома цифры
        while ((ch = getchar()) != '\n')
        {
            if ( ((ch >= 48) && (ch <= 57)))
            {
                input = 1;
            }
            else{
                input = 0;
                while ((ch = getchar()) != '\n')
                    ;
                break;
            }

            apartment_number[check] = ch;
            ++check;
        }
         if(!((check == 0) && ((ch) == '\n'))){
        apartment_number[check] = '\0';}
        else{input = 0;}


        if (!input) // выводим сообщение о некорректном вводе и повторяем ввод
        {
            printf("Номер квартиры может содержать только цифры. Попробуйте еще раз\n");
        }
        else // если значение переменной не было изменено, то заканчиваем цикл
        {

            break;
        }
    }
   

    // объединение всех частей адреса в одну строку
    snprintf(address, size, "ул. %s, д. %s, кв. %s ", street, house_number, apartment_number);
}

void menu(){
        printf("В магазине сформирован список постоянных клиентов,\n"
               "который включает ФИО, пол, возраст, домашний адрес покупателя и размер предоставляемой скидки.\n\n");
        printf("Выберите, что бы вы хотели сделать:\n(Введите номер девийствия, после нажмите ENTER)\n");
        printf("1. Создать новый список постоянных клиентов. Учтите, что старый список(при его наличии) будет удален\n");
        printf("2. Просмотреть список всех постоянных клиентов\n");
        printf("3. Добавить постоянного клиента в список\n");
        printf("4. Найти постоянного клиента по признаку\n");
        printf("5. Удалить постоянного клиента из списка\n");
        printf("6. Изменить информацию о постоянном клиенте\n");
        printf("7. Завершить программу\n");
        fflush(stdout); // очищает буффер чтоб точно сразу выводилось
}
//#include "header.h"

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


size_t WriteCallback(void *contents, size_t size, size_t nmemb, void *userp) {
    size_t totalSize = size * nmemb;
    strncat((char *)userp, (char *)contents, totalSize);
    return totalSize;
}
void customerToJson(RegCustomers *customer, char *jsonBuffer) {
    float discountValue;

    if (customer->is_float) {
        discountValue = customer->discount.floatV;
    } else {
        discountValue = (float)customer->discount.intV;
    }

    snprintf(jsonBuffer, BUFFER_SIZE, 
        "{\"uuid\":\"%s\",\"name\":\"%s\",\"surname\":\"%s\",\"fathername\":\"%s\",\"sex\":%d,\"age\":%d,\"address\":\"%s\",\"discount\":%.2f}", 
        customer->uuid, customer->name, customer->surname, customer->fathername, customer->sex, customer->age, customer->address, discountValue);
}
void sendToElasticSearch(char *jsonBuffer) {
    CURL *curl;
    CURLcode res;
    char buffer[BUFFER_SIZE] = {0};  // Буфер для хранения ответа сервера

    curl_global_init(CURL_GLOBAL_DEFAULT);
    curl = curl_easy_init();

    if(curl) {
        curl_easy_setopt(curl, CURLOPT_URL, "https://localhost:9200/customers_index/_doc/");
        curl_easy_setopt(curl, CURLOPT_USERPWD, "elastic:JpOhGLIUvjwBMhvpgZUY");
        curl_easy_setopt(curl, CURLOPT_POSTFIELDS, jsonBuffer);

        // Игнорирование проверки SSL сертификатов
        curl_easy_setopt(curl, CURLOPT_SSL_VERIFYPEER, 0L);
        curl_easy_setopt(curl, CURLOPT_SSL_VERIFYHOST, 0L);

        struct curl_slist *headers = NULL;
        headers = curl_slist_append(headers, "Content-Type: application/json");
        curl_easy_setopt(curl, CURLOPT_HTTPHEADER, headers);

        // Устанавливаем функцию обратного вызова и буфер для записи данных
        curl_easy_setopt(curl, CURLOPT_WRITEFUNCTION, WriteCallback);
        curl_easy_setopt(curl, CURLOPT_WRITEDATA, (void *)buffer);

        res = curl_easy_perform(curl);
        if(res != CURLE_OK) {
            fprintf(stderr, "curl_easy_perform() failed: %s\n", curl_easy_strerror(res));
        }

        curl_slist_free_all(headers);
        curl_easy_cleanup(curl);
    }

    curl_global_cleanup();
}
void deleteAllCustomers() {
    CURL *curl;
    CURLcode res;
    char buffer[BUFFER_SIZE] = {0};  // Буфер для хранения ответа сервера

    curl_global_init(CURL_GLOBAL_DEFAULT);
    curl = curl_easy_init();

    if(curl) {
        curl_easy_setopt(curl, CURLOPT_URL, "https://localhost:9200/customers_index/_delete_by_query");
        curl_easy_setopt(curl, CURLOPT_USERPWD, "elastic:JpOhGLIUvjwBMhvpgZUY");
        curl_easy_setopt(curl, CURLOPT_CUSTOMREQUEST, "POST");

        const char *json_query = "{\"query\": {\"match_all\": {}}}";
        curl_easy_setopt(curl, CURLOPT_POSTFIELDS, json_query);

        // Игнорирование проверки SSL сертификатов
        curl_easy_setopt(curl, CURLOPT_SSL_VERIFYPEER, 0L);
        curl_easy_setopt(curl, CURLOPT_SSL_VERIFYHOST, 0L);

        struct curl_slist *headers = NULL;
        headers = curl_slist_append(headers, "Content-Type: application/json");
        curl_easy_setopt(curl, CURLOPT_HTTPHEADER, headers);

        // Устанавливаем функцию обратного вызова и буфер для записи данных
        curl_easy_setopt(curl, CURLOPT_WRITEFUNCTION, WriteCallback);
        curl_easy_setopt(curl, CURLOPT_WRITEDATA, (void *)buffer);

        res = curl_easy_perform(curl);
        if(res != CURLE_OK) {
            fprintf(stderr, "curl_easy_perform() failed: %s\n", curl_easy_strerror(res));
        } else {
            //printf("Записи успешно удалены.\n");
        }

        curl_slist_free_all(headers);
        curl_easy_cleanup(curl);
    }

    curl_global_cleanup();
}
#include <ctype.h>
#include <ctype.h>
#include <ctype.h>

int is_numeric_query(const char *query) {
    while (*query) {
        if (!isdigit(*query) && *query != '-') {
            return 0; // Ненумерический символ найден
        }
        query++;
    }
    return 1; // Все символы числовые или тире
}

void searchInElasticSearch(const char *query) {
    CURL *curl;
    CURLcode res;

    curl_global_init(CURL_GLOBAL_DEFAULT);
    curl = curl_easy_init();

    if (curl) {
        char url[256];
        snprintf(url, sizeof(url), "https://localhost:9200/customers_index/_search");

        curl_easy_setopt(curl, CURLOPT_URL, url);
        curl_easy_setopt(curl, CURLOPT_USERPWD, "elastic:JpOhGLIUvjwBMhvpgZUY");
        curl_easy_setopt(curl, CURLOPT_WRITEFUNCTION, WriteCallback);

        char response[BUFFER_SIZE] = {0};
        curl_easy_setopt(curl, CURLOPT_WRITEDATA, response);

        // Определение, является ли ввод числом или текстом
        double from, to;
        char json_query[BUFFER_SIZE];

        if (is_numeric_query(query) && sscanf(query, "%lf-%lf", &from, &to) == 2) {
            // Формат JSON для запроса range с включением первого и последнего значений и обработкой дробных значений
            snprintf(json_query, sizeof(json_query), 
                "{\"query\": {\"bool\": {\"should\": [{\"range\": {\"age\": {\"gte\": %d, \"lte\": %d}}}, {\"range\": {\"discount\": {\"gte\": %.2f, \"lte\": %.2f}}}]}}}", 
                (int)from, (int)to, from, to);
        } else if (is_numeric_query(query) && sscanf(query, "%lf", &from) == 1) {
            // Формат JSON для запроса range с одним значением
            snprintf(json_query, sizeof(json_query), 
                "{\"query\": {\"bool\": {\"should\": [{\"range\": {\"age\": {\"gte\": %d}}}, {\"range\": {\"discount\": {\"gte\": %.2f}}}]}}}", 
                (int)from, from);
        } else {
            // Формат JSON для запроса multi_match по текстовым полям
            snprintf(json_query, sizeof(json_query), 
                "{\"query\": {\"multi_match\": {\"query\": \"%s\", \"fields\": [\"name\", \"surname\", \"fathername\", \"address\"]}}}", 
                query);
        }

        curl_easy_setopt(curl, CURLOPT_POSTFIELDS, json_query);
        curl_easy_setopt(curl, CURLOPT_CUSTOMREQUEST, "POST");

        curl_easy_setopt(curl, CURLOPT_SSL_VERIFYPEER, 0L);
        curl_easy_setopt(curl, CURLOPT_SSL_VERIFYHOST, 0L);

        struct curl_slist *headers = NULL;
        headers = curl_slist_append(headers, "Content-Type: application/json");
        curl_easy_setopt(curl, CURLOPT_HTTPHEADER, headers);

        res = curl_easy_perform(curl);
        if (res != CURLE_OK) {
            fprintf(stderr, "curl_easy_perform() failed: %s\n", curl_easy_strerror(res));
        } else {
            struct json_object *parsed_json;
            struct json_object *hits_obj;
            struct json_object *hits_arr;
            struct json_object *total_obj;
            struct json_object *hit;
            struct json_object *source_obj;

            parsed_json = json_tokener_parse(response);
            if (parsed_json == NULL) {
                printf("Ошибка парсинга JSON ответа: Неверный формат JSON.\n");
                return;
            }
            
            if (json_object_object_get_ex(parsed_json, "hits", &hits_obj) &&
                json_object_object_get_ex(hits_obj, "total", &total_obj)) {
                struct json_object *total_value;
                if (json_object_object_get_ex(total_obj, "value", &total_value)) {
                    int total_hits = json_object_get_int(total_value);
                    if (total_hits > 0) {
                        printf("Найдено клиентов: %d\n\n", total_hits);

                        json_object_object_get_ex(hits_obj, "hits", &hits_arr);
                        for (int i = 0; i < total_hits; i++) {
                            hit = json_object_array_get_idx(hits_arr, i);
                            json_object_object_get_ex(hit, "_source", &source_obj);

                            struct json_object *uuid;
                            struct json_object *name;
                            struct json_object *surname;
                            struct json_object *fathername;
                            struct json_object *sex;
                            struct json_object *age;
                            struct json_object *address;
                            struct json_object *discount;

                            json_object_object_get_ex(source_obj, "uuid", &uuid);
                            json_object_object_get_ex(source_obj, "name", &name);
                            json_object_object_get_ex(source_obj, "surname", &surname);
                            json_object_object_get_ex(source_obj, "fathername", &fathername);
                            json_object_object_get_ex(source_obj, "sex", &sex);
                            json_object_object_get_ex(source_obj, "age", &age);
                            json_object_object_get_ex(source_obj, "address", &address);
                            json_object_object_get_ex(source_obj, "discount", &discount);

                            printf("Клиент №%d:\n", i + 1);
                            //printf("UUID: %s\n", json_object_get_string(uuid));
                            printf("Имя: %s\n", json_object_get_string(name));
                            printf("Фамилия: %s\n", json_object_get_string(surname));
                            printf("Отчество: %s\n", json_object_get_string(fathername));
                            printf("Пол: %d\n", json_object_get_int(sex));
                            printf("Возраст: %d\n", json_object_get_int(age));
                            printf("Адрес: %s\n", json_object_get_string(address));
                            printf("Скидка: %.2f\n", json_object_get_double(discount));
                            printf("\n");
                        }
                    } else {
                        printf("Клиенты не найдены.\n");
                    }
                } else {
                    printf("Ошибка парсинга поля 'value' в 'total'.\n");
                }
            } else {
                printf("Ошибка парсинга JSON ответа.\n");
            }

            json_object_put(parsed_json);
        }

        curl_slist_free_all(headers);
        curl_easy_cleanup(curl);
    }

    curl_global_cleanup();
}


void uploadAllCustomers(RegCustomers *list, int numb) {
    for (int i = 0; i < numb; i++) {
        char jsonBuffer[BUFFER_SIZE];
        RegCustomersToJson(&list[i], jsonBuffer);
        sendToElasticSearch(jsonBuffer);
    }
}
void RegCustomersToJson(RegCustomers *customer, char *jsonBuffer) {
    float discountValue;

    if (customer->is_float) {
        discountValue = customer->discount.floatV;
    } else {
        discountValue = (float)customer->discount.intV;
    }

    snprintf(jsonBuffer, BUFFER_SIZE, 
        "{\"uuid\":\"%s\",\"name\":\"%s\",\"surname\":\"%s\",\"fathername\":\"%s\",\"sex\":%d,\"age\":%d,\"address\":\"%s\",\"discount\":%.2f}", 
        customer->uuid, customer->name, customer->surname, customer->fathername, customer->sex, customer->age, customer->address, discountValue);
}
