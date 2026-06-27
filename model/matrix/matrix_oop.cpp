#include "matrix_oop.h"

#include <cfloat>
#include <cstring>

static bool EqDouble(double x, double y) {
  if (fabs(x - y) > DBL_EPSILON * fmax(fabs(x), fabs(y)) * 4 &&
      fabs(x - y) > 0.000001) {
    return false;
  }
  return true;
}

void Matrix::AllocMem(int rows, int cols) {
  if (rows <= 0 || cols <= 0) {
    throw std::out_of_range("Invalid row or column values");
  }
  this->_rows = rows;
  this->_cols = cols;
  this->matrix_ = new double *[rows];
  for (int i = 0; i < rows; i++) {
    this->matrix_[i] = new double[cols];
    memset(this->matrix_[i], 0, sizeof(double) * cols);
  }
}

void Matrix::FreeMem() {
  if (matrix_) {
    for (int i = 0; i < _rows; i++) {
      delete[] matrix_[i];
    }
    delete[] matrix_;
    matrix_ = nullptr;
  }
}

bool Matrix::EqSizeMatrix(const Matrix &other) const {
  return _rows == other._rows && _cols == other._cols;
}

Matrix Matrix::MinorMatrix(int row_i, int column_j) {
  Matrix minor;
  if (_rows == 1 && _cols == 1) {
    minor = Matrix(1, 1);
  } else {
    minor = Matrix(_rows - 1, _cols - 1);
  }
  int n = 0;
  for (int i = 0; i < _rows; i++) {
    if (i != row_i) {
      int k = 0;
      for (int j = 0; j < _cols; j++) {
        if (j != column_j) {
          minor.matrix_[n][k] = matrix_[i][j];
          k++;
        }
      }
      n++;
    }
  }
  return minor;
}

bool Matrix::IsSquareMatrix() { return _rows == _cols; }

/*базовый конструктор, инициализирующий матрицу некоторой заранее заданной
 * размерностью*/
Matrix::Matrix() {
  _rows = 0;
  _cols = 0;
  matrix_ = nullptr;
}

Matrix::Matrix(int rows, int cols) { AllocMem(rows, cols); }

Matrix::Matrix(const Matrix &other) {
  AllocMem(other._rows, other._cols);

  for (int i = 0; i < _rows; i++) {
    for (int j = 0; j < _cols; j++) {
      matrix_[i][j] = other.matrix_[i][j];
    }
  }
}

Matrix::Matrix(Matrix &&other)
    : _rows(other._rows), _cols(other._cols) {
  matrix_ = other.matrix_;
  other._rows = 0;
  other._cols = 0;
  other.matrix_ = nullptr;
}

Matrix::~Matrix() { FreeMem(); }

bool Matrix::EqMatrix(const Matrix &other) const {
  if (!EqSizeMatrix(other)) {
    return false;
  }
  for (int i = 0; i < _rows; i++) {
    for (int j = 0; j < _cols; j++) {
      if (!EqDouble(matrix_[i][j], other.matrix_[i][j])) {
        return false;
      }
    }
  }
  return true;
}

void Matrix::SumMatrix(const Matrix &other) {
  if (!EqSizeMatrix(other)) {
    throw std::invalid_argument(
        "Incorrect input, matrices should have the same size");
  }
  for (int i = 0; i < _rows; i++) {
    for (int j = 0; j < _cols; j++) {
      matrix_[i][j] += other.matrix_[i][j];
    }
  }
}

void Matrix::SubMatrix(const Matrix &other) {
  if (!EqSizeMatrix(other)) {
    throw std::invalid_argument(
        "Incorrect input, matrices should have the same size");
  }
  for (int i = 0; i < _rows; i++) {
    for (int j = 0; j < _cols; j++) {
      matrix_[i][j] -= other.matrix_[i][j];
    }
  }
}

void Matrix::MulNumber(const double num) {
  for (int i = 0; i < _rows; i++) {
    for (int j = 0; j < _cols; j++) {
      matrix_[i][j] *= num;
    }
  }
}

void Matrix::MulMatrix(const Matrix &other) {
  if (_cols != other._rows) {
    throw std::invalid_argument(
        "Incorrect input, the number of columns of the first matrix must be "
        "equal to the number of rows of the second matrix");
  }
  Matrix mul(_rows, other._cols);
  for (int i = 0; i < _rows; i++) {
    for (int j = 0; j < other._cols; j++) {
      for (int n = 0; n < _cols; n++) {
        mul.matrix_[i][j] += matrix_[i][n] * other.matrix_[n][j];
      }
    }
  }
  *this = std::move(mul);
}

