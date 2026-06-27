#include "paint_viewer.h"

#include <QDir>
#include <QImage>

#include "../controller/parser_settings.h"

using namespace vp;

PaintViewer::PaintViewer(QMainWindow *parent, Facade *c) : QMainWindow(parent) {
  ui = new Ui::MainWindow;
  ui->setupUi(this);

  pgs = new PaintHelp(this);

  paint_model = new PaintModel(ui->field, c);

  ui->field->setSizePolicy(QSizePolicy::Expanding, QSizePolicy::Expanding);
  ui->menu->setSizePolicy(QSizePolicy::Fixed, QSizePolicy::Expanding);

  set_onOrOff_buttons(false);

  if (ParserSettings::checkExistFile(ParserSettings::filename) == true) {
    set_start_saved_settings();
  } else {
    initialize_text_box();
  }

  set_textToQLabel(ui->number_of_edges, paint_model->onGetSizeFacets());
  set_textToQLabel(ui->number_of_vertices, paint_model->onGetSizeVertices());

  _screencastTimer = new QTimer(this);
  connect(_screencastTimer, &QTimer::timeout, this, &PaintViewer::recordFrame);
  connect(_screencastTimer, &QTimer::timeout, this,
          &PaintViewer::updateCountdown);

  connect(pgs, &PaintHelp::progressChanged, this, &PaintViewer::updateProgress);

  _fpsTimer.start();
  _frameCount = 0;
  _fps = 0.0;
  _lastTime = _fpsTimer.elapsed();  // cохраняем текущее время

  connect(paint_model, &PaintModel::frameRendered, this, &PaintViewer::updateFps);
};

PaintViewer::~PaintViewer() {
  delete ui;
  delete paint_model;
  delete _screencastTimer;
  delete pgs;
};

// прогресс бар
void PaintViewer::updateProgress(int value) {
  ui->progressBar->setVisible(true);
  ui->label_progress->setVisible(true);

  ui->progressBar->setValue(value);
  printProgress(ui->label_progress, value);
  QCoreApplication::processEvents();  // Для мгновенного обновления

  if (value == 100) {
    ui->progressBar->setVisible(false);
    ui->label_progress->setVisible(false);
  }
}

void PaintViewer::updateFps() {
  qint64 currentTime = _fpsTimer.elapsed();  // Получаем текущее время
  _frameCount++;  // Увеличиваем счетчик кадров

  // прошла ли 1 секунда
  if (currentTime - _lastTime >= 1000) {
    _fps = _frameCount;  // Устанавливаем значение FPS
    set_textToQLabel(ui->number_of_fps, _fps);
    _frameCount = 0;  // Сбрасываем счетчик
    _lastTime = currentTime;  // Обновляем время последнего обновления
  }
}

void PaintViewer::closeEvent(QCloseEvent *event) {
  ParserSettings::saveSettingsToFile(paint_model->getParamController(),
                                     paint_model->getSettingPaint());
  event->accept();
}

void PaintViewer::maximizeButton_clicked() {
  this->showMaximized();

  update();
}

void PaintViewer::restoreButton_clicked() {
  this->showNormal();

  update();
}

void PaintViewer::resizeEvent(QResizeEvent *event) {
  QMainWindow::resizeEvent(event);

  ui->field->setGeometry(rect());
  ui->menu->setGeometry(ui->field->width() - ui->menu->width(), 0,
                        ui->menu->width(), ui->field->height());

  if (paint_model) {
    paint_model->setGeometry(0, 0, ui->field->width() - ui->menu->width(),
                             ui->field->height());
    paint_model->update();
  }

  updateFps();
}

