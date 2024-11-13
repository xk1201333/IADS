#ifndef FTPWINDOW_H
#define FTPWINDOW_H

#include <QWidget>
#include <QNetworkRequest>
#include <QNetworkReply>
#include <QUrl>
#include <QFile>
#include <QDebug>
#include <QMessageBox>
#include <QProgressBar>
#include <QMainWindow>

namespace Ui {
class ftpwindow;
}

class ftpwindow : public QWidget
{
    Q_OBJECT

public:
    explicit ftpwindow(QWidget *parent = nullptr);
    ~ftpwindow();

    void loadStyleSheet(const QString &styleSheetFile);

private slots:
    void on_uploadButton_clicked();
    void on_downloadButton_clicked();

    void readContent();
    void replyFinished(QNetworkReply*);
    void loadError(QNetworkReply::NetworkError);
    void loadProgress(qint64 bytesSent,qint64 bytesTotal);

    void on_setPara_clicked();

private:
    Ui::ftpwindow *ui;

    QNetworkReply *reply;
    //QProgressBar *progressBar;
    QFile *file;

    //ftp parameter
    QString serverIp;
    int portNumebr;
    QString userName;
    QString passWord;
};

#endif // FTPWINDOW_H

