
#include "poisson.h"

int *numMsgs;
int msgsPerMin;
int bMadeArray = 0;

/*int main(int argc, char *argv[]){
	int i, result;
	float lambda = LAMBDA;
	int zeroTime = time(NULL);
	char buffer[LENGTH];

	if(argc == 2)
		lambda = atoi(argv[1]);
	else if(argc != 1){
		printf("Usage: poisson [lambda] \n Choose a lambda between 0 - 10.");
		exit(0);
	}

	srand(time(NULL));
	while(1){
		if(bIsItTimeForAMessage(lambda, (time(NULL) - zeroTime)) % 60){
			GetMessageFromFile(buffer, LENGTH);
			printf("\n\n%s\n\n", buffer);
		}
		Sleep(950);
		printf("tick ");
	}

	printf("\nNumber of Messages this minute: %d\n\n", msgsPerMin);

	for(i = 0; i < N; i++)
		printf("%d ", numMsgs[i]);
}

// gets the number of messages per minute based on lambda
// and the poisson distribution*/
int NumberOfMessages(float lambda){
	int i, j;
	float p;
	int numProb;
	int index = 0;
    int result;



	// check if the array was made already, if it hasn't, make it so
	if(!bMadeArray){
		numMsgs = (int *)malloc(sizeof(int)*N);

		// get the probability of each number between 0 and N/10
		for(i = 0; i < N/10; i++){
			p = pow(lambda, i);
			p *= exp(-1 * lambda);
			p /= (float)Factorial(i);

			// multiply by 100 and truncate to get the number of times
			// out of 100 it will appear
			numProb = p * N;

			// populate the array with i, the number of times it will appear
			// based on its probability
			for(j = 0; j < numProb; j++){
				numMsgs[index + j] = i;
			}

			// keep track of where we are in the array
			index += j;
		}

		// fill remaining spaces (due to truncation) with -1
		for(j = 0; index + j < N;j++){
			numMsgs[index+j] = -1;
		}
		bMadeArray = 1;
	}

	// get a random index of the array, if the result is -1 try again 
	// (3/100 times it goes twice, 1/1000 it goes 3 times)
	do{
		result = rand() % N;
	}while(numMsgs[result] == -1);

	// return random result
	return numMsgs[result];
}

int bIsItTimeForAMessage(float lambda, long timeInSec){
	if(timeInSec % 60 == 0)
		msgsPerMin = NumberOfMessages(lambda);
	else if(msgsPerMin == 0)
		return 0;
	else if(timeInSec % ((60 / msgsPerMin) - 1) == 0)
		return 1;

	return 0;
}

// returns the factorial of the parameter
int Factorial(int i){
	int j, num = 1;
	for(j = 1; j <= i; j++){
		num = num * j;
	}
	return num;
}
