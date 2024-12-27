#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <ctype.h>
#include "header.h"
#define MAX_UNIQUE_CITIES 100
#define NUM_CALLS 100 // Предположительное количество звонков
#include <curl/curl.h>
#include <json-c/json.h>
#define BUFFER_SIZE 1024


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

void menu(){
        printf("В магазине сформирован список звонков,\n"
               "который включает дату, код города, город, время разговора, тариф, номер телефона.\n\n");
        printf("Выберите, что бы вы хотели сделать:\n(Введите номер девийствия, после нажмите ENTER)\n");
        printf("1. Создать новый список звонков. Учтите, что старый список(при его наличии) будет удален\n");
        printf("2. Просмотреть список всех звонков\n");
        printf("3. Добавить звонок в список\n");
        printf("4. Найти информацию о звонке по признаку\n");
        printf("5. Удалить звонок из списка\n");
        printf("6. Изменить данные звонка.\n");
        printf("7. Вывести по каждому городу общее время разговоров с абонентом(по номеру) и стоимость разговором по каждому городу.\n");
        printf("8. Завершить программу\n");
        fflush(stdout); // очищает буффер чтоб точно сразу выводилось
}


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
            if (((ch >= 65) && (ch <= 90)) || ((ch >= 97) && (ch <= 122)))
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

        if (!((check == 0) && ((ch) == '\n')))
        {
            name[check] = '\0';
        }
        else
        {
            input = 0;
        }

        if (!input)
        {
            printf("\n");
        }
        else
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
            if (((ch > 47) && (ch < 58)))
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

        if (!((check == 0) && ((ch) == '\n')))
        {
            name[check] = '\0';
        }
        else
        {
            input = 0;
        }

        if (!input)
        {
            printf("Некорректный ввод. Вводить можно только числа.\n");
        }
        else
        {
            printf("Ваш ввод: %s\n", name);
            break;
        }
    }
}

void input_date(int *day, int *month, int *year)
{
    printf("Введите день:\t");
    *day = input(0, 32);
    printf("Введите месяц:\t");
    *month = input(0, 13);
    printf("Введите год(oт 1900 до 2024 включительно):\t");
    *year = input(1899, 2025);
}


// Функция для записи клиента в файл
void File_Enter(const char *filename, calls *call, int size) {
    FILE* file = fopen(filename, "w+");
    if (file != NULL) {
        for (int i = 0; i < size; i++) {
            fprintf(file, "Дата разговора: %02d.%02d.%04d\n", call[i].day, call[i].month, call[i].year);
            fprintf(file, "Код города: %s\n", call[i].code);
            fprintf(file, "Город: %s\n", call[i].city);
            fprintf(file, "Время разговора: %.2f\n", call[i].time);
            if (call[i].is_float) {
                fprintf(file, "Тариф: %.2f\n", call[i].tarif.floatV);
            } else {
                fprintf(file, "Тариф: %d\n", call[i].tarif.intV);
            }
            fprintf(file, "Номер города: %s\n", call[i].PhoneNumber);
            fprintf(file, "Номер абонента: %s\n", call[i].PhoneAbonents);
            fprintf(file, "\n"); // Разделение записей пустой строкой
        }
        fclose(file);
    } else {
        perror("Ошибка записи в файл");
    }
}

// Функция для подсчета количества записей в файле
static int countRecordsInFile(const char *filename) {
    FILE *file = fopen(filename, "r");
    if (file == NULL) {
        perror("Ошибка открытия файла для чтения");
        return -1;
    }

    int count = 0;
    char line[256];
    while (fgets(line, sizeof(line), file) != NULL) {
        if (strncmp(line, "Дата разговора:", 15) == 0) {
            count++;
        }
    }

    fclose(file);
    return count;
}

