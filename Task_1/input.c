#include <stdio.h>
#include <string.h>
#include <ctype.h>

// функция для ввода целого числа в заданном диапазоне
int input(int a, int b)
{
    while (1)
    {
        int n;
        char control;
        while (scanf("%d%c", &n, &control) != 2 || control != '\n')
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

