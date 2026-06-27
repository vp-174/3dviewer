#include <QTextStream>
#include <cmath>

#include "../controller/controller.h"

#define STEP_MOVE 5

const QString testFileName = "./models_3d/cube.obj";
const QString nonExistentFile = "./models_3d/non_existent.obj";
const QString invalidFileFormat = "./models_3d/invalid_format.txt";

// Тест: Инициализация параметров
TEST(FacadeTest, ResetParamTest) {
  vp::Facade facade;
  facade.resetParam();
  vp::Parameters *params = facade.getParam();

  EXPECT_FLOAT_EQ(params->scale, 1.0f);
  EXPECT_FLOAT_EQ(params->shift_x, 0.0f);
  EXPECT_FLOAT_EQ(params->shift_y, 0.0f);
  EXPECT_FLOAT_EQ(params->shift_z, 0.0f);
  EXPECT_FLOAT_EQ(params->rotate_x, 0.0f);
  EXPECT_FLOAT_EQ(params->rotate_y, 0.0f);
  EXPECT_FLOAT_EQ(params->rotate_z, 0.0f);

  EXPECT_EQ(params->type_projection, vp::ProjectionType::Central);
}

// Тест: Загрузка корректного файла
TEST(FacadeTest, LoadFileTest) {
  vp::Facade facade;

  ASSERT_NO_THROW(facade.loadFile(testFileName));

  EXPECT_EQ(facade.getParam()->filename, testFileName);
  EXPECT_GT(facade.getSizeVertices(), 0u);
  EXPECT_GT(facade.getSizeFacets(), 0u);
}

// Тест: Перемещение фигуры
TEST(FacadeTest, MoveFigureTest) {
  vp::Facade facade;
  facade.resetParam();

  float move_x = 10.0f, move_y = 20.0f, move_z = 30.0f;
  facade.moveFigure(move_x, move_y, move_z);

  vp::Parameters *params = facade.getParam();
  EXPECT_FLOAT_EQ(params->shift_x, move_x);
  EXPECT_FLOAT_EQ(params->shift_y, move_y);
  EXPECT_FLOAT_EQ(params->shift_z, move_z);
}

// Тест: Проверка матрицы трансформации после перемещения
TEST(FacadeTest, MoveFigureTest2) {
  vp::Facade facade;
  facade.resetParam();
  facade.loadFile(testFileName);

  float move_x = 2.0f, move_y = 3.0f, move_z = -7.0f;

  // Копируем матрицу ДО операции (getTotalTransform возвращает const&)
  vp::TransformMatrix transform_before = facade.getTotalTransform();
  facade.moveFigure(move_x, move_y, move_z);
  const auto &transform_after = facade.getTotalTransform();

  // Матрица не должна быть единичной (draw transform изменился)
  bool changed = false;
  for (int r = 0; r < 4 && !changed; r++)
    for (int c = 0; c < 4 && !changed; c++)
      if (std::abs(transform_after(r, c) - transform_before(r, c)) > 1e-6f)
        changed = true;
  EXPECT_TRUE(changed);

  // Вершины должны остаться неизменными (трансформация накапливается в матрице)
  EXPECT_EQ(facade.getFigure()->getVertices().size(), 8u);

  vp::Parameters *params = facade.getParam();
  EXPECT_FLOAT_EQ(params->shift_x, move_x);
  EXPECT_FLOAT_EQ(params->shift_y, move_y);
  EXPECT_FLOAT_EQ(params->shift_z, move_z);
}

// Тест: Вращение фигуры
TEST(FacadeTest, RotateFigureTest) {
  vp::Facade facade;
  facade.resetParam();

  float rotate_x = 45.0f, rotate_y = 90.0f, rotate_z = 180.0f;
  facade.rotateFigure(rotate_x, rotate_y, rotate_z);

  vp::Parameters *params = facade.getParam();
  EXPECT_FLOAT_EQ(params->rotate_x, rotate_x);
  EXPECT_FLOAT_EQ(params->rotate_y, rotate_y);
  EXPECT_FLOAT_EQ(params->rotate_z, rotate_z);
}

// Тест: Проверка матрицы трансформации после вращения
TEST(FacadeTest, RotateFigureTest2) {
  vp::Facade facade;
  facade.resetParam();
  facade.loadFile(testFileName);

  float rotate_x = 45.0f, rotate_y = 90.0f, rotate_z = 30.0f;

  vp::TransformMatrix transform_before = facade.getTotalTransform();
  facade.rotateFigure(rotate_x, rotate_y, rotate_z);
  const auto &transform_after = facade.getTotalTransform();

  // Матрица не должна быть единичной
  bool changed = false;
  for (int r = 0; r < 4 && !changed; r++)
    for (int c = 0; c < 4 && !changed; c++)
      if (std::abs(transform_after(r, c) - transform_before(r, c)) > 1e-6f)
        changed = true;
  EXPECT_TRUE(changed);

  // Вершины должны остаться неизменными
  EXPECT_EQ(facade.getFigure()->getVertices().size(), 8u);

  vp::Parameters *params = facade.getParam();
  EXPECT_FLOAT_EQ(params->rotate_x, rotate_x);
  EXPECT_FLOAT_EQ(params->rotate_y, rotate_y);
  EXPECT_FLOAT_EQ(params->rotate_z, rotate_z);
}

// Тест: Масштабирование фигуры
TEST(FacadeTest, ScaleFigureTest) {
  vp::Facade facade;
  facade.resetParam();

  float scale = 2.0f;
  facade.scaleFigure(scale, scale, scale);

  vp::Parameters *params = facade.getParam();
  EXPECT_FLOAT_EQ(params->scale, scale);
}

