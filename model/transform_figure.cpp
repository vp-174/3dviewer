#include "transform_figure.h"

using namespace vp;

Vertex NormalizeParameters::getMinCoord(const QVector<Vertex> &v) {
  Vertex minCoord;

  minCoord = v[0];
  for (const auto &i : v) {
    minCoord.setX(std::min(i.x(), minCoord.x()));
    minCoord.setY(std::min(i.y(), minCoord.y()));
    minCoord.setZ(std::min(i.z(), minCoord.z()));
  }
  return minCoord;
}

Vertex NormalizeParameters::getMaxCoord(const QVector<Vertex> &v) {
  Vertex maxCoord;

  maxCoord = v[0];
  for (const auto &i : v) {
    maxCoord.setX(std::max(i.x(), maxCoord.x()));
    maxCoord.setY(std::max(i.y(), maxCoord.y()));
    maxCoord.setZ(std::max(i.z(), maxCoord.z()));
  }
  return maxCoord;
}

Vertex NormalizeParameters::getCentralCoord(const Vertex minV,
                                            const Vertex maxV) {
  Vertex centr;
  centr.setX(minV.x() + (maxV.x() - minV.x()) / 2);
  centr.setY(minV.y() + (maxV.y() - minV.y()) / 2);
  centr.setZ(minV.z() + (maxV.z() - minV.z()) / 2);

  return centr;
}

void NormalizeParameters::setCentralVertex(QVector<Vertex> &v) {
  Vertex centr = getCentralCoord(getMinCoord(v), getMaxCoord(v));
  for (int i = 0; i < v.size(); i++) {
    v[i] -= centr;
  }
}

void NormalizeParameters::setScaleVertex(QVector<Vertex> &v) {
  Vertex min_coord = getMinCoord(v);
  Vertex scale = getMaxCoord(v) - min_coord;
  float common_scale = std::max(scale.x(), std::max(scale.y(), scale.z()));
  common_scale = common_scale == 0.0f ? 1.0f : common_scale;
  float min = std::min(min_coord.x(), std::min(min_coord.y(), min_coord.z()));

  for (int i = 0; i < v.size(); i++) {
    v[i].setX(((v[i].x() - min) / common_scale) * 2 - 1);
    v[i].setY(((v[i].y() - min) / common_scale) * 2 - 1);
    v[i].setZ(((v[i].z() - min) / common_scale) * 2 - 1);
  }
}

void NormalizeParameters::setNormalVertex(QVector<Vertex> &v) {
  setCentralVertex(v);
  setScaleVertex(v);
}

void NormalizeParameters::remDuplicateFaces(QVector<Edge> &f) {
  // сортировка с использованием перегруженного оператора <
  std::sort(f.begin(), f.end());

  // удаление последовательных дубликатов
  auto last = std::unique(f.begin(), f.end());
  f.erase(last, f.end());

  // освобождение неиспользуемой памяти
  f.squeeze();
}
