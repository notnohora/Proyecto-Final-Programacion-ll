#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <ctype.h>
#include "structs.h"

// Function to print the date
void PrintDate() {
    
    time_t currentTime = time(NULL); // currentTime: variable used to store the actual time in the system
    struct tm *local_time = localtime(&currentTime); // local_time: variable used to store a struct of time
    char formatedTime[50] = "";  // formatedTime: variable used to store a string
    strftime(formatedTime, sizeof(formatedTime), "Valid to %Y-%b-%d at %H:%M hours.", local_time);
    printf("%s\n", formatedTime);
}

// Funcion to calculate and printf the time
// time: variable used to store the time
void PrintExecutionTime(double time){
    int minutes = 0, seconds = 0; // variables used to store minuts and seconds respectively
    char totalTime[6] = ""; // totalTime: variable used to store the time calculated

    minutes = (int) time / 60;
    seconds = (int) time % 60;

    sprintf(totalTime, "%02d'%02d''", minutes, seconds);
    printf("%s", totalTime);
}

// Function that calculates the number of records
// nameFile: variable used to save the file name
// sizeOfRecord: variable used to save the size of a record
// mode: variable used to save a number to identify if the file is csv or c
int CalculateNumberOfRecords(char nameFile[], int sizeOfRecord, int mode){

   int numberOfRecords = 0; // numberOfRecords: variable used to store the number of records
   FILE *filePointer = fopen(nameFile, "r"); // filePointer: variable used to store the memory address of the file
   if(filePointer == NULL){
      return -1;
   }
   if(mode){
     fseek(filePointer, 0, SEEK_END);
     long num = ftell(filePointer); // num: variable used to store the position of the cursor
     fclose(filePointer);
     numberOfRecords = num/sizeOfRecord;
   }
   else{
      char line[200]= "";
      while(fgets(line, sizeof(line), filePointer)){
           numberOfRecords++;
      }
    }
     return numberOfRecords;
}

// Function to determine if a string is numeric
// string: variable to store a memory adress of a string
int is_number(char *string){
  if (*string == '\0')
    return 0;
  for (int i = 0; string[i] != '\0'; i++){
    if (!isdigit((unsigned char)string[i])){
      return 0;
    }
  }
  return 1;
}

// Function to create Stores table
// filePointer: variable used to store the memory address of the file
// nameToTable : variable used to store the name of the new file
int CreateStoresTable(FILE *filePointer, char nameToTable[]) {

    FILE *storesPointer = fopen( nameToTable, "wb"); // storesPointer: ariable used to store the memory address of the file
    char line[100] = ""; // line: variable used to save a record
    int numberOfRecords = 0; // numberOfRecords: variable used to store the number of records
    storesRecords stores; // stores: variable used to store the struct of stores

    while (fgets(line, sizeof(line), filePointer)) {
        char *token = strtok(line, ","); // token: variable used to store each atribute of the records read temporaly
        stores.storeKey = (unsigned short int)atoi(token);
        token = strtok(NULL, ",");

        if (strlen(token) > 35) {
            printf("ERROR exceeds the size of the string for the country-stores\"%s\"\n", token);
            return 0;
        } else {
            strcpy(stores.country, token);
            token = strtok(NULL, ",");

            if (strlen(token) > 35) {
                printf("ERROR exceeds the size of the string for the state-stores\"%s\"\n", token);
                return 0;
            } else {
                strcpy(stores.state, token);
                token = strtok(NULL, ",");

                if (is_number(token)) {
                    stores.squareMeters = (unsigned short int)atoi(token);
                    token = strtok(NULL, ",");
                    sscanf(token, "%hhu/%hhu/%hu", &stores.openDate.DD, &stores.openDate.MM, &stores.openDate.AAAA);
                } else {
                    stores.squareMeters = 0;
                    sscanf(token, "%hhu/%hhu/%hu", &stores.openDate.DD, &stores.openDate.MM, &stores.openDate.AAAA);
                }

                fwrite(&stores, sizeof(stores), 1, storesPointer);
                numberOfRecords++;
                // printf("%hu,%s,%s,%hu,%hu/%hhu/%hu\n", stores.storeKey, stores.country, stores.state, stores.squareMeters, stores.openDate.DD, stores.openDate.MM, stores.openDate.AAAA);
            }
        }
    }
    fclose(storesPointer);fclose(filePointer);
    return numberOfRecords;
}

