// exception.cc 
//	Entry point into the Nachos kernel from user programs.
//	There are two kinds of things that can cause control to
//	transfer back to here from user code:
//
//	syscall -- The user code explicitly requests to call a procedure
//	in the Nachos kernel.  Right now, the only function we support is
//	"Halt".
//
//	exceptions -- The user code does something that the CPU can't handle.
//	For instance, accessing memory that doesn't exist, arithmetic errors,
//	etc.  
//
//	Interrupts (which can also cause control to transfer from user
//	code into the Nachos kernel) are handled elsewhere.
//
// For now, this only handles the Halt() system call.
// Everything else core dumps.
//
// Copyright (c) 1992-1993 The Regents of the University of California.
// All rights reserved.  See copyright.h for copyright notice and limitation 
// of liability and disclaimer of warranty provisions.

#include "copyright.h"
#include "system.h"
#include "syscall.h"

//----------------------------------------------------------------------
// ExceptionHandler
// 	Entry point into the Nachos kernel.  Called when a user program
//	is executing, and either does a syscall, or generates an addressing
//	or arithmetic exception.
//
// 	For system calls, the following is the calling convention:
//
// 	system call code -- r2
//		arg1 -- r4
//		arg2 -- r5
//		arg3 -- r6
//		arg4 -- r7
//
//	The result of the system call, if any, must be put back into r2. 
//
// And don't forget to increment the pc before returning. (Or else you'll
// loop making the same system call forever!
//
//	"which" is the kind of exception.  The list of possible exceptions 
//	are in machine.h.
//----------------------------------------------------------------------



// machine object is in machine.h
void advancePC(){
	machine->WriteRegister(PrevPCReg, machine->ReadRegister(PCReg));
    machine->WriteRegister(PCReg, machine->ReadRegister(NextPCReg));
    machine->WriteRegister(NextPCReg, machine->ReadRegister(NextPCReg) + 4);
}



//  Copy buffer from system memory to user memory
int System2User(int virtAddr, int len, char* buffer)
{
	if (len < 0) return -1;
	if (len == 0) return 0;
	int i = 0;
	int oneChar = 0;
	do {
		oneChar = (int)buffer[i];
		// WriteMem in translate.cc. Gan value vao machine->mainMemory[]
		machine->WriteMem(virtAddr+i, 1, oneChar); 
		i++;
	} while (i< len && oneChar != 0);

	return i; // Number bytes copied
}


// Copy buffer from user memory to system memory
char* User2System(int virtAddr, int limit){
	int i;
	int oneChar;
	char* kernelBuf = NULL;

	kernelBuf = new char [limit+1];
	if (kernelBuf == NULL)
		return NULL;

	memset(kernelBuf, 0, limit+1);

	for (i = 0; i< limit;i++){
		machine->ReadMem(virtAddr+i, 1, &oneChar);
		kernelBuf[i] = (char)oneChar;
		if (oneChar == 0)
			break;
	}
	
	return kernelBuf;
}



char* readString(){
	int virtAddr, limit, numBytes;
	char* str;

	// Dia chi luu chuoi cua USER
	virtAddr = machine->ReadRegister(4); 

	// Do dai chuoi ma USER chon
	limit = machine->ReadRegister(5);

	// Doc chuoi tu console vao str
	// Lay duoc so ki tu doc duoc
	str = new char [limit+1];
	numBytes = gSynchConsole->Read(str, limit);

	if (numBytes >= 0)
		// Copy buffer tu System vao User voi so byte = numBytes doc duoc tu console
		System2User(virtAddr, numBytes,  str);
	else
		printf("Console ERROR\n\n");

	return str;
}



void printString(){
	int virtAddr, numBytes;
	char* buffer;

	// Lay dia chi luu chuoi cua USER
	virtAddr = machine->ReadRegister(4);

	// Khoi tao kernel buffer. Copy chuoi tu user sang kernel buffer
	buffer = User2System(virtAddr, 200); 

	// In ra man hinh console
	gSynchConsole->Write(buffer, 200);

	// Delete buffer tranh leak memory
	delete buffer;
}



char readChar(){
	int virtAddr, limit = 200, numBytes;
	char* buffer, ch = '\0';

	// Lay dia chi luu ki tu cua USER
	virtAddr = machine->ReadRegister(4);

	// Doc chuoi tu console
	buffer = new char [limit+1];
	numBytes = gSynchConsole->Read(buffer, limit);

	// Check If A Valid Char
	if (numBytes == 1)
		ch = buffer[0];
	else if (numBytes > 1)
		printf("Invalid Char\n\n");
	else
		printf("Console ERROR\n\n");
	

	// Delete buffer tranh leak memory
	delete buffer;
	return ch;
}


