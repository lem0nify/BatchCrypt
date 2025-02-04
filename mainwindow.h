#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QMainWindow>
#include <QTimer>
#include "filebatchencryptor.h"

QT_BEGIN_NAMESPACE
namespace Ui {
class MainWindow;
}
QT_END_NAMESPACE

class MainWindow : public QMainWindow
{
    Q_OBJECT

public:
    MainWindow(QWidget *parent = nullptr);
    ~MainWindow();

public slots:
    void process();

private slots:
    void on_pbInDirOpen_clicked();

    void on_pbOutDirOpen_clicked();

    void on_cbTimer_checkStateChanged(const Qt::CheckState &state);

    void on_pbProcess_clicked();

private:
    Ui::MainWindow *ui;
    QTimer *timer;
    FileBatchEncryptor *encryptor;
};
#endif // MAINWINDOW_H
