/*
===================================================================================
***********************************************************************************
		Filename:		Source.c
		Team Members:	NOVONIL DAS & PARTH PADALKAR
		UTD_ID:			2021477505 & 2021473758
		NetID:			NXD180022 & PXP180054
		Class:			Operating Systems
		Project:		Project # 2 (Multiplication of Large Numbers)
***********************************************************************************
===================================================================================
*/

#include <stdio.h>
#include <unistd.h>
#include <stdbool.h>
#include <stdlib.h>
#include <pthread.h>

#define MAX_OUTPUT 200000

//This struct is passed to each thread
struct ThreadComponents
{
	int length1;
	int length2;
	int start_ind;
	int end_ind;
	int* FirstArr;
	int* SecondArr;
	int result[MAX_OUTPUT];
	int sum;
};

void* Product(void* t);

void main(int argc, char* argv[])
{
    clock_t tc;
    tc = clock();
	char* FirstNumber = argv[1];
	char* SecondNumber = argv[2];
	char count = *argv[3];
	int ThreadCount = (int)(count - '0');
	int length1 = 0;
	int length2 = 0;
	long i=0,j,m = 0;
	char c;
	int Sum[MAX_OUTPUT];
    int carry = 0;
    short zflag = 1;
    //Open the file that has the first number
	FILE* readerA;
	readerA = fopen(FirstNumber, "r");

	while ((c = fgetc(readerA)) != ' ')
	{
		length1 = (length1 * 10) + (c - '0');
	}
	
	int A[length1];

	while ((c = fgetc(readerA)) != EOF)
	{
		A[i++] = (int)(c - '0');
		
	}
	fclose(readerA);

    //Open the file that has the second number
	i = 0;
	FILE* readerB;
	readerB = fopen(SecondNumber, "r");
	
	while ((c = fgetc(readerB)) != ' ')
	{
		length2 = (length2 * 10) + (c - '0');
	}
	
	int B[length2];

    if(length2 < ThreadCount)
    {
        ThreadCount = 1;
    }
    while ((c = fgetc(readerB)) != EOF)
	{
		B[i++] = (int)(c - '0');
	}
	
	fclose(readerB);

	struct ThreadComponents t[ThreadCount];
	pthread_t tids[ThreadCount];

	long start_index = 0;
	long end_index = 0;

	//Creating the threads
	for (j = 0; j < ThreadCount; j++)
	{
		if((ThreadCount == 1) || ((j == ThreadCount - 1) && (length2 % ThreadCount != 0)))
		{
			end_index = length2;
		}
		else
		{
			end_index = start_index + (length2 / ThreadCount);
		}
		t[j].length1 = length1;
		t[j].start_ind = start_index;
		t[j].end_ind = end_index;
		t[j].FirstArr = A;
		t[j].SecondArr = B;
		pthread_attr_t attr;
		pthread_attr_init(&attr);
		pthread_create(&tids[j], &attr, Product, &t[j]);
		start_index = end_index;
	}
	//waiting for the threads to complete
	for (j = 0; j < ThreadCount; j++) {
        pthread_join(tids[j], NULL);

        for (i = 0; i < length1 + length2; i++) {



            Sum[i + m] = Sum[i + m] + t[j].result[i];

        }

        m++;
    }



	for (i = length1 + length2 - 1; i > 0; i--)
	{
		if (Sum[i] / 10 > 0)
		{
			Sum[i - 1] += (Sum[i] / 10);
			Sum[i] %= 10;
		}
	}

	//writing the product in to a file
    FILE *out_file = fopen("c2.txt", "w");
    fprintf(out_file, "Product calculated by the %d threads: ", ThreadCount);
	for (i = 0; i < length1 + length2; i++)
	{
        if(Sum[i] == 0 && zflag == 1)
        {
            continue;
        }
	    zflag = 0;


        fprintf(out_file, "%d", Sum[i]);

	}


    tc = clock() - tc;
    double time_taken = ((double)tc)/CLOCKS_PER_SEC; // in seconds
    printf("program took %f seconds to execute \n", time_taken);
}



// function to compute the product
void* Product(void* tt)
{
	struct ThreadComponents* tcomp = (struct ThreadComponents*) tt;
	int carry = 0;
	int first, second, s = 0;
	int product = 0, sum = 0;
	int l3 = tcomp->length1 + tcomp->end_ind - tcomp->start_ind;
	int C[l3];
	int i=l3, j, k = 1;
    for(i = 0; i< l3; i++)
    {
        C[i] = 0;
    }
	for (second = tcomp->end_ind-1; second >= tcomp->start_ind; second--)
	{
		i = l3 - k;
		carry = 0;
		for (first = tcomp->length1 - 1; first >= 0; first--)
		{
			product = tcomp->FirstArr[first] * tcomp->SecondArr[second];
			sum = (C[i] + product + carry);
			C[i] = sum % 10;
			carry = sum / 10;
			i--;
		}
		C[i] += carry;
		k++;
	}
	for (j = 0; j < l3; j++)
	{
		tcomp->result[j] = C[j];
	}
	pthread_exit(0);
}

