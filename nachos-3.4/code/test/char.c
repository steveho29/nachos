#include "syscall.h"

int main(){
	char ch = ReadChar();
	PrintString("This is my char: ");
	PrintChar(ch);
	Halt();
}