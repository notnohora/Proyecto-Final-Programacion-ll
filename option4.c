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
void MergeFileOption4(FILE *file, int left, int right, int middle, int recordSize, int (*function)(void*, void*)) {
   
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

void MergeSortFileOption4(FILE *file, int left, int right, int recordSize, int (*function)(void*, void*)) {
    if (left < right) {
        int medium = left + ((right - left) / 2);
        MergeSortFileOption4(file, left, medium, recordSize, function);
        MergeSortFileOption4(file, medium + 1, right, recordSize, function);
        MergeFileOption4(file, left, right, medium, recordSize, function);
    }
}

// Function to compare customerkey
// a/b: variable to store a pointer of any type
int FunctionCompareInSalesByCustomerkey4( void* a, void* b){
    salesRecords *sale1 = (salesRecords*)a;
    salesRecords *sale2 = (salesRecords*)b;
    return (*sale1).customerKey - (*sale2).customerKey;
}

// Function to calculate the avarage
// pointerSales: variable used to store a memory addres of a file
// mode: variable to sto store the type of sort
void CalculateAverageDeliveryTime(FILE *pointerSales, int numberOfSales, int mode){

    FILE *pointerTemporalSales = fopen("TemporalFileSalesOption4", "wb+"); //variable used to store the pointer of a temporal file for sales
    salesRecords recordsSales; //variable used to store sales struct
    int numOfDeliveredSales = 0; //variable used to store the number of delivery sales
    int averageDeliveryTime[6][2] = {0}; //array used to store the sum of sales and the number of sales
    char sortType[10]= ""; //variable used to store the name of the sort type

    for (int salesIndex = 0, temporalIndex = 0; salesIndex < numberOfSales; salesIndex += 1) {
        fseek(pointerSales, sizeof(salesIndex) * salesIndex, SEEK_SET);
        fread(&recordsSales, sizeof(salesRecords), 1, pointerSales);

        // Verify that is a sale from the online store and a delivery date valid
        if (recordsSales.storeKey == 0 && recordsSales.deliveryDate.AAAA != 0 && recordsSales.deliveryDate.MM != 0 && recordsSales.deliveryDate.DD != 0) {
            fseek(pointerTemporalSales, sizeof(salesRecords) * temporalIndex, SEEK_SET);
            fwrite(&recordsSales, sizeof(salesRecords), 1, pointerTemporalSales);
            temporalIndex++;  
            numOfDeliveredSales++;
        }
    }

    for (int i = 0; i < numOfDeliveredSales; i += 1) {
        fseek(pointerTemporalSales, sizeof(salesRecords) * i, SEEK_SET);
        fread(&recordsSales, sizeof(salesRecords), 1, pointerTemporalSales);

        // Calculate the diference of day if the delivery date is valid
        if (recordsSales.deliveryDate.AAAA != 0 && recordsSales.deliveryDate.MM != 0 && recordsSales.deliveryDate.DD != 0) {
            int year = (int)recordsSales.deliveryDate.AAAA % 2016;
            long int
            orderDate = recordsSales.orderDate.AAAA * 360 + recordsSales.orderDate.MM * 30 + recordsSales.orderDate.DD,
            deliveryDate = recordsSales.deliveryDate.AAAA * 360 + recordsSales.deliveryDate.MM * 30 + recordsSales.deliveryDate.DD,
            deliveryTimeInDays = deliveryDate - orderDate;
            averageDeliveryTime[year][0] += deliveryTimeInDays;
            averageDeliveryTime[year][1] += 1;
        
        } else {
            printf("\t\t\t\t\tOrderNumber: %li has an invalid Delivery Date.\n", recordsSales.orderNumber);
        }
    }
    
    if(mode == 1){
      strcpy(sortType, "Bubble");
    }
    if(mode == 2){
      strcpy(sortType, "Merge");
    }
    printf("\nIn this project, the average delivery time of orders has been calculated using the available dataset,\nwhich includes the order dates and delivery dates. To achieve this, the %s Sort algorithm was incluye\nimplemented  in ascending order, allowing for an accurate analysis of the\ntime elapsed between the order date and its delivery.\n\n", sortType);
    printf("Once the average delivery time in days was calculated, which\nrefers to the average number of days between the order date and the delivery date. This calculation\nis crucial to understand the efficiency of delivery times based on the provided data.\n\n");
    printf("It is important to note that, in the analyzed dataset, there are no records of physical stores with a\ndelivery date. Therefore, the comparison was made only between online stores that\ndo have valid delivery date records.\n\n");
    printf("\033[1mResults:\033[0m\n\n");
    printf("After analyzing the data, the following results were obtained for the average delivery time in days:\n\n");
    for(int i = 0; i < 6; i += 1){
        printf("\n-Average delivery time in: %i:\t%.2f days\n", i + 2016, (1.0 * averageDeliveryTime[i][0]) / (1.0 * averageDeliveryTime[i][1]));
    }
    printf("\nOver the years analyzed, a progressive decrease in delivery times is observed, suggesting that online\n stores have managed to improve their delivery efficiency over time.\n\n");
    printf("\033[1mConclusion:\033[0m\n\n");
    printf("In the provided dataset, there are no records for physical stores with delivery dates, so the analysis\nhas been limited to online. The calculation of average delivery time shows a trend of\nimprovement over the years, with a significant reduction in average delivery time, from 7.31 days in\n2016 to 3.45 days in 2021.\n\n");
    printf("This analysis highlights how, over time, delivery processes have improved, which may reflect\noptimization in logistical systems or operational efficiency in both online and local stores.\n\n");
    fclose(pointerTemporalSales);
}

// Function to direct the bubble sort option
// salesName: variable used to store the file's name
void BubbleSortOption4( char salesName[]){

	int numberOfSales = CalculateNumberOfRecords(salesName, sizeof(salesRecords), 1); // numberOfSales: variable used to store the number of records
    FILE *pointerSales = fopen(salesName, "rb+");// pointerSales: variable used to store a memory address of a file	
	if (pointerSales == NULL){
		printf("4.1 Error opening the Sales Table File");
		return;
	}

	for (int step = 0; step < numberOfSales - 1; step += 1) {
        //printf("Ordena Sales: %i\n", step);
		salesRecords reg1, reg2; // variables to store the sale struct
        for (int i = 0; i < numberOfSales - step - 1; i += 1) {
            fseek(pointerSales, sizeof(salesRecords) * i, SEEK_SET);
            fread(&reg1, sizeof(salesRecords), 1, pointerSales);

            fseek(pointerSales, sizeof(salesRecords) * (i + 1), SEEK_SET);
            fread(&reg2, sizeof(salesRecords), 1, pointerSales);

            if (reg1.customerKey > reg2.customerKey) {
                fseek(pointerSales, sizeof(salesRecords) * i, SEEK_SET);
                fwrite(&reg2, sizeof(salesRecords), 1, pointerSales);

                fseek(pointerSales, sizeof(salesRecords) * (i + 1), SEEK_SET);
                fwrite(&reg1, sizeof(salesRecords), 1, pointerSales);
            }
        }
    }
    CalculateAverageDeliveryTime(pointerSales, numberOfSales, 1);
    fclose(pointerSales);
}

// Function to direct the merge sort option
// salesName: variable used to store the file's name
void MergeSortOption4( char salesName[]){

	int numberOfSales = CalculateNumberOfRecords( salesName, sizeof(salesRecords), 1); //numberOfSales: variable used to store the number of records
    FILE *pointerSales = fopen( salesName, "rb+"); // pointerSales: variable used to store a memory address of a file
	if (pointerSales == NULL){
		printf("4.2 Error opening the Sales Table File");
		return;
	}
	MergeSortFileOption4(pointerSales, 0, numberOfSales - 1, sizeof(salesRecords), FunctionCompareInSalesByCustomerkey4);
	CalculateAverageDeliveryTime(pointerSales, numberOfSales, 2);
    fclose(pointerSales);
}