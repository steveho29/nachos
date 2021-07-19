#include "syscall.h"

int main(){
	char str[200];
	ReadString(str, 200);
	PrintString("This is my string: ");
	PrintString(str);
	PrintString('\n\n');
	Halt();
}