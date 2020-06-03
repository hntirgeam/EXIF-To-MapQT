#pragma once

#include <QFile>
#include <QTextStream>

#include <QObject>

#include <unistd.h>

class MainProcess : public QObject
{
  Q_OBJECT
public:
  MainProcess();
  MainProcess(QVector<QString> &file_urls);
  ~MainProcess();

  void GetGpsCoordinate(const std::string &file_name, double &lat, double &lon, double &alt);
  QString getDataFromCommand(std::string command);

private:
  QVector<QString> file_urls;

public slots:
  void startThread();
  void sendGpsToTxt(const QVector<QString> &file_urls);

signals:
  void send(QString);
  void workerFinish();
  void sendUpdateProgressBar(unsigned int);
};
