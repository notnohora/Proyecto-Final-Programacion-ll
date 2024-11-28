/* 
Fecha de publicación: 22/11/2024
Hora de publicación: 6:00 am
Versión de su código: 1
Autor. Ing(c) Nohora Camila Cardozo Cardenas
Nombre del lenguaje utilizado: C
Versión del lenguaje utilizado: C11
Nombre y versión del Sistema Operativo(S.O): Microsoft Windows 11 Home Single Language 23H2 10.022631 compilacion 22631
Version del compilador utilizado: GCC-14.1.0
Presentado a: Doctor Ricardo Moreno Laverde
Universidad Tecnológica de Pereira
Programa de Ingeniería de Sistemas y Computación
Asignatura IS284 Programación II
Un descriptivo de que hace el programa: gestor de base de datos para la compañia Global Electronics Retailer
Salvedades: Ejecutar la opcion uno antes de cualquer otra opcion exceptuando la opcion 0. No se garantiza su 
funcionamiento en otras versiones del sistema operativo, en otros sistemas operativos, en otras versiones del 
lenguaje, del compilador u otro compilador. */
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <ctype.h>
#include <time.h>
#include "option1.c"
#include "option2.c"
#include "option3.c"
#include "option4.c"
#include "option5.c"

// Function to direct option 5 of menu
// customers/products/sales/exchangesTable: variables used to store the name of the table files
// mode: variable used to store the type of sort to use
int Option5(char productsTable[], char salesTable[], char customersTable[], char exchangesTable[], int mode){

    int start = 0, end = 0; //variables used to store the start and finish of the ejecution
    start = clock();
    printf("------------------------------------------------------------------------------------------------------------------------\n");
    printf("\033[1mCompany Global Electronics Retailer\033[0m\n");
    PrintDate(); 
    printf("Customer list ordered by Costumer name  + order date for sale + ProductKey \n");
    if(mode){
       BubbleSortOption5(productsTable, salesTable, customersTable, exchangesTable);
    }
    else{
       MergeSortOption5(productsTable, salesTable, customersTable, exchangesTable);
    }
    end = clock();
    double seconds =((double)(end-start))/CLOCKS_PER_SEC; //varible used to store the time in seconds calculated
    printf("------------------------------------------------------------------------------------------------------------------------\n"); 
    printf("Time used to produce this listing: ");
    PrintExecutionTime(seconds);
    printf("\n***************************LAST LINE OF THE REPORT***************************\n");
    printf("------------------------------------------------------------------------------------------------------------------------\n"); 
    return 1;
}

// Function to direct option 4 of menu
// salesTable: variables used to store the name of the table file
// mode: variable used to store the type of sort to use
int Option4(char salesTable[], int mode){
  
    int start = 0, end = 0; //variables used to store the start and finish of the ejecution
    start = clock();
    printf("------------------------------------------------------------------------------------------------------------------------\n");
    printf("\033[1mCompany Global Electronics Retailer\033[0m\n");
    PrintDate(); 
    printf("\n\033[1mTitle: Analysis of the Average Delivery Time in Days and Its Change Over Time for Company Global Electronics Retailer\033[0m\n");
    if(mode){
       BubbleSortOption4(salesTable);
    }
    else{
       MergeSortOption4(salesTable);
    }
    end = clock();
    double seconds =((double)(end-start))/CLOCKS_PER_SEC; //varible used to store the time in seconds calculated
    printf("------------------------------------------------------------------------------------------------------------------------\n"); 
    printf("Time used to produce this listing: ");
    PrintExecutionTime(seconds);
    printf("\n***************************LAST LINE OF THE REPORT***************************\n");
    printf("------------------------------------------------------------------------------------------------------------------------\n"); 
    return 1;
}

// Function to direct option 3 of menu
// sales/productsTable: variables used to store the name of the table file
// mode: variable used to store the type of sort to use
int Option3(char salesTable[], char productsTable[], int mode){
  
    int start = 0, end = 0; //variables used to store the start and finish of the ejecution
    start = clock();
    printf("------------------------------------------------------------------------------------------------------------------------\n");
    printf("\033[1mCompany Global Electronics Retailer\033[0m\n");
    PrintDate(); 
    printf("\n\033[1mTitle: Analysis of Seasonal Patterns in Orders and Income for Company Global Electronics Retailer\033[0m\n");
    if(mode){
       BubbleSortOption3(salesTable, productsTable);
    }
    else{
       MergeSortOption3(salesTable, productsTable);
    }
    end = clock();
    double seconds =((double)(end-start))/CLOCKS_PER_SEC; //varible used to store the time in seconds calculated
    printf("------------------------------------------------------------------------------------------------------------------------\n"); 
    printf("Time used to produce this listing: ");
    PrintExecutionTime(seconds);
    printf("\n***************************LAST LINE OF THE REPORT***************************\n");
    printf("------------------------------------------------------------------------------------------------------------------------\n"); 
    return 1;
}

