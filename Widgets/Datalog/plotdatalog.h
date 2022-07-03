#pragma once

#include <QWidget>
#include <QWheelEvent>
#include "pointlist.h"

class PlotDatalog : public QWidget
{
    Q_OBJECT

public:
    PlotDatalog(QWidget *pParent);
    ~PlotDatalog();

protected:
    void mousePressEvent(QMouseEvent *pEvent) override;
    void mouseMoveEvent(QMouseEvent *pEvent) override;
    void wheelEvent(QWheelEvent *pEvent) override;
    void paintEvent(QPaintEvent *) override;
    void resizeEvent(QResizeEvent* pEvent) override;
    void contextMenuEvent(QContextMenuEvent *pEvent) override;

public slots:
    void setDrawPoints(bool p);
    void setAntiAliasing(bool p);
    void addPointList(PointList* pList);
    void clearPointLists();
    void setDrawCurrentValueMarkers(bool draw);

signals:
    void mousePressEventSignal(QMouseEvent *pEvent);
    void mouseMoveEventSignal(QMouseEvent *pEvent);
    void wheelEventSignal(QWheelEvent *pEvent);
    void resizeEventSignal(QResizeEvent* pEvent);

private:
    void __inline__ dataPointMapToScreenY(double &yValue, double yAxisBoundMin, double yAxisBoundMax);
    void __inline__ dataPointMapToScreenX(double &xValue, double xAxisBoundMin, double xAxisBoundMax);

    const int currentValueMarkerLineThickness = 4;
    const int currentValueMarkerLineHalfLength = 5;

    const int lineWidth = 1;
    const int lineColorBetweenA = 70;
    const int lineColorBetweenB = 255;

    const int pointSize = 4;

    int marginTop = 3;
    int marginBottom = 3;
    int marginLeft = 0;
    int marginRight = 0;

    QList<PointList*> listPlots;
    bool drawPoints = false;
    bool drawCurrentValueMarkers = true;
    bool antiAliasing = false;
};
