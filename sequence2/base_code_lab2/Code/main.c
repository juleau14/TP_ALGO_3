// Need this to use the getline C function on Linux. Works without this on MacOs. Not tested on Windows.
#define _GNU_SOURCE
#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include <assert.h>
#include <math.h>

#include "token.h"
#include "queue.h"
#include "stack.h"


/** Main function for testing.
 * The main function expects one parameter that is the file where expressions to translate are
 * to be read.
 *
 * This file must contain a valid expression on each line
 *
 */


 bool isSymbol(char c) {
 	char symbols[] = {'+', '-', '*', '^', '(', ')'};
 	int i = 0;
 	while (symbols[i] != '\0') {
 		if (c == symbols[i]) {
 			return 1;
 		}
 		i++;
 	}
 	return 0;
 }


 Queue * stringToTokenQueue(char * expression) {
 	Queue * finalQueue = createQueue();
 	char * curspos = expression;

 	while (*curspos != '\0') {

 		if (*curspos != ' ' && *curspos != '\n') {

 			Token * newToken;

 			if (!isSymbol(*curspos)) {

 				int lg;
 				char * cursposbis = curspos;

 				while (!isSymbol(*cursposbis)) {
 					lg++;
 					cursposbis++;
 				}

 				char number[lg];
 				for (int i = 0; i < lg; i++) {
 					number[i] = *curspos;
 					curspos++;
 				}

 				newToken = createTokenFromString(number, lg);
 				finalQueue = queuePush(finalQueue, newToken);

 			}

 			else {
 				newToken = createTokenFromString(curspos, 1);
 				finalQueue = queuePush(finalQueue, newToken);
 			}

 		}

 		curspos++;

   }
 	return finalQueue;
 }

void computeExpressions(FILE * input) {

	char * line = NULL;
	size_t len = 0;
	ssize_t read;

	FILE * f = fdopen(STDOUT_FILENO, "r");

	while ((read = getline(&line, &len, input)) != -1) {
		if (read > 1) {
			printf("Input : %s", line);

			Queue * myQueue = stringToTokenQueue(line);
			queueDump(f, myQueue, printToken);
		}
	}

	free(line);
}







int main(int argc, char **argv){
	FILE *input;

	if (argc<2) {
		fprintf(stderr,"usage : %s filename\n", argv[0]);
		return 1;
	}

	input = fopen(argv[1], "r");

	if ( !input ) {
		perror(argv[1]);
		return 1;
	}

	computeExpressions(input);

	fclose(input);
	return 0;
}
