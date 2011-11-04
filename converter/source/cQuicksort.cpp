#include "cQuicksort.h"
#include <ctype.h>



// Quicksort controller function, it partitions the different pieces of our array.
void cQuicksort::quicksort(int *arIntegers, int left, int right)
{
    if (right > left)
    {
         int pivotIndex = median3(arIntegers,left,right);
         int pivotNewIndex = partition(arIntegers, left, right, pivotIndex);

         // Recursive call to quicksort to sort each half.
         quicksort(arIntegers, left, pivotNewIndex-1);
         quicksort(arIntegers, pivotNewIndex+1, right);
    }
}

int cQuicksort::median3(int *arIntegers,int left,int right)
{
        int center = (left+right)/2;

   if(arIntegers[center] < arIntegers[left])
        swap(arIntegers[left],arIntegers[center]);
   if(arIntegers[right] < arIntegers[left])
        swap(arIntegers[left],arIntegers[right]);
   if(arIntegers[right] < arIntegers[center])
        swap(arIntegers[center],arIntegers[right]);

   swap(arIntegers[center],arIntegers[right-1]);

   return center;
}

// This function takes an array (or one half an array) and sorts it.
// It then returns a new pivot index number back to quicksort.
int cQuicksort::partition(int *arIntegers, int left, int right, int pivot)
{
     int pivotValue = arIntegers[pivot];

     // Swap it out all the way to the end of the array
     // So we know where it always is.
     swap(arIntegers[pivot], arIntegers[right]);
     int storeIndex = left;

     // Move through the array from start to finish comparing each to our
     // pivot value (not index, the value that was located at the pivot index)
     for (int i = left; i < right; i++)
     {
         if (arIntegers[i] <= pivotValue)
         {
             swap(arIntegers[i], arIntegers[storeIndex]);
             storeIndex++;
         }
     }
     swap(arIntegers[storeIndex], arIntegers[right]);
     return storeIndex;
}

// Simple swap function for our in place swapping.
void cQuicksort::swap(int &val1, int &val2)
{
    int temp = val1;
    val1 = val2;
    val2 = temp;
}