// Тест: Проверка матрицы трансформации после масштабирования
TEST(FacadeTest, ScaleFigureTest2) {
  vp::Facade facade;
  facade.resetParam();
  facade.loadFile(testFileName);

  float scale = 2.0f;

  vp::TransformMatrix transform_before = facade.getTotalTransform();
  facade.scaleFigure(scale, scale, scale);
  const auto &transform_after = facade.getTotalTransform();

  // Матрица не должна быть единичной
  bool changed = false;
  for (int r = 0; r < 4 && !changed; r++)
    for (int c = 0; c < 4 && !changed; c++)
      if (std::abs(transform_after(r, c) - transform_before(r, c)) > 1e-6f)
        changed = true;
  EXPECT_TRUE(changed);

  // Вершины должны остаться неизменными
  EXPECT_EQ(facade.getFigure()->getVertices().size(), 8u);

  vp::Parameters *params = facade.getParam();
  EXPECT_FLOAT_EQ(params->scale, scale);
}

// Тест: Получение фигуры
TEST(FacadeTest, GetFigureTest) {
  vp::Facade facade;
  facade.resetParam();

  EXPECT_NE(facade.getFigure(), nullptr);
}

// Тест: Размеры вершин и граней
TEST(FacadeTest, GetSizeVerticesAndFacetsTest) {
  vp::Facade facade;

  ASSERT_NO_THROW(facade.loadFile(testFileName));

  EXPECT_GT(facade.getSizeVertices(), 0u);
  EXPECT_GT(facade.getSizeFacets(), 0u);
}

// Тест: Обработка загрузки несуществующего файла
TEST(FacadeTest, LoadNonExistentFileTest) {
  vp::Facade facade;
  try {
    facade.loadFile(nonExistentFile);
    FAIL() << "Expected std::runtime_error";
  } catch (const std::runtime_error &e) {
    EXPECT_STREQ(e.what(), "Cannot open file: ./models_3d/non_existent.obj");
  } catch (...) {
    FAIL() << "Expected std::runtime_error";
  }
}

// Тест: Обработка файла с неправильным форматом
TEST(FacadeTest, LoadInvalidFileFormatTest) {
  vp::Facade facade;
  QFile file(invalidFileFormat);
  if (file.open(QIODevice::WriteOnly | QIODevice::Truncate)) {
    QTextStream out(&file);
    out << "This is an invalid format file.\n";
    out << "It doesn't follow the .obj file structure.\n";
    file.close();
  } else {
    FAIL() << "Could not create invalid vertex file for testing.";
  }
  facade.loadFile(invalidFileFormat);

  EXPECT_EQ(facade.getSizeVertices(), 0u);
  EXPECT_EQ(facade.getSizeFacets(), 0u);
  QFile::remove(invalidFileFormat);
}

// Тест: Изменение типа проекции
TEST(FacadeTest, ChangeProjectionTypeTest) {
  vp::Facade facade;
  facade.resetParam();

  EXPECT_EQ(facade.getParam()->type_projection, vp::ProjectionType::Central);

  facade.getParam()->type_projection = vp::ProjectionType::Parallel;
  EXPECT_EQ(facade.getParam()->type_projection, vp::ProjectionType::Parallel);

  facade.getParam()->type_projection = vp::ProjectionType::Central;
  EXPECT_EQ(facade.getParam()->type_projection, vp::ProjectionType::Central);
}

// Тест: Обработка пустого файла
TEST(FacadeTest, LoadEmptyFileTest) {
  vp::Facade facade;
  QString emptyFileName = "./models_3d/empty.obj";
  QFile file(emptyFileName);
  if (file.open(QIODevice::WriteOnly | QIODevice::Truncate)) {
    file.close();
  } else {
    FAIL() << "Could not create empty file for testing.";
  }

  ASSERT_NO_THROW(facade.loadFile(emptyFileName));
  EXPECT_EQ(facade.getSizeVertices(), 0u);
  EXPECT_EQ(facade.getSizeFacets(), 0u);
  QFile::remove(emptyFileName);
}

// Тест: Перемещение с нулевыми значениями
TEST(FacadeTest, MoveFigureWithZeroValues) {
  vp::Facade facade;
  facade.resetParam();

  facade.moveFigure(0.0f, 0.0f, 0.0f);

  vp::Parameters *params = facade.getParam();
  EXPECT_FLOAT_EQ(params->shift_x, 0.0f);
  EXPECT_FLOAT_EQ(params->shift_y, 0.0f);
  EXPECT_FLOAT_EQ(params->shift_z, 0.0f);
}

// Тест: Загрузка файла с большим количеством вершин и граней
TEST(FacadeTest, LoadLargeFileTest) {
  vp::Facade facade;
  QString largeFileName = "./models_3d/m_2.obj";

  ASSERT_NO_THROW(facade.loadFile(largeFileName));

  EXPECT_GT(facade.getSizeVertices(), 1000u);
  EXPECT_GT(facade.getSizeFacets(), 1000u);
}

// Тест: Загрузка файла с некорректными данными (отсутствующие вершины или
// грани)
TEST(FacadeTest, LoadFileWithMissingDataTest) {
  vp::Facade facade;
  QString invalidDataFile = "./models_3d/empty.obj";
  QFile file(invalidDataFile);
  if (file.open(QIODevice::WriteOnly | QIODevice::Truncate)) {
    file.close();
  } else {
    FAIL() << "Could not create empty file for testing.";
  }
  ASSERT_NO_THROW(facade.loadFile(invalidDataFile));

  EXPECT_EQ(facade.getSizeVertices(), 0u);
  EXPECT_EQ(facade.getSizeFacets(), 0u);
  QFile::remove(invalidDataFile);
}