#include "ticker.hpp"
// This is not a toy : it is used to accept unexpected messages while the manager is idle
// The display tell us when the timer used to accept() is working...
Ticker::Ticker(const int sockfd, QWidget *parent)
    : sockfd_(sockfd), QWidget(parent), money(0), nbFans(0) {
    offset = 0; // starting position of the text displayed
    myTimerId = 0; // no timer started yet
    move(100,50); // position in parent window
    setWindowTitle(tr("Attention"));
}

void Ticker::paintEvent(QPaintEvent * /* event */) {
    QPainter painter(this);

    int textWidth = fontMetrics().width(myText);
    if (textWidth < 1) return;
    int x = -offset;
    //if banner is larger than the text, the text is repeated
    while (x < width()) {
        painter.drawText(x, 0, textWidth, height(),
                         Qt::AlignLeft | Qt::AlignVCenter, myText);
        x += textWidth;
    }
}

void Ticker::showEvent(QShowEvent * /* event */) {
    //starts the timer when the widget is visible
    askForManagerInfos(sockfd_);
    receiveManagerInfos(sockfd_,&nbPlayers,&money,&nbFans);
    myText=QString(" --- MONEY : %1 ||| NUMBER OF FANS : %2").arg(money).arg(nbFans);
    myTimerId = startTimer(30);
}

void Ticker::timerEvent(QTimerEvent *event) {
    //checks first it's about our event !
    if (event->timerId() == myTimerId) {
        //here we put the non blocking recv of the message
        //...
        //and manage it with appropriate dialog
        ++offset;
        if (offset >= fontMetrics().width(myText)) offset = 0;
        scroll(-1, 0);
        //scrolls the banner 1 pixel left
    }
    else QWidget::timerEvent(event);
    //it's not about our event - let's give it to our base class
}

void Ticker::hideEvent(QHideEvent * /* event */) {
    //timer stops when widget is hidden
    killTimer(myTimerId);
    myTimerId = 0;
}
