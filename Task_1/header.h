#ifndef HEADER_H
#define HEADER_H

#include <stdio.h>
#include <string.h>

typedef union {
    int intV;
    float floatV;
} discountType;

typedef struct {
    char name[50];
    char surname[50];
    char fathername[50];
    short sex;
    int age;
    char address[100];
    discountType discount;
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

void customerToJson(RegCustomers *, char *);
void sendToElasticSearch(char *);
void deleteAllCustomers();
void uploadAllCustomers(RegCustomers *, int);
size_t WriteCallback(void *, size_t , size_t , void *);
void searchInElasticSearch(const char *, const char *);
const char* chooseField();

#endif 
