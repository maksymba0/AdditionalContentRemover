#include "mainwindow.h"
#include "ui_mainwindow.h"
#include <qlabel.h>
#include <QComboBox>
#include <QPushButton>
#include <QProgressBar>
#include <QDebug>
#include <QThread>
#include <QTimer>
#include <QSettings>
#include <QDir>
#include <QMessageBox>

MainWindow::MainWindow(QWidget *parent)
    : QMainWindow(parent)
    , ui(new Ui::MainWindow)
{
    ui->setupUi(this);
    this->setFixedSize(600,400);
    this->setWindowTitle("ACR - Additional Content Remover");
    QLabel* title = new QLabel(this);
    title->setText("Additional Content Remover");
    title->setIndent(20);
    QFont font("Arial",20);
    title->setFont(font);
    title->adjustSize();
    title->move(125,25);
    box = new QComboBox(this);
    box->addItem("DOTA 2");
    box->addItem("PUBG");
    box->move(230,60);

    scanbutton = new QPushButton(this);

    scanbutton->setText(" SCAN ");
    scanbutton->move(230,90);
    connect(scanbutton,&QPushButton::clicked,this,&MainWindow::scanPath);
    progressBar = new QProgressBar(this);
    progressBar->move(0,120);
    progressBar->setMinimum(0);
    progressBar->setMaximum(100);
    progressBar->setValue(1);
    progressBar->adjustSize();
    progressBar->setGeometry(0,120,670,400);



}

void MainWindow::scanPath()
{
    scanbutton->setDisabled(true);
    progressBar->setValue(0);

    QSettings settings("HKEY_CURRENT_USER\\Software\\Valve\\Steam", QSettings::NativeFormat);
    QString steamPath = settings.value("SteamPath").toString();
    steamPath.remove("\"");
    qDebug() << "Steam folder: " + steamPath;

    QDir appsDir(steamPath + "/steamapps/common/");
    if (!appsDir.exists()) {
        qDebug() << "Steam apps folder not found!";
         scanbutton->setEnabled(true);
        return;
    }

    QString TargetGame = box->currentText();
    if (TargetGame == "DOTA 2")
    {
        QDir contentDir(appsDir.absolutePath() + "/dota 2 beta/game/dota/panorama/videos/");
        if (!contentDir.exists()) {
            qDebug() << "DOTA 2 content folder not found!";
            scanbutton->setEnabled(true);
            return;
        }

        QMessageBox::StandardButton reply = QMessageBox::question(nullptr, "Removing confirmation",
                                                                  "Are you sure you want to clear content data files for DOTA 2? This is irreversible.");

        if (reply != QMessageBox::Yes)
        {
            scanbutton->setEnabled(true);
            return;
        }


        QStringList contentList = contentDir.entryList(QDir::Dirs | QDir::Files | QDir::NoDotAndDotDot);
        int totalItems = contentList.size();
        qDebug() << "Amount of files/folders to remove: " << totalItems;

        if (totalItems == 0) {
            qDebug() << "No files to remove.";
            QMessageBox::critical(nullptr,"Error - no files","The content folder is empty. What are you trying to clean?");
            progressBar->setValue(100);
            scanbutton->setEnabled(true);
            return;
        }


        int currentProgress = 0;
        QTimer* timer = new QTimer(this);
        connect(timer, &QTimer::timeout, this, [=]() mutable {
            if (contentList.isEmpty()) {
                progressBar->setValue(100);
                qDebug() << "Scan complete!";
                scanbutton->setEnabled(true);
                timer->stop();
                timer->deleteLater();
                return;
            }

            QString obj = contentList.takeFirst(); // Remove first item
            QFileInfo fileInfo(contentDir.absoluteFilePath(obj));

            if (fileInfo.isDir()) {
                QDir folder(fileInfo.absoluteFilePath());
                folder.removeRecursively();
            } else {
                contentDir.remove(obj);
            }


            currentProgress++;
            int progressValue = (currentProgress * 100) / totalItems;
            progressBar->setValue(progressValue);
            qDebug() << "Progress: " << progressValue << "%";
        });

        timer->start(100); // Adjust interval for smooth animation
    }else if(TargetGame == "PUBG")
    {
        QDir contentDir(appsDir.absolutePath() + "/PUBG/TslGame/Content/Movies/");
        if (!contentDir.exists()) {
            qDebug() << "PUBG content folder not found!";
            scanbutton->setEnabled(true);
            return;
        }

        QMessageBox::StandardButton reply = QMessageBox::question(nullptr, "Removing confirmation",
                                                                  "Are you sure you want to clear content data files for PUBG? This is irreversible.");

        if (reply != QMessageBox::Yes)
        {
            scanbutton->setEnabled(true);
            return;
        }


        QStringList contentList = contentDir.entryList(QDir::Dirs | QDir::Files | QDir::NoDotAndDotDot);
        int totalItems = contentList.size();
        qDebug() << "Amount of files/folders to remove: " << totalItems;

        if (totalItems == 0) {
            qDebug() << "No files to remove.";
            QMessageBox::critical(nullptr,"Error - no files","The content folder is empty. What are you trying to clean?");
            progressBar->setValue(100);
            scanbutton->setEnabled(true);
            return;
        }

        int currentProgress = 0;
        QTimer* timer = new QTimer(this);
        connect(timer, &QTimer::timeout, this, [=]() mutable {
            if (contentList.isEmpty()) {
                progressBar->setValue(100);
                qDebug() << "Scan complete!";
                scanbutton->setEnabled(true);
                timer->stop();
                timer->deleteLater();
                return;
            }

            QString obj = contentList.takeFirst(); // Remove first item
            QFileInfo fileInfo(contentDir.absoluteFilePath(obj));

            if (fileInfo.isDir()) {
                QDir folder(fileInfo.absoluteFilePath());
                folder.removeRecursively();
            } else {
                contentDir.remove(obj);
            }


            currentProgress++;
            int progressValue = (currentProgress * 100) / totalItems;
            progressBar->setValue(progressValue);
            qDebug() << "Progress: " << progressValue << "%";
        });

        timer->start(100);

    }
}

MainWindow::~MainWindow()
{
    delete ui;
}
