#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QMainWindow>

#include "pageloader.h"
#include "textparser.h"

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
    void on_actionExit_triggered();

    void on_btnStart_clicked();

    void on_btnStop_clicked();

    void on_btnPause_clicked();

    void on_urlEdit_returnPressed();

    void startProcessing(const QString &url);

private:
    Ui::MainWindow  *ui;
    PageLoader      *m_webPageLoader;
    TextParser      *m_txtParser;
};

#endif // MAINWINDOW_H
