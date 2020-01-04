// PathFinder.cpp : Defines the entry point for the console application.
//

#include "stdafx.h"
#include <stdlib.h>
#include <Windows.h>

#include <iostream>

using namespace std;

#define RANDOM_MODE	1

#if RANDOM_MODE
#define MAX_ROW_COUNT	64
#define COLUMN_COUNT	8
#define DIGIT_COUNT		7
#else
#define MAX_ROW_COUNT	64
#define COLUMN_COUNT	16
#define DIGIT_COUNT		6
#endif

#define MAX_PERM_COUNT		1000


// ===== Input Data ================================================
void generateInputValue(int **xx, int row_count, int column_count)
{
	srand(GetTickCount());
	int i = 0;
	for(i = 0; i < row_count; i++)
	{
		xx[i] = (int *)calloc(column_count, sizeof(int));
		for(int j = 0; j < column_count; j++)
		{
			xx[i][j] = rand() % 3 + 1;
		}
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
// ===== Input Data ================================================

// ===== Main Engine ================================================

// ===== Generate Possible Permutaion ================================================
void generatePermList(int *perm_list, int level, int &count, int perm_len, int max_number)
{
	if( level == perm_len )
	{
		count++;
		return;
	}

	int i = 0;
	for(i = 1; i <= max_number; i++)
	{
		if( i > 1 )
			memcpy(perm_list + perm_len * count,  perm_list + perm_len * (count - 1), sizeof(int) * level);

		perm_list[perm_len * count + level] = i;
		
		generatePermList(perm_list, level + 1, count, perm_len, max_number);
	}
}
// ===== Generate Possible Permutaion ================================================

// ===== Caculate Best Path(Permutation List, missing values) ================================================
// [Params]:
//			x: 2-dimension array
				//1213232312321122
				//3232111213213121
				//2111323231231213
				//2323112323121213
				//1213233321231231
				//2111323132231222
				//1123133212311123
				//3233123132313211
				//3112312312323121
				//1232312312321113
				//2132311232312123
				//3121213232112312
//			b: scan range (bottom) 
//			t: scan range (top) 
//			p: permutation list
//			p_count: permutation count
int calcBestPath(int **x, int b, int t, int *p, int p_count, int *max_perm_num_missed, int digit_count, int max_row_count)
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
		pp = p + k * digit_count;		
		missed_value = -1;

		//if( pp[0] == 13 && pp[1] == 13 && pp[2] == 13 )
		//	pp[0] = 13;
		for(i = b - 1, j = 0; i >= t - 1; i--, j++ )
		{	
			if( j % digit_count == 0 )
				memset(hist, 0, 4 * sizeof(int));

			val = x[i][pp[j % digit_count] - 1];
			hist[val]++;

			// check missed value
			if( j == digit_count - 1 )	// 3
			{
				if(hist[1] == 0 && hist[2] >= 1 && hist[3] >= 1 )
					missed_value = 1;
				else if(hist[1] >= 1 && hist[2] == 0 && hist[3] >= 1 )
					missed_value = 2;
				else if(hist[1] >= 1 && hist[2] >= 1 && hist[3] == 0 )
					missed_value = 3;	 

				if( missed_value < 0 ) // Breakdown
					break;		
			}
			else if( j < digit_count - 1 )
			{
				
			}
			else	// 
			{
				if( hist[missed_value] > 0 ) // Breakdown														
					break;

				if( j % 3 == digit_count - 1 )
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
			memset(max_perm_num_missed, 0, 2 * max_row_count * sizeof(int));
			max_perm_count = 0;

			max_perm_num_missed[0] = k;
			max_perm_num_missed[1] = missed_value;
			max_perm_count++;

			max_len = len;
		}
		else if( len >= max_len && len > 0 )
		{
			if( max_perm_count < max_row_count )
			{
				max_perm_num_missed[max_perm_count * 2] = k;
				max_perm_num_missed[max_perm_count * 2 + 1] = missed_value;
				max_perm_count++;
			}
		}
	}

	return max_len;
}
// ===== Caculate Best Path(Permutation List, missing values) ================================================

int _tmain(int argc, _TCHAR* argv[])
{
	int i = 0, j = 0;
	int PERM_TOTAL_COUNT  = 1;

	for(i = 0; i < DIGIT_COUNT; i++)
		PERM_TOTAL_COUNT *= COLUMN_COUNT;

	long long size_len = PERM_TOTAL_COUNT * DIGIT_COUNT;

	int *perm_list = (int*)calloc(size_len, sizeof(int));
	if( perm_list == NULL )
		return -1;

	// generate perm list
	int count = 0;
	generatePermList(perm_list, 0, count, DIGIT_COUNT, COLUMN_COUNT);

	//for(i = 0; i < PERM_TOTAL_COUNT; i += DIGIT_COUNT )
	//{
	//	for(j = 0; j < DIGIT_COUNT; j++)
	//		cout << perm_list[i + j];
	//	cout << endl;
	//}

	int *xx[MAX_ROW_COUNT];
	memset(xx, 0, MAX_ROW_COUNT * sizeof(int*));

	
#if RANDOM_MODE
	generateInputValue(xx, MAX_ROW_COUNT, COLUMN_COUNT);
#else
	//char x[][COLUMN_COUNT + 1] = {
	//	"1213232312321122",
	//	"3232111213213121",
	//	"2111323231231213",
	//	"2323112323121213",
	//	"1213233321231231",
	//	"2111323132231222",
	//	"1123133212311123",
	//	"3233123132313211",
	//	"3112312312323121",
	//	"1232312312321113",
	//	"2132311232312123",
	//	"3121213232112312"
	//}; 

	char x[][COLUMN_COUNT + 1] = {
		"1223132313231222",
		"2313111323123231",
		"3213231231231232",
		"1121332131132332",
		"2131223113313221",
		"1313212321121321",
		"2131213123113212",
		"2223123213123231",
		"3121322313122213",
		"2132121322131333",
		"1231221111231321",
		"3213132313231121",
		"2312132121323112",
		"2113133321223123",
		"1322231312213312",
		"3121322123331212",
		"3211231333212321",
		"2313231312231323",
		"1123133231322123",
		"2223133211113231",
		"1213323122231231",
		"2333121332212312",
		"1112313223113231",
		"2132313323123121",
		"2223123131233231",
		"2133231231321223",
		"1132313211321322",
		"3311312313213222",
		"2132312231132312",
		"3121321123332223",
		"2313213213321221",
		"2313231323121212",
		"1132312331221332",
		"3212323131231212",
		"2131212123312113",
		"1231212311332131",
		"1332213132123212",
		"2232213112311233",
		"2113312132132321",
		"3213312321332132",
		"3331112313231212",
		"1211123323133322",
		"1133213321323113",
		"2232213231323131",
		"2132312321121223",
		"1323132331212231",
		"2313231223111231",
		"1323121323123132",
		"3231223121123233",
		"3231223313111123",
		"2313231213211132",
		"1323121312213331",
		"2133231323121211",
		"1313231213232311",
		"2313233121112331",
		"1213323112231331",
		"3223132313323113",
		"2232221113213231",
		"1131323132331322",
		"1323132321323111",
		"2313231223132111",
		"3323132311212313",
		"1323132313212111",
		"3221132313131223"
	};

	for(i = 0; i < MAX_ROW_COUNT; i++)
		xx[i] = parseValue(x[i]);	
#endif

	
	long int  start = GetTickCount();
	int *max_perm_num_missed = (int *) calloc(MAX_PERM_COUNT * 2, sizeof(int));
	int max_len = calcBestPath(xx, MAX_ROW_COUNT, 1, perm_list, PERM_TOTAL_COUNT, max_perm_num_missed, DIGIT_COUNT, MAX_PERM_COUNT);
	long int  end = GetTickCount();

	// display input values
	cout << "Input Values: " << endl;
	for(i = MAX_ROW_COUNT - max_len; i < MAX_ROW_COUNT; i++)
	{
		for(j = 0; j < COLUMN_COUNT; j++)
			cout << xx[i][j];
		cout << endl;
	}
	cout << endl;

	// display permutation - missed value

	int max_perm_count = 0;
	cout << "Permutation" << "\t" << "Missing" << endl;	
	for(i = 0; i < MAX_PERM_COUNT; i++)
	{
		int p_num = max_perm_num_missed[i * 2];
		if(max_perm_num_missed[i * 2 + 1] < 1 )
			break;

		int *p = perm_list + p_num * DIGIT_COUNT;

		cout << "(";
		for(j = 0; j < DIGIT_COUNT; j++)
		{
			if( j > 0 )
				cout << ",";		

			cout << p[j];			
		}
		cout << ") height " << (max_len / DIGIT_COUNT);
		int val = max_len % DIGIT_COUNT;
		if( val > 0  )
			cout << " " << val << "/" << DIGIT_COUNT;
		cout << " length " << max_len << " missing " << max_perm_num_missed[i * 2 + 1] << endl;		

		max_perm_count++;
	}

	// display solution.
	cout << endl;	
	cout << "Max Len = " << max_len << endl;	
	cout << "Total Count = " << max_perm_count << endl;
	cout << "Execute Time  = " << (end - start) << "ms" << endl;
	cout << endl;	


	char new_value[DIGIT_COUNT + 1];
	cin >> new_value;

	free(max_perm_num_missed);
	for(i = 0; i < MAX_ROW_COUNT; i++)
		free(xx[i]);

	free(perm_list);

	return 0;
}

