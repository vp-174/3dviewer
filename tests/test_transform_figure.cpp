#include <unordered_set>

#include "../model/figure.h"
#include "../model/transform_figure.h"

#define EPS 1e-6

class TransformFigureTest : public ::testing::Test {
 protected:
  void SetUp() override { testFileName = "./models_3d/cube.obj"; }

  QString testFileName;
};

namespace vp {
class TestNormalizeParameters : public NormalizeParameters {
 public:
  static Vertex TestGetMinCoord(const QVector<Vertex> &v) {
    return vp::NormalizeParameters::getMinCoord(v);
  }
  static Vertex TestGetMaxCoord(const QVector<Vertex> &v) {
    return vp::NormalizeParameters::getMaxCoord(v);
  }
  static Vertex TestGetCentralCoord(const Vertex minV, const Vertex maxV) {
    return vp::NormalizeParameters::getCentralCoord(minV, maxV);
  }
  static void TestSetCentralVertex(QVector<Vertex> &v) {
    vp::NormalizeParameters::setCentralVertex(v);
  }
  static void TestSetScaleVertex(QVector<Vertex> &v) {
    vp::NormalizeParameters::setScaleVertex(v);
  }
};
}  // namespace vp

// Тест метода transform, который выполняет преобразование вершин
TEST_F(TransformFigureTest, Transform) {
  vp::Figure figure(testFileName);

  auto originalVertex = figure.getVertices()[0];

  vp::TransformMatrix transformMatrix;
  figure.transform(transformMatrix);

  EXPECT_NE(originalVertex.x(), figure.getVertices()[0].x());
  EXPECT_NE(originalVertex.y(), figure.getVertices()[0].y());
  EXPECT_NE(originalVertex.z(), figure.getVertices()[0].z());
}

// Тест метода transform, который не меняет количество вершин
TEST_F(TransformFigureTest, TransformDoesNotChangeVertexCount) {
  vp::Figure figure(testFileName);

  int originalVertexCount = figure.getVertices().size();

  vp::TransformMatrix transformMatrix;
  figure.transform(transformMatrix);

  ASSERT_EQ(figure.getVertices().size(), originalVertexCount);
}

// Тест, который проверяет, что преобразование не изменяет количество граней
TEST_F(TransformFigureTest, TransformDoesNotChangeFacetCount) {
  vp::Figure figure(testFileName);

  int originalFacetCount = figure.getFacets().size();

  vp::TransformMatrix transformMatrix;
  figure.transform(transformMatrix);

  ASSERT_EQ(figure.getFacets().size(), originalFacetCount);
}

// Проверяет, что метод getMinCoord находит минимальные координаты среди всех
// вершин.
TEST_F(TransformFigureTest, GetMinCoord) {
  vp::Figure figure(testFileName);
  QVector<vp::Vertex> vertices = figure.getVertices();

  vp::Vertex minCoord =
      vp::TestNormalizeParameters::TestGetMinCoord(vertices);

  EXPECT_NEAR(minCoord.x(), -1, EPS);
  EXPECT_NEAR(minCoord.y(), -1, EPS);
  EXPECT_NEAR(minCoord.z(), -1, EPS);
}

// Проверяет, что метод getMaxCoord находит максимальные координаты среди всех
// вершин.
TEST_F(TransformFigureTest, GetMaxCoord) {
  vp::Figure figure(testFileName);

  QVector<vp::Vertex> vertices = figure.getVertices();

  vp::Vertex maxCoord =
      vp::TestNormalizeParameters::TestGetMaxCoord(vertices);

  EXPECT_NEAR(maxCoord.x(), 1, EPS);
  EXPECT_NEAR(maxCoord.y(), 1, EPS);
  EXPECT_NEAR(maxCoord.z(), 1, EPS);
}