//Function to create Stores tables
// filePointer: variable used to store the memory address of the file
// nameToTable : variable used to store the name of the new file
  int CreateExchangesTable(FILE *filePointer,  char nameToTable[]) {

    FILE *exchangesPointer = fopen( nameToTable, "wb"); // exchangesPointer: variable used to store the memory address of the file
    char line[100] = ""; // line: variable used to save a record 
    int numberOfRecords = 0; // numberOfRecords: variable used to store the number of record
    exchangeRatesRecords exchange; // exchange: variable used to store the struct of exchanges

    while (fgets(line, sizeof(line), filePointer)) {
        char *token = strtok(line, ","); // token: variable used to store each atribute of the records read temporaly
        if (strlen(token) >= 11) {
            printf("ERROR exceeds the size of the string for the date-exchanges\"%s\"\n", token);
            return 0;
        } else {
            strcpy(exchange.date, token);
            token = strtok(NULL, ",");
            if (strlen(token) > 3) {
                printf("ERROR exceeds the size of the string for the currency-exchanges\"%s\"\n", token);
                return 0;
            } else {
                strcpy(exchange.currency, token);
                token = strtok(NULL, ",");
                exchange.exchanges = atof(token);
                fwrite(&exchange, sizeof(exchange), 1, exchangesPointer);
                numberOfRecords++;
                //printf("%s,%s,%f\n", exchange.date, exchange.currency, exchange.exchanges);
            }
        }
    }
    fclose(exchangesPointer);fclose(filePointer);
    return numberOfRecords;
}

// Function to create Sales tables
// filePointer: variable used to store the memory address of the file
// nameToTable: variable used to store the name for the nwe file
int CreateSalesTable(FILE *filePointer,  char nameToTable[]) {

    FILE *salesPointer = fopen( nameToTable, "wb+"); // salesPointer: variable used to store the memory adress of a file
    char line[100] = ""; // line: variable used to store a record temporaly
    int numberOfRecords = 0; // numberOfRecords: variable used to store the number of records
    salesRecords sales; // sales: variable used to store the struct of sales records

    while (fgets(line, sizeof(line), filePointer)) {
        char *token = strtok(line, ",");// token: variable used to store each atribute of the record temporaly

        if (token == NULL) {
            printf("ERROR invalid order number-sales\"%s\"\n", token);
            return 0;
        }
        sales.orderNumber = atol(token);
        token = strtok(NULL, ",");
        if (token == NULL) {
            printf("ERROR invalid line item-sales\"%s\"\n", token);
            return 0;
        }
        sales.lineItem = (unsigned char)atoi(token);
        token = strtok(NULL, ",");
        if (token == NULL) {
            printf("ERROR invalid order date-sales\"%s\"\n", token);
            return 0; 
        }
        sscanf(token, "%hhu/%hhu/%hu", &sales.orderDate.MM, &sales.orderDate.DD, &sales.orderDate.AAAA);
        token = strtok(NULL, ",");
        if (token == NULL) {
    
            sales.deliveryDate.DD = 0;
            sales.deliveryDate.MM = 0;
            sales.deliveryDate.AAAA = 0;
        } else {
            sscanf(token, "%hhu/%hhu/%hu", &sales.deliveryDate.MM, &sales.deliveryDate.DD, &sales.deliveryDate.AAAA);
        }
        token = strtok(NULL, ",");
        if (token == NULL) {
           printf("ERROR invalid customer key-sales \"%s\"\n", token);
            return 0; 
        }
        sales.customerKey = (unsigned int)atoi(token);
        token = strtok(NULL, ",");
        if (token == NULL) {
            printf("ERROR invalid store key-sales\"%s\"\n", token);
            return 0;
        }
        sales.storeKey = (unsigned short int)atoi(token);
        token = strtok(NULL, ",");
        if (token == NULL) {
            printf("ERROR invalid product key-sales\"%s\"\n", token);
            return 0;
        }
        sales.productKey = (unsigned short int)atoi(token);
        token = strtok(NULL, ",");
        if (token == NULL) {
            printf("ERROR invalid quantity-sales\"%s\"\n", token);
            return 0;
        }
        sales.quantity = (unsigned short int)atoi(token);
        token = strtok(NULL, ",");
        if (token == NULL) {
            printf("ERROR invalid currency code-sales\"%s\"\n", token);
            return 0;
        }
        if (strlen(token) > 4) {
            printf("ERROR invalid currency code-sales\"%s\"\n", token);
            return 0;
        }
        strcpy(sales.currencyCode, token);
        fwrite(&sales, sizeof(sales), 1, salesPointer);
        numberOfRecords++;
    }

    fclose(salesPointer);
    fclose(filePointer);
    return numberOfRecords;
}


