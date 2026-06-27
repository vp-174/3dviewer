#pragma once
#include <QObject>

/**
 * @class PaintHelp
 * @brief Класс для работы с прогресс баром
 */
class PaintHelp : public QObject {
  Q_OBJECT

 public:
  explicit PaintHelp(QObject* parent = nullptr)
      : QObject(parent) {
  }  // конструктор с инициализацией указателя на родительский объект (QObject)

  virtual ~PaintHelp() = default;  // виртуальный деструктор

  void setProgress(int progress);  // установка значения прогресса

  int getProgress();  // получение значения прогресса

 signals:
  void progressChanged(
      int value);  // сигнал, который будет испускаться при изменении прогресс

 private:
  int _progress = 0;  // значение прогресса
};