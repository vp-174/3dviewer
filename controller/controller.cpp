#include "controller.h"

/**
 * @def STEP
 * @brief Шаг для сдвига.
 */
#define STEP 5

#include <cmath>

using namespace vp;

float Facade::degreesInRadians(float x) { return x * M_PI / 180.0f; }

Facade::Facade() : p({}) { _totalTransform.setIdentity(); }

void Facade::resetParam() {
  p.scale = 1.0f;
  p.shift_x = 0.0f;
  p.shift_y = 0.0f;
  p.shift_z = 0.0f;
  p.rotate_x = 0.0f;
  p.rotate_y = 0.0f;
  p.rotate_z = 0.0f;
  p.type_projection = ProjectionType::Central;
  _totalTransform.setIdentity();
}

void Facade::loadFile(const QString &filename) {
  if (filename.isEmpty()) {
    return;
  }
  p.filename = filename;
  Figure f(filename);
  model = f;
}

#ifdef GUI
void Facade::loadFile(const QString &filename, PaintHelp *progress) {
  if (filename.isEmpty()) {
    return;
  }
  p.filename = filename;
  Figure f(filename, progress);
  model = f;
}
#endif

void Facade::clearModel() {
  model = Figure();
  p.filename.clear();
  resetParam();
  _totalTransform.setIdentity();
}

void Facade::moveFigure(float x, float y, float z) {
  TransformMatrix delta = TransformMatrixBuilder::createMoveMatrix(
      (x - p.shift_x) / STEP, (y - p.shift_y) / STEP, (z - p.shift_z) / STEP);
  _totalTransform = _totalTransform * delta;
  p.shift_x = x;
  p.shift_y = y;
  p.shift_z = z;
}

void Facade::rotateFigure(float x, float y, float z) {
  TransformMatrix rotateMat = TransformMatrixBuilder::createRotateMatrix(
      degreesInRadians(p.rotate_x), degreesInRadians(p.rotate_y),
      degreesInRadians(p.rotate_z));
  rotateMat.InverseTransformMatrix();

  rotateMat =
      TransformMatrixBuilder::createMoveMatrix(
          -p.shift_x / STEP, -p.shift_y / STEP, -p.shift_z / STEP) *
      rotateMat *
      TransformMatrixBuilder::createRotateMatrix(
          degreesInRadians(x), degreesInRadians(y), degreesInRadians(z)) *
      TransformMatrixBuilder::createMoveMatrix(
          p.shift_x / STEP, p.shift_y / STEP, p.shift_z / STEP);

  _totalTransform = _totalTransform * rotateMat;
  p.rotate_x = x;
  p.rotate_y = y;
  p.rotate_z = z;
}

void Facade::scaleFigure(float x, float y, float z) {
  TransformMatrix scaleMat =
      TransformMatrixBuilder::createMoveMatrix(
          -p.shift_x / STEP, -p.shift_y / STEP, -p.shift_z / STEP) *
      TransformMatrixBuilder::createScaleMatrix(x / p.scale, y / p.scale,
                                                z / p.scale) *
      TransformMatrixBuilder::createMoveMatrix(
          p.shift_x / STEP, p.shift_y / STEP, p.shift_z / STEP);

  _totalTransform = _totalTransform * scaleMat;
  p.scale = x;
}

Figure *Facade::getFigure() { return &model; }

size_t Facade::getSizeVertices() { return model.getVertices().size(); }

size_t Facade::getSizeFacets() { return model.getFacets().size(); }

Parameters *Facade::getParam() { return &p; }

const TransformMatrix &Facade::getTotalTransform() const {
  return _totalTransform;
}
