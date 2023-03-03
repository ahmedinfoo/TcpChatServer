#ifndef SERVER_H
#define SERVER_H

#include <QObject>
#include <QTcpServer>
#include <QTcpSocket>

class Server : public QTcpServer
{
    Q_OBJECT
public:
    explicit Server(QObject *parent = nullptr);
    int count();
    void close();

    const QString &getMessage() const;
    void setMessage(const QString &newMessage);

public slots:
   void readyread();
   void disconnect();

signals :
   void changed();
    // QTcpServer interface
protected:
    void incomingConnection(qintptr handle) override;

private:
    QString message;
    QList<QTcpSocket*> m_clientSockets;
};

#endif // SERVER_H
