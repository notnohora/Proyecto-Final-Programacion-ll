#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <ctype.h>
#include <time.h>
#include "Structs.h"

// Function to merge sort the files
// file: variable used to store a file pointer
// letf: variable used to store where start to sorting
// rigth: variable used to store where stop sorting
// middle: variable used to store the middle of the array
// recordSize: variable used to store the size of the record
// function: variable used to store the name of a function with those parameters
void MergeFileOption5(FILE *file, int left, int right, int middle, int recordSize, int (*function)(void*, void*)) {

    int lenFirstPart= middle - left + 1;//variable used to store the num of records in one side
    int lenSecondPart = right - middle;//variable used to store the num of records in one side
    FILE *leftFile = fopen("tempLeft.bin", "wb+");//variable used to store a memory addres of a file
    FILE *rightFile = fopen("tempRight.bin", "wb+");//variable used to store a memory addres of a file

    void *buffer = malloc(recordSize);//variable to store a struct 

    // put the firts part in the temporal file
    for (int i = 0; i < lenFirstPart; i++) {
        fseek(file, (left + i) * recordSize, SEEK_SET);
        fread(buffer, recordSize, 1, file);
        fwrite(buffer, recordSize, 1, leftFile);
    }

    // put the second part in the temporal file
    for (int j = 0; j < lenSecondPart; j++) {
        fseek(file, (middle + 1 + j) * recordSize, SEEK_SET);
        fread(buffer, recordSize, 1, file);
        fwrite(buffer, recordSize, 1, rightFile);
    }

    fseek(leftFile, 0, SEEK_SET);
    fseek(rightFile, 0, SEEK_SET);

    int i = 0, j = 0;
    int posicion = left;

    // merge and sort both parts in the original file
    while (i < lenFirstPart && j < lenSecondPart) {
        fseek(leftFile, i * recordSize, SEEK_SET);
        fseek(rightFile, j * recordSize, SEEK_SET);

        void *leftRecord = malloc(recordSize);
        void *rightRecord = malloc(recordSize);

        fread(leftRecord, recordSize, 1, leftFile);
        fread(rightRecord, recordSize, 1, rightFile);

        if (function(leftRecord, rightRecord) <= 0) {
            fseek(file, posicion * recordSize, SEEK_SET);
            fwrite(leftRecord, recordSize, 1, file);
            i++;
        } else {
            fseek(file, posicion * recordSize, SEEK_SET);
            fwrite(rightRecord, recordSize, 1, file);
            j++;
        }

        posicion++;
        free(leftRecord);
        free(rightRecord);
    }

    // copy the rest of elements in the left temporal
    while (i < lenFirstPart) {
        fseek(leftFile, i * recordSize, SEEK_SET);
        fread(buffer, recordSize, 1, leftFile);
        fseek(file, posicion * recordSize, SEEK_SET);
        fwrite(buffer, recordSize, 1, file);
        i++;
        posicion++;
    }

    // copy the rest of elements in the rigth temporal
    while (j < lenSecondPart) {
        fseek(rightFile, j * recordSize, SEEK_SET);
        fread(buffer, recordSize, 1, rightFile);
        fseek(file, posicion * recordSize, SEEK_SET);
        fwrite(buffer, recordSize, 1, file);
        j++;
        posicion++;
    }

    free(buffer);fclose(leftFile);fclose(rightFile);
    remove("tempLeft.bin");remove("tempRight.bin");
}

void MergeSortFileOption5(FILE *file, int left, int right, int recordSize, int (*function)(void*, void*)) {
    if (left < right) {
        int medium = left + ((right - left) / 2);
        MergeSortFileOption5(file, left, medium, recordSize, function);
        MergeSortFileOption5(file, medium + 1, right, recordSize, function);
        MergeFileOption5(file, left, right, medium, recordSize, function);
    }
}

