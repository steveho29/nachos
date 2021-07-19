/* sort.c 
 *    Test program to sort a large number of integers.
 *
 *    Intention is to stress virtual memory system.
 *
 *    Ideally, we could read the unsorted array off of the file system,
 *	and store the result back to the file system!
 */

#include "syscall.h"


int main()
{   
    int n, *A;
    PrintString("Input N <= 100: ");
    n = ReadInt();
    if (n > 0)
        A = new int[n];
    else
    {
        Halt();
        return 0;
    }

    for (int i = 0; i<n; i++){
        int num = ReadInt();
        A[i] = num;
    }
    

    // Bubble sort
    for (int i = 0; i<n-1; i++)
        for (int j = 0; j<n-i-1; j++){
            if (A[j] > A[j+1])
            {
                int tmp = A[j];
                A[j] = A[j+1];
                A[j+1] = tmp;
            }
        }
    


    // Output sorted Array
    PrintString("Sorted Array: ");
    for (int i = 0; i<n; i++){
        PrintInt(A[i]);
        PrintString(" ");
    }
    Halt();
}
