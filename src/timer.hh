#ifndef MINES_SRC_TIMER_HH_1538024709907504321_
#define MINES_SRC_TIMER_HH_1538024709907504321_

#include <QLCDNumber>
#include <QString>
#include <QTimer>

#include <chrono>
#include <optional>

/**
 * Shows the elapsed time
 */
class timer : public QLCDNumber {
  Q_OBJECT
  QTimer* _timer;
  std::optional<double> _first = std::nullopt;
  std::optional<double> _last  = std::nullopt;
  std::chrono::high_resolution_clock::time_point _t_start;
  std::chrono::high_resolution_clock::time_point _t_stop;

public:
  explicit timer(std::optional<int> first, std::optional<int> last,
                 QWidget* parent = nullptr) noexcept;
  double seconds() const noexcept;

public slots:
  void show_time();
  void start();
  void reset();
  void stop();
  void set_text_color(const QColor& color) noexcept;
};

#endif // MINES_SRC_TIMER_HH_1538024709907504321_
