#include "qne.h"

#include <QGridLayout>
#include <QToolButton>
#include <QToolBar>
#include <QInputDialog>
#include "Qne/qneport.h"

Qne::Qne(QWidget *pParent) : QWidget(pParent)
{
    m_QneScene = new QneQGraphicsScene(pParent);

    m_QneView = new QneQGraphicsView(this);
    m_QneView->setScene(m_QneScene);
    m_QneView->setRenderHint(QPainter::Antialiasing, true);
    //view->setFixedSize(width(), height());
    //_qneView->setSceneRect(0, 0, width(), height());
    //view->setHorizontalScrollBarPolicy(Qt::ScrollBarAlwaysOff);
    //view->setVerticalScrollBarPolicy(Qt::ScrollBarAlwaysOff);

    m_QneNodesEditor = new QneController(this);
    m_QneNodesEditor->install(m_QneScene);

    /// Tool Buttons ///
    QToolButton *btnAddNodeAddition = new QToolButton;
    btnAddNodeAddition->setText("+");
    btnAddNodeAddition->setToolTip("New node: addition");
    //btnAddNodeAddition->setIcon(QIcon(":/icons/images/Folder-document-open-icon.png"));

    QObject::connect(btnAddNodeAddition,
                     SIGNAL(clicked()),
                     this,
                     SLOT(btnAddNodeAdditionClicked()));

    QToolButton *btnAddNodeSubtract = new QToolButton;
    btnAddNodeSubtract->setText("−");
    btnAddNodeSubtract->setToolTip("New node: subtract");

    QObject::connect(btnAddNodeSubtract,
                     SIGNAL(clicked()),
                     this,
                     SLOT(btnAddNodeSubtractClicked()));

    QToolButton *btnAddNodeMultiply = new QToolButton;
    btnAddNodeMultiply->setText("×");
    btnAddNodeMultiply->setToolTip("New node: multiply");

    QObject::connect(btnAddNodeMultiply,
                     SIGNAL(clicked()),
                     this,
                     SLOT(btnAddNodeMultiplyClicked()));

    QToolButton *btnAddNodeDivide = new QToolButton;
    btnAddNodeDivide->setText("÷");
    btnAddNodeDivide->setToolTip("New node: subtract");

    QObject::connect(btnAddNodeDivide,
                     SIGNAL(clicked()),
                     this,
                     SLOT(btnAddNodeDivideClicked()));

    QToolButton *btnAddNodeConstant = new QToolButton;
    btnAddNodeConstant->setText("c");
    btnAddNodeConstant->setToolTip("New node: constant");

    QObject::connect(btnAddNodeConstant,
                     SIGNAL(clicked()),
                     this,
                     SLOT(btnAddNodeConstantClicked()));

    QToolButton *btnAddNodeIfLessThan = new QToolButton;
    btnAddNodeIfLessThan->setText("<");
    btnAddNodeIfLessThan->setToolTip("New node: if less than");

    QObject::connect(btnAddNodeIfLessThan,
                     SIGNAL(clicked()),
                     this,
                     SLOT(btnAddNodeLessThanClicked()));

    QToolButton *btnAddNodeIfMoreThan = new QToolButton;
    btnAddNodeIfMoreThan->setText(">");
    btnAddNodeIfMoreThan->setToolTip("New node: if more than");

    QObject::connect(btnAddNodeIfMoreThan,
                     SIGNAL(clicked()),
                     this,
                     SLOT(btnAddNodeMoreThanClicked()));

    QToolButton *btnAddNodeEqualTo = new QToolButton;
    btnAddNodeEqualTo->setText("==");
    btnAddNodeEqualTo->setToolTip("New node: if equal to");

    QObject::connect(btnAddNodeEqualTo,
                     SIGNAL(clicked()),
                     this,
                     SLOT(btnAddNodeEqualToClicked()));

    QToolButton *btnAddNodeNotEqualTo = new QToolButton;
    btnAddNodeNotEqualTo->setText("!=");
    btnAddNodeNotEqualTo->setToolTip("New node: if not equal to");

    QObject::connect(btnAddNodeNotEqualTo,
                     SIGNAL(clicked()),
                     this,
                     SLOT(btnAddNodeNotEqualToClicked()));

    /// Toolbar ///
    QToolBar *toolbar = new QToolBar("toolbar", this);
    toolbar->setIconSize(QSize(16, 16));
    toolbar->addWidget(btnAddNodeAddition);
    toolbar->addWidget(btnAddNodeSubtract);
    toolbar->addSeparator();
    toolbar->addWidget(btnAddNodeMultiply);
    toolbar->addWidget(btnAddNodeDivide);
    toolbar->addSeparator();
    toolbar->addWidget(btnAddNodeIfLessThan);
    toolbar->addWidget(btnAddNodeIfMoreThan);
    toolbar->addSeparator();
    toolbar->addWidget(btnAddNodeEqualTo);
    toolbar->addWidget(btnAddNodeNotEqualTo);
    toolbar->addSeparator();
    toolbar->addWidget(btnAddNodeConstant);

    /// Layout ///
    QGridLayout *contentLayout = new QGridLayout(this);
    contentLayout->setSpacing(0);
    contentLayout->setContentsMargins(0, 0, 0, 0);
    contentLayout->addWidget(toolbar);
    contentLayout->addWidget(m_QneView);
}

