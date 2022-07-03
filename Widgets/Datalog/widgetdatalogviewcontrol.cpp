#include "widgetdatalogviewcontrol.h"
#include "helpers.h"
#include "dialogplotchannelchoose.h"
#include <QInputDialog>
#include <QFileDialog>
#include <QMessageBox>
#include <QGridLayout>
#include <QToolBar>
#include <QDebug>
#include <QHeaderView>
#include <QCheckBox>

WidgetDatalogViewControl::WidgetDatalogViewControl(QWidget *parent) : QWidget(parent)
{
    QGridLayout *contentLayout = new QGridLayout(this);
    contentLayout->setSpacing(0);
    contentLayout->setContentsMargins(0, 0, 0, 0);

    QToolButton *btnOpenDatalog = new QToolButton;
    btnOpenDatalog->setIcon(QIcon(":/icons/images/Folder-document-open-icon.png"));
    btnOpenDatalog->setText("Open Datalog");
    btnOpenDatalog->setToolTip("Open Datalog File");

    QToolButton *btnSettings = new QToolButton;
    btnSettings->setIcon(QIcon(":/icons/images/settings.png"));
    btnSettings->setCheckable(true);
    btnSettings->setText("Setup Datalog");
    btnSettings->setToolTip("Setup Datalog");

    btnDrawPoints = new QToolButton;
    btnDrawPoints->setCheckable(true);
    btnDrawPoints->setText("Draw Points");
    btnDrawPoints->setToolTip("Draw Points");

    btnDrawCurrentValueMarkers = new QToolButton;
    btnDrawCurrentValueMarkers->setCheckable(true);
    btnDrawCurrentValueMarkers->setChecked(true);
    btnDrawCurrentValueMarkers->setText("Markers");
    btnDrawCurrentValueMarkers->setToolTip("Draw markers at the current values");

    btnAntiAliasing = new QToolButton;
    btnAntiAliasing->setCheckable(true);
    btnAntiAliasing->setText("Anti Aliasing");
    btnAntiAliasing->setToolTip("Anti Aliasing");

    QToolButton *btnAddPlot = new QToolButton;
    //btnAddPlot->setIcon(QIcon(":/icons/images/Folder-document-open-icon.png"));
    btnAddPlot->setText("Add Plot");
    btnAddPlot->setToolTip("Add plot to the bottom");

    QToolButton *btnDeletePlot = new QToolButton;
    //btnDeletePlot->setIcon(QIcon(":/icons/images/Folder-document-open-icon.png"));
    btnDeletePlot->setText("Delete Plot");
    btnDeletePlot->setToolTip("Delete the bottom plot");

    QToolBar *toolbar = new QToolBar("toolbar", this);
    toolbar->setIconSize(QSize(16, 16));
    toolbar->addWidget(btnOpenDatalog);
    toolbar->addWidget(btnSettings);
    toolbar->addWidget(btnDrawPoints);
    toolbar->addWidget(btnDrawCurrentValueMarkers);
    toolbar->addWidget(btnAntiAliasing);
    toolbar->addWidget(btnAddPlot);
    toolbar->addWidget(btnDeletePlot);

    QObject::connect(btnOpenDatalog,
                     SIGNAL(clicked()),
                     this,
                     SLOT(openDatalogClicked()));

    QObject::connect(btnSettings,
                     SIGNAL(clicked(bool)),
                     this,
                     SLOT(settingsClicked(bool)));

    QObject::connect(btnAddPlot,
                     SIGNAL(clicked()),
                     this,
                     SLOT(addPlot()));

    QObject::connect(btnDeletePlot,
                     SIGNAL(clicked()),
                     this,
                     SLOT(deleteBottomPlot()));

    splitterSetting = new QSplitter;

    contentLayout->addWidget(toolbar);
    contentLayout->addWidget(splitterSetting);

    tableSettings = new QTableWidget(this);
    tableSettings->setVerticalScrollMode(QAbstractItemView::ScrollPerPixel);
    tableSettings->setHorizontalScrollMode(QAbstractItemView::ScrollPerPixel);
    tableSettings->setSelectionMode(QAbstractItemView::NoSelection);
    tableSettings->setAlternatingRowColors(true);
    tableSettings->verticalHeader()->hide();
    tableSettings->verticalHeader()->setDefaultSectionSize(15);
    tableSettings->setColumnCount(4);
    tableSettings->hide();

    tableSettings->setHorizontalHeaderItem(0, new QTableWidgetItem("Channel"));
    tableSettings->setColumnWidth(0, 100);
    tableSettings->setHorizontalHeaderItem(1, new QTableWidgetItem("Min"));
    tableSettings->setColumnWidth(1, 50);
    tableSettings->setHorizontalHeaderItem(2, new QTableWidgetItem("Value"));
    tableSettings->setColumnWidth(2, 50);
    tableSettings->setHorizontalHeaderItem(3, new QTableWidgetItem("Max"));
    tableSettings->setColumnWidth(3, 50);

    splitterPlots = new QSplitter;
    splitterPlots->setOrientation(Qt::Vertical);
    splitterPlots->setHandleWidth(3);
    splitterPlots->setStyleSheet("QSplitter::handle { background-color: gray };");

    splitterSetting->addWidget(splitterPlots);
    splitterSetting->addWidget(tableSettings);

    //////// Tab Right Click Menu ////////
    plotContextMenu = new QMenu(this);
    plotContextMenu->addAction(plotContextMenuStrChooseChannels);

    addPlot();
}

