#include <curl/curl.h>
#include <json-c/json.h>
#include "header.h"
#define BUFFER_SIZE 1024

/*
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


*/

// Функция для преобразования данных клиента в JSON
void customerToJson(RegCustomers *customer, char *jsonBuffer) // jsonBuffer строка в которую будет записываться преобразованная структура
{
    float discountValue; // тк шаблон отправленный на сервер эластик не разделяет скидку на цилочисленную и с плавающей запятой то пробразуем все к формату float

    if (customer->is_float)
    {
        discountValue = customer->discount.floatV;
    }
    else
    {
        discountValue = (float)customer->discount.intV;
    }
    // создание форматированной строки с вводом в наш заданный буфер
    // .2f : 2 знака после запятой потому что мы преоьразуем целочисленное в число с плавающей точкой
    //       (+ для удобства хранения и поиска, ну и скидка вряд ли будет содержать больше 1 знака)
    snprintf(jsonBuffer, BUFFER_SIZE, "{\"name\":\"%s\",\"surname\":\"%s\",\"fathername\":\"%s\",\"sex\":%d,\"age\":%d,\"address\":\"%s\",\"discount\":%.2f}",
             customer->name, customer->surname, customer->fathername, customer->sex, customer->age, customer->address, discountValue);
}

size_t Writeback(void *contents, size_t size, size_t nmemb, void *userp)
{
    return size * nmemb;
}

// Функция для удаления всех клиентов из Elasticsearch
void deleteAllCustomers()
{
    CURL *curl;   // объявление переменной для работы с cURL
    CURLcode res; // объявление переменной для хранения кода результата выполнения

    curl_global_init(CURL_GLOBAL_DEFAULT); // инициализация глобальных настроек cURL
    curl = curl_easy_init();               // инициализация cURL и получение указателя на структуру CURL

    if (curl)
    {                                                                                                  // проверка, была ли успешна инициализация cURL
        curl_easy_setopt(curl, CURLOPT_URL, "https://localhost:9200/customer_index/_delete_by_query"); // _delete_by_query запрос удаляет документы
        curl_easy_setopt(curl, CURLOPT_USERPWD, "elastic:JpOhGLIUvjwBMhvpgZUY");                       // установка пользователя и пароля для аутентификации
        curl_easy_setopt(curl, CURLOPT_CUSTOMREQUEST, "POST");                                         // установка типа запроса как POST

        const char *json_query = "{\"query\": {\"match_all\": {}}}"; // тк match_all то удаляем все доки
        curl_easy_setopt(curl, CURLOPT_POSTFIELDS, json_query);      // установка тела POST-запроса

        // игнорирование проверки ssl сертификатов
        curl_easy_setopt(curl, CURLOPT_SSL_VERIFYPEER, 0L); // отключение проверки сертификата сервера
        curl_easy_setopt(curl, CURLOPT_SSL_VERIFYHOST, 0L); // отключение проверки имени хоста

        struct curl_slist *headers = NULL;                                      // объявление указателя на список заголовков
        headers = curl_slist_append(headers, "Content-Type: application/json"); // добавление заголовка Content-Type
        curl_easy_setopt(curl, CURLOPT_HTTPHEADER, headers);                    // установка заголовков запроса
                                                                                // Использование пустой функции-обработчика для отключения вывода данных
        curl_easy_setopt(curl, CURLOPT_WRITEFUNCTION, Writeback);
        res = curl_easy_perform(curl); // выполнение запроса
        if (res != CURLE_OK)
        {                                                                                 // проверка на наличие ошибок при выполнении запроса
            fprintf(stderr, "curl_easy_perform() failed: %s\n", curl_easy_strerror(res)); // вывод сообщения об ошибке
        }

        curl_slist_free_all(headers); // освобождение памяти, выделенной для списка заголовков
        curl_easy_cleanup(curl);      // очистка и освобождение ресурсов, использованных cURL
    }

    curl_global_cleanup(); // очистка глобальных ресурсов, использованных cURL
}

