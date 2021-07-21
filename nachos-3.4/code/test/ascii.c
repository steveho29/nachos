#include "syscall.h"

void intToHex(int a){
	char hex[100];
	int i = 0;
	while (a != 0) {
		int temp = 0;
		temp = a%16;
		if (temp < 10) {
			hex[i] = temp + 48;
			i++;
		}
		else {
			hex[i] = temp + 55;
			i++;
		}
		a = a/16;
	}
	for (int j = i - 1; j >= 0; j--)
		PrintChar(hex[j]);
}

int main() {
	int i = 32;
	PrintString("-----Ascii Table----");
	PrintString("  Dec    Hex    Char");
	for (;i<128;i++) {
		PrintString("   ");
		PrintInt((int)i);
		PrintString("    ");
		intToHex(i);
		PrintString("     ");
		PrintChar(i);
		PrintChar('\n');
	}
	Halt();
}
