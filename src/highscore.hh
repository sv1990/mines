#ifndef MINES_SRC_HIGHSCORE_HH_1538249909205925627_
#define MINES_SRC_HIGHSCORE_HH_1538249909205925627_

#include <QWidget>

#include <optional>
#include <set>
#include <string>

#include <ctime>

struct score {
  int seconds;
  std::time_t date;
  std::string name;
};

bool operator<(const score& lhs, const score& rhs) noexcept;

class highscorelist : public QWidget {
public:
  explicit highscorelist(const std::multiset<score>& scores,
                         QWidget* parent = nullptr) noexcept;
};

class highscore : QWidget {
  std::string _location;
  std::multiset<score> _scores;
  std::optional<int> _first = std::nullopt;
  std::optional<int> _last  = std::nullopt;

public:
  explicit highscore() noexcept;
  void add(int seconds) noexcept;
  void show() noexcept;
  std::optional<int> first() const noexcept { return _first; }
  std::optional<int> last() const noexcept { return _last; }
};

#endif // MINES_SRC_HIGHSCORE_HH_1538249909205925627_
