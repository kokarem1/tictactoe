#include "gamenetworkservicebase.h"

#include "createnetsession.h"

#include <QTcpSocket>
#include <QNetworkSession>
#include <QTcpSocket>
#include <QDataStream>
#include <QStringList>

GameNetworkServiceBase::GameNetworkServiceBase(QObject *parent, const QString &hostName, quint16 port)
    :
      QObject(parent)
    , hostName (hostName)
    , port (port)
{
}

GameNetworkServiceBase::~GameNetworkServiceBase()
{
}

QTcpSocket *GameNetworkServiceBase::getSocket()
{
    return m_pSocket;
}

void GameNetworkServiceBase::setGameField(GameField *gameField)
{
    this->gameField = gameField;
    this->gameField->disconnect(this->gameField, &GameField::cellChangedByUser, 0, 0);
    connect(gameField, &GameField::cellChangedByUser,
            this, &GameNetworkServiceBase::on_cellChanged);
}

void GameNetworkServiceBase::restartGame()
{
    gameField->setFieldEnabled(true);
    gameField->block();
    gameField->clearField();
}

void GameNetworkServiceBase::sendRestart()
{
    restartGame();
    QDataStream inout(m_pSocket);
    QString message = QString::fromLocal8Bit("restart");
    inout << message;
}

void GameNetworkServiceBase::on_cellChanged(int index, GameField::CellState state)
{
    QDataStream inout(m_pSocket);
    QStringList messageParts;
    messageParts << QString::fromLocal8Bit("move");
    for (int i = 0; i < gameField->FIELD_SIZE; ++i) {
        messageParts << QString::number(gameField->getCell(i));
    }
    QString message = messageParts.join(QString::fromLocal8Bit("."));
    inout << message;
}
