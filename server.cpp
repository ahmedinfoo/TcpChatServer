#include "server.h"

Server::Server(QObject *parent)
    : QTcpServer{parent}
{

        connect(this, &QTcpServer::newConnection, this, &Server::newConnection);

        if (!listen(QHostAddress::Any, 1234)) {
            qWarning() << "Unable to start the server: " << errorString();
            return;
        }

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

        for (QTcpSocket *socket : m_clientSockets) {
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
    QTcpSocket *clientSocket = nextPendingConnection();
    clientSocket->setSocketDescriptor(handle);
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
