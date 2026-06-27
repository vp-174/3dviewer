#include <QDebug>
#include <QTextStream>

#include "../model/figure.h"

class FigureTest : public ::testing::Test {
 protected:
  void SetUp() override { testFileName = "./models_3d/cube.obj"; }

  QString testFileName;
};

// Тест конструктора класса Figure
TEST_F(FigureTest, Constructor) {
  vp::Figure figure(testFileName);
  ASSERT_EQ(figure.getVertices().size(), 8);  // Проверка количества вершин
  ASSERT_EQ(figure.getFacets().size(), 18);  // Проверка количества граней
}

// Тест конструктора копирования класса Figure
TEST_F(FigureTest, CopyConstructor) {
  vp::Figure figure(testFileName);
  vp::Figure copiedFigure = figure;
  ASSERT_EQ(copiedFigure.getVertices().size(), figure.getVertices().size());
  ASSERT_EQ(copiedFigure.getFacets().size(), figure.getFacets().size());
}

// Тест оператора присваивания класса Figure
TEST_F(FigureTest, AssignmentOperator) {
  vp::Figure figure(testFileName);
  vp::Figure assignedFigure(testFileName);
  assignedFigure = figure;

  ASSERT_EQ(assignedFigure.getVertices().size(), figure.getVertices().size());
  ASSERT_EQ(assignedFigure.getFacets().size(), figure.getFacets().size());
}

// Тест метода transform, который выполняет преобразование вершин
TEST_F(FigureTest, Transform) {
  vp::Figure figure(testFileName);
  int originalVertexCount = figure.getVertices().size();

  vp::TransformMatrix transformMatrix;
  figure.transform(transformMatrix);

  ASSERT_EQ(figure.getVertices().size(), originalVertexCount);
}

// Тест: Конструктор корректно обрабатывает несуществующий файл
TEST_F(FigureTest, HandlesInvalidFileGracefully) {
  QString invalidFileName = "./models_3d/invalid.obj";

  try {
    vp::Figure figure(invalidFileName);
    EXPECT_EQ(figure.getVertices().size(), 0u);
    EXPECT_EQ(figure.getFacets().size(), 0u);
    FAIL() << "Expected std::runtime_error";
  } catch (const std::runtime_error &e) {
    EXPECT_STREQ(e.what(), "Cannot open file: ./models_3d/invalid.obj");
  } catch (...) {
    FAIL() << "Expected std::runtime_error, but caught a different exception";
  }
}

// Тест: Нормализация вершин при создании объекта Figure
TEST_F(FigureTest, Normalization) {
  vp::Figure figure(testFileName);

  bool normalizationApplied = false;
  for (const auto &vertex : figure.getVertices()) {
    if (vertex.x() != 0.0 || vertex.y() != 0.0 || vertex.z() != 0.0) {
      normalizationApplied = true;
      break;
    }
  }

  ASSERT_TRUE(normalizationApplied);
}

// Тест: Проверка методов getVertices и getFacets
TEST_F(FigureTest, GetVerticesAndFacets) {
  vp::Figure figure(testFileName);

  ASSERT_EQ(figure.getVertices().size(), 8);
  ASSERT_EQ(figure.getFacets().size(), 18);

  const auto &vertices = figure.getVertices();
  float epsilon = 0.000001f;
  EXPECT_NEAR(vertices[0].x(), 1.0, epsilon);
  EXPECT_NEAR(vertices[0].y(), -1.0, epsilon);
  EXPECT_NEAR(vertices[0].z(), -1.0, epsilon);

  const auto &facets = figure.getFacets();
  EXPECT_NEAR(facets[0].getBeginPosition()->x(), 1.0, epsilon);
  EXPECT_NEAR(facets[0].getEndPosition()->y(), -1.0, epsilon);
}

// Тест: Обработка пустых данных
TEST_F(FigureTest, HandlesEmptyDataGracefully) {
  QString emptyFileName = "./models_3d/empty.obj";
  QFile file(emptyFileName);
  if (file.open(QIODevice::WriteOnly | QIODevice::Truncate)) {
    file.close();
  } else {
    FAIL() << "Could not create empty file for testing.";
  }
  EXPECT_NO_THROW({
    vp::Figure figure(emptyFileName);
    EXPECT_EQ(figure.getVertices().size(), 0u);
    EXPECT_EQ(figure.getFacets().size(), 0u);
  });
  QFile::remove(emptyFileName);
}

// Тест: Загрузка файла с дублирующимися гранями
TEST_F(FigureTest, LoadFileWithDuplicatesTest) {
  QString duplicatesFile = "./models_3d/duplicates.obj";
  QFile file(duplicatesFile);
  if (file.open(QIODevice::WriteOnly | QIODevice::Truncate)) {
    QTextStream out(&file);
    out << "v 1.000000 -1.000000 -1.000000\n";
    out << "v 1.000000 -1.000000 1.000000\n";
    out << "v -1.000000 -1.000000 1.000000\n";
    out << "v -1.000000 -1.000000 -1.000000\n";
    out << "v 1.000000 1.000000 -0.999999\n";
    out << "v 0.999999 1.000000 1.000001\n";
    out << "v -1.000000 1.000000 1.000000\n";
    out << "v -1.000000 1.000000 -1.000000\n";
    out << "f 2/1/1 3/2/1 4/3/1\n";
    out << "f 8/1/2 7/4/2 6/5/2\n";
    out << "f 5/6/3 6/7/3 2/8/3\n";
    out << "f 6/8/4 7/5/4 3/4/4\n";
    out << "f 3/9/5 7/10/5 8/11/5\n";
    out << "f 1/12/6 4/13/6 8/11/6\n";
    out << "f 1/4/1 2/1/1 4/3/1\n";
    out << "f 5/14/2 8/1/2 6/5/2\n";
    out << "f 1/12/3 5/6/3 2/8/3\n";
    out << "f 2/12/4 6/8/4 3/4/4\n";
    out << "f 4/13/5 3/9/5 8/11/5\n";
    out << "f 5/6/6 1/12/6 8/11/6\n";

    out << "f 2/1/1 3/2/1 4/3/1\n";
    out << "f 8/1/2 7/4/2 6/5/2\n";
    out << "f 5/6/3 6/7/3 2/8/3\n";
    out << "f 6/8/4 7/5/4 3/4/4\n";
    out << "f 3/9/5 7/10/5 8/11/5\n";
    out << "f 1/12/6 4/13/6 8/11/6\n";
    out << "f 1/4/1 2/1/1 4/3/1\n";
    out << "f 5/14/2 8/1/2 6/5/2\n";
    out << "f 1/12/3 5/6/3 2/8/3\n";
    out << "f 2/12/4 6/8/4 3/4/4\n";
    out << "f 4/13/5 3/9/5 8/11/5\n";
    out << "f 5/6/6 1/12/6 8/11/6\n";
    file.close();
  } else {
    FAIL() << "Could not create unrecognized lines file for testing.";
  }
  vp::Figure figure(duplicatesFile);
  EXPECT_EQ(figure.getFacets().size(), 18);
  QFile::remove(duplicatesFile);
}