// Function to direct option 2 of menu
// customers/products/salesTable: variables used to store the name of the table files
// mode: variable used to store the type of sort to use
int Option2(char productsTable[], char salesTable[], char customersTable[], int mode){

    int start = 0, end = 0; //variables used to store the start and finish of the ejecution
    start = clock();
    printf("------------------------------------------------------------------------------------------------------------------------\n");
    printf("\033[1mCompany Global Electronics Retailer\033[0m\n");
    PrintDate(); 
    printf("Products list ordered by ProductName  + Continent + Country + Sate + City\n");
    if(mode){
      BubbleSortOption2(productsTable, salesTable, customersTable);
    }
    else{
      MergeSortOption2(productsTable, salesTable, customersTable);
    }
    end = clock();
    double seconds =((double)(end-start))/CLOCKS_PER_SEC; //varible used to store the time in seconds calculated
    printf("------------------------------------------------------------------------------------------------------------------------\n"); 
    printf("Time used to produce this listing: ");
    PrintExecutionTime(seconds);
    printf("\n***************************LAST LINE OF THE REPORT***************************\n");
    printf("------------------------------------------------------------------------------------------------------------------------\n"); 
    return 1;
}

// Function to create the table files
// customers/exchanges/products/sales/storesFile: variables used to store the name of files
// customers/exchanges/products/sales/storesName: variables used to store the name of the Tables files
int CreateFiles(char customersFile[], char exchangesFile[], char productsFile[], char salesFile[], char storesFile[],
                char customersName[], char exchangesName[], char productsName[], char salesName[], char storesName[]){
  
   int numberOfRecords = CalculateNumberOfRecords(customersFile, sizeof(customersRecords), 0); // numberOfRecords: variable used to store the number of records
   FILE *customersPointer = fopen(customersFile, "r");// customersPointer: variable used to store the memory address of the file
   if (customersPointer == NULL) {
      printf("1. ERROR opening customers file.\n");
      return 0;
   }
   if (CreateCostumersTable(customersPointer, customersName) == numberOfRecords) {
      numberOfRecords = CalculateNumberOfRecords(exchangesFile, sizeof(exchangeRatesRecords), 0);
      FILE *exchangesPointer = fopen(exchangesFile, "r"); // exchangesPointer: variable used to store the memory address of the file
      if (exchangesPointer == NULL) {
          printf("1. ERROR opening exchanges file.\n");
          return 0;
      }
      if (CreateExchangesTable(exchangesPointer, exchangesName) == numberOfRecords) {
          numberOfRecords = CalculateNumberOfRecords(productsFile, sizeof(productsRecords), 0);
          FILE *productsPointer= fopen(productsFile, "r"); // productsPointer: variable used to store the memory address of the file
          if (productsPointer == NULL) {
              printf("1. ERROR openning products file.\n");
              return 0;
          }
          if (CreateProductsTable(productsPointer, productsName) == numberOfRecords) {
              numberOfRecords = CalculateNumberOfRecords(salesFile, sizeof(salesRecords), 0);
              FILE *salesPointer = fopen(salesFile, "r"); // salesPointer: variable used to store the memory address of the file
              if (salesPointer == NULL) {
                 printf("1. ERROR opening sales file.\n");
                 return 0;
              }
              if (CreateSalesTable(salesPointer, salesName) == numberOfRecords) {
                  numberOfRecords = CalculateNumberOfRecords(storesFile, sizeof(storesRecords), 0);
                  FILE *storesPointer = fopen(storesFile, "r"); // storesPointer: variable used to store the memory address of the file
                  if (storesPointer == NULL) {
                      printf("1. ERROR opening stores file.\n");
                      return 0;
                  }
                  if (CreateStoresTable(storesPointer, storesName) == numberOfRecords) {
                      return 1;
                  }
              }
          }
      }
  }
  else{
    return 0; 
  }
}

