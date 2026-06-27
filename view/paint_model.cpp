#include "paint_model.h"

#include <QDebug>
#include <QOpenGLShader>

using namespace vp;

#define FOCUS 10
#define SCALE 150

static const char *vertexShaderSrc = R"(
#version 330 core
layout(location = 0) in vec3 aPos;

uniform mat4 uTotalMatrix;
uniform int uProjectionType;
uniform vec2 uScreenSize;
uniform float uScale;
uniform float uFocus;

out vec3 vPos;

void main() {
    vec4 tPos = vec4(aPos, 1.0) * uTotalMatrix;
    vec3 pos = tPos.xyz;
    vPos = pos;

    if (uProjectionType == 0) {
        float ndcX = pos.x * uScale * 2.0 / uScreenSize.x;
        float ndcY = pos.y * uScale * 2.0 / uScreenSize.y;
        float ndcZ = clamp(pos.z / 500.0, -1.0, 1.0);
        gl_Position = vec4(ndcX, ndcY, ndcZ, 1.0);
    } else {
        float z = pos.z + uFocus;
        if (z <= 0.001) {
            gl_Position = vec4(0.0, 0.0, -2.0, 1.0);
            return;
        }
        float ndcX = pos.x * uFocus / z * uScale * 2.0 / uScreenSize.x;
        float ndcY = pos.y * uFocus / z * uScale * 2.0 / uScreenSize.y;
        float ndcZ = clamp(pos.z / 500.0, -1.0, 1.0);
        gl_Position = vec4(ndcX, ndcY, ndcZ, 1.0);
    }
}
)";

static const char *fragmentShaderSrc = R"(
#version 330 core
uniform vec4 uColor;
uniform int uVertexType;
uniform int uShadeMode;
uniform vec2 uScreenSize;

in vec3 vPos;
out vec4 FragColor;

void main() {
    if (uShadeMode == 1) {
        vec3 dx = dFdx(vPos);
        vec3 dy = dFdy(vPos);
        vec3 n = normalize(cross(dx, dy));

        vec3 lightDir = normalize(vec3(0.5, 1.0, 0.8));
        vec3 viewDir = normalize(-vPos);
        vec3 halfDir = normalize(lightDir + viewDir);

        float diff = max(dot(n, lightDir), 0.0);
        float spec = pow(max(dot(n, halfDir), 0.0), 32.0);

        float ambient = 0.3;
        vec3 base = vec3(0.5, 0.5, 0.5);
        vec3 lit = base * (ambient + diff * 0.5) + vec3(0.5) * spec;

        FragColor = vec4(lit, uColor.a);
    } else {
        if (uVertexType == 0) {
            vec2 coord = gl_PointCoord - vec2(0.5);
            if (dot(coord, coord) > 0.25) discard;
        }
        FragColor = uColor;
    }
}
)";

void PaintModel::buildShaders() {
  shaderProgram = new QOpenGLShaderProgram(this);
  shaderProgram->addShaderFromSourceCode(QOpenGLShader::Vertex,
                                         vertexShaderSrc);
  shaderProgram->addShaderFromSourceCode(QOpenGLShader::Fragment,
                                         fragmentShaderSrc);
  shaderProgram->link();
}

void PaintModel::uploadBuffers() {
  const auto &vertices = controller->getFigure()->getVertices();
  const auto &facets = controller->getFigure()->getFacets();
  const auto &triIdx = controller->getFigure()->getTriangleIndices();

  if (vertices.isEmpty()) {
    vertexCount = 0;
    indexCount = 0;
    triangleCount = 0;
    return;
  }

  // Upload original vertex positions (once)
  QVector<GLfloat> vertData;
  vertData.reserve(vertices.size() * 3);
  for (const Vertex &v : vertices)
    vertData << v.x() << v.y() << v.z();
  vertexCount = vertices.size();

  vertexBuffer.bind();
  vertexBuffer.allocate(vertData.constData(),
                        vertData.size() * sizeof(GLfloat));
  vertexBuffer.release();

  // Build and upload edge index buffer
  QVector<GLuint> idxData;
  idxData.reserve(facets.size() * 2);
  for (const Edge &e : facets) {
    const Vertex *b = e.getBeginPosition();
    const Vertex *end = e.getEndPosition();
    ptrdiff_t bi = b - vertices.constData();
    ptrdiff_t ei = end - vertices.constData();
    idxData << static_cast<GLuint>(bi) << static_cast<GLuint>(ei);
  }
  indexCount = idxData.size();

  indexBuffer.bind();
  indexBuffer.allocate(idxData.constData(), idxData.size() * sizeof(GLuint));
  indexBuffer.release();

  // Upload triangle index buffer
  triangleCount = triIdx.size();
  if (triangleCount > 0) {
    triangleBuffer.bind();
    triangleBuffer.allocate(triIdx.constData(),
                            triIdx.size() * sizeof(unsigned int));
    triangleBuffer.release();
  }
}

