#include "mainwindow.h"
#include "exif.h"

#include "./ui_mainwindow.h"

MainWindow::MainWindow(QWidget *parent)
  : QMainWindow(parent)
  , ui(new Ui::MainWindow)
{
  ui->setupUi(this);
}

MainWindow::~MainWindow()
{
  delete ui;
}

void MainWindow::update(const QString &info)
{
  ui->textBrowser->append(info);
}

void MainWindow::on_addDirectory_clicked()
{
  fileURLs.clear();
  ui->textBrowser->clear();
  ui->textBrowser_2->clear();

  QString folder_name = QFileDialog::getExistingDirectory(this, "Select folder", QDir::homePath());
  QDirIterator it(folder_name, QStringList() << "*.jpg" << "*.jpeg" << "*.JPG" << "*.JPEG", QDir::Files, QDirIterator::Subdirectories);

  while(it.hasNext())
  {
      fileURLs.push_back(it.next());
      ui->textBrowser->append(it.filePath());
  }
  ui->textBrowser_2->append("Files in your dir: " + QString::number(fileURLs.size()));
}

void MainWindow::on_showOnMap_clicked()
{
  my->moveToThread(thread);

  connect(my, SIGNAL(send(QString)), this, SLOT(update(QString)));
  connect(my, SIGNAL(workerFinish()), this, SLOT(startMap()));
  connect(my, SIGNAL(sendUpdateProgressBar()), this, SLOT(updateProgressBar()));
  connect(thread, SIGNAL(started()), my, SLOT(startThread()));


  thread->start();
}

void MainWindow::updateProgressBar(unsigned int max, unsigned int current)
{
  ui->progressBar->setRange(0, max);
  ui->progressBar->setValue(current);
}

void MainWindow::startMap()
{
  ui->textBrowser_2->append("boop");
  ui->textBrowser_2->append(my->getDataFromCommand("python3 map.py"));
  my->getDataFromCommand("xdg-open map.html");
}



