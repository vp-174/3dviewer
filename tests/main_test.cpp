#include <gtest/gtest.h>

#include <chrono>

#include "test_controller.cpp"
#include "test_figure.cpp"
#include "test_parser.cpp"
#include "test_parser_settings.cpp"
#include "test_matrix_oop.cpp"
#include "test_transform_figure.cpp"
#include "test_transform_matrix.cpp"
#include "test_transform_matrix_builder.cpp"

int main() {
  // Измерение времени
  clock_t start_time, end_time;
  double cpu_time_used;

  start_time = clock();  // Начало измерения времени

  ::testing::InitGoogleTest();

  int run_tests = RUN_ALL_TESTS();

  end_time = clock();  // Конец измерения времени
  cpu_time_used = ((double)(end_time - start_time)) / CLOCKS_PER_SEC;

  printf("\n\e[30m Time: %f sec\e[0m\n\n", cpu_time_used);

  return run_tests;
}