/*
// Функция для отправки данных в Elasticsearch
void sendToElasticSearch(char *jsonBuffer) {
    CURL *curl; // CURL это тип данных, обохначающий дескрипторр сеанса HTTP запроса
                // CURL * — это указатель на структуру данных, которая управляется библиотекой libcurl
                // структ содержит всю информацию, необходимую для выполнения HTTP-запроса (URL, заголовки, тело запроса..)
    CURLcode res; // перечислительный тип данных(халоу enum)

    curl_global_init(CURL_GLOBAL_DEFAULT); // функция инициализирует библиотеку libcurl; должна быть вызвана один раз в начале использования библиотеки
    curl = curl_easy_init(); //curl_easy_init создает новый сеанс и возвращает указатель на объект CURL, который является дескриптором сеанса

    if(curl) //проверка успешно ли создан обьект(если создан неуспешно то возвращается NULL)
    {
        //curl_easy_setopt используется для настройки различных параметров сеанса, таких как URL для запроса
        // 1 аргумент : указатель на дескриптор сеанса CURL
        // 2 аргумент : параметр или опция, которую хотим установить
                      // CURLPT_URL: опция, указывающая URL, на который будет отправлен HTTP-запрос
                      // CURLOPT_USERPWD: опция, указывающая учетные данные для базовой HTTP-аутентификации
                      // CURLOPT_POSTFIELDS: опция, указывающая данные для HTTP POST-запроса
        // 3 аргумент : значение, которое хотим присвоить этой опции
        curl_easy_setopt(curl, CURLOPT_URL, "https://localhost:9200/customer_index/_doc/"); //устанавливает URL, на который будет отправлен HTTP-запрос
        curl_easy_setopt(curl, CURLOPT_USERPWD, "elastic:JpOhGLIUvjwBMhvpgZUY"); //устанавливает учетные данные для базовой HTTP-авторизации
        curl_easy_setopt(curl, CURLOPT_POSTFIELDS, jsonBuffer); //  устанавливает данные для HTTP POST-запроса(при использовании пост запроса данные обычно отправляются в теле запроса(в отличии от гет запроса))

        // Игнорирование проверки SSL сертификатов(цифровой сертификат, который аутентифицирует веб-сайт и позволяет установить зашифрованное соединение.)
        curl_easy_setopt(curl, CURLOPT_SSL_VERIFYPEER, 0L); // отключает проверку SSL сертификата сервера
        curl_easy_setopt(curl, CURLOPT_SSL_VERIFYHOST, 0L); // отключает проверку имени хоста в сертификате

        // устанавливает заголовок HTTP-запроса, указывая, что данные отправляются в формате JSON
        struct curl_slist *headers = NULL;
        headers = curl_slist_append(headers, "Content-Type: application/json"); //  ф из библи cURL, используется для добавления нового заголовка в список заголовков
        curl_easy_setopt(curl, CURLOPT_HTTPHEADER, headers);//устанавливает список заголовков для HTTP-запроса.

        res = curl_easy_perform(curl); // curl_easy_perform выполняет HTTP-запрос. Если запрос не удался, выводится сообщение об ошибке
        if(res != CURLE_OK) {
            fprintf(stderr, "curl_easy_perform() failed: %s\n", curl_easy_strerror(res));
        }

        //функции ниже освобождают выделенные ресурсы и очищают CURL-объект
        curl_slist_free_all(headers);
        curl_easy_cleanup(curl);
    }

    curl_global_cleanup(); // функция завершает работу с библиотекой libcurl и освобождает глобальные ресурсы, выделенные curl_global_init
}

deleteCustomerById("document_id");

void deleteCustomerById(const char *doc_id) {
    CURL *curl; // объявление переменной для работы с cURL
    CURLcode res; // объявление переменной для хранения кода результата выполнения

    curl_global_init(CURL_GLOBAL_DEFAULT); // инициализация глобальных настроек cURL
    curl = curl_easy_init(); // инициализация cURL и получение указателя на структуру CURL

    if(curl) { // проверка, была ли успешна инициализация cURL
        // Формирование URL с использованием идентификатора документа
        char url[256];
        snprintf(url, sizeof(url), "https://localhost:9200/customer_index/_doc/%s", doc_id);

        curl_easy_setopt(curl, CURLOPT_URL, url); // установка URL для запроса
        curl_easy_setopt(curl, CURLOPT_USERPWD, "elastic:JpOhGLIUvjwBMhvpgZUY"); // установка пользователя и пароля для аутентификации
        curl_easy_setopt(curl, CURLOPT_CUSTOMREQUEST, "DELETE"); // установка типа запроса как DELETE

        // Игнорирование проверки SSL сертификатов (не рекомендуется для продуктивных систем)
        curl_easy_setopt(curl, CURLOPT_SSL_VERIFYPEER, 0L); // отключение проверки сертификата сервера
        curl_easy_setopt(curl, CURLOPT_SSL_VERIFYHOST, 0L); // отключение проверки имени хоста

        res = curl_easy_perform(curl); // выполнение запроса
        if(res != CURLE_OK) { // проверка на наличие ошибок при выполнении запроса
            fprintf(stderr, "curl_easy_perform() failed: %s\n", curl_easy_strerror(res)); // вывод сообщения об ошибке
        }

        curl_easy_cleanup(curl); // очистка и освобождение ресурсов, использованных cURL
    }

    curl_global_cleanup(); // очистка глобальных ресурсов, использованных cURL
}

*/