//Function to create Customers tables
// filePointer: variable used to store the memory address of the file
// nameToTable: variable used to store the name for the new file
int CreateCostumersTable(FILE *filePointer,  char nameToTable[]) {

    FILE *customersPointer = fopen( nameToTable, "wb"); // customersPointer: variable used to store the memory address of the file
    char line[200] = ""; // line: variable used to save a record
    int numberOfRecords = 0; // numberOfRecords: variable used to store the number of record
    customersRecords custom; // custom: variable used to store the struct of customers

    while (fgets(line, sizeof(line), filePointer)) {
        char *token = strtok(line, ";"); // token: variable used to store each atribute of the records read temporaly
        custom.customerKey = (unsigned int)atoi(token);
        token = strtok(NULL, ";");

        if (strlen(token) > 8) {
            printf("ERROR exceeds the size of the string for the gender-customers \"%s\"\n", token);
            return 0;
        } else {
            strcpy(custom.gender, token);
            token = strtok(NULL, ";");

            if (strlen(token) > 40) {
                printf("ERROR exceeds the size of the string for the name-customers \"%s\"\n", token);
                return 0;
            } else {
                strcpy(custom.name, token);
                token = strtok(NULL, ";");

                if (strlen(token) > 40) {
                    printf("ERROR exceeds the size of the string for the city-customers \"%s\"\n", token);
                    return 0;
                } else {
                    strcpy(custom.city, token);
                    token = strtok(NULL, ";");

                    if (strlen(token) > 30) {
                        printf("ERROR exceeds the size of the string for the state code-customers \"%s\"\n", token);
                        printf("%s\n", token);
                        return 0;
                    } else {
                        strcpy(custom.stateCode, token);
                        token = strtok(NULL, ";");

                        if (strlen(token) > 30) {
                            printf("ERROR exceeds the size of the string for the state-customers \"%s\"\n", token);
                            return 0;
                        } else {
                            strcpy(custom.state, token);
                            token = strtok(NULL, ";");

                            if (is_number(token)) {
                                custom.zipCode = (unsigned int)atoi(token);
                                token = strtok(NULL, ";");

                                if (strlen(token) > 20) {
                                    printf("ERROR exceeds the size of the string for the country-customers \"%s\"\n", token);
                                    return 0;
                                } else {
                                    strcpy(custom.country, token);
                                    token = strtok(NULL, ";");

                                    if (strlen(token) > 20) {
                                        printf("ERROR exceeds the size of the string for the continent-customers \"%s\"\n", token);
                                        return 0;
                                    } else {
                                        strcpy(custom.continent, token);
                                        token = strtok(NULL, "/");
                                        custom.birthday.DD = (unsigned char)atoi(token);
                                        token = strtok(NULL, "/");
                                        custom.birthday.MM = (unsigned char)atoi(token);
                                        token = strtok(NULL, "/");
                                        custom.birthday.AAAA = (unsigned short int)atoi(token);
                                    }
                                }
                            } else {
                                custom.zipCode = 0;

                                if (strlen(token) > 40) {
                                    printf("ERROR exceeds the size of the string for the country-customers \"%s\"\n", token);
                                    return 0;
                                } else {
                                    strcpy(custom.country, token);
                                    token = strtok(NULL, ";");

                                    if (strlen(token) > 40) {
                                        printf("ERROR exceeds the size of the string for the continent-customers \"%s\"\n", token);
                                        return 0;
                                    } else {
                                        strcpy(custom.continent, token);
                                        token = strtok(NULL, "/");
                                        custom.birthday.DD = (unsigned char)atoi(token);
                                        token = strtok(NULL, "/");
                                        custom.birthday.MM = (unsigned char)atoi(token);
                                        token = strtok(NULL, "/");
                                        custom.birthday.AAAA = (unsigned short int)atoi(token);
                                    }
                                }
                            }
                            fwrite(&custom, sizeof(custom), 1, customersPointer);
                            numberOfRecords++;
                            //printf("%i;%s;%s;%s;%s;%s;%i;%s;%s;%hhu/%hhu/%i\n",custom.customerKey, custom.gender, custom.name, custom.city, custom.stateCode, custom.state,custom.zipCode, custom.country, custom.continent,custom.birthday.DD, custom.birthday.MM, custom.birthday.AAAA);
                        }
                    }
                }
            }
        }
    }
    fclose(customersPointer);fclose(filePointer);
    return numberOfRecords;
}

