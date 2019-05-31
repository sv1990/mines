#ifndef MINES_SRC_BOMB_COUNT_HH_1538292750862338912_
#define MINES_SRC_BOMB_COUNT_HH_1538292750862338912_

#include <QLCDNumber>

/**
 * Displays remaining number of bombs
 */
class bomb_count : public QLCDNumber {
  Q_OBJECT
  int _count;

public:
  explicit bomb_count(int count, QWidget* parent = nullptr) noexcept;
  void set_text_color(const QColor& color) noexcept;
  void count_changed(int mark_change) noexcept;

public slots:
  void restart(int count);
};

#endif // MINES_SRC_BOMB_COUNT_HH_1538292750862338912_
