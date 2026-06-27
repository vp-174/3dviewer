#pragma once

#include "matrix/matrix_oop.h"

namespace vp {

class Vertex;

/**
 * @class TransformMatrix
 * @brief Класс матрицы преобразования.
 */
class TransformMatrix {
 private:
  Matrix _matrix;  // матрица преобразования

 public:
  TransformMatrix()
      : _matrix(Matrix(4, 4)) {}  // конструктор с инициализацией матрицы 4х4

  TransformMatrix(const TransformMatrix &m);  // конструктор копирования

  ~TransformMatrix() = default;  // деструктор по умолчанию

  TransformMatrix operator*(
      const TransformMatrix &m);  // оператор умножения матриц трансформирования

  TransformMatrix &operator=(
      const TransformMatrix &m);  // оператор присваивания копированием

  double &operator()(int row,
                     int col) const;  // оператор доступа к элементу матрицы

  void transformPoint(
      Vertex &v) const;  // метод применения преобразования к вершине

  void InverseTransformMatrix();  // метод нахождения обратной матрицы

  void toFloatArray(float arr[16]) const;  // выгрузка в GL-совместимый массив
  void setIdentity();                      // установить единичную матрицу
};

}  // namespace vp