// Function to show the options to the users
// customers/exchanges/products/sales/storesFile: variables used to store the name of files
// customers/exchanges/products/sales/storesName: variables used to store the name of the Tables files
// timesTablesCreated: variable used to store the times that the users create the tables
void MenuOptions(char customersFile[], char exchangesFile[], char productsFile[], char salesFile[], char storesFile[],
                 char customersName[], char exchangesName[], char productsName[], char salesName[], char storesName[], int timesTablesCreated){
  
  char option[4] = " "; //option: variable used to store the number that the user insert
  printf("\033[1mCompany Global Electronics Retailer\033[0m\nOptions menu\n0. Exit program\n1. Construction of the Database with the dataset tables\n");
  printf("2. List of ¿What types of products does the company sell, and where are customers located?\n");
  printf("   2.1 Utility bubbleSort\n   2.2 Utility mergeSort\n");
  printf("3. List of ¿ Are there any seasonal patterns or trends for order volume or revenue?\n");
  printf("   3.1 Utility bubbleSort\n   3.2 Utility mergeSort\n");
  printf("4. List of ¿How long is the average delivery time in days? Has that changed over time?\n");
  printf("   4.1 Utility bubbleSort\n   4.2 Utility mergeSort\n");
  printf("5. List of sales order by “Costumer Name”+”Order Date”+”ProductKey”;\n");
  printf("   5.1 Utility bubbleSort\n   5.2 Utility mergeSort\n");
  printf("What is your option: ");
  scanf("%s", option);
  if (strcmp(option, "0") == 0){
    printf("------------------------------------------------------------------------------------------------------------------------\n");
    printf("GoodBye\n");
    printf("------------------------------------------------------------------------------------------------------------------------\n");
  }
  else if (strcmp(option, "1") == 0){
    if(CreateFiles(customersFile, exchangesFile, productsFile, salesFile, storesFile, customersName, exchangesName, productsName, salesName, storesName)){
    printf("------------------------------------------------------------------------------------------------------------------------\n");
    printf("The construction of the Database with the dataset tables was succesfully.\n");
    printf("------------------------------------------------------------------------------------------------------------------------\n");
    MenuOptions(customersFile, exchangesFile, productsFile, salesFile, storesFile, customersName, exchangesName, productsName, salesName, storesName, timesTablesCreated+1);
    }
  }
  else if ((strcmp(option, "2.1") == 0)&&(timesTablesCreated > 0)){
    Option2( productsName, salesName, customersName, 1);
    MenuOptions(customersFile, exchangesFile, productsFile, salesFile, storesFile, customersName, exchangesName, productsName, salesName, storesName, timesTablesCreated);
    }
   else if((strcmp(option, "2.2") == 0)&&(timesTablesCreated > 0)){
    Option2( productsName, salesName, customersName, 0);
    MenuOptions(customersFile, exchangesFile, productsFile, salesFile, storesFile, customersName, exchangesName, productsName, salesName, storesName, timesTablesCreated);
    }
    else if((strcmp(option, "3.1") == 0)&&(timesTablesCreated > 0)){
      Option3(salesName, productsName, 1);
      MenuOptions(customersFile, exchangesFile, productsFile, salesFile, storesFile, customersName, exchangesName, productsName, salesName, storesName, timesTablesCreated);
    }
     else if((strcmp(option, "3.2") == 0)&&(timesTablesCreated > 0)){
      Option3(salesName, productsName, 0);
      MenuOptions(customersFile, exchangesFile, productsFile, salesFile, storesFile, customersName, exchangesName, productsName, salesName, storesName, timesTablesCreated);
    }
    else if((strcmp(option, "4.1") == 0)&&(timesTablesCreated > 0)){
      Option4(salesName, 1);
      MenuOptions(customersFile, exchangesFile, productsFile, salesFile, storesFile, customersName, exchangesName, productsName, salesName, storesName, timesTablesCreated);
    }
    else if((strcmp(option, "4.2") == 0)&&(timesTablesCreated > 0)){
      Option4(salesName, 0);
      MenuOptions(customersFile, exchangesFile, productsFile, salesFile, storesFile, customersName, exchangesName, productsName, salesName, storesName, timesTablesCreated);
    }
    else if((strcmp(option, "5.1") == 0)&&(timesTablesCreated > 0)){
      Option5(productsName, salesName, customersName, exchangesName, 1);
      MenuOptions(customersFile, exchangesFile, productsFile, salesFile, storesFile, customersName, exchangesName, productsName, salesName, storesName, timesTablesCreated);
    }
    else if((strcmp(option, "5.2") == 0)&&(timesTablesCreated > 0)){
      Option5(productsName, salesName, customersName, exchangesName, 0);
      MenuOptions(customersFile, exchangesFile, productsFile, salesFile, storesFile, customersName, exchangesName, productsName, salesName, storesName, timesTablesCreated);
    }
    else if(strlen(option) == 1 && option != "1"){
      printf("------------------------------------------------------------------------------------------------------------------------\n");
      printf("The option isn't valid, please try again.\n");
      printf("------------------------------------------------------------------------------------------------------------------------\n");
      MenuOptions(customersFile, exchangesFile, productsFile, salesFile, storesFile, customersName, exchangesName, productsName, salesName, storesName, timesTablesCreated);
   
    }
    else{
      printf("------------------------------------------------------------------------------------------------------------------------\n");
      printf("There is no built dataset tables, please enter option 1.\n");
      printf("------------------------------------------------------------------------------------------------------------------------\n");
      MenuOptions(customersFile, exchangesFile, productsFile, salesFile, storesFile, customersName, exchangesName, productsName, salesName, storesName, timesTablesCreated);
    }

}

// Principal function to try the program
int main(){

  MenuOptions("Customers.dat", "Exchange_Rates.dat", "Products.dat", "Sales.dat", "Stores.dat",
              "CustomersTable", "ExchangesTable", "ProductsTable", "SalesTable", "StoresTable", 0);          
  return 0;
}