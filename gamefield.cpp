#include "gamefield.h"

#include <QGridLayout>
#include <QtMath>
#include <QAbstractButton>
#include <QFont>
#include <QFontMetrics>
#include <QEvent>
#include <QWidget>

#include <QDebug>

GameField::GameField(QWidget *parent)
    :
      fieldButtons (new QVector<QPushButton *> (FIELD_SIZE))
    , layout (new QGridLayout)
    , parentWidget (new QWidget)
    , side (Noughts)
    , blocked (false)
{
    int dimSize = qSqrt(FIELD_SIZE);

    for (int i = 0; i < dimSize; ++i)
        for (int j = 0; j < dimSize; ++j)
        {
            QPushButton *button = new QPushButton("");
            connect (button, &QAbstractButton::clicked,
                     this, &GameField::on_clicked);

            button->setMinimumSize(20, 20);
            button->setMaximumSize(2000,2000);
            button->installEventFilter(this);
            fieldButtons->operator [](i*dimSize+j) = button;
            layout->addWidget (button, i, j);
        }

//    QLayout *oldLayout = parent->layout();
//    if (oldLayout)
//        delete oldLayout;

    parentWidget->setLayout(layout);
    parent->layout()->addWidget(parentWidget);
    parentWidget->setEnabled(false);
}

GameField::~GameField()
{
    if (fieldButtons)
        delete fieldButtons;
}

bool GameField::eventFilter(QObject *watched, QEvent *event)
{
    QPushButton *button;
    if ((button = qobject_cast<QPushButton *>(watched)) != 0
            && event->type() == QEvent::Resize) {
        QFont font = button->font();
        font.setPointSize(0.33 * button->height());
        button->setFont(font);
    }
}

GameField::CellState GameField::textToCellState (const QString &text)
{
    if (text == BLANK_STR)
        return CellState::Blank;
    else if (text == NOUGHT_STR)
        return CellState::Nought;
    else if (text == CROSS_STR)
        return CellState::Cross;
    else
        return CellState::Invalid;
}

GameField::CellState GameField::getCell (int index)
{
    QString text = fieldButtons->at(index)->text ();

    return textToCellState (text);
}

void GameField::setCell (int index, GameField::CellState state)
{
    QString text;

    switch (state) {
    case CellState::Blank:
        text = BLANK_STR;
        break;
    case CellState::Nought:
        text = NOUGHT_STR;
        break;
    case CellState::Cross:
        text = CROSS_STR;
        break;
    default:
        return;
    }

    fieldButtons->operator [](index)->setText(text);
}

GameField::PlayerSide GameField::getSide()
{
    return side;
}

void GameField::setSide(GameField::PlayerSide side)
{
    this->side = side;

    if (side == Noughts)
        block();
    else
        unblock();

    emit newSide(side);
}

void GameField::clearField ()
{
    for (int i = 0; i < FIELD_SIZE; ++i)
        fieldButtons->operator [](i)->setText(BLANK_STR);
}

void GameField::block()
{
    blocked = true;

//    parentWidget->setEnabled(false);
}

void GameField::unblock()
{
    blocked = false;

    //    parentWidget->setEnabled(true);
}

void GameField::setFieldEnabled(bool enabled)
{
    parentWidget->setEnabled(enabled);
}

void GameField::on_clicked()
{
    if (blocked)
        return;

    QPushButton *pButton =
        qobject_cast <QPushButton *> (sender());

    if (pButton->text() == BLANK_STR) {
        pButton->setText (side == Noughts? NOUGHT_STR: CROSS_STR);
        int cellIndex = fieldButtons->indexOf(pButton);
        qDebug () << cellIndex << getCell (cellIndex);
        emit cellChangedByUser(cellIndex, getCell (cellIndex));
        emit cellChanged();

        block();
    }
}

// GameField()