// Function to create Products tables
// filePointer: variable used to store the memory address of the file
// nameToTable: variable used to store the name for the new file
int CreateProductsTable(FILE *filePointer, char nameToTable[]){
    
    FILE *pointerProducts = fopen(nameToTable, "wb");// pointerProducts: variable used to store the memory address of a file
    if(pointerProducts == NULL){
        printf("Error al abrir el archivo de destino.");
        return 0;
    }
    productsRecords products; // products: variable used to store a struct
    char line[300] = ""; // line: variable used to store temporaly a record
    int numberOfRecords = 0; // numberOfRecords: used to store 

    while (fgets(line, sizeof(line), filePointer)){
        if(numberOfRecords != 0){
            char *token = strtok(line, ","), temp[200] = ""; // token/temp: variables used to store a string (atribute) temporaly
            products.productKey = (unsigned short int) atoi(token);

            token = strtok(NULL, ",");
            if(token[0] == '"'){
                while (token[strlen(token) - 1] != '"'){
                    strcat(token, strtok(NULL, ","));
                }
                strncpy(products.productName, token + 1, strlen(token));
                products.productName[strlen(products.productName) - 1] = '\0';
            } else {
                strcpy(products.productName, token);
            }

            token = strtok(NULL, ",");
            strcpy(products.brand, token);

            token = strtok(NULL, ",");
            strcpy(products.color, token);

            token = strtok(NULL, ",");
            if(token[0] == '"'){
                strcat(token, strtok(NULL, ","));
                products.unitCostUSD = atof(strncpy(temp, token + 2, strlen(token)));
            } else {
                products.unitCostUSD = atof(strncpy(temp, token + 1, strlen(token)));
            }

            token = strtok(NULL, ",");
            if(token[0] == '"'){
                strcat(token, strtok(NULL, ","));
                products.unitPriceUSD = atof(strncpy(temp, token + 2, strlen(token)));
            } else {
                products.unitPriceUSD = atof(strncpy(temp, token + 1, strlen(token)));
            }

            token = strtok(NULL, ",");
            strcpy(products.subCategoryKey, token);

            token = strtok(NULL, ",");
            if(token[0] == '"'){
                strcat(token, strtok(NULL, ","));
                strncpy(products.subCategory, token + 1, strlen(token));
                products.subCategory[strlen(products.subCategory) - 1] = '\0';
            } else {
                strcpy(products.subCategory, token);
            }

            token = strtok(NULL, ",");
            strcpy(products.categoryKey, token);

            token = strtok(NULL, ",");
            if(token[0] == '"'){
                strcat(token, strtok(NULL, ","));
                strncpy(products.category, token + 1, strlen(token));
                products.category[strlen(products.category) - 1] = '\0';
            } else {
                strcpy(products.category, token);
            }

            fwrite(&products, sizeof(products), 1, pointerProducts);
            //printf("%hu,%s,%s,%s,$%.2f,$%.2f,%s,%s,%s,%s\n",products.productKey, products.productName, products.brand, products.color, products.unitCostUSD, products.unitPriceUSD, products.subCategoryKey, products.subCategory, products.categoryKey , products.category);
            
        }
        numberOfRecords++;
    }
    fclose(filePointer); fclose(pointerProducts);
    return numberOfRecords;
}