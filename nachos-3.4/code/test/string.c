#include "syscall.h"

int main(){
	char str[200];
	ReadString(str, 200);
	PrintString("My string: ");
	PrintString(str);
	PrintString('\n\n');
	Halt();
}