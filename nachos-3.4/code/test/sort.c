/* sort.c 
 *    Test program to sort a large number of integers.
 *
 *    Intention is to stress virtual memory system.
 *
 *    Ideally, we could read the unsorted array off of the file system,
 *	and store the result back to the file system!
 */
int A[1024];
#include "syscall.h"


int main()
{   
    int n, i, j, tmp;
    PrintString("Input N <= 100: ");
    n = ReadInt();
    if (n <= 0)
    {
        Halt();
        return 0;
    }

    for (i = 0; i<n; i++){
        PrintString("A[");
        PrintChar(((char)i+48));
        PrintString("] = ");
        A[i] = ReadInt();
    }
    

    // Bubble sort
    for (i = 0; i<n-1; i++)
        for (j = 0; j<n-i-1; j++){
            if (A[j] > A[j+1])
            {
                tmp = A[j];
                A[j] = A[j+1];
                A[j+1] = tmp;
            }
        }
    

    // Output sorted Array
    PrintString("Sorted Array: ");
    for (i = 0; i<n; i++){
        PrintInt(A[i]);
        PrintString(" ");
    }
    Halt();
}
