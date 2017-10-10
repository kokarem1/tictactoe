#include "gameclient.h"

#include "createnetsession.h"
#include "mainwindow.h"

#include <QTcpSocket>
#include <QNetworkSession>
#include <QTcpSocket>
#include <QDataStream>
#include <QStringList>

GameClient::GameClient(QObject *parent)
    :
      base(parent)
{
    //
//    connect(
//        m_pButtonClose, SIGNAL(clicked()), this, SLOT(close()));
    //
    m_pSession = createNetworkSession(this);
    if (m_pSession)
    {
        connect(
            m_pSession, &QNetworkSession::opened,
            this, &GameClient::on_sessionOpened);
            m_pSession->open();
    }
    else
        on_sessionOpened();
}

void GameClient::connectToServer()
{
    if (m_pSocket->state() != QAbstractSocket::UnconnectedState)
        m_pSocket->abort();

    m_pSocket->connectToHost(
        hostName,
        port);

    if (m_pSocket->waitForConnected(3000)) {
        gameField->unblock();
        gameField->setFieldEnabled(true);
    }
}

void GameClient::on_sessionOpened()
{
    m_pSocket = new QTcpSocket(this);
    connect(
        m_pSocket, &QAbstractSocket::connected,
        this, &GameClient::on_connected);
    connect(
        m_pSocket, &QAbstractSocket::disconnected,
        this, &GameClient::on_disconnected);
    connect(
        m_pSocket, &QIODevice::readyRead,
        this, &GameClient::on_readyRead);

    MainWindow *mw = qobject_cast<MainWindow *>(parent());
    connect(m_pSocket, &QTcpSocket::connected,
            mw, &MainWindow::on_networkConnected);
    connect(m_pSocket, &QTcpSocket::disconnected,
            mw, &MainWindow::on_networkDisconnected);
}

void GameClient::on_connected()
{
    gameField->block();
//    QDataStream inout(m_pSocket);
//    inout << QString("m_pTextEditLine->toPlainText()");
    //    inout << QString("Hello");
}

void GameClient::on_disconnected()
{
    restartGame();
}

void GameClient::on_readyRead()
{
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
        sendRestart();
    }
}
