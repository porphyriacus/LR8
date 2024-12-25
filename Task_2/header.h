#ifndef HEADER_H
#define HEADER_H

#include <stdio.h>
#include <string.h>


typedef union {
    int intV;
    float floatV;
} tarifType;

typedef struct {
    char date[12];
    char code[20];
    char city[20];
    float time;
    tarifType tarif;
    int is_float;
    char PhoneNumber[13];
    char PhoneAbonents[13];
} calls;

void menu();

int input(int, int);
float Finput(float, float);
void input_char(char *, int);
void input_numb(char *, int);
void input_date(char *, int);

calls *addCall(const char *, calls *, int *);
calls *removeCall(const char *, calls *, int *, int);
void modifyCalls(const char *, calls *, int);
calls *createArray(const char *, int);
void calculateTotals(calls*, int);
void removeByField(calls **callList, int *size);


void writeSingleCallToFile(FILE *, const calls );
void File_Enter(const char *, calls *, int);
calls* readCallsFromFile(calls *, const char *, int *);

void callToJson(calls *, char *);
void sendToElasticSearch(char *);
void deleteAllcalls();
void uploadAllcalls(calls *, int);
size_t WriteCallback(void *, size_t, size_t, void *);
const char* chooseField();
void searchInElasticSearch(const char *, const char *);
void writecallsToJson(calls *, int, const char *);

void list(calls *, int );
void printCall(const calls *);

#endif 
