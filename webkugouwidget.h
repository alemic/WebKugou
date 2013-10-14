#ifndef WIDGET_H
#define WIDGET_H
#include<QtWebKitWidgets>
#include <QtWidgets>

class WebKugouWidget : public QWidget
{
    Q_OBJECT

public:
    WebKugouWidget(QWidget *parent = 0);
    ~WebKugouWidget();
private:
    QWebView* kgwebView;
    QLabel*   tittleBar;
    QToolButton* closeButton;
    QPushButton* minToTrayBtn;
    QPushButton* minButton;
    QPushButton* aboutButton;

    QSystemTrayIcon trayIcon;

private:
    //重写鼠标事件，实现窗口移动
    bool isPress;
    QPoint dragPosition;
    void mousePressEvent(QMouseEvent *);
    void mouseMoveEvent(QMouseEvent *);
    void mouseReleaseEvent(QMouseEvent *);

    //初始化托盘图标;
    void initTrayIcon();

private slots:

    void emitminimizedTOTray();

    //关闭
    void closeEx();

    //下一首
    void playNext();

    //上一首
    void playOrPause();

    //上一首
    void playPrevious();

    //关于窗口
    void showAboutDlg();

    //激活窗口
    void activeWindow(QSystemTrayIcon::ActivationReason reasson);

signals:
    //为了方便这些代码嵌入到别的项目中，没实现最小化到托盘
    //点击最小化到托盘，发出以下信号
    void minimizedTOTray();
};

#endif // WIDGET_H
