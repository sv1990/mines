#include "highscore.hh"

#include <chrono>
#include <filesystem>
#include <fstream>
#include <iomanip>
#include <set>
#include <tuple>

bool operator<(const score& lhs, const score& rhs) noexcept {
  return std::tie(lhs.seconds, lhs.date) < std::tie(rhs.seconds, rhs.date);
}

highscorelist::highscorelist(const std::multiset<score>& scores,
                             QWidget* parent) noexcept
    : QWidget(parent) {
  QVBoxLayout* layout = new QVBoxLayout(this);
  this->setLayout(layout);
  auto title = new QLabel(this);
  title->setText("Highscores");
  int rang = 1;
  for (const auto& [seconds, date, name] : scores) {
    auto line = new QLabel(this);
    line->setText(QString("%1 %2 %3s")
                      .arg(rang++)
                      .arg(QString::fromStdString(name))
                      .arg(seconds));
    layout->addWidget(line);
  }
}

highscore::highscore() noexcept
    : _location(QStandardPaths::writableLocation(QStandardPaths::CacheLocation)
                    .toStdString() +
                "/highscore") {
  const auto dir =
      QStandardPaths::writableLocation(QStandardPaths::CacheLocation)
          .toStdString();
  if (!std::filesystem::exists(dir)) {
    std::filesystem::create_directories(dir);
  }
  std::ifstream ifs{_location};
  for (auto [sec, date, name] = std::tuple<int, long, std::string>{};
       ifs >> sec >> date >> std::quoted(name);) {
    _scores.insert({sec, date, name});
  }
}

void highscore::add(int seconds) noexcept {
  if (_scores.size() < 10 ||
      (!empty(_scores) && seconds < prev(end(_scores))->seconds)) {
    bool ok;
    auto name = QInputDialog::getText(
        this, "Highscore",
        "You are in the Top 10! What is your name?:", QLineEdit::Normal,
        "Anonymous", &ok);
    _scores.insert({seconds,
                    std::chrono::system_clock::now().time_since_epoch().count(),
                    name.toStdString()});
    if (_scores.size() > 10) {
      _scores.erase(prev(end(_scores)));
    }
  }
}

void highscore::show() const noexcept {
  if (!empty(_scores)) {
    auto h = new highscorelist(_scores);
    h->setFixedSize(320, 320);
    h->show();
    std::ofstream ofs(_location);
    for (const auto& [seconds, date, name] : _scores) {
      ofs << seconds << ' ' << date << ' ' << std::quoted(name) << '\n';
    }
  }
}