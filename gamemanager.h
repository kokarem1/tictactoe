#ifndef GAMEMANAGER_H
#define GAMEMANAGER_H

#include <QObject>
#include <QVector>

class GameField;

class GameManager: public QObject
{
    Q_OBJECT

public:
    enum GameOverReason {
        NoughtsWins,
        CrossesWins,
        GamefieldIsFull
    };

    GameManager(/*GameField *gameField, */QObject *parent = Q_NULLPTR);
    void setGamefield(GameField *gameField);

signals:
    void gameOver(int gameOverReason);

private:
    GameField *gameField;
    const QVector< QVector<int> > winConditions =
    {
        {0,1,2},
        {3,4,5},
        {6,7,8},
        {0,3,6},
        {1,4,7},
        {2,5,8},
        {0,4,8},
        {2,4,6}
    };

private slots:
    void on_cellChanged();
};

#endif // GAMEMANAGER_H
