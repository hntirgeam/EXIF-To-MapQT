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
  sendGpsToTxt(fileURLs);
  ui->textBrowser_2->append("boop");
  ui->textBrowser_2->append(getDataFromCommand("python3 map.py"));
  getDataFromCommand("xdg-open map.html");
}

void MainWindow::sendGpsToTxt(const QVector<QString> &file_urls)
{
    double lat = 0, lon = 0, alt = 0;

    QString filename = "gps_coordinates.txt";
    QFile file(filename);


    if (!file_urls.empty() && file.open(QIODevice::ReadWrite))
    {
        QTextStream stream(&file);

        for(auto f : file_urls)
        {
            GetGpsCoordinate(f.toStdString(), lat, lon, alt);
            if (lat != 0.0 && lon != 0.0)
            {
                stream << lat << " " << lon << " " << alt << "\n";
//                std::cout << "ZAPISALI :\t\"" << f.toStdString() << "\"" << '\n';
            }
        }
        ui->textBrowser->append("\nDone! Now you can run map.py");
    }
    else
    {
        ui->textBrowser->append("Sorry, but no matching files were found (or file error)");
    }

    file.close();
}

void MainWindow::GetGpsCoordinate(const std::string &file_name, double &lat, double &lon, double &alt)
{
    std::cout << file_name << std::endl;
    FILE *fp = fopen(file_name.c_str(), "rb");
    if (!fp)
    {
        ui->textBrowser->append("well...");
    }
    fseek(fp, 0, SEEK_END);
    unsigned long fsize = ftell(fp);
    rewind(fp);
    unsigned char *buf = new unsigned char[fsize];
    if (fread(buf, 1, fsize, fp) != fsize)
    {
        delete[] buf;
    }
    fclose(fp);

    easyexif::EXIFInfo result;
    int code = result.parseFrom(buf, fsize);
    delete[] buf;
    if (code)
    {
      ui->textBrowser->append("well...");
    }

    lat = result.GeoLocation.Latitude;
    lon = result.GeoLocation.Longitude;
    alt = result.GeoLocation.Altitude;
}

QString MainWindow::getDataFromCommand(std::string command)
{
    FILE *stream;
    const int max_buffer = 256;
    char buffer[max_buffer];
    command.append(" 2>&1");

    stream = popen(command.c_str(), "r");
    std::string data;
    if (stream)
    {
        while (!feof(stream))
            if (fgets(buffer, max_buffer, stream) != nullptr)
                data.append(buffer);
        pclose(stream);
    }
    return QString::fromStdString(data.c_str());;
}

