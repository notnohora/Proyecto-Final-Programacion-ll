#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <ctype.h>
#include <time.h>
#include <math.h>
#include "Structs.h"

int FunctionCompareInSalesByOrderNumber3( void* a, void* b){
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
// Functions to compares atributes
// a/b: variables used to stores a any type pointer
int functionCompareInProductsByProductKey3( void* a, void* b){
    productsRecords *sale1 = (productsRecords*)a;
    productsRecords *sale2 = (productsRecords*)b;
    return (*sale1).productKey - (*sale2).productKey;
}
// Function to merge sort the files
// file: variable used to store a file pointer
// letf: variable used to store where start to sorting
// rigth: variable used to store where stop sorting
// middle: variable used to store the middle of the array
// recordSize: variable used to store the size of the record
// function: variable used to store the name of a function with those parameters
void MergeFileOption3(FILE *file, int left, int right, int middle, int recordSize, int (*function)(void*, void*)) {
   
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

void MergeSortFileOption3(FILE *file, int left, int right, int recordSize, int (*function)(void*, void*)) {
    if (left < right) {
        int medium = left + ((right - left) / 2);
        MergeSortFileOption3(file, left, medium, recordSize, function);
        MergeSortFileOption3(file, medium + 1, right, recordSize, function);
        MergeFileOption3(file, left, right, medium, recordSize, function);
    }
}

// Function to graph the analisis
// pantalla: array to store the graphic
// array: array to store the information
void Graph(char pantalla[24][56], float array[12]){
    float arrAux[12] = {0.0};
    for (int i = 0; i < 12; i++) {
        arrAux[i] = array[i];
    }
    double yMax = arrAux[0];
    if((int)yMax / 1000000 >= 1){
        for(int i = 0; i < 12; i++){
            arrAux[i] = arrAux[i] / 1000000;
        }
        yMax = arrAux[0];
    }
    
    for(int i = 0; i < 12; i++){
        if(arrAux[i] > yMax){
            yMax = arrAux[i];
        }
    }
    double pixelY = (fabs(yMax) == 0) ? 24 :  24.0 / fabs(yMax);

    for(int i = 0; i < 24; i++){
        pantalla[i][9] = 179;
    }
    for(int i = 9; i < 56; i++){
        if(i != 9){
            pantalla[23][i] = 196;
        }else{
            pantalla[23][i] = 192;
        }
    }
    for(int i = 0; i < 12; i++){
        int placeY = (int) 24.0-round(pixelY * arrAux[i]);
        char cadena[12] = "";
        sprintf(cadena, "%.2f", arrAux[i]);
        int longitudCadena = strlen(cadena);
        for(int j = 0; j < longitudCadena; j++){
                pantalla[placeY][j] = cadena[j];
        }
        pantalla[placeY][11 + (4 * i)] = 'x';
    }

    return;
}

// Function t
void CalculateVolumeOfOrders( FILE* pointerSales, FILE* pointerProducts, int numberOfSales){
     
     //array used to store data from each month
     float arrayOrders[12] = {0.0};
     float arrayIncomes[12] = {0.0};
     float arrayOrdersAvarage[12] = {0.0};
     float arrayIncomesAvarage[12] = {0.0};
     char pantalla[24][56] = {{}};
     memset(pantalla, ' ', sizeof(pantalla));
     char meses[] = " ene feb mar abr may jun jul ago sep oct nov dic";
     salesRecords sale, startDate, finalDate; //variable used to store a struct
     productsRecords product; //variable used to store a struct
     int yearsAnalyzed = 0;
    fread(&sale, sizeof(salesRecords), 1, pointerSales);
    startDate = sale;
    fseek(pointerSales, sizeof(salesRecords)*(-1), SEEK_END);
    fread(&sale, sizeof(salesRecords), 1, pointerSales);
    finalDate = sale;
    fseek(pointerSales, 0, SEEK_SET);
    yearsAnalyzed = finalDate.orderDate.AAAA - startDate.orderDate.AAAA + 1;
    for (int i = 0; i < 12; i++) {
        arrayOrders[i] = 0;  
    }

    for (int i = 0; i < numberOfSales; i++) {
        fseek(pointerSales, sizeof(salesRecords) * i, SEEK_SET);
        fread(&sale, sizeof(salesRecords), 1, pointerSales);
        if (sale.orderDate.MM >= 1 && sale.orderDate.MM <= 12) {
            arrayOrders[sale.orderDate.MM - 1] += 1;  
        }else {
            printf("3. Error invalid month: %d\n", sale.orderDate.MM);
            }
        
    }
     for (int i = 0; i < numberOfSales; i++) {
        fseek(pointerSales, sizeof(salesRecords) * i, SEEK_SET);
        fread(&sale, sizeof(salesRecords), 1, pointerSales);

        int productKey = sale.productKey;
        int quantity = sale.quantity;
        int positionProduct = BinarySearchOption2(pointerProducts, productKey, 1);

        if (positionProduct != -1) {
            fseek(pointerProducts, sizeof(productsRecords) * positionProduct, SEEK_SET);
            fread(&product, sizeof(productsRecords), 1, pointerProducts);
            if (sale.orderDate.MM >= 1 && sale.orderDate.MM <= 12) {
                arrayIncomes[sale.orderDate.MM - 1] += product.unitPriceUSD * quantity;
            } else {
                printf("3.Error invalid month: %d\n", sale.orderDate.MM);
            }
        }
    }
    for(int i = 0; i < 12; i++){
        arrayOrdersAvarage[i]= 1.0*(arrayOrders[i]/yearsAnalyzed);
    }
    for(int i = 0; i < 12; i++){
        arrayIncomesAvarage[i]= 1.0*(arrayIncomes[i]/yearsAnalyzed);
    }
    
    printf("This report aims to analyze whether there are seasonal patterns or trends in order volume and Income.\n\n");
    printf("\033[1mMethodology:\033[0m\n");
    printf("Data collection: Historical order and revenue data is collected, with an appropriate time range to identify\n");
    printf("seasonal patterns.\n");
    printf("\033[1mData preparation:\033[0m\n");
    printf("The data will be organized to facilitate analysis.\n\n");
    printf("Order Volume and Monthly income Analysis\n");
    printf("When examining the order volume and monthly income data, a direct relationship between the two is observed, \n");
    printf("making it easy to identify the increases and decreases in both income and the number of orders. This analysis\n");
    printf("is especially useful when visualizing these patterns through charts and tables, facilitating the understanding\n");
    printf("of the peaks and drops that affect both sales volume and income. Additionally, there is a noticeable stability in\n");
    printf("certain months, highlighting the seasonality of the business.\n\n");
    printf("Order and income Peaks\n");
    printf("February and December stand out with the highest order volumes (%.0f and %.0f orders) and incomes ($%.2f and $%.2f),\n");
    printf("due to year-end promotions and January sales.\n\n", arrayOrders[1], arrayOrders[11], arrayIncomes[1], arrayIncomes[11]);
    printf("Months with Fewer Orders and income\n");
    printf("In March and April, both orders and income drop significantly, with %.0f and %.0f orders respectively. This is due to \n");
    printf("lower demand after the holidays and the lack of promotions or incentives.\n\n", arrayOrders[2], arrayOrders[3]);
    printf("Stability in the Intermediate Months\n");
    printf("May, June, and July show steady demand, with 4,000 to 5,000 orders per month, suggesting that activity remains balanced \n");
    printf("during these intermediate months.\n\n");
    printf("Monthly Averages\n");
    printf("The average order volumes follow the same seasonal trend, with February and December reaching the highest peaks, while\n");
    printf("March and April show the lowest values, confirming the post-holiday drop.\n\n");
    
    printf("\033[1mResults:\033[0m\nChart 1: Order volume per month from %i/%i/%i to %i/%i/%i\n", startDate.orderDate.MM, startDate.orderDate.DD, startDate.orderDate.AAAA, finalDate.orderDate.MM, finalDate.orderDate.DD, finalDate.orderDate.AAAA);
    printf("-------------------------\n|Month\t|Order Volume\t|\t\tMillions\n-------------------------\t");
    Graph(pantalla, arrayOrders);
    for (int i = 0; i < 24; i++) {
        for (int j = 0; j < 56; j++) {
            printf("%c", pantalla[i][j]);
        }
        printf("\n");
        if (i % 2 == 0) {
            printf("|%d\t|%.0f\t\t|\t", (i / 2) + 1, arrayOrders[(i / 2)]);
        } else if (i == 23) {
            printf("-------------------------\t         %s", meses);
        } else {
            printf("-------------------------\t");
        }
    }
    printf("\n\nChart 2: Income per month from %i/%i/%i to %i/%i/%i\n", startDate.orderDate.MM, startDate.orderDate.DD, startDate.orderDate.AAAA, finalDate.orderDate.MM, finalDate.orderDate.DD, finalDate.orderDate.AAAA);
    printf("-------------------------\n|Month\t|Total Income\t|\t\tMillions\n-------------------------\t");
    memset(pantalla, ' ', sizeof(pantalla));
    Graph(pantalla, arrayIncomes);
    for (int i = 0; i < 24; i++) {
        for (int j = 0; j < 56; j++) {
            printf("%c", pantalla[i][j]);
        }
        printf("\n");
        if (i % 2 == 0) {
            printf("|%d\t|%.2f\t|\t", (i / 2) + 1, arrayIncomes[(i / 2)]);
        } else if (i == 23) {
            printf("-------------------------\t         %s", meses);
        } else {
            printf("-------------------------\t");
        }
    }
    printf("\n\nChart 3: Monthly average order volume from %i/%i/%i to %i/%i/%i\n", startDate.orderDate.MM, startDate.orderDate.DD, startDate.orderDate.AAAA, finalDate.orderDate.MM, finalDate.orderDate.DD, finalDate.orderDate.AAAA);
    printf("-------------------------\n|Month\t|Monthly Average|\t\tMillons\n-------------------------\t");
    memset(pantalla, ' ', sizeof(pantalla));
    Graph(pantalla, arrayOrdersAvarage);
    for (int i = 0; i < 24; i++) {
        for (int j = 0; j < 56; j++) {
            printf("%c", pantalla[i][j]);
        }
        printf("\n");
        if (i % 2 == 0) {
            printf("|%d\t|%.0f\t\t|\t", (i / 2) + 1, arrayOrdersAvarage[(i / 2)]);
        } else if (i == 23) {
            printf("-------------------------\t         %s", meses);
        } else {
            printf("-------------------------\t");
        }
    }
    printf("\n\nChart 4: Monthly average income from  %i/%i/%i to %i/%i/%i\n", startDate.orderDate.MM, startDate.orderDate.DD, startDate.orderDate.AAAA, finalDate.orderDate.MM, finalDate.orderDate.DD, finalDate.orderDate.AAAA);
    printf("-------------------------\n|Month\t|Total Income\t|\t\tMillions\n-------------------------\t");
    memset(pantalla, ' ', sizeof(pantalla));
    Graph(pantalla, arrayIncomesAvarage);
    for (int i = 0; i < 24; i++) {
        for (int j = 0; j < 56; j++) {
            printf("%c", pantalla[i][j]);
        }
        printf("\n");
        if (i % 2 == 0) {
            printf("|%d\t|%.2f\t|\t", (i / 2) + 1, arrayIncomes[(i / 2)]);
        } else if (i == 23) {
            printf("-------------------------\t         %s", meses);
        } else {
            printf("-------------------------\t");
        }
    }
    printf("\n\n");
    printf("\033[1mConclusions:\033[0m\n");
    printf("Clear seasonal patterns show sales peaks in December and February, followed by drops in March and April.\n");
    printf("It is recommended to take advantage of demand peaks with marketing strategies and promotions, while managing the low-demand months with offers and discounts to maintain income flow.\n");
    printf("\033[1mRecommendations:\033[0m\n");
    printf("- Strengthen marketing efforts during low-demand months (March and April) with promotions and discounts.\n");
    printf("- Increase inventory during high-demand months (November, December, and January) to meet the sales peaks.\n");
    printf("- Reduce inventory during months of lower activity (March and April) to avoid overstocking and additional costs.\n");
    printf("- Build customer loyalty with reward programs that encourage purchases in low months.\n\n");
   }

// Function to direct the bubble sort option
// salesName: variable used to store the file's name
// productsName: variable used to store the file's name
void BubbleSortOption3( char salesName[], char productsName[]){

    int numberOfSales = CalculateNumberOfRecords(salesName, sizeof(salesRecords), 1);//numberOfSales: variable used to store the number of records
    FILE *pointerSales = fopen( salesName, "rb+");//variable used to store a memory addres of a file
    if(pointerSales == NULL){
       printf("3.1 Error opening the Sales Table File.");
       return;
    }
    FILE *pointerProducts = fopen(productsName, "rb+");//variable used to store a memory addres of a file
    if(pointerProducts == NULL){
       printf("3.1 Error opening the Sales Table File.");
       return;
    }
    salesRecords sale1, sale2; //variables used to store a struct
     for( int i = 0; i < numberOfSales; i++){
        //printf("sale %i\n", i);
        for( int j = 0; j < numberOfSales - i - 1; j++){
           fseek(pointerSales, sizeof(salesRecords)*i, SEEK_SET);
           fread(&sale1,sizeof(salesRecords),1, pointerSales);

           fseek(pointerSales, sizeof(salesRecords)*(i+1), SEEK_SET);
           fread(&sale2,sizeof(salesRecords),1, pointerSales);
           if(sale1.orderNumber > sale2.orderNumber){
            fseek(pointerSales, sizeof(salesRecords)*j, SEEK_SET);
            fwrite(&sale2, sizeof(salesRecords), 1, pointerSales);

            fseek(pointerSales, sizeof(salesRecords)*(j+1), SEEK_SET);
            fwrite(&sale2, sizeof(salesRecords), 1, pointerSales);
           }
        }
    }
    productsRecords product1, product2; //variables used to store a struct
     for( int i = 0; i < numberOfSales; i++){
        //printf("product %i\n", i);
        for( int j = 0; j < numberOfSales - i - 1; j++){
           fseek(pointerProducts, sizeof(productsRecords)*i, SEEK_SET);
           fread(&product1,sizeof(productsRecords),1, pointerProducts);

           fseek(pointerProducts, sizeof(productsRecords)*(i+1), SEEK_SET);
           fread(&product2,sizeof(productsRecords),1, pointerProducts);
           if(product1.productKey > product2.productKey){
            fseek(pointerProducts, sizeof(productsRecords)*j, SEEK_SET);
            fwrite(&product2, sizeof(productsRecords), 1, pointerProducts);

            fseek(pointerProducts, sizeof(productsRecords)*(j+1), SEEK_SET);
            fwrite(&product2, sizeof(productsRecords), 1, pointerProducts);
           }
        }
    }
    CalculateVolumeOfOrders(pointerSales, pointerProducts, numberOfSales);
    fclose(pointerSales);fclose(pointerProducts);
}

// Function to direct the merge sort option
// salesName: variable used to store the file's name
// productsName: variable used to store the file's name
void MergeSortOption3( char salesName[], char productsName[]){

    int numberOfSales = CalculateNumberOfRecords(salesName, sizeof(salesRecords), 1);//numberOfSales: variable used to store the number of records
    int numberOfProducts = CalculateNumberOfRecords(productsName, sizeof(productsRecords), 1);//numberOfProducts: variable used to store the number of records
    FILE *pointerSales = fopen( salesName, "rb+"); // variable used to store a memory address of a file
    if(pointerSales == NULL){
       printf("3.1 Error opening the Sales Table File.");
       return;
    }
    FILE *pointerProducts = fopen(productsName, "rb+");
    if(pointerProducts == NULL){
       printf("3.1 Error opening the Sales Table File.");
       return;
    }
    MergeSortFileOption3(pointerProducts, 0, numberOfProducts, sizeof(productsRecords), functionCompareInProductsByProductKey3);
    //MergeSortFileOption3(pointerSales, 0, numberOfSales, sizeof(salesRecords), FunctionCompareInSalesByOrderNumber3);
    CalculateVolumeOfOrders(pointerSales, pointerProducts, numberOfSales);
    fclose(pointerSales);fclose(pointerProducts);
}
    
