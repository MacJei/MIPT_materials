#include <stdio.h>
#include <stdlib.h>
#include <time.h>
#include <omp.h>

int my_rand();

int main(int argc, char* argv[]) {

  double start_time = omp_get_wtime();
  double end_time, elapsed_time;
  int a, b, x, N, P;
  int count_a = 0, count_b = 0;
  double p;
  double total_life_time = 0, avg_life_time;

  srand(time(NULL));

  if (argc > 5) {
    a = atoi(argv[1]);
    b = atoi(argv[2]);
    x = atoi(argv[3]);
    N = atoi(argv[4]);
    p = atof(argv[5]) * 100;
    P = atoi(argv[6]);
  }
  else {
    printf("too few argumets");
    return 0;
  }
  omp_set_num_threads(P);


#pragma omp parallel for
  for (int i = 0; i < N; ++i) {
    double start_particle_time = omp_get_wtime();
    int cur_x = x;
    while(cur_x != a && cur_x != b) {
      int chance = my_rand() % 100;
      if (chance < p) {
        cur_x++;
      }
      else {
        cur_x--;
      }
    }
    double end_particle_time = omp_get_wtime();
    double life_time = end_particle_time - start_particle_time;
    total_life_time += life_time;
    if (cur_x == a) {
#pragma omp atomic update
      count_a++;
    }
    if (cur_x == b) {
#pragma omp atomic update
      count_b++;
    }
  }

  avg_life_time = total_life_time / N;
  end_time = omp_get_wtime();
  elapsed_time = end_time - start_time;

  FILE* fp = fopen("stats2.txt", "a");
  fprintf(fp, "%.4f %.4f %.6f %d %d %d %d %.3f %d\n", (float)count_b / N, elapsed_time, avg_life_time, a, b, x, N, (float)p / 100, P);
  fclose(fp);

  return 0;
}

int my_rand() {
  static __thread unsigned int seed = 0;
  if (seed == 0) {
    seed = rand();
  }
  return rand_r(&seed);
}
