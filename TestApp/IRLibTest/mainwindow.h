#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QtCore>
#include <QtGui>
#include <QMainWindow>
#include <QMessageBox>

namespace Ui {
class MainWindow;
}

class MainWindow : public QMainWindow
{
    Q_OBJECT

public:
    explicit MainWindow(QWidget *parent = 0);
    ~MainWindow();

private slots:
    void on_frFindPushButton_clicked();

    void on_decDecodePushButton_clicked();

private:
    Ui::MainWindow *ui;

    std::string printHexStringToBinaryString(std::string hexStr, bool printByteNum);
};

#endif // MAINWINDOW_H
