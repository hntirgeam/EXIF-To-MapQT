#pragma once

#include <QMainWindow>

#include <QDirIterator>
#include <QFileDialog>
#include <QThread>

QT_BEGIN_NAMESPACE
namespace Ui { class MainWindow; }
QT_END_NAMESPACE

class MainWindow : public QMainWindow
{
  Q_OBJECT

public:
  MainWindow(QWidget *parent = nullptr);
  ~MainWindow();

private slots:
  void on_addDirectory_clicked();
  void on_showOnMap_clicked();
  void update(const QString &info);
  void startMap();

private:
  Ui::MainWindow *ui;
  QVector<QString> fileURLs;
};
