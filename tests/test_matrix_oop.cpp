#include <gtest/gtest.h>

#include "../model/matrix/matrix_oop.h"

// Тест конструктора по умолчанию
TEST(MatrixTest, DefaultConstructor) {
  Matrix matrix;
  EXPECT_EQ(matrix.AccessorRow(), 0);
  EXPECT_EQ(matrix.AccessorCol(), 0);
}

// Тест конструктора с параметрами
TEST(MatrixTest, ParameterizedConstructor) {
  Matrix matrix(3, 4);
  EXPECT_EQ(matrix.AccessorRow(), 3);
  EXPECT_EQ(matrix.AccessorCol(), 4);
}

// Тест конструктора копирования
TEST(MatrixTest, CopyConstructor) {
  Matrix matrix(2, 2);
  matrix(0, 0) = 1.0;
  matrix(1, 1) = 2.0;
  Matrix copy(matrix);
  EXPECT_TRUE(copy == matrix);
}

// Тест конструктора перемещения
TEST(MatrixTest, MoveConstructor) {
  Matrix matrix(2, 2);
  matrix(0, 0) = 1.0;
  matrix(1, 1) = 2.0;
  Matrix moved(std::move(matrix));
  EXPECT_EQ(matrix.AccessorRow(), 0);
  EXPECT_EQ(matrix.AccessorCol(), 0);
  EXPECT_EQ(moved(0, 0), 1.0);
  EXPECT_EQ(moved(1, 1), 2.0);
}

// Тест оператора сложения
TEST(MatrixTest, OperatorAddition) {
  Matrix matrix1(2, 2);
  Matrix matrix2(2, 2);

  matrix1(0, 0) = 1.0;
  matrix1(0, 1) = 2.0;
  matrix1(1, 0) = 3.0;
  matrix1(1, 1) = 4.0;

  matrix2(0, 0) = 5.0;
  matrix2(0, 1) = 6.0;
  matrix2(1, 0) = 7.0;
  matrix2(1, 1) = 8.0;

  Matrix result = matrix1 + matrix2;

  EXPECT_EQ(result(0, 0), 6.0);
  EXPECT_EQ(result(0, 1), 8.0);
  EXPECT_EQ(result(1, 0), 10.0);
  EXPECT_EQ(result(1, 1), 12.0);
}

// Тест оператора вычитания
TEST(MatrixTest, OperatorSubtraction) {
  Matrix matrix1(2, 2);
  Matrix matrix2(2, 2);

  matrix1(0, 0) = 5.0;
  matrix1(0, 1) = 6.0;
  matrix1(1, 0) = 7.0;
  matrix1(1, 1) = 8.0;

  matrix2(0, 0) = 1.0;
  matrix2(0, 1) = 2.0;
  matrix2(1, 0) = 3.0;
  matrix2(1, 1) = 4.0;

  Matrix result = matrix1 - matrix2;

  EXPECT_EQ(result(0, 0), 4.0);
  EXPECT_EQ(result(0, 1), 4.0);
  EXPECT_EQ(result(1, 0), 4.0);
  EXPECT_EQ(result(1, 1), 4.0);
}

// Тест умножения матрицы на число
TEST(MatrixTest, OperatorMultiplicationByScalar) {
  Matrix matrix(2, 2);

  matrix(0, 0) = 1.0;
  matrix(0, 1) = 2.0;
  matrix(1, 0) = 3.0;
  matrix(1, 1) = 4.0;

  Matrix result1 = matrix * 2.0;
  Matrix result2 = 2.0 * matrix;

  EXPECT_EQ(result1(0, 0), 2.0);
  EXPECT_EQ(result1(0, 1), 4.0);
  EXPECT_EQ(result1(1, 0), 6.0);
  EXPECT_EQ(result1(1, 1), 8.0);

  EXPECT_EQ(result2(0, 0), 2.0);
  EXPECT_EQ(result2(0, 1), 4.0);
  EXPECT_EQ(result2(1, 0), 6.0);
  EXPECT_EQ(result2(1, 1), 8.0);
}

// Тест транспонирования матрицы
TEST(MatrixTest, Transpose) {
  Matrix matrix(2, 3);
  matrix(0, 1) = 1.0;
  matrix(1, 2) = 2.0;
  Matrix transposed = matrix.Transpose();
  EXPECT_EQ(transposed.AccessorRow(), 3);
  EXPECT_EQ(transposed.AccessorCol(), 2);
  EXPECT_EQ(transposed(1, 0), 1.0);
  EXPECT_EQ(transposed(2, 1), 2.0);
}

