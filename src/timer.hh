#ifndef MINES_SRC_TIMER_HH_1538024709907504321_
#define MINES_SRC_TIMER_HH_1538024709907504321_

#include <QLCDNumber>
#include <QString>
#include <QTimer>

#include <optional>

/**
 * Shows the elapsed time
 */
class timer : public QLCDNumber {
  int _seconds = 0;
  QTimer* _timer;
  std::optional<int> _first = std::nullopt;
  std::optional<int> _last  = std::nullopt;

public:
  explicit timer(std::optional<int> first, std::optional<int> last,
                 QWidget* parent = nullptr) noexcept;
  int seconds() const noexcept;

public slots:
  void show_time();
  void start();
  void reset();
  void stop();
  void set_text_color(QColor color) noexcept;
};

#endif // MINES_SRC_TIMER_HH_1538024709907504321_
