#include "mainprocess.h"
#include "exif.h"

#include <iostream>

MainProcess::MainProcess()
{

}

MainProcess::MainProcess(const QVector<QString> &file_urls)
{
  this->file_urls = std::move(file_urls);
}

MainProcess::~MainProcess()
{

}

void MainProcess::startThread()
{
  sendGpsToTxt(file_urls);
}

void MainProcess::sendGpsToTxt(const QVector<QString> &file_urls)
{
    double lat = 0, lon = 0, alt = 0;

    QString filename = "gps_coordinates.txt";
    QFile file(filename);
    file.remove();

    unsigned int vecSize = file_urls.size();
    unsigned int vecSizeDe = file_urls.size();
    emit sendUpdateProgressBar(vecSize, vecSizeDe);

    if (!file_urls.empty() && file.open(QIODevice::ReadWrite))
    {
        QTextStream stream(&file);

        for(auto f : file_urls)
        {
            GetGpsCoordinate(f.toStdString(), lat, lon, alt);
            if (lat != 0.0 && lon != 0.0)
            {
                stream << lat << " " << lon << " " << alt << "\n";
                emit sendUpdateProgressBar(vecSize, vecSizeDe);
                vecSizeDe--;
            }
        }
        emit send("=========================");
        emit workerFinish();
    }
    else
    {
        emit send("Sorry, but no matching files were found (or file error)");
    }
    file.close();
}

void MainProcess::GetGpsCoordinate(const std::string &file_name, double &lat, double &lon, double &alt)
{
    FILE *fp = fopen(file_name.c_str(), "rb");
    if (!fp)
    {
        emit send("Can't open this file");
    }
    fseek(fp, 0, SEEK_END);
    unsigned long fsize = ftell(fp);
    rewind(fp);
    unsigned char *buf = new unsigned char[fsize]; // not a good idea to allocate here but mb i'll fix it later (never)
    if (fread(buf, 1, fsize, fp) != fsize)
    {
        delete[] buf;
        emit send("Can't read this file");
    }
    fclose(fp);

    easyexif::EXIFInfo result;
    int code = result.parseFrom(buf, fsize);
    delete[] buf;
    if (code)
    {
        emit send("Can't parse this file");
    }

    lat = result.GeoLocation.Latitude;
    lon = result.GeoLocation.Longitude;
    alt = result.GeoLocation.Altitude;
    QString res = "ADDED: " + QString::fromStdString(file_name);
    emit send(res);
}

QString MainProcess::getDataFromCommand(std::string command)
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
