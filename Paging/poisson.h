#ifndef POISSON_H
#define POISSON_H


#include <stdio.h>
#include <stdlib.h>
#include <math.h>
#include <time.h>
#include <Windows.h>



#define N		100
#define LAMBDA	2.5

extern int bMadeArray;
extern int *numMsgs;
extern int msgsPerMin;

int NumberOfMessages(float lambda);
int bIsItTimeForAMessage(float lambda, long timeInSec);
int Factorial(int i);

#endif // POISSON_H
