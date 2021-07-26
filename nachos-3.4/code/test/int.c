#include "syscall.h"

int main(){
	int n= 0;
	PrintString("Input your number: ");
	n = ReadInt();
	PrintString("Your Number: ");
	PrintInt(n);
	PrintString("\n\n");
	Halt();
}
