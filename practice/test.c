#include<stdio.h>
#include<string.h>

int main(){
	int i = 0;
	char a[] = "321";
	char s[] = "123";

	printf("s :%s ,i: %d\n",s,i);
	strcpy(s,"321");
	printf("Doing strcpy.\n");
	printf("s  :%s, i: %d\n",s,i);
	printf("a :%s\n",a);
	
	return 0;
}