void PaintViewer::set_start_saved_settings() {
  Parameters p =
      ParserSettings::getSettingsFromFile(paint_model->getSettingPaint());
  if (ParserSettings::checkExistFile(p.filename.toStdString()) == false) {
    return;
  }
  set_onOrOff_buttons(true);
  paint_model->onLoadModel(p.filename, pgs);

  set_file_name(get_filename(p.filename));

  if (p.type_projection == ProjectionType::Central) {
    ui->typeProjection->setCurrentIndex(0);
  } else {
    ui->typeProjection->setCurrentIndex(1);
  }
  paint_model->getParamController()->type_projection = p.type_projection;

  on_scrollShiftX_valueChanged(p.shift_x);
  on_scrollShiftY_valueChanged(p.shift_y);
  on_scrollShiftZ_valueChanged(p.shift_z);

  on_scrollRotateX_valueChanged(p.rotate_x);
  on_scrollRotateY_valueChanged(p.rotate_y);
  on_scrollRotateZ_valueChanged(p.rotate_z);
  on_scrollScale_valueChanged(p.scale);

  if (paint_model->getSettingPaint()->sett_v.type_vertex ==
      SettingVertex::Type::Circle) {
    ui->typeSelectVertices->setCurrentIndex(0);
  } else if (paint_model->getSettingPaint()->sett_v.type_vertex ==
             SettingVertex::Type::Square) {
    ui->typeSelectVertices->setCurrentIndex(1);
  } else {
    ui->typeSelectVertices->setCurrentIndex(2);
  }
  if (paint_model->getSettingPaint()->sett_l.type_lines ==
      SettingLines::Type::Solid) {
    ui->typeSelectFacets->setCurrentIndex(0);
  } else if (paint_model->getSettingPaint()->sett_l.type_lines ==
             SettingLines::Type::Dashed) {
    ui->typeSelectFacets->setCurrentIndex(1);
  } else {
    ui->typeSelectFacets->setCurrentIndex(2);
  }
  ui->thicknessSelectFacets->setValue(
      paint_model->getSettingPaint()->sett_l.line_thickness);
  ui->sizeSelectVerties->setValue(
      paint_model->getSettingPaint()->sett_v.size_vertex);
  ui->shadeMode->setChecked(
      paint_model->getSettingPaint()->shadeMode);
}

void PaintViewer::updateTextBox(const QString &text, QLineEdit *line,
                                QSlider *scroll) {
  bool ok;
  int value = text.toInt(&ok);
  if (ok) {
    scroll->setValue(value);
  }
  if (value > scroll->maximum()) {
    line->setText(QString::number(scroll->maximum()));
  }
  if (value < scroll->minimum()) {
    line->setText(QString::number(scroll->minimum()));
  }
}

void PaintViewer::initialize_text_box() {
  on_scrollShiftX_valueChanged(ui->scrollShiftX->value());
  on_scrollShiftY_valueChanged(ui->scrollShiftY->value());
  on_scrollShiftZ_valueChanged(ui->scrollShiftZ->value());
  on_scrollRotateX_valueChanged(ui->scrollRotateX->value());
  on_scrollRotateY_valueChanged(ui->scrollRotateY->value());
  on_scrollRotateZ_valueChanged(ui->scrollRotateZ->value());
  on_scrollScale_valueChanged(ui->scrollScale->value());
}

void PaintViewer::on_scrollShiftX_valueChanged(int value) {
  ui->boxShiftX->setText(QString::number(value));
  paint_model->onMove(ui->scrollShiftX->value(), ui->scrollShiftY->value(),
                      ui->scrollShiftZ->value());

  updateFps();
}

void PaintViewer::on_boxShiftX_textChanged(const QString &text) {
  updateTextBox(text, ui->boxShiftX, ui->scrollShiftX);
}

void PaintViewer::on_scrollShiftY_valueChanged(int value) {
  ui->boxShiftY->setText(QString::number(value));
  paint_model->onMove(ui->scrollShiftX->value(), ui->scrollShiftY->value(),
                      ui->scrollShiftZ->value());

  updateFps();
}

void PaintViewer::on_boxShiftY_textChanged(const QString &text) {
  updateTextBox(text, ui->boxShiftY, ui->scrollShiftY);
}

void PaintViewer::on_scrollShiftZ_valueChanged(int value) {
  ui->boxShiftZ->setText(QString::number(value));
  paint_model->onMove(ui->scrollShiftX->value(), ui->scrollShiftY->value(),
                      ui->scrollShiftZ->value());

  updateFps();
}

void PaintViewer::on_boxShiftZ_textChanged(const QString &text) {
  updateTextBox(text, ui->boxShiftZ, ui->scrollShiftZ);
}

void PaintViewer::on_scrollRotateX_valueChanged(int value) {
  ui->boxRotateX->setText(QString::number(value));
  paint_model->onRotate(ui->scrollRotateX->value(),
                        paint_model->getParamController()->rotate_y,
                        paint_model->getParamController()->rotate_z);

  updateFps();
}

