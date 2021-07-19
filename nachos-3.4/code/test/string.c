#include "syscall.h"

int main(){
	char str[200];
	ReadString(str);
	PrintString("Hello World I am Minh Duc\n");
	PrintString(str);
	Halt();
}