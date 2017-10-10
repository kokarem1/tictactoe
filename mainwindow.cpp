#include "mainwindow.h"
#include "ui_mainwindow.h"

#include "gameserver.h"
#include "gameclient.h"
#include "gamefield.h"
#include "gamemanager.h"

#include <QString>
#include <QTcpSocket>
#include <QMessageBox>

#include <QDebug>

MainWindow::MainWindow(QWidget *parent) :
    QMainWindow(parent),
    ui(new Ui::MainWindow)
{
    ui->setupUi(this);

    qDebug () << "Hello";

    gameManager = new GameManager (this);
    connect(gameManager, &GameManager::gameOver,
            this, &MainWindow::on_gameOver);
    gameField = new GameField (centralWidget());
    gameManager->setGamefield(gameField);

//    server->setGameField(gameField);
    server = new GameServer(this);
    client = new GameClient(this);
    client->setGameField(gameField);
    connect(gameField, &GameField::newSide,
            this, &MainWindow::on_newSide);

//  TEST
//    gameField->setCell(1, GameField::Cross);
//    gameField->setCell(5, GameField::Nought);

    qDebug () << gameField->getCell(0);
}

MainWindow::~MainWindow()
{
    delete ui;
}

void MainWindow::on_actionCreate_triggered()
{
    isServer = true;
    server->setGameField(gameField);
    server->startServer();
}

void MainWindow::on_actionJoin_triggered()
{
    isServer = false;
    client->setGameField(gameField);
    client->connectToServer();
}

void MainWindow::on_newSide(int side)
{
    QString message = side == GameField::Noughts?
                QString::fromLocal8Bit("Noughts, wait for opponents move."):
                QString::fromLocal8Bit("Your move is first, Crosses!");
    ui->statusBar->showMessage(message, 5000);
}

void MainWindow::on_actionAgain_triggered()
{
    if (isServer) {
        server->sendRestart();
    }
    else
        client->sendRestart();
}

void MainWindow::on_gameOver(int gameOverReason)
{
    gameField->setFieldEnabled(false);
    QString message;
    GameField::PlayerSide side = gameField->getSide();
    if (gameOverReason == GameManager::NoughtsWins)
    {
        if (side == GameField::Noughts)
            message = QString::fromLocal8Bit("YOU WIN!");
        else
            message = QString::fromLocal8Bit("YOU LOSE.");
    } else if (gameOverReason == GameManager::CrossesWins) {
        if (side == GameField::Crosses)
            message = QString::fromLocal8Bit("YOU WIN!");
        else
            message = QString::fromLocal8Bit("YOU LOSE.");
    } else {
        message = QString::fromLocal8Bit("Game over. DRAW. Game field is full.");
    }
    ui->statusBar->showMessage(message, 5000);
}

void MainWindow::on_networkConnected()
{
    ui->actionAgain->setEnabled(true);
}

void MainWindow::on_networkDisconnected()
{
    ui->actionAgain->setEnabled(false);
}

void MainWindow::on_actionAbout_triggered()
{
    QMessageBox::information(this, QString::fromLocal8Bit("About"),
                             QString::fromLocal8Bit("Tic-Tac-Toe game via TCP"));
}
