#ifndef HEADER_H
#define HEADER_H

#include <stdio.h>
#include <string.h>
#define BUFFER_SIZE 1024
typedef struct {
    char uuid[37]; // уникальный UUID (36 символов + терминальный ноль)
    char name[50];
    char surname[50];
    char fathername[50];
    int sex;
    int age;
    char address[100];
    union {
        int intV;
        float floatV;
    } discount;
    int is_float;
} RegCustomers;

void List(RegCustomers*, int);

void menu();

int input(int, int);
float Finput(float, float);
void input_char(char *, int);
void input_address(char *, int);

RegCustomers *createArray(FILE* , int);
RegCustomers *addCustomer(FILE* , RegCustomers *, int*);
RegCustomers *removeCustomer(FILE* , RegCustomers *, int *, int);
void modifyCustomer(FILE* , RegCustomers *, int);
RegCustomers* sort(RegCustomers* , const int);
void discount(RegCustomers*, const int);

RegCustomers *readAllCustomersFromFile(const char *, int *);
void File_Enter(FILE *, RegCustomers*, int) ;
void ChangeBin(FILE *, RegCustomers, int );
void deleteCustomerByIndex(FILE*, int);


RegCustomers *removeCustomerByUUID(FILE*, RegCustomers *, int *, const char *);
size_t WriteCallback(void *contents, size_t , size_t, void *);
void customerToJson(RegCustomers *, char *);
void sendToElasticSearch(char *);
void deleteAllCustomers();
void searchInElasticSearch(const char *);
void uploadAllCustomers(RegCustomers *, int);
void RegCustomersToJson(RegCustomers *, char *);
void fre(RegCustomers*);
#endif 
