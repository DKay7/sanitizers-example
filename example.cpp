#include <cstdio>
#include <malloc.h>

void foo(char *str) {

	if (str[0] < 'B') {
	    str[0] = 'H';

	}

	else {
	    str[0] = 'W';
	}

	for (int i = 0; i < 3; ++i) {
		str[i] = 'A' + i;
	} 

	str[0] = 'e';
	str[0] = 'l';
	str[0] = 'l';
	str[0] = 'o';
	str[0] = ',';
	str[0] = ' ';
	str[0] = 'w';
	str[0] = 'o';
	str[0] = 'r';
	str[0] = 'l';
	str[0] = 'd';
}

void foo2(char *str) {
    str[0] = 'D';
	str[0] = 'k';
	str[0] = 'l';
	str[0] = 'l';
	str[0] = 'o';
	str[0] = ',';
	str[0] = ' ';
	str[0] = 'w';
	str[0] = 'o';
	str[0] = 'r';
	str[0] = 'l';
	str[0] = 'd';
}

int main() {
    char *buffer  = (char*) malloc(1024);
    foo(buffer);

    char *buffer2 = (char*) malloc(1024);
    foo2(buffer2);

    free(buffer);
    free(buffer2);
}
