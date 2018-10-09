#ifndef MINES_SRC_HIGHSCORE_HH_1538249909205925627_
#define MINES_SRC_HIGHSCORE_HH_1538249909205925627_

#include <QVBoxLayout>
#include <QWidget>

#include <optional>
#include <set>
#include <string>

#include <ctime>

struct score {
  int seconds;
  std::time_t date;
  std::string name;

  template <class Archive>
  void serialize(Archive& ar, const unsigned int /*version*/) {
    // clang-format off
    ar & seconds;
    ar & date;
    ar & name;
    // clang-format on
  }
};

bool operator<(const score& lhs, const score& rhs) noexcept;

class highscore : public QWidget {
  std::string _location;
  std::multiset<score> _scores;
  std::optional<int> _first = std::nullopt;
  std::optional<int> _last  = std::nullopt;

  QVBoxLayout* _layout;

  void initialize_ui() noexcept;
  void print_scores() noexcept;
  void add_ok_button() noexcept;

public:
  explicit highscore(QWidget* parent = nullptr) noexcept;
  void add(int seconds) noexcept;
  void show() noexcept;
  std::optional<int> first() const noexcept { return _first; }
  std::optional<int> last() const noexcept { return _last; }
};

#endif // MINES_SRC_HIGHSCORE_HH_1538249909205925627_
