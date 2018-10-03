#include "highscore.hh"

#include <fmt/format.h>

#include <QDir>
#include <QInputDialog>
#include <QLabel>
#include <QMessageBox>
#include <QPushButton>
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
  this->setWindowTitle("Highscores");
  this->setLayout(layout);
  auto title = new QLabel(this);
  title->setText("Highscores");
  int rang = 1;
  layout->addStretch(1);
  for (const auto& [seconds, date, name] : scores) {
    auto line = new QLabel(this);

    line->setText(QString::fromStdString(fmt::format(
        "{:>4} {:>18} {:>14}s {:>22}", rang++, name, seconds, to_date(date))));
    layout->addWidget(line);
  }
  layout->addStretch(8);
  auto botton_bar    = new QWidget(this);
  auto bottom_layout = new QHBoxLayout(botton_bar);
  botton_bar->setLayout(bottom_layout);
  bottom_layout->addStretch(1);
  auto ok_button = new QPushButton(botton_bar);
  ok_button->setText("Ok");
  bottom_layout->addWidget(ok_button);
  connect(ok_button, &QPushButton::clicked, this, &highscorelist::close);
  layout->addWidget(botton_bar);
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
