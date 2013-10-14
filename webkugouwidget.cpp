#include "webkugouwidget.h"
#include"aboutdialog.h"
WebKugouWidget::WebKugouWidget(QWidget *parent)
    : QWidget(parent),isPress(false)
{
    //设置插件路径，fash插件----------------------
    //将NPSWF32_11_8_800_168.dll拷贝至pugins目录下
    QApplication::addLibraryPath("./plugins");

    QWebSettings *settings = QWebSettings::globalSettings();

    settings->setAttribute(QWebSettings::PluginsEnabled, true);//允许插件

    settings->setAttribute(QWebSettings::JavascriptEnabled, true);//JavaScript

    settings->setAttribute(QWebSettings::DeveloperExtrasEnabled, true);//

    settings->setAttribute(QWebSettings::JavascriptCanOpenWindows, true);

    //将系统边框去掉
    setWindowFlags(Qt::FramelessWindowHint);

    kgwebView = new QWebView(this);

    kgwebView->page()->setLinkDelegationPolicy(QWebPage::DelegateAllLinks);

    //关闭水平滚动条
    kgwebView->page()->mainFrame()->setScrollBarPolicy(Qt::Horizontal,Qt::ScrollBarAlwaysOff);

    kgwebView->load(QUrl("http://web.kugou.com/index.html?"));

    //经过试验如下设置比较好
    kgwebView->setGeometry(-1,0,740,508);

    //默认界面的rgb为rgb(1,160,234）
    kgwebView->setStyleSheet("background:rgba(1,160,234,0);");

    //用于显示WebKugou，窗口拖动以及屏蔽web酷狗上侧控件；
    tittleBar=new QLabel(this);

    tittleBar->setGeometry(0,0,747,30);

    tittleBar->setText(" WebKugou");

    tittleBar->setCursor(Qt::SizeAllCursor);

    this->setWindowTitle("WebKugou");

    this->resize(740,510);

    //关闭按钮
    closeButton=new QToolButton(this);

    connect(closeButton,SIGNAL(clicked()),this,SLOT(closeEx()));

    closeButton->setGeometry(740-25,0,25,25);

    closeButton->setText("×");

    closeButton->setToolTip(tr("关闭"));

    //最小化到托盘
    minToTrayBtn=new QPushButton("↘",this);

    minToTrayBtn->setGeometry(740-25-25,0,25,25);

    minToTrayBtn->setToolTip(tr("最小化到托盘"));

    connect(minToTrayBtn,SIGNAL(clicked()),this,SLOT(emitminimizedTOTray()));


    //最小化
    minButton=new QPushButton("-",this);

    connect(minButton,SIGNAL(clicked()),this,SLOT(showMinimized()));

    minButton->setGeometry(740-25-25-25,0,25,25);

    minButton->setToolTip(tr("最小化"));

    //关于按钮
    aboutButton=new QPushButton(tr("?"),this);
    aboutButton->setGeometry(740-25-25-25-25,0,25,25);
    connect(aboutButton,SIGNAL(clicked()),this,SLOT(showAboutDlg()));


    this->setWindowIcon(QIcon(":/Image/Icon/Icon.png"));
    //设置风格表
    this->setStyleSheet(QString("WebKugouWidget{border:5px solid gray;border-color:rgb(1,160,234);font-size:12pt;background:rgb(1,160,234);color:white}"
                                "QToolButton{font:13pt \"Arial\";color:white;background:rgba(0,0,0,0);}"
                                "QToolButton::hover{border:0px solid;color:black;background-color: qlineargradient(spread:pad, x1:0, y1:0, x2:0, y2:1, stop:0 rgba(254, 67, 101, 220), stop:1 rgba(254, 67, 101, 0));}"
                                "QPushButton{color:white;background:rgba(0,0,0,0);}"
                                "QPushButton::hover{border:0px solid;color:black;background-color: qlineargradient(spread:pad, x1:0, y1:0, x2:0, y2:1, stop:0 rgba(170, 138, 87, 220), stop:1 rgba(170, 138, 87, 0));}"
                                "QLabel{color:white;font-size:15pt;background:qlineargradient(x1:0,y1:0,x2:0,y2:1,stop:0 rgb(3,208,234),stop:0.8 rgb(1,160,234),stop:1 rgb(1,160,234));}"
                                ));


    initTrayIcon();
}

WebKugouWidget::~WebKugouWidget()
{

}
void WebKugouWidget::showAboutDlg()
{
    AboutDialog aboutdlg;
    aboutdlg.setLogo(":/Image/Icon/aboutLogo.png");
    aboutdlg.setInfo("<table border=\"0\"><tr><td width=270 valign=\"middle\" ><b>WebKugou客户端 v1.0</b></td><td><a href=\"https://github.com/pansinm/WebKugou/\">查看源代码</a></td></tr><tr height=\"22\"><td width=300 valign=\"middle\">by X_PAN</td><td>pansinm@163.com</td></tr></table>");
    aboutdlg.setText(":/file/license.txt",true);
    aboutdlg.setWindowTitle(tr("关于WebKugou客户端"));
    aboutdlg.exec();
}

