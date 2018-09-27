#ifndef MINES_SRC_MINES_HH_1537684785294630186_
#define MINES_SRC_MINES_HH_1537684785294630186_

#include "gameboard.hh"

#include <QMainWindow>

class mines : public QMainWindow {
  gameboard* _field;

public:
  mines() : QMainWindow(nullptr) {
    int rows = 16;
    int cols = 30;
    _field   = new gameboard(rows, cols, 99, this);
    this->setCentralWidget(_field);
    this->setFixedSize(cols * 18, rows * 18);
  }
};

#endif // MINES_SRC_MINES_HH_1537684785294630186_
