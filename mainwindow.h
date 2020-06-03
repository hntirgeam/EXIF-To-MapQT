#pragma once

#include <QMainWindow>

#include "mainprocess.h"

#include <QDirIterator>
#include <QFileDialog>
#include <QThread>
#include <QDesktopServices>

QT_BEGIN_NAMESPACE
namespace Ui
{
  class MainWindow;
}
QT_END_NAMESPACE

class MainWindow : public QMainWindow
{
  Q_OBJECT

public:
  MainWindow(QWidget *parent = nullptr);
  ~MainWindow();

private slots:
  void updateProgressBar(unsigned int current);
  void on_addDirectory_clicked();
  void on_showOnMap_clicked();
  void update(const QString &info);
  void startMap();

  void on_fileList_itemClicked();
  void on_fileList_itemDoubleClicked();

private:
  MainProcess p;
  QVector<QString> fileURLs;
  Ui::MainWindow *ui;
};
