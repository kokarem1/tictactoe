#include "gamemanager.h"

#include "gamefield.h"

#include <QDebug>

GameManager::GameManager(/*GameField *gameField, */QObject *parent):
    QObject(parent)
//  , gameField(gameField)
{
//    connect(this->gameField, &GameField::cellChanged,
    //            this, &GameManager::on_cellChanged);
}

void GameManager::setGamefield(GameField *gameField)
{
    this->gameField = gameField;
    connect(this->gameField, &GameField::cellChanged,
            this, &GameManager::on_cellChanged);
}


void GameManager::on_cellChanged()
{
    for (int checkingCellState = GameField::Nought; checkingCellState <= GameField::Cross; ++checkingCellState)
        for (QVector<QVector<int>>::const_iterator it = winConditions.cbegin();
             it != winConditions.end(); ++it) {
            const QVector<int>& winCondition = *it;
            int cellsCountInWinCondition = winCondition.size();
            QVector<int> checkingVector(cellsCountInWinCondition);
            QVector<int> cellsVector(cellsCountInWinCondition);
            for (int i = 0; i < winCondition.size(); ++i) {
                checkingVector[i] = checkingCellState;
                cellsVector[i] = gameField->getCell(winCondition[i]);
            }
            if (checkingVector == cellsVector) {
                GameOverReason gameOverReason = checkingCellState == GameField::Nought?
                            NoughtsWins: CrossesWins;
                emit gameOver(gameOverReason);
                qDebug() << "game Over: " << gameOverReason;
                return;
            }
        }


    bool gameFieldIsFull = true;

    for (int i = 0; i < gameField->FIELD_SIZE; ++i) {
        GameField::CellState cellState = gameField->getCell(i);
        if (cellState == GameField::Blank || cellState == GameField::Invalid)
        {
            gameFieldIsFull = false;
            break;
        }
    }

    if (gameFieldIsFull) {
        emit gameOver(GamefieldIsFull);
        qDebug () << "GamefieldIsFull";
    }
}
