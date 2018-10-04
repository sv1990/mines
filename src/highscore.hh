#ifndef MINES_SRC_HIGHSCORE_HH_1538249909205925627_
#define MINES_SRC_HIGHSCORE_HH_1538249909205925627_

#include <QWidget>

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
  int _rows;
  int _cols;
  int _bombs;

  std::string current_location() const noexcept;

public:
  explicit highscore(int rows, int cols, int bombs) noexcept;
  void add(int seconds) noexcept;
  void show() noexcept;

  void set_rows(int rows) noexcept { _rows = rows; }
  void set_cols(int cols) noexcept { _cols = cols; }
  void set_bombs(int bombs) noexcept { _bombs = bombs; }
};

#endif // MINES_SRC_HIGHSCORE_HH_1538249909205925627_
