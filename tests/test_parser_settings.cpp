#include <fstream>

#include "../controller/parser_settings.h"

using namespace vp;

Parameters CreateTestParameters() {
  Parameters p;
  p.filename = "test_file.obj";
  p.shift_x = 1.0;
  p.shift_y = 2.0;
  p.shift_z = 3.0;
  p.rotate_x = 10.0;
  p.rotate_y = 20.0;
  p.rotate_z = 30.0;
  p.scale = 2.0;
  p.type_projection = ProjectionType::Central;
  return p;
}

RenderSetting CreateTestRenderSettings() {
  RenderSetting s;
  s.sett_v.type_vertex = SettingVertex::Type::Circle;
  s.sett_v.size_vertex = 5.0;
  s.sett_v.color_vertex = QColor(255, 0, 0, 255);

  s.sett_l.type_lines = SettingLines::Type::Dashed;
  s.sett_l.line_thickness = 2;
  s.sett_l.color_lines = QColor(0, 255, 0, 255);

  s.color_background = QColor(0, 0, 255, 255);
  return s;
}

// Тест на сохранение и загрузку настроек
TEST(ParserSettingsTest, SaveAndLoadSettings) {
  Parameters p = CreateTestParameters();
  RenderSetting s = CreateTestRenderSettings();

  ParserSettings::saveSettingsToFile(&p, &s);

  ASSERT_TRUE(ParserSettings::checkExistFile("settings.txt"));

  RenderSetting s_loaded;
  Parameters p_loaded = ParserSettings::getSettingsFromFile(&s_loaded);

  EXPECT_EQ(p.filename.toStdString(), p_loaded.filename.toStdString());
  EXPECT_EQ(p.shift_x, p_loaded.shift_x);
  EXPECT_EQ(p.shift_y, p_loaded.shift_y);
  EXPECT_EQ(p.shift_z, p_loaded.shift_z);
  EXPECT_EQ(p.rotate_x, p_loaded.rotate_x);
  EXPECT_EQ(p.rotate_y, p_loaded.rotate_y);
  EXPECT_EQ(p.rotate_z, p_loaded.rotate_z);
  EXPECT_EQ(p.scale, p_loaded.scale);
  EXPECT_EQ(p.type_projection, p_loaded.type_projection);

  EXPECT_EQ(s.sett_v.type_vertex, s_loaded.sett_v.type_vertex);
  EXPECT_EQ(s.sett_v.size_vertex, s_loaded.sett_v.size_vertex);
  EXPECT_EQ(s.sett_v.color_vertex, s_loaded.sett_v.color_vertex);

  EXPECT_EQ(s.sett_l.type_lines, s_loaded.sett_l.type_lines);
  EXPECT_EQ(s.sett_l.line_thickness, s_loaded.sett_l.line_thickness);
  EXPECT_EQ(s.sett_l.color_lines, s_loaded.sett_l.color_lines);

  EXPECT_EQ(s.color_background, s_loaded.color_background);
  std::remove("settings.txt");
}

// Тест на проверку существования файла
TEST(ParserSettingsTest, CheckFileExistence) {
  std::ofstream file("test_file.txt");
  file.close();

  EXPECT_TRUE(ParserSettings::checkExistFile("test_file.txt"));
  EXPECT_FALSE(ParserSettings::checkExistFile("non_existent_file.txt"));

  std::remove("test_file.txt");
}
