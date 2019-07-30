#include "timer.hh"

timer::timer(std::optional<int> first, std::optional<int> last,
             QWidget* parent) noexcept
    : QLCDNumber(parent), _first(first), _last(last) {
  this->setDigitCount(4);
  this->setSegmentStyle(Flat);
  _timer = new QTimer(this);
  connect(_timer, &QTimer::timeout, this, &timer::show_time);
  set_text_color(Qt::green);
  this->display(0);
  this->setToolTip("Green: Highscore\nYellow: Top 10\nRed: Below Top 10");
}
int timer::seconds() const noexcept {
  return _seconds;
}

void timer::show_time() {
  ++_seconds;
  if (_last.has_value() && _seconds >= _last.value()) {
    set_text_color(Qt::red);
  }
  else if (_first.has_value() && _seconds >= _first.value()) {
    set_text_color(Qt::yellow);
  }
  this->display(_seconds);
}
void timer::start() {
  _timer->start(1000);
  show_time();
}
void timer::reset() {
  stop();
  _seconds = 0;
  set_text_color(Qt::green);
  this->display(_seconds);
}
void timer::stop() {
  _timer->stop();
}

void timer::set_text_color(const QColor& color) noexcept {
  auto palette = this->palette();
  palette.setColor(QPalette::WindowText, color);
  this->setPalette(palette);
}
