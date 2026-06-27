#pragma once

#include "../model/figure.h"

namespace vp {

/**
 * @enum ProjectionType
 * @brief Тип проекции.
 */
enum class ProjectionType {
  /** Параллельная проекция. */
  Parallel,
  /** Центральная проекция. */
  Central
};

/**
 * @struct Parameters
 * @brief Структура параметров.
 */
struct Parameters {
  /** Имя файла для загрузки модели. */
  QString filename = "";
  /** Текущий сдвиг по оси OX. */
  float shift_x = 0.0f;
  /** Текущий сдвиг по оси OY. */
  float shift_y = 0.0f;
  /** Текущий сдвиг по оси OZ. */
  float shift_z = 0.0f;
  /** Текущий угол поворота по оси OX. */
  float rotate_x = 0.0f;
  /** Текущий угол поворота по оси OY. */
  float rotate_y = 0.0f;
  /** Текущий угол поворота по оси OZ. */
  float rotate_z = 0.0f;
  /** Текущий масштаб. */
  float scale = 1.0f;
  /** Текущий тип проекции. */
  ProjectionType type_projection = ProjectionType::Central;
};

/**
 * @class Facade
 * @brief Предназначен для работы с моделью.
 */
class Facade {
 private:
  /** Модель (фигура). */
  Figure model;
  /** Параметры трансформаций. */
  Parameters p;
  /** Суммарная матрица трансформации. */
  TransformMatrix _totalTransform;
  /**
   * @brief Преобразует угол из градусов в радианы.
   * @param x Угол в градусах.
   * @return Угол в радианах.
   */
  float degreesInRadians(float x);

 public:
  /**
   * @brief Конструктор класса Facade.
   */
  Facade();

  /**
   * @brief Деструктор класса Facade.
   */
  ~Facade() = default;

  /**
   * @brief Перемещение фигуры на заданные координаты.
   * @param x Значение сдвига по оси OX.
   * @param y Значение сдвига по оси OY.
   * @param z Значение сдвига по оси OZ.
   */
  void moveFigure(float x, float y, float z);

  /**
   * @brief Поворот фигуры на заданные углы.
   * @param x Угол поворота по оси OX.
   * @param y Угол поворота по оси OY.
   * @param z Угол поворота по оси OZ.
   */
  void rotateFigure(float x, float y, float z);

  /**
   * @brief Масштабирование фигуры на заданные коэффициенты.
   * @param x Коэффициент масштабирования по оси OX.
   * @param y Коэффициент масштабирования по оси OY.
   * @param z Коэффициент масштабирования по оси OZ.
   */
  void scaleFigure(float x, float y, float z);
  /**
   * @brief Сброс параметров объекта Facade к значениям по умолчанию.
   */
  void resetParam();

  /**
   * @brief Загрузка файла с данными и обновление модели.
   * @param filename Имя файла для загрузки.
   */
  void loadFile(const QString &filename);

#ifdef GUI
  /**
   * @brief Загрузка файла с данными и обновление модели с прогрессом.
   * @param filename Имя файла для загрузки.
   * @param progress Указатель на объект PaintHelp для отслеживания прогресса.
   */
  void loadFile(const QString &filename, PaintHelp *progress);
#endif

  /**
   * @brief Очистка модели.
   */
  void clearModel();

  /**
   * @brief Получение модели.
   * @return Указатель на фигуру(модель).
   */
  Figure *getFigure();

  /**
   * @brief Получение количества вершин.
   * @return Количество вершин.
   */
  size_t getSizeVertices();

  /**
   * @brief Получение количества граней.
   * @return Количество граней.
   */
  size_t getSizeFacets();

  /**
   * @brief Получение параметров.
   * @return Указатель на параметры.
   */
  Parameters *getParam();

  /**
   * @brief Получение суммарной матрицы трансформации.
   * @return Константная ссылка на матрицу трансформации.
   */
  const TransformMatrix &getTotalTransform() const;
};

}  // namespace vp
