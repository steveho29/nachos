#include "syscall.h"

int main(){
	int n = ReadInt();
	PrintString("Your Number: ");
	PrintInt(n);
	PrintString("\n\n");
	Halt();
}