// Тест детерминанта
TEST(MatrixTest, Determinant) {
  Matrix matrix(2, 2);
  matrix(0, 0) = 1.0;
  matrix(0, 1) = 2.0;
  matrix(1, 0) = 3.0;
  matrix(1, 1) = 4.0;
  EXPECT_EQ(matrix.Determinant(), -2.0);
}

// Тест вычисления обратной матрицы
TEST(MatrixTest, InverseMatrix) {
  Matrix matrix(2, 2);
  matrix(0, 0) = 4.0;
  matrix(0, 1) = 7.0;
  matrix(1, 0) = 2.0;
  matrix(1, 1) = 6.0;
  Matrix inverse = matrix.InverseMatrix();
  EXPECT_NEAR(inverse(0, 0), 0.6, 1e-6);
  EXPECT_NEAR(inverse(0, 1), -0.7, 1e-6);
  EXPECT_NEAR(inverse(1, 0), -0.2, 1e-6);
  EXPECT_NEAR(inverse(1, 1), 0.4, 1e-6);
}

// Тест матричного умножения
TEST(MatrixTest, MulMatrix) {
  Matrix matrix1(2, 3);
  Matrix matrix2(3, 2);

  matrix1(0, 0) = 1.0;
  matrix1(0, 1) = 2.0;
  matrix1(0, 2) = 3.0;
  matrix1(1, 0) = 4.0;
  matrix1(1, 1) = 5.0;
  matrix1(1, 2) = 6.0;

  matrix2(0, 0) = 7.0;
  matrix2(0, 1) = 8.0;
  matrix2(1, 0) = 9.0;
  matrix2(1, 1) = 10.0;
  matrix2(2, 0) = 11.0;
  matrix2(2, 1) = 12.0;

  matrix1.MulMatrix(matrix2);

  EXPECT_EQ(matrix1.AccessorRow(), 2);
  EXPECT_EQ(matrix1.AccessorCol(), 2);
  EXPECT_EQ(matrix1(0, 0), 58.0);
  EXPECT_EQ(matrix1(0, 1), 64.0);
  EXPECT_EQ(matrix1(1, 0), 139.0);
  EXPECT_EQ(matrix1(1, 1), 154.0);
}

// Тест оператора +=
TEST(MatrixTest, OperatorPlusEquals) {
  Matrix matrix1(2, 2);
  Matrix matrix2(2, 2);

  matrix1(0, 0) = 1.0;
  matrix1(0, 1) = 2.0;
  matrix1(1, 0) = 3.0;
  matrix1(1, 1) = 4.0;

  matrix2(0, 0) = 5.0;
  matrix2(0, 1) = 6.0;
  matrix2(1, 0) = 7.0;
  matrix2(1, 1) = 8.0;

  matrix1 += matrix2;

  EXPECT_EQ(matrix1(0, 0), 6.0);
  EXPECT_EQ(matrix1(0, 1), 8.0);
  EXPECT_EQ(matrix1(1, 0), 10.0);
  EXPECT_EQ(matrix1(1, 1), 12.0);
}

// Тест оператора -=
TEST(MatrixTest, OperatorMinusEquals) {
  Matrix matrix1(2, 2);
  Matrix matrix2(2, 2);

  matrix1(0, 0) = 5.0;
  matrix1(0, 1) = 6.0;
  matrix1(1, 0) = 7.0;
  matrix1(1, 1) = 8.0;

  matrix2(0, 0) = 1.0;
  matrix2(0, 1) = 2.0;
  matrix2(1, 0) = 3.0;
  matrix2(1, 1) = 4.0;

  matrix1 -= matrix2;

  EXPECT_EQ(matrix1(0, 0), 4.0);
  EXPECT_EQ(matrix1(0, 1), 4.0);
  EXPECT_EQ(matrix1(1, 0), 4.0);
  EXPECT_EQ(matrix1(1, 1), 4.0);
}

// Тест оператора *= (умножение на скаляр)
TEST(MatrixTest, OperatorMultiplyEqualsScalar) {
  Matrix matrix(2, 2);

  matrix(0, 0) = 1.0;
  matrix(0, 1) = 2.0;
  matrix(1, 0) = 3.0;
  matrix(1, 1) = 4.0;

  matrix *= 2.0;

  EXPECT_EQ(matrix(0, 0), 2.0);
  EXPECT_EQ(matrix(0, 1), 4.0);
  EXPECT_EQ(matrix(1, 0), 6.0);
  EXPECT_EQ(matrix(1, 1), 8.0);
}

