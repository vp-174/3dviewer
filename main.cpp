#include <QApplication>

#include "view/paint_viewer.h"

/**
 * @brief Главная функция приложения 3D_VIEWER.
 * Функция `main` является входной точкой для приложения 3D_VIEWER,
 * которое создает графический интерфейс для отображения и взаимодействия
 * с 3D-сценами. Использует классы из библиотеки Qt и пользовательский
 * класс `vp::Facade` для управления сценой и `vp::PaintViewer`
 * для отображения графического интерфейса.
 */

#ifndef FOR_TESTS

int main(int argc, char *argv[]) {
  QApplication app(argc, argv);
  vp::Facade scene;
  vp::PaintViewer window(nullptr, &scene);

  window.resize(1200, 800);
  window.setWindowTitle("VP 3DViewer (OpenGL)");
  window.show();
  return app.exec();
}

#endif
