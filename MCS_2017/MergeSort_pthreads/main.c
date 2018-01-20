#include <stdio.h>
#include <time.h>
#include <omp.h>
#include "Merge_sort_pthreads.h"

int main(int argc, char* argv[]) {
  if (argc != 4) {
    printf("Wrong number of arguments\n");
    exit(-1);
  }
  int n = atoi(argv[1]);
  int chunk_size = atoi(argv[2]);
  int num_threads = atoi(argv[3]);

  srand(time(NULL));

  FILE* data_file = fopen("data.txt", "w");
  int* mas = (int*)malloc(n * sizeof(int));
  for (int i = 0; i < n; ++i) {
    mas[i] = rand();
    fprintf(data_file, "%d ", mas[i]);
  }
  fprintf(data_file, "\n");

  float start_time = omp_get_wtime();
  MergeSort(mas, 0, n - 1, chunk_size, num_threads);
  float end_time = omp_get_wtime();
  float elapsed = end_time - start_time;

  if (!isSorted(mas, n)) {
    printf("UNSORTED!!!\n");
  }

  for (int i = 0; i < n; ++i) {
    fprintf(data_file, "%d ", mas[i]);
  }
  fprintf(data_file, "\n");
  fclose(data_file);

  FILE* stats_file = fopen("stats.txt", "a");
  fprintf(stats_file, "%.6fs %d %d %d\n", elapsed, n, chunk_size, num_threads);
  fclose(stats_file);

  for (int i = 0; i < n; ++i) {
    mas[i] = rand();
  }
  start_time = omp_get_wtime();
  qsort(mas, n, sizeof(int), Compare);
  end_time = omp_get_wtime();
  elapsed = end_time - start_time;
  FILE* qSort_stats = fopen("qSort_time.txt", "a");
  fprintf(qSort_stats, "%.6fs %d %d %d\n", elapsed, n, chunk_size, num_threads);
  fclose(qSort_stats);


  free(mas);

  return 0;
}
