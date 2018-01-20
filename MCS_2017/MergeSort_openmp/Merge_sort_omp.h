#ifndef MERGE_SORT_OMP_H
#define MERGE_SORT_OMP_H

int Compare(const void* a, const void* b);

int isSorted(const int* mas, int n); // 0 - not sorted, 1 - sorted

int BinSearch(int x, const int* mas, int left, int right);// Finds max index from [left, right]
                                                          // in the sorted array: mas[index - 1] < x

void swap(int* a, int* b);

void StandartMerge(int* mas, int left1, int right1, int left2, int right2,
                   int* result_mas, int left_result); // Merges two sorted parts of array ([left1, right1] and
                                                      // [left2, right2]) in the result array at left_result ([left_result, ...])
                                                      // position

void ParallelMerge(int* mas, int left1, int right1, int left2,
                   int right2, int max_chunk_size, int* result_mas, int left_result); //Does the same as StandartMerge, but
                                                                                      //with other algorithm, to do it in parallel

void HelpMergeSort(int* mas, int left, int right, int max_chunk_size,
                   int* help_mas, int left_help); // To implement the main MergeSort function

void MergeSort(int* mas, int left, int right, int max_chunk_size); //Main MergeSort function for users




#endif // MERGE_SORT_OMP_H
