#include "server.h"


namespace Ui {


Server::Server(QWidget* parent)
{
    setWindowFlags(Qt::Window);
    setWindowTitle("Log");
    resize(320, 640);

    QGridLayout* grid = new QGridLayout(this);
        grid->setContentsMargins(5, 10, 5, 5);

        QHBoxLayout* left_button_layout = new QHBoxLayout();

        QPushButton* save_log_button = new QPushButton(tr("Save"), this);
       // connect(save_log_button, &QPushButton::released, this, &LogWidget::SaveLog);
        left_button_layout->addWidget(save_log_button);

        QPushButton* clear_button = new QPushButton(tr("Clear"), this);
        connect(clear_button, &QPushButton::released, this, &Server::clear);
        left_button_layout->addWidget(clear_button);

        grid->addLayout(left_button_layout, 0, 0, Qt::AlignLeft);

        QHBoxLayout* right_button_layout = new QHBoxLayout();

        grid->addLayout(right_button_layout, 0, 1, Qt::AlignRight);

        textEdit = new QTextEdit(this);
        textEdit->setObjectName(QStringLiteral("textEdit"));
        textEdit->setReadOnly(true);
        grid->addWidget(textEdit, 1, 0, 1, 2);

    connect(&tcpServer, &QTcpServer::newConnection,
                this, &Server::acceptConnection);
    if (!tcpServer.listen(QHostInfo(), 6666)) {
        qDebug() << tcpServer.errorString();
        close();
        return;
    }

    totalBytes = 0;

}

void Server::clear(){
    textEdit->clear();
}

void Server::acceptConnection()
{
    tcpServerConnection = tcpServer.nextPendingConnection();
    connect(tcpServerConnection, &QTcpSocket::readyRead,
            this, &Server::updateServerProgress);
//    connect(tcpServerConnection, SIGNAL(error(QAbstractSocket::SocketError)),
//            this, SLOT(displayError(QAbstractSocket::SocketError)));
  //  ui->serverStatusLabel->setText(tr("接受连接"));
  //  关闭服务器，不再进行监听
  //  tcpServer.close();
}

void Server::updateServerProgress()
{
    QDateTime time = QDateTime::currentDateTime();
    QString str = time.toString("\n[ hh:mm:ss ]"); //设置显示格式
    qDebug()<<str;

    qint64 len = tcpServerConnection->bytesAvailable();
    qDebug()<<"socket data len:"<< len;
    QByteArray alldata = tcpServerConnection->read(len);
    //开始转换编码
    QTextCodec *utf8codec = QTextCodec::codecForName("UTF-8");
    QString utf8str = utf8codec->toUnicode(alldata.mid(2));
   // qDebug()<<"hex:["<<alldata.toHex().toUpper()<<"]";
    qDebug()<<"utf-8 ["<< (utf8str) << "]";


    json = getJsonObjectFromString(utf8str);

    if(json.contains("GPS")){
        jsonGPS = json["GPS"].toObject();
//        qDebug()<<"\n->"<< jsonGPS["altitude"].toDouble();
    }

    if(json.contains("Gimbal")){
        jsonGimbal = json["Gimbal"].toObject();
    }
    if(json.contains("Battery")){
        jsonBattery = json["Battery"].toObject();
    }

    //显示到控件上
    textEdit->insertPlainText(str);//在标签上显示时间
    textEdit->insertPlainText(utf8str);
    textEdit->insertPlainText(tr("\n"));


    //自动滚动到最底
    scrollbar = textEdit->verticalScrollBar();
    if (scrollbar)
    {
       scrollbar->setSliderPosition(scrollbar->maximum());
    }

}

//void Server::displayError(QAbstractSocket::SocketError socketError)
//{
//    qDebug() << tcpServerConnection->errorString();
//    tcpServerConnection->close();
//    //ui->serverProgressBar->reset();
//    //ui->serverStatusLabel->setText(tr("服务端就绪"));

//}

QJsonObject Server::getJsonObjectFromString(const QString jsonString){
    QJsonDocument jsonDocument = QJsonDocument::fromJson(jsonString.toLocal8Bit().data());
    if( jsonDocument.isNull() ){
        qDebug()<< "===> please check the string "<< jsonString.toLocal8Bit().data();
    }
    QJsonObject jsonObject = jsonDocument.object();
    return jsonObject;
}

} //namespce Ui
