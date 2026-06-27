#pragma once

#include "parser.h"
#include "transform_matrix_builder.h"

namespace vp {

/**
 * @class Figure
 * @brief Класс представляет фигуру, состоящую из вершин и граней.
 */
class Figure {
 private:
  QVector<Vertex> vertices = {};  // список вершин фигуры

  QVector<Edge> facets = {};  // список граней фигуры

  QVector<unsigned int> triangleIndices = {};  // индексы треугольников

 public:
  Figure() {}  // конструктор по умолчанию

  explicit Figure(
      const QString &filename);  // конструктор с параметром (имя файла)

#ifdef GUI
  explicit Figure(const QString &filename,
                  PaintHelp *progress);  // конструктор с параметром (имя файла
                                         // и объект PaintHelp)
#endif

  Figure(const Figure &f);  // конструктор копирования

  ~Figure() = default;  // деструктор по умолчанию

  Figure &operator=(const Figure &f);  // оператор присваивания (возвращает
                                       // ссылку на текущий объект)

  void transform(
      const TransformMatrix &m);  // применение преобразований к вершинам фигуры

  const QVector<Vertex> &getVertices() const;  // получение списка вершин фигуры

  const QVector<Edge> &getFacets() const;  // получение списка граней фигуры

  const QVector<unsigned int> &getTriangleIndices()
      const;  // получение списка индексов треугольников
};

}  // namespace vp