// Функция для чтения данных из файла и создания массива структур
calls* readCallsFromFile(calls *call, const char *filename, int *size) {
    FILE *file = fopen(filename, "r+");
    if (file == NULL) {
        perror("Ошибка открытия файла для чтения");
        return NULL;
    }
    int count = countRecordsInFile(filename);
    call = malloc(count * sizeof(calls));
    if (call == NULL) {
        perror("Ошибка выделения памяти");
        fclose(file);
        return NULL;
    }

    for (int i = 0; i < count; i++) {
        fscanf(file, "Дата разговора: %d.%d.%d\n", &call[i].day, &call[i].month, &call[i].year);
        fscanf(file, "Код города: %s\n", call[i].code);
        fscanf(file, "Город: %s\n", call[i].city);
        fscanf(file, "Время разговора: %f\n", &call[i].time);
        char tarifType[10];
        fscanf(file, "Тариф: %s\n", tarifType);
        if (strchr(tarifType, '.') != NULL) {
            call[i].is_float = 1;
            call[i].tarif.floatV = atof(tarifType);
        } else {
            call[i].is_float = 0;
            call[i].tarif.intV = atoi(tarifType);
        }
        fscanf(file, "Номер города: %s\n", call[i].PhoneNumber);
        fscanf(file, "Номер абонента: %s\n", call[i].PhoneAbonents);
        fscanf(file, "\n"); // Пропуск пустой строки между записями
    }
    *size = count;
    fclose(file);
    return call;
}


size_t WriteCallback(void *contents, size_t size, size_t nmemb, void *userp)
{
    size_t totalSize = size * nmemb;
    strncat((char *)userp, (char *)contents, totalSize);
    return totalSize;
}

// Функция для преобразования данных клиента в JSON
void callToJson(calls *call, char *jsonBuffer)
{
    float tarifValue; // тк шаблон отправленный на сервер эластик не разделяет скидку на цилочисленную и с плавающей запятой то пробразуем все к формату float

    if (call->is_float)
    {
        tarifValue = call->tarif.floatV;
    }
    else
    {
        tarifValue = (float)call->tarif.intV;
    }
    // создание форматированной строки с вводом в наш заданный буфер
    // .2f : 2 знака после запятой потому что мы преоьразуем целочисленное в число с плавающей точкой
    //       (+ для удобства хранения и поиска, ну и тариф вряд ли будет содержать больше 1 знака)
    snprintf(jsonBuffer, BUFFER_SIZE,
             "{\"day\":%d,\"month\":%d,\"year\":%d,\"code\":\"%s\",\"city\":\"%s\",\"time\":%.2f,\"tarif\":%.2f,\"PhoneNumber\":\"%s\",\"PhoneAbonents\":\"%s\"}",
             call->day, call->month, call->year, call->code, call->city, call->time, tarifValue, call->PhoneNumber, call->PhoneAbonents);
}

