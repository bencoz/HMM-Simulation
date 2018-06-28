#define _CRT_SECURE_NO_WARNINGS
#include <stdio.h>
#include <stdlib.h>
#include <time.h>
#define SIZE 300
void willChangeCube();
int tossCube();
int isFairCube; //global boolean

int main()
{
	time_t t;
	FILE *fp1 = NULL;
	FILE *fp2 = NULL;
    int i;
    char tossResult[SIZE + 1] = {0};
    char cubesResult[SIZE + 1] = {0};
	isFairCube = 0; //starting with the UNFAIR cube
	fp1 = fopen("TOSS_OUT.txt" ,"w");
	fp2 = fopen("CUBES_OUT.txt" ,"w");
    srand((unsigned)time(&t));
    for (i = 0; i < SIZE; ++i){
        willChangeCube();
        tossResult[i] = (char)('0' + tossCube());
        if (isFairCube){
            cubesResult[i] = 'F';
        } else {
            cubesResult[i] = 'U';
        }
    }
	tossResult[SIZE] = '\0';
	cubesResult[SIZE] = '\0';
	fprintf(fp1, "%s", tossResult);
	fprintf(fp2, "%s", cubesResult);
	fclose(fp1);
	fclose(fp2);
    return 0;
}

int tossCube(){
    int output;
    if (isFairCube){
        output = rand() % 6;
        return ++output;
    } else {
        output = rand() % 10;
        if (output >= 5)
            return 6;
        else
            return ++output;
    }
}

void willChangeCube(){
    int output;
    if (isFairCube){
        output = rand() % 100;
        if (output < 95){
            isFairCube = 1;
        } else {
            isFairCube = 0;
        }
    } else {
        output = rand() % 10;
        if (output < 9){
            isFairCube = 0;
        } else {
            isFairCube = 1;
        }
    }
}