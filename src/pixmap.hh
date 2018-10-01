#ifndef MINES_NEW_SRC_PIXMAP_HH_1537958690672118070_
#define MINES_NEW_SRC_PIXMAP_HH_1537958690672118070_

#include "util/overloaded.hh"

#include <QLabel>
#include <QMouseEvent>

#include <variant>

class gameboard;

class pixmap : public QLabel {
  gameboard* _board;
  std::size_t _row;
  std::size_t _col;

public:
  pixmap(gameboard* field, std::size_t row, std::size_t col) noexcept;

  void show_marked_empty() noexcept;
  void show_marked_bomb() noexcept;
  void show_bomb() noexcept;
  void uncover() noexcept;

  void update_pixmap() noexcept;

  void mousePressEvent(QMouseEvent* event) override;
};

#endif // MINES_NEW_SRC_PIXMAP_HH_1537958690672118070_