// Функция для отправки данных в Elasticsearch
void sendToElasticSearch(char *jsonBuffer)
{
    CURL *curl;                     // CURL это тип данных, обохначающий дескрипторр сеанса HTTP запроса
                                    // CURL * — это указатель на структуру данных, которая управляется библиотекой libcurl
                                    // структ содержит всю информацию, необходимую для выполнения HTTP-запроса (URL, заголовки, тело запроса..)
    CURLcode res;                   // перечислительный тип данных(халоу enum)
    char buffer[BUFFER_SIZE] = {0}; // Буфер для хранения ответа сервера

    curl_global_init(CURL_GLOBAL_DEFAULT); // функция инициализирует библиотеку libcurl; должна быть вызвана один раз в начале использования библиотеки
    curl = curl_easy_init();               // curl_easy_init создает новый сеанс и возвращает указатель на объект CURL, который является дескриптором сеанса

    if (curl) // проверка успешно ли создан обьект(если создан неуспешно то возвращается NULL)
    {
        // curl_easy_setopt используется для настройки различных параметров сеанса, таких как URL для запроса
        //  1 аргумент : указатель на дескриптор сеанса CURL
        //  2 аргумент : параметр или опция, которую хотим установить
        //  CURLPT_URL: опция, указывающая URL, на который будет отправлен HTTP-запрос
        //  CURLOPT_USERPWD: опция, указывающая учетные данные для базовой HTTP-аутентификации
        //  CURLOPT_POSTFIELDS: опция, указывающая данные для HTTP POST-запроса
        // 3 аргумент : значение, которое хотим присвоить этой опции
        curl_easy_setopt(curl, CURLOPT_URL, "https://localhost:9200/calls_index/_doc/"); // устанавливает URL, на который будет отправлен HTTP-запрос
        curl_easy_setopt(curl, CURLOPT_USERPWD, "elastic:JpOhGLIUvjwBMhvpgZUY");         // устанавливает учетные данные для базовой HTTP-авторизации
        curl_easy_setopt(curl, CURLOPT_POSTFIELDS, jsonBuffer);                          //  устанавливает данные для HTTP POST-запроса

        // Игнорирование проверки SSL сертификатов
        curl_easy_setopt(curl, CURLOPT_SSL_VERIFYPEER, 0L);
        curl_easy_setopt(curl, CURLOPT_SSL_VERIFYHOST, 0L);

        // устанавливает заголовок HTTP-запроса, указывая, что данные отправляются в формате JSON
        struct curl_slist *headers = NULL;
        headers = curl_slist_append(headers, "Content-Type: application/json");
        curl_easy_setopt(curl, CURLOPT_HTTPHEADER, headers);

        // Устанавливаем функцию обратного вызова и буфер для записи данных
        curl_easy_setopt(curl, CURLOPT_WRITEFUNCTION, WriteCallback);
        curl_easy_setopt(curl, CURLOPT_WRITEDATA, (void *)buffer);

        res = curl_easy_perform(curl); // curl_easy_perform выполняет HTTP-запрос. Если запрос не удался, выводится сообщение об ошибке
        if (res != CURLE_OK)
        {
            fprintf(stderr, "curl_easy_perform() failed: %s\n", curl_easy_strerror(res));
        }
        else
        {
            // Обработка ответа сервера (если необходимо)
        }

        // функции ниже освобождают выделенные ресурсы и очищают CURL-объект
        curl_slist_free_all(headers);
        curl_easy_cleanup(curl);
    }

    curl_global_cleanup(); // функция завершает работу с библиотекой libcurl и освобождает глобальные ресурсы, выделенные curl_global_init
}

void deleteAllcalls()
{
    CURL *curl;
    CURLcode res;
    char buffer[BUFFER_SIZE] = {0}; // Буфер для хранения ответа сервера

    curl_global_init(CURL_GLOBAL_DEFAULT);
    curl = curl_easy_init();

    if (curl)
    {
        curl_easy_setopt(curl, CURLOPT_URL, "https://localhost:9200/calls_index/_delete_by_query");
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
        if (res != CURLE_OK)
        {
            fprintf(stderr, "curl_easy_perform() failed: %s\n", curl_easy_strerror(res));
        }
        else
        {
            // Обработка ответа
            // printf("Записи успешно удалены.\n");
        }

        curl_slist_free_all(headers);
        curl_easy_cleanup(curl);
    }

    curl_global_cleanup();
}

// Функция для загрузки всех клиентов на сервер
void uploadAllcalls(calls *list, int numb)
{
    for (int i = 0; i < numb; i++)
    {
        char jsonBuffer[BUFFER_SIZE];
        callToJson(&list[i], jsonBuffer);
        sendToElasticSearch(jsonBuffer);
    }
}
#include <ctype.h>

int is_numeric_query(const char *query)
{
    while (*query)
    {
        if (!isdigit(*query) && *query != '-')
        {
            return 0; // Ненумерический символ найден
        }
        query++;
    }
    return 1; // Все символы числовые или тире
}

