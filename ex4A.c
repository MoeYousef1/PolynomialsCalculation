#include <stdio.h>
#include "string.h"
#include <stdlib.h>
#include <pthread.h>

#define MAXIMUM 512

typedef struct polynomAns { // struct
    char *str; // input
    int Ans; // answer
}
        polynomAns;

static int val = 0; // starting val as zero

void basicMethod();

int doPow(int a, int b);

char *getVal(char str[], char getVal[]);

char *getExponent(char str[], char getPower[]);

int numOfXs(const char str[]);

char *getCoefficient(const char str[], char coefficient[]);

int numOfPlus(const char str[]);

void *print_data(void *arg);

int main() {

    basicMethod();
    return 0;
}

void basicMethod() {
    char str[MAXIMUM]; // array to save user input
    char copyStr[MAXIMUM]; // array to copy user input to it
    char *ptr; // pointer to pass it to strol
    char *split; // using it with strtok
    int i, k, status, Ans, plus; // variables used in the program

    while (1) {
        Ans = 0;
        printf("Enter a Polynom and a Val in this order (POLYNOM, Val) or done to exit\n");
        fgets(str, MAXIMUM, stdin); // get user input

        if ((strlen(str) > 0) && (str[strlen(str) - 1] == '\n')) { // if the last char is \n we change it to \0
            str[strlen(str) - 1] = '\0';
        }

        if (strlen(str) == 0) { // if the user entered nothing
            continue;
        }

        if (strcmp(str, "done") == 0) { // if the user entered done the program exits
            exit(0);
        }
        if (str[strlen(str) - 1] == '0' && str[strlen(str) - 2] == ' ') {// a special case , if the last char is 0 and the char before it is a space
            printf("Answer is: %d\n", 0);
            continue;
        }

        for (i = 0; i < strlen(str); i++) { // copying the user input until the comma
            if (str[i] == ',' || str[i] == ' ') {
                copyStr[i] = '\0';
                break;
            }
            copyStr[i] = str[i];
        }

        k = numOfXs(str); // number of letter x in the string
        plus = numOfPlus(str); // number of pluses in the string
        val = (int) strtol(getVal(str, ""), &ptr, 10); // value using strtol
        if (val == 0 || str[strlen(str) - 1] < '0' || str[strlen(str) - 1] > '9') {// if the value is zero(not a num) or the value is written without a space before it
            printf("Value is not calculable, enter a valid number or add a space before Val\n");
            continue;
        }
        polynomAns *pol = (polynomAns *) malloc(sizeof(polynomAns) * (k)); // allocating memory for the struct
        pthread_t *threadArr = (pthread_t *) malloc(sizeof(pthread_t) * (k));// allocating memory for the thread

        if (plus != 0) {// if we have pluses in the polynom
            for (i = 0; i < strlen(str); i++) {// change the comma to \0 in order to stop splitting there
                if (str[i] == ',')
                    str[i] = '\0';
            }
            split = strtok(str, "+");// using strtok to split the polynom
            i = 0;
            while (i < k && split != NULL) {

                pol[i].str = (char *) malloc(sizeof(char) * (strlen(str) / k)); //allocating memory for each split
                strcpy(pol[i].str, split);// copying the split string into the struct
                i++;
                split = strtok(NULL, "+");// in order to check if we have more splits
            }
        } else { // if we don't have pluses
            split = strtok(str, ","); // we do the same as above but with (,)
            i = 0;
            while (i < k && split != NULL) {
                pol[i].str = (char *) malloc(sizeof(char) * (strlen(str) / k));
                strcpy(pol[i].str, split);
                i++;
                split = strtok(NULL, ",");
            }
        }

        i = 0;
        while (i < k) {
            status = pthread_create(&threadArr[i], NULL, print_data, (void *) &pol[i]);// create threads
            if (status != 0) {
                printf("pthread creation failed");
                exit(1);
            }
            i++;
        }

        i = 0;
        while (i < k) {
            pthread_join(threadArr[i], (void **) &pol[i]); // join threads
            Ans += pol[i].Ans;// add each result to the Ans
            free(pol[i].str); // free allocation
            i++;
        }

        if(k==plus){ // for the last number which is not multiplied by x
            for(i=(int)strlen(copyStr)-1;i>=0;i--){
                if (copyStr[i] > '9'||copyStr[i]<'0') {
                    printf("not a valid number\n");
                    break;
                }
                if(copyStr[i-1]=='+'||i==0){// when we reach the plus mark, strtol returns the number after the plus mark
                    Ans +=(int) strtol(&(copyStr[i]), &ptr, 10);
                    printf("Answer is: %d\n", Ans);// print the answer
                    break;
                }}}

        if(k!=plus) {
            printf("Answer is: %d\n", Ans);// print the answer
        }
        free(pol);
        free(threadArr);
    }
}

char *getVal(char str[], char getVal[]) { //here we get the value that's after the comma
    getVal = strchr(str, ',');
    return getVal + 2;
}

char *getExponent(char str[], char getPower[]) { // get the power
    char *power = strchr(str, '^');

    while (NULL != power) {
        int j = 0;
        for (int i = 1; i < strlen(power); i++) { // if the power is a number, then copy it to getPower
            if (power[i] >= '0' && power[i] <= '9') {
                getPower[j] = power[i];
                j++;
            }
        }
        getPower[j] = '\0';
        return getPower; // return the power
    }
    return "1";
}

int numOfXs(const char str[]) { // count the number of x in the polynom
    int ThreadCount = 0;
    int i = 0;
    while (str[i] != '\0') {
        if (str[i] == 'x') {
            ThreadCount++;
        }
        i++;
    }
    return ThreadCount;
}

int numOfPlus(const char str[]) { // count the number of plus in the polynom
    int ThreadCount = 0;
    int i = 0;
    while (str[i] != '\0') {
        if (str[i] == '+') {
            ThreadCount++;
        }
        i++;
    }
    return ThreadCount;
}

char *getCoefficient(const char str[], char coefficient[]) { // get the factor of x
    int i = 0, j = 0;

    if (str[0] == 'x' || strlen(str) == 0)// if there is no coefficient it mans we have 1
        return "1";

    while (str[i] != '\0') { // if we have an x and * then we change x and * to \0, and we return the coefficient
        if (str[i + 1] == 'x' && str[i] == '*') {
            coefficient[j] = '\0';
            coefficient[j + 1] = '\0';
            return coefficient;
        }

        if (str[i] >= '0' && str[i] <= '9') {// if the coefficient is a number we copy it to coefficient array
            coefficient[j] = str[i];
            j++;
            i++;
        }
    }
    coefficient[i] = '\0';
    return coefficient;
}

int doPow(int a, int b) { // do val^ coefficient
    int x = 1;
    while (b != 0) {
        x *= a;
        b--;
    }
    return x;
}

void *print_data(void *arg) {
    char getStr[MAXIMUM];
    char *ptr;
    polynomAns *Pol = (polynomAns *) arg;
    int coefficient = (int) strtol(getCoefficient(Pol->str, getStr), &ptr, 10);// coefficient using strtol
    int exponent = (int) strtol(getExponent(Pol->str, getStr), &ptr, 10);// exponent using strtol
    Pol->Ans = (int) (coefficient * doPow(val, exponent)); // calculating the answer
    return NULL;
}