// Функция для загрузки всех клиентов на сервер
void createCustomerWithId(const char *doc_id, const char *json_data)
{
    CURL *curl;   // объявление переменной для работы с cURL
    CURLcode res; // объявление переменной для хранения кода результата выполнения

    curl_global_init(CURL_GLOBAL_DEFAULT); // инициализация глобальных настроек cURL
    curl = curl_easy_init();               // инициализация cURL и получение указателя на структуру CURL

    if (curl)
    { // проверка, была ли успешна инициализация cURL
        // формирование URL с использованием идентификатора документа
        char url[256];
        snprintf(url, sizeof(url), "https://localhost:9200/customer_index/_doc/%s", doc_id);

        curl_easy_setopt(curl, CURLOPT_URL, url);                                // установка URL для запроса
        curl_easy_setopt(curl, CURLOPT_USERPWD, "elastic:JpOhGLIUvjwBMhvpgZUY"); // установка пользователя и пароля для аутентификации
        curl_easy_setopt(curl, CURLOPT_CUSTOMREQUEST, "PUT");                    // установка типа запроса как PUT

        curl_easy_setopt(curl, CURLOPT_POSTFIELDS, json_data); // установка тела запроса с JSON-данными

        // игнорирование проверки ssl сертификатов (не рекомендуется для продуктивных систем)
        curl_easy_setopt(curl, CURLOPT_SSL_VERIFYPEER, 0L); // отключение проверки сертификата сервера
        curl_easy_setopt(curl, CURLOPT_SSL_VERIFYHOST, 0L); // отключение проверки имени хоста

        struct curl_slist *headers = NULL;                                      // объявление указателя на список заголовков
        headers = curl_slist_append(headers, "Content-Type: application/json"); // добавление заголовка content-type
        curl_easy_setopt(curl, CURLOPT_HTTPHEADER, headers);                    // установка заголовков запроса
        // Использование пустой функции-обработчика для отключения вывода данных
        curl_easy_setopt(curl, CURLOPT_WRITEFUNCTION, Writeback);

        res = curl_easy_perform(curl); // выполнение запроса if(res != CURLE_OK) { // проверка на наличие ошибок при выполнении запроса

        res = curl_easy_perform(curl); // выполнение запроса
        if (res != CURLE_OK)
        {                                                                                 // проверка на наличие ошибок при выполнении запроса
            fprintf(stderr, "curl_easy_perform() failed: %s\n", curl_easy_strerror(res)); // вывод сообщения об ошибке
        }

        curl_slist_free_all(headers); // освобождение памяти, выделенной для списка заголовков
        curl_easy_cleanup(curl);      // очистка и освобождение ресурсов, использованных cURL
    }

    curl_global_cleanup(); // очистка глобальных ресурсов, использованных cURL
}

void uploadAllCustomers(RegCustomers *list, int numb)
{
    for (int i = 0; i < numb; i++)
    {
        char jsonBuffer[BUFFER_SIZE];
        customerToJson(&list[i], jsonBuffer);

        // Формирование идентификатора на основе индекса
        char doc_id[32];
        snprintf(doc_id, sizeof(doc_id), "customer_%d", i);

        createCustomerWithId(doc_id, jsonBuffer);
    }
}

// Функция для обработки ответа от Elasticsearch
size_t WriteCallback(void *contents, size_t size, size_t nmemb, void *userp)
{
    strncat((char *)userp, (char *)contents, size * nmemb);
    return size * nmemb;
}

// Функция для вывода доступных полей и выбора пользователем
const char *chooseField()
{
    // Поля для поиска
    const char *fields[] = {"address", "name", "surname", "fathername", "sex", "age", "discount"};
    int numFields = sizeof(fields) / sizeof(fields[0]);

    printf("Выберите поле для поиска:\n");
    for (int i = 0; i < numFields; i++)
    {
        printf("%d. %s\n", i + 1, fields[i]);
    }

    int choice;
    scanf("%d", &choice);
    if (choice < 1 || choice > numFields)
    {
        printf("Неверный выбор. Попробуйте снова.\n");
        return NULL;
    }
    if (choice == 5)
    {
        printf("1 - female, 2 - male\n");
    }

    return fields[choice - 1];
}