void searchInElasticSearch(const char *query)
{
    CURL *curl;
    CURLcode res;

    curl_global_init(CURL_GLOBAL_DEFAULT);
    curl = curl_easy_init();

    if (curl)
    {
        char url[256];
        snprintf(url, sizeof(url), "https://localhost:9200/calls_index/_search");

        curl_easy_setopt(curl, CURLOPT_URL, url);
        curl_easy_setopt(curl, CURLOPT_USERPWD, "elastic:JpOhGLIUvjwBMhvpgZUY");
        curl_easy_setopt(curl, CURLOPT_WRITEFUNCTION, WriteCallback);

        char response[BUFFER_SIZE] = {0};
        curl_easy_setopt(curl, CURLOPT_WRITEDATA, response);

        // Определение, является ли ввод числом или текстом
        double from, to;
        char json_query[BUFFER_SIZE];

        if (is_numeric_query(query) && sscanf(query, "%lf-%lf", &from, &to) == 2)
        {
            // Формат JSON для запроса range с включением первого и последнего значений и обработкой дробных значений
            snprintf(json_query, sizeof(json_query),
                     "{\"query\": {\"bool\": {\"should\": [{\"range\": {\"time\": {\"gte\": %.2f, \"lte\": %.2f}}}, {\"range\": {\"tarif\": {\"gte\": %.2f, \"lte\": %.2f}}}, {\"range\": {\"day\": {\"gte\": %d, \"lte\": %d}}}, {\"range\": {\"month\": {\"gte\": %d, \"lte\": %d}}}, {\"range\": {\"year\": {\"gte\": %d, \"lte\": %d}}}]}}}",
                     from, to, from, to, (int)from, (int)to, (int)from, (int)to, (int)from, (int)to);
        }
        else if (is_numeric_query(query) && sscanf(query, "%lf", &from) == 1)
        {
            // Формат JSON для запроса range с одним значением
            snprintf(json_query, sizeof(json_query),
                     "{\"query\": {\"bool\": {\"should\": [{\"range\": {\"time\": {\"gte\": %.2f}}}, {\"range\": {\"tarif\": {\"gte\": %.2f}}}, {\"range\": {\"day\": {\"gte\": %d}}}, {\"range\": {\"month\": {\"gte\": %d}}}, {\"range\": {\"year\": {\"gte\": %d}}}]}}}",
                     from, from, (int)from, (int)from, (int)from);
        }
        else
        {
            // Формат JSON для запроса multi_match по текстовым полям
            snprintf(json_query, sizeof(json_query),
                     "{\"query\": {\"multi_match\": {\"query\": \"%s\", \"fields\": [\"code\", \"city\", \"PhoneNumber\", \"PhoneAbonents\"]}}}",
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
        if (res != CURLE_OK)
        {
            fprintf(stderr, "curl_easy_perform() failed: %s\n", curl_easy_strerror(res));
        }
        else
        {
            struct json_object *parsed_json;
            struct json_object *hits_obj;
            struct json_object *hits_arr;
            struct json_object *total_obj;
            struct json_object *hit;
            struct json_object *source_obj;

            parsed_json = json_tokener_parse(response);
            if (parsed_json == NULL)
            {
                printf("Ошибка парсинга JSON ответа: Неверный формат JSON.\n");
                return;
            }

            if (json_object_object_get_ex(parsed_json, "hits", &hits_obj) &&
                json_object_object_get_ex(hits_obj, "total", &total_obj))
            {
                struct json_object *total_value;
                if (json_object_object_get_ex(total_obj, "value", &total_value))
                {
                    int total_hits = json_object_get_int(total_value);
                    if (total_hits > 0)
                    {
                        printf("Найдено клиентов: %d\n\n", total_hits);

                        json_object_object_get_ex(hits_obj, "hits", &hits_arr);
                        for (int i = 0; i < total_hits; i++)
                        {
                            hit = json_object_array_get_idx(hits_arr, i);
                            json_object_object_get_ex(hit, "_source", &source_obj);

                            struct json_object *day;
                            struct json_object *month;
                            struct json_object *year;
                            struct json_object *code;
                            struct json_object *city;
                            struct json_object *time;
                            struct json_object *tarif;
                            struct json_object *PhoneNumber;
                            struct json_object *PhoneAbonents;

                            json_object_object_get_ex(source_obj, "day", &day);
                            json_object_object_get_ex(source_obj, "month", &month);
                            json_object_object_get_ex(source_obj, "year", &year);
                            json_object_object_get_ex(source_obj, "code", &code);
                            json_object_object_get_ex(source_obj, "city", &city);
                            json_object_object_get_ex(source_obj, "time", &time);
                            json_object_object_get_ex(source_obj, "tarif", &tarif);
                            json_object_object_get_ex(source_obj, "PhoneNumber", &PhoneNumber);
                            json_object_object_get_ex(source_obj, "PhoneAbonents", &PhoneAbonents);

                            printf("Звонок №%d:\n", i + 1);
                            printf("Дата: %02d.%02d.%04d\n", json_object_get_int(day), json_object_get_int(month), json_object_get_int(year));
                            printf("Код города: %s\n", json_object_get_string(code));
                            printf("Город: %s\n", json_object_get_string(city));
                            printf("Время: %s\n", json_object_get_string(time));
                            printf("Тариф: %.2f\n", json_object_get_double(tarif));
                            printf("Номер города: %s\n", json_object_get_string(PhoneNumber));
                            printf("Номер абонента: %s\n", json_object_get_string(PhoneAbonents));
                            printf("\n");
                        }
                    }
                    else
                    {
                        printf("Звонки не найдены.\n");
                    }
                }
                else
                {
                    printf("Ошибка парсинга поля 'value' в 'total'.\n");
                }
            }
            else
            {
                printf("Ошибка парсинга JSON ответа.\n");
            }

            json_object_put(parsed_json);
        }

        curl_slist_free_all(headers);
        curl_easy_cleanup(curl);
    }

    curl_global_cleanup();
}

// Функция для записи данных в JSON
void writecallsToJson(calls *calls, int size, const char *filename)
{
    FILE *file = fopen(filename, "w");
    if (file == NULL)
    {
        printf("Could not open file for writing.\n");
        return;
    }

    fprintf(file, "[\n");
    for (int i = 0; i < size; i++)
    {
        fprintf(file, "  {\n");
        fprintf(file, "    \"day\": %d,\n", calls[i].day);
        fprintf(file, "    \"month\": %d,\n", calls[i].month);
        fprintf(file, "    \"year\": %d,\n", calls[i].year);
        fprintf(file, "    \"code\": \"%s\",\n", calls[i].code);
        fprintf(file, "    \"city\": \"%s\",\n", calls[i].city);
        fprintf(file, "    \"time\": %.2f,\n", calls[i].time);
        if (calls[i].is_float)
        {
            fprintf(file, "    \"tarif\": %.2f,\n", calls[i].tarif.floatV);
        }
        else
        {
            fprintf(file, "    \"tarif\": %d,\n", calls[i].tarif.intV);
        }
        fprintf(file, "    \"PhoneNumber\": \"%s\",\n", calls[i].PhoneNumber);
        fprintf(file, "    \"PhoneAbonents\": \"%s\"\n", calls[i].PhoneAbonents);
        if (i == size - 1)
        {
            fprintf(file, "  }\n");
        }
        else
        {
            fprintf(file, "  },\n");
        }
    }
    fprintf(file, "]\n");

    fclose(file);
}
calls *addCall(const char *filename, calls *, int *);
calls *removeCall(const char *filename, calls *, int *, int);
void modifyCalls(const char *filename, calls *, int);
calls *createArray(const char *filename, int);
void calculateTotals(calls *call, int);

typedef struct {
    char city[20];
    char code[20];
} CityCode;

CityCode uniqueCities[MAX_UNIQUE_CITIES];
int uniqueCityCount = 0;

const char* getCityCode(const char *city) {
    for (int i = 0; i < uniqueCityCount; i++) {
        if (strcmp(uniqueCities[i].city, city) == 0) {
            return uniqueCities[i].code;
        }
    }
    return NULL;
}

void addCityCode(const char *city, const char *code) {
    if (uniqueCityCount < MAX_UNIQUE_CITIES) {
        strcpy(uniqueCities[uniqueCityCount].city, city);
        strcpy(uniqueCities[uniqueCityCount].code, code);
        uniqueCityCount++;
    } else {
        printf("Error: Maximum number of unique cities reached.\n");
    }
}

void inputCityAndCode(calls *call) {
    printf("Введите город:\n");
    input_char(call->city, sizeof(call->city));

    const char *existingCode = getCityCode(call->city);
    if (existingCode) {
        strcpy(call->code, existingCode);
    } else {
        printf("Введите код города (состоит из чисел):\n");
        input_numb(call->code, sizeof(call->code));
        addCityCode(call->city, call->code);
    }
}

void createCityCodeArray(calls *callList, int size) {
    uniqueCityCount = 0;
    for (int i = 0; i < size; i++) {
        int cityExists = 0;
        for (int j = 0; j < uniqueCityCount; j++) {
            if (strcmp(callList[i].city, uniqueCities[j].city) == 0) {
                cityExists = 1;
                break;
            }
        }
        if (!cityExists && uniqueCityCount < MAX_UNIQUE_CITIES) {
            strcpy(uniqueCities[uniqueCityCount].city, callList[i].city);
            strcpy(uniqueCities[uniqueCityCount].code, callList[i].code);
            uniqueCityCount++;
        }
    }
}

calls* createArray(const char *filename, int size) {
    FILE *file = fopen(filename, "r+");
    if (file == NULL) {
        perror("Ошибка открытия файла");
        return NULL;
    }

    calls *call = (calls *)malloc(size * sizeof(calls));
    if (call == NULL) {
        perror("Ошибка выделения памяти");
        fclose(file);
        return NULL;
    }

    for (int i = 0; i < size; i++) {
        printf("Введите информацию про звонок номер %d\n", i + 1);
        printf("Введите день:\n");
        call[i].day = input(0, 32);
        printf("Введите месяц:\n");
        call[i].month = input(0, 13);
        printf("Введите год (от 1900 до 2024 включительно):\n");
        call[i].year = input(1899, 2025);

        printf("Введите город:\n");
        input_char(call[i].city, sizeof(call[i].city));
        
        const char *existingCode = getCityCode(call[i].city);
        if (existingCode) {
            strcpy(call[i].code, existingCode);
        } else {
            printf("Введите код города (состоит из чисел):\n");
            input_numb(call[i].code, sizeof(call[i].code));
            addCityCode(call[i].city, call[i].code);
        }

        printf("Введите время разговора (в мин):\n");
        call[i].time = Finput(0.0, 10000.0);

        printf("Введите тариф (р./мин):\n");
        float s = Finput(0.0, 101.0);
        int ss = (int)s;
        if (ss == s) {
            call[i].tarif.intV = ss;
            call[i].is_float = 0;
        } else {
            call[i].tarif.floatV = s;
            call[i].is_float = 1;
        }

        printf("Номер содержит не более 12 чисел. Введите номер, на который звонили (номер города):\n");
        input_numb(call[i].PhoneNumber, sizeof(call[i].PhoneNumber));

        printf("Введите номер абонента:\n");
        input_numb(call[i].PhoneAbonents, sizeof(call[i].PhoneAbonents));
    }

    File_Enter(filename, call, size);
    fclose(file);
    return call;
}

// Функция для добавления нового звонка
calls* addCall(const char *filename, calls *call, int *size) {
    FILE *file = fopen(filename, "r+");
    if (file == NULL) {
        perror("Ошибка открытия файла");
        return NULL;
    }

    // Создаем структуру городов и кодов по имеющемуся массиву звонков
    createCityCodeArray(call, *size);

    *size += 1;
    call = (calls *)realloc(call, (*size) * sizeof(calls));
    if (call == NULL) {
        printf("Memory allocation failed!\n");
        fclose(file);
        exit(1);
    }

    int i = *size - 1;
    printf("Введите информацию про звонок номер %d\n", i + 1);

    printf("Введите день:\n");
    call[i].day = input(0, 32);
    printf("Введите месяц:\n");
    call[i].month = input(0, 13);
    printf("Введите год (от 1900 до 2024 включительно):\n");
    call[i].year = input(1899, 2025);

    inputCityAndCode(&call[i]);

    printf("Введите время разговора (в мин):\n");
    call[i].time = Finput(0.0, 10000.0);

    printf("Введите тариф (р./мин):\n");
    float s = Finput(0.0, 101.0);
    int ss = (int)s;
    if (ss == s) {
        call[i].tarif.intV = ss;
        call[i].is_float = 0;
    } else {
        call[i].tarif.floatV = s;
        call[i].is_float = 1;
    }

    printf("Номер содержит не более 12 чисел. Введите номер, на который звонили (номер города):\n");
    input_numb(call[i].PhoneNumber, sizeof(call[i].PhoneNumber));

    printf("Введите номер абонента:\n");
    input_numb(call[i].PhoneAbonents, sizeof(call[i].PhoneAbonents));

     File_Enter(filename, call, *size);
    fclose(file);
    return call;
}

// Функция для удаления звонка
calls* removeCall(const char *filename, calls *call, int *size, int index) {
    for (int i = index; i < *size - 1; i++) {
        call[i] = call[i + 1];
    }
    *size -= 1;
    call = (calls *)realloc(call, (*size) * sizeof(calls));
    if (call == NULL && *size > 0) {
        printf("Memory allocation failed!\n");
        exit(1);
    }
    File_Enter(filename, call, *size);

    return call;
}

// Функция для изменения поля структуры клиента в файле и массиве
void modifyCalls(const char *filename, calls *call, int size) {

    printf("Список покупателей по номеру абонента:\n");
    for (int i = 0; i < size; ++i)
    {
        printf("%d. %s\n", i + 1, call[i].PhoneAbonents);
    }
    printf("Введите номер абонента, информацию про звонок, которого вы бы хотели изменить:\n");
    int ind = input(0, size + 1);
    FILE *file = fopen(filename, "r+");
    printf("выберите поле для изменения:\n");
    printf("1. дата\n");
    printf("2. код города\n");
    printf("3. город\n");
    printf("4. время разговора\n");
    printf("5. тариф\n");
    printf("6. номер города\n");
    printf("7. номер абонента\n");
    int choice = input(1, 8); // получает выбор пользователя

    calls *ca = &call[ind - 1];

    switch (choice) { // выбирает поле для изменения на основе ввода пользователя
    case 1:
        printf("Введите другой день:\n");
        ca->day = input(0, 32);
        printf("Введите другой месяц:\n");
        ca->month = input(0, 13);
        printf("Введите другой год (от 1900 до 2024 включительно):\n");
        ca->year = input(1899, 2025);
        break;
    case 2:
        printf("Введите новый код города:\n");
        input_numb(ca->code, sizeof(ca->code));
        break;
    case 3:
        printf("Введите другой город:\n");
        input_char(ca->city, sizeof(ca->city));
        break;
    case 4:
        printf("Введите другое время разговора:\n");
        ca->time = Finput(0, 10000.0);
        break;
    case 5:
        printf("Введите новый тариф:\n");
        float s = Finput(0.0, 100.0);
        int ss = (int)s;
        if (ss == s) {
            ca->tarif.intV = ss;
            ca->is_float = 0;
        } else {
            ca->tarif.floatV = s;
            ca->is_float = 1;
        }
        break;
    case 6:
        printf("Введите новый номер города:\n");
        input_numb(ca->PhoneNumber, sizeof(ca->PhoneNumber));
        break;
    case 7:
        printf("Введите новый номер абонента:\n");
        input_numb(ca->PhoneAbonents, sizeof(ca->PhoneAbonents));
        break;
    default:
        printf("некорректный выбор\n");
        break;
    }

    File_Enter(filename, call, size);
    fclose(file);
}


// Функция для вычисления общего времени и суммы по городам
void calculateTotals(calls *call, int num_calls)
{
    char cities[NUM_CALLS][20];
    int city_count = 0;

    float total_time[NUM_CALLS] = {0};
    float total_sum[NUM_CALLS] = {0};

    for (int i = 0; i < num_calls; i++)
    {
        int city_index = -1;

        for (int j = 0; j < city_count; j++)
        {
            if (strcmp(cities[j], call[i].city) == 0)
            {
                city_index = j;
                break;
            }
        }

        if (city_index == -1)
        {
            strcpy(cities[city_count], call[i].city);
            city_index = city_count;
            city_count++;
        }

        total_time[city_index] += call[i].time;
        if (call[i].is_float)
        {
            total_sum[city_index] += call[i].tarif.floatV * call[i].time;
        }
        else
        {
            total_sum[city_index] += call[i].tarif.intV * call[i].time;
        }
    }

    for (int i = 0; i < city_count; i++)
    {
        printf("Город: %s, Общее время разговоров: %.2f, Сумма: %.2f\n", cities[i], total_time[i], total_sum[i]);
    }
}

// Функции сравнения для различных полей
typedef int (*cmp_func)(const calls *, const void *);

int cmp_date(const calls *call, const void *value)
{
    const calls *target = (const calls *)value;
    if (call->year == target->year && call->month == target->month && call->day == target->day)
    {
        return 0;
    }
    return 1;
}

int cmp_code(const calls *call, const void *value)
{
    return strcmp(call->code, (const char *)value);
}

int cmp_city(const calls *call, const void *value)
{
    return strcmp(call->city, (const char *)value);
}

int cmp_time(const calls *call, const void *value)
{
    return call->time == *(const float *)value ? 0 : 1;
}

int cmp_tarif(const calls *call, const void *value)
{
    if (call->is_float)
    {
        return call->tarif.floatV == *(const float *)value ? 0 : 1;
    }
    else
    {
        return call->tarif.intV == *(const int *)value ? 0 : 1;
    }
}

int cmp_phone_number(const calls *call, const void *value)
{
    return strcmp(call->PhoneNumber, (const char *)value);
}

int cmp_phone_abonents(const calls *call, const void *value)
{
    return strcmp(call->PhoneAbonents, (const char *)value);
}

calls *removeCallsByField(calls *callList, int *size, cmp_func cmp, const void *value)
{
    int j = 0;
    for (int i = 0; i < *size; i++)
    {
        if (cmp(&callList[i], value) != 0)
        {
            callList[j++] = callList[i];
        }
    }

    *size = j;
    callList = realloc(callList, (*size) * sizeof(calls));
    if (callList == NULL && *size > 0)
    {
        perror("Ошибка перераспределения памяти");
        exit(1);
    }
    const char filename[] = "/home/porphyriacus/Documents/453502/ОАиП/LR8/Task_2/call.txt";
    File_Enter(filename, callList, *size);
    return callList;
}

void removeByField(calls **callList, int *size)
{
    if (*callList != NULL)
    {
        printf("Выберите поле для удаления:\n");
        printf("1. Дата\n");
        printf("2. Код города\n");
        printf("3. Город\n");
        printf("4. Время разговора\n");
        printf("5. Тариф\n");
        printf("6. Номер города\n");
        printf("7. Номер абонента\n");

        int choice = input(1, 8); // Функция для ввода выбора пользователя

        void *value;
        char strValue[20];
        float floatValue;
        int intValue;
        cmp_func cmp;

        switch (choice)
        {
        case 1:
        {
            printf("Введите день, месяц и год:\n");
            calls target;
            target.day = input(0, 32);
            target.month = input(0, 13);
            target.year = input(1899, 2025);
            value = &target;
            cmp = cmp_date;
            break;
        }
        case 2:
            printf("Введите код города: ");
            input_numb(strValue, sizeof(strValue));
            value = strValue;
            cmp = cmp_code;
            break;
        case 3:
            printf("Введите город: ");
            input_char(strValue, sizeof(strValue));
            value = strValue;
            cmp = cmp_city;
            break;
        case 4:
            printf("Введите время разговора: ");
            floatValue = Finput(0.0, 10000.0);
            value = &floatValue;
            cmp = cmp_time;
            break;
        case 5:
            printf("Введите тариф: ");
            floatValue = Finput(0.0, 100.0);
            value = &floatValue;
            cmp = cmp_tarif;
            break;
        case 6:
            printf("Введите номер города: ");
            input_numb(strValue, sizeof(strValue));
            value = strValue;
            cmp = cmp_phone_number;
            break;
        case 7:
            printf("Введите номер абонента: ");
            input_numb(strValue, sizeof(strValue));
            value = strValue;
            cmp = cmp_phone_abonents;
            break;
        default:
            printf("Некорректный выбор.\n");
            return;
        }

        *callList = removeCallsByField(*callList, size, cmp, value);

        printf("Записи успешно удалены.\n");
    }
    else
    {
        printf("Список звонков пуст.\n");
    }
}

void fre(calls* call){
    free(call);
}