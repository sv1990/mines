#include "timer.hh"

#include "util/format.hh"
#include "util/make_qobject.hh"

timer::timer(
    std::optional<int> first, std::optional<int> last, QWidget* parent
) noexcept
    : QLCDNumber(parent), _first(first), _last(last) {
  this->setDigitCount(4);
  this->setSegmentStyle(Flat);
  _timer = util::make_qobject<QTimer>(this);
  connect(_timer, &QTimer::timeout, this, &timer::show_time);
  set_text_color(Qt::green);
  this->display(0);
  this->setToolTip("Green: Highscore\nYellow: Top 10\nRed: Below Top 10");
}

double timer::seconds() const noexcept {
  return static_cast<double>(
             std::chrono::duration_cast<std::chrono::milliseconds>(
                 _t_stop - _t_start
             )
                 .count()
         )
         / 1000.;
}

void timer::show_time() {
  const auto now = std::chrono::high_resolution_clock::now();
  const auto dur =
      static_cast<double>(
          std::chrono::duration_cast<std::chrono::milliseconds>(now - _t_start)
              .count()
      )
      / 1000.;
  if (_last.has_value() && dur >= _last.value()) {
    set_text_color(Qt::red);
  }
  else if (_first.has_value() && dur >= _first.value()) {
    set_text_color(Qt::yellow);
  }
  this->display(util::format("{:.0f}", dur));
}

void timer::start() {
  _timer->start(100);
  _t_start = std::chrono::high_resolution_clock::now();
  show_time();
}

void timer::reset() {
  stop();
  _t_start = _t_stop = std::chrono::high_resolution_clock::now();
  set_text_color(Qt::green);
  this->display(0);
}

void timer::stop() {
  _t_stop = std::chrono::high_resolution_clock::now();
  _timer->stop();
}

void timer::set_text_color(const QColor& color) noexcept {
  auto palette = this->palette();
  palette.setColor(QPalette::WindowText, color);
  this->setPalette(palette);
}
