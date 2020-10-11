#include "Netgame.h"

Netgame::Netgame(bool server)
{
    _server=NULL;
    _socket=NULL;

    if(server)
    {
        _server=new QTcpServer(this);
        _server->listen(QHostAddress::Any,817);
        connect(_server,SIGNAL(newConnection()),this,SLOT(slotNewConnection()));

    }
    else
    {
        _socket=new QTcpSocket(this);
        _socket->connectToHost(QHostAddress("127.0.0.1"),817);

        connect(_socket,SIGNAL(readyRead()),this,SLOT(slotRecv()));
    }
}

void Netgame::slotNewConnection()
{
    if(_socket) return;
    _socket=_server->nextPendingConnection();

    connect(_socket,SIGNAL(readyRead()),this,SLOT(slotRecv()));

}

void Netgame::mousePressEvent(QMouseEvent *ev)
{
    Board::mousePressEvent(ev);
    int buf[64];
    char Buf[64];

    for(int i=0;i<32;i++)
    {
        buf[i]=_s[i]._row*10+_s[i]._col;
        if(_s[i]._dead) buf[i+32]=_s[i]._side+100;
        if(!_s[i]._dead) buf[i+32]=_s[i]._side;
    }
    for(int j=0;j<64;j++)
    {
        Buf[j]=(char) buf[j];
        Buf[j]=(char) buf[j];
    }
    update();
    _socket->write(Buf,64);
}


void Netgame::slotRecv()
{
    QByteArray Ba=_socket->readAll();
    int ba[64]={0};
    for(int j=0;j<64;j++)
    {
        ba[j]=(int)(Ba[j]);
        ba[j]=(int)(Ba[j]);
    }

    for(int i=0;i<32;i++)
    {
        _s[i]._row=ba[i]/10;
        _s[i]._col=ba[i]%10;
        _s[i]._dead=ba[i+32]/100;
        _s[i]._side=ba[i+32]%100;
    }
    update();


}