void Qne::addQneBlock(QneBlockOperationType type, double value)
{
    _addQneBlock(m_QneScene, type, value);
}

void Qne::_addQneBlock(QneQGraphicsScene *pScene, QneBlockOperationType type, double value)
{
    QneBlock *block = new QneBlock(0);
    pScene->addItem(block);

    switch (type) {
    case QneBlockOperationType::Add:
        block->addPort("Add", 0, QnePort::NamePort);
        block->addInputPort("In 1");
        block->addInputPort("In 2");
        block->addOutputPort("Out");
        break;

    case QneBlockOperationType::Subract:
        block->addPort("Subtract", 0, QnePort::NamePort);
        block->addInputPort("In 1");
        block->addInputPort("In 2");
        block->addOutputPort("Out");
        break;

    case QneBlockOperationType::Divide:
        block->addPort("Divide", 0, QnePort::NamePort);
        block->addInputPort("In 1");
        block->addInputPort("In 2");
        block->addOutputPort("Out");
        break;

    case QneBlockOperationType::Multiply:
        block->addPort("Multiply", 0, QnePort::NamePort);
        block->addInputPort("In 1");
        block->addInputPort("In 2");
        block->addOutputPort("Out");
        break;

    case QneBlockOperationType::Constant:
        block->addPort("Constant", 0, QnePort::NamePort);
        block->addPort(QString::number(value), 0, QnePort::TypePort);
        block->addOutputPort("Out");
        break;

    case QneBlockOperationType::LessThan:
        block->addPort("Less Than", 0, QnePort::NamePort);
        block->addPort(QString::number(value), 0, QnePort::TypePort);
        block->addInputPort("In");
        block->addOutputPort("True");
        block->addOutputPort("False");
        break;

    case QneBlockOperationType::MoreThan:
        block->addPort("More Than", 0, QnePort::NamePort);
        block->addPort(QString::number(value), 0, QnePort::TypePort);
        block->addInputPort("In");
        block->addOutputPort("True");
        block->addOutputPort("False");
        break;

    case QneBlockOperationType::EqualTo:
        block->addPort("Equal To", 0, QnePort::NamePort);
        block->addPort(QString::number(value), 0, QnePort::TypePort);
        block->addInputPort("In");
        block->addOutputPort("True");
        block->addOutputPort("False");
        break;

    case QneBlockOperationType::NotEqualTo:
        block->addPort("Not Equal To", 0, QnePort::NamePort);
        block->addPort(QString::number(value), 0, QnePort::TypePort);
        block->addInputPort("In");
        block->addOutputPort("True");
        block->addOutputPort("False");
        break;

    default:
        break;
    }
}

void Qne::btnAddNodeAdditionClicked()
{
    addQneBlock(QneBlockOperationType::Add);
}

void Qne::btnAddNodeSubtractClicked()
{
    addQneBlock(QneBlockOperationType::Subract);
}

void Qne::btnAddNodeMultiplyClicked()
{
    addQneBlock(QneBlockOperationType::Multiply);
}

void Qne::btnAddNodeDivideClicked()
{
    addQneBlock(QneBlockOperationType::Divide);
}

void Qne::btnAddNodeConstantClicked()
{
    bool ok;
    double d = QInputDialog::getDouble(this, tr("New node: constant"),
                                     tr("Value:"), 0, -2147483647, 2147483647, 5, &ok, Qt::WindowTitleHint);
    if (ok)
    {
        addQneBlock(QneBlockOperationType::Constant, d);
    }
}

void Qne::btnAddNodeLessThanClicked()
{
    bool ok;
    double d = QInputDialog::getDouble(this, tr("New node: if less than"),
                                     tr("Value:"), 0, -2147483647, 2147483647, 5, &ok, Qt::WindowTitleHint);
    if (ok)
    {
        addQneBlock(QneBlockOperationType::LessThan, d);
    }
}

void Qne::btnAddNodeMoreThanClicked()
{
    bool ok;
    double d = QInputDialog::getDouble(this, tr("New node: if more than"),
                                     tr("Value:"), 0, -2147483647, 2147483647, 5, &ok, Qt::WindowTitleHint);
    if (ok)
    {
        addQneBlock(QneBlockOperationType::MoreThan, d);
    }
}

void Qne::btnAddNodeEqualToClicked()
{
    bool ok;
    double d = QInputDialog::getDouble(this, tr("New node: if equal to"),
                                     tr("Value:"), 0, -2147483647, 2147483647, 5, &ok, Qt::WindowTitleHint);
    if (ok)
    {
        addQneBlock(QneBlockOperationType::EqualTo, d);
    }
}

void Qne::btnAddNodeNotEqualToClicked()
{
    bool ok;
    double d = QInputDialog::getDouble(this, tr("New node: if not equal to"),
                                     tr("Value:"), 0, -2147483647, 2147483647, 5, &ok, Qt::WindowTitleHint);
    if (ok)
    {
        addQneBlock(QneBlockOperationType::NotEqualTo, d);
    }
}
