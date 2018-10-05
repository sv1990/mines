#ifndef MINES_SRC_MINES_HH_1537684785294630186_
#define MINES_SRC_MINES_HH_1537684785294630186_

#include "bomb_count.hh"
#include "gameboard.hh"
#include "highscore.hh"
#include "timer.hh"

#include <vector>

#include <QComboBox>
#include <QMainWindow>
#include <QVBoxLayout>

class mines : public QMainWindow {
  struct difficulty {
    int rows;
    int cols;
    int bombs;
  };
  static const std::vector<difficulty> _difficulties;

  QWidget* _central_widget;
  QVBoxLayout* _layout;
  QComboBox* _difficulty_box;

  gameboard* _board;
  timer* _timer;
  highscore* _highscore;
  bomb_count* _bomb_count;

public:
  mines() noexcept;
  void show_highscore() noexcept;
  void add_highscore() noexcept;

  void change_difficulty(int index);
  void restart();

  void set_rows(int rows);
  void set_cols(int cols);
  void set_bombs(int bombs);
};

#endif // MINES_SRC_MINES_HH_1537684785294630186_
