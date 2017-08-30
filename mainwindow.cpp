#include "mainwindow.h"
#include "ui_mainwindow.h"

MainWindow::MainWindow(QWidget *parent) :
    QMainWindow(parent),
    ui(new Ui::MainWindow),
    m_webPageLoader(new PageLoader(this)),
    m_txtParser(new TextParser(this))
{
    ui->setupUi(this);

    connect(m_webPageLoader, &PageLoader::pageLoaded, m_txtParser, &TextParser::setParseData);
}

MainWindow::~MainWindow()
{
    delete ui;
}
/**
 * @brief Exit app
 */
void MainWindow::on_actionExit_triggered()
{
    close();
}
/**
 * @brief MainWindow::on_urlEdit_returnPressed
 */
void MainWindow::on_urlEdit_returnPressed()
{
    startProcessing(ui->urlEdit->text());
}
/**
 * @brief Start processing
 */
void MainWindow::on_btnStart_clicked()
{
    m_webPageLoader->loadUrl(ui->urlEdit->text());
}
/**
 * @brief Stop processing
 */
void MainWindow::on_btnStop_clicked()
{
    startProcessing(ui->urlEdit->text());
}
/**
 * @brief Pause/Resume processing
 */
void MainWindow::on_btnPause_clicked()
{

}

void MainWindow::startProcessing(const QString &url)
{
    m_webPageLoader->loadUrl(url);
}
