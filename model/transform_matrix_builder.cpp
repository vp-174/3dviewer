#include "transform_matrix_builder.h"

using namespace vp;

TransformMatrix TransformMatrixBuilder::createRotateMatrix(float a, float b,
                                                           float c) {
  TransformMatrix matrix;

  matrix(0, 0) = cos(b) * cos(c);
  matrix(0, 1) = -sin(c) * cos(b);
  matrix(0, 2) = sin(b);
  matrix(0, 3) = 0.0f;
  matrix(1, 0) = sin(a) * sin(b) * cos(c) + sin(c) * cos(a);
  matrix(1, 1) = -sin(a) * sin(b) * sin(c) + cos(a) * cos(c);
  matrix(1, 2) = -sin(a) * cos(b);
  matrix(1, 3) = 0.0f;
  matrix(2, 0) = sin(a) * sin(c) - sin(b) * cos(a) * cos(c);
  matrix(2, 1) = sin(a) * cos(c) + sin(b) * sin(c) * cos(a);
  matrix(2, 2) = cos(a) * cos(b);
  matrix(2, 3) = 0.0f;
  matrix(3, 0) = 0.0f;
  matrix(3, 1) = 0.0f;
  matrix(3, 2) = 0.0f;
  matrix(3, 3) = 1.0f;

  return matrix;
}

TransformMatrix TransformMatrixBuilder::createMoveMatrix(float a, float b,
                                                         float c) {
  TransformMatrix matrix;

  matrix(0, 0) = 1.0f;
  matrix(0, 1) = 0.0f;
  matrix(0, 2) = 0.0f;
  matrix(0, 3) = 0.0f;
  matrix(1, 0) = 0.0f;
  matrix(1, 1) = 1.0f;
  matrix(1, 2) = 0.0f;
  matrix(1, 3) = 0.0f;
  matrix(2, 0) = 0.0f;
  matrix(2, 1) = 0.0f;
  matrix(2, 2) = 1.0f;
  matrix(2, 3) = 0.0f;
  matrix(3, 0) = a;
  matrix(3, 1) = b;
  matrix(3, 2) = c;
  matrix(3, 3) = 1.0f;

  return matrix;
}

TransformMatrix TransformMatrixBuilder::createScaleMatrix(float a, float b,
                                                          float c) {
  TransformMatrix matrix;

  matrix(0, 0) = a;
  matrix(0, 1) = 0.0f;
  matrix(0, 2) = 0.0f;
  matrix(0, 3) = 0.0f;
  matrix(1, 0) = 0.0f;
  matrix(1, 1) = b;
  matrix(1, 2) = 0.0f;
  matrix(1, 3) = 0.0f;
  matrix(2, 0) = 0.0f;
  matrix(2, 1) = 0.0f;
  matrix(2, 2) = c;
  matrix(2, 3) = 0.0f;
  matrix(3, 0) = 0.0f;
  matrix(3, 1) = 0.0f;
  matrix(3, 2) = 0.0f;
  matrix(3, 3) = 1.0f;

  return matrix;
}