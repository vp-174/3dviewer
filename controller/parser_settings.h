#pragma once

#include <string>

#include "../view/paint_model.h"
#include "controller.h"

namespace vp {
/**
 * @class ParserSettings
 * @brief Предназначен для сохранения и получения настроек фигуры и отрисовки.
 */
class ParserSettings {
 private:
  /**
   * @brief Конструктор класса ParserSettings.
   */
  ParserSettings() {}

  /**
   * @brief Деструктор класса ParserSettings.
   */
  ~ParserSettings() = default;

 public:
  /** Имя файла настроек. */
  inline static const std::string filename = "settings.txt";

  /**
   * @brief Сохранение настроек в файл.
   * @param p Указатель на структуру параметров.
   * @param s Указатель на структуру настроек рендеринга.
   */
  static void saveSettingsToFile(Parameters *p, RenderSetting *s);

  /**
   * @brief Получение настроек из файла.
   * @param s Указатель на структуру настроек рендеринга.
   * @return Структура параметров.
   */
  static Parameters getSettingsFromFile(RenderSetting *s);

  /**
   * @brief Проверка существования файла.
   * @param name Имя файла.
   * @return true - если файл существует, false - если не существует.
   */
  static bool checkExistFile(const std::string &name);
};
}  // namespace vp
