#include "mainprocess.h"
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
  QThread *thread= new QThread;
  MainProcess *my = new MainProcess(fileURLs);

  my->moveToThread(thread);

  connect(my, SIGNAL(send(QString)), this, SLOT(update(QString)));
  connect(thread, SIGNAL(started()), my, SLOT(startThread()));
  connect(my, SIGNAL(workerFinish()), this, SLOT(startMap()));

  thread->start();
}

void MainWindow::startMap()
{
  MainProcess p;

  ui->textBrowser_2->append("boop");
  ui->textBrowser_2->append(p.getDataFromCommand("python3 map.py"));
  p.getDataFromCommand("xdg-open map.html");
}



