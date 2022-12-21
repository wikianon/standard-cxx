/*
 * C++ program to sort an array using bucket sort
 * https://www.geeksforgeeks.org/bucket-sort-2/
 * 
 */
#include <vector>
#include <iostream>
#include <algorithm>

// Function to sort arr[] of size n using bucket sort
void bucketSort(float arr[], int n)
{
    // 1) Create n empty buckets
    std::vector<float> b[n];
    
    // 2) Put array elements in different buckets
    for (int i=0; i<n; i++)
    {
       int bi = n*arr[i]; // Index in bucket
       b[bi].push_back(arr[i]);
    }
 
    // 3) Sort individual buckets
    for (int i=0; i<n; i++)
       sort(b[i].begin(), b[i].end());
 
    // 4) Concatenate all buckets into arr[]
    int index = 0;
    for (int i = 0; i < n; i++)
        for (int j = 0; j < b[i].size(); j++)
          arr[index++] = b[i][j];
}
 
/* Driver program to test above funtion */
int main()
{
    float arr[] = {0.897, 0.565, 0.656, 0.1234, 0.665, 0.3434};
    int n = sizeof(arr)/sizeof(arr[0]);
    bucketSort(arr, n);
 
    std::cout << "Sorted array is \n";
    for (int i=0; i<n; i++)
       std::cout << arr[i] << " ";
    std::cout<<std::endl;
    return 0;
}
/*
Output:

Sorted array is
0.1234 0.3434 0.565 0.656 0.665 0.897
*/