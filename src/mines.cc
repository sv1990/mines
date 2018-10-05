#include "mines.hh"

#include <QHBoxLayout>
#include <QPushButton>
#include <QSpinBox>

const std::vector<difficulty> mines::_difficulties{
    {8, 8, 10, 200, 280},   // Easy
    {16, 16, 40, 390, 410}, // Intermediate
    {19, 30, 99, 600, 470}, // Hard
};

mines::mines() noexcept : QMainWindow(nullptr) {
  const int initial_difficulty_index = 2;

  auto [rows, cols, num_bombs, width, height] =
      _difficulties[initial_difficulty_index];

  _board      = new gameboard(rows, cols, num_bombs, this);
  _timer      = new timer(this);
  _bomb_count = new bomb_count(num_bombs, this);

  auto top_bar    = new QWidget(this);
  auto top_layout = new QHBoxLayout(top_bar);
  top_bar->setLayout(top_layout);

  _difficulty_box = new QComboBox(this);
  _difficulty_box->addItem("Easy");
  _difficulty_box->addItem("Intermediate");
  _difficulty_box->addItem("Hard");
  _difficulty_box->setCurrentIndex(initial_difficulty_index);

  _highscore = new highscore(_difficulty_box->currentText().toStdString());

  top_layout->addWidget(_difficulty_box);
  top_layout->addWidget(_timer);
  top_layout->addWidget(_bomb_count);

  _central_widget = new QWidget(this);
  _layout         = new QVBoxLayout(_central_widget);
  _layout->addWidget(top_bar);
  _layout->addWidget(_board);
  _central_widget->setLayout(_layout);

  auto bottom_bar    = new QWidget(this);
  auto bottom_layout = new QHBoxLayout(bottom_bar);
  // bottom_layout->addSpacing(256);
  auto highscore_button = new QPushButton(this);
  highscore_button->setText("Show Highscore");
  bottom_layout->addWidget(highscore_button);
  auto restart_button = new QPushButton(this);
  restart_button->setText("Restart");
  bottom_layout->addWidget(restart_button);
  _layout->addWidget(bottom_bar);

  this->setCentralWidget(_central_widget);
  this->setFixedSize(width, height);

  connect(_board, &gameboard::game_started, _timer, &timer::start);
  connect(_board, &gameboard::game_done, _timer, &timer::stop);
  connect(_board, &gameboard::game_done, this, &mines::add_highscore);
  connect(_board, &gameboard::marks_changed, _bomb_count,
          &bomb_count::count_changed);
  connect(highscore_button, &QPushButton::clicked, this,
          &mines::show_highscore);
  connect(restart_button, &QPushButton::clicked, this, &mines::restart);
  connect(_difficulty_box, QOverload<int>::of(&QComboBox::currentIndexChanged),
          this, &mines::change_difficulty);
}

void mines::show_highscore() noexcept {
  _highscore->show();
}

void mines::add_highscore() noexcept {
  if (_board->is_done() && !_board->lost()) {
    _highscore->add(_timer->seconds());
    _highscore->show();
  }
}

void mines::change_difficulty() {
  auto [rows, cols, num_bombs, width, height] =
      _difficulties[static_cast<std::size_t>(_difficulty_box->currentIndex())];
  set_rows(rows);
  set_cols(cols);
  set_bombs(num_bombs);
  _highscore->change_difficulty(_difficulty_box->currentText().toStdString());
  this->setFixedSize(width, height);
  restart();
}

void mines::restart() {
  _board->reset();
  _timer->reset();
  auto [rows, cols, bombs, widht, height] =
      _difficulties[static_cast<std::size_t>(_difficulty_box->currentIndex())];
  _bomb_count->restart(bombs);
  connect(_board, &gameboard::resetted_bombs, _bomb_count,
          &bomb_count::restart);
}

void mines::set_rows(int rows) {
  _board->set_rows(rows);
}
void mines::set_cols(int cols) {
  _board->set_cols(cols);
}
void mines::set_bombs(int bombs) {
  _board->set_bombs(bombs);
}
