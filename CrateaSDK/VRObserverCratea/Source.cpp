#pragma once
#include "stdafx.h"
#include "Source.h"
#define BUFLEN 256
void CheckCommand(char buffer[])
{
	printf("Welcome to Oculus interface\n");
	char bufferd[BUFLEN];   /* the string is stored through pointer to this buffer */
	printf("Please Enter new Commands:");
	fflush(stdout);
	fgets(bufferd, BUFLEN, stdin); /* buffer is sent as a pointer to fgets */
	printf("the command was: %s", bufferd);
}