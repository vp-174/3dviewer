#pragma once

#include <QVector>

#include "vertex_edge.h"

namespace vp {

/**
 * @class NormalizeParameters
 * @brief Класс для нормализации вершин и удаления дублирующихся граней.
 */
class NormalizeParameters {
 protected:
  /**
   * @brief Получение минимальной координаты из списка вершин.
   * @param v Ссылка на список вершин.
   * @return Вершина с минимальными координатами.
   */
  static Vertex getMinCoord(const QVector<Vertex> &v);

  /**
   * @brief Получение максимальной координаты из списка вершин.
   * @param v Ссылка на список вершин.
   * @return Вершина с максимальными координатами.
   */
  static Vertex getMaxCoord(const QVector<Vertex> &v);

  /**
   * @brief Получение центральной координаты.
   * @param minV Минимальная координата.
   * @param maxV Максимальная координата.
   * @return Вершина с центральными координатами.
   */
  static Vertex getCentralCoord(const Vertex minV, const Vertex maxV);

  /**
   * @brief Отцентрирование всех координат.
   * @param v Ссылка на список вершин.
   */
  static void setCentralVertex(QVector<Vertex> &v);

  /**
   * @brief Установка масштаба координат (установка координат в отрезке от -1 до
   * 1).
   * @param v Ссылка на список вершин.
   */
  static void setScaleVertex(QVector<Vertex> &v);

  /**
   * @brief Конструктор класса NormalizeParameters.
   */
  NormalizeParameters(){};

  /**
   * @brief Деструктор класса NormalizeParameters.
   */
  ~NormalizeParameters() = default;

 public:
  /**
   * @brief Нормализация вершины.
   * @param v Ссылка на список вершин.
   */
  static void setNormalVertex(QVector<Vertex> &v);

  /**
   * @brief Удаление дублирующихся граней.
   * @param f Ссылка на список граней.
   */
  static void remDuplicateFaces(QVector<Edge> &f);
};

}  // namespace vp
