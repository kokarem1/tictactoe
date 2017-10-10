#ifndef GAMESERVER_H
#define GAMESERVER_H

#include <QObject>

class QNetworkSession;
class QTcpServer;

#include "gamenetworkservicebase.h"

class GameServer: public GameNetworkServiceBase
{
    Q_OBJECT

    typedef GameNetworkServiceBase base;

public:
    GameServer(QObject *parent);
    void startServer();
    void sendFirstMove();

private slots:
    void on_sessionOpened();
    void on_newConnection();
    void on_disconnected();
    void on_readyRead();

private:
    QNetworkSession *m_pSession;
    QTcpServer *m_pServer;
    int activeConnectionsCount;
};

#endif // GAMESERVER_H
