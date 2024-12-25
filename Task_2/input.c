#include <stdio.h>
#include <string.h>
#include <ctype.h>

// Функция для ввода целого числа в заданном диапазоне
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

// Функция для ввода числа с плавающей запятой в заданном диапазоне
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
            printf("\n");
        }
        else // если значение переменной не было изменено, то заканчиваем цикл
        {
            printf("Ваш ввод: %s\n", name);
            break;
        }
    }
}


void input_numb(char *name, int size)
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

            if (((ch > 47) && (ch < 58)) )
            {
                input = 1;
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
            printf("Некорректный ввод. Вводить можно только числа.\n");
        }
        else // если значение переменной не было изменено, то заканчиваем цикл
        {
            printf("Ваш ввод: %s\n", name);
            break;
        }
    }
}


// функция для ввода адреса
void input_date(char *date, int size)
{      
    int day;     
    int month; 
    int year;   

    printf("Введите день:\t");
    day = input(0, 32);
    printf("Введите месяц:\t");
    month = input(0, 13);
    printf("Введите год(oт 1900 до 2024 включительно):\t");
    year = input(1899, 2025);
    // объединение всех частей адреса в одну строку
    snprintf(date, size, "%d.%d.%d", day, month, year);
}

