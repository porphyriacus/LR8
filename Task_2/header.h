#ifndef HEADER_H
#define HEADER_H

typedef union {
    int intV;
    float floatV;
} tarifType;

typedef struct {
    int day;
    int month;
    int year;
    char code[20];
    char city[20];
    float time;
    tarifType tarif;
    int is_float;
    char PhoneNumber[13];
    char PhoneAbonents[13];
} calls;


calls *addCall(const char *, calls *, int *);
calls *removeCall(const char *, calls *, int *, int);
void modifyCalls(const char *, calls *, int);
calls *createArray(const char *, int);
void calculateTotals(calls*, int);
void fre(calls*);
void removeByField(calls **callList, int *size);

void File_Enter(const char *, calls *, int);
calls* readCallsFromFile(calls *, const char *, int *);

void menu();

int input(int, int);
float Finput(float, float);
void input_char(char *, int);
void input_numb(char *, int);
void input_date(int *, int *, int *);

void list(calls *, int );
void printCall(const calls *);

void callToJson(calls *, char *);
void sendToElasticSearch(char *);
void deleteAllcalls();
void uploadAllcalls(calls *, int);
size_t WriteCallback(void *, size_t, size_t, void *);

void searchInElasticSearch(const char *);
void writecallsToJson(calls *, int, const char *);


#endif 
