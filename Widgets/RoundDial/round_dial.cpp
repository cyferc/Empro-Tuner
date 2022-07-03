#include "round_dial.h"
#include <QPainter>
#include <QTimer>
#include <QTime>
#include <QTextItem>
#include <math.h>

#ifndef M_PI
#define M_PI		3.14159265358979323846
#endif

RoundDial::RoundDial(QWidget *parent) :
    QWidget(parent),
    m_Min(-100),
    m_Max(100),
    m_Value(0),
    m_Step( 1 ),
    m_MinAngle(-150),
    m_MaxAngle(150),
    m_UseSafetyRanges( false )
{
    QPalette pal = palette();
    pal.setColor(QPalette::Background, Qt::black);
    setPalette(pal);
    setAutoFillBackground(true);
}

RoundDial::~RoundDial()
{
}

void RoundDial::paintEvent( QPaintEvent* /*e */)
{
    auto radius = height() / ( sin( (m_MaxAngle +10 - 90.0)/180*M_PI ) + 1 );
    radius = qMin( width()/2.0, radius );

    auto hord_height = sin( (m_MaxAngle +10 - 90.0)/180*M_PI ) * radius;
    auto w = 2 * radius;
    //auto h = radius + hord_height;
    const QPoint nubmer_point( 0, -(radius - radius*0.11) );

    const QPoint arrow[3] = {
        QPoint( radius * m_ArrowThickness, 0),
        QPoint( -radius * m_ArrowThickness, 0),
        QPoint(0, -radius * m_ArrowLength)
    };

    const QPoint lile_risk[2] = {
        QPoint( 0, -radius ),
        QPoint( 0, -(radius - radius*0.04) )
    };
    const QPoint mid_risk[2] = {
        QPoint( 0, -radius ),
        QPoint( 0, -(radius - radius*0.07) )
    };
    const QPoint big_risk[2] = {
        QPoint( 0, -radius ),
        QPoint( 0, -(radius - radius*0.09) )
    };

    //QPoint window_center( radius,  radius );
    QPoint window_center(  qMax( static_cast<double>( width()/2 ), radius),  qMax( static_cast<double>( height()/2 ), radius) );
    auto border = QRectF( -radius, -radius, w, w );

    QPainter painter( this );
    painter.setRenderHint(QPainter::Antialiasing);

    //painter.drawRect(rect());
    painter.translate( window_center );
    //painter.drawRect(border);

    painter.setFont( font() );
    QFontMetrics metrix( font() );

    qreal step = ( m_MaxAngle - m_MinAngle )/ (m_Max - m_Min) * m_Step;

    // Draw dial back face
    {
        painter.save();
        auto angle = qMax( qAbs(m_MinAngle), qAbs(m_MaxAngle) ) + 10;

        if ( angle > 180 )
            angle -= 10;

        painter.setPen( Qt::NoPen );   // border
        painter.setBrush( m_DialBackFaceColor ); // fill
        painter.drawChord( border, (90-angle) * 16,  2 * angle *16 );
        painter.restore();
    }

    // Draw safety ranges
    if ( m_UseSafetyRanges )
    {
        auto draw_zones = [&]( RangeList const& range, QColor color )
        {
            foreach (auto zone, range)
            {
                painter.save();
                auto start_angle = m_MaxAngle - ( qMax( zone.x(), m_Min )  - m_Min ) * step / m_Step;
                auto stop_angle = m_MaxAngle - ( qMax( zone.y(), m_Min )  - m_Min ) * step / m_Step;

                painter.setPen( Qt::NoPen );
                painter.setBrush( color );
                painter.drawPie( border, (start_angle+90)*16, (stop_angle - start_angle)*16 );

                painter.restore();
            }
        };

        QPainterPath path;
        path.addRect(0, 0, radius, radius);
        path.addEllipse(QPointF(0.0, 0.0), radius * m_SafetyRangesThicknes, radius * m_SafetyRangesThicknes);

        painter.setClipPath(path);
        painter.setClipping(true);

        draw_zones( m_GreenRanges, Qt::green );
        draw_zones( m_YellowRanges, Qt::yellow );
        draw_zones( m_RedRanges, Qt::red );

        painter.setClipping(false);
    }

    // Draw ticks and tick text
    for ( qreal angle = m_MinAngle; round( angle * 100 )/100 <= m_MaxAngle; angle += step )
    {
        painter.save();
        painter.rotate( angle );

        qint32 ofset = ceil( round( ( ( angle - m_MinAngle ) / step ) * 100 ) / 100 );

        if ( !( ofset % 10 ) )
        {
            painter.setPen( m_MajorTickColor );
            painter.drawLine( big_risk[0], big_risk[1] );
        }
        else if ( !( ofset % 5 ) )
        {
            painter.setPen( m_MidTickColor );
            painter.drawLine( mid_risk[0], mid_risk[1] );
        }
        else
        {
            painter.setPen( m_MinorTickColor );
            painter.drawLine( lile_risk[0], lile_risk[1] );
        }

        if ( !( ofset % 10 ) )
        {
            painter.translate( nubmer_point );
            QString text = QString::number( m_Min + ofset * m_Step, 'f', 0);

//            painter.rotate( 90 );

//            int x = 0;
//            int y = 0;

            int x = -metrix.width( text )/2;
            int y = metrix.height()/2;

            painter.setPen( m_MajorTickTextColor );
            painter.drawText( x, y, text );
        }

        painter.restore();
    }

    // Draw dial outline
    {
        painter.save();
        auto angle = qMax( qAbs(m_MinAngle), qAbs(m_MaxAngle) ) + 10;

        if ( angle > 180 )
            angle -= 10;

        painter.setPen( m_DialOutlineColor );
        painter.drawChord( border, (90-angle) * 16,  2 * angle *16 );
        painter.restore();
    }

    // Draw arrow
    {
        painter.save();
        painter.setPen( m_ArrowColor );   // border
        painter.setBrush( m_ArrowColor ); // fill

        qreal pos = m_Value - m_Min;
        painter.rotate( m_MinAngle + pos * step / m_Step  );
        painter.drawConvexPolygon( arrow, 3 );
        painter.restore();
    }

    // Draw current value text
    {
        QString text = QString::number( m_Value, 'f', 0) + " " + m_Units;
        auto width = metrix.width( text );
        auto max_height = hord_height;

        QFont font = painter.font() ;

        // twice the size than the current font size
        font.setPointSize(font.pointSizeF() * 2);

        // set the modified font to the painter
        painter.setFont(font);

        painter.setPen( m_CurrentValueColor );

        painter.drawText( -width/2, max_height - 3 - metrix.height()/2, text );
    }
}


