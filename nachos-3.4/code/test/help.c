#include "syscall.h"


void printTree(){
	PrintString(" 	  .
         DUC\n
       19127368\n
     .....VY.....\n
    ...19127088...\n
  .......NGA........\n
 .....19127219.......\n
         |||\n
         |||\n
         |||\n
 DO AN 2 HE DIEU HANH\n\n\n");
	
}
int main(){

	printTree();
	PrintString("SORT:\n	-Cho phep nguoi dung nhap vao mot mang n so nguyen voi n la so do nguoi dung nhap vao(n<=100)\n	-Neu nhap khong hop le thi mac dinh la 0\n	-In ra man hinh mang tang dan\n\n");
	PrintString("Ascii:\n	-In ra bang ma ascii tu ki tu 32 den 127 theo cau truc:  dec - hex - char\n\n");
	Halt();
}
