#include "syscall.h"

void printTriangle(int n){
	int i, j;
	for (i = 0;i<n;i++){
		for (j = 0;j<n-i;j++)
			PrintChar(' ');
		for (j = 0;j<2 * i + 1; j ++)
			PrintChar('V');
		PrintString("\n");

	}
}

void printTree(int n){
	printTriangle(n);
	int i, j;
	for (i = 0;i<n/2;i++){
		for (j = 0;j<n-i;j++)
			PrintChar(' ');
		PrintString("|||\n");
	}
}
int main(){
	printTree(10);
	Halt();
}