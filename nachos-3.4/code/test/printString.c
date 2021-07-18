#include "syscall.h"

int main(){
	char* str = NULL;
	ReadString(str, 200);
	PrintString("Your String: ");
	PrintString(str);
	Halt();
}