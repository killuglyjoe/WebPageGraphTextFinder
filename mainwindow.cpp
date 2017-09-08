#include "mainwindow.h"
#include "ui_mainwindow.h"

static const QString cStrBegin(QObject::trUtf8("<span style=\" font-weight:600;\">Начало поиска для...</span>"));

static const QString cStrStop(QObject::trUtf8("<span style=\" font-weight:600;\">Отмена</span>"));

static const QString cStrFinished(QObject::trUtf8("<span style=\" font-weight:600;\">Конец</span>"));

static const QString cStrNetError(QObject::trUtf8("<span style=\" font-style:italic; color:#e83720;\">"
                                                 "Сетевая ошибка адрес: %1 значение: %2 </span>"));

static const QString cStrMatchResult(QObject::trUtf8("<span style=\" font-style:italic; text-decoration: "
                                                    "underline; color:#0000ff;\">Найдено %1 совпадений по адресу %2 </span>"));

static const QString cStrNoResult(QObject::trUtf8("<span style=\"color:#442222;"
                                                    "\">Ненайдено совпадений по адресу %2 </span>"));

MainWindow::MainWindow(QWidget *parent) :
    QMainWindow(parent),
    ui(new Ui::MainWindow),
    m_webPageLoader(new PageLoader(this)),
    m_urlParser(QSharedPointer<UrlParser>(new UrlParser))
{
    ui->setupUi(this);

    // Suppose we must do all the processings
    // in one base URL frame, e.g. processing that page
    // and all subpages in founded hyperlinks.
    // Not in all subpages->subPages->...
    connect(m_webPageLoader, &PageLoader::pageLoaded,
            m_urlParser.data(), &UrlParser::setData);

    connect(m_webPageLoader, &PageLoader::errorLoadingUrl,
            this, &MainWindow::handleNetworkError);

    connect(m_urlParser.data(), &UrlParser::onParsedStringList,
            this, &MainWindow::processParsedSubURLs);

    QButtonGroup *ctrlBtnGroup = new QButtonGroup(this);
    ctrlBtnGroup->addButton(ui->btnStart);
    ctrlBtnGroup->addButton(ui->btnStop);
    ctrlBtnGroup->addButton(ui->btnPause);

    // Pause/Resume processing (* не обязательно для выполнения)
    ui->btnPause->setDisabled(true);
}

MainWindow::~MainWindow()
{
    cleanAllProcesses();
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
    ui->btnStart->click();
//    startProcessing(ui->urlEdit->text());
}
/**
 * @brief Start processing
 */
void MainWindow::on_btnStart_clicked(bool checked)
{
    ui->statusEdit->clear();
    if(checked)
        startProcessing(ui->urlEdit->text());
}
/**
 * @brief Stop processing
 */
void MainWindow::on_btnStop_clicked(bool checked)
{
    bool isFinished(m_procesorsList.count() == 1);

    // finishing
    if(isFinished) // only base URL
        ui->statusEdit->append(trUtf8(cStrFinished.toUtf8()));

    // canceling
    if(checked)
    {
        cleanAllProcesses();
        if(!isFinished)
            ui->statusEdit->append(trUtf8(cStrStop.toUtf8()));
    }
}
/**
 * @brief Output NetworkError
 * @param url
 * @param error
 */
void MainWindow::handleNetworkError(const QString &url, const QString &error)
{
    ui->statusEdit->append(cStrNetError.arg(url).arg(error));
}
/**
 * @brief Output MatchResults
 * @param url
 * @param matchesCount
 */
void MainWindow::handleMatchResults(const QString &url, const int &matchesCount)
{
    ui->statusEdit->append(cStrMatchResult.arg(matchesCount).arg(url));
}
/**
 * @brief Output handleNoResults
 * @param url
 */
void MainWindow::handleNoResults(const QString &url)
{
    ui->statusEdit->append(cStrNoResult.arg(url));
}
/**
 * @brief checkFinish
 */
void MainWindow::checkFinish()
{
    if(m_procesorsList.count() == 1)
    {
        ui->btnStop->click();
    }
}
/**
 * @brief Starting all
 * @param url
 */
void MainWindow::startProcessing(const QString &url)
{
    if(url.isEmpty()) return;

    ui->statusEdit->append(trUtf8(cStrBegin.toUtf8()));
    ui->statusEdit->append(ui->urlEdit->text());

    m_webPageLoader->loadUrl(url);
}
/**
 * @brief All hyperlinks in base web page
 * @param subURLs
 */
void MainWindow::processParsedSubURLs(const QStringList &subURLs)
{
    QStringList procListURL(subURLs);

    //Cut URLs
    if(subURLs.count() > ui->urlCountBox->value())
        procListURL = subURLs.mid(0, ui->urlCountBox->value());

    QString txt2Search(ui->txtSrchEdit->text().toUtf8());

    // Process base WebPage
    WebPageTextThreadProcessor  *processor = new WebPageTextThreadProcessor(QStringList(m_webPageLoader->curURL()),
                                                                            txt2Search);

    ui->statusEdit->append(trUtf8("Поиск текста \"%1\" для %2").arg(txt2Search).arg(m_webPageLoader->curURL()));

    connectProcessor(processor);

    if(!m_procesorsList.contains(processor))
        m_procesorsList.append(processor);

    // balancing tasks(URLs) between processing threads
    // if all_tasks == thread_count then each processor will get one task,
    // else each processor will get (all_tasks/thread_count) tasks
    // or whatever last
    QStringList subList;

    int balancedUrlsCount(procListURL.count() / ui->threadCountBox->value());
    balancedUrlsCount = balancedUrlsCount < 1 ? 1 : balancedUrlsCount;

    foreach (QString url, procListURL)
    {
        if(url.isEmpty()) continue;//???

        subList.append(url);

        ui->statusEdit->append(trUtf8("Поиск текста \"%1\" для %2").arg(txt2Search).arg(url));

        if(subList.count() == balancedUrlsCount || procListURL.indexOf(url) == procListURL.count()-1)
        {

            WebPageTextThreadProcessor  *processor = new WebPageTextThreadProcessor(subList,
                                                                                    txt2Search);

            connectProcessor(processor, true);

            m_procesorsList.append(processor);
        }
    }
}
/**
 * @brief Helper function
 * @param processor
 */
void MainWindow::connectProcessor(WebPageTextThreadProcessor  *processor, const bool &needCheckFinish)
{
    connect(processor,  &WebPageTextThreadProcessor::errorLoadingUrl,
            this,   &MainWindow::handleNetworkError);

    connect(processor,  &WebPageTextThreadProcessor::foundMatches,
            this,   &MainWindow::handleMatchResults);

    connect(processor,  &WebPageTextThreadProcessor::notFoundMatches,
            this,   &MainWindow::handleNoResults);

    // for not base URL
    if(needCheckFinish)
    {
        // ugly lambda goes below
        connect(processor,  &WebPageTextThreadProcessor::finished,
                this,  [this]()
        {
            m_procesorsList.removeOne((WebPageTextThreadProcessor*)sender());
            sender()->deleteLater();
            checkFinish();
        });
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
/**
 * @brief Cancels and deletes all running processes
 */
void MainWindow::cleanAllProcesses()
{
    qDeleteAll(m_procesorsList);
    m_procesorsList.clear();
}
