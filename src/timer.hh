#ifndef MINES_SRC_TIMER_HH_1538024709907504321_
#define MINES_SRC_TIMER_HH_1538024709907504321_

#include <QLabel>
#include <QTimer>
#include <QString>

class timer : public QLabel {
  int _seconds = 0;

public:
  explicit timer(QWidget* parent = nullptr) noexcept : QLabel(parent) {
    QTimer* timer = new QTimer(this);
    connect(timer, &QTimer::timeout, this, &timer::show_time);
    timer->start(1000);

    show_time();

    setWindowTitle(tr("Digital Clock"));
    resize(150, 60);
  }
  void show_time() {
    this->setText(QString::number(_seconds++).leftJustified(4, ' '));
  }
};

#endif // MINES_SRC_TIMER_HH_1538024709907504321_
