#ifndef MINES_NEW_SRC_GAMEBOARD_HH_1537958827568077733_
#define MINES_NEW_SRC_GAMEBOARD_HH_1537958827568077733_

#include "entry.hh"
#include "field.hh"
#include "pixmap.hh"

#include "util/make_qobject.hh"

#include <QGridLayout>
#include <QWidget>

#include <vector>

class gameboard : public QWidget {
  Q_OBJECT
  field _field;
  QGridLayout* _layout;
  bool _lost    = false;
  bool _started = false;
  bool _cheated = false;

public:
  gameboard(int rows, int cols, int num_bombs,
            QWidget* parent = nullptr) noexcept
      : QWidget(parent), _field(rows, cols, num_bombs) {
    _layout = util::make_qobject<QGridLayout>(this);
    for (int row = 0; row < rows; ++row) {
      for (int col = 0; col < cols; ++col) {
        _layout->addWidget(util::make_qobject<pixmap>(this, row, col), row,
                           col);
      }
    }
    _layout->setSpacing(1);
    this->setLayout(_layout);
  }

private:
  template <typename Self>
  static auto* get_pixmap_at(Self& self, int row, int col) noexcept {
    return dynamic_cast<pixmap*>(
        self._layout->itemAtPosition(row, col)->widget());
  }

public:
  pixmap* operator()(int row, int col) noexcept {
    return get_pixmap_at(*this, row, col);
  }
  const pixmap* operator()(int row, int col) const noexcept {
    return get_pixmap_at(*this, row, col);
  }

  void update_pixmaps() noexcept;

  /**
   * Uncover all bombs
   */
  void uncover() noexcept;

  void reset() noexcept;
  /**
   * Start a new game with a click on (row, col)
   *
   */
  void start(int row, int col) noexcept;
  void open(int row, int col) noexcept;
  void open_around(int row, int col) noexcept;
  void open_around_all_numbers() noexcept;
  void mark(int row, int col) noexcept;
  bool is_bomb(int row, int col) const noexcept;
  entry::state_t state(int row, int col) noexcept;
  entry::value_t value(int row, int col) noexcept;

  bool is_finished() const noexcept;
  bool is_lost() const noexcept { return _lost; }
  bool is_active() const noexcept { return !is_lost() && !is_finished(); }
  bool cheated() const noexcept { return _cheated; }
signals:
  void game_started();
  void game_done();
  void marks_changed(int);
  void resetted_bombs(int);
};

#endif // MINES_NEW_SRC_GAMEBOARD_HH_1537958827568077733_
