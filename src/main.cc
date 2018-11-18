#include "mines.hh"

#include <QApplication>
#include <QMessageBox>

int main(int argc, char* argv[]) {
  QApplication a(argc, argv);
  try {
    mines m;
    m.show();
  }
  catch (std::exception& e) {
    QMessageBox exception_msg_;
    exception_msg_.setText("An exception occurred:");
    exception_msg_.setInformativeText(e.what());
    exception_msg_.exec();
  }
  return a.exec();
}
