#include "Merge_sort_pthreads.h"

int Compare(const void* a, const void* b) {
  return (*(int*)a - *(int*)b);
}

int isSorted(int* mas, int n) {
  for (int i = 0; i < n - 1; ++i) {
    if (mas[i] > mas[i + 1]) {
      return 0;
    }
  }
  return 1;
}

int BinSearch(int x, const int *mas, int left, int right) {
  if (right < 0) {
    return 0;
  }
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

void StandardMerge(ParallelMergeArgument* arg) {
  int cur_left = arg->left1_;
  int cur_right = arg->left2_;
  int cur = arg->left_result_;

  while (cur_left <= arg->right1_ && cur_right <= arg->right2_) {
    if (arg->mas_[cur_left] < arg->mas_[cur_right]) {
      arg->result_mas_[cur] = arg->mas_[cur_left];
      cur_left++;
    }
    else {
      arg->result_mas_[cur] = arg->mas_[cur_right];
      cur_right++;
    }
    cur++;
  }

  while (cur_left <= arg->right1_) {
    arg->result_mas_[cur] = arg->mas_[cur_left];
    cur++;
    cur_left++;
  }

  while (cur_right <= arg->right2_) {
    arg->result_mas_[cur] = arg->mas_[cur_right];
    cur++;
    cur_right++;
  }
  return;
}

void* ParallelMerge(void* _arg) {
  ParallelMergeArgument* arg = (ParallelMergeArgument*)_arg;
  int size1 = arg->right1_ - arg->left1_ + 1;
  int size2 = arg->right2_ - arg->left2_ + 1;
  if (size1 < size2) {
    swap(&(arg->left1_), &(arg->left2_));
    swap(&size1, &size2);
    swap(&(arg->right1_), &(arg->right2_));
  }

  if (size1 + size2 <= arg->max_chunk_size_ || size2 == 1) {
    StandardMerge(arg);
    return NULL;
  }

  int mid1 = (arg->left1_ + arg->right1_) / 2;
  int mid2 = BinSearch(arg->mas_[mid1], arg->mas_, arg->left2_, arg->right2_);
  int mid3 = arg->left_result_ + (mid1 - arg->left1_) + (mid2 - arg->left2_);
  arg->result_mas_[mid3] = arg->mas_[mid1];

  ParallelMergeArgument new_arg1;
  new_arg1.left1_ = arg->left1_;
  new_arg1.right1_ = mid1 - 1;
  new_arg1.left2_ = arg->left2_;
  new_arg1.right2_ = mid2 - 1;
  new_arg1.max_chunk_size_ = arg->max_chunk_size_;
  new_arg1.mas_ = arg->mas_;
  new_arg1.result_mas_ = arg->result_mas_;
  new_arg1.left_result_ = arg->left_result_;
  new_arg1.semaphore_ = arg->semaphore_;

  ParallelMergeArgument new_arg2;
  new_arg2.left1_ = mid1 + 1;
  new_arg2.right1_ = arg->right1_;
  new_arg2.left2_ = mid2;
  new_arg2.right2_ = arg->right2_;
  new_arg2.max_chunk_size_ = arg->max_chunk_size_;
  new_arg2.mas_ = arg->mas_;
  new_arg2.result_mas_ = arg->result_mas_;
  new_arg2.left_result_ = mid3 + 1;
  new_arg2.semaphore_ = arg->semaphore_;

  if (sem_trywait(arg->semaphore_) == 0) {
    pthread_t thread;
    pthread_create(&thread, NULL, ParallelMerge, (void*)&new_arg1);
    ParallelMerge((void*)&new_arg2);
    pthread_join(thread, NULL);
    sem_post(arg->semaphore_);
  }
  else {
    ParallelMerge((void*)&new_arg1);
    ParallelMerge((void*)&new_arg2);
  }

  return NULL;
}

void* HelpMergeSort(void *_arg) {
  HelpMergeSortArgument* arg = (HelpMergeSortArgument*)_arg;
  int size = arg->right_ - arg->left_ + 1;
  if (size <= arg->max_chunk_size_) {
    qsort(arg->mas_ + arg->left_, size, sizeof(int), Compare);
    return NULL;
  }

  int split = (arg->left_ + arg->right_) / 2;

  HelpMergeSortArgument new_arg1;
  new_arg1.semaphore_ = arg->semaphore_;
  new_arg1.mas_ = arg->mas_;
  new_arg1.left_ = arg->left_;
  new_arg1.right_ = split;
  new_arg1.max_chunk_size_ = arg->max_chunk_size_;
  new_arg1.help_mas_ = arg->help_mas_;
  new_arg1.left_help_ = arg->left_help_;

  HelpMergeSortArgument new_arg2;
  new_arg2.semaphore_ = arg->semaphore_;
  new_arg2.mas_ = arg->mas_;
  new_arg2.left_ = split + 1;
  new_arg2.right_ = arg->right_;
  new_arg2.max_chunk_size_ = arg->max_chunk_size_;
  new_arg2.help_mas_ = arg->help_mas_;
  new_arg2.left_help_ = split + 1;

  ParallelMergeArgument paralle_merge_arg;
  paralle_merge_arg.semaphore_ = arg->semaphore_;
  paralle_merge_arg.mas_ = arg->mas_;
  paralle_merge_arg.left1_ = arg->left_;
  paralle_merge_arg.right1_ = split;
  paralle_merge_arg.left2_ = split+ 1;
  paralle_merge_arg.right2_ = arg->right_;
  paralle_merge_arg.max_chunk_size_ = arg->max_chunk_size_;
  paralle_merge_arg.result_mas_ = arg->help_mas_;
  paralle_merge_arg.left_result_ = arg->left_help_;

  if (sem_trywait(arg->semaphore_) == 0) {
    pthread_t thread;
    pthread_create(&thread, NULL, HelpMergeSort, (void*)&new_arg1);
    HelpMergeSort((void*)&new_arg2);
    pthread_join(thread, NULL);
    sem_post(arg->semaphore_);
  }
  else {
    HelpMergeSort((void*)&new_arg1);
    HelpMergeSort((void*)&new_arg2);
  }

  ParallelMerge((void*)&paralle_merge_arg);

  for (int i = arg->left_help_; i < arg->left_help_ + size; ++i) {
    arg->mas_[i] = arg->help_mas_[i];
  }
  return NULL;
}

void MergeSort(int *mas, int left, int right, int max_chunk_size, int num_threads) {
  int* help_mas = (int*)malloc(sizeof(int) * (right - left + 1));

  HelpMergeSortArgument arg;
  sem_t semaphore;
  sem_init(&semaphore, 0, num_threads - 1);
  arg.semaphore_ = &semaphore;
  arg.mas_ = mas;
  arg.left_ = left;
  arg.right_ = right;
  arg.max_chunk_size_ = max_chunk_size;
  arg.left_help_ = 0;
  arg.help_mas_ = help_mas;

  HelpMergeSort(&arg);

  sem_destroy(&semaphore);
  free(help_mas);
  return;
}





