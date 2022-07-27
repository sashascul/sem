#include<stdio.h>
#include<string.h>
#include<stdlib.h>
// F_n = F_{n-1} + F_{n-2}
//
//
// F_0 = 1 , F_1 = 1
//
//
//
int K = 0;

// S_{n+1} = F(S_n);
//
// S_0 = {1, 1}
// S_1 = {1, 2}
// S_2 = {2, 3}
// S_3 = {3, 5}

int fib2(int n, int a, int b) {
	K++;
	if ( n <= 1 )
		return b;
	else
		return fib2(n-1, b, a+b);

}

//
//  (0, 1)  (1)     (1)
//  (    ) x( )   = ( )
//  (1, 1)  (1)     (2)
//
//  A x A x A x v == A^3 x v
//
//
//
int pow(int x, int n) {
	if ( n == 0) return 1;
	else if ( n == 1) return x;
	else if ( n % 2 == 0 ) {
		int y = pow(x, n/2);
		return y*y;
	} else
		return x*pow(x, n-1);

}


typedef struct _int_matrix {
	int cols;
	int rows;
	int data[];

} int_matrix;

int int_matrix_init_zero(int_matrix** result, int cols, int rows) {
	// alloctation
	*result = malloc(sizeof(int_matrix) + cols*rows*sizeof(int));
	if ( !*result ) return -1;
	// zeroing
	bzero((*result)->data,  cols*rows*sizeof(int));
	(*result)->cols = cols;
	(*result)->rows = rows;

	return 0;
}

#define CELL(r, i, j)  ((r)*(i)+(j))
#define MCELL(m, i, j) (m)->data[(m)->cols*(i) + (j)]

int int_matrix_init_eye(int_matrix** result, int cols, int rows) {
	int i;
	if ( cols != rows ) return -2; // error, only square matrix
	if ( int_matrix_init_zero(result, cols, rows) ) return -1;
	for ( i = 0; i < rows; i++ )
		MCELL(*result, i, i) = 1;
	return 0;
}


int int_matrix_multiply(int_matrix** result, int_matrix* a, int_matrix* b) {
	int i, j, k;
	if ( a -> cols != b->rows ) return -2;
	if ( int_matrix_init_zero(result, b->cols, a->rows)) return -1;
	for ( i = 0; i < a->rows; i++ )//rows
		for ( j = 0; j < b->cols; j++ )//cols
			for (k = 0; k < a->cols; k++ )//cols
				MCELL(*result, i, j) += MCELL(a, i, k) * MCELL(b, k, j);
	return 0;
}


int int_matrix_transpose(int_matrix** result, int_matrix* a) {
	int i, j;
	if ( int_matrix_init_zero(result, a->cols, a->rows) ) return -1;
	for ( i = 0; i < a->rows; i++ )
		for (j =0; j < a->cols; j++ )
			MCELL(*result, i, j) = MCELL(a, j, i);

	return 0;
}

int int_matrix_free(int_matrix* a) {
	free(a);
	return 0;
}

int int_matrix_copy(int_matrix** result, int_matrix* a) {
	*result = malloc(sizeof(int_matrix) + a->rows*a->cols*sizeof(int));
	if ( !* result ) return -1;
	memcpy(*result, a, sizeof(int_matrix) + a->rows*a->cols*sizeof(int));
	return 0;
}

int int_matrix_power(int_matrix** result, int_matrix* a, int power) {
	if ( power == 0)
		return int_matrix_init_eye(result, a->rows, a->rows);
	else if ( power == 1)
		return int_matrix_copy(result, a);
	else if ( power  % 2 == 0 ) {
		int_matrix* b;
		int_matrix_power(&b, a, power/2);
		int_matrix_multiply(result, b, b);
		int_matrix_free(b);
	} else {
		int_matrix* b;
		int_matrix_power(&b, a, power-1);
		int_matrix_multiply(result, a, b);
		int_matrix_free(b);
	}
	return 0;
}

int int_matrix_print(int_matrix* a, char* prefix) {
	int i,  j;
	int maxs  =0;
	printf("Matrix %s: %dx%d:\n", prefix, a->cols, a->rows);
	for ( i = 0; i < a->rows; i++ )//rows
		for ( j = 0; j < a->cols; j++ ) {//cols
			char buf[20];
			sprintf(buf, "%d", MCELL(a, i ,j));
			if ( strlen(buf) > maxs) maxs = strlen(buf);
		}
	for ( i = 0; i < a->rows; i++ ) {//rows
		for ( j = 0; j < a->cols; j++ ) {//cols
			printf("%*d ", maxs, MCELL(a, i, j));
		}
		printf("\n");
	}
	return 0;
}


int fib(int n) {
	K++;
	if ( n <= 1) return 1;
	return fib(n-1) + fib(n-2);

}


int ns[10] = {1,2,3,4,5,6,7,8,9,20};
int fibs[10] = {2,3,5,8,13,21,34,55,89,144};
int test() {
	int i;
	for ( i = 0; i < sizeof(ns)/sizeof(ns[0]); i++ ) {
		printf("Checking fib(%d) = %d...",ns[i], fibs[i]);
		if ( fib(ns[i]) == fibs[i] ) {
			printf("OK\n");
		} else {
			printf("Failed!\n");
		}
	}
}

typedef struct _mat {
	int a, b, c, d;
} mat;


int mpow(mat* r, mat x, int n) {
	if (n == 0)
		*r = (mat) {1,0,0,1};
	else if ( n == 1) {
		*r = x;
	}else if ( n % 2 == 0 ) {
		mat y;
		mpow(&y, x, n/2);
		mmul(r, y, y);
	} else {
		mat y;
		mpow(&y, x, n-1);
		mmul(r, x, y);
	}
	return 0;
}
// (a b)
// (c d)

#define MIN(a, b) ((a)<(b)?(a):(b))

int mmul(mat* r, mat x, mat y) {
	*r = (mat){ x.a*y.a+ x.b*y.c,
		x.a*y.b + x.b*y.d,
		x.c*y.a + x.d*y.c,
		x.c*y.b + x.d*y.d
	};
	return 0;
}

int mprint(mat x) {
	printf("%d %d  \n%d %d", x.a , x.b , x.c, x.d);
    return 0;
}

int main() {
	int n;
    int_matrix* a;
	int_matrix* b;
	int_matrix* c;
	int_matrix* r;
    scanf("%d", &n);
	int_matrix_init_zero(&b, 2,2);
	int_matrix_init_zero(&c, 1,2);
    MCELL(c, 0, 0) = 1;
	MCELL(c, 0, 1) = 1;
	MCELL(b, 0, 0) = 0;
	MCELL(b, 0, 1) = 1;
	MCELL(b, 1, 0) = 1;
	MCELL(b, 1, 1) = 1;
	int_matrix_power(&a, b, n);
    int_matrix_multiply(&r, a, c);//a c
	int_matrix_print(a, "A");
	int_matrix_print(b, "B");
	int_matrix_print(r, "R");
    //printf("%d\n", MCELL(r, 0, 0));
	int_matrix_free(a);
	int_matrix_free(b);
	int_matrix_free(c);
	int_matrix_free(r);
    mat x=(mat){0, 1, 1, 1};
    mat y;
    mpow(&y, x, n);
    printf("%d\n", (y.a + y.b));
    mprint(y);
    return 0;
}