// Проверяет, что метод getCentralCoord правильно вычисляет центральную точку
// между минимальными и максимальными координатами.
TEST_F(TransformFigureTest, GetCentralCoord) {
  vp::Figure figure(testFileName);

  QVector<vp::Vertex> vertices = figure.getVertices();
  vp::Vertex minCoord =
      vp::TestNormalizeParameters::TestGetMinCoord(vertices);
  vp::Vertex maxCoord =
      vp::TestNormalizeParameters::TestGetMaxCoord(vertices);

  vp::Vertex centralCoord =
      vp::TestNormalizeParameters::TestGetCentralCoord(minCoord, maxCoord);

  EXPECT_NEAR(centralCoord.x(), 0, EPS);
  EXPECT_NEAR(centralCoord.y(), 0, EPS);
  EXPECT_NEAR(centralCoord.z(), 0, EPS);
}

// Проверяет, что метод setCentralVertex правильно перемещает все вершины
// относительно центральной точки.
TEST_F(TransformFigureTest, SetCentralVertex) {
  vp::Figure figure(testFileName);

  QVector<vp::Vertex> verticesBefore = figure.getVertices();

  vp::Vertex minCoord =
      vp::TestNormalizeParameters::TestGetMinCoord(verticesBefore);
  vp::Vertex maxCoord =
      vp::TestNormalizeParameters::TestGetMaxCoord(verticesBefore);
  vp::TestNormalizeParameters::TestSetCentralVertex(verticesBefore);

  vp::Vertex centralVertex =
      vp::TestNormalizeParameters::TestGetCentralCoord(minCoord, maxCoord);

  EXPECT_EQ(centralVertex.x(), 0);
  EXPECT_EQ(centralVertex.y(), 0);
  EXPECT_EQ(centralVertex.z(), 0);
}

// Проверяет, что метод setScaleVertex корректно масштабирует вершины в диапазон
// от -1 до 1.
TEST_F(TransformFigureTest, SetScaleVertex) {
  vp::Figure figure(testFileName);

  QVector<vp::Vertex> verticesBefore = figure.getVertices();

  vp::TestNormalizeParameters::TestSetScaleVertex(verticesBefore);

  for (const auto &vertex : verticesBefore) {
    EXPECT_GE(vertex.x(), -1);
    EXPECT_LE(vertex.x(), 1);
    EXPECT_GE(vertex.y(), -1);
    EXPECT_LE(vertex.y(), 1);
    EXPECT_GE(vertex.z(), -1);
    EXPECT_LE(vertex.z(), 1);
  }
}

// Проверяет, что метод setNormalVertex корректно работает,
// вызывая функции setCentralVertex и setScaleVertex последовательно.
TEST_F(TransformFigureTest, SetNormalVertex) {
  vp::Figure figure(testFileName);

  QVector<vp::Vertex> verticesBefore = figure.getVertices();

  vp::NormalizeParameters::setNormalVertex(verticesBefore);

  for (const auto &vertex : verticesBefore) {
    EXPECT_GE(vertex.x(), -1);
    EXPECT_LE(vertex.x(), 1);
    EXPECT_GE(vertex.y(), -1);
    EXPECT_LE(vertex.y(), 1);
    EXPECT_GE(vertex.z(), -1);
    EXPECT_LE(vertex.z(), 1);
  }
}

TEST_F(TransformFigureTest, RemoveDuplicateFaces) {
  vp::Figure figure(testFileName);

  QVector<vp::Edge> originalFacets = figure.getFacets();

  ASSERT_FALSE(originalFacets.isEmpty());

  QVector<vp::Edge> facetsWithDuplicates = originalFacets;
  facetsWithDuplicates.append(originalFacets[0]);
  facetsWithDuplicates.append(originalFacets[1]);

  int originalCount = facetsWithDuplicates.size();
  int uniqueCount =
      std::unordered_set<vp::Edge, vp::Edge::HashEdge>(
          facetsWithDuplicates.begin(), facetsWithDuplicates.end())
          .size();
  ASSERT_GT(originalCount, uniqueCount);

  vp::NormalizeParameters::remDuplicateFaces(facetsWithDuplicates);

  EXPECT_EQ(facetsWithDuplicates.size(), uniqueCount);

  ASSERT_EQ(facetsWithDuplicates.size(), originalFacets.size());
  for (int i = 0; i < originalFacets.size(); ++i) {
    EXPECT_EQ(facetsWithDuplicates[i], originalFacets[i]);
  }
}

