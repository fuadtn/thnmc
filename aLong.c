//0 файл[1] знак[2] файл[3] результат[4] бинарный[5] модуль[6]

#include "stdio.h"
#include "stdlib.h"

int aAdd(int *a, int *b, int len_max, int flag_a, int flag_b);
void aCode(int *num, int len_max);
int rWrite(int *num, int len_max, char *binf, int flag_a, char *sign, char *fresult);
void aMul(int *result, int *a, int *b, int len_a, int len_b);
int cmpAB(int *a, int *b, int len_max, int x);
int aDiv(int *result, int *a, int *b, int len_b, int len_max);
int aRem(int *a, int *b, int len_b, int len_max);

int aPow(int* a, int* b, int len_a, int len_b, int flag_a, char *binf, char *fresult);
void copy_ab(int *result, int *a, int len_a);
int isnull_ab(int *b, int len_b);

int main(int argc, char* argv[])
{
	system("clear");

	FILE *fp_a = NULL, *fp_b = NULL;
	int len_max, i;
	int *a, flag_a = 0, len_a = 0;
	int *b, flag_b = 0, len_b = 0;

	fp_a = fopen(argv[1], "r");
	fp_b = fopen(argv[3], "r");
	if(fp_a == NULL || fp_b == NULL)
	{
		printf("ERROR OF READING");
		return 0;
	}
	
// Проверили знак____________
	if(fgetc(fp_a) == '-') flag_a = 1;	
	if(fgetc(fp_b) == '-') flag_b = 1;

	fseek(fp_a, 0, SEEK_END);
	len_a = ftell(fp_a) - 1 - flag_a;
	fseek(fp_a, 0, SEEK_SET);

	fseek(fp_b, 0, SEEK_END);
	len_b = ftell(fp_b) - 1 - flag_b;
	fseek(fp_b, 0, SEEK_SET);
	
	if(len_a >= len_b) len_max = len_a;
	else len_max = len_b;

	a = (int *) malloc((len_max+2)*sizeof(int));
	b = (int *) malloc((len_max+2)*sizeof(int));
	
	for(i = 0; i <= len_max; i++)
	{
		a[i] = 0;
		b[i] = 0;
	}
	
	int b5 = 48;
	if(strcmp(argv[5], "-b") == 0)
	b5 = 0; 
	
	fseek(fp_a, flag_a, SEEK_SET);
	fseek(fp_b, flag_b, SEEK_SET);
	for(i = len_a - 1; i >= 0; i--)
	a[i] = fgetc(fp_a) - b5;
	for(i = len_b - 1; i >= 0; i--)
	b[i] = fgetc(fp_b) - b5;	

	if(strcmp(argv[2],"-") == 0) flag_b++;
	
	printf("\nFirst number: ");
	for(i = 0; i <= len_max; i++)
	printf("%d", a[i]);
	printf("\nSecond number: ");
	for(i = 0; i <= len_max; i++)
	printf("%d", b[i]);
	printf("\n");

	if(strcmp(argv[2],"+") == 0 || strcmp(argv[2],"-") == 0)				// Сложение
	{	
		flag_a = aAdd(a, b, len_max, flag_a, flag_b);
		rWrite(a, len_max, argv[5], flag_a, "+", argv[4]);
	}
	if(strcmp(argv[2], "x") == 0)												// Умножение
	{
		if((flag_a + flag_b) == 1)
		flag_a = 1;
		else flag_a = 0;

		int *result = (int *) malloc((len_a + len_b + 1)*sizeof(int));
		for(i = 0; i <= len_a + len_b; i++)
		result[i] = 0;

		aMul(result, a, b, len_a, len_b);
		rWrite(result, len_a + len_b, argv[5], flag_a, "x", argv[4]);
		free(result);
	}
	if(strcmp(argv[2],"/") == 0)													// Деление
	{
		if((flag_a + flag_b) == 1)
		flag_a = 1;
		else flag_a = 0;
		
		int *result = (int *) malloc((len_max+1)*sizeof(int));
		for(i = 0; i < len_max+1; i++)
		result[i] = 0;
		int dlen = aDiv(result, a, b, len_b, len_max);
		rWrite(result, dlen, argv[5], flag_a, "/", argv[4]);
		free(result);
	}
	if(strcmp(argv[2],"%") == 0)												// Остаток от деления
	{	
		aRem(a, b, len_b, len_max);
		rWrite(a, len_a, argv[5], 0, "%", argv[4]);
	}

	int mn =  aPow(a, b, len_a, len_b, flag_a, argv[5], argv[4]);

	printf("First array: ");
	for(i = 0; i <= len_max; i++)
	printf("%d", a[i]);
	
	free(a);
	free(b);
	return 0;
}










// Функции_________________________________________
int aAdd(int *a, int *b, int len_max, int flag_a, int flag_b)
{
	int i, tmp, r = 0;
	
	if(flag_a == 1)
	aCode(a, len_max+1);	
	if(flag_b == 1)
	aCode(b, len_max+1);
	
	for(i = 0; i <= len_max; i++)
	{
		tmp = a[i];
		a[i] = (a[i] + b[i] + r) % 10;
		r = (tmp + b[i] + r) / 10;
	}	
	if(a[len_max+1] == 9)
	{
		flag_a = 1;
		aCode(a, len_max+1);
	}
	else flag_a = 0;
	return flag_a;
}