// Functions to compares atributes
// a/b: variables used to stores a any type pointer
int FunctionCompareInCustomersByName( void* a, void* b){
    customersRecords *customer1 = (customersRecords*)a;
    customersRecords *customer2 = (customersRecords*)b;
    return strcmp((*customer1).name, (*customer2).name);
}
int FunctionCompareInSalesByCustomerkey( void* a, void* b){
    salesRecords *sale1 = (salesRecords*)a;
    salesRecords *sale2 = (salesRecords*)b;
    return (*sale1).customerKey - (*sale2).customerKey;
}
int FunctionCompareInProductsByProductkey( void* a, void* b){
    productsRecords *product1 = (productsRecords*)a;
    productsRecords *product2 = (productsRecords*)b;
    return (*product1).productKey - (*product2).productKey;
}
int FunctionCompareInSalesByOrderNumber( void* a, void* b){
    salesRecords *sale1 = (salesRecords*)a;
    salesRecords *sale2 = (salesRecords*)b;
    long int comparation = (*sale1).orderNumber - (*sale2).orderNumber;

    if (comparation != 0)
    {
        return (int)comparation;
    }
    else
    {
        return (int)((*sale1).productKey - (*sale2).productKey);
    }
}
int FunctionCompareInExchangeByCurrencyCode( void* a, void* b){
    exchangeRatesRecords *exchange1 = (exchangeRatesRecords*)a;
    exchangeRatesRecords *exchange2 = (exchangeRatesRecords*)b;
    return strcmp((*exchange1).currency, (*exchange2).currency);
}

// Function to search throw the records
// filePointer: variable used to store a memory address of a file
// valueToSearch: variable used to store an atribute
// mode: variable used to store the number of which file 
int BinarySearchOption5(FILE *pointerFile, unsigned long int valueToSearch, int mode){

    //variables used to limited the search
    unsigned int start = 0, middle = 0, end = 0, sizeOfRecord = 0;
    // variable to store an atribute until is equal to the value to search
    unsigned long int key = 0;

    if (mode == 1){
        sizeOfRecord = sizeof(productsRecords);
    }
    else if (mode == 2){
        sizeOfRecord = sizeof(customersRecords);
    }
    else if (mode == 3){
        sizeOfRecord = sizeof(salesRecords);
    }
    else if (mode == 4){
        sizeOfRecord = sizeof(salesRecords);
    }
    else{
        return -1;
    }

    fseek(pointerFile, 0, SEEK_END);
    end = (ftell(pointerFile) / sizeOfRecord) - 1;
    while (start <= end){
        middle = start + ((end - start) / 2);
        fseek(pointerFile, middle * sizeOfRecord, SEEK_SET);

        if (mode == 1){
            productsRecords recordProduct;
            fread(&recordProduct, sizeOfRecord, 1, pointerFile);
            key = (unsigned long int)recordProduct.productKey;
        }
        else if (mode == 2){
            customersRecords recordCustomer;
            fread(&recordCustomer, sizeOfRecord, 1, pointerFile);
            key = (unsigned long int)recordCustomer.customerKey;
        }
        else if (mode == 3){
            salesRecords recordSale;
            fread(&recordSale, sizeOfRecord, 1, pointerFile);
            key = (unsigned long int)recordSale.productKey;
        }
        else if (mode == 4){
            salesRecords recordSale;
            fread(&recordSale, sizeOfRecord, 1, pointerFile);
            key = (unsigned long int)recordSale.customerKey;
        }
        else{
            return -1;
        }
        //printf("Start: %u\tEnd: %u\tMiddle: %u\tValueToSearch: %lu\tKey: %lu\n", start, end, middle, valueToSearch, key);
        if (key == valueToSearch){
            return middle;
        }
        else if (key < valueToSearch){
            start = middle + 1;
        }
        else{
            end = middle - 1;
        }
    }
    return -1;
}

