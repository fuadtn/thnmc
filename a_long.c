// файл[1] знак[2] файл[3] результат[4] бинарный[5] модуль[6]

#include "stdio.h"
#include "stdlib.h"
#include "string.h"



class b_int
{
	public:
	long int length;
	short int *digit;
	int flag;
	
	b_int();
    ~b_int();
};

int a_add(b_int &a, b_int &b, long int len_max);
int a_mod(b_int &a, b_int &b, long int len_max);
int a_div(b_int &c, b_int &a, b_int &b, long int len_max);
void a_mul(b_int &c, b_int &a, b_int &b);
int a_pow(b_int &a, b_int &b, char *binf, char *fresult, b_int &m);

int s_null(b_int &b);
int s_write(b_int &c, char *binf, char *sign, char *fresult);
int s_cmp(b_int &a, b_int &b, long int x);
void s_code(short int *digit, long int len_max);
void s_sub(b_int &a, b_int &b, long int len_max);
void s_copy(b_int &c, b_int &a);

int main(int argc, char* argv[])
{
	system("clear");

	b_int a, b, m;
	long int len_max, i;
	FILE *fp_a = NULL, *fp_b = NULL, *fp_m;
	fp_a = fopen(argv[1], "r");
	fp_b = fopen(argv[3], "r");
	if(argv[6] != NULL)
	fp_m = fopen(argv[6], "r");
	
	if(fp_a == NULL || fp_b == NULL)
	{
		printf("ERROR OF READING");
		return 0;
	}

// Проверка знака_____________________________________________________________	
	if(fgetc(fp_a) == '-') a.flag = 1;	
	if(fgetc(fp_b) == '-') b.flag = 1;

	fseek(fp_a, 0, SEEK_END);
	a.length = ftell(fp_a) - 1 - a.flag;
	fseek(fp_a, 0, SEEK_SET);

	fseek(fp_b, 0, SEEK_END);
	b.length = ftell(fp_b) - 1 - b.flag;
	fseek(fp_b, 0, SEEK_SET);
	
	if(fp_m != NULL)
	{
		fseek(fp_m, 0, SEEK_END);
		m.length = ftell(fp_m) - 1;
		fseek(fp_m, 0, SEEK_SET);
	}	

// Определение максимальной длины________________________________________	
	if(a.length >= b.length) len_max = a.length;
	else len_max = b.length;
	
// Выделение памяти числам_________________________________________________	
	a.digit = (short *) calloc(len_max+2, sizeof(short));
	b.digit = (short *) calloc(len_max+2, sizeof(short));
	if(fp_m != NULL)
	m.digit = (short *) calloc(m.length+1, sizeof(short));	

	if(a.digit == NULL || b.digit == NULL)
	{
		printf("ERROR OF MEMORY ALLOCATION");
		return 0;
	}

// Запись чисел в массив_____________________________________________________	
	int b5 = 48;
	if(strcmp(argv[5], "-b") == 0)
	b5 = 0; 

	fseek(fp_a, a.flag, SEEK_SET);
	fseek(fp_b, b.flag, SEEK_SET);
	for(i = a.length - 1; i >= 0; i--)
	a.digit[i] = fgetc(fp_a) - b5;
	for(i = b.length - 1; i >= 0; i--)
	b.digit[i] = fgetc(fp_b) - b5;
	for(i = m.length - 1; i >= 0; i--)
	m.digit[i] = fgetc(fp_m) - b5;

// Выбор действия_____________________________________________________________
	if(strcmp(argv[2],"+") == 0 || strcmp(argv[2],"-") == 0)
	{
		if(strcmp(argv[2],"-") == 0) b.flag = (b.flag + 1) % 2;	
		a.flag = a_add(a, b, len_max);
		a.length = len_max;
		if(fp_m != NULL)
		a_mod(a, m, len_max);
		s_write(a, argv[5], "+", argv[4]);
	}
	if(strcmp(argv[2], "x") == 0)
	{
		b_int c;
		if((a.flag + b.flag) == 1) c.flag = 1;
		else c.flag = 0;

		c.digit = (short *) calloc(a.length + b.length + 1, sizeof(short));
		c.length = a.length + b.length;
		a_mul(c, a, b);
		a_mod(c, m, c.length);	
		s_write(c, argv[5], "x", argv[4]);
	}
	if(strcmp(argv[2],"/") == 0)
	{
		b_int c;
		if((a.flag + b.flag) == 1) c.flag = 1;
		else c.flag = 0;
		
		c.digit = (short *) calloc(len_max+1, sizeof(short));
		c.length = a_div(c, a, b, len_max);
		a_mod(c, m, c.length);
		s_write(c, argv[5], "/", argv[4]);
	}
	if(strcmp(argv[2],"%") == 0)
	{	
		a.flag = 0;
		a_mod(a, b, len_max);
		a_mod(a, m, len_max);
		s_write(a, argv[5], "%", argv[4]);
	}
	if(strcmp(argv[2], "^") == 0)
	{
		a_pow(a, b, argv[5], argv[4], m);
	}

// Вывод_______________________________________________________________________
	printf("SUCCESSFUL COMPLETION\n");
	return 0;
}


// Конструктор-Деструктор___________________________________________________
b_int::b_int()
{
	length = 0;
	flag = 0;
	digit  = NULL;
}

b_int::~b_int()
{
	free(digit);
}


// Основные функции_________________________________________________________
int a_add(b_int &a, b_int &b, long int len_max)
{
	long int i;
	int tmp, r = 0;
	
	if(a.flag == 1) s_code(a.digit, len_max+1);	
	if(b.flag == 1) s_code(b.digit, len_max+1);
	
	for(i = 0; i <= len_max; i++)
	{
		tmp = a.digit[i];
		a.digit[i] = (a.digit[i] + b.digit[i] + r) % 10;
		r = (tmp + b.digit[i] + r) / 10;
	}
	if(a.digit[len_max] == 9)
	{
		a.flag = 1;
		s_code(a.digit, len_max+1);
	}
	else a.flag = 0;
	return a.flag;
}

