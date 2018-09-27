#ifndef MINES_NEW_SRC_PIXMAP_HH_1537958690672118070_
#define MINES_NEW_SRC_PIXMAP_HH_1537958690672118070_

#include "util/overloaded.hh"

#include <QLabel>
#include <QMouseEvent>

#include <iostream>
#include <variant>

class gameboard;

inline QPixmap make_pixmap(const char* filepath) noexcept {
  return QPixmap(filepath).scaled(16, 16, Qt::KeepAspectRatio,
                                  Qt::SmoothTransformation);
}

class pixmap : public QLabel {
  gameboard* _board;
  std::size_t _row;
  std::size_t _col;

public:
  pixmap(gameboard* field, std::size_t row, std::size_t col) noexcept;

  void update_pixmap() noexcept;

  void mousePressEvent(QMouseEvent* event) override;
};

#endif // MINES_NEW_SRC_PIXMAP_HH_1537958690672118070_
