#include "parser_settings.h"

#include <fstream>
#include <iostream>

using namespace vp;

void ParserSettings::saveSettingsToFile(Parameters *p, RenderSetting *s) {
  std::ofstream file(filename);
  file.clear();
  if (p->filename.isEmpty()) {
    file << ". ";
  } else {
    file << p->filename.toStdString() << " ";
  }
  file << p->shift_x << " " << p->shift_y << " " << p->shift_z << " ";
  file << p->rotate_x << " " << p->rotate_y << " " << p->rotate_z << " ";
  file << p->scale << " ";
  file << static_cast<int>(p->type_projection) << " ";

  file << static_cast<int>(s->sett_v.type_vertex) << " "
       << s->sett_v.size_vertex << " ";
  file << s->sett_v.color_vertex.red() << " " << s->sett_v.color_vertex.green()
       << " " << s->sett_v.color_vertex.blue() << " "
       << s->sett_v.color_vertex.alpha() << " ";

  file << static_cast<int>(s->sett_l.type_lines) << " "
       << s->sett_l.line_thickness << " ";
  file << s->sett_l.color_lines.red() << " " << s->sett_l.color_lines.green()
       << " " << s->sett_l.color_lines.blue() << " "
       << s->sett_l.color_lines.alpha() << " ";

  file << s->color_background.red() << " " << s->color_background.green() << " "
       << s->color_background.blue() << " " << s->color_background.alpha()
       << " " << (s->shadeMode ? 1 : 0) << "\n";
  file.close();
}
Parameters ParserSettings::getSettingsFromFile(RenderSetting *s) {
  std::ifstream file(filename);
  Parameters p;
  std::string str;
  file >> str;
  if (str == ".") {
    p.filename = QString();
  } else {
    p.filename = QString::fromStdString(str);
  }
  file >> p.shift_x >> p.shift_y >> p.shift_z;
  file >> p.rotate_x >> p.rotate_y >> p.rotate_z;
  file >> p.scale;
  int tmp;
  file >> tmp;
  if (tmp == 0) {
    p.type_projection = ProjectionType::Parallel;
  } else {
    p.type_projection = ProjectionType::Central;
  }

  file >> tmp;
  if (tmp == 0) {
    s->sett_v.type_vertex = SettingVertex::Type::Circle;
  } else if (tmp == 1) {
    s->sett_v.type_vertex = SettingVertex::Type::Square;
  } else {
    s->sett_v.type_vertex = SettingVertex::Type::None;
  }
  file >> s->sett_v.size_vertex;
  int red, green, blue, alpha;
  file >> red >> green >> blue >> alpha;
  s->sett_v.color_vertex.setRed(red);
  s->sett_v.color_vertex.setGreen(green);
  s->sett_v.color_vertex.setBlue(blue);
  s->sett_v.color_vertex.setAlpha(alpha);

  file >> tmp;
  if (tmp == 0) {
    s->sett_l.type_lines = SettingLines::Type::Solid;
  } else if (tmp == 1) {
    s->sett_l.type_lines = SettingLines::Type::Dashed;
  } else {
    s->sett_l.type_lines = SettingLines::Type::None;
  }
  file >> s->sett_l.line_thickness;
  file >> red >> green >> blue >> alpha;
  s->sett_l.color_lines.setRed(red);
  s->sett_l.color_lines.setGreen(green);
  s->sett_l.color_lines.setBlue(blue);
  s->sett_l.color_lines.setAlpha(alpha);

  file >> red >> green >> blue >> alpha;
  s->color_background.setRed(red);
  s->color_background.setGreen(green);
  s->color_background.setBlue(blue);
  s->color_background.setAlpha(alpha);

  int shadeTmp;
  file >> shadeTmp;
  s->shadeMode = (shadeTmp != 0);
  file.close();
  return p;
}
bool ParserSettings::checkExistFile(const std::string &name) {
  return std::filesystem::exists(name);
}