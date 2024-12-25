#include <curl/curl.h>
#include <json-c/json.h>
#include "header.h"
#define BUFFER_SIZE 1024


// Функция для преобразования данных клиента в JSON
void callToJson(calls *call, char *jsonBuffer) // jsonBuffer строка в которую будет записываться преобразованная структура
 {
    float tarifValue; // тк шаблон отправленный на сервер эластик не разделяет скидку на цилочисленную и с плавающей запятой то пробразуем все к формату float

    if (call->is_float) {
        tarifValue = call->tarif.floatV;
    } else {
        tarifValue = (float)call->tarif.intV;
    }
    // создание форматированной строки с вводом в наш заданный буфер
    // .2f : 2 знака после запятой потому что мы преоьразуем целочисленное в число с плавающей точкой 
    //       (+ для удобства хранения и поиска, ну и тариф вряд ли будет содержать больше 1 знака)
    snprintf(jsonBuffer, BUFFER_SIZE, "{\"date\":\"%s\",\"code\":\"%s\",\"city\":\"%s\",\"time\":%f,\"tarif\":%.2f,\"PhoneNumber\":\"%s\",\"PhoneAbonents\":%s}", 
             call->date, call->code, call->city, call->time, tarifValue, call->PhoneNumber, call->PhoneAbonents);

}

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
        curl_easy_setopt(curl, CURLOPT_URL, "https://localhost:9200/calls_index/_doc/"); //устанавливает URL, на который будет отправлен HTTP-запрос
        curl_easy_setopt(curl, CURLOPT_USERPWD, "elastic:JpOhGLIUvjwBMhvpgZUY"); //устанавливает учетные данные для базовой HTTP-авторизации
        curl_easy_setopt(curl, CURLOPT_POSTFIELDS, jsonBuffer); //  устанавливает данные для HTTP POST-запроса

        // Игнорирование проверки SSL сертификатов
        curl_easy_setopt(curl, CURLOPT_SSL_VERIFYPEER, 0L);
        curl_easy_setopt(curl, CURLOPT_SSL_VERIFYHOST, 0L);

        // устанавливает заголовок HTTP-запроса, указывая, что данные отправляются в формате JSON
        struct curl_slist *headers = NULL;
        headers = curl_slist_append(headers, "Content-Type: application/json");
        curl_easy_setopt(curl, CURLOPT_HTTPHEADER, headers);

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

// Функция для удаления всех клиентов из Elasticsearch
void deleteAllcalls() {
    CURL *curl;
    CURLcode res;

    curl_global_init(CURL_GLOBAL_DEFAULT);
    curl = curl_easy_init();

    if(curl) {
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

        res = curl_easy_perform(curl);
        if(res != CURLE_OK) {
            fprintf(stderr, "curl_easy_perform() failed: %s\n", curl_easy_strerror(res));
        }

        curl_slist_free_all(headers);
        curl_easy_cleanup(curl);
    }

    curl_global_cleanup();
}

// Функция для загрузки всех клиентов на сервер
void uploadAllcalls(calls *list, int numb) {
    for (int i = 0; i < numb; i++) {
        char jsonBuffer[BUFFER_SIZE];
        callToJson(&list[i], jsonBuffer);
        sendToElasticSearch(jsonBuffer);
    }
}

// Функция для обработки ответа от Elasticsearch
size_t WriteCallback(void *contents, size_t size, size_t nmemb, void *userp) {
    strncat((char *)userp, (char *)contents, size * nmemb);
    return size * nmemb;
}


// Функция для вывода доступных полей и выбора пользователем
const char* chooseField() {
    // Поля для поиска
    const char *fields[] = {"date", "code", "city", "time", "tarif", "PhoneNumber", "PhoneAbonents"};
    int numFields = sizeof(fields) / sizeof(fields[0]);

    printf("Выберите поле для поиска:\n");
    for (int i = 0; i < numFields; i++) {
        printf("%d. %s\n", i + 1, fields[i]);
    }

    int choice;
    scanf("%d", &choice);
    if (choice < 1 || choice > numFields) {
        printf("Неверный выбор. Попробуйте снова.\n");
        return NULL;
    }
    
    return fields[choice - 1];
}

// Функция для выполнения поиска в Elasticsearch
void searchInElasticSearch(const char *field, const char *query) {
    CURL *curl;
    CURLcode res;

    curl_global_init(CURL_GLOBAL_DEFAULT);
    curl = curl_easy_init();

    if (curl) {
        char url[256];
        snprintf(url, sizeof(url), "https://localhost:9200/calls_index/_search");

        curl_easy_setopt(curl, CURLOPT_URL, url);
        curl_easy_setopt(curl, CURLOPT_USERPWD, "elastic:JpOhGLIUvjwBMhvpgZUY");
        curl_easy_setopt(curl, CURLOPT_WRITEFUNCTION, WriteCallback);

        char response[BUFFER_SIZE] = {0};  // Инициализация буфера
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
        if (res != CURLE_OK) {
            fprintf(stderr, "curl_easy_perform() failed: %s\n", curl_easy_strerror(res));
        } else {
            // Парсинг и вывод результатов
            struct json_object *parsed_json;
            struct json_object *hits_obj;
            struct json_object *hits_arr;
            struct json_object *total_obj;
            struct json_object *hit;
            struct json_object *source_obj;

            parsed_json = json_tokener_parse(response);
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

                            struct json_object *date;
                            struct json_object *code;
                            struct json_object *city;
                            struct json_object *time;
                            struct json_object *tarif;
                            struct json_object *PhoneNumber;
                            struct json_object *PhoneAbonents;

                            json_object_object_get_ex(source_obj, "date", &date);
                            json_object_object_get_ex(source_obj, "code", &code);
                            json_object_object_get_ex(source_obj, "city", &city);
                            json_object_object_get_ex(source_obj, "time", &time);
                            json_object_object_get_ex(source_obj, "tarif", &tarif);
                            json_object_object_get_ex(source_obj, "PhoneNumber", &PhoneNumber);
                            json_object_object_get_ex(source_obj, "PhoneAbonents", &PhoneAbonents);

                            printf("Звонок №%d:\n", i + 1);
                            printf("Дата: %s\n", json_object_get_string(date));
                            printf("Код города: %s\n", json_object_get_string(code));
                            printf("Город: %s\n", json_object_get_string(city));
                         
                            printf("Тариф: %.2f\n", json_object_get_double(tarif));
                            printf("Номер города: %s\n", json_object_get_string(PhoneNumber));
                            printf("Номер абонента: %s\n", json_object_get_string(PhoneAbonents));
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

            json_object_put(parsed_json);  // Освобождение памяти
        }

        curl_slist_free_all(headers);
        curl_easy_cleanup(curl);
    }

    curl_global_cleanup();
}

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
        fprintf(file, "    \"date\": \"%s\",\n", calls[i].date);
        fprintf(file, "    \"code\": \"%s\",\n", calls[i].code);
        fprintf(file, "    \"city\": \"%s\",\n", calls[i].city);
        fprintf(file, "    \"sex\": \"%.2f\",\n", calls[i].time);
        if (calls[i].is_float)
        {
            fprintf(file, "    \"tarif\": %.2f\n", calls[i].tarif.floatV);
        }
        else
        {
            fprintf(file, "    \"tarif\": %d\n", calls[i].tarif.intV);
        }
        fprintf(file, "    \"PhoneNumber\": %s,\n", calls[i].PhoneNumber);
        fprintf(file, "    \"PhoneAbonents\": \"%s\",\n", calls[i].PhoneAbonents);
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