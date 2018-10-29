#include "bomb_count.hh"

bomb_count::bomb_count(int count, QWidget* parent) noexcept
    : QLCDNumber(parent), _count(count) {
  this->setDigitCount(4);
  this->setSegmentStyle(Flat);
  this->display(_count);
  set_text_color(Qt::white);
  this->setToolTip("Remaining Bombs");
}

void bomb_count::set_text_color(QColor color) noexcept {
  auto palette = this->palette();
  palette.setColor(QPalette::Foreground, color);
  this->setPalette(palette);
}

void bomb_count::count_changed(int mark_change) noexcept {
  _count -= mark_change;
  this->display(_count);
}

void bomb_count::restart(int count) {
  _count = count;
  set_text_color(Qt::white);
  this->display(_count);
}