void PaintViewer::on_boxRotateX_textChanged(const QString &text) {
  updateTextBox(text, ui->boxRotateX, ui->scrollRotateX);
}

void PaintViewer::on_scrollRotateY_valueChanged(int value) {
  ui->boxRotateY->setText(QString::number(value));
  paint_model->onRotate(paint_model->getParamController()->rotate_x,
                        ui->scrollRotateY->value(),
                        paint_model->getParamController()->rotate_z);

  updateFps();
}

void PaintViewer::on_boxRotateY_textChanged(const QString &text) {
  updateTextBox(text, ui->boxRotateY, ui->scrollRotateY);
}

void PaintViewer::on_scrollRotateZ_valueChanged(int value) {
  ui->boxRotateZ->setText(QString::number(value));
  paint_model->onRotate(paint_model->getParamController()->rotate_x,
                        paint_model->getParamController()->rotate_y,
                        ui->scrollRotateZ->value());

  updateFps();
}

void PaintViewer::on_boxRotateZ_textChanged(const QString &text) {
  updateTextBox(text, ui->boxRotateZ, ui->scrollRotateZ);
}

void PaintViewer::on_scrollScale_valueChanged(int value) {
  ui->boxScale->setText(QString::number(value));
  paint_model->onScale(value, value, value);

  updateFps();
}

void PaintViewer::on_boxScale_textChanged(const QString &text) {
  updateTextBox(text, ui->boxScale, ui->scrollScale);
}

void PaintViewer::on_thicknessSelectFacets_valueChanged() {
  QToolTip::showText(QCursor::pos(),
                     QString::number(ui->thicknessSelectFacets->value()), this);
  paint_model->getSettingPaint()->sett_l.line_thickness =
      ui->thicknessSelectFacets->value();
  paint_model->update();
}

void PaintViewer::on_sizeSelectVerties_valueChanged() {
  QToolTip::showText(QCursor::pos(),
                     QString::number(ui->sizeSelectVerties->value()), this);
  paint_model->getSettingPaint()->sett_v.size_vertex =
      ui->sizeSelectVerties->value();
  paint_model->update();
}

void PaintViewer::on_typeSelectFacets_currentIndexChanged(int index) {
  if (index == 0) {
    paint_model->getSettingPaint()->sett_l.type_lines =
        SettingLines::Type::Solid;
  } else if (index == 1) {
    paint_model->getSettingPaint()->sett_l.type_lines =
        SettingLines::Type::Dashed;
  } else {
    paint_model->getSettingPaint()->sett_l.type_lines =
        SettingLines::Type::None;
  }
  paint_model->update();
}

void PaintViewer::on_typeSelectVertices_currentIndexChanged(int index) {
  if (index == 0) {
    paint_model->getSettingPaint()->sett_v.type_vertex =
        SettingVertex::Type::Circle;
  } else if (index == 1) {
    paint_model->getSettingPaint()->sett_v.type_vertex =
        SettingVertex::Type::Square;
  } else if (index == 2) {
    paint_model->getSettingPaint()->sett_v.type_vertex =
        SettingVertex::Type::None;
  }
  paint_model->update();
}

void PaintViewer::on_shadeMode_clicked(bool checked) {
  paint_model->getSettingPaint()->shadeMode = checked;
  paint_model->update();
}

void PaintViewer::on_colorSelectFacets_pressed() {
  QColor color = QColorDialog::getColor(Qt::white, this, "Выберите цвет ребер");

  if (color.isValid()) {
    paint_model->getSettingPaint()->sett_l.color_lines = color;
    paint_model->update();
  }
}

void PaintViewer::on_colorSelectVertices_pressed() {
  QColor color =
      QColorDialog::getColor(Qt::white, this, "Выберите цвет вершин");

  if (color.isValid()) {
    paint_model->getSettingPaint()->sett_v.color_vertex = color;
    paint_model->update();
  }
}

void PaintViewer::on_colorSelectBackground_pressed() {
  QColor color = QColorDialog::getColor(Qt::white, this, "Выберите цвет фона");

  if (color.isValid()) {
    paint_model->getSettingPaint()->color_background = color;
    paint_model->update();
  }
}

