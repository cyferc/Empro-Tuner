#pragma once

#include <QWidget>
#include <QSplitter>
#include <QTableWidget>
#include <QToolButton>
#include <QMouseEvent>
#include <QMenu>
#include "pointlist.h"
#include "plotdatalog.h"

class WidgetDatalogViewControl : public QWidget
{
    Q_OBJECT
public:
    explicit WidgetDatalogViewControl(QWidget *parent = 0);

signals:
    void setXAxisBounds(double min, double max);
    void repaintPlots();

public slots:
    void openDatalogClicked();
    void settingsClicked(bool checked);
    void addPlot();
    void deleteBottomPlot();
    void plotMousePressEvent(QMouseEvent *event);
    void plotMouseMoveEvent(QMouseEvent *event);
    void plotWheelEvent(QWheelEvent *event);
    void plotResizeEvent(QResizeEvent* event);

private:
    void deleteAllIn_ListOfPointLists();
    bool processDatalogLine(QByteArray line, int lineNumber);
    bool processDatalogFirstLine(QByteArray line);
    void addPlointListsToSettings();

    QToolButton *btnDrawPoints;
    QToolButton *btnDrawCurrentValueMarkers;
    QToolButton *btnAntiAliasing;
    QSplitter *splitterPlots;
    QSplitter *splitterSetting;
    QTableWidget *tableSettings;
    QList<PointList*> listOfPointLists;
    QMenu *plotContextMenu;

    double xAxisBoundMin = 2.0;
    double xAxisBoundMax = 5.0;
    const int initialXAxisBoundMax = 10; // initial zoom

    const int lineColorBetweenA = 70;
    const int lineColorBetweenB = 255;

    int timeColumn;
    QString timeColumnName;
    const QString plotContextMenuStrChooseChannels = "Choose channels";

    QPoint lastMousePos;
    double originalXAxisBoundMin;
    double originalXAxisBoundMax;
};
