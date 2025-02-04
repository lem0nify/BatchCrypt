#include "mainwindow.h"
#include "./ui_mainwindow.h"
#include <QDir>
#include <QFileDialog>
#include <QStringList>

MainWindow::MainWindow(QWidget *parent)
    : QMainWindow(parent)
    , ui(new Ui::MainWindow)
{
    ui->setupUi(this);
    ui->leInDir->setText(QDir::currentPath() + "/in");
    ui->leOutDir->setText(QDir::currentPath() + "/out");

    timer = nullptr;
    encryptor = nullptr;
}

MainWindow::~MainWindow()
{
    if (encryptor != nullptr) delete encryptor;
    if (timer != nullptr)
    {
        timer->stop();
        delete timer;
    }
    delete ui;
}

void MainWindow::on_pbInDirOpen_clicked()
{
    QString path = ui->leInDir->text();
    path = QFileDialog::getExistingDirectory(
        nullptr,
        "Выберите директорию",
        path);
    ui->leInDir->setText(path);
}


void MainWindow::on_pbOutDirOpen_clicked()
{
    QString path = ui->leOutDir->text();
    path = QFileDialog::getExistingDirectory(
        nullptr,
        "Выберите директорию",
        path);
    ui->leOutDir->setText(path);
}


void MainWindow::on_cbTimer_checkStateChanged(const Qt::CheckState &state)
{
    ui->wTimer->setDisabled(!state);
    ui->lbTimer->setDisabled(!state);
    ui->pbProcess->setText(state ? "Старт" : "Выполнить");
}


void MainWindow::on_pbProcess_clicked()
{
    if (encryptor != nullptr)
    {
        delete encryptor;
        encryptor = nullptr;
    }
    encryptor = new FileBatchEncryptor(QByteArray::fromHex(ui->leKey->text().toUtf8()));
    encryptor->fileExistAction = static_cast<FileExistAction>(ui->cobFileExistAction->currentIndex());
    encryptor->deleteProcessedFiles = ui->cbDelete->isChecked();
    if (!ui->cbTimer->isChecked())
        process();
    else
    {
        if (timer != nullptr)
        {
            timer->stop();
            delete timer;
            timer = nullptr;

            ui->wOptions->setDisabled(false);
            ui->pbProcess->setText("Старт");
            return;
        }

        int duration = ui->sbInterval->value();
        switch (ui->cobIntervalUnit->currentIndex())
        {
        case 0:
            duration += 60;
        case 1:
            duration *= 60;
        case 2:
            duration *= 1000;
        }

        timer = new QTimer(this);
        connect(timer, &QTimer::timeout, this, &MainWindow::process);
        timer->start(duration);
        ui->wOptions->setDisabled(true);
        ui->pbProcess->setText("Стоп");
    }
}

void MainWindow::process()
{
    if (encryptor != nullptr)
    {
        QDir inDir = QDir(ui->leInDir->text());
        QDir outDir = QDir(ui->leOutDir->text());
        encryptor->processDirectory(inDir, outDir, QStringList(ui->leMask->text()));
    }
}