Matrix Matrix::Transpose() {
  Matrix tran_mat(_cols, _rows);
  for (int i = 0; i < _rows; i++) {
    for (int j = 0; j < _cols; j++) {
      tran_mat.matrix_[j][i] = matrix_[i][j];
    }
  }
  return tran_mat;
}

Matrix Matrix::CalcComplements() {
  if (!IsSquareMatrix()) {
    throw std::invalid_argument("Incorrect input, the matrix must be square");
  }
  Matrix calc_com(_rows, _cols);
  for (int i = 0; i < _rows; i++) {
    for (int j = 0; j < _cols; j++) {
      calc_com.matrix_[i][j] = pow(-1, i + j) * MinorMatrix(i, j).Determinant();
    }
  }
  return calc_com;
}

double Matrix::Determinant() {
  if (!IsSquareMatrix()) {
    throw std::invalid_argument("Incorrect input, the matrix must be square");
  }
  if (_rows == 1) {
    return matrix_[0][0];
  }
  if (_rows == 2) {
    return matrix_[0][0] * matrix_[1][1] - matrix_[0][1] * matrix_[1][0];
  }
  double det = 0;
  for (int j = 0; j < _cols; j++) {
    det +=
        (j % 2 == 0 ? 1 : -1) * matrix_[0][j] * MinorMatrix(0, j).Determinant();
  }
  return det;
}

Matrix Matrix::InverseMatrix() {
  if (!IsSquareMatrix()) {
    throw std::invalid_argument("Incorrect input, the matrix must be square");
  }
  double det = Determinant();
  Matrix inv_mat(_rows, _cols);
  if (det == 0) {
    throw std::out_of_range(
        "For a matrix with a determinant equal to zero, there is no inverse "
        "matrix");
  }
  Matrix tmp = CalcComplements().Transpose();
  tmp.MulNumber(1 / det);
  for (int i = 0; i < _rows; i++) {
    for (int j = 0; j < _cols; j++) {
      inv_mat.matrix_[i][j] = tmp.matrix_[i][j];
    }
  }
  return inv_mat;
}

Matrix Matrix::operator+(const Matrix &other) {
  Matrix sum = *this;
  sum.SumMatrix(other);
  return sum;
}

Matrix Matrix::operator-(const Matrix &other) {
  Matrix sub = *this;
  sub.SubMatrix(other);
  return sub;
}

Matrix Matrix::operator*(const Matrix &other) {
  Matrix mul = *this;
  mul.MulMatrix(other);
  return mul;
}

Matrix operator*(const double num, const Matrix &other) {
  Matrix mul = other;
  mul.MulNumber(num);
  return mul;
}

Matrix operator*(const Matrix &other, const double num) {
  Matrix mul = other;
  mul.MulNumber(num);
  return mul;
}

bool Matrix::operator==(const Matrix &other) const { return EqMatrix(other); }

Matrix &Matrix::operator=(const Matrix &other) {
  FreeMem();
  AllocMem(other._rows, other._cols);
  for (int i = 0; i < _rows; i++) {
    for (int j = 0; j < _cols; j++) {
      matrix_[i][j] = other.matrix_[i][j];
    }
  }
  return *this;
}

Matrix &Matrix::operator=(Matrix &&other) {
  FreeMem();
  _rows = other._rows;
  _cols = other._cols;
  matrix_ = other.matrix_;
  other._rows = 0;
  other._cols = 0;
  other.matrix_ = nullptr;
  return *this;
}

Matrix &Matrix::operator+=(const Matrix &other) {
  SumMatrix(other);
  return *this;
}

Matrix &Matrix::operator-=(const Matrix &other) {
  SubMatrix(other);
  return *this;
}

Matrix &Matrix::operator*=(const Matrix &other) {
  MulMatrix(other);
  return *this;
}

Matrix &Matrix::operator*=(const double num) {
  MulNumber(num);
  return *this;
}

double &Matrix::operator()(int row, int col) const {
  if (row < 0 || col < 0 || row >= _rows || col >= _cols) {
    throw std::out_of_range("The index is outside the matrix");
  }
  return matrix_[row][col];
}

int Matrix::AccessorRow() const { return _rows; }

int Matrix::AccessorCol() const { return _cols; }

void Matrix::MutatorRow(const int rows) { SetNewNatrix(rows, _cols); }
void Matrix::MutatorCol(const int cols) { SetNewNatrix(_rows, cols); }

void Matrix::SetNewNatrix(const int NewRow, const int NewCol) {
  Matrix newMatrix(NewRow, NewCol);
  for (int i = 0; i < std::min(NewRow, _rows); i++) {
    for (int j = 0; j < std::min(NewCol, _cols); j++) {
      newMatrix(i, j) = matrix_[i][j];
    }
  }
  FreeMem();
  *this = std::move(newMatrix);
}