TEST_F(TransformFigureTest, NormalizedScaleData) {
  QVector<vp::Vertex> vertices = {
      {0.5, -0.5, 0.0}, {-0.8, 0.8, -0.2}, {0.0, 0.0, 0.0}};

  QVector<vp::Vertex> expectedVertices = {{1.0f, -0.5384615, 0.2307692},
                                           {-1.0f, 1.4615385, -0.0769230},
                                           {0.23076927, 0.2307692, 0.2307692}};

  vp::TestNormalizeParameters::TestSetScaleVertex(vertices);
  for (int i = 0; i < vertices.size(); ++i) {
    EXPECT_NEAR(vertices[i].x(), expectedVertices[i].x(), EPS);
    EXPECT_NEAR(vertices[i].y(), expectedVertices[i].y(), EPS);
    EXPECT_NEAR(vertices[i].z(), expectedVertices[i].z(), EPS);
  }
}

// Тест: Преобразование с нулевыми значениями
TEST_F(TransformFigureTest, TransformWithZeroValues) {
  vp::Figure figure(testFileName);

  vp::TransformMatrix transformMatrix;
  transformMatrix(0, 0) = 0.0;
  transformMatrix(1, 1) = 0.0;
  transformMatrix(2, 2) = 0.0;

  figure.transform(transformMatrix);

  for (const auto &vertex : figure.getVertices()) {
    EXPECT_NEAR(vertex.x(), 0.0, EPS);
    EXPECT_NEAR(vertex.y(), 0.0, EPS);
    EXPECT_NEAR(vertex.z(), 0.0, EPS);
  }
}

TEST_F(TransformFigureTest, TransformValues01) {
  vp::Figure figure(testFileName);
  vp::TransformMatrix transformMatrix;
  transformMatrix(0, 0) = 0.33;
  transformMatrix(1, 1) = 0.33;
  transformMatrix(2, 2) = 0.33;
  figure.transform(transformMatrix);

  // Ожидаемые значения для X, Y и Z
  std::array<float, 3> expectedValuesX = {0.33, 0.33, -0.33};
  std::array<float, 3> expectedValuesY = {-0.33, -0.33, -0.33};
  std::array<float, 3> expectedValuesZ = {-0.33, 0.33, 0.33};

  for (qsizetype i = 0; i <= 2; ++i) {
    const auto &vertex = figure.getVertices()[i];
    EXPECT_NEAR(vertex.x(), expectedValuesX[i], EPS);
    EXPECT_NEAR(vertex.y(), expectedValuesY[i], EPS);
    EXPECT_NEAR(vertex.z(), expectedValuesZ[i], EPS);
  }
}

TEST_F(TransformFigureTest, TransformValues02) {
  vp::Figure figure(testFileName);
  vp::TransformMatrix transformMatrix;
  transformMatrix(0, 0) = -0.91;
  transformMatrix(1, 1) = -0.91;
  transformMatrix(2, 2) = -0.91;
  figure.transform(transformMatrix);

  // Ожидаемые значения для X, Y и Z
  std::array<float, 3> expectedValuesX = {-0.91, -0.91, 0.91};
  std::array<float, 3> expectedValuesY = {0.91, 0.91, 0.91};
  std::array<float, 3> expectedValuesZ = {0.91, -0.91, -0.91};

  for (qsizetype i = 0; i <= 2; ++i) {
    const auto &vertex = figure.getVertices()[i];
    EXPECT_NEAR(vertex.x(), expectedValuesX[i], EPS);
    EXPECT_NEAR(vertex.y(), expectedValuesY[i], EPS);
    EXPECT_NEAR(vertex.z(), expectedValuesZ[i], EPS);
  }
}