// Function to search throw the records
// pointerFile: variable used to store a memory address of a file
// recordSale: variable used to store a struct
int BinarySearchExchangeDate(FILE *pointerFile, salesRecords recordSale){

    //variables used to limited the search
    unsigned int start = 0, middle = 0, end = 0, sizeOfRecord = sizeof(exchangeRatesRecords);
    //variable used to store a struct
    exchangeRatesRecords record;

    fseek(pointerFile, 0, SEEK_END);
    end = (ftell(pointerFile) / sizeOfRecord) - 1;
    while (start <= end){

        middle = start + ((end - start) / 2);
        fseek(pointerFile, middle * sizeOfRecord, SEEK_SET);
        fread(&record, sizeOfRecord, 1, pointerFile);

        char key[11] = "", currencyCode[3] = "";
        strcpy(key, record.date);
        strcpy(currencyCode, record.currency);

        //printf("Start: %u, Middle: %u, End: %u, Clave: '%s', Buscando: %d/%d/%d \n", start, middle, end, record.date, recordSale.orderDate.MM, recordSale.orderDate.DD, recordSale.orderDate.AAAA);
        //printf(",KeyAux: %s\n", key);

        int currentMonth = 0, currentDay = 0, currentYear = 0;
        int month = 0, day = 0, year = 0;
        month = recordSale.orderDate.MM;
        day = recordSale.orderDate.DD;
        year = recordSale.orderDate.AAAA;

        sscanf(record.date, "%d/%d/%d", &currentMonth, &currentDay, &currentYear);
        if (currentYear == year){
            if (currentMonth == month){
                if (currentDay == day){
                    return middle;
                }
                else if (currentDay > day){
                    end = middle - 1;
                }
                else{
                    start = middle + 1;
                }
            }
            else if (currentMonth > month){
                end = middle - 1;
            }
            else{
                start = middle + 1;
            }
        }
        else if (currentYear > year){
            end = middle - 1;
        }
        else{
            start = middle + 1;
        }
    }
    return -1;
}

