#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include "serialport.h"

#include <QMainWindow>
#include <QLabel>
#include <QDebug>
#include <QMessageBox>
#include <QShortcut>
#include <sstream>
#include <string>
#include <iostream>
#include <Windows.h>
#include <iomanip>

namespace Ui {
class MainWindow;
}

class MainWindow : public QMainWindow {
  Q_OBJECT

 public:
  explicit MainWindow(QWidget* parent = nullptr);
  ~MainWindow();
 private slots:
  void openSerialPort();
  void changeData();
  void closeSerialPort();
  void outputConcditions();
  bool initPort(HANDLE port, int baud_rate);
 private:
  HANDLE clientHandle;
  HANDLE serverHandle;
  Ui::MainWindow* ui;

};

#endif // MAINWINDOW_H
