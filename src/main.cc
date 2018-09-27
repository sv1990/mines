#include "mines.hh"

#include <QApplication>

int main(int argc, char* argv[]) {
  QApplication a(argc, argv);
  mines m;
  m.show();
  return a.exec();
}
