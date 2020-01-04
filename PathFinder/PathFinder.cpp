// PathFinder.cpp : Defines the entry point for the console application.
//

#include "stdafx.h"
#include <stdlib.h>


#include <iostream>
using namespace std;

#define COLUMN_COUNT	4
#define DIGIT_COUNT		3

void generatePermList(int *perm_list, int level, int &count)
{
	if( level == DIGIT_COUNT )
	{
		count++;
		return;
	}

	int i = 0;
	for(i = 1; i <= COLUMN_COUNT; i++)
	{
		if( i > 1 )
			memcpy(perm_list + DIGIT_COUNT * count,  perm_list + DIGIT_COUNT * (count - 1), sizeof(int) * level);

		perm_list[DIGIT_COUNT * count + level] = i;
		
		generatePermList(perm_list, level + 1, count);
	}
}

int _tmain(int argc, _TCHAR* argv[])
{
	int i = 0, j = 0;
	int PERM_TOTAL_COUNT  = DIGIT_COUNT;

	for(i = 0; i < DIGIT_COUNT; i++)
		PERM_TOTAL_COUNT *= COLUMN_COUNT;

	int *perm_list = (int*)calloc(PERM_TOTAL_COUNT, sizeof(int));
	if( perm_list == NULL )
		return -1;

	// generate perm list
	int count = 0;
	generatePermList(perm_list, 0, count);

	for(i = 0; i < PERM_TOTAL_COUNT; i += DIGIT_COUNT )
	{
		for(j = 0; j < DIGIT_COUNT; j++)
			cout << perm_list[i + j];
		cout << endl;
	}
	
	free(perm_list);

	return 0;
}

