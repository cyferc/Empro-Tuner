#pragma once

#include <QWidget>
#include <QColor>

class RoundDial : public QWidget
{
    Q_OBJECT

public:
    typedef QVector< QPointF > RangeList;
    explicit RoundDial(QWidget *parent);
    ~RoundDial();

    void SetRange( qreal min, qreal max );
    void SetAngle( qreal angle = 200 );

    void SetStep( qreal step );

    qreal Step ();
    qreal Value();

    void EnableSaffetyRanges( bool use = true );

    RangeList GreenRanges();
    RangeList YellowRanges();
    RangeList RedRanges();

    void SetGreenRanges( RangeList const& ranges );
    void SetYellowRanges( RangeList const& ranges );
    void SetRedRanges( RangeList const& ranges );

    QString Units();
    void SetUnits( QString const& u );
public slots:
    void update_value( qreal val );

protected:
    void paintEvent(QPaintEvent *);

    qreal m_Min; //минимальное значение
    qreal m_Max; //максимальное значение
    qreal m_Value; //текущее значение
    qreal m_Step; // цена деления
    QString m_Units; //еденицы измерения

    qreal m_MinAngle; //минимальный угол на табло
    qreal m_MaxAngle; //максимальный угол на табло

    bool m_UseSafetyRanges;

    RangeList m_GreenRanges;
    RangeList m_YellowRanges;
    RangeList m_RedRanges;

    QColor m_CurrentValueColor = QColor(255, 255, 255);
    QColor m_ArrowColor = QColor(255, 255, 0);
    QColor m_DialOutlineColor = QColor(255, 255, 255);
    QColor m_DialBackFaceColor = QColor(50, 50, 50);

    QColor m_MajorTickTextColor = QColor(255, 255, 255);
    QColor m_MajorTickColor = QColor(255, 255, 255);
    QColor m_MidTickColor = QColor(200, 200, 200);
    QColor m_MinorTickColor = QColor(150, 150, 150);

    double m_ArrowThickness = 0.04; // of radius
    double m_ArrowLength = 0.9; // of radius
    double m_SafetyRangesThicknes = 0.91;
};
