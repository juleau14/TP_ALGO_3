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


 bool isSymbol(char * c) {
 	switch(*c) {
		case '+':
			return 1;
		case '-':
			return 1;
		case '*':
			return 1;
		case '^':
			return 1;
		case '(':
			return 1;
		case ')':
			return 1;
		default:
			return 0;
	}
 }


 Queue * stringToTokenQueue(char * expression) {
 	Queue * finalQueue = createQueue();
 	char * curspos = expression;

 	while (*curspos != '\0') {

 		if (*curspos != ' ' && *curspos != '\n') {

 			Token * newToken;

 			if (!isSymbol(curspos)) {

 				int lg = 0;
 				char * cursposbis = curspos;

 				while (!isSymbol(cursposbis) && (*cursposbis != ' ') && (*cursposbis != '\n')) {
 					lg++;
 					cursposbis++;
 				}

 				newToken = createTokenFromString(curspos, lg);
 				finalQueue = queuePush(finalQueue, newToken);
				curspos += lg;
 			}

 			else {
 				newToken = createTokenFromString(curspos, 1);
 				finalQueue = queuePush(finalQueue, newToken);
				curspos++;
 			}

 		}

		else {

 		curspos++;

		}

   }
 	return finalQueue;
 }

Queue * shuttingYard(Queue * infix) {
	
	Queue * output = createQueue();
	Stack * op_stack = createStack(0);
	Token * token;

	while (!queueEmpty(infix)) {

		token = queueTop(infix);

		if (tokenIsNumber(token)) {
			output = queuePush(output, token);
			infix = queuePop(infix);
		}

		else if (tokenIsOperator(token)) {
			while ( (( tokenGetOperatorPriority(stackTop(op_stack)) > tokenGetOperatorPriority(token) ) || ( tokenGetOperatorPriority(stackTop(op_stack)) == tokenGetOperatorPriority(token) && tokenOperatorIsLeftAssociative(token) )) && ( tokenGetOperatorSymbol(stackTop(op_stack)) != '(' ) ) {
				output = queuePush(output, stackTop(op_stack));
				op_stack = stackPop(op_stack);
			}

			op_stack = stackPush(op_stack, token);

			if (tokenGetOperatorSymbol(token) == '(') {
				output = queuePush(output, token);
			}

			if (tokenGetOperatorSymbol(token) == ')') {
				while (tokenGetOperatorSymbol(stackTop(op_stack)) != '(') {
					output = queuePush(output, stackTop(op_stack));
					op_stack = stackPop(op_stack);
				}
				op_stack =  stackPop(op_stack);
			}
		}

		infix = queuePop(infix);

	}

	while (!stackEmpty(op_stack)) {
		op_stack = stackPop(op_stack);
	}

	return output;

}

void computeExpressions(FILE * input) {

	char * line = NULL;
	size_t len = 0;
	ssize_t read;
	Queue * myQueue;
	Queue * notationPolonaise;

	FILE * f = fdopen(1, "w");

	while ((read = getline(&line, &len, input)) != -1) {
		if (read > 1) {
			fprintf(f, "Input : %sInfix : ", line);

			myQueue = stringToTokenQueue(line);
			queueDump(f, myQueue, printToken);
			notationPolonaise = shuttingYard(myQueue);
			queueDump(f, notationPolonaise, printToken);
			fprintf(f, "\n\n");
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
