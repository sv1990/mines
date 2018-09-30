#ifndef MINES_SRC_BOMB_COUNT_HH_1538292750862338912_
#define MINES_SRC_BOMB_COUNT_HH_1538292750862338912_

#include <QLCDNumber>

class bomb_count : public QLCDNumber {
  int _count;

public:
  explicit bomb_count(int count, QWidget* parent = nullptr) noexcept
      : QLCDNumber(parent), _count(count) {
    this->setDigitCount(4);
    this->setSegmentStyle(Flat);
    this->display(_count);
  }

  void count_changed(int mark_change) noexcept {
    _count -= mark_change;
    this->display(_count);
  }
};

#endif // MINES_SRC_BOMB_COUNT_HH_1538292750862338912_
