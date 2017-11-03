#ifndef SERVER_H
#define SERVER_H


#include <QAbstractSocket>
#include <QTcpServer>
#include <QScrollBar>
#include <QMessageBox>

#include <QJsonDocument>
#include <QJsonObject>

#include <QtWidgets>
#include <QtNetwork>


class QTcpSocket;

namespace Ui{
class MainWindow;

class Server : public QWidget
{

public:
    Server(QWidget* parent);
    QJsonObject json, jsonGPS, jsonGimbal, jsonBattery;
private:

    QTcpServer tcpServer;
    QTcpSocket *tcpServerConnection;
    qint64 totalBytes;     // 存放总大小信息
    QString fileName;      // 存放文件名
    QByteArray inBlock;    // 数据缓冲区
    QScrollBar *scrollbar;

    QTextEdit *textEdit;
    QJsonObject getJsonObjectFromString(const QString jsonString);

    QCheckBox *ifHostIp;
    QLineEdit *setIpAddress;


    void responseToCheckBox();
    void startListening();
    void updateServerProgress();
    void acceptConnection();
    void clear();

  //  void displayError(QAbstractSocket::SocketError socketError);
};

} //namespace Ui
#endif // SERVER_H
