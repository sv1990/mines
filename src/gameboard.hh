#ifndef MINES_NEW_SRC_GAMEBOARD_HH_1537958827568077733_
#define MINES_NEW_SRC_GAMEBOARD_HH_1537958827568077733_

#include "entry.hh"
#include "field.hh"
#include "pixmap.hh"

#include <QGridLayout>
#include <QWidget>

#include <vector>

class gameboard : public QWidget {
  Q_OBJECT
  field _field;
  QGridLayout* _layout;
  bool _lost    = false;
  bool _started = false;

public:
  gameboard(std::size_t rows, std::size_t cols, std::size_t num_bombs,
            QWidget* parent = nullptr) noexcept
      : QWidget(parent), _field(rows, cols, num_bombs) {
    _layout = new QGridLayout(this);
    for (std::size_t row = 0; row < rows; ++row) {
      for (std::size_t col = 0; col < cols; ++col) {
        _layout->addWidget(new pixmap(this, row, col), row, col);
      }
    }
    _layout->setSpacing(1);
    this->setLayout(_layout);
  }

  pixmap* operator()(std::size_t row, std::size_t col) noexcept {
    return dynamic_cast<pixmap*>(_layout->itemAtPosition(row, col)->widget());
  }
  const pixmap* operator()(std::size_t row, std::size_t col) const noexcept {
    return const_cast<gameboard&>(*this)(row, col);
  }

  void update_pixmaps() noexcept;

  void uncover() noexcept;

  void start() noexcept;
  void open(std::size_t row, std::size_t col) noexcept;
  void open_around(std::size_t row, std::size_t col) noexcept;
  void mark(std::size_t row, std::size_t col) noexcept;
  bool is_bomb(std::size_t row, std::size_t col) const noexcept;
  entry::state_t state(std::size_t row, std::size_t col) noexcept;
  entry::value_t value(std::size_t row, std::size_t col) noexcept;

  bool is_done() const noexcept;
  bool lost() const noexcept { return _lost; }
signals:
  void game_started();
  void game_done();
  void marks_changed(int);
};

#endif // MINES_NEW_SRC_GAMEBOARD_HH_1537958827568077733_
