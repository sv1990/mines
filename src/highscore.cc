#include "highscore.hh"

#include <fmt/format.h>

#include <QDir>
#include <QInputDialog>
#include <QLabel>
#include <QMessageBox>
#include <QStandardPaths>
#include <QVBoxLayout>

#include <chrono>
#include <filesystem>
#include <fstream>
#include <iomanip>
#include <set>
#include <sstream>
#include <tuple>

#include <iostream>

std::string to_date(std::time_t seconds) noexcept {
  std::ostringstream oss;
  std::tm tm = *std::localtime(&seconds);
  oss << std::put_time(&tm, "%d.%m.%y %H:%M:%S");
  return std::move(oss).str();
}

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

    line->setText(QString::fromStdString(fmt::format(
        "{:>4} {:>18} {:>14}s {:>22}", rang++, name, seconds, to_date(date))));
    layout->addWidget(line);
  }
}

highscore::highscore(int rows, int cols, int bombs) noexcept
    : _location(QStandardPaths::writableLocation(QStandardPaths::CacheLocation)
                    .toStdString() +
                "/highscore"),
      _rows(rows), _cols(cols), _bombs(bombs) {
  const auto dir =
      QStandardPaths::writableLocation(QStandardPaths::CacheLocation)
          .toStdString();
  if (!std::filesystem::exists(dir)) {
    std::filesystem::create_directories(dir);
  }
}

std::string highscore::current_location() const noexcept {
  return fmt::format("{}.{}.{}.{}", _location, _rows, _cols, _bombs);
}

void highscore::add(int seconds) noexcept {
  std::ifstream ifs{current_location()};
  for (auto [sec, date, name] = std::tuple<int, long, std::string>{};
       ifs >> sec >> date >> std::quoted(name);) {
    _scores.insert({sec, date, name});
  }
  if (_scores.size() < 10 ||
      (!empty(_scores) && seconds < prev(end(_scores))->seconds)) {
    bool ok;
    auto name = QInputDialog::getText(
        this, "Highscore",
        "You are in the Top 10! What is your name?:", QLineEdit::Normal,
        "Anonymous", &ok);
    _scores.insert(
        {seconds,
         std::chrono::system_clock::to_time_t(std::chrono::system_clock::now()),
         name.toStdString()});
    if (_scores.size() > 10) {
      _scores.erase(prev(end(_scores)));
    }
  }
}

void highscore::show() noexcept {
  if (empty(_scores)) {
    std::ifstream ifs{current_location()};
    for (auto [sec, date, name] = std::tuple<int, long, std::string>{};
         ifs >> sec >> date >> std::quoted(name);) {
      _scores.insert({sec, date, name});
    }
  }
  if (!empty(_scores)) {
    auto h = new highscorelist(_scores);
    h->setFixedSize(320, 320);
    h->show();
    std::ofstream ofs(_location);
    for (const auto& [seconds, date, name] : _scores) {
      ofs << seconds << ' ' << date << ' ' << std::quoted(name) << '\n';
    }
  }
  else {
    QMessageBox::information(this, "Highscore", "No highscores yet!");
  }
}
