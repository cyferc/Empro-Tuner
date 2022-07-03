#pragma once

#include <QWidget>

#include "Qne/qnodeseditor.h"
#include "Qne/qneqgraphicsscene.h"
#include "Qne/qneqgraphicsview.h"
#include "Qne/qneblock.h"

class Qne : public QWidget
{
    Q_OBJECT
public:
    enum class QneBlockOperationType {
        Add,
        Subract,
        Multiply,
        Divide,
        Constant,
        LessThan,
        MoreThan,
        EqualTo,
        NotEqualTo,
        Blank
    };

    explicit Qne(QWidget *pParent);
    void addQneBlock(QneBlockOperationType type, double value = 0.0);

signals:

private slots:
    void btnAddNodeAdditionClicked();
    void btnAddNodeSubtractClicked();
    void btnAddNodeMultiplyClicked();
    void btnAddNodeDivideClicked();
    void btnAddNodeConstantClicked();
    void btnAddNodeLessThanClicked();
    void btnAddNodeMoreThanClicked();
    void btnAddNodeEqualToClicked();
    void btnAddNodeNotEqualToClicked();

private:
    QneController* m_QneNodesEditor;
    QneQGraphicsView *m_QneView;
    QneQGraphicsScene *m_QneScene;

    void _addQneBlock(QneQGraphicsScene *pScene, QneBlockOperationType type, double value = 0.0);
};