QString RoundDial::Units()
{
    return m_Units;
}
void RoundDial::SetUnits( QString const& u )
{
    m_Units = u;
}

void RoundDial::SetRange( qreal min, qreal max )
{
    m_Min = min;
    m_Max = max;

    update_value( m_Value );
}
void RoundDial::SetAngle( qreal angle )
{
    angle = qMax ( angle, 200.0 );
    m_MinAngle = -angle / 2;
    m_MaxAngle = angle / 2;

    update_value( m_Value );
}

void RoundDial::SetStep( qreal step )
{
    m_Step = step;

    update();
}
qreal RoundDial::Step ()
{
    return m_Step;
}
qreal RoundDial::Value()
{
    return m_Value;
}
void RoundDial::EnableSaffetyRanges( bool use )
{
    m_UseSafetyRanges = use;
}
RoundDial::RangeList RoundDial::GreenRanges()
{
    return m_GreenRanges;
}
RoundDial::RangeList RoundDial::YellowRanges()
{
    return m_YellowRanges;
}
RoundDial::RangeList RoundDial::RedRanges()
{
    return m_RedRanges;
}

void RoundDial::SetGreenRanges( RangeList const& ranges )
{
    m_GreenRanges = ranges;
}
void RoundDial::SetYellowRanges( RangeList const& ranges )
{
    m_YellowRanges = ranges;
}
void RoundDial::SetRedRanges( RangeList const& ranges )
{
    m_RedRanges = ranges;
}

void RoundDial::update_value( qreal val )
{
    if ( val > m_Min )
    {
        if ( val < m_Max)
        {
            m_Value = val;
        }
        else
            m_Value = m_Max;
    }
    else
        m_Value = m_Min;
    update();
}
