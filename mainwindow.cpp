#include "mainwindow.h"
#include "exif.h"

#include "./ui_mainwindow.h"

MainWindow::MainWindow(QWidget *parent)
    : QMainWindow(parent), ui(new Ui::MainWindow)
{
  ui->setupUi(this);
  ui->progressBar->setValue(0);
  ui->locationLabel->setTextInteractionFlags(Qt::TextSelectableByMouse);
}

MainWindow::~MainWindow()
{
  delete ui;
}

void MainWindow::update(const QString &info)
{
  ui->logTextBrowser->append(info);
}

void MainWindow::on_addDirectory_clicked()
{
  fileURLs.clear();
//  ui->fileList->clear();
  ui->preview->clear();
  ui->logTextBrowser->clear();
  ui->progressBar->setValue(0);

  QString folder_name = QFileDialog::getExistingDirectory(this, "Select folder", QDir::homePath());
  QDirIterator it(folder_name, QStringList() << "*.jpg"
                                             << "*.jpeg"
                                             << "*.JPG"
                                             << "*.JPEG",
                  QDir::Files, QDirIterator::Subdirectories);

  while (it.hasNext())
  {
    fileURLs.push_back(it.next());

    QListWidgetItem *item = new QListWidgetItem(it.next());

    ui->fileList->addItem(item);
//    ui->logTextBrowser->append(it.filePath());
  }

  ui->logTextBrowser->append("Files in your dir: " + QString::number(fileURLs.size()));
  ui->progressBar->setRange(0, fileURLs.size());
}

void MainWindow::on_showOnMap_clicked()
{
  QThread *thread = new QThread;
  MainProcess *my = new MainProcess(fileURLs);

  my->moveToThread(thread);

  connect(my, SIGNAL(send(QString)), this, SLOT(update(QString)));
  connect(my, SIGNAL(workerFinish()), this, SLOT(startMap()));
  connect(my, SIGNAL(sendUpdateProgressBar(unsigned int)), this, SLOT(updateProgressBar(unsigned int)));
  connect(thread, SIGNAL(started()), my, SLOT(startThread()));

  thread->start();
}

void MainWindow::updateProgressBar(unsigned int current)
{
  ui->progressBar->setValue(current);
}

void MainWindow::startMap()
{
  ui->logTextBrowser->append("boop");
  ui->logTextBrowser->append(p.getDataFromCommand("python3 map.py"));
  p.getDataFromCommand("xdg-open map.html");
}

void MainWindow::on_fileList_itemClicked()
{
  int file_i = ui->fileList->currentRow();

  double latitude = 0, longitude = 0, altitude = 0;
  p.GetGpsCoordinate(fileURLs[file_i].toStdString(), latitude, longitude, altitude);

  ui->locationLabel->clear();
  QString locationAppend = "Current picture location (e.g. copy-paste this in google maps):   " + QString::number(latitude) + " " + QString::number(longitude);
  ui->locationLabel->setText(locationAppend);

  QPixmap pic (fileURLs[file_i]);
  pic = pic.scaled(400, 400, Qt::KeepAspectRatio, Qt::FastTransformation);
  ui->preview->setPixmap(pic);
}

void MainWindow::on_fileList_itemDoubleClicked()
{
  int i = ui->fileList->currentRow();
  QDesktopServices::openUrl(QUrl::fromLocalFile((fileURLs[i])));
}