void printChar(){
	char ch = (char)machine->ReadRegister(4);
	gSynchConsole->Write(&ch, 1);
}


int readInt(){
	int num = 0, digit, numBytes, limit = 200;

	int MAX_INT = 2147483647, MIN_INT = -2147483648;
	long long tmp = 0;
	char* buffer;
	bool isNegative = false;


	// Doc chuoi vao buffer
	buffer = new char[limit+1];
	numBytes = gSynchConsole->Read(buffer, limit);
	if (numBytes == 0) 
		return 0;

	int i = 0;

	// Skip khoang trang truoc chu so
	for (;i<numBytes && buffer[i] == ' ';i++);


	// Check neu la so am
	if (buffer[i] == '-')
	{
		isNegative = true;
		++i;
	}

	for (;i<numBytes;i++){

		// Ki tu khong phai chu so
		if ('9' < buffer[i] || buffer[i] < '0' ){
			printf("Invalid Integer\n\n");
			return 0;
		}

		digit = (int)(buffer[i] - 48);

		tmp = tmp * 10 + digit;

		// Check overflow integer
		if ((tmp > MAX_INT && !isNegative) || (-tmp < MIN_INT && isNegative))
		{
			printf("Integer Overflow\n\n");
			return 0;
		}

		num = num * 10 + digit;
	}

	if (isNegative)
		num = -num;
	

	// Giai phong buffer
	delete buffer;

	return num;
}



void printInt(){
	int num = machine->ReadRegister(4);
	// Neu la sp duong lon hon 0
	if (0 <= num && num <= 9)
	{	
		char ch = (char)(num+48);
		gSynchConsole->Write(&ch, 1);
		return;
	}

	// max number digits of integer = 10 (MAX_INT = 2^31-1 = 2147483647)
	int limit = 10;

	// number of digits
	int n = 0;

	char* buffer = new char[limit+1];
	bool isNegative = false;

	if (num < 0){
		isNegative = true;
		num = -num;
	}

	while (num > 0){
		buffer[n] = (char)(num%10 + 48);
		num /= 10;
		++n;
	}


	int i = 0;
	char* number;
	if (isNegative)
	{	
		++n;
		++i;
		number = new char[n+1];
		number[0] = '-';
	}
	else number = new char[n+1];

	for (;i<n;i++)
		number[i] = buffer[n-i-1];


	// In ra man hinh console
	gSynchConsole->Write(number, n);

	delete buffer;
	delete number;
}



void ExceptionHandler(ExceptionType which)
{
	int type = machine->ReadRegister(2);
	


	switch (which){
		case NoException:
			return;
		case PageFaultException:
			printf("PageFaultException dang dien ra\n");
			interrupt->Halt();
			break;
		case ReadOnlyException:
			printf("ReadOnlyException dang dien ra\n");
			interrupt->Halt();
			break;
		case BusErrorException:
			printf("BusErrorException dang dien ra\n");
			interrupt->Halt();
			break;
		case AddressErrorException:
			printf("AddressErrorException dang dien ra\n");
			interrupt->Halt();
			break;
		case OverflowException:
			printf("OverflowException dang dien ra\n");
			interrupt->Halt();
			break;
		case IllegalInstrException:
			printf("IllegalInstrException dang dien ra\n");
			interrupt->Halt();
			break;
		case NumExceptionTypes:
			printf("NumExceptionTypes dang dien ra\n");
			interrupt->Halt();
			break;
		case SyscallException:
		{
			switch (type){
				case SC_Halt:
					DEBUG('a', "Shutdown, initiated by user program.\n");
   					interrupt->Halt();
					break;


				case SC_Sub:
					int op1, op2, result;
					op1 = machine->ReadRegister(4);
					op2 = machine->ReadRegister(5);
					result = op1 - op2;
					machine->WriteRegister(2, result);
					break;


				case SC_ReadString:
					if (readString() == NULL)
						printf("\nNot enought memory in system");
					break;


				case SC_PrintString:
					printString();
					break;


				case SC_ReadChar:
					char ch;
					ch = readChar();
					machine->WriteRegister(2, int(ch));
					break;


				case SC_PrintChar:
					printChar();
					break;


				case SC_ReadInt:
					int num;
					num = readInt();
					machine->WriteRegister(2, num);
					break;


				case SC_PrintInt:
					printInt();
					break;
			} 
			advancePC();
			break;
		}

		default:
			printf("Unexpected user mode exception %d %d\n", which, type);
			ASSERT(FALSE);
	}

   
}	
