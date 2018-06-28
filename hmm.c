#define _CRT_SECURE_NO_WARNINGS
#include <stdio.h>
#include <stdlib.h>
#define SIZE 300
#define STATES_SIZE 2

typedef struct twodouble
{
	double f;
	double u;
} _2Double;

void normallize(_2Double*);
char* coinHMM(int*);
_2Double* foward_algo(int*, int*, double*, double(*mat)[2], double(*mat2)[6]);
_2Double* backward_algo(int*, int*, double(*mat)[2], double(*mat2)[6]);

int main(int argc, char* argv[])
{
	FILE *tossFile, *hmmOut;
	tossFile = hmmOut = NULL;
	int cubes[SIZE];
	char* hmmResult;
	int c, i = 0;
	//open files
	if (argc < 2) {
		printf("Please try again...(missing file TOSS_OUT)");
		return 1;
	}
	tossFile = fopen(argv[1], "r");
	hmmOut = fopen("HMM_OUT.txt", "w");

	if (tossFile == NULL || hmmOut == NULL) {
		printf("Something went wrong :(\n(file could not be opened)");
		return 1;
	}
	//transform char into int
	while ((c = fgetc(tossFile)) != EOF) {
		cubes[i++] = c - '0';
	}
	//actual calculations 
	hmmResult = coinHMM(cubes);
	//printing and finish
	fprintf(hmmOut, "%s", hmmResult);

	free(hmmResult);
	fclose(tossFile);
	fclose(hmmOut);
	return 0;
}

char* coinHMM(int* tosses) {
	int states[2] = { 0, 1 }; // 0 := Fair, 1 := Unfair
	double startProb[2] = { 0.5, 0.5 }; // TODO:: Ask nilly if it need to change
	double transtionMatix[2][2] = { { 0.95, 0.05 }, { 0.1, 0.9 } };
	double emissionMatrix[2][6] = { { 1.0 / 6, 1.0 / 6, 1.0 / 6, 1.0 / 6, 1.0 / 6, 1.0 / 6 }, { 0.1, 0.1, 0.1, 0.1, 0.1, 0.5 } };
	int i;
	char *res = (char *)malloc(SIZE + 1);

	_2Double *fwd = foward_algo(tosses, states, startProb, transtionMatix, emissionMatrix);
	_2Double *bkw = backward_algo(tosses, states, transtionMatix, emissionMatrix);
	
	//answer if fair or not by probability 
	for (i = 0; i < SIZE; ++i) {
		double fair, unfair;
		fair = fwd[i].f * bkw[i].f;
		unfair = fwd[i].u * bkw[i].u;
		if (fair > unfair)
			res[i] = 'F';
		else
			res[i] = 'U';
	}
	res[SIZE] = '\0';
	free(fwd);
	free(bkw);
	return res;
}

_2Double* foward_algo(int *tosses, int *states, double *startProb, double(*transtionMatix)[2], double(*emissionMatrix)[6]) {
	int i, j, k;
	_2Double f_prev = { startProb[0], startProb[1] };
	_2Double f_curr;
	_2Double *res = (_2Double*)malloc(SIZE * sizeof(_2Double));
	
	for (i = 0; i < SIZE; ++i) {
		f_curr.f = f_curr.u = 0;
		for (j = 0; j < STATES_SIZE; ++j) {
			if (i == 0) {
				if (states[j] == 0) //fair
					f_curr.f = emissionMatrix[states[j]][tosses[i] - 1] * f_prev.f;
				else
					f_curr.u = emissionMatrix[states[j]][tosses[i] - 1] * f_prev.u;
			}
			else {
				for (k = 0; k < STATES_SIZE; ++k){
					if (states[j] == 0) { //fair
						if (states[k] == 0) //fair
							f_curr.f += emissionMatrix[states[j]][tosses[i] - 1] * transtionMatix[states[k]][states[j]] * f_prev.f;
						else
							f_curr.f += emissionMatrix[states[j]][tosses[i] - 1] * transtionMatix[states[k]][states[j]] * f_prev.u;
					}
					else {//unfair
						if (states[k] == 1)// unfair
							f_curr.u += emissionMatrix[states[j]][tosses[i] - 1] * transtionMatix[states[k]][states[j]] * f_prev.u;
						else
							f_curr.u += emissionMatrix[states[j]][tosses[i] - 1] * transtionMatix[states[k]][states[j]] * f_prev.f;
					}
				}
			}
		}
		normallize(&f_curr);
		res[i] = f_curr;
		f_prev = f_curr;
	}
	return res;
}

_2Double* backward_algo(int *tosses, int *states, double(*transtionMatix)[2], double(*emissionMatrix)[6]) {
	int i, j, k;
	_2Double b_prev = { 1.0, 1.0 }; // end probability
	_2Double b_curr;
	_2Double *res = (_2Double *)malloc(SIZE * sizeof(_2Double));

	for (i = SIZE - 1; i >= 0; --i) { //loop from end to start
		b_curr.f = b_curr.u = 0;
		for (j = 0; j < STATES_SIZE; ++j) {
			if (i == SIZE - 1){
				b_curr = b_prev;
				break;
			}
			for (k = 0; k < STATES_SIZE; ++k) {
				if (states[j] == 0){ //fair
					if (states[k] == 0) //fair
						b_curr.f += transtionMatix[states[j]][states[k]] * emissionMatrix[states[k]][tosses[i+1] - 1] * b_prev.f;
					else
						b_curr.f += transtionMatix[states[j]][states[k]] * emissionMatrix[states[k]][tosses[i+1] - 1] * b_prev.u;
				}
				else { //unfair
					if (states[k] == 1) //unfair
						b_curr.u += transtionMatix[states[j]][states[k]] * emissionMatrix[states[k]][tosses[i+1] - 1] * b_prev.u;
					else
						b_curr.u += transtionMatix[states[j]][states[k]] * emissionMatrix[states[k]][tosses[i+1] - 1] * b_prev.f;
				}
			}
		}
		if (i != SIZE - 1)
			normallize(&b_curr);
		res[i] = b_curr;
		b_prev = b_curr;
	}
	return res;
}

void normallize(_2Double *input){
	double sum = input->f + input->u;
	double scalar = (1.0 / sum);
	input->f *= scalar;
	input->u *= scalar;
}