#pragma once

#include <QMainWindow>
#include <QDirIterator>
#include <QFileDialog>
#include <QDebug>
#include <QtDebug>

#include <iostream>

#include <QFile>
#include <QTextStream>

QT_BEGIN_NAMESPACE
namespace Ui { class MainWindow; }
QT_END_NAMESPACE

class MainWindow : public QMainWindow
{
  Q_OBJECT

public:
  MainWindow(QWidget *parent = nullptr);
  ~MainWindow();

  void sendGpsToTxt(const QVector<QString> &file_urls);
  void GetGpsCoordinate(std::string file_name, double &lat, double &lon, double &alt);
  QString getDataFromCommand(std::string command);

private slots:
  void on_addDirectory_clicked();
  void on_showOnMap_clicked();

private:
  Ui::MainWindow *ui;
  QVector<QString> fileURLs;
};