void PaintModel::initializeGL() {
  initializeOpenGLFunctions();
  glClearColor(0.788f, 0.788f, 0.788f, 1.0f);
  glEnable(GL_DEPTH_TEST);
  buildShaders();

  vao.create();
  vao.bind();
  vertexBuffer.create();
  indexBuffer.create();
  triangleBuffer.create();
  vao.release();

  uploadBuffers();
}

void PaintModel::resizeGL(int w, int h) { glViewport(0, 0, w, h); }

void PaintModel::paintGL() {
  QColor bg = s.color_background;
  glClearColor(bg.redF(), bg.greenF(), bg.blueF(), bg.alphaF());
  glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

  if (vertexCount == 0) {
    return;
  }
  shaderProgram->bind();

  QVector2D screenSize(static_cast<float>(width()),
                       static_cast<float>(height()));
  shaderProgram->setUniformValue("uScreenSize", screenSize);
  shaderProgram->setUniformValue(
      "uProjectionType",
      controller->getParam()->type_projection == ProjectionType::Parallel
          ? 0
          : 1);
  shaderProgram->setUniformValue("uScale", 150.0f);
  shaderProgram->setUniformValue("uFocus", 10.0f);

  // Upload total transform matrix as uniform
  float matArr[16];
  controller->getTotalTransform().toFloatArray(matArr);
  shaderProgram->setUniformValue("uTotalMatrix", QMatrix4x4(matArr));

  // Draw shaded surface
  if (s.shadeMode && triangleCount > 0) {
    shaderProgram->setUniformValue("uShadeMode", 1);
    shaderProgram->setUniformValue("uVertexType", 1);
    shaderProgram->setUniformValue(
        "uColor",
        QVector4D(s.sett_l.color_lines.redF(), s.sett_l.color_lines.greenF(),
                  s.sett_l.color_lines.blueF(), s.sett_l.color_lines.alphaF()));

    vao.bind();
    vertexBuffer.bind();
    shaderProgram->enableAttributeArray(0);
    shaderProgram->setAttributeBuffer(0, GL_FLOAT, 0, 3, 0);
    triangleBuffer.bind();
    glDrawElements(GL_TRIANGLES, triangleCount, GL_UNSIGNED_INT, nullptr);
    triangleBuffer.release();
    shaderProgram->disableAttributeArray(0);
    vertexBuffer.release();
    vao.release();
  }

  // Draw edges
  if (s.sett_l.type_lines != SettingLines::Type::None && s.sett_l.line_thickness > 0) {
    shaderProgram->setUniformValue("uShadeMode", 0);
    shaderProgram->setUniformValue("uVertexType", 1);  // square (no discard)
    shaderProgram->setUniformValue(
        "uColor",
        QVector4D(s.sett_l.color_lines.redF(), s.sett_l.color_lines.greenF(),
                  s.sett_l.color_lines.blueF(), s.sett_l.color_lines.alphaF()));

    glLineWidth(static_cast<GLfloat>(qMax(s.sett_l.line_thickness, 1)));
    vao.bind();
    vertexBuffer.bind();
    shaderProgram->enableAttributeArray(0);
    shaderProgram->setAttributeBuffer(0, GL_FLOAT, 0, 3, 0);
    indexBuffer.bind();
    glDrawElements(GL_LINES, indexCount, GL_UNSIGNED_INT, nullptr);
    shaderProgram->disableAttributeArray(0);
    indexBuffer.release();
    vertexBuffer.release();
    vao.release();
    glLineWidth(1.0f);
  }

  // Draw vertices
  if (s.sett_v.type_vertex != SettingVertex::Type::None &&
      s.sett_v.size_vertex > 0) {
    shaderProgram->setUniformValue("uShadeMode", 0);
    shaderProgram->setUniformValue(
        "uVertexType",
        s.sett_v.type_vertex == SettingVertex::Type::Circle
            ? 0
            : 1);
    shaderProgram->setUniformValue(
        "uColor",
        QVector4D(s.sett_v.color_vertex.redF(), s.sett_v.color_vertex.greenF(),
                  s.sett_v.color_vertex.blueF(),
                  s.sett_v.color_vertex.alphaF()));

    glPointSize(static_cast<GLfloat>(s.sett_v.size_vertex));
    vao.bind();
    vertexBuffer.bind();
    shaderProgram->enableAttributeArray(0);
    shaderProgram->setAttributeBuffer(0, GL_FLOAT, 0, 3, 0);
    glDrawArrays(GL_POINTS, 0, static_cast<GLsizei>(vertexCount));
    shaderProgram->disableAttributeArray(0);
    vertexBuffer.release();
    vao.release();
  }

  shaderProgram->release();
  emit frameRendered();
}