// Function to show the purchases of each customer
// pointesSales/customers/products: vaiable used to store the memory adress of the fles
// exchangesName: variable used to store the name of a file
// numOfCustomers: variable used to store the num of records in a file
// mode: variable to identify the type of sort
void ShowCustomersPurchases(FILE *pointerSales, FILE *pointerCustomers, FILE *pointerProducts, char exchangesName[], int numOfCustomers, int mode){
	
    //variables used to store a struct
	salesRecords recordSale; 
	customersRecords recordCustomer;
	exchangeRatesRecords recordExchange;
	
	FILE *pointerExchangeRates = NULL;
	FILE *pointerTemporalSales = NULL;

    char customerName[40] = ""; //variable used to store the ProductName of each Product in ProductsTable
	unsigned int customerKey = 0; //variable used to store the customerkey of each customer

    for(int i = 0; i < numOfCustomers; i += 1){
        fseek(pointerCustomers, sizeof(customersRecords) * i, SEEK_SET);
    	fread(&recordCustomer, sizeof(customersRecords), 1, pointerCustomers);

        strcpy(customerName, recordCustomer.name);
        printf("\nCustomer name: %-40s", customerName);

        customerKey = recordCustomer.customerKey;
		//printf("%u\n", customerKey);
        int positionSales = BinarySearchOption5(pointerSales, customerKey, 4);
        if(positionSales == -1){
            printf(" - No purchases reported\n");
        } else {
			pointerTemporalSales = fopen("TemporalFileSalesOption5", "wb+");
			pointerExchangeRates = fopen(exchangesName, "rb+");

			if(pointerTemporalSales == NULL){
				printf("Error abiendo el temporal de sales\n");
				return;
			}
			if(pointerExchangeRates == NULL){
				printf("Error abriendo el archivo de exchange\n");
				return;
			}

            fseek(pointerSales, (positionSales - 1) * sizeof(salesRecords), SEEK_SET);
            fread(&recordSale, sizeof(salesRecords), 1, pointerSales);

            for( int i = positionSales - 1; i >= 0 && customerKey == recordSale.customerKey; i -= 1){
				//Reading of the previous record in sales to verify if its the first one with the currenr CustomerKey
    	    	fseek(pointerSales, sizeof(salesRecords) * (i - 1), SEEK_SET);
    	    	fread(&recordSale, sizeof(salesRecords), 1, pointerSales);

    	    	positionSales = i; //Changing positionSales to be the index of first record in sales with the current CustomerKey
    	    }
            fseek(pointerSales, positionSales * sizeof(salesRecords), SEEK_SET);
            fread(&recordSale, sizeof(salesRecords), 1, pointerSales);

            // variables used to store nums acording counts
            int numOfOrders = 0, totalNumOfPurchases = 0;
            //variable used to store the order number of each sale
			long orderNumber = recordSale.orderNumber;
            for(int index = positionSales, i = 0; recordSale.customerKey == customerKey && index < CalculateNumberOfRecords("SalesTable", sizeof(salesRecords), 1); index += 1, i += 1){
				//printf("\norder number: %li, order date: %hu/%d/%d, productKey: %d, quantity: %d, currency code: %s\n", recordSale.OrderNumber, recordSale.OrderDate.AAAA, recordSale.OrderDate.MM, recordSale.OrderDate.DD, recordSale.ProductKey, recordSale.Quantity, recordSale.CurrencyCode);
                fseek(pointerTemporalSales, i * sizeof(salesRecords), SEEK_SET);
				fwrite(&recordSale, sizeof(salesRecords), 1, pointerTemporalSales);
				totalNumOfPurchases++;
                fseek(pointerSales, (index + 1) * sizeof(salesRecords), SEEK_SET);
                fread(&recordSale, sizeof(salesRecords), 1, pointerSales);
				if(orderNumber != recordSale.orderNumber){
					numOfOrders++;
					orderNumber = recordSale.orderNumber;
				}
            }

			if(mode == 1){
				for(int step = 0; step < totalNumOfPurchases - 1; step += 1){
					salesRecords reg1, reg2;
					for(int i = 0; i < totalNumOfPurchases - step - 1; i += 1){
						fseek(pointerTemporalSales, sizeof(salesRecords) * i, SEEK_SET);
						fread(&reg1, sizeof(salesRecords), 1, pointerTemporalSales);

						fseek(pointerTemporalSales, sizeof(salesRecords) * (i + 1), SEEK_SET);
						fread(&reg2, sizeof(salesRecords), 1, pointerTemporalSales);
						int comparation = reg1.orderNumber - reg2.orderNumber;
						if(comparation == 0){
							comparation = reg1.productKey - reg2.productKey;
						}
						if(comparation > 0){
							fseek(pointerTemporalSales, sizeof(salesRecords) * i, SEEK_SET);
							fwrite(&reg2, sizeof(salesRecords), 1, pointerTemporalSales);

							fseek(pointerTemporalSales, sizeof(salesRecords) * (i + 1), SEEK_SET);
							fwrite(&reg1, sizeof(salesRecords), 1, pointerTemporalSales);
						}
					}
				}
			} else if(mode == 2){
				MergeSortFileOption5(pointerTemporalSales, 0, totalNumOfPurchases - 1, sizeof(salesRecords), FunctionCompareInSalesByOrderNumber);
			}

			//variable used to store a struct
			salesRecords tempRecordSale1, tempRecordSale2;

			fseek(pointerSales, positionSales * sizeof(salesRecords), SEEK_SET);
            fread(&recordSale, sizeof(salesRecords), 1, pointerSales);
			//variable used to store the count of orders for each customer
			int orderIndex = 0; 			
			float totalValue = 0.0;   //variabel used to store the total value   	

			for (int order = 0; order < numOfOrders; order += 1) {
			    
			    fseek(pointerTemporalSales, sizeof(salesRecords) * orderIndex, SEEK_SET);
			    fread(&tempRecordSale1, sizeof(salesRecords), 1, pointerTemporalSales);

			    
			    fseek(pointerTemporalSales, sizeof(salesRecords) * (orderIndex + 1), SEEK_SET);
			    fread(&tempRecordSale2, sizeof(salesRecords), 1, pointerTemporalSales);

			    float exchange = -1.0;// variable used to store the exchange to calculate the total value in that coin

				// variable used to search and store the position of the exchange of that order
				int positionExchange = BinarySearchExchangeDate(pointerExchangeRates, tempRecordSale1);
				if (positionExchange != -1){
					exchangeRatesRecords staticExchangeRecord;//variable used to store a struct
					fseek(pointerExchangeRates, sizeof(exchangeRatesRecords) * positionExchange, SEEK_SET);
					fread(&staticExchangeRecord, sizeof(exchangeRatesRecords), 1, pointerExchangeRates);
					int index = positionExchange;
					// printf("DATE:%s   currency:%s \n", staticExchangeRecord.Date, staticExchangeRecord.Currency);
					if (strcmp("USD", staticExchangeRecord.currency) != 0){
						fseek(pointerExchangeRates, sizeof(exchangeRatesRecords) * positionExchange, SEEK_SET);
						fread(&recordExchange, sizeof(exchangeRatesRecords), 1, pointerExchangeRates);
						for (; strcmp("USD", recordExchange.currency) != 0 && index > 0; index--){
							fseek(pointerExchangeRates, sizeof(exchangeRatesRecords) * index, SEEK_SET);
							fread(&recordExchange, sizeof(exchangeRatesRecords), 1, pointerExchangeRates);
						}
						index++;
					}

					// printf("date:%s   currency:%s" , recordExchange.Date , recordExchange.Currency );
					FILE *fpTemporalExchange = tmpfile();
					for (int i = 0; i < 5; i++, index++){
						fseek(pointerExchangeRates, sizeof(exchangeRatesRecords) * index, SEEK_SET);
						fread(&recordExchange, sizeof(exchangeRatesRecords), 1, pointerExchangeRates);
						fseek(fpTemporalExchange, sizeof(exchangeRatesRecords) * i, SEEK_SET);
						fwrite(&recordExchange, sizeof(exchangeRatesRecords), 1, fpTemporalExchange);
						// printf("date:%s   currency:%s exchange:%f \n", recordExchange.Date, recordExchange.Currency, recordExchange.Exchange);
					}

					int indexTemoralExchange = -1;
					if (tempRecordSale1.currencyCode[0] == 'U'){
						indexTemoralExchange = 0;
					}
					else if (tempRecordSale1.currencyCode[0] == 'C'){
						indexTemoralExchange = 1;
					}
					else if (tempRecordSale1.currencyCode[0] == 'A'){
						indexTemoralExchange = 2;
					}
					else if (tempRecordSale1.currencyCode[0] == 'E'){
						indexTemoralExchange = 3;
					}
					else if (tempRecordSale1.currencyCode[0] == 'G'){
						indexTemoralExchange = 4;
					}
					//printf ("\n\t indextemporal : %i" , indexTemoralExchange);
					fseek(fpTemporalExchange, sizeof(exchangeRatesRecords) * indexTemoralExchange, SEEK_SET);
					fread(&recordExchange, sizeof(exchangeRatesRecords), 1, fpTemporalExchange);
                
					exchange = recordExchange.exchanges;
					//printf("\n fecha : %s     exchange: %f\n", recordExchange.Date ,  exchange);
                   
					fclose(fpTemporalExchange);
				}

			    // Mostrar encabezado de la orden
			    printf("\nOrder date: %hu/%u/%u\tOrder Number: %li\n", 
			           tempRecordSale1.orderDate.AAAA, tempRecordSale1.orderDate.MM, tempRecordSale1.orderDate.DD, 
			           tempRecordSale1.orderNumber);
				printf("%-17s%-100s%-15s%s %s", "ProductKey", "ProductName", "Quantity", "Value", tempRecordSale1.currencyCode);
			    printf("_________________________________________________________________________________________________________________________________________________________\n");

			    float subTotal = 0.0;
			    productsRecords tempProductRecord;

			    // for of the products in the same order
			    while (orderIndex < totalNumOfPurchases - 1 && tempRecordSale1.orderNumber == tempRecordSale2.orderNumber) {
			    unsigned long int productKey = tempRecordSale1.productKey;

			        // search each products
			        int positionProducts = BinarySearchOption5(pointerProducts, productKey, 1);
			        if (positionProducts != -1) {
			            fseek(pointerProducts, sizeof(productsRecords) * positionProducts, SEEK_SET);
			            fread(&tempProductRecord, sizeof(productsRecords), 1, pointerProducts);

	                    //variable used to calculate the price for thata product in that quantity
			            float price = tempProductRecord.unitPriceUSD * exchange * tempRecordSale1.quantity;
			            printf("%-17hu%-100s%-15hu%.2f\n", tempProductRecord.productKey, tempProductRecord.productName, tempRecordSale1.quantity, price);
			            subTotal += price;
			        }

			        orderIndex++;

			        // update the date for the next iteration
			        fseek(pointerTemporalSales, sizeof(salesRecords) * orderIndex, SEEK_SET);
			        fread(&tempRecordSale1, sizeof(salesRecords), 1, pointerTemporalSales);

			        fseek(pointerTemporalSales, sizeof(salesRecords) * (orderIndex + 1), SEEK_SET);
			        fread(&tempRecordSale2, sizeof(salesRecords), 1, pointerTemporalSales);
			    }

			    // Processs the last product of each order
			    unsigned long int productKey = tempRecordSale1.productKey;

			    int positionProducts = BinarySearchOption5(pointerProducts, productKey, 1);
			    if (positionProducts != -1) {
			        fseek(pointerProducts, sizeof(productsRecords) * positionProducts, SEEK_SET);
			        fread(&tempProductRecord, sizeof(productsRecords), 1, pointerProducts);

			        float price = tempProductRecord.unitPriceUSD * exchange * tempRecordSale1.quantity;
                    //printf("ORDEN %i\n", orderIndex);
			        printf("%-17hu%-100s%-15hu%.2f\n", tempProductRecord.productKey, tempProductRecord.productName, tempRecordSale1.quantity, price);
			        subTotal += price;
			    }

			    orderIndex++; 
			    printf("_________________________________________________________________________________________________________________________________________________________\n");
				printf("%-117s%-15s%.2lf\n", "", "Subtotal", subTotal);
			    totalValue += subTotal;
			}

			
			printf("%-117s%-15s%.2lf\n", "", "TOTAL", totalValue);

		}
        printf("\n/--------------------------------------------------------------------------------------------------------------------------------------------------------/\n");
    	fclose(pointerTemporalSales); fclose(pointerExchangeRates);
	}
}

