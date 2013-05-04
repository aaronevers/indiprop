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

#include "indiclient.h"

extern QTextStream qout;

IndiClient::IndiClient(const int &attempts) : mPort(indi::PORT), mAttempts(attempts), mAttempt(1)
{
    connect(&mQTcpSocket, SIGNAL(connected()), SLOT(socketConnected()));
    connect(&mQTcpSocket, SIGNAL(disconnected()), SLOT(socketDisconnected()));
    connect(&mQTcpSocket, SIGNAL(error(QAbstractSocket::SocketError)), SLOT(socketError(QAbstractSocket::SocketError)));
    connect(&mQTcpSocket, SIGNAL(readyRead()), SLOT(socketReadyRead()));
}

void IndiClient::socketConnect(const QString &hoststring)
{
    mHost = hoststring;
    mPort = indi::PORT;

    QStringList hostsplit = hoststring.split(":");
    if (hostsplit.size() > 0)
    {
        if (hostsplit.size() > 1)
            mPort = hostsplit[1].toUShort();

        mQTcpSocket.disconnectFromHost();
        mQTcpSocket.connectToHost(hostsplit[0], mPort);
    }
}

void IndiClient::reconnect()
{
    socketConnect(mHost);
}

void IndiClient::socketConnected()
{
    qout << "Connected to indiserver@" << mQTcpSocket.peerName() << ":" << mPort << endl;

    /* Subscribe to all properties */
    QDomDocument doc("");
    QDomElement getProperties = doc.createElement("getProperties");
    getProperties.setAttribute("version", indi::VERSION);
    doc.appendChild(getProperties);

    sendProperty(doc);
}

bool IndiClient::connected()
{
    return mQTcpSocket.isWritable();
}

void IndiClient::socketDisconnected()
{
    qout << "Disconnected from indiserver@" << mQTcpSocket.peerName() << ":" << mPort << endl;
}

void IndiClient::socketError(QAbstractSocket::SocketError)
{
    if (mQTcpSocket.error() == QAbstractSocket::RemoteHostClosedError)
    {
        mAttempt = 1;
        qout << "The remote host closed the connection.  Attempting to reconnect..." << endl;
        QTimer::singleShot(1000, this, SLOT(reconnect()));
    }
    else if (mQTcpSocket.error() == QAbstractSocket::ConnectionRefusedError && mAttempt < mAttempts)
    {
        mAttempt++;
        qout << "Attempting to reconnect..." << endl;
        QTimer::singleShot(1000, this, SLOT(reconnect()));
    }
    else
    {
        qout << "Socket error from indiserver@" << mQTcpSocket.peerName() << ":" << mPort
            << " (" << mQTcpSocket.errorString() << ")" << endl;
    }
}

void IndiClient::sendProperty(QDomDocument doc)
{
    mQTcpSocket.write(doc.toString().toAscii());
}

void IndiClient::socketReadyRead()
{
    QDomDocument doc("");
    static QByteArray bytes;

    while (mQTcpSocket.canReadLine())
    {
        bytes += mQTcpSocket.readLine();
        if (doc.setContent(bytes, false))
        {
            emit propertyUpdate(doc);
            bytes.clear();
        }
    }
}

QString IndiClient::formatNumber(const QString &format, const QString &number, const bool &sexagesimal)
{
    QString text = number;

    if (format.size())
    {
        if (format.endsWith("m") && sexagesimal)
        {
            QStringList wf = format.mid(1, format.size() - 2).split('.');
            int f = 0;
            if (wf.size() > 1)
                f = wf[1].toInt();

            double h = text.toDouble();

            text = "";
            if (h < 0.0)
                text = "-";

            h = fabs(h);
            double m = (h - floor(h))*60.0;
            double s = (m - floor(m))*60.0;

            switch (f)
            {
                case 9:
                    text += QString("%1:%2:%3").arg(floor(h), 0, 'f', 0, '0').arg(floor(m), 2, 'f', 0, '0').arg(s, 2, 'f', 2, '0');
                    break;
                case 8:
                    text += QString("%1:%2:%3").arg(floor(h), 0, 'f', 0, '0').arg(floor(m), 2, 'f', 0, '0').arg(s, 2, 'f', 1, '0');
                    break;
                case 6:
                    text += QString("%1:%2:%3").arg(floor(h), 0, 'f', 0, '0').arg(floor(m), 2, 'f', 0, '0').arg(s, 2, 'f', 0, '0');
                    break;
                case 5:
                    text += QString("%1:%2").arg(floor(h), 0, 'f', 0, '0').arg(m, 2, 'f', 1, '0');
                    break;
                case 3:
                    text += QString("%1:%2").arg(floor(h), 0, 'f', 0, '0').arg(m, 2, 'f', 0, '0');
                    break;
                default:
                    text += QString("%1").arg(h, 0, 'f', f, '0');
                    break;
            }
        }
        else
        {
            QString f = format;
            f.replace('m', 'f');

            text = text.sprintf(f.toAscii(), text.toDouble()).trimmed();
        }
    }

    return text;
}
