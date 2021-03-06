#ifndef MINES_NEW_SRC_PIXMAP_HH_1537958690672118070_
#define MINES_NEW_SRC_PIXMAP_HH_1537958690672118070_

#include "util/overloaded.hh"

#include <QLabel>
#include <QMouseEvent>

class gameboard;

/**
 * One square of the minesweeper gameboard
 */
class pixmap : public QLabel {
  Q_OBJECT
  gameboard* _board;
  int _row;
  int _col;

public:
  pixmap(gameboard* field, int row, int col) noexcept;

  void show_marked_empty() noexcept;
  void show_marked_bomb() noexcept;
  void show_bomb() noexcept;
  void uncover() noexcept;

  void update_pixmap();

  void mousePressEvent(QMouseEvent* event) override;
};

#endif // MINES_NEW_SRC_PIXMAP_HH_1537958690672118070_
