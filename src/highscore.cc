#include "highscore.hh"

#include "util/expect.hh"
#include "util/format.hh"
#include "util/make_qobject.hh"

#include <range/v3/view/enumerate.hpp>

#include <boost/archive/binary_iarchive.hpp>
#include <boost/archive/binary_oarchive.hpp>
#include <boost/serialization/set.hpp>
#include <boost/serialization/string.hpp>

#include <QDebug>
#include <QDir>
#include <QInputDialog>
#include <QLabel>
#include <QMessageBox>
#include <QPushButton>
#include <QStandardPaths>

#include <chrono>
#include <filesystem>
#include <fstream>
#include <set>
#include <sstream>

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
  EXPECT(layout);
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
  auto botton_bar    = util::make_qobject<QWidget>(this);
  auto bottom_layout = util::make_qobject<QHBoxLayout>(botton_bar);
  botton_bar->setLayout(bottom_layout);
  bottom_layout->addStretch(1);
  auto ok_button = util::make_qobject<QPushButton>(botton_bar);
  ok_button->setText("Ok");
  bottom_layout->addWidget(ok_button);
  connect(ok_button, &QPushButton::clicked, this, &highscore::close);
  _layout->addWidget(botton_bar);
}

void highscore::initialize_ui() noexcept {
  _layout = util::make_qobject<QVBoxLayout>(this);
  this->setWindowTitle("Highscores");
  this->setLayout(_layout);
  if (!empty(_scores)) {
    print_scores();
  }
  add_ok_button();
}

void highscore::print_scores() noexcept {
  for (auto [rang, score] : ranges::views::enumerate(_scores)) {
    const auto& [seconds, date, name] = score;

    auto line = util::make_qobject<QLabel>(this);

    // TODO: Improve the alignment of the seconds
    line->setText(util::format(
        "{:>4} {:>18} {:>18.2f}s {:>22}", rang + 1, name, seconds, to_date(date)
    ));
    _layout->addWidget(line);
  }
}

highscore::highscore(QWidget* parent)
    : QWidget(parent),
      _location(
          QStandardPaths::writableLocation(QStandardPaths::CacheLocation)
              .toStdString()
          + "/highscore1"
      ) {
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
  if (!empty(_scores)) {
    _first = begin(_scores)->seconds;
    if (std::size(_scores) == 10) {
      _last = prev(end(_scores))->seconds;
    }
  }
  qInfo() << "Saving highscores in" << _location.c_str();
  initialize_ui();
}

void highscore::add(double seconds) noexcept {
  if (_scores.size() < 10
      || (!empty(_scores) && seconds < prev(end(_scores))->seconds))
  {
    bool ok;
    auto name = QInputDialog::getText(
        this,
        "Highscore",
        "You are in the Top 10! What is your name?:",
        QLineEdit::Normal,
        "Anonymous",
        &ok
    );
    if (name.isEmpty()) {
      return;
    }
    auto it = _scores.insert(
        {seconds,
         std::chrono::system_clock::to_time_t(std::chrono::system_clock::now()),
         name.toStdString()}
    );
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
