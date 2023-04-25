//This is my attempt (Dylan Dinh) at the bonus programming assignment
//The previous implementation did not work as expected so I have updated it
//I believe the issue lied within the least recently used logic


#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <math.h>

#define SIZE 32
#define CHAR_LEN 20


//Direct mapped %32
//2 way, %16
//4 way %8
//FUlly (We don't need index cause all of them in same set)

int directMap(char cache[SIZE][CHAR_LEN]);
long long int toDecimal(char input[CHAR_LEN]);
int toNumber (char x);
int twoWay(char cache [SIZE/2][2][CHAR_LEN]);
int fourWay(char cache[SIZE/4][4][20]);
int fullyAssociative(char cache [SIZE][CHAR_LEN]);


int main(){
    
    char cache1[SIZE][CHAR_LEN];

    int direct_hits = directMap(cache1);

    printf("The num of accesses should be 9999 (The amount of data within the traces.txt file)\n\n");

    printf("Hits using direct-mapped: %d\n Hit rate is %f\n--------------\n", direct_hits, (float)direct_hits/9999);

    char cache2[SIZE/2][2][CHAR_LEN];

    int twoHits = twoWay(cache2);

    printf("Hits using 2-way: %d\n Hit rate is %f\n--------------\n", twoHits, (float)twoHits/9999);

    char cache3[SIZE/4][4][CHAR_LEN];

    int fourHits = fourWay(cache3);

    printf("Hits using 4-way: %d\n Hit rate is %f\n--------------\n", fourHits, (float)fourHits/9999);

    char cache4[SIZE][CHAR_LEN];

    int fully = fullyAssociative(cache4);

    printf("Hits using fully associative: %d\n Hit rate is %f\n--------------\n", fully, (float)fully/9999);

    return 0;
}


int directMap(char cache[SIZE][CHAR_LEN]){

    char temp[CHAR_LEN];

    FILE * fp = fopen("traces.txt", "r");

    int hit_counter = 0;

    while (fscanf(fp, "%s", temp) != -1){

        long long int conv = toDecimal(temp);
        //Get the index
        conv = conv %SIZE;

        //If we have a match at that address then good, else we replace
        if (strcmp(cache[conv], temp) == 0){
            hit_counter = hit_counter + 1;
            
            continue;
        }
        else{
            strcpy(cache[conv], temp);
        }
        
    }

    fclose(fp);
    return hit_counter;
}


int twoWay(char cache [SIZE/2][2][CHAR_LEN]){

    char temp[CHAR_LEN];

    FILE * fp = fopen("traces.txt", "r");

    int hit_counter = 0;

    //Create helper array to implement least recently used logic (probably bad runtime)
    int arr[SIZE/2][2];
    //Make sure to initialize at 1
    for (int i = 0; i < SIZE/2; i++){
        for (int j = 0; j < 2; j++){
            arr[i][j] = 1;
        }
    }
   
    while (fscanf(fp, "%s", temp) != -1){

        long long int conv = toDecimal(temp);
        //Get the index
        conv = conv %(SIZE/2);

        //If we have a match at that address then good, else we replace

        //For hit
        int flag = 0;
        for (int i = 0; i < 2; i++){
            
            if (strcmp(cache[conv][i], temp) == 0){
                hit_counter++;
                flag = 1;
                
                //If we have a hit then we have technically "recently used"
                for (int j = 0; j < 2; j++){
                    if (j != i) {
                        arr[conv][j]++;
                    }
                    else {
                        arr[conv][j] = 0;
                    }
                }

                break;
    
            }
        }
        
        if (flag != 1){

            int largest = -1;
            int index = 0;

            //Find largest since last replaced
            for (int i = 0; i < 2; i++){
                if (arr[conv][i] > largest){
                    largest = arr[conv][i];
                    index = i;
                }
            }

            strcpy(cache[conv][index], temp);
            //Reset the counter for that slot
            arr[conv][index] = 0;

        }
        //We update the least recently used as needed
        for (int i = 0; i < 2; i++){
            arr[conv][i]++;
        }
    }

    //Close file and return the value
    fclose(fp);
    return hit_counter;
    
}

