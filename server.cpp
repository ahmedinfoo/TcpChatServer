#include "server.h"

Server::Server(QObject *parent)
    : QTcpServer{parent}
{



        qInfo() << "Server started on port " << serverPort();
}

int Server::count()
{
    return m_clientSockets.count();
}

void Server::close()
{
    foreach(QTcpSocket * socket , m_clientSockets){
        socket->close();
    }
    qDeleteAll(m_clientSockets);
    m_clientSockets.clear();
}

void Server::readyread()
{

    QTcpSocket *clientSocket = qobject_cast<QTcpSocket*>(sender());
        if (!clientSocket) {
            return;
        }

        QByteArray data = clientSocket->readAll();
        qInfo() << "Received data from " << clientSocket->peerAddress().toString() << ": " << data;

        foreach (QTcpSocket *socket , m_clientSockets) {
            socket->write(data);
        }
}

void Server::disconnect()
{
    QTcpSocket *clientSocket = qobject_cast<QTcpSocket*>(sender());
        if (!clientSocket) {
            return;
        }

        m_clientSockets.removeOne(clientSocket);
        clientSocket->deleteLater();

        qInfo() << "Client disconnected from " << clientSocket->peerAddress().toString();
}


void Server::incomingConnection(qintptr handle)
{
    qInfo() << "incomming connection in server  " ;
    QTcpSocket *clientSocket = new QTcpSocket();
    clientSocket->setSocketDescriptor(handle);
    qInfo() << "incomming connection in server 2  "<<clientSocket->canReadLine() ;
    if(!clientSocket->waitForConnected(3000))
    {
      delete clientSocket;
        return;
    }
    m_clientSockets.append(clientSocket);

    connect(clientSocket, &QTcpSocket::readyRead, this, &Server::readyread);
    connect(clientSocket, &QTcpSocket::disconnected, this, &Server::disconnect);
    emit changed();

    qInfo() << "New client connected from " << clientSocket->peerAddress().toString();
    clientSocket->write(message.toLatin1());
}

const QString &Server::getMessage() const
{
    return message;
}

void Server::setMessage(const QString &newMessage)
{
    message = newMessage;
}
