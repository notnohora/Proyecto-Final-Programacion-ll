#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <ctype.h>
#include <time.h>
#include "Structs.h"

// Function to search throw the recods
// filePointer: variable used to store a memory address of a file
// valueToSearch: variable used to store an atribute
// mode: variable used to store the number of which file 
int BinarySearchOption2(FILE *filePointer, unsigned int valueToSearch, int mode) {
  
    int start = 0; // start: variable used to store the initial value of the search
    int middle = 0;  // middle: variable used to store the medium value of the search
    int sizeOfRecord = 0; // sizeofRecord: variable used to store the size of the struct
    unsigned int atribute = 0; // atribute: variable used to store the atribute
    fseek(filePointer, 0, SEEK_END);
    if(mode == 1){
        sizeOfRecord = sizeof(productsRecords);
    } else if(mode == 2){
        sizeOfRecord = sizeof(customersRecords);    
    } else if(mode == 3){
        sizeOfRecord = sizeof(salesRecords);
    } else {
        return -1;
    }
    int end = (ftell(filePointer) / sizeOfRecord) - 1; // end: variable used to store the final value of the search

    while(start <= end) {
        middle = start + ((end - start) / 2);
        fseek(filePointer, middle * sizeOfRecord, SEEK_SET);

        if (mode == 1){
            productsRecords record;
            fread(&record, sizeOfRecord, 1, filePointer);
            atribute = (unsigned int) record.productKey;
        } else if (mode == 2){
            customersRecords record;
            fread(&record, sizeOfRecord, 1, filePointer);
            atribute = (unsigned int) record.customerKey;
        } else if (mode == 3){
            salesRecords record;
            fread(&record, sizeOfRecord, 1, filePointer);
            atribute = (unsigned int) record.productKey;
        } else {
            return -1;
        }

        if(valueToSearch == atribute){
            return middle;
        } else if(valueToSearch > atribute){
            start = middle + 1;
        } else {
            end = middle - 1;
        }
    }
    //printf("%i %i %i\n", start, middle, end);
    return -1;
}

