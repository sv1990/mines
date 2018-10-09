#include "highscore.hh"

#include <fmt/format.h>

#include <boost/archive/binary_iarchive.hpp>
#include <boost/archive/binary_oarchive.hpp>
#include <boost/serialization/set.hpp>
#include <boost/serialization/string.hpp>

#include <QDir>
#include <QInputDialog>
#include <QLabel>
#include <QMessageBox>
#include <QPushButton>
#include <QStandardPaths>

#include <chrono>
#include <filesystem>
#include <fstream>
#include <iomanip>
#include <set>
#include <sstream>
#include <tuple>

#include <cassert>

std::multiset<score> load_highscore(std::istream& is) {
  boost::archive::binary_iarchive ia{is};
  std::multiset<score> scores;
  ia >> scores;
  return scores;
}
void save_highscore(std::ostream& os, const std::multiset<score>& scores) {
  boost::archive::binary_oarchive oa{os};
  oa << scores;
}

std::string to_date(std::time_t seconds_since_epoch) noexcept {
  std::ostringstream oss;
  std::tm tm = *std::localtime(&seconds_since_epoch);
  oss << std::put_time(&tm, "%d.%m.%y %H:%M:%S");
  return std::move(oss).str();
}

void clear_layout(QLayout* layout) noexcept {
  assert(layout);
  QLayoutItem* item;
  while ((item = layout->takeAt(0))) {
    auto widget = item->widget();
    if (widget) {
      widget->deleteLater();
    }
    layout->removeItem(item);
  }
}

bool operator<(const score& lhs, const score& rhs) noexcept {
  return std::tie(lhs.seconds, lhs.date) < std::tie(rhs.seconds, rhs.date);
}

void highscore::add_ok_button() noexcept {
  auto botton_bar    = new QWidget(this);
  auto bottom_layout = new QHBoxLayout(botton_bar);
  botton_bar->setLayout(bottom_layout);
  bottom_layout->addStretch(1);
  auto ok_button = new QPushButton(botton_bar);
  ok_button->setText("Ok");
  bottom_layout->addWidget(ok_button);
  connect(ok_button, &QPushButton::clicked, this, &highscore::close);
  _layout->addWidget(botton_bar);
}

void highscore::initialize_ui() noexcept {
  _layout = new QVBoxLayout(this);
  this->setWindowTitle("Highscores");
  this->setLayout(_layout);
  if (!empty(_scores)) {
    print_scores();
  }
  add_ok_button();
  this->setSizePolicy(QSizePolicy::Maximum, QSizePolicy::Maximum);
}

void highscore::print_scores() noexcept {
  int rang = 1;
  for (const auto& [seconds, date, name] : _scores) {
    auto line = new QLabel(this);

    line->setText(QString::fromStdString(fmt::format(
        "{:>4} {:>18} {:>14}s {:>22}", rang++, name, seconds, to_date(date))));
    _layout->addWidget(line);
  }
}

highscore::highscore(QWidget* parent) noexcept
    : QWidget(parent),
      _location(QStandardPaths::writableLocation(QStandardPaths::CacheLocation)
                    .toStdString() +
                "/highscore") {
  const auto dir =
      QStandardPaths::writableLocation(QStandardPaths::CacheLocation)
          .toStdString();
  if (!std::filesystem::exists(dir)) {
    std::filesystem::create_directories(dir);
  }
  if (std::filesystem::exists(_location)) {
    std::ifstream ifs{_location};
    _scores = load_highscore(ifs);
  }
  initialize_ui();
  if (empty(_scores)) {
    return;
  }
  _first = begin(_scores)->seconds;
  if (std::size(_scores) == 10) {
    _last = prev(end(_scores))->seconds;
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
    auto it = _scores.insert(
        {seconds,
         std::chrono::system_clock::to_time_t(std::chrono::system_clock::now()),
         name.toStdString()});
    auto pos = std::distance(begin(_scores), it);
    if (_scores.size() > 10) {
      _scores.erase(prev(end(_scores)));
    }
    if (pos <= 10) {
      clear_layout(_layout);
      print_scores();
      add_ok_button();
    }
  }
}

void highscore::show() noexcept {
  if (!empty(_scores)) {
    this->QWidget::show();
    std::ofstream ofs(_location);
    save_highscore(ofs, _scores);
  }
  else {
    QMessageBox::information(this, "Highscore", "No highscores yet!");
  }
}
