#include <stdio.h>
#include <stdlib.h>
#include <time.h>
#include <omp.h>
#include <unistd.h>
#include "Merge_sort_omp.h"

int main(int argc, char* argv[]) {
  if (argc != 4) {
    printf("Wrong number of arguments\n");
    exit(-1);
  }
  int n = atoi(argv[1]);
  int chunk_size = atoi(argv[2]);
  int num_threads = atoi(argv[3]);

  srand(time(NULL));
  omp_set_num_threads(num_threads);

  FILE* data_file = fopen("data.txt", "w");
  int* mas = (int*)malloc(n * sizeof(int));
  for (int i = 0; i < n; ++i) {
    mas[i] = rand();
    fprintf(data_file, "%d ", mas[i]);
  }
  fprintf(data_file, "\n");

  float start_time = omp_get_wtime();
  MergeSort(mas, 0, n - 1, chunk_size);
  float end_time = omp_get_wtime();
  float elapsed = end_time - start_time;

  for (int i = 0; i < n; ++i) {
    fprintf(data_file, "%d ", mas[i]);
  }
  fprintf(data_file, "\n");
  fclose(data_file);

  FILE* stats_file = fopen("stats.txt", "a");
  fprintf(stats_file, "%.6fs %d %d %d\n", elapsed, n, chunk_size, num_threads);
  fclose(stats_file);

  free(mas);
  return 0;
}

