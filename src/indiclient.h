/***********************************************************************
 * Copyright © 2012 Aaron Evers
 *
 * This program is free software: you can redistribute it and/or modify
 * it under the terms of the GNU General Public License as published by
 * the Free Software Foundation version 3 of the License.
 *
 * This program is distributed in the hope that it will be useful,
 * but WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
 * GNU General Public License for more details.
 *
 * You should have received a copy of the GNU General Public License
 * along with this program.  If not, see <http://www.gnu.org/licenses/>.
 ***********************************************************************/

#ifndef INDICLIENT_H
#define INDICLIENT_H

#include <QtCore>
#include <QtNetwork>
#include <QtXml>

#include <QTcpSocket>
#include <QByteArray>

#include <QTextStream>

namespace indi
{
    static const char * const VERSION = "1.7";
    static const unsigned short PORT = 7624;
};

class IndiClient : public QObject
{
    Q_OBJECT;

public:
    IndiClient(const int &attempts);
    void socketConnect(const QString &hoststring);
    bool connected();

    static QString formatNumber(const QString &format, const QString &number, const bool &sexagesimal);

private:
    quint16 mPort;
    QTcpSocket mQTcpSocket;
    QString mHost;
    int mAttempts;
    int mAttempt;

signals:
    void propertyUpdate(QDomDocument);

private slots:
    void reconnect();
    void socketConnected();
    void socketDisconnected();
    void socketError(QAbstractSocket::SocketError);
    void socketReadyRead();

public slots:
    void sendProperty(QDomDocument dom);

};

#endif
