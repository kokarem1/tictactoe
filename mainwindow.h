#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QMainWindow>

class GameServer;
class GameClient;
class GameField;
class GameManager;

namespace Ui {
class MainWindow;
}

class MainWindow : public QMainWindow
{
    Q_OBJECT

public:
    explicit MainWindow(QWidget *parent = 0);
    ~MainWindow();

private slots:
    void on_actionCreate_triggered();

    void on_actionJoin_triggered();

    void on_newSide(int side);

    void on_actionAgain_triggered();

    void on_gameOver(int gameOverReason);

    void on_actionAbout_triggered();

public slots:

    void on_networkConnected();

    void on_networkDisconnected();

private:
    Ui::MainWindow *ui;
    GameServer *server;
    GameClient *client;
    GameField *gameField;
    GameManager *gameManager;

    bool isServer;
};

#endif // MAINWINDOW_H
