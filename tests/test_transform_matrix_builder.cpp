#include <QString>

#include "../model/transform_matrix_builder.h"

class TransformMatrixBuilderTest : public ::testing::Test {
 protected:
  void SetUp() override { testFileName = "./models_3d/cube.obj"; }

  QString testFileName;
};

// Тест на создание матрицы вращения вокруг оси Z
TEST_F(TransformMatrixBuilderTest, CreateRotateMatrix) {
  float a = 0.0f, b = 0.0f, c = 3.14159265359f;

  vp::TransformMatrix result =
      vp::TransformMatrixBuilder::createRotateMatrix(a, b, c);

  EXPECT_FLOAT_EQ(result(0, 0), -1.0f);
  EXPECT_FLOAT_EQ(result(1, 1), -1.0f);
  EXPECT_FLOAT_EQ(result(2, 2), 1.0f);
}

// Тест на создание матрицы перемещения
TEST_F(TransformMatrixBuilderTest, CreateMoveMatrix) {
  float a = 1.0f, b = 2.0f, c = 3.0f;

  vp::TransformMatrix result =
      vp::TransformMatrixBuilder::createMoveMatrix(a, b, c);

  EXPECT_FLOAT_EQ(result(3, 0), 1.0f);
  EXPECT_FLOAT_EQ(result(3, 1), 2.0f);
  EXPECT_FLOAT_EQ(result(3, 2), 3.0f);
}

// Тест на создание матрицы масштабирования
TEST_F(TransformMatrixBuilderTest, CreateScaleMatrix) {
  float a = 2.0f, b = 3.0f, c = 4.0f;

  vp::TransformMatrix result =
      vp::TransformMatrixBuilder::createScaleMatrix(a, b, c);

  EXPECT_FLOAT_EQ(result(0, 0), 2.0f);
  EXPECT_FLOAT_EQ(result(1, 1), 3.0f);
  EXPECT_FLOAT_EQ(result(2, 2), 4.0f);
}

// Тест: Создание матрицы масштабирования с нулевыми значениями
TEST_F(TransformMatrixBuilderTest, CreateScaleMatrixWithZeroValues) {
  float a = 0.0f, b = 0.0f, c = 0.0f;

  vp::TransformMatrix result =
      vp::TransformMatrixBuilder::createScaleMatrix(a, b, c);

  EXPECT_FLOAT_EQ(result(0, 0), 0.0f);
  EXPECT_FLOAT_EQ(result(1, 1), 0.0f);
  EXPECT_FLOAT_EQ(result(2, 2), 0.0f);
}