void WidgetDatalogViewControl::addPlot()
{
    PlotDatalog *plot = new PlotDatalog(splitterPlots);
    plot->setAntiAliasing(btnAntiAliasing->isChecked());
    plot->setDrawCurrentValueMarkers(btnDrawCurrentValueMarkers->isChecked());
    plot->setDrawPoints(btnDrawPoints->isChecked());

    splitterPlots->addWidget(plot);

    QObject::connect(btnDrawPoints,
                     SIGNAL(clicked(bool)),
                     plot,
                     SLOT(setDrawPoints(bool)));

    QObject::connect(btnDrawCurrentValueMarkers,
                     SIGNAL(clicked(bool)),
                     plot,
                     SLOT(setDrawCurrentValueMarkers(bool)));

    QObject::connect(btnAntiAliasing,
                     SIGNAL(clicked(bool)),
                     plot,
                     SLOT(setAntiAliasing(bool)));

    QObject::connect(plot,
                     SIGNAL(mousePressEventSignal(QMouseEvent*)),
                     this,
                     SLOT(plotMousePressEvent(QMouseEvent*)));

    QObject::connect(plot,
                     SIGNAL(mouseMoveEventSignal(QMouseEvent*)),
                     this,
                     SLOT(plotMouseMoveEvent(QMouseEvent*)));

    QObject::connect(plot,
                     SIGNAL(wheelEventSignal(QWheelEvent*)),
                     this,
                     SLOT(plotWheelEvent(QWheelEvent*)));

    QObject::connect(plot,
                     SIGNAL(resizeEventSignal(QResizeEvent*)),
                     this,
                     SLOT(plotResizeEvent(QResizeEvent*)));

    QObject::connect(this,
                     SIGNAL(repaintPlots()),
                     plot,
                     SLOT(repaint()));
}

void WidgetDatalogViewControl::plotResizeEvent(QResizeEvent*)
{
    emit repaintPlots();
}

void WidgetDatalogViewControl::plotMouseMoveEvent(QMouseEvent *event)
{
    if (event->buttons() & Qt::LeftButton)
    {
        float dx = (event->x() - lastMousePos.x()) * (originalXAxisBoundMax - originalXAxisBoundMin);
        dx = dx / width();
        xAxisBoundMin = originalXAxisBoundMin - dx;
        xAxisBoundMax = originalXAxisBoundMax - dx;

        emit setXAxisBounds(xAxisBoundMin, xAxisBoundMax);
    }
    emit repaintPlots();
}

void WidgetDatalogViewControl::plotWheelEvent(QWheelEvent *event)
{
    double amount = (event->angleDelta().y() / 180.0);
    double temp_xAxisBoundMin = xAxisBoundMin - amount;
    double temp_xAxisBoundMax = xAxisBoundMax + amount;

    if (temp_xAxisBoundMin > temp_xAxisBoundMax)
    {
        return;
    }

    xAxisBoundMin = temp_xAxisBoundMin;
    xAxisBoundMax = temp_xAxisBoundMax;

    // Send to all plots
    emit setXAxisBounds(xAxisBoundMin, xAxisBoundMax);
    emit repaintPlots();
}

