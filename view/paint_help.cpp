#include <../view/paint_help.h>

void PaintHelp::setProgress(int progress) {
  _progress = progress;
  emit progressChanged(_progress);  // Имитируем сигнал
}

int PaintHelp::getProgress() { return _progress; }
