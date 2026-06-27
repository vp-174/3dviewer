#pragma once

#include <QFile>
#include <QString>

#include "../view/paint_help.h"
#include "transform_figure.h"

namespace vp {

/**
 * @class Parser
 * @brief Класс для парсинга и сохранения вершин и граней .obj файла.
 */
class Parser {
 private:
  Parser() {}  // конструктор по умолчанию

  ~Parser() = default;  // деструктор по умолчанию

  /**
   * @brief Сохранение координат вершины из строки.
   * @param str Список строк
   * @param vertices Ссылка на список вершин.
   */
  static void saveCoordVertices(const QStringList &str,
                                QVector<Vertex> &vertices);

  /**
   * @brief Сохранение ссылок на грани из строки.
   * @param str Список строк
   * @param facets Ссылка на список граней.
   * @param vertices Ссылка на список вершин.
   */
  static void saveRefFacets(const QStringList &str, QVector<Edge> &facets,
                            QVector<Vertex> &vertices,
                            QVector<unsigned int> *triangleIndices);

 public:
  /**
   * @brief Запись координат вершин и ссылокна грани из файла.
   * @param filename Имя файла с данными.
   * @param vertices Ссылка на список вершин.
   * @param facets Ссылка на список граней.
   */

  static void recordCoordFromFile(const QString &filename,
                                  QVector<Vertex> &vertices,
                                  QVector<Edge> &facets,
                                  QVector<unsigned int> *triangleIndices = nullptr);

#ifdef GUI
  /**
   * @brief Запись координат вершин и ссылок на грани из файла с прогрессом.
   * @param filename Имя файла с данными.
   * @param vertices Ссылка на список вершин.
   * @param facets Ссылка на список граней.
   * @param progress Указатель на объект PaintHelp для отслеживания прогресса.
   * @param triangleIndices Ссылка на список индексов треугольников.
   */
  static void recordCoordFromFile(const QString &filename,
                                  QVector<Vertex> &vertices,
                                  QVector<Edge> &facets, PaintHelp *progress,
                                  QVector<unsigned int> *triangleIndices = nullptr);
#endif
};

}  // namespace vp
