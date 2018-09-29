#ifndef MINES_SRC_HIGHSCORE_HH_1538249909205925627_
#define MINES_SRC_HIGHSCORE_HH_1538249909205925627_

#include <QDir>
#include <QInputDialog>
#include <QLabel>
#include <QStandardPaths>
#include <QVBoxLayout>
#include <QWidget>

#include <set>
#include <string>

struct score {
  int seconds;
  long date;
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

public:
  explicit highscore() noexcept;
  void add(int seconds) noexcept;
  void show() const noexcept;
};

#endif // MINES_SRC_HIGHSCORE_HH_1538249909205925627_
