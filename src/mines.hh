#ifndef MINES_SRC_MINES_HH_1537684785294630186_
#define MINES_SRC_MINES_HH_1537684785294630186_

#include "bomb_count.hh"
#include "gameboard.hh"
#include "highscore.hh"
#include "timer.hh"

#include <QKeySequence>
#include <QMainWindow>
#include <QShortcut>
#include <QVBoxLayout>

class mines : public QMainWindow {
  Q_OBJECT
  QWidget* _central_widget;
  QVBoxLayout* _layout;
  QShortcut* _cheat;

  gameboard* _board;
  timer* _timer;
  highscore* _highscore;
  bomb_count* _bomb_count;

public:
  mines();
  void show_highscore();
  void add_highscore();
  virtual ~mines() {
    if (_highscore) {
      delete _highscore;
    }
  }
};

#endif // MINES_SRC_MINES_HH_1537684785294630186_
