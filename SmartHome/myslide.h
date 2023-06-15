#ifndef MYSLIDE_H
#define MYSLIDE_H

#include <QWidget>
#include <QHBoxLayout>
#include <QScroller>
#include <QScrollArea>
#include <QTimer>
#include <QHBoxLayout>
#include <QLabel>


class MySlide : public QWidget
{
    Q_OBJECT

public:
    MySlide(QWidget *parent = nullptr);
    ~MySlide();

    void addPage(QWidget *);//添加新页面
    int getPageCount();     //获取页面总数
    int getCurPageIndex();  //获取当前页面索引

private:
    QScrollArea *scrollarea;//滚动区域
    QWidget *mainwidget;    //主面板
    QHBoxLayout *mainhbl;   //主面板水平布局
    QScroller *scroller;    //滚动对象
    QTimer *timer;          //定时器——检测拖动屏幕时间
    int curpageindex;       //当期页索引号
    int pagecount;          //页面总数
    bool isdragging;        //拖动状态标志
    QWidget *bottomwidget;  //底部面板（翻页图标）
    QHBoxLayout *bottomhbl; //底部水平布局
    QVector<QLabel *> pageicon;        //翻页图标
    void resizeEvent(QResizeEvent *ev);//重设事件回调（重载）

private slots:
    void hScrollBarValueChanged(int);  //水平滚动条值变化
    void onStateChanged(QScroller::State);//滚动对象状态变化
    void onTimerTimeOut();  //定时器超时槽函数
    void onCurrentPageIndexChanged(int);  //页索引改变槽函数

signals:
    void curPageIndexChanged(int);        //当前页面索引变化信号
};

#endif // MYSLIDE_H
