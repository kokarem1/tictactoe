#ifndef GAMEFIELD_H
#define GAMEFIELD_H

#include <QObject>

class QWidget;
class QGridLayout;
class QEvent;
class QWidget;

#include <QVector>
#include <QPushButton>

class GameField: public QObject
{
    Q_OBJECT

    const QString BLANK_STR = QString::fromLocal8Bit("");
    const QString NOUGHT_STR = QString::fromLocal8Bit("O");
    const QString CROSS_STR = QString::fromLocal8Bit("X");

public:
    const int FIELD_SIZE = 9;

    enum CellState {
        Blank, Nought, Cross, Invalid
    };

    enum PlayerSide {
        Noughts, Crosses
    };

    GameField(QWidget *parent);
    virtual ~GameField();
    virtual bool eventFilter(QObject *watched, QEvent *event);
    CellState getCell (int index);
    void setCell (int index, CellState state);
    PlayerSide getSide();
    void setSide (PlayerSide side);
    void clearField ();
    void block ();
    void unblock ();
    void setFieldEnabled(bool enabled);

private:
    QWidget *parentWidget;
    QVector<QPushButton *> *fieldButtons;
    QGridLayout *layout;
    PlayerSide side;
    bool blocked;

    CellState textToCellState (const QString &text);

private slots:
    void on_clicked();

signals:
    void cellChangedByUser(int index, CellState state);
    void cellChanged();
    void newSide(int side);
};

#endif // GAMEFIELD_H
