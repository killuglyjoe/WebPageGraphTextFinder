#include "mainwindow.h"
#include "ui_mainwindow.h"

MainWindow::MainWindow(QWidget *parent) :
    QMainWindow(parent),
    ui(new Ui::MainWindow),
    m_webPageLoader(new PageLoader(this)),
    m_urlParser(QSharedPointer<UrlParser>(new UrlParser))
{
    ui->setupUi(this);

    //Suppose we must do all the processings
    //in one base URL frame, e.g. processing that page
    //and all subpages in founded hyperlinks.
    //Not in all subpages->subPages->...
    connect(m_webPageLoader, &PageLoader::pageLoaded,
            m_urlParser.data(), &UrlParser::setData);

    connect(m_urlParser.data(), &UrlParser::onParsedStringList,
            this, &MainWindow::processParsedSubURLs);

    QButtonGroup *ctrlBtnGroup = new QButtonGroup(this);
    ctrlBtnGroup->addButton(ui->btnStart);
    ctrlBtnGroup->addButton(ui->btnStop);
    ctrlBtnGroup->addButton(ui->btnPause);
}

MainWindow::~MainWindow()
{
    qDeleteAll(m_procesorsList);
    m_procesorsList.clear();
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
 * @brief Start processing by pressing ENTER button
 */
void MainWindow::on_urlEdit_returnPressed()
{
    startProcessing(ui->urlEdit->text());
}
/**
 * @brief Start processing
 */
void MainWindow::on_btnStart_clicked(bool checked)
{
    if(checked)
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
 * @brief Pause/Resume processing (* не обязательно для выполнения)
 */
void MainWindow::on_btnPause_clicked()
{
}
/**
 * @brief Starting all
 * @param url
 */
void MainWindow::startProcessing(const QString &url)
{
    if(url.isEmpty()) return;

    m_webPageLoader->loadUrl(url);
}
/**
 * @brief All hyperlinks in base web page
 * @param subURLs
 */
void MainWindow::processParsedSubURLs(const QStringList &subURLs)
{
    QStringList procListURL(subURLs);

    if(subURLs.count() > ui->urlCountBox->value())
        procListURL = subURLs.mid(0, ui->urlCountBox->value());

    //balancing tasks(URLs) between processing threads
    //if all tasks == thread count then processor will get one task,
    //else each processor will get (all tasks/thread count) tasks
    foreach (QString url, procListURL)
    {
        if(url.isEmpty())
            continue;

        WebPageTextThreadProcessor  *processor = new WebPageTextThreadProcessor(QStringList(url),
                                                                                ui->txtSrchEdit->text());
        m_procesorsList.append(processor);
    }
}
/**
 * @brief Translate UI
 * @param event
 */
void MainWindow::changeEvent(QEvent *event)
{
    if(event->type() == QEvent::LanguageChange)
    {
        ui->retranslateUi(this);
    }
    QWidget::changeEvent(event);
}

