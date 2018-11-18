#include "mines.hh"

#include <QApplication>
#include <QMessageBox>

int main(int argc, char* argv[]) {
  try {
    QApplication a(argc, argv);
    mines m;
    m.show();
    return a.exec();
  }
  catch (std::exception& e) {
    QApplication a(argc, argv);
    QMessageBox exception_msg_;
    exception_msg_.setText("An exception occurred:");
    exception_msg_.setInformativeText(e.what());
    exception_msg_.exec();
    return a.exec();
  }
}
