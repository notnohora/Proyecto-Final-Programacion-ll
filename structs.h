#ifndef STRUCTS_H
#define STRUCTS_H
#include <stdio.h>

//struct of sales
typedef struct
{
  long int orderNumber;
  unsigned char lineItem;
  struct{
     unsigned char DD;
     unsigned char MM;
    unsigned short int AAAA;
  } orderDate;
  struct{
    unsigned char DD;
    unsigned char MM;
    unsigned short int AAAA;
  } deliveryDate;
  unsigned int customerKey;
  unsigned short int storeKey;
  unsigned short int productKey;
  unsigned short int quantity;
  char currencyCode[4];//fixed
} salesRecords;

//struct of stores
typedef struct
{
  unsigned short int storeKey;
  char country[35];
  char state[35];
  unsigned short int squareMeters;
  struct
  {
    unsigned char DD;
    unsigned char MM;
    unsigned short int AAAA;
  } openDate;
} storesRecords;

//struct of products
typedef struct
{
  unsigned short int productKey;
  char productName[100]; //fixed
  char brand[30];//fixed
  char color[15];
  float unitCostUSD;
  float unitPriceUSD;
  char subCategoryKey[5];//fixed
  char subCategory[50];//fixed
  char categoryKey[3];//fixed
  char category[20];
} productsRecords;

//struct of exchanges rates
typedef struct
{
  char date[11];//fixed 
  char currency[3];
  float exchanges;
} exchangeRatesRecords;

//struct of customer
typedef struct
{
  unsigned int customerKey;
  char gender[8];
  char name[40];
  char city[40];
  char stateCode[30];//fixed
  char state[30];
  unsigned int zipCode;
  char country[20];
  char continent[20];
  struct
  {
    unsigned char DD;
    unsigned char MM;
    unsigned short int AAAA;
  } birthday;
} customersRecords;

int is_number(char *string);
int CreateStoresTable(FILE *filePointer, char nameToTable[]);
int CreateExchangesTable(FILE *filePointer, char nameToTable[]);
int CreateSalesTable(FILE *filePointer, char nameToTable[]);
int CreateCostumersTable(FILE *filePointer, char nameToTable[]);
int CreateProductsTable(FILE *filePointer, char nameToTable[]);
int CalculateNumberOfRecords(char nameFile[], int, int);
void PrintDate();
void PrintExecutionTime(double time);

#endif // STRUCTS_H