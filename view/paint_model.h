#pragma once

#include <QMouseEvent>
#include <QOpenGLBuffer>
#include <QOpenGLFunctions>
#include <QOpenGLShaderProgram>
#include <QOpenGLVertexArrayObject>
#include <QOpenGLWidget>
#include <QWheelEvent>

#include "../controller/controller.h"

namespace vp {

/**
 * @struct SettingVertex
 * @brief Настройки отображения вершин.
 */
struct SettingVertex {
  /** Тип отображения вершин. */
  enum class Type { Circle, Square, None };
  /** Тип вершин. */
  Type type_vertex = Type::None;
  /** Размер вершин. */
  int size_vertex = 1;
  /** Цвет вершин. */
  QColor color_vertex = Qt::yellow;
};

/**
 * @struct SettingLines
 * @brief Настройки отображения рёбер.
 */
struct SettingLines {
  /** Тип отображения линий. */
  enum class Type { Solid, Dashed, None };
  /** Тип линий. */
  Type type_lines = Type::None;
  /** Толщина линий. */
  int line_thickness = 1;
  /** Цвет линий. */
  QColor color_lines = Qt::black;
};

/**
 * @struct RenderSetting
 * @brief Параметры рендеринга сцены.
 */
struct RenderSetting {
  /** Настройки вершин. */
  SettingVertex sett_v = {};
  /** Настройки рёбер. */
  SettingLines sett_l = {};
  /** Цвет фона. */
  QColor color_background = QColor("#c9c9c9");
  /** Цвет шрифта. */
  QColor color_font = QColor("#ffffff");
  /** Включение ShadeMode. */
  bool shadeMode = true;
};

/**
 * @class PaintModel
 * @brief Виджет OpenGL для отрисовки 3D-модели с шейдерами.
 *
 * Управляет VAO/VBO/EBO, загружает данные на GPU,
 * обрабатывает события мыши для вращения/панорамирования/масштабирования.
 */
class PaintModel : public QOpenGLWidget, protected QOpenGLFunctions {
  Q_OBJECT
 private:
  /** Указатель на фасад контроллера. */
  Facade *controller;
  /** Текущие настройки рендеринга. */
  RenderSetting s;

  /** Шейдерная программа GLSL. */
  QOpenGLShaderProgram *shaderProgram = nullptr;
  /** VAO для хранения привязок буферов. */
  QOpenGLVertexArrayObject vao;
  /** VBO для хранения вершинных данных. */
  QOpenGLBuffer vertexBuffer{QOpenGLBuffer::VertexBuffer};
  /** EBO для индексов рёбер. */
  QOpenGLBuffer indexBuffer{QOpenGLBuffer::IndexBuffer};
  /** EBO для индексов треугольников (ShadeMode). */
  QOpenGLBuffer triangleBuffer{QOpenGLBuffer::IndexBuffer};
  /** Количество вершин. */
  int vertexCount = 0;
  /** Количество индексов рёбер. */
  int indexCount = 0;
  /** Количество индексов треугольников. */
  int triangleCount = 0;

  /**
   * @brief Обработка изменения размера виджета.
   * @param event Событие изменения размера.
   */
  void resizeEvent(QResizeEvent *event) override;
  /**
   * @brief Сброс настроек до значений по умолчанию.
   */
  void resetSetting();

 signals:
  /** Сигнал об отрисовке кадра (для FPS). */
  void frameRendered();

 public:
  /**
   * @brief Конструктор.
   * @param parent Родительский виджет.
   * @param c Указатель на фасад.
   */
  PaintModel(QWidget *parent, Facade *c)
      : QOpenGLWidget(parent), controller(c), s({}) {
    QSurfaceFormat fmt;
    fmt.setSamples(4);
    setFormat(fmt);
  }
  /** @brief Деструктор. */
  ~PaintModel() override = default;

  /**
   * @brief Перемещение модели.
   * @param x Сдвиг по OX.
   * @param y Сдвиг по OY.
   * @param z Сдвиг по OZ.
   */
  void onMove(float x, float y, float z);
  /**
   * @brief Поворот модели.
   * @param x Угол по OX.
   * @param y Угол по OY.
   * @param z Угол по OZ.
   */
  void onRotate(float x, float y, float z);
  /**
   * @brief Масштабирование модели.
   * @param x Коэффициент по OX.
   * @param y Коэффициент по OY.
   * @param z Коэффициент по OZ.
   */
  void onScale(float x, float y, float z);
  /**
   * @brief Сброс трансформаций.
   */
  void onReset();
  /**
   * @brief Получение количества вершин.
   * @return Количество вершин.
   */
  size_t onGetSizeVertices();
  /**
   * @brief Получение количества граней.
   * @return Количество граней.
   */
  size_t onGetSizeFacets();
  /**
   * @brief Загрузка модели из файла.
   * @param filename Имя файла.
   * @param progress Указатель на прогресс-бар.
   */
  void onLoadModel(const QString &filename, PaintHelp *progress);
  /**
   * @brief Очистка модели.
   */
  void onClearModel();
  /**
   * @brief Получение указателя на параметры контроллера.
   * @return Указатель на Parameters.
   */
  Parameters *getParamController();
  /**
   * @brief Получение указателя на настройки рендеринга.
   * @return Указатель на RenderSetting.
   */
  RenderSetting *getSettingPaint();

 protected:
  /**
   * @brief Инициализация OpenGL (шейдеры, буферы).
   */
  void initializeGL() override;
  /**
   * @brief Обработка изменения размера окна OpenGL.
   * @param w Новая ширина.
   * @param h Новая высота.
   */
  void resizeGL(int w, int h) override;
  /**
   * @brief Отрисовка кадра.
   */
  void paintGL() override;
  /**
   * @brief Обработка нажатия кнопки мыши.
   * @param event Событие мыши.
   */
  void mousePressEvent(QMouseEvent *event) override;
  /**
   * @brief Обработка движения мыши.
   * @param event Событие мыши.
   */
  void mouseMoveEvent(QMouseEvent *event) override;
  /**
   * @brief Обработка отпускания кнопки мыши.
   * @param event Событие мыши.
   */
  void mouseReleaseEvent(QMouseEvent *event) override;
  /**
   * @brief Обработка колесика мыши (масштаб).
   * @param event Событие колесика.
   */
  void wheelEvent(QWheelEvent *event) override;

 private:
  /**
   * @brief Сборка и компиляция шейдеров.
   */
  void buildShaders();
  /**
   * @brief Загрузка данных вершин/индексов на GPU.
   */
  void uploadBuffers();

  /** Флаг вращения мышью. */
  bool _isRotating = false;
  /** Флаг панорамирования мышью. */
  bool _isPanning = false;
  /** Последняя позиция мыши. */
  QPointF _lastMousePos;
  /** Накопленный угол вращения по OX. */
  float _rotX = 0.0f;
  /** Накопленный угол вращения по OY. */
  float _rotY = 0.0f;
  /** Накопленный сдвиг по X. */
  float _panX = 0.0f;
  /** Накопленный сдвиг по Y. */
  float _panY = 0.0f;
  /** Накопленный масштаб. */
  float _scaleVal = 1.0f;
};

}  // namespace vp