// Function to bubble sort the customers table file
// nameFile: variable used to store the name of a file
// numberOfRecords : variable used to store the number of records
void BubbleSortCustomersOption5(char nameFile[], int numberOfRecords) {

    FILE *pointerCustomers = fopen(nameFile, "rb+"); //pointerCustomers: variable used to store a memory address of the file
    customersRecords record1, record2;
    if (pointerCustomers == NULL) {
        printf("2. ERROR opening the customer table file.\n");
        return;
    }

    // Bubble Sort: comparando y escribiendo en el archivo
    for (int i = 0; i < numberOfRecords - 1; i++) {
        printf("clientes %i", i);
        for (int j = 0; j < numberOfRecords - i - 1; j++) {
            fseek(pointerCustomers, sizeof(customersRecords) * j, SEEK_SET);
            fread(&record1, sizeof(customersRecords), 1, pointerCustomers);

            fseek(pointerCustomers, sizeof(customersRecords) * (j + 1), SEEK_SET);
            fread(&record2, sizeof(customersRecords), 1, pointerCustomers);

            if (strcmp(record1.name, record2.name) > 0) {
                // Intercambiar los registros si están fuera de orden
                fseek(pointerCustomers, sizeof(customersRecords) * j, SEEK_SET);
                fwrite(&record2, sizeof(customersRecords), 1, pointerCustomers);

                fseek(pointerCustomers, sizeof(customersRecords) * (j + 1), SEEK_SET);
                fwrite(&record1, sizeof(customersRecords), 1, pointerCustomers);
            }
        }
    }
    fclose(pointerCustomers); 
    
}

