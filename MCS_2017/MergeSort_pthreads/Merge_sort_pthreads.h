#ifndef MERGE_SORT_PTHREADS_H
#define MERGE_SORT_PTHREADS_H
#include <stdlib.h>
#include <pthread.h>
#include <semaphore.h>

typedef struct ParallelMergeArgument {
  int* mas_;
  int left1_;
  int right1_;
  int left2_;
  int right2_;
  int* result_mas_;
  int left_result_;
  int max_chunk_size_;
  sem_t* semaphore_;
} ParallelMergeArgument;

typedef struct HelpMergeSortArgument {
  int* mas_;
  int left_;
  int right_;
  int max_chunk_size_;
  int* help_mas_;
  int left_help_;
  int num_threads_;
  sem_t* semaphore_;
} HelpMergeSortArgument;

int Compare(const void* a, const void* b);

int isSorted(int* mas, int n); // 0 - not sorted, 1 - sorted

int BinSearch(int x, const int* mas, int left, int right); // Finds max index from [left, right]
                                                     // in the sorted array: mas[index - 1] < x

void swap(int* a, int* b);

void StandardMerge(ParallelMergeArgument* arg); // Merges two sorted parts of array ([left1, right1] and
                                                // [left2, right2]) in the result array at left_result ([left_result, ...])
                                                // position

void* ParallelMerge(void* arg); //Does the same as StandartMerge, but
                                //with other algorithm, to do it in parallel

void* HelpMergeSort(void* arg); // To implement the main MergeSort function

void MergeSort(int* mas, int left, int right, int max_chunk_size, int num_threads); //Main MergeSort function for users

#endif // MERGE_SORT_PTHREADS_H
