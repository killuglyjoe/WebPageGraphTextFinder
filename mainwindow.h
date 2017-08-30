#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QMainWindow>

#include "pageloader.h"
#include "urlparser.h"

#include "webpagetextthreadprocessor.h"

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

    void on_btnStart_clicked(bool checked);
    void on_btnStop_clicked();
    void on_btnPause_clicked();

    void on_urlEdit_returnPressed();

    void startProcessing(const QString &url);

    void processParsedSubURLs(const QStringList &subURLs);

protected:
    void changeEvent(QEvent *event);

private:
    Ui::MainWindow                      *ui;
    PageLoader                          *m_webPageLoader;   //base URL loader
    QSharedPointer<UrlParser>           m_urlParser;        //base WebPage URLs finder
    QList<WebPageTextThreadProcessor*>  m_procesorsList;
};

#endif // MAINWINDOW_H