void BubbleSortSalesOption5(char nameFile[], int numberOfRecords) {

    FILE *pointerSales = fopen(nameFile, "rb+"); //pointersales: variable used to store a memory address of the file
    salesRecords record1, record2;
    if ( pointerSales == NULL) {
        printf("2.1 ERROR opening sales table file.\n\n");
        return;
    }

    // Bubble Sort: comparando y escribiendo en el archivo
    for (int i = 0; i < numberOfRecords - 1; i++) {
        printf("sales %i ", i);
        for (int j = 0; j < numberOfRecords - i - 1; j++) {
            fseek(pointerSales, sizeof(salesRecords) * j, SEEK_SET);
            fread(&record1, sizeof(salesRecords), 1, pointerSales);

            fseek(pointerSales, sizeof(salesRecords) * (j + 1), SEEK_SET);
            fread(&record2, sizeof(salesRecords), 1, pointerSales);

            if (record1.customerKey > record2.customerKey) {
                // Intercambiar los registros si están fuera de orden
                fseek(pointerSales, sizeof(salesRecords) * j, SEEK_SET);
                fwrite(&record2, sizeof(salesRecords), 1, pointerSales);

                fseek(pointerSales, sizeof(salesRecords) * (j + 1), SEEK_SET);
                fwrite(&record1, sizeof(salesRecords), 1, pointerSales);
            }
        }
    }
    fclose(pointerSales); 
}