// Тест оператора *= (умножение на матрицу)
TEST(MatrixTest, OperatorMultiplyEqualsMatrix) {
  Matrix matrix1(2, 3);
  Matrix matrix2(3, 2);

  matrix1(0, 0) = 1.0;
  matrix1(0, 1) = 2.0;
  matrix1(0, 2) = 3.0;
  matrix1(1, 0) = 4.0;
  matrix1(1, 1) = 5.0;
  matrix1(1, 2) = 6.0;

  matrix2(0, 0) = 7.0;
  matrix2(0, 1) = 8.0;
  matrix2(1, 0) = 9.0;
  matrix2(1, 1) = 10.0;
  matrix2(2, 0) = 11.0;
  matrix2(2, 1) = 12.0;

  matrix1 *= matrix2;

  EXPECT_EQ(matrix1.AccessorRow(), 2);
  EXPECT_EQ(matrix1.AccessorCol(), 2);
  EXPECT_EQ(matrix1(0, 0), 58.0);
  EXPECT_EQ(matrix1(0, 1), 64.0);
  EXPECT_EQ(matrix1(1, 0), 139.0);
  EXPECT_EQ(matrix1(1, 1), 154.0);
}

// Тест функции MutatorRow
TEST(MatrixTest, MutatorRow) {
  Matrix matrix(2, 2);
  matrix(0, 0) = 1.0;
  matrix(1, 1) = 2.0;

  matrix.MutatorRow(3);

  EXPECT_EQ(matrix.AccessorRow(), 3);
  EXPECT_EQ(matrix.AccessorCol(), 2);
  EXPECT_EQ(matrix(0, 0), 1.0);
  EXPECT_EQ(matrix(1, 1), 2.0);
  EXPECT_EQ(matrix(2, 0), 0.0);
}

// Тест функции MutatorCol
TEST(MatrixTest, MutatorCol) {
  Matrix matrix(2, 2);
  matrix(0, 0) = 1.0;
  matrix(1, 1) = 2.0;

  matrix.MutatorCol(3);

  EXPECT_EQ(matrix.AccessorRow(), 2);
  EXPECT_EQ(matrix.AccessorCol(), 3);
  EXPECT_EQ(matrix(0, 0), 1.0);
  EXPECT_EQ(matrix(1, 1), 2.0);
  EXPECT_EQ(matrix(0, 2), 0.0);
}

// Исключение при сложении матриц разных размеров
TEST(MatrixTest, ExceptionAddition) {
  Matrix matrix1(2, 2);
  Matrix matrix2(3, 3);
  EXPECT_THROW(matrix1 + matrix2, std::invalid_argument);
}

// Исключение при вычислении детерминанта не квадратной матрицы
TEST(MatrixTest, ExceptionDeterminantNonSquare) {
  Matrix matrix(2, 3);
  EXPECT_THROW(matrix.Determinant(), std::invalid_argument);
}

// Исключение при нахождении обратной матрицы с нулевым детерминантом
TEST(MatrixTest, ExceptionInverseMatrixZeroDeterminant) {
  Matrix matrix(2, 2);
  matrix(0, 0) = 1.0;
  matrix(0, 1) = 2.0;
  matrix(1, 0) = 2.0;
  matrix(1, 1) = 4.0;  // det = 0
  EXPECT_THROW(matrix.InverseMatrix(), std::logic_error);
}

// Матрица 1x1
TEST(MatrixTest, SingleElementMatrix) {
  Matrix matrix(1, 1);
  matrix(0, 0) = 42.0;
  EXPECT_EQ(matrix(0, 0), 42.0);
}

// Тест оператора присваивания
TEST(MatrixTest, AssignmentOperator) {
  Matrix mat1(2, 3);
  mat1(0, 0) = 1.0;
  mat1(0, 1) = 2.0;
  mat1(0, 2) = 3.0;
  mat1(1, 0) = 4.0;
  mat1(1, 1) = 5.0;
  mat1(1, 2) = 6.0;

  Matrix mat2(1, 1);
  mat2 = mat1;

  EXPECT_DOUBLE_EQ(mat2(0, 0), 1.0);
  EXPECT_DOUBLE_EQ(mat2(0, 1), 2.0);
  EXPECT_DOUBLE_EQ(mat2(0, 2), 3.0);
  EXPECT_DOUBLE_EQ(mat2(1, 0), 4.0);
  EXPECT_DOUBLE_EQ(mat2(1, 1), 5.0);
  EXPECT_DOUBLE_EQ(mat2(1, 2), 6.0);
}