#pragma once

#include "transform_matrix.h"

namespace vp {

/**
 * @class TransformMatrixBuilder
 * @brief Класс для создания матриц поворота, перемещения и масштабирования.
 */
class TransformMatrixBuilder {
 private:
  TransformMatrixBuilder(){};  // конструктор по умолчанию

  ~TransformMatrixBuilder() = default;  // деструктор по умолчанию

 public:
  static TransformMatrix createRotateMatrix(
      float a, float b, float c);  // создание матрицы поворота (OX, OY, OZ)

  static TransformMatrix createMoveMatrix(
      float a, float b, float c);  // создание матрицы перемещения (OX, OY, OZ)

  static TransformMatrix createScaleMatrix(
      float a, float b,
      float c);  // создание матрицы масштабирования (OX, OY, OZ)
};

}  // namespace vp
