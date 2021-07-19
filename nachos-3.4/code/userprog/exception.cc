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
	int virtAddr, limit, numBytes
	char* str;

	// Địa chỉ lưu chuỗi của user
	virtAddr = machine->ReadRegister(4); 

	// Độ dài chuỗi user chọn
	limit = machine->ReadRegister(5);

	// Đọc chuỗi từ console vào str 
	// Lấy ra so ki tu doc duoc
	str = new char [limit+1];
	numBytes = gSynchConsole->Read(str, limit)

	// Copy buffer từ system vào user với số byte = numBytes đọc được từ console
	System2User(virtAddr, numBytes,  str); 
	return str;
}



void printString(){
	int virtAddr;
	char* buffer;

	// Lấy địa chỉ lưu chuỗi của user
	virtAddr = machine->ReadRegister(4);

	// Khởi tạo kernel buffer. Copy chuỗi từ user sang kernel buffer
	buffer = User2System(virtAddr, 200); 

	// In ra màn hình console
	gSynchConsole->Write(buffer, 200);
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

			} 
			advancePC();
			break;
		}

		default:
			printf("Unexpected user mode exception %d %d\n", which, type);
			ASSERT(FALSE);
	}

   
}	
