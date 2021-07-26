#include "syscall.h"

int main(){
	char ch;
	PrintString("Input a char: ");
	ch = ReadChar();
	PrintString("This is my char: ");
	PrintChar(ch);
	PrintString("\n\n");
	Halt();
}