int fourWay(char cache[SIZE/4][4][CHAR_LEN]){

    char temp[CHAR_LEN];

    FILE * fp = fopen("traces.txt", "r");

    int hit_counter = 0;

    //Create helper array to implement least recently used logic (probably bad runtime)
    int arr[SIZE/4][4];

    //Make sure to initialize at 1
    for (int i = 0; i < SIZE/4; i++){
        for (int j = 0; j < 4; j++){
            arr[i][j] = 1;
        }
    }
   
    while (fscanf(fp, "%s", temp) != -1){

        long long int conv = toDecimal(temp);
        //Get the index
        conv = conv %(SIZE/4);
  

        //If we have a match at that address then good, else we replace
        //For hit
        int flag = 0;
        for (int i = 0; i < 4; i++){
            if (strcmp(cache[conv][i], temp) == 0){

                hit_counter++;
                flag = 1;
                
                //If we have a hit then we have technically "recently used"
                for (int j = 0; j < 4; j++){
                    if (j != i) {
                        arr[conv][j]++;
                    }
                    else {
                        arr[conv][j] = 0;
                    }
                }

                break;
    
            }
        }
        
        if (flag != 1){

            int largest = -1;
            int index = 0;

            //Find largest since last replaced
            for (int i = 0; i < 4; i++){
                if (arr[conv][i] > largest){
                    largest = arr[conv][i];
                    index = i;
                }
            }

            strcpy(cache[conv][index], temp);
            //Reset the counter for that slot
            arr[conv][index] = 0;

        }
        for (int i = 0; i < 4; i++){
            arr[conv][i]++;
        }
    }

    //Close file and return the value
    fclose(fp);
    return hit_counter;
    
    
}

//Should have the most matches in this scenario
int fullyAssociative(char cache [SIZE][CHAR_LEN]){
    
    char temp[CHAR_LEN];

    FILE * fp = fopen("traces.txt", "r");

    int hit_counter = 0;
    //Index does not matter in fully associative

    //Create helper array to implement least recently used logic (probably bad runtime)
    int arr[SIZE];

    //Make sure to initialize at 1
    for (int i = 0; i < SIZE; i++){
        arr[i] = 1;
    }

    while (fscanf(fp, "%s", temp) != -1){
        
        //For hit
        int flag = 0;
        for (int i = 0; i < SIZE; i++){
            if (strcmp(cache[i], temp) == 0){
                hit_counter++;
                flag = 1;

                for (int j = 0; j < SIZE; j++) {
                    if (arr[j] > arr[i] && arr[j] != -1){
                        arr[j]--;
                    }
                }
                arr[i] = SIZE - 1;
                break;
    
            }
        }
        
        if (flag != 1){

            int min = arr[0];
            int index = 0;

            //Find largest since last replaced
            for (int i = 0; i < SIZE; i++){
                if (arr[i] < min){
                    min = arr[i];
                    index = i;
                }
            }

            strcpy(cache[index], temp);


        

            for (int i = 0; i < SIZE; i++){
                //printf(" - %s -\n", cache[i]);
                if (arr[i] != -1){
                    arr[i]--;
                }
            }
            
            //Update array
            arr[index] = SIZE - 1;
        }
    }

    //Close file and return the value
    fclose(fp);
    return hit_counter;

}



long long int toDecimal(char input[]){

    int i = strlen(input) - 1;    

    int j = 0;

    long long int total = 0;

    while (i > 1){   
        
        //we need to first convert the string to number

        long long int temp = toNumber(input[i]);

        //Convert hexadecimal to decimal
        total += (long long int)(temp * pow(16, j));

        j++;
        i--;
    }
    return total;
}


int toNumber (char x){
    //Converting string to number
    if (x >= '0' && x <= '9'){
        return (x - '0');
    }
    else if (x >= 'a' && x <= 'f'){
        return ((x - 'a') + 10);
    }
}

