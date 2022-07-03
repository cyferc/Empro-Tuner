
#pragma once

#include <QLabel>

class VerticalLabel : public QLabel
{
    Q_OBJECT

public:
    explicit VerticalLabel(QWidget *pParent);
    explicit VerticalLabel(const QString &text, QWidget *pParent);

protected:
    void paintEvent(QPaintEvent*);
    QSize sizeHint() const ;
    QSize minimumSizeHint() const;
};
