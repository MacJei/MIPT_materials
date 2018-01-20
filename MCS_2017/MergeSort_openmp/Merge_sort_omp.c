#include "Merge_sort_omp.h"
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <omp.h>

int Compare(const void* a, const void* b) {
  return (*(int*)a - *(int*)b);
}

int isSorted(const int* mas, int n) {
  for (int i = 0; i < n - 1; ++i) {
    if (mas[i] - mas[i + 1] > 0) {
      return 0;
    }
  }
  return 1;
}

int BinSearch(int x, const int *mas, int left, int right) {
  if (x > mas[right]) {
    return right + 1;
  }
  if (x < mas[left]) {
    return left;
  }
  int mid = (left + right) / 2;
  while (left < right - 1) {
    if (mas[mid] > x) {
      right = mid;
      mid = (left + right) / 2;
    }
    else {
      left = mid;
      mid = (left + right) / 2;
    }
  }
  return mid + 1;
}

void swap(int* a, int* b) {
  int temp = *b;
  *b = *a;
  *a = temp;
  return;
}

void StandartMerge(int *mas, int left1, int right1, int left2, int right2,
                   int *result_mas, int left_result) {
  int cur_left = left1;
  int cur_right = left2;
  int cur = left_result;

  while (cur_left <= right1 && cur_right <= right2) {
    if (mas[cur_left] < mas[cur_right]) {
      result_mas[cur] = mas[cur_left];
      cur_left++;
    }
    else {
      result_mas[cur] = mas[cur_right];
      cur_right++;
    }
    cur++;
  }

  while (cur_left <= right1) {
    result_mas[cur] = mas[cur_left];
    cur_left++;
    cur++;
  }
  while (cur_right <= right2) {
    result_mas[cur] = mas[cur_right];
    cur_right++;
    cur++;
  }
  return;
}

void ParallelMerge(int *mas, int left1, int right1, int left2,
                   int right2, int max_chunk_size, int *result_mas, int left_result) {
  int size1 = right1 - left1 + 1;
  int size2 = right2 - left2 + 1;
  if (size1 < size2) {
    swap(&left1, &left2);
    swap(&size1, &size2);
    swap(&right1, &right2);
  }

  if (size1 + size2 <= max_chunk_size || size2 == 1) {
    StandartMerge(mas, left1, right1, left2, right2, result_mas, left_result);
    return;
  }

  int mid1 = (left1 + right1) / 2;
  int mid2 = BinSearch(mas[mid1], mas, left2, right2);
  int mid3 = left_result + (mid1 - left1) + (mid2 - left2);
  result_mas[mid3] = mas[mid1];
  #pragma omp task
  {
    ParallelMerge(mas, left1, mid1 - 1, left2, mid2 - 1, max_chunk_size, result_mas,
                  left_result);
  }

  #pragma omp task
  {
    ParallelMerge(mas, mid1 + 1, right1, mid2, right2, max_chunk_size, result_mas,
                  mid3 + 1);
  }

#pragma omp taskwait
  return;
}

void HelpMergeSort(int *mas, int left, int right, int max_chunk_size,
                   int *help_mas, int left_help) {
  int size = right - left + 1;
  if (size <= max_chunk_size) {
    qsort(mas + left, size, sizeof(int), Compare);
    return;
  }

  int split = (left + right) / 2;
  #pragma omp task
  {
    HelpMergeSort(mas, left, split, max_chunk_size, help_mas, left_help);
  }

  #pragma omp task
  {
    HelpMergeSort(mas, split + 1, right, max_chunk_size,
                  help_mas, split + 1);
  }

  #pragma omp taskwait

  ParallelMerge(mas, left, split, split + 1, right, max_chunk_size, help_mas, left_help);
  for (int i = left_help; i < left_help + size; ++i) {
    mas[i] = help_mas[i];
  }
  return;
}

void MergeSort(int *mas, int left, int right, int max_chunk_size) {
  int* help_mas = (int*)malloc((right - left + 1) * sizeof(int));
  #pragma omp parallel
  {
    #pragma omp single
    {
      HelpMergeSort(mas, left, right, max_chunk_size, help_mas, 0);
    }
  }

  free(help_mas);
  return;
}