void BubbleSortProductsOption5(char nameFile[], int numberOfRecords) {

    FILE *pointerProducts = fopen(nameFile, "rb+"); // pointerProducts: variable used to store a memory address of a file
    if (pointerProducts == NULL) {
        printf("2.1 ERROR opening the products table file.\n");
        return;
    }
   productsRecords record1, record2;

    // Bubble Sort: comparando y escribiendo en el archivo
    for (int i = 0; i < numberOfRecords - 1; i++) {
        printf("productos %i", i);
        for (int j = 0; j < numberOfRecords - i - 1; j++) {
            fseek(pointerProducts, sizeof(productsRecords) * j, SEEK_SET);
            fread(&record1, sizeof(productsRecords), 1, pointerProducts);

            fseek(pointerProducts, sizeof(productsRecords) * (j + 1), SEEK_SET);
            fread(&record2, sizeof(productsRecords), 1, pointerProducts);

            if (record1.productKey > record2.productKey) {
                // Intercambiar los registros si están fuera de orden
                fseek(pointerProducts, sizeof(productsRecords) * j, SEEK_SET);
                fwrite(&record2, sizeof(productsRecords), 1, pointerProducts);

                fseek(pointerProducts, sizeof(productsRecords) * (j + 1), SEEK_SET);
                fwrite(&record1, sizeof(productsRecords), 1, pointerProducts);
            }
        }
    }
    
    
    fclose(pointerProducts);
}

void BubbleSortOption5(char productsName[], char salesName[], char customersName[], char exchangesName[]){
	int numberOfCustomers = CalculateNumberOfRecords(customersName, sizeof(customersRecords), 1); 
	int numberOfSales = CalculateNumberOfRecords(salesName, sizeof(salesRecords),1);			   
	int numberOfProducts = CalculateNumberOfRecords(productsName, sizeof(productsRecords),1);	  
    BubbleSortProductsOption5(productsName, numberOfProducts);
	BubbleSortCustomersOption5(customersName, numberOfCustomers);
	BubbleSortSalesOption5(salesName, numberOfSales);
	FILE *pointerCustomers = fopen(customersName, "rb+"); 
	FILE *pointerSales = fopen(salesName, "rb+");			
	FILE *pointerProducts = fopen(productsName, "rb+");
    if (pointerProducts == NULL){
		printf("5.1 Error opening the products table file.\n");
		return;
	}
	if (pointerCustomers == NULL){
		printf("5.1 Error opening the customers table file.\n");
		return;
	}
	if (pointerSales == NULL){
		printf("5.1 Error opening the sales table file.\n");
		return;
	}
 
	ShowCustomersPurchases(pointerSales, pointerCustomers, pointerProducts, exchangesName, numberOfCustomers, 1);

	fclose(pointerCustomers);fclose(pointerSales);fclose(pointerProducts);
}

void MergeSortOption5(char productsName[], char salesName[], char customersName[], char exchangesName[]){
	int numberOfCustomers = CalculateNumberOfRecords(customersName, sizeof(customersRecords), 1); 	//Quantity of products in CustomersTable
	int numberOfSales = CalculateNumberOfRecords(salesName, sizeof(salesRecords), 1); 				//Quantity of products in SalesTable
	int numberOfProducts = CalculateNumberOfRecords(productsName, sizeof(productsRecords), 1); 		//Quantity of products in ProductsTable
    FILE *pointerCustomers = fopen("CustomersTable", "rb+");	//Pointer to CustomersTable
    FILE *pointerSales = fopen("SalesTable", "rb+");			//Pointer to SalesTable	
    FILE *pointerProducts = fopen("ProductsTable", "rb+");

	if(pointerProducts == NULL){
		printf("5.2 Error opening the 'ProductsTable' File");
		return;
	}
	if (pointerCustomers == NULL){
		printf("5.2 Error opening the 'CustomersTable' File");
		return;
	}
	if (pointerSales == NULL){
		printf("5.2 Error opening the 'SalesTable' File");
		return;
	}

	MergeSortFileOption5(pointerCustomers, 0, numberOfCustomers - 1, sizeof(customersRecords), FunctionCompareInCustomersByName);

	MergeSortFileOption5(pointerSales, 0, numberOfSales - 1, sizeof(salesRecords), FunctionCompareInSalesByCustomerkey);

	MergeSortFileOption5(pointerProducts, 0, numberOfProducts - 1, sizeof(productsRecords), FunctionCompareInProductsByProductkey); 

	ShowCustomersPurchases(pointerSales, pointerCustomers, pointerProducts, exchangesName, numberOfCustomers, 2);

    fclose(pointerCustomers);fclose(pointerSales);fclose(pointerProducts);
}
