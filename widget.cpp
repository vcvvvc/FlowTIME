#include "widget.h"
#include "ui_widget.h"

Widget::Widget(QWidget *parent)
    : QWidget(parent)
    ,   ui(new Ui::Widget),
        dlg(new SettingsDialog(this)),
        fltime(new QTimer(this)),
        trayIcon(new QSystemTrayIcon(this)),
        trayMenu(new QMenu(this)),
        showAction(new QAction("Show",this)),
        hideAction(new QAction("Hide", this)),
        exitAction(new QAction("Exit", this)),
        settingsAction(new QAction("Settings", this)),
        target_time(90 * 10 * 1000)
{
    ui->setupUi(this);
    QObject::connect(fltime, &QTimer::timeout, this, &Widget::updateTime);

    trayIcon->setIcon(style()->standardIcon(QStyle::SP_ComputerIcon));
    trayMenu->addAction(showAction);
    trayMenu->addAction(hideAction);
    trayMenu->addAction(settingsAction);
    trayMenu->addAction(exitAction);


    // trayIcon->setContextMenu(trayMenu);

    QObject::connect(showAction, &QAction::triggered, this, &Widget::show);
    QObject::connect(hideAction, &QAction::triggered, this, &Widget::hide);
    QObject::connect(settingsAction, &QAction::triggered, this, &Widget::on_settings_triggered);

    QObject::connect(exitAction, &QAction::triggered, this, &QApplication::quit);


    QObject::connect(trayIcon, &QSystemTrayIcon::activated, this, &Widget::onTrayIconActivated);

    trayIcon->show();

}

Widget::~Widget()
{
    delete ui;
}

void Widget::updateTime() {
    switch(this->state) {
    case Stopped:
        fltime->stop();
        rest_time = 0;
        start_time = 0;
        sh_win = 0;
        this->updateTimelabel(start_time);
        return;
    case Working:
        start_time += 1000;
        if (start_time >= target_time && sh_win == 0) {
            trayIcon->showMessage("提醒", "已到极限，注意休息！", QSystemTrayIcon::Information, 10000);
            sh_win++;
        }
        this->updateTimelabel(start_time);

        break;
    case Resting:
        rest_time -= 1000;
        if(rest_time <= 0) {
            qDebug() << "休息结束，是否重新开始 \n";
            start_time = 0;
            rest_time = 0;
            trayIcon->showMessage("提醒", "休息时间结束！", QSystemTrayIcon::Information, 10000);

            this->state = Stopped;
        }
        this->updateTimelabel(rest_time);
        break;
    default:
        break;
    }
}

void Widget::updateTimelabel(double st_time) {

    int total_seconds = static_cast<int>(st_time / 1000);
    m = total_seconds / 60;
    s = total_seconds % 60;

    QString timeText = QString("%1:%2")
                           .arg(m, 2, 10, QChar('0'))
                           .arg(s, 2, 10, QChar('0'));
    ui->time_label->setText(timeText);
}

void Widget::on_stop_btn_clicked()
{
    this->state = Stopped;
}


void Widget::on_start_btn_clicked()
{
    if(this->state == Working) {
        return;
    }
    start_time = 0;
    sh_win = 0;

    this->state = Working;

    fltime->start(1000);
}

void Widget::deal_rest_time() {

    if(start_time < MIN_WORK_THRESHOLD * 60 * 1000) { //工作时间低于最小工作时限 无法休息
        rest_time = 0;

    } else if(start_time >= MAX_WORK_CAP * 60 * 1000) { //工作时间超过90分钟
        rest_time = MAX_WORK_CAP * 60 * 1000 / DIVISOR;//奖励1s 真棒
        start_time = 0;
    }
    else { //正常时间
        rest_time = start_time / DIVISOR;
        start_time = 0;
    }
}

void Widget::onTrayIconActivated(QSystemTrayIcon::ActivationReason reason)
{
    // 1. 先判断是不是左键单击 (Trigger)
    if (reason == QSystemTrayIcon::Trigger) {

        // 2. 再判断窗口当前是 显示 还是 隐藏
        if (this->isVisible()) {
            // 如果是显示的，就隐藏
            this->hide();
        } else {
            // 否则，就显示并置顶
            this->showNormal();
            this->activateWindow();
        }
    } else if (reason == QSystemTrayIcon::Context) {
        // 右键点击

        trayMenu->exec(QCursor::pos());
    }

}

void Widget::on_settings_triggered()
{
    if(dlg->exec() == QDialog::Accepted) {
        target_time = dlg->getWorkTime() * 60 * 1000;
        if(start_time > target_time) {
            ui->stop_btn->click();
        }
    }

}

void Widget::on_rest_btn_clicked()
{
    this->state = Resting;
    sh_win = 0;
    rest_time = 0;

    this->deal_rest_time();
    this->updateTimelabel(rest_time);
}


void Widget::closeEvent(QCloseEvent *event)
{
    event->ignore();
    this->hide();
}
