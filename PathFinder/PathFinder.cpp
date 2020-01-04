// PathFinder.cpp : Defines the entry point for the console application.
//

#include "stdafx.h"
#include <stdlib.h>


#include <iostream>
using namespace std;

#define MAX_ROW_COUNT	200
#define COLUMN_COUNT	16
#define DIGIT_COUNT		3
#define MAX_PERM_COUNT		1000

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

int* parseValue(char *val)
{
	if( val == NULL || strlen(val) < 1 )
		return NULL;

	int count = strlen(val);
	int* x = (int *)calloc(count, sizeof(int));

	int i = 0; 

	for(i = 0; i < count; i++)
		x[i] = val[i] - 48;

	return x;
}

int calcBestPath(int **x, int b, int t, int *p, int p_count, int *max_perm_num_missed)
{
	int i = 0, j = 0, k = 0;
	int val = 0;
	int *pp = NULL;
	int hist[4];
	int missed_value = -1;

	int max_len = 0;
	int len = 0;

	int max_perm_count = 0;
	
	for(k = 0; k < p_count; k++)
	{
		pp = p + k * DIGIT_COUNT;		
		missed_value = -1;
		for(i = b - 1, j = 0; i >= t; i--, j++ )
		{				
			if( j % 3 == 0 )
				memset(hist, 0, 4 * sizeof(int));

			val = x[i][pp[j % 3] - 1];
			hist[val]++;

			// check missed value
			if( j == 2 )	// 3
			{
				if(hist[1] == 0 && hist[2] >= 1 && hist[3] >= 1 )
					missed_value = 1;
				else if(hist[1] >= 1 && hist[2] == 0 && hist[3] >= 1 )
					missed_value = 2;
				else if(hist[1] >= 1 && hist[2] >= 1 && hist[3] == 0 )
					missed_value = 2;	 

				if( missed_value < 0 ) // Breakdown
					break;		
			}
			else if( j < 2 )
			{
				
			}
			else	// 
			{
				if( hist[missed_value] > 0 ) // Breakdown														
					break;

				if( j % 3 == 2 )
				{
					int breakdown_flag = 0;
					for(int q = 1; q < 3; q++)
					{
						if( q == missed_value )
							continue;

						if( hist[q] == 0 )
						{
							breakdown_flag = 1;
							break;
						}
					}

					if( breakdown_flag == 1 )
						break;
				}
			}

			len = j + 1;
		}

		if( len > max_len )
		{
			memset(max_perm_num_missed, 0, 2 * MAX_PERM_COUNT * sizeof(int));
			max_perm_count = 0;

			max_perm_num_missed[0] = k;
			max_perm_num_missed[1] = missed_value;
			max_perm_count++;

			max_len = len;
		}
		else if( len >= max_len && len > 0 )
		{
			if( max_perm_count < MAX_PERM_COUNT )
			{
				max_perm_num_missed[max_perm_count * 2] = k;
				max_perm_num_missed[max_perm_count * 2 + 1] = missed_value;
				max_perm_count++;
			}
		}
	}

	return max_len;
}

int _tmain(int argc, _TCHAR* argv[])
{
	int i = 0, j = 0;
	int PERM_TOTAL_COUNT  = 1;

	for(i = 0; i < DIGIT_COUNT; i++)
		PERM_TOTAL_COUNT *= COLUMN_COUNT;

	int *perm_list = (int*)calloc(PERM_TOTAL_COUNT * DIGIT_COUNT, sizeof(int));
	if( perm_list == NULL )
		return -1;

	// generate perm list
	int count = 0;
	generatePermList(perm_list, 0, count);

	//for(i = 0; i < PERM_TOTAL_COUNT; i += DIGIT_COUNT )
	//{
	//	for(j = 0; j < DIGIT_COUNT; j++)
	//		cout << perm_list[i + j];
	//	cout << endl;
	//}

	int row_count = 12;
	char x[][COLUMN_COUNT + 1] = {
		"1213232312321122",
		"3232111213213121",
		"2111323231231213",
		"2323112323121213",
		"1213233321231231",
		"2111323132231222",
		"1123133212311123",
		"3233123132313211",
		"3112312312323121",
		"1232312312321113",
		"2132311232312123",
		"3121213232112312"
	}; 
	 
	int *xx[MAX_ROW_COUNT];
	memset(xx, 0, MAX_ROW_COUNT * sizeof(int*));

	for(i = 0; i < row_count; i++)
	{
		xx[i] = parseValue(x[i]);
	}

	int *max_perm_num_missed = (int *) calloc(MAX_PERM_COUNT * 2, sizeof(int));
	calcBestPath(xx, row_count, 0, perm_list, PERM_TOTAL_COUNT, max_perm_num_missed);

	for(i = 0; i < MAX_PERM_COUNT; i++)
	{
		int p_num = max_perm_num_missed[i * 2];
		if(max_perm_num_missed[i * 2 + 1] < 1 )
			break;

		int *p = perm_list + p_num * DIGIT_COUNT;
		cout << "(" << p[0] << "," << p[1] << "," << p[2] << ") " << max_perm_num_missed[i * 2 + 1] << endl;		
	}

	free(max_perm_num_missed);
	for(i = 0; i < row_count; i++)
		free(xx[i]);

	free(perm_list);

	return 0;
}