// Функция для выполнения поиска в Elasticsearch
void searchInElasticSearch(const char *field, const char *query)
{
    CURL *curl;
    CURLcode res;

    curl_global_init(CURL_GLOBAL_DEFAULT);
    curl = curl_easy_init();

    if (curl)
    {
        char url[256];
        snprintf(url, sizeof(url), "https://localhost:9200/customer_index/_search");

        curl_easy_setopt(curl, CURLOPT_URL, url);
        curl_easy_setopt(curl, CURLOPT_USERPWD, "elastic:JpOhGLIUvjwBMhvpgZUY");
        curl_easy_setopt(curl, CURLOPT_WRITEFUNCTION, WriteCallback);

        char response[BUFFER_SIZE] = {0}; // Инициализация буфера
        curl_easy_setopt(curl, CURLOPT_WRITEDATA, response);

        // Формат JSON для запроса match_phrase
        char json_query[BUFFER_SIZE];
        snprintf(json_query, sizeof(json_query), "{\"query\": {\"match_phrase\": {\"%s\": \"%s\"}}}", field, query);
        curl_easy_setopt(curl, CURLOPT_POSTFIELDS, json_query);
        curl_easy_setopt(curl, CURLOPT_CUSTOMREQUEST, "POST");

        // Игнорирование проверки SSL сертификатов
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
            // Парсинг и вывод результатов
            struct json_object *parsed_json;
            struct json_object *hits_obj;
            struct json_object *hits_arr;
            struct json_object *total_obj;
            struct json_object *hit;
            struct json_object *source_obj;

            parsed_json = json_tokener_parse(response);
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

                            struct json_object *name;
                            struct json_object *surname;
                            struct json_object *fathername;
                            struct json_object *sex;
                            struct json_object *age;
                            struct json_object *address;
                            struct json_object *discount;

                            json_object_object_get_ex(source_obj, "name", &name);
                            json_object_object_get_ex(source_obj, "surname", &surname);
                            json_object_object_get_ex(source_obj, "fathername", &fathername);
                            json_object_object_get_ex(source_obj, "sex", &sex);
                            json_object_object_get_ex(source_obj, "age", &age);
                            json_object_object_get_ex(source_obj, "address", &address);
                            json_object_object_get_ex(source_obj, "discount", &discount);

                            printf("Клиент #%d:\n", i + 1);
                            printf("Имя: %s\n", json_object_get_string(name));
                            printf("Фамилия: %s\n", json_object_get_string(surname));
                            printf("Отчество: %s\n", json_object_get_string(fathername));
                            if (json_object_get_int(sex) == 1)
                            {
                                printf("Пол: %s\n", "female");
                            }
                            else
                            {
                                printf("Пол: %s\n", "male");
                            }
                            printf("Возраст: %d\n", json_object_get_int(age));
                            printf("Адрес: %s\n", json_object_get_string(address));
                            printf("Скидка: %.2f%%\n", json_object_get_double(discount));
                            printf("\n");
                        }
                    }
                    else
                    {
                        printf("Клиенты не найдены.\n");
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

            json_object_put(parsed_json); // Освобождение памяти
        }

        curl_slist_free_all(headers);
        curl_easy_cleanup(curl);
    }

    curl_global_cleanup();
}

void writeCustomersToJson(RegCustomers *customers, int size, const char *filename)
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
        fprintf(file, "    \"name\": \"%s\",\n", customers[i].name);
        fprintf(file, "    \"surname\": \"%s\",\n", customers[i].surname);
        fprintf(file, "    \"fathername\": \"%s\",\n", customers[i].fathername);
        fprintf(file, "    \"sex\": \"%s\",\n", customers[i].sex ? "Male" : "Female");
        fprintf(file, "    \"age\": %d,\n", customers[i].age);
        fprintf(file, "    \"address\": \"%s\",\n", customers[i].address);
        if (customers[i].is_float)
        {
            fprintf(file, "    \"discount\": %.2f\n", customers[i].discount.floatV);
        }
        else
        {
            fprintf(file, "    \"discount\": %d\n", customers[i].discount.intV);
        }
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