void PaintModel::onMove(float x, float y, float z) {
  controller->moveFigure(x, y, z);
  update();
}

void PaintModel::onRotate(float x, float y, float z) {
  controller->rotateFigure(x, y, z);
  update();
}

void PaintModel::onScale(float x, float y, float z) {
  controller->scaleFigure(x, y, z);
  update();
}

void PaintModel::onReset() {
  controller->resetParam();
  resetSetting();
  _rotX = 0.0f;
  _rotY = 0.0f;
  _panX = 0.0f;
  _panY = 0.0f;
  _scaleVal = 1.0f;
  update();
}

void PaintModel::onLoadModel(const QString &filename, PaintHelp *progress) {
  controller->loadFile(filename, progress);
  uploadBuffers();
  update();
}

void PaintModel::onClearModel() {
  controller->clearModel();
  resetSetting();
  vertexCount = 0;
  indexCount = 0;
  triangleCount = 0;
  update();
}

size_t PaintModel::onGetSizeVertices() { return controller->getSizeVertices(); }

size_t PaintModel::onGetSizeFacets() { return controller->getSizeFacets(); }

Parameters *PaintModel::getParamController() { return controller->getParam(); }

RenderSetting *PaintModel::getSettingPaint() { return &s; };

void PaintModel::resetSetting() { s = {}; }

void PaintModel::resizeEvent(QResizeEvent *event) {
  QOpenGLWidget::resizeEvent(event);
  update();
}

void PaintModel::mousePressEvent(QMouseEvent *event) {
  if (event->button() == Qt::LeftButton) {
    _isRotating = true;
    _lastMousePos = event->pos();
    setCursor(Qt::ClosedHandCursor);
  } else if (event->button() == Qt::RightButton) {
    _isPanning = true;
    _lastMousePos = event->pos();
    setCursor(Qt::SizeAllCursor);
  }
}

void PaintModel::mouseMoveEvent(QMouseEvent *event) {
  float dx = event->pos().x() - _lastMousePos.x();
  float dy = event->pos().y() - _lastMousePos.y();
  _lastMousePos = event->pos();

  if (_isRotating) {
    _rotX += dy * 0.3f;
    _rotY += dx * 0.3f;
    controller->rotateFigure(_rotX, _rotY, 0);
    update();
  } else if (_isPanning) {
    _panX += dx * 0.03f;
    _panY -= dy * 0.03f;
    controller->moveFigure(_panX, _panY, 0);
    update();
  }
}

void PaintModel::mouseReleaseEvent(QMouseEvent *event) {
  if (event->button() == Qt::LeftButton && _isRotating) {
    _isRotating = false;
    setCursor(Qt::ArrowCursor);
  } else if (event->button() == Qt::RightButton && _isPanning) {
    _isPanning = false;
    setCursor(Qt::ArrowCursor);
  }
}

void PaintModel::wheelEvent(QWheelEvent *event) {
  float delta = event->angleDelta().y() / 120.0f;
  float factor = 1.0f + delta * 0.1f;
  _scaleVal *= factor;
  controller->scaleFigure(_scaleVal, _scaleVal, _scaleVal);
  update();
}
