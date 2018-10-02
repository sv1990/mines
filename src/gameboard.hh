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

  void resize_grid(int rows, int cols) noexcept {
    // TODO: Resize grid inplace
    delete _layout;
    _layout = new QGridLayout(this);
    for (int row = 0; row < rows; ++row) {
      for (int col = 0; col < cols; ++col) {
        _layout->addWidget(new pixmap(this, row, col), row, col);
      }
    }
    _layout->setSpacing(1);
    this->setLayout(_layout);
  }

public:
  gameboard(int rows, int cols, int num_bombs,
            QWidget* parent = nullptr) noexcept
      : QWidget(parent), _field(rows, cols, num_bombs), _layout(nullptr) {
    resize_grid(rows, cols);
  }

  pixmap* operator()(int row, int col) noexcept {
    return dynamic_cast<pixmap*>(_layout->itemAtPosition(row, col)->widget());
  }
  const pixmap* operator()(int row, int col) const noexcept {
    return const_cast<gameboard&>(*this)(row, col);
  }

  void update_pixmaps() noexcept;

  void uncover() noexcept;

  void set_rows(int rows) noexcept {
    _field.set_rows(rows);
    resize_grid(_field.rows(), _field.cols());
    reset();
  }
  void set_cols(int cols) noexcept {
    _field.set_cols(cols);
    resize_grid(_field.rows(), _field.cols());
    reset();
  };
  void set_bombs(int bombs) noexcept {
    _field.set_bombs(bombs);
    reset();
  };

  void reset() noexcept;
  void start(int row, int col) noexcept;
  void open(int row, int col) noexcept;
  void open_around(int row, int col) noexcept;
  void mark(int row, int col) noexcept;
  bool is_bomb(int row, int col) const noexcept;
  entry::state_t state(int row, int col) noexcept;
  entry::value_t value(int row, int col) noexcept;

  bool is_done() const noexcept;
  bool lost() const noexcept { return _lost; }
signals:
  void game_started();
  void game_done();
  void marks_changed(int);
  void resetted_bombs(int);
};

#endif // MINES_NEW_SRC_GAMEBOARD_HH_1537958827568077733_
