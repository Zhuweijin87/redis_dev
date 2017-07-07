#include <stdio.h>
#include "sds.h"

int main()
{
	sds  a = sdsnewlen("abcd234345", 10);

	printf("a: %s\n", a);	
	sdsfree(a);
	printf("a: %s\n", a);

	sds b = sdscpy(a, "aaaaaa");
	printf("a: %s\n", a);
	printf("b: %s\n", b);
	sdsclear(b);
	printf("b: %s\n", b);
	
	sdscpy(b, "12345678");
	sds c = sdsdup(b);
	printf("c: %s\n", c);
	
	sdsrange(c, 2, 4);
	printf("range c[2,4]: %s\n", c);

	long long int n = 9231454;	
	sds d = sdsfromlonglong(n);
	//printf("d: %s\n", d);
	return 0;
}
