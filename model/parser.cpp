#include "parser.h"

#include <QRegularExpression>
#include <QTextStream>

using namespace vp;

namespace {

qint64 estimateLineCount(const QFile &file) {
  qint64 size = file.size();
  return qMax(size / 60, qint64(1));
}

}  // namespace

void Parser::recordCoordFromFile(const QString &filename,
                                 QVector<Vertex> &vertices,
                                 QVector<Edge> &facets,
                                 QVector<unsigned int> *triangleIndices) {
  QFile file(filename);

  if (!file.open(QIODevice::ReadOnly | QIODevice::Text)) {
    throw std::runtime_error("Cannot open file: " + filename.toStdString());
  }

  vertices.clear();
  facets.clear();
  if (triangleIndices) triangleIndices->clear();

  qint64 est = estimateLineCount(file);
  vertices.reserve(est / 2);
  facets.reserve(est * 3 / 2);
  if (triangleIndices) triangleIndices->reserve(est);

  QTextStream text(&file);

  while (!text.atEnd()) {
    QString line = text.readLine().trimmed();
    if (line.isEmpty()) continue;

    QChar prefix = line[0];
    if (prefix == u'v' && (line.size() < 2 || line[1].isSpace())) {
      QStringList parts =
          line.mid(2).trimmed().split(' ', Qt::SkipEmptyParts);
      saveCoordVertices(parts, vertices);
    } else if (prefix == u'f' && (line.size() < 2 || line[1].isSpace())) {
      QStringList parts =
          line.mid(2).trimmed().split(' ', Qt::SkipEmptyParts);
      saveRefFacets(parts, facets, vertices, triangleIndices);
    }
  }

  file.close();
}

#ifdef GUI
void Parser::recordCoordFromFile(const QString &filename,
                                 QVector<Vertex> &vertices,
                                 QVector<Edge> &facets, PaintHelp *progress,
                                 QVector<unsigned int> *triangleIndices) {
  QFile file(filename);

  if (!file.open(QIODevice::ReadOnly | QIODevice::Text)) {
    throw std::runtime_error("Cannot open file: " + filename.toStdString());
  }

  vertices.clear();
  facets.clear();
  if (triangleIndices) triangleIndices->clear();

  qint64 fileSize = file.size();
  qint64 est = estimateLineCount(file);
  vertices.reserve(est / 2);
  facets.reserve(est * 3 / 2);

  QTextStream text(&file);

  int lastProgress = -1;

  while (!text.atEnd()) {
    QString line = text.readLine();
    qint64 pos = file.pos();
    int pct = static_cast<int>((pos * 100) / fileSize);
    if (pct > 100) pct = 100;
    progress->setProgress(pct);
    if (pct != lastProgress && (pct % 10 == 0 || pct == 100)) {
      lastProgress = pct;
    }

    line = line.trimmed();
    if (line.isEmpty()) continue;

    QChar prefix = line[0];
    if (prefix == u'v' && (line.size() < 2 || line[1].isSpace())) {
      QStringList parts =
          line.mid(2).trimmed().split(' ', Qt::SkipEmptyParts);
      saveCoordVertices(parts, vertices);
    } else if (prefix == u'f' && (line.size() < 2 || line[1].isSpace())) {
      QStringList parts =
          line.mid(2).trimmed().split(' ', Qt::SkipEmptyParts);
      saveRefFacets(parts, facets, vertices, triangleIndices);
    }
  }

  file.close();
}
#endif

void Parser::saveCoordVertices(const QStringList &str,
                               QVector<Vertex> &vertices) {
  if (str.size() >= 3) {
    bool okX, okY, okZ;
    float x = str[0].toFloat(&okX);
    float y = str[1].toFloat(&okY);
    float z = str[2].toFloat(&okZ);
    if (okX && okY && okZ) {
      vertices.append(Vertex(x, y, z));
    }
  }
}
void Parser::saveRefFacets(const QStringList &str, QVector<Edge> &facets,
                           QVector<Vertex> &vertices,
                           QVector<unsigned int> *triangleIndices) {
  if (str.size() < 2) return;

  QVarLengthArray<int, 8> index;
  int vertexCount = vertices.size();

  for (int i = 0; i < str.size(); i++) {
    const QString &token = str[i];
    int slashPos = token.indexOf('/');
    bool ok;
    int vertexIndex =
        (slashPos >= 0 ? token.left(slashPos) : token).toInt(&ok) - 1;
    if (ok && vertexIndex >= 0 && vertexIndex < vertexCount) {
      index.append(vertexIndex);
    }
  }

  if (index.size() < 2) return;

  int last = index.size() - 1;
  for (int i = 0; i < last; i++) {
    facets.append(Edge(&vertices[index[i]], &vertices[index[i + 1]]));
  }
  facets.append(Edge(&vertices[index[last]], &vertices[index[0]]));

  // triangle fan indices
  if (triangleIndices && index.size() >= 3) {
    for (int i = 1; i < last; i++) {
      triangleIndices->append(index[0]);
      triangleIndices->append(index[i]);
      triangleIndices->append(index[i + 1]);
    }
  }
}
