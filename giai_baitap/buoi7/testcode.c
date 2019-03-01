#include <stdio.h>

void f(int *a )
{
	printf("\n in f *a = %d , &a = %p\n" , *a , a );
}
int main()
{
	int k = 1;


	printf("\n in main : *a = %d , &a = %p\n" , k , &k );
	f(&k);
	return 0;

}