void WebKugouWidget::initTrayIcon()
{

    //托盘图标
    trayIcon.setIcon(QIcon(":/Image/Icon/Icon.png"));


    //菜单
    QMenu* trayMenu=new QMenu;

    QAction* playPreAction=new QAction(tr("上一首"),0);
    connect(playPreAction,SIGNAL(triggered()),this,SLOT(playPrevious()));
    playPreAction->setIcon(QIcon(":/Image/Icon/pre.png"));

    QAction* playOrPauseAction=new QAction(tr("播放|暂停"),0);
    connect(playOrPauseAction,SIGNAL(triggered()),this,SLOT(playOrPause()));
    playOrPauseAction->setIcon(QIcon(":/Image/Icon/playorpause.png"));

    QAction* playNextAction=new QAction(tr("下一首"),0);
    connect(playNextAction,SIGNAL(triggered()),this,SLOT(playNext()));
    playNextAction->setIcon(QIcon(":/Image/Icon/next.png"));


    QAction* exitAction=new QAction(tr("退出"),0);
    exitAction->setIcon(QIcon(":/Image/Icon/exit.png"));

    connect(exitAction,SIGNAL(triggered()),&trayIcon,SLOT(hide()));
    connect(exitAction,SIGNAL(triggered()),this,SLOT(closeEx()));

    connect(&trayIcon,SIGNAL(activated(QSystemTrayIcon::ActivationReason)),this,SLOT(activeWindow(QSystemTrayIcon::ActivationReason)));
    connect(this,SIGNAL(minimizedTOTray()),this,SLOT(hide()));
    connect(this,SIGNAL(destroyed()),&trayIcon,SLOT(hide()));

    trayMenu->addAction(playPreAction);
    trayMenu->addAction(playOrPauseAction);
    trayMenu->addAction(playNextAction);
    trayMenu->addSeparator();
    trayMenu->addAction(exitAction);

    trayMenu->setStyleSheet(QString("QMenu {background-color: rgb(238, 241, 255);border:1px solid rgb(158,157,131);margin: 1px; } "
                                    "QMenu::item {padding: 4px 25px 2px 20px;border: 2px solid transparent;}"
                                    "QMenu::item:selected {border:1px solid darkblue;background: rgba(1,160,234,50);}"
                                    "QMenu::icon:checked { background: gray;border: 1px inset gray;position: absolute;top: 1px;right: 1px;bottom: 1px;left: 1px;}"
                                    "QMenu::separator {height: 1px;background:  rgb(200, 210, 215);margin-left: 10px;margin-right: 5px;}"
                                    "QMenu::indicator {width: 13px;height: 13px;}"
                                    ));
    trayIcon.setContextMenu(trayMenu);
    trayIcon.show();

}

void WebKugouWidget::closeEx()
{
    this->hide();
    trayIcon.hide();
    exit(0);
}

void WebKugouWidget::emitminimizedTOTray()
{
    emit minimizedTOTray();
}
void WebKugouWidget::activeWindow(QSystemTrayIcon::ActivationReason reasson)
{


    switch(reasson)
    {
    //单击
     case QSystemTrayIcon::Trigger:

     //双击
     case QSystemTrayIcon::DoubleClick:
         //恢复窗口显示
         this->setWindowState(Qt::WindowActive);
         this->activateWindow();
         this->show();

         break;
     default:
         break;
     }


}

void WebKugouWidget::playNext()
{

    QMouseEvent* press=new QMouseEvent(QEvent::MouseButtonPress,QPoint(206,116),Qt::LeftButton,Qt::LeftButton,Qt::NoModifier);
    QApplication::postEvent(kgwebView,press);
    QMouseEvent* release=new QMouseEvent(QEvent::MouseButtonRelease,QPoint(206,116),Qt::LeftButton,Qt::LeftButton,Qt::NoModifier);
    QApplication::postEvent(kgwebView,release);

}

void WebKugouWidget::playOrPause()
{

    QMouseEvent* press=new QMouseEvent(QEvent::MouseButtonPress,QPoint(168,116),Qt::LeftButton,Qt::LeftButton,Qt::NoModifier);
    QApplication::postEvent(kgwebView,press);
    QMouseEvent* release=new QMouseEvent(QEvent::MouseButtonRelease,QPoint(168,116),Qt::LeftButton,Qt::LeftButton,Qt::NoModifier);
    QApplication::postEvent(kgwebView,release);

}

void WebKugouWidget::playPrevious()
{

    QMouseEvent* press=new QMouseEvent(QEvent::MouseButtonPress,QPoint(128,116),Qt::LeftButton,Qt::LeftButton,Qt::NoModifier);
    QApplication::postEvent(kgwebView,press);
    QMouseEvent* release=new QMouseEvent(QEvent::MouseButtonRelease,QPoint(128,116),Qt::LeftButton,Qt::LeftButton,Qt::NoModifier);
    QApplication::postEvent(kgwebView,release);

}

void WebKugouWidget::mousePressEvent(QMouseEvent *event)
{
    if(event->button()==Qt::LeftButton)
    {
        dragPosition=event->globalPos()-this->pos();
        isPress=true;
        event->accept();
    }
}

void WebKugouWidget::mouseMoveEvent(QMouseEvent *event)
{

    if(isPress&&event->buttons()&&Qt::LeftButton)
    {

        move(event->globalPos()-dragPosition);
        event->accept();

    }


}



void WebKugouWidget::mouseReleaseEvent(QMouseEvent *event)
{
    if(event->button()==Qt::LeftButton)
    {
        isPress=false;
         event->accept();
    }
}