void PaintViewer::on_typeProjection_currentIndexChanged(int index) {
  if (index == 0) {
    paint_model->getParamController()->type_projection =
        ProjectionType::Central;

  } else {
    paint_model->getParamController()->type_projection =
        ProjectionType::Parallel;
  }
  paint_model->update();
}

void PaintViewer::on_resetSettings_pressed() {
  on_scrollShiftX_valueChanged(0);
  on_scrollShiftY_valueChanged(0);
  on_scrollShiftZ_valueChanged(0);

  on_scrollRotateX_valueChanged(0);
  on_scrollRotateY_valueChanged(0);
  on_scrollRotateZ_valueChanged(0);

  on_scrollScale_valueChanged(1);
  paint_model->onReset();
  reset_button();
}
void PaintViewer::reset_button() {
  on_boxShiftX_textChanged("0");
  on_boxShiftY_textChanged("0");
  on_boxShiftZ_textChanged("0");
  on_boxRotateX_textChanged("0");
  on_boxRotateY_textChanged("0");
  on_boxRotateZ_textChanged("0");
  on_boxScale_textChanged("1");

  ui->typeSelectVertices->setCurrentIndex(2);
  ui->typeProjection->setCurrentIndex(0);
  ui->typeSelectFacets->setCurrentIndex(2);
  ui->thicknessSelectFacets->setValue(1);
  ui->sizeSelectVerties->setValue(1);
  ui->shadeMode->setChecked(true);
}

void PaintViewer::on_chooseFile_pressed() {
  QString dir = QDir::currentPath() + "/models_3d";
  QString filePath = QFileDialog::getOpenFileName(
      this, tr("Выберите файл"), dir, tr("Файлы 3D моделей (*.obj)"));
  if (filePath.isEmpty()) {
    QMessageBox::warning(this, tr("Ошибка"), tr("Файл не выбран"));
    return;
  }
  set_onOrOff_buttons(true);
  reset_button();
  paint_model->onLoadModel(filePath, pgs);

  set_textToQLabel(ui->number_of_edges, paint_model->onGetSizeFacets());
  // qDebug() << "size of facets: " << paint_model->onGetSizeFacets();
  set_textToQLabel(ui->number_of_vertices, paint_model->onGetSizeVertices());
  // qDebug() << "size of vertices: " << paint_model->onGetSizeVertices();
  QString fileName = get_filename(filePath);
  set_file_name(fileName);
  on_resetSettings_pressed();
}

void PaintViewer::on_closeFile_pressed() {
  paint_model->onClearModel();
  ParserSettings::saveSettingsToFile(paint_model->getParamController(),
                                     paint_model->getSettingPaint());
  set_onOrOff_buttons(false);
  set_file_name("");
  set_textToQLabel(ui->number_of_edges, 0);
  set_textToQLabel(ui->number_of_vertices, 0);
  set_textToQLabel(ui->number_of_fps, 0);
}

void PaintViewer::on_saveAsBmpOrJpeg_pressed() {
  QString format;
  QDir dir("saved_images");
  if (!dir.exists()) {
    dir.mkpath(".");
  }
  QString fileName = QFileDialog::getSaveFileName(
      this, tr("Сохранить изображение"), dir.absolutePath(),
      tr("BMP (*.bmp);; JPEG (*.jpg)"), &format);

  if (!fileName.isEmpty()) {
    QPixmap image = this->grab(QRect(
        0, 0, ui->field->width() - ui->menu->width(), this->height()));

    QStringList formatList = format.split(";");
    QString extension = fileName.section('.', -1);
    QString imageFormat = formatList.contains("BMP (*.bmp)") ? "bmp" : "jpg";
    if (extension != imageFormat) {
      fileName += "." + imageFormat;
    }
    image.save(fileName, imageFormat.toLatin1().constData());
    QMessageBox::information(this, "Export info",
                             "Изображение сохранено в папку saved_images/");
  } else {
    QMessageBox::warning(this, tr("Error"), tr("Не удалось сохранить файл"));
  }
}

void PaintViewer::set_textToQLabel(QLabel *text, const size_t size) {
  text->setText(QString::number(size));
}

void PaintViewer::printProgress(QLabel *text, const size_t size) {
  text->setText(QString::number(size) + "%");
}