void aCode(int *num, int len_max)
{
	int i, tmp;
	int r = (10 - num[0]) / 10;
	num[0] = (10 - num[0]) % 10;

	for(i = 1; i <= len_max; i++)
	{
		tmp = num[i];
		num[i] = (9 - num[i] + r) % 10; 
		r = (9 - tmp + r) / 10;
 	}
}

int rWrite(int *num, int len_max, char *binf, int flag_a, char *sign, char *fresult)
{
	FILE *fp;
	int i, iresult;
	int bf = 0;
	char c;
	
	if(strcmp(binf, "-b") == 0)
	{	
		if(fresult == NULL)
		fresult = "result.bin";
		
		fp = fopen(fresult, "wb");
		bf = 48;
	}
	else
	{
		if(fresult == NULL)
		fresult = "result.bin";
		fp = fopen("result", "w");	
	}
	
	if(strcmp(sign, "/") == 0)
	{
		iresult = 0;
		for(i = 0; i  < len_max; i++)
		if(num[i] == 0)
		iresult++;
		else break;

		if(flag_a == 1)
		fprintf(fp, "-");
		for(i = iresult; i < len_max; i++)
		{
			c = num[i] + 48 - bf;
			fprintf(fp, "%c", c);
		}
		return 0;
	}
	
	iresult = len_max;
	for(i = len_max; i > 0; i--)
	if(num[i] == 0)
	iresult--;
	else break;
	
	if(flag_a == 1)
	fprintf(fp, "-");
	for(i = iresult; i >= 0; i--)
	{
		c = num[i] + 48 - bf;
		fprintf(fp, "%c", c);
	}
	return 0;
}

void aMul(int *result, int *a, int *b, int len_a, int len_b)
{		
	int i, j;

	for(i = 0; i < len_a; i++)
	{
		for(j = 0; j < len_b; j++)
		{
			result[i+j] += a[i] * b[j];
			result[i+j+1] += result[i+j] / 10;
			result[i+j] = result[i+j] % 10;
		}	
	}
}

int cmpAB(int *a, int *b, int len_max, int x)
{
	int i;
	
	for(i = len_max - 1; i >= 0; i--)
	{
		if(a[i+x] > b[i])
		{
		printf("cmpAB = 1\n");
		return 1;
		}
		if(a[i+x] < b[i])
		{
		printf("cmpAB = 2\n");
		return 2;
		}	
	}
	printf("cmpAB = 0\n");	
	return 0;	
}

int aDiv(int *result, int *a, int *b, int len_b, int len_max)
{
	int i = 0;
	while(len_b <= len_max)
	{
		while(cmpAB(a, b, len_b, len_max - len_b) != 2)
		{
			Sub(a, b, len_b, len_max);
			result[i]++;
		}
		i++;
		len_b++;
	}
	return i;
}

int Sub(int *a, int *b, int len_b, int len_max)
{
	int i, cf = 0;
	int x = len_max - len_b;
	
	for(i = 0; i < len_b; i++)
	{
		a[i+x] = a[i+x] - b[i] - cf;
		if(a[i+x] < 0)
		{
			a[i+x] += 10;
			cf = 1;
		}
		else cf = 0;
	}
}

int aRem(int *a, int *b, int len_b, int len_max)
{	
	while(cmpAB(a, b, len_b, len_max - len_b) != 2)
	Sub(a, b, len_b, len_max);
	len_b++;
	if(len_b > len_max)
	return 0;
	aRem(a, b, len_b, len_max);
}

int aPow(int* a, int* b, int len_a, int len_b, int flag_a, char *binf, char *fresult)
{
	int *result;
	int sum = len_a, count;
	
	if(flag_a == 1 && (b[0] % 2) == 1)
	flag_a = 1;
	else 
	flag_a = 0;	

	if(len_a == 1 && a[len_a] == 0)
	{
		a[0] = 1;
		rWrite(a, len_a, binf, flag_a, "%", fresult);
	}

	result = (int *) malloc((len_a+1)* sizeof(int));
	copy_ab(result, a, len_a);
//	do
//	{
		count = isnull_ab(b, len_b);
		sum += len_a;
		result = realloc(result, sum*sizeof(int));
//		aMul(result, result, a, sum, len_a);
//	}while(count == 1);

	rWrite(result, sum, binf, flag_a, "%", fresult);
	free(result);
	return 0;
}

void copy_ab(int *result, int *a, int len_a)
{
	int i;
	for(i = 0; i < len_a + 1; i++)
	result[i] = a[i];
}

int isnull_ab(int *b, int len_b)
{
	int i, cf = 0;

	for(i = 0; i < len_b; i++)
	{
		b[i] = b[i] - 1 - cf;
		if(b[i] < 0)
		{
			b[i] += 10;
			cf = 1;
		}
		else cf = 0;
	}

	for(i = 0; i < len_b +1; i++)
	{
		if(b[i] != 0)
		return 1;	
	}
	return 0 ;		
}