void WidgetDatalogViewControl::plotMousePressEvent(QMouseEvent *event)
{
    PlotDatalog* senderPlot = qobject_cast<PlotDatalog*>(sender());

    if (event->button() == Qt::RightButton)
    {
        QAction* selectedItem = plotContextMenu->exec(senderPlot->mapToGlobal(event->pos()));

        // If something was pressed
        if (selectedItem)
        {
            if (selectedItem->text().compare(plotContextMenuStrChooseChannels) == 0)
            {
                DialogPlotChannelChoose dialog(listOfPointLists, this);
                dialog.exec();

                if (dialog.result() == QDialog::Accepted)
                {
                    senderPlot->clearPointLists();

                    QList<int> selectedIndices = dialog.getSelectedIndices();

                    for (int i = 0; i < selectedIndices.length(); i++)
                    {
                        senderPlot->addPointList(listOfPointLists.at(selectedIndices.at(i)));
                    }
                }
            }
        }
    }
    else if (event->button() == Qt::LeftButton)
    {
        lastMousePos = event->pos();
        originalXAxisBoundMin = xAxisBoundMin;
        originalXAxisBoundMax = xAxisBoundMax;
    }
}

void WidgetDatalogViewControl::deleteBottomPlot()
{
    if (splitterPlots->count() > 1)
    {
        splitterPlots->widget(splitterPlots->count() - 1)->deleteLater();
    }
}

void WidgetDatalogViewControl::addPlointListsToSettings()
{
    tableSettings->clearContents();
    tableSettings->setRowCount(listOfPointLists.count());

    // Add channels to table
    for (int channel = 0; channel < listOfPointLists.count(); channel++)
    {
        PointList *pList = listOfPointLists.at(channel);

        QTableWidgetItem *itemName = new QTableWidgetItem(pList->getName());
        itemName->setFlags(itemName->flags() ^ Qt::ItemIsEditable);

        tableSettings->setItem(channel, 0, itemName);

        QTableWidgetItem *itemMin = new QTableWidgetItem(QString::number(pList->getMinY()));
        itemMin->setFlags(itemMin->flags() ^ Qt::ItemIsEditable);

        tableSettings->setItem(channel, 1, itemMin);

        QTableWidgetItem *itemMax = new QTableWidgetItem(QString::number(pList->getMaxY()));
        itemMax->setFlags(itemMax->flags() ^ Qt::ItemIsEditable);

        tableSettings->setItem(channel, 3, itemMax);
    }
}

void WidgetDatalogViewControl::openDatalogClicked()
{
    QString fileName = QFileDialog::getOpenFileName(this, tr("Open Log File"),
                                                    "",
                                                    tr("Files (*.csv);;Files (*.log)"));

    if (!fileName.isEmpty())
    {
        QFile file(fileName);
        if (!file.open(QIODevice::ReadOnly | QIODevice::Text))
        {
            QMessageBox::warning(this, "Error", "File is unreadable!", QMessageBox::Ok);
            return;
        }

        deleteAllIn_ListOfPointLists();

        int lineNum = 1;
        while (!file.atEnd())
        {
            QByteArray line = file.readLine();
            line = line.trimmed(); // Remove start and end white spaces

            if (line.startsWith('#') || line.startsWith('"'))
            {
                continue;
            }

            if (lineNum == 1)
            {
                // First Line
                if (!processDatalogFirstLine(line))
                {
                    deleteAllIn_ListOfPointLists();
                    return;
                }
            }
            else
            {
                // Rest of Lines
                if (!processDatalogLine(line, lineNum))
                {
                    deleteAllIn_ListOfPointLists();
                    return;
                }
            }

            lineNum++;
        }

        // Set initial axis bounds
        for (int i = 0; i < listOfPointLists.count(); i++)
        {
            if (i == 0)
            {
                /// Initial X axis bounds
                xAxisBoundMin = listOfPointLists.at(0)->getMinX();

                if (initialXAxisBoundMax < listOfPointLists.at(0)->getMaxX())
                {
                    xAxisBoundMax = xAxisBoundMin + initialXAxisBoundMax;
                }
                else
                {
                    xAxisBoundMax = listOfPointLists.at(0)->getMaxX();
                }

                emit setXAxisBounds(xAxisBoundMin, xAxisBoundMax);
            }

            /// Initial Y axis bounds
            listOfPointLists.at(i)->setAxisBoundsY(listOfPointLists.at(i)->getMinY(),
                                                   listOfPointLists.at(i)->getMaxY());
        }

        addPlointListsToSettings();
    }
}