void PaintViewer::set_file_name(const QString &filename) {
  ui->fileName->setText(filename);
}

QString PaintViewer::get_filename(const QString &filePath) {
  QFileInfo fileInfo(filePath);
  return fileInfo.fileName();
}

void PaintViewer::set_onOrOff_buttons(bool enabled) {
  ui->scrollRotateX->setEnabled(enabled);
  ui->scrollRotateY->setEnabled(enabled);
  ui->scrollRotateZ->setEnabled(enabled);
  ui->scrollShiftX->setEnabled(enabled);
  ui->scrollShiftY->setEnabled(enabled);
  ui->scrollShiftZ->setEnabled(enabled);
  ui->scrollScale->setEnabled(enabled);

  ui->typeProjection->setEnabled(enabled);

  ui->resetSettings->setEnabled(enabled);

  ui->boxShiftX->setEnabled(enabled);
  ui->boxShiftY->setEnabled(enabled);
  ui->boxShiftZ->setEnabled(enabled);
  ui->boxRotateX->setEnabled(enabled);
  ui->boxRotateY->setEnabled(enabled);
  ui->boxRotateZ->setEnabled(enabled);
  ui->boxScale->setEnabled(enabled);

  ui->thicknessSelectFacets->setEnabled(enabled);
  ui->typeSelectFacets->setEnabled(enabled);
  ui->colorSelectFacets->setEnabled(enabled);

  ui->sizeSelectVerties->setEnabled(enabled);
  ui->typeSelectVertices->setEnabled(enabled);
  ui->colorSelectVertices->setEnabled(enabled);
  ui->shadeMode->setEnabled(enabled);

  ui->colorSelectBackground->setEnabled(enabled);

  ui->saveAsBmpOrJpeg->setEnabled(enabled);
  ui->saveAsGif->setEnabled(enabled);
  ui->closeFile->setEnabled(enabled);
}

void PaintViewer::on_saveAsGif_pressed() {
  _frameCounter = 0;
  _outputDir = "saved_screencasts/";
  QDir dir(_outputDir);
  if (!dir.exists()) {
    dir.mkpath(".");
  }
  _screencastTimer->start(100);
  ui->saveAsGif->setEnabled(false);
}

void PaintViewer::recordFrame() {
  if (_frameCounter < 100) {
    QString filename = QString("%1frame_%2.png")
                           .arg(_outputDir)
                           .arg(_frameCounter, 3, 10, QChar('0'));

    QPixmap pixmap = this->grab(QRect(
        0, 0, ui->field->width() - ui->menu->width(), this->height()));

    QImage frame = pixmap.toImage().scaled(640, 480, Qt::KeepAspectRatio,
                                           Qt::SmoothTransformation);

    frame.save(filename, "PNG");

    _frameCounter++;

  } else {
    _screencastTimer->stop();

    QString gifFileName = QFileDialog::getSaveFileName(
        this, "Сохранить GIF", _outputDir, "GIF Files (*.gif)");

    int error = gifFileName.isEmpty();

    if (!error) {
      if (!gifFileName.endsWith(".gif")) {
        gifFileName += ".gif";
      }
      QString gifCommand =
          QString("convert -delay 10 -loop 0 %1frame_*.png \"%2\"")
              .arg(_outputDir)
              .arg(gifFileName);

      error |= system(gifCommand.toStdString().c_str());

      QString cleanupCommand = QString("rm -f %1frame_*.png").arg(_outputDir);

      error |= system(cleanupCommand.toStdString().c_str());

      if (!error) {
        QMessageBox::information(
            this, "Export info",
            "Скринкаст сохранён в папку saved_screencasts/");
      }

    } else {
      QMessageBox::warning(this, tr("Ошибка"), tr("Не удалось сохранить файл"));
    }
    ui->saveAsGif->setEnabled(true);
  }
}

void PaintViewer::updateCountdown() {
  static int countdown = 100;

  if (!ui->saveAsGif->isEnabled()) {
    if (countdown > 0) {
      if (countdown % 10 == 0) {
        ui->saveAsGif->setText(QString::number(countdown / 10));
      }
      countdown--;
    }
    if (countdown == 0) {
      ui->saveAsGif->setText("gif");
      countdown = 100;
    }
  }
}
