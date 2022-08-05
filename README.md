==Description==
ex4A

In this program the user will enter a POLYNOM and a VALUE, the VALUE will be inplace of X, and the POLYNOM will be calculated and answer will be returned,
then the user will be asked to enter another POLYNOM,  and it will go on in an infinite loop until the user enters the word (done).
The way that the POLYNOM is calculated is by splitting it into different parts, and what I did here is that I split the POLYNOM everytime I find a plus mark(+),
or if I find the Comma mark if I have no plus mark.

Then the program will create a number of threads depending on the number of x's in the POLYNOM, and every thread will calculate a different part of the POLYNOM,
after the calculation is done, the answer of each thread will be added to the final result, and it will be printed on the screen.

ex4B

It does the same thing as ex4A, but instead of using threads we used shared memory and fork.

=Program DATABASE=

ex4A

static int val to save the value
char str[MAXIMUM] array to save user input
char copyStr[MAXIMUM]; array to copy user input to it
char *ptr; pointer to pass it to strol
char *split; using it with strtok
int i, k, status, Ans, plus; variables used in the program

ex4B

same as ex4A but we added     pid_t pid; to use fork

Functions:

ex4A
Using a struct that saves the input and the result.

Using 9 main functions

1)basicMethod();
this method has an infinite loop, asks the user to enter a polynom and a value, then it calls the other functions,
in order to get the value and the exponent and the coefficient each on seperated and returns the answer of the polynom.

2)doPow(int a, int b);
does value^ exponent

3)getVal(char str[], char getVal[]);
seperates the value which is written after the comma by the user, and returns it as a char, then it will be changed into
integer using strtol.

4)getExponent(char str[], char getPower[]);
same as getVal, but this time we get the exponent.

5)getCoefficient(const char str[], char coefficient[]);
same as getVal, but this time we get the coefficient.

6)numOfXs(const char str[]);
returns the number of x's in the string.

7)numOfPlus(const char str[]);
returns the number of pluses in the string.

8)print_data(void *arg);
changes the coefficient from char to int , and the same thing for the exponent,
then calls doPow on value , exponent and multiplies it by the coefficient, then saves the
answer to the struct.

9)main
calls the basicMethod.

ex4B

same methods as in ex4A , but we have some difference in the code
1) in the basic method in first ex we used threads, but in this ex we used shared memory,
and we used forks.

2)in the method print_data(void *arg) in first ex we gave it a void, and it returns NULL,
but in this ex we gave it a variable polynomAns, and it doesn't return.

==Program Files==

ex4A,B

contains the main and the functions

==How to compile==

ex4A

gcc ex4A.c -o ex4A -pthread
./ex4A

ex4B

gcc ex4B.c -o ex4B
./ex4B

==Input==

ex4A,B

POLYNOM, VAL

==Output==

ex4A,B

the answer is : the result of the polynom after value compensation.
