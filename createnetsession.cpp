#include "createnetsession.h"

#include <QNetworkConfigurationManager>
#include <QNetworkSession>

QNetworkSession *createNetworkSession(QObject *pParent)
{
    QNetworkSession *pSession = Q_NULLPTR;
    QNetworkConfigurationManager manager;
    if (manager.capabilities() &
        QNetworkConfigurationManager::NetworkSessionRequired)
    {
        QNetworkConfiguration config = manager.defaultConfiguration();
        pSession = new QNetworkSession(config, pParent);
    }
    //
    return pSession;
}
