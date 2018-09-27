#ifndef MINES_SRC_TIMER_HH_1538024709907504321_
#define MINES_SRC_TIMER_HH_1538024709907504321_

#include <QLCDNumber>
#include <QTimer>
#include <QString>

class timer : public QLCDNumber {
  int _seconds = 0;

public:
  explicit timer(QWidget* parent = nullptr) noexcept : QLCDNumber(parent) {
    this->setDigitCount(4);
    this->setSegmentStyle(Flat);

    QTimer* timer = new QTimer(this);
    connect(timer, &QTimer::timeout, this, &timer::show_time);
    timer->start(1000);

    show_time();
  }
  void show_time() {
    this->display(_seconds++);
  }
};

#endif // MINES_SRC_TIMER_HH_1538024709907504321_