// Function to bubble sort the sales table file
// nameFile: variable used to store the name of a file
// numberOfRecords : variable used to store the number of records
void BubbleSortSalesOption2(char nameFile[], int numberOfRecords) {

    FILE *pointerSales = fopen(nameFile, "rb+"); //pointersales: variable used to store a memory address of the file
    salesRecords record1, record2; // variables used to store a struct
    if ( pointerSales == NULL) {
        printf("2.1 ERROR opening sales table file.\n\n");
        return;
    }

    // Bubble Sort: comparando y escribiendo en el archivo
    for (int i = 0; i < numberOfRecords - 1; i++) {
        //printf("sales %i ", i);
        for (int j = 0; j < numberOfRecords - i - 1; j++) {
            fseek(pointerSales, sizeof(salesRecords) * j, SEEK_SET);
            fread(&record1, sizeof(salesRecords), 1, pointerSales);

            fseek(pointerSales, sizeof(salesRecords) * (j + 1), SEEK_SET);
            fread(&record2, sizeof(salesRecords), 1, pointerSales);

            if (record1.productKey > record2.productKey) {
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

// Function to bubble sort the customers table file
// nameFile: variable used to store the name of a file
// numberOfRecords : variable used to store the number of records
void BubbleSortCostumersOption2(char nameFile[], int numberOfRecords) {

    FILE *pointerCustomers = fopen(nameFile, "rb+"); //pointerCustomers: variable used to store a memory address of the file
    customersRecords record1, record2; // variables used to store a struct
    if (pointerCustomers == NULL) {
        printf("2.1 ERROR opening the customer table file.\n");
        return;
    }

    // Bubble Sort: comparando y escribiendo en el archivo
    for (int i = 0; i < numberOfRecords - 1; i++) {
        //printf("clientes %i", i);
        for (int j = 0; j < numberOfRecords - i - 1; j++) {
            fseek(pointerCustomers, sizeof(customersRecords) * j, SEEK_SET);
            fread(&record1, sizeof(customersRecords), 1, pointerCustomers);

            fseek(pointerCustomers, sizeof(customersRecords) * (j + 1), SEEK_SET);
            fread(&record2, sizeof(customersRecords), 1, pointerCustomers);

            if (record1.customerKey > record2.customerKey) {
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

// Function to bubble sort the products table file
// nameFile: variable used to store the name of a file
// numberOfRecords : variable used to store the number of records
void BubbleSortProductsOption2(char nameFile[], int numberOfRecords) {

    FILE *pointerProducts = fopen(nameFile, "rb+"); // pointerProducts: variable used to store a memory address of a file
    if (pointerProducts == NULL) {
        printf("2.1 ERROR opening the products table file.\n");
        return;
    }
   productsRecords record1, record2;// variables used to store a struct

    // Bubble Sort: comparando y escribiendo en el archivo
    for (int i = 0; i < numberOfRecords - 1; i++) {
        //printf("productos %i", i);
        for (int j = 0; j < numberOfRecords - i - 1; j++) {
            fseek(pointerProducts, sizeof(productsRecords) * j, SEEK_SET);
            fread(&record1, sizeof(productsRecords), 1, pointerProducts);

            fseek(pointerProducts, sizeof(productsRecords) * (j + 1), SEEK_SET);
            fread(&record2, sizeof(productsRecords), 1, pointerProducts);

            if (strcmp(record1.productName, record2.productName) > 0) {
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

// Functions to compares atributes
// a/b: variables used to stores a any type pointer
int functionCompareInProductsByProductName( void* a, void* b){
    productsRecords *product1 = (productsRecords*)a;
    productsRecords *product2 = (productsRecords*)b;
    return strcmp((*product1).productName, (*product2).productName);
}
int functionCompareInCustomersByCustomerKey( void* a, void* b){
    customersRecords *customer1 = (customersRecords*)a;
    customersRecords *customer2 = (customersRecords*)b;
    return (*customer1).customerKey - (*customer2).customerKey;
}
int functionCompareInSalesByProductKey( void* a, void* b){
    salesRecords *sale1 = (salesRecords*)a;
    salesRecords *sale2 = (salesRecords*)b;
    return (*sale1).productKey - (*sale2).productKey;
}
int functionCompareInCustomersLocation( void* a, void* b){
    customersRecords *customer1 = (customersRecords*)a;
    customersRecords *customer2 = (customersRecords*)b;
    int comparison = strcmp((*customer1).continent, (*customer2).continent);// comparison: variable used to store a number acording the comparison
    if (comparison != 0){
        return comparison;
    }
    comparison = strcmp((*customer1).country, (*customer2).country);
    if(comparison != 0){
        return comparison;
    }
    comparison = strcmp((*customer1).state, (*customer2).state);
    if(comparison != 0){
            return comparison;
    }
    comparison = strcmp((*customer1).city, (*customer2).city);
    return comparison;
}

// Function to merge sort the files
// file: variable used to store a file pointer
// letf: variable used to store where start to sorting
// rigth: variable used to store where stop sorting
// middle: variable used to store the middle of the array
// recordSize: variable used to store the size of the record
// function: variable used to store the name of a function with those parameters
void MergeFileOption2(FILE *file, int left, int right, int medium, int recordSize, int (*function)(void*, void*)) {
    
    int firstArray = medium - left + 1;//variable used to store the num of records in one side
    int secondArray = right - medium;//variable used to store the num of records in one side
    FILE *leftFile = fopen("tempLeft.bin", "wb+");//variable used to store a memory addres of a file
    FILE *rightFile = fopen("tempRight.bin", "wb+");//variable used to store a memory addres of a file
    void *buffer = malloc(recordSize);//variable to store a struct 

    // put the firts part in the temporal file
    for (int i = 0; i < firstArray; i++) {
        fseek(file, (left + i) * recordSize, SEEK_SET);
        fread(buffer, recordSize, 1, file);
        fwrite(buffer, recordSize, 1, leftFile);
    }

     // put the second part in the temporal file
    for (int j = 0; j < secondArray; j++) {
        fseek(file, (medium + 1 + j) * recordSize, SEEK_SET);
        fread(buffer, recordSize, 1, file);
        fwrite(buffer, recordSize, 1, rightFile);
    }

    fseek(leftFile, 0, SEEK_SET);
    fseek(rightFile, 0, SEEK_SET);

    int i = 0, j = 0;
    int posicion = left;

    // merge and sort both parts in the original file
    while (i < firstArray && j < secondArray) {
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
    while (i < firstArray) {
        fseek(leftFile, i * recordSize, SEEK_SET);
        fread(buffer, recordSize, 1, leftFile);
        fseek(file, posicion * recordSize, SEEK_SET);
        fwrite(buffer, recordSize, 1, file);
        i++;
        posicion++;
    }

    // copy the rest of elements in the rigth temporal
    while (j < secondArray) {
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

void MergeSortFileOption2(FILE *file, int left, int right, int recordSize, int (*function)(void*, void*)) {
    if (left < right) {
        int medium = left + ((right - left) / 2);
        MergeSortFileOption2(file, left, medium, recordSize, function);
        MergeSortFileOption2(file, medium + 1, right, recordSize, function);
        MergeFileOption2(file, left, right, medium, recordSize, function);
    }
}

// Function to determinate and print the location of the customers
// pointerProducts: variable used to store a memory address of a file
// pointerSales: variable used to store a memory address of a file
// pointerCustomers: variable used to store a memory address of a file
// numOfProducts: variable used to store the number of products records
// mode: variable used to store the number of the mode sort
void DeterminatedCustomersLocation(FILE *pointerProducts, FILE *pointerSales, FILE *pointerCustomers, int numOfProducts, int mode){
  	
    productsRecords products; // products: variable used to store the products struct
	customersRecords customers; // customers: variable used to store the customers struct
	salesRecords sales; // sales: variable used to store the sales struct

	char productName[200] = ""; // productName: variable used to store the product name
	unsigned short int productKey = 0; // productKey: variable used to store the product nkey
	unsigned int customerKey = 0; // customerKey: variable used to store the customer key

	for(int i = 0; i < numOfProducts; i++){
    	fseek(pointerProducts, sizeof(productsRecords) * i, SEEK_SET);
    	fread(&products, sizeof(products), 1, pointerProducts);

    	strcpy(productName, products.productName);
    	printf("\n%s\n", productName);

		productKey = products.productKey;

    	int positionSales = BinarySearchOption2(pointerSales, productKey, 3); // positionSales: variable used to store the the position of that productkey in the sales table

    	if( positionSales == -1){
    		printf(" - No sales reported\n");
    	} else {
    		FILE *fpTemporal = fopen("TemporalFileOption2", "wb+"); // fpTemporal: variable used to store the memory address of a file
			if (fpTemporal == NULL) {
			    printf("Error opening temporal file.\n");
			    return;
			}

    	    printf("\n%-35s %-35s %-35s %-35s\n", "Continent", "Country", "State", "City");
    	    printf("_______________________________________________________________________________________________________________________________________\n");

    	    fseek(pointerSales, sizeof(salesRecords) * (positionSales - 1), SEEK_SET);
    	    fread(&sales, sizeof(salesRecords), 1, pointerSales);

    	    for( int i = positionSales - 1; i >= 0 && productKey == sales.productKey; i -= 1){
    	    	fseek(pointerSales, sizeof(salesRecords) * (i - 1), SEEK_SET);
    	    	fread(&sales, sizeof(salesRecords), 1, pointerSales);

    	    	positionSales = i;
    	    }

			fseek(pointerSales, sizeof(salesRecords) * positionSales, SEEK_SET);
			fread(&sales, sizeof(salesRecords), 1, pointerSales);

			int index = positionSales, numOfBuyers = 0, positionCustomers = 0; // variables used to store the number of record, number of buyers and the customer position respectively

    	    while (sales.productKey == productKey && index <= CalculateNumberOfRecords("SalesTable", sizeof(salesRecords), 1)){
				customerKey = sales.customerKey;

				positionCustomers = BinarySearchOption2(pointerCustomers, customerKey, 2);

				if (positionCustomers != -1) {
                    fseek(pointerCustomers, sizeof(customersRecords) * positionCustomers, SEEK_SET);
                    fread(&customers, sizeof(customersRecords), 1, pointerCustomers);

                    fwrite(&customers, sizeof(customersRecords), 1, fpTemporal);
                    numOfBuyers++;
                }

				fseek(pointerSales, sizeof(salesRecords) * (index + 1), SEEK_SET);
				fread(&sales, sizeof(salesRecords), 1, pointerSales);

				index++;
			}
			
		customersRecords record1, record2; //variables used to store customers struct
		if(mode){
            for (int i = 0; i < numOfBuyers - 1; i++) {
        for (int j = 0; j < numOfBuyers - i - 1; j++) {
            // move the pointer to the record position to compare
            fseek(fpTemporal, sizeof(customersRecords) * j, SEEK_SET);
            fread(&record1, sizeof(customersRecords), 1, fpTemporal);

            fseek(fpTemporal, sizeof(customersRecords) * (j + 1), SEEK_SET);
            fread(&record2, sizeof(customersRecords), 1, fpTemporal);

            // Compare continent
            int comparation = strcmp(record1.continent, record2.continent);
            if (comparation > 0) {
                // change records
                fseek(fpTemporal, sizeof(customersRecords) * j, SEEK_SET);
                fwrite(&record2, sizeof(customersRecords), 1, fpTemporal);

                fseek(fpTemporal, sizeof(customersRecords) * (j + 1), SEEK_SET);
                fwrite(&record1, sizeof(customersRecords), 1, fpTemporal);
            } else if (comparation == 0) {
                // if continents are equals, compare country
                comparation = strcmp(record1.country, record2.country);
                if (comparation > 0) {
                    fseek(fpTemporal, sizeof(customersRecords) * j, SEEK_SET);
                    fwrite(&record2, sizeof(customersRecords), 1, fpTemporal);

                    fseek(fpTemporal, sizeof(customersRecords) * (j + 1), SEEK_SET);
                    fwrite(&record1, sizeof(customersRecords), 1, fpTemporal);
                } else if (comparation == 0) {
                    // if country are equals, compare state
                    comparation = strcmp(record1.state, record2.state);
                    if (comparation > 0) {
                        fseek(fpTemporal, sizeof(customersRecords) * j, SEEK_SET);
                        fwrite(&record2, sizeof(customersRecords), 1, fpTemporal);

                        fseek(fpTemporal, sizeof(customersRecords) * (j + 1), SEEK_SET);
                        fwrite(&record1, sizeof(customersRecords), 1, fpTemporal);
                    } else if (comparation == 0) {
                        // if state are equals, compare city
                        comparation = strcmp(record1.city, record2.city);
                        if (comparation > 0) {
                            fseek(fpTemporal, sizeof(customersRecords) * j, SEEK_SET);
                            fwrite(&record2, sizeof(customersRecords), 1, fpTemporal);

                            fseek(fpTemporal, sizeof(customersRecords) * (j + 1), SEEK_SET);
                            fwrite(&record1, sizeof(customersRecords), 1, fpTemporal);
                        }
                    }
                }
            }
        }
    }
}
        else{
            MergeSortFileOption2(fpTemporal, 0, numOfBuyers-1, sizeof(customersRecords), functionCompareInCustomersLocation);
        }
			


			for(int i  = 0; i < numOfBuyers ; i++){
				fseek(fpTemporal, sizeof(customersRecords) * i, SEEK_SET);
				fread(&customers, sizeof(customersRecords), 1, fpTemporal);

				printf("%-35s %-35s %-35s %-35s\n", customers.continent, customers.country, customers.state, customers.city);
			}
			fclose(fpTemporal);
 		}
	}
}

// Function to direct the bubble sort option
// products/sales/customersTable: variables used to store the name of the tables file
void BubbleSortOption2(char productsTable[], char salesTable[], char customersTable[]){

    int numberOfProducts = CalculateNumberOfRecords( productsTable, sizeof(productsRecords), 1); // numberOfProducts: variable used to store the number of products records
	int numberOfCustomers = CalculateNumberOfRecords( customersTable, sizeof(customersRecords), 1); // numberOfCustomers: variable used to store the number of customers records
	int numberOfSales = CalculateNumberOfRecords( salesTable, sizeof(salesRecords), 1); // numberOfSales: variable used to store the number of sales records
	
    BubbleSortSalesOption2(salesTable, numberOfSales);	
    BubbleSortProductsOption2(productsTable, numberOfProducts);	
	BubbleSortCostumersOption2(customersTable, numberOfCustomers);		

	FILE *pointerProducts = fopen(productsTable, "rb+"); // pointerProducts: variable used to store a memory address of a file table 
    FILE *pointerCustomers = fopen(customersTable, "rb+"); // pointerCustomers: variable used to store a memory address of a file table 
    FILE *pointerSales = fopen(salesTable, "rb+");	//pointerSales: variable used to store a memory address of a file table 
	if (pointerProducts == NULL){
		printf("2.1 Error opening the products table file.\n");
		return;
	}
	if (pointerCustomers == NULL){
		printf("2.1 Error opening the customers table file.\n");
		return;
	}
	if (pointerSales == NULL){
		printf("2.1 Error opening the sales table file.\n");
		return;
	}
	DeterminatedCustomersLocation( pointerProducts, pointerSales, pointerCustomers, numberOfProducts, 1);
    
    fclose(pointerProducts);fclose(pointerCustomers);fclose(pointerSales);
}

// Function to direct the merge sort option
// products/sales/customersTable: variables used to store the name of the tables file
void MergeSortOption2(char productsTable[], char salesTable[], char customersTable[]){

    int numberOfProducts = CalculateNumberOfRecords( productsTable, sizeof(productsRecords), 1); // numberOfProducts: variable used to store the number of products records
	int numberOfCustomers = CalculateNumberOfRecords( customersTable, sizeof(customersRecords), 1); // numberOfCustomers: variable used to store the number of customers records
	int numberOfSales = CalculateNumberOfRecords( salesTable, sizeof(salesRecords), 1); // numberOfSales: variable used to store the number of sales records
	
    FILE *pointerProducts = fopen(productsTable, "rb+"); // pointerProducts: variable used to store a memory address of a file table 
    FILE *pointerCustomers = fopen(customersTable, "rb+");	// pointerCustomers: variable used to store a memory address of a file table
    FILE *pointerSales = fopen(salesTable, "rb+");	//pointerSales: variable used to store a memory address of a file table 
	if (pointerProducts == NULL){
		printf("2.2 Error opening the products table file.\n");
		return;
	}
	if (pointerCustomers == NULL){
		printf("2.2 Error opening the customers table file.\n");
		return;
	}
	if (pointerSales == NULL){
		printf("2.2 Error opening the sales table file.\n");
		return;
	}
    MergeSortFileOption2(pointerSales, 0, numberOfSales-1, sizeof(salesRecords), functionCompareInSalesByProductKey);
    MergeSortFileOption2(pointerProducts, 0, numberOfProducts-1, sizeof(productsRecords), functionCompareInProductsByProductName);
    MergeSortFileOption2(pointerCustomers, 0, numberOfCustomers-1, sizeof(customersRecords), functionCompareInCustomersByCustomerKey);

    DeterminatedCustomersLocation( pointerProducts, pointerSales, pointerCustomers, numberOfProducts, 0);

    fclose(pointerProducts);fclose(pointerCustomers);fclose(pointerSales);
}