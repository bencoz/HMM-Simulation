#define _CRT_SECURE_NO_WARNINGS
#include <stdio.h>
#define SIZE 300

int main(int argc, char* argv[])
{
	FILE *cubesFile, *hmmFile, *fp3, *fp4;
	cubesFile = hmmFile = fp3 = fp4 = NULL;
	int i, c, tp, tn, fn, fp;
	double sensFair, specFair, sensUnfair, specUnfair;
	tp = tn = fn = fp = 0;
    char result[(SIZE * 3) + 1 + 2] = {0}; // 1 for '\0' and 2 for '\n'
	char hmm[SIZE + 1] = {0};
    char cubes[SIZE + 1] = {0};
	//Open files
	if (argc < 3){
		printf("Please try again...(enter 2 files CUBES_OUT and HMM_OUT)");
		return 1;
	}
	cubesFile = fopen(argv[1], "r");
	hmmFile = fopen(argv[2], "r");
	fp3 = fopen("COMPARE.txt", "w");
	fp4 = fopen("SUMMARY.txt", "w");
    if (cubesFile == NULL || hmmFile == NULL || fp3 == NULL || fp4 == NULL) {
		printf("Something went wrong :(\n(files could not be opened)");
		return 1;
	}
	i = 0; //Reading input files into arrays
	while((c = fgetc(cubesFile)) != EOF) {
        result[i] = (char)c; //Setting 1st line of result too
		cubes[i++] = (char)c;
    }
	cubes[SIZE] = '\0';
	result[SIZE] = '\n';
	fseek(cubesFile, 0, SEEK_SET);
	i = 0;
	while((c = fgetc(hmmFile)) != EOF) {
		hmm[i++] = (char)c;
	}
	hmm[SIZE] = '\0';
	fseek(hmmFile, 0, SEEK_SET);
	
	//Process input
	for (i = 0; i < SIZE; ++i){
		if (cubes[i] == 'F'){
			if (hmm[i] == 'F'){
				tp++;
				result[SIZE + i + 1] = '+';
			} else {
				fn++;
				result[SIZE + i + 1] = ' ';
			}
		} 
		else {
			if (hmm[i] == 'U'){
				fp++;
				result[SIZE + i + 1] = '+';
			} else {
				tn++;
				result[SIZE + i + 1] = ' ';
			}
		}
	}
	result[(SIZE * 2) + 1] = '\n';
	sensFair = (double)tp/(tp+fn);
	specFair = (double)tn/(tn+fp);
	sensUnfair = (double)fp/(fp+tn);
	specUnfair = (double)fn/(tp+fn);
	//printf("Sens = %f\nSpec = %f",(double)tp/(tp+fn),(double)tp/(tp+fp));
	//Setting 3rd line of result
	i = 0;
	while((c = fgetc(hmmFile)) != EOF) {
		result[(SIZE * 2) + 2 + i++] = (char)c;
    }
	result[SIZE * 3 + 2] = '\0';
	fprintf(fp3, "%s", result);
	fprintf(fp4, "SUMMARY:\nSensitivity (Fair):\t%f\nSpecificity (Fair):\t%f\n--------------------------------\nSensitivity (Unfair):\t%f\nSpecificity (Unfiar):\t%f\n",
						 sensFair, specFair, sensUnfair, specUnfair);
	fclose(cubesFile);
	fclose(hmmFile);
	fclose(fp3);
	fclose(fp4);
    return 0;
}