#ifndef GAMECLIENT_H
#define GAMECLIENT_H

#include <QObject>

class QNetworkSession;
class QTcpSocket;
#include "gamefield.h"
#include "gamenetworkservicebase.h"

class GameClient: public GameNetworkServiceBase
{
    Q_OBJECT

    typedef GameNetworkServiceBase base;

public:
    GameClient(QObject *parent);
    void connectToServer();

private slots:
    void on_sessionOpened();
    void on_connected();
    void on_disconnected();
    void on_readyRead();

private:
    QNetworkSession *m_pSession;
};

#endif // GAMECLIENT_H
