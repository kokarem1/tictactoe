#include "gameserver.h"

#include "createnetsession.h"
#include "mainwindow.h"

#include <QTcpServer>
#include <QTcpSocket>
#include <QString>
#include <QNetworkSession>
#include <QTcpServer>
#include <QDataStream>

#include <time.h>

GameServer::GameServer(QObject *parent)
    :
      base(parent)
    , activeConnectionsCount (0)
{
    //
//    connect(
//    m_pButtonClose, SIGNAL(clicked()), this, SLOT(close()));
    //
    m_pSession = createNetworkSession(this);
    if (m_pSession)
    {
        connect(
            m_pSession, &QNetworkSession::opened,
            this, &GameServer::on_sessionOpened);
        m_pSession->open();
    }
    else
        on_sessionOpened();
}

void GameServer::startServer()
{
    if (m_pServer->isListening())
        m_pServer->close();

    if (!m_pServer->listen(QHostAddress::Any, 45569))
    {
        QString strErrors;// = m_pTextEditErrors->toPlainText();
        strErrors += QString("Error: ") + m_pServer->errorString() + '\n';
        //
        qDebug () << strErrors;
//        m_pTextEditErrors->setPlainText(strErrors);
    }
}

void GameServer::sendFirstMove()
{
    srand(time(0));
    int mySide = rand() % 2;
    gameField->setSide((GameField::PlayerSide) mySide);
    QDataStream inout(m_pSocket);
    QString message = QString::fromLocal8Bit("firstMove.%1").arg(mySide? 0: 1);
    inout << message;
}

void GameServer::on_sessionOpened()
{
    m_pServer = new QTcpServer(this);
    connect(
        m_pServer, &QTcpServer::newConnection,
        this, &GameServer::on_newConnection);
}

void GameServer::on_newConnection()
{
    QTcpSocket *pClientSocket = m_pServer->nextPendingConnection();

    if (activeConnectionsCount >= 1) {
        QDataStream inout(pClientSocket);
        inout << QString::fromLocal8Bit ("bussy");
        pClientSocket->disconnectFromHost();
        return;
    }

    connect(
        pClientSocket, &QAbstractSocket::disconnected,
        this, &GameServer::on_disconnected);
    connect(
        pClientSocket, &QAbstractSocket::disconnected,
        pClientSocket, &QObject::deleteLater);
    connect(
        pClientSocket, &QIODevice::readyRead,
        this, &GameServer::on_readyRead);

    MainWindow *mw = qobject_cast<MainWindow *>(parent());

    mw->on_networkConnected();

    connect(pClientSocket, &QTcpSocket::disconnected,
            mw, &MainWindow::on_networkDisconnected);

    m_pSocket = pClientSocket;

    sendFirstMove();
    gameField->setFieldEnabled(true);

    ++activeConnectionsCount;
}

void GameServer::on_disconnected()
{
    --activeConnectionsCount;

    restartGame();
}

void GameServer::on_readyRead()
{
    QTcpSocket *pClientSocket =
        qobject_cast <QTcpSocket *> (sender());
    if (!pClientSocket)
        return;
    //
    QDataStream inout(m_pSocket);
    inout.startTransaction();
    QString message;
    inout >> message;
    if (!inout.commitTransaction())
        return;
    //
    qDebug () << message;
    QStringList messageParts = message.split(QString::fromLocal8Bit("."));
    const QString &messageType = messageParts.first();
    if (messageType == QString::fromLocal8Bit("firstMove")) {
        int side = messageParts[1].toInt();
        gameField->setSide((GameField::PlayerSide) side);
    } else if (messageType == QString::fromLocal8Bit("move")) {
        for (int i = 0; i < gameField->FIELD_SIZE; ++i) {
            int cellState = messageParts[i+1].toInt();
            gameField->setCell(i, (GameField::CellState) cellState);
        }
        emit gameField->cellChanged();

        gameField->unblock();
    } else if (messageType == QString::fromLocal8Bit("restart")) {
        restartGame();
        sendFirstMove();
    }
} // on_readyRead()
