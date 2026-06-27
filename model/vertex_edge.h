#pragma once
#include <QVector3D>
#include <utility>

#include "transform_matrix.h"

namespace vp {

/**
 * @class Vertex
 * @brief Класс представления вершины.
 */
class Vertex : public QVector3D {
 public:
  Vertex() : QVector3D() {}  // конструктор по умолчанию
  Vertex(float x, float y, float z)
      : QVector3D(x, y, z) {}  // конструктор с координатами
  Vertex(const QVector3D& v) : QVector3D(v) {}  // конструктор копирования

  ~Vertex() = default;  // деструктор

  void transform(const TransformMatrix& m);  // применение матрицы преобразования к вершине
};

/**
 * @class Edge
 * @brief Класс представления ребра.
 */
class Edge {
 private:
  Vertex* _begin;  // указатель на начальную вершину ребра
  Vertex* _end;  // указатель на конечную вершину ребра

  std::pair<Vertex*, Vertex*> ordered()
      const;  // вспомогательный метод для
              // упорядочивания двух указателей на вершины (_begin и _end)
              // возвращает их в виде пары (std::pair)
 public:
  Edge(Vertex* begin, Vertex* end);  // конструктор с указателями на вершины

  bool operator<(const Edge& other) const;  // оператор сравнения (для sort)
  bool operator==(const Edge& other) const;  // оператор равенства

  const Vertex* getBeginPosition()
      const;  // получение указателя на начальную точку ребра
  const Vertex* getEndPosition()
      const;  // получение указателя на конечную точку ребра

  struct HashEdge {  // структура для хэширования ребер
    size_t operator()(const Edge& e) const;  // хэш функция
  };
};

}  // namespace vp
