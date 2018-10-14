#ifndef MINES_SRC_TIMER_HH_1538024709907504321_
#define MINES_SRC_TIMER_HH_1538024709907504321_

#include <QLCDNumber>
#include <QString>
#include <QTimer>

#include <optional>

class timer : public QLCDNumber {
  int _seconds = 0;
  QTimer* _timer;
  std::optional<int> _first = std::nullopt;
  std::optional<int> _last  = std::nullopt;

public:
  explicit timer(std::optional<int> first, std::optional<int> last,
                 QWidget* parent = nullptr) noexcept
      : QLCDNumber(parent), _first(first), _last(last) {
    this->setDigitCount(4);
    this->setSegmentStyle(Flat);
    _timer = new QTimer(this);
    connect(_timer, &QTimer::timeout, this, &timer::show_time);
    set_text_color(Qt::green);
    this->display(0);
    this->setToolTip("Green: Highscore\nYellow: Top 10\nRed: Below Top 10");
  }
  int seconds() const noexcept { return _seconds; }

public slots:
  void show_time() {
    ++_seconds;
    if (_last.has_value() && _seconds >= _last.value()) {
      set_text_color(Qt::red);
    }
    if (_first.has_value() && _seconds >= _first.value()) {
      set_text_color(Qt::yellow);
    }
    this->display(_seconds);
  }
  void start() {
    _timer->start(1000);
    show_time();
  }
  void reset() {
    stop();
    _seconds = 0;
    set_text_color(Qt::green);
    this->display(_seconds);
  }
  void stop() { _timer->stop(); }

  void set_text_color(QColor color) noexcept {
    auto palette = this->palette();
    palette.setColor(QPalette::Foreground, color);
    this->setPalette(palette);
  }
};

#endif // MINES_SRC_TIMER_HH_1538024709907504321_
