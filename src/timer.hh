#ifndef MINES_SRC_TIMER_HH_1538024709907504321_
#define MINES_SRC_TIMER_HH_1538024709907504321_

#include <QLCDNumber>
#include <QString>
#include <QTimer>

class timer : public QLCDNumber {
  int _seconds = 0;
  QTimer* _timer;

public:
  explicit timer(QWidget* parent = nullptr) noexcept : QLCDNumber(parent) {
    this->setDigitCount(4);
    this->setSegmentStyle(Flat);
    _timer = new QTimer(this);
    connect(_timer, &QTimer::timeout, this, &timer::show_time);
    this->display(0);
  }
  int seconds() const noexcept { return _seconds; }

public slots:
  void show_time() {
    ++_seconds;
    this->display(_seconds);
  }
  void start() {
    _timer->start(1000);
    show_time();
  }
  void stop() { _timer->stop(); }
};

#endif // MINES_SRC_TIMER_HH_1538024709907504321_