void a_mul(b_int &c, b_int &a, b_int &b)
{		
	long int i, j;
	for(i = 0; i < c.length; i++)
	c.digit[i] = 0;

	for(i = 0; i < a.length; i++)
	{
		for(j = 0; j < b.length; j++)
		{
			c.digit[i+j] += a.digit[i] * b.digit[j];
			c.digit[i+j+1] += c.digit[i+j] / 10;
			c.digit[i+j] = c.digit[i+j] % 10;
		}	
	}
}

int a_div(b_int &c, b_int &a, b_int &b, long int len_max)
{
	long int i = 0;
	while(b.length <= len_max)
	{
		while(s_cmp(a, b, len_max - b.length) != 2)
		{
			s_sub(a, b, len_max);
			c.digit[i]++;
		}
		i++;
		b.length++;
	}
	return i;
}

int a_mod(b_int &a, b_int &b, long int len_max)
{
	if(len_max < b.length)
	return 0;
	while(s_cmp(a, b, len_max - b.length) != 2)
	{
		s_sub(a, b, len_max);
	}
	b.length++;
	a_mod(a, b, len_max);
}

int a_pow(b_int &a, b_int &b, char *binf, char *fresult, b_int &m)
{
	b_int c, c_r;
	long int i;

	if(a.flag == 1 && (b.digit[0] % 2) == 1) c_r.flag = 1;
	else c_r.flag = 0;	
	
	if(a.length == 1 && a.digit[0] == 0)
	{
		a.digit[0] = 1;
		a.flag = 0;
		s_write(a, binf, "^", fresult);
	}
	
	c_r.length = 1;
	for(i = 0; i < b.length; i++)
	c_r.length *= 10;
	c_r.length 100000;//*= a.length;
	c.length = c_r.length;

	c.digit = (short *) calloc(c.length, sizeof(short));
	c_r.digit = (short *) calloc(c.length, sizeof(short));
	
	if(c.digit == NULL || c_r.digit == NULL)
	{
		printf("ERROR OF MEMORY ALLOCATION");
		return 0;
	}
	
	s_copy(c, a);
	while(s_null(b) != 0)
	{
		a_mul(c_r, c, a);
		s_copy(c, c_r);
	}
//	a_mod(c_r, m, c_r.length);
	s_write(c_r, binf, "^", fresult);
	return 0;
}


// Вспомогательные функции_________________________________________________
void s_code(short int *digit, long int len_max)
{
	long int i;
	int tmp;
	int r = (10 - digit[0]) / 10;
	digit[0] = (10 - digit[0]) % 10;

	for(i = 1; i <= len_max; i++)
	{
		tmp = digit[i];
		digit[i] = (9 - digit[i] + r) % 10; 
		r = (9 - tmp + r) / 10;
 	}
}

void s_sub(b_int &a, b_int &b, long int len_max)
{
	long int i;
	int cf = 0;
	long int x = len_max - b.length;
	
	for(i = 0; i < b.length; i++)
	{
		a.digit[i+x] = a.digit[i+x] - b.digit[i] - cf;
		if(a.digit[i+x] < 0)
		{
			a.digit[i+x] += 10;
			cf = 1;
		}
		else cf = 0;
	}
}

int s_cmp(b_int &a, b_int &b, long int x)
{
	long int i;
	
	for(i = b.length - 1 + x; i >= 0; i--)
	{
		if(a.digit[i+x] > b.digit[i]) return 1;
		if(a.digit[i+x] < b.digit[i]) return 2;
	}
	return 0;	
}

int s_write(b_int &c, char *binf, char *sign, char *fresult)
{
	FILE *fp;
	long int i, iresult;
	int bf = 0;
	char cf;
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
		fresult = "result";
		fp = fopen(fresult, "w");	
	}
	
	if(strcmp(sign, "/") == 0)
	{
		iresult = 0;
		for(i = 0; i  < c.length; i++)
		if(c.digit[i] == 0)
		iresult++;
		else break;

		if(iresult == c.length)
		{
			iresult--;
			c.flag = 0;
		}
		
		if(c.flag == 1)
		fprintf(fp, "-");
		for(i = iresult; i < c.length; i++)
		{
			cf = c.digit[i] + 48 - bf;
			fprintf(fp, "%c", cf);
		}
		return 0;
	}
	
	iresult = c.length;
	for(i = c.length; i > 0; i--)
	if(c.digit[i] == 0) iresult--;
	else break;
	
	if(iresult == 0) c.flag = 0;
	if(c.flag == 1) fprintf(fp, "-");
	for(i = iresult; i >= 0; i--)
	{
		cf = c.digit[i] + 48 - bf;
		fprintf(fp, "%c", cf);
	}
	return 0;
}

void s_copy(b_int &c, b_int &a)
{
	long int i;
	for(i = 0; i < a.length + 1; i++)
	c.digit[i] = a.digit[i];
}

int s_null(b_int &b)
{
	long int i; 
	int cf = 0;

	b.digit[0] = b.digit[0] - 1;
	for(i = 0; i < b.length + 1; i++)
	{
		b.digit[i] -= cf;
		if(b.digit[i] < 0)
		{
			b.digit[i] += 10;
			cf = 1;
		}
		else cf = 0;
	}
	for(i = 0; i < b.length + 1; i++)
	if(b.digit[i] != 0)
	return 1;
	return 0 ;
}