void WidgetDatalogViewControl::deleteAllIn_ListOfPointLists()
{
    // Delete dynamic allocated memory in container
    qDeleteAll(listOfPointLists);
    listOfPointLists.clear();

    for (int i = 0; i < splitterPlots->count(); i++)
    {
        PlotDatalog* plot = qobject_cast<PlotDatalog *>(splitterPlots->children().at(i));
        plot->clearPointLists();
    }

    // This will clear the settingTable since theres nothing in listOfPointLists
    addPlointListsToSettings();
}

bool WidgetDatalogViewControl::processDatalogLine(QByteArray line, int lineNumber)
{
    bool conversionOK;
    int currentCol = 0;
    QList<QByteArray> array = line.split(',');

    double timeValue = array[timeColumn].toDouble(&conversionOK);

    if (!conversionOK)
    {
        QString errorText("Error converting time value to double, line ");
        errorText.append(QString::number(lineNumber)).append(", column ").append(QString::number(timeColumn + 1)).append(".");

        qDebug() << errorText;
        QMessageBox::warning(this, "Error", errorText, QMessageBox::Ok);
        return conversionOK;
    }

    for (int col = 0; col < array.length(); col++)
    {
        if (col == timeColumn)
        {
            continue;
        }

        PointList *pList = listOfPointLists.value(currentCol);

        double value = array[col].toDouble(&conversionOK);

        if (conversionOK)
        {
            pList->append(QPointF(timeValue, value));
        }
        else
        {
            QString errorText("Error converting value to double, line ");
            errorText.append(QString::number(lineNumber)).append(", column ").append(QString::number(col + 1)).append(".").append(" Adding 0 instead.");

            qDebug() << errorText;
            QMessageBox::warning(this, "Error", errorText, QMessageBox::Ok);
            pList->append(QPointF(timeValue, 0));
        }

        currentCol++;
    }
    return true;
}

/**
 * @brief PlotDatalog::processDatalogFirstLine
 * @param line
 * @return true if time column was found
 *         false if time column was not found
 */
bool WidgetDatalogViewControl::processDatalogFirstLine(QByteArray line)
{
    QStringList columnNames;
    bool timeColumnFound = false;

    QList<QByteArray> array = line.split(',');

    // add column names to items list for selection dialog
    for (int col = 0; col < array.length(); col++)
    {
        array[col] = array[col].trimmed(); // trim whitespaces from start and end
        columnNames << tr(array[col]);
    }

    // show dialog
    bool ok;
    timeColumnName = QInputDialog::getItem(this, tr("Time Axis Selection"),
                                           tr("Select time column:"), columnNames, 0, false, &ok);

    // if something went wrong
    if (!ok || timeColumnName.isEmpty())
    {
        QMessageBox::warning(this, "Error", "Time column was not found in datalog file", QMessageBox::Ok);
        return false;
    }

    for (int col = 0; col < array.length(); col++)
    {
        // get time column number
        if (array[col] == timeColumnName)
        {
            timeColumn      = col;
            timeColumnFound = true;
            continue;
        }

        PointList *plist = new PointList;

        // We only change the x axis bounds
        QObject::connect(this,
                         SIGNAL(setXAxisBounds(double, double)),
                         plist,
                         SLOT(setAxisBoundsX(double,double)));

        plist->setName(array[col]);
        plist->setColor(QColor(Helpers::randInt(lineColorBetweenA, lineColorBetweenB),
                               Helpers::randInt(lineColorBetweenA, lineColorBetweenB),
                               Helpers::randInt(lineColorBetweenA, lineColorBetweenB)));

        listOfPointLists.append(plist);
    }

    // Check if time column was found
    if (timeColumnFound)
    {
        return true;
    }

    QMessageBox::warning(this, "Error", "Time column was not found in datalog file", QMessageBox::Ok);
    return false;
}

void WidgetDatalogViewControl::settingsClicked(bool checked)
{
    if (checked)
    {
        tableSettings->show();
    }
    else
    {
        tableSettings->hide();
    }
}
