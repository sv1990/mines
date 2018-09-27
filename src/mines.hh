#ifndef MINES_SRC_MINES_HH_1537684785294630186_
#define MINES_SRC_MINES_HH_1537684785294630186_

#include "gameboard.hh"
#include "timer.hh"

#include <QMainWindow>
#include <QVBoxLayout>

class mines : public QMainWindow {
  QWidget* _central_widget;
  QVBoxLayout* _layout;

  gameboard* _field;
  timer* _timer;

public:
  mines() : QMainWindow(nullptr) {
    int rows        = 16;
    int cols        = 30;
    _central_widget = new QWidget(this);
    this->setCentralWidget(_central_widget);
    this->setFixedSize(cols * 21, rows * 21);

    _layout = new QVBoxLayout(this);
    _field  = new gameboard(rows, cols, 99, this);
    _timer  = new timer(this);

    _layout->addWidget(_timer);
    _layout->addWidget(_field);

    _central_widget->setLayout(_layout);
  }
};

#endif // MINES_SRC_MINES_HH_1537684785294630186_
