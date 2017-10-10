#ifndef GAMENETWORKSERVICEBASE_H
#define GAMENETWORKSERVICEBASE_H

#include <QObject>

class QNetworkSession;
class QTcpSocket;
#include "gamefield.h"

class GameNetworkServiceBase: public QObject
{
    Q_OBJECT

public:
    GameNetworkServiceBase(QObject *parent,
                           const QString& hostName=QString::fromLocal8Bit("127.0.0.1"),
                           quint16 port = 45569);
    virtual ~GameNetworkServiceBase();
    QTcpSocket* getSocket();
    void setGameField(GameField *gameField);
    void restartGame();
    void sendRestart();

protected slots:
    virtual void on_cellChanged(int index, GameField::CellState state);

protected:
    QTcpSocket *m_pSocket;
    GameField *gameField;

    QString hostName;
    quint16 port;
};

#endif // GAMENETWORKSERVICEBASE_H
