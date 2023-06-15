#include "myslide.h"
#include <QDebug>
#include <QPropertyAnimation>
#include <QScrollBar>

MySlide::MySlide(QWidget *parent):
    QWidget(parent),
    curpageindex(0),
    pagecount(0),
    isdragging(false)

{
    pageicon.clear();
    this->setMinimumSize(400, 300);
    this->setAttribute(Qt::WA_TranslucentBackground, true);//半透明
    scrollarea = new QScrollArea(this);
    scrollarea->setAlignment(Qt::AlignCenter);
    mainwidget = new QWidget(this);
    mainwidget->setStyleSheet("backgroud:transparent"); //透明
    scrollarea->setWidget(mainwidget);
    bottomwidget = new QWidget(this);
    bottomwidget->setStyleSheet("background:transparent");
    bottomhbl = new QHBoxLayout();
    bottomhbl->setContentsMargins(0, 0, 0, 0);
    bottomhbl->setAlignment(Qt::AlignCenter);
    bottomwidget->setLayout(bottomhbl);
    scrollarea->setVerticalScrollBarPolicy(Qt::ScrollBarAlwaysOff);//隐藏滚动条
    scrollarea->setHorizontalScrollBarPolicy(Qt::ScrollBarAlwaysOff);
    scroller = QScroller::scroller(scrollarea);
    //arm即平板为触摸式，PC为鼠标式
#if __arm__
    QScroller::ScrollerGestureType gesture =
            QScroller::TouchGesture;
#else
    QScroller::ScrollerGestureType gesture =
            QScroller::LeftMouseButtonGesture;
#endif
    scroller->grabGesture(scrollarea, gesture);
    QScrollerProperties properties = scroller->scrollerProperties();
    properties.setScrollMetric(QScrollerProperties::SnapTime, 0.5);//翻页时间
    properties.setScrollMetric(QScrollerProperties::MinimumVelocity, 1);
    scroller->setScrollerProperties(properties);

    mainhbl = new QHBoxLayout();
    mainhbl->setContentsMargins(0, 0, 0, 0);
    mainhbl->setSpacing(0);
    mainwidget->setLayout(mainhbl);

    timer = new QTimer(this);

    connect(scrollarea->horizontalScrollBar(), &QScrollBar::valueChanged,this, &MySlide::hScrollBarValueChanged);
    connect(scroller, &QScroller::stateChanged,this, &MySlide::onStateChanged);
    connect(timer, &QTimer::timeout,this, &MySlide::onTimerTimeOut);
    connect(this, &MySlide::curPageIndexChanged,this, &MySlide::onCurrentPageIndexChanged);
}

MySlide::~MySlide()
{
}

//底部点
void MySlide::addPage(QWidget *w) {
    mainhbl->addWidget(w, 1);//每页等宽
    pagecount++;
    QLabel *l = new QLabel();
    l->setPixmap(QPixmap(":/icons/icon1.png"));
    pageicon.append(l);
    bottomhbl->addWidget(l);
}

int MySlide::getPageCount() {
    return pagecount;
}

int MySlide::getCurPageIndex() {
    return curpageindex;
}

void MySlide::resizeEvent(QResizeEvent *ev) {
    Q_UNUSED(ev)
    scrollarea->resize(this->size());
    mainwidget->resize(this->width()*pagecount, this->height()-4);
    if(pagecount == 0) qDebug() << "当前页面总数0";
    else               onCurrentPageIndexChanged(0);
    bottomwidget->setGeometry(0, this->height()-20, this->width(), 20);
}

//计算页面索引，0.5*width决定滚动页面过半才翻页
void MySlide::hScrollBarValueChanged(int) {
    int value = scrollarea->horizontalScrollBar()->value();
    int width = this->width();
    curpageindex = value / width;
    curpageindex = value>=(curpageindex*width+width*0.5) ? curpageindex+1: curpageindex;
}

void MySlide::onStateChanged(QScroller::State state) {
    static int pressedvalue = 0;
    static int releasevalue = 0;
    static int currentindex = 0;
    //当前页0，无变化
    if(pagecount == 0) return;
    //松手
    if(state == QScroller::Inactive) {
        timer->stop();
        releasevalue = QCursor::pos().x();      //得到释放时x坐标
        if(pressedvalue == releasevalue) return;//不翻页
        //未拖动
        if(!isdragging) {
            if((pressedvalue-releasevalue)>5 && currentindex == curpageindex) {
                if(curpageindex < pagecount-1) {
                    curpageindex++;
                } else {
                    if(curpageindex > 0) curpageindex--;
                }
            }
        }
        //翻页动画
        QPropertyAnimation *animation = new QPropertyAnimation(scrollarea->horizontalScrollBar(), "value");
        animation->setDuration(200);
        animation->setStartValue(scrollarea->horizontalScrollBar()->value());
        animation->setEasingCurve(QEasingCurve::OutCurve);
        animation->setEndValue(curpageindex * this->width());
        animation->start();

        if(currentindex != curpageindex) {
            emit curPageIndexChanged(curpageindex);
        }

        pressedvalue = 0;
        releasevalue = 0;
        isdragging = false;
    }
    //拖动则记录开始x坐标，计算页面索引
#if __arm__
    if(state == QScroller::Dragging) {
        pressedvalue = QCursor::pos().x();
        currentindex = scrollarea->horizontalScrollBar()->value() / this->width();
        timer->start(300);
    }
#else
    if(state == QScroller::Pressed) {
        pressedvalue = QCursor::pos().x();
        currentindex = scrollarea->horizontalScrollBar()->value() / this->width();
        timer->start(300);
    }
#endif
}
//定时器超时槽函数：超时表示拖动发生了
void MySlide::onTimerTimeOut() {
    isdragging = true;
    timer->stop();
}
//选中页面图标为白点
void MySlide::onCurrentPageIndexChanged(int index) {

    for(int i=0; i<pageicon.count(); i++) {
        if(i == index) pageicon[i]->setPixmap(QPixmap(":/icons/icon2.png"));
        else           pageicon[i]->setPixmap(QPixmap(":/icons/icon1.png"));
    }
}
