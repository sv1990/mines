#ifndef MINES_SRC_MINES_HH_1537684785294630186_
#define MINES_SRC_MINES_HH_1537684785294630186_

#include "bomb_count.hh"
#include "gameboard.hh"
#include "highscore.hh"
#include "timer.hh"

#include <QMainWindow>
#include <QVBoxLayout>

class mines : public QMainWindow {
  QWidget* _central_widget;
  QVBoxLayout* _layout;

  gameboard* _board;
  timer* _timer;
  highscore* _highscore;
  bomb_count* _bomb_count;

public:
  mines() noexcept;
  void show_highscore() noexcept;
};

#endif // MINES_SRC_MINES_HH_1537684785294630186_
