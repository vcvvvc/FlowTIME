#ifndef WIDGET_H
#define WIDGET_H

#include <QWidget>
#include <QTimer>
#include <QDebug>
#include <QSystemTrayIcon>
#include <QMenu>
#include <QStyle>
#include <QString>
#include <QCloseEvent>

#include "./settingsdialog.h"

QT_BEGIN_NAMESPACE
namespace Ui {
class Widget;
}
QT_END_NAMESPACE

class Widget : public QWidget
{
    Q_OBJECT

public:
    Widget(QWidget *parent = nullptr);
    ~Widget();

    enum FlowState {
        Stopped,
        Working,
        Resting
    };

private slots:
    void on_stop_btn_clicked();

    void on_start_btn_clicked();

    void on_rest_btn_clicked();

private:
    void updateTime();
    void updateTimelabel(double st_time);
    void deal_rest_time();


private slots:
    void onTrayIconActivated(QSystemTrayIcon::ActivationReason reason);
    void on_settings_triggered();

private:
    Ui::Widget *ui;
    SettingsDialog *dlg;
    QTimer *fltime;
    QSystemTrayIcon *trayIcon;
    QMenu *trayMenu;
    QAction *exitAction;
    QAction *showAction;
    QAction *hideAction;
    QAction *settingsAction;

    FlowState state = Stopped;

    const double MIN_WORK_THRESHOLD = 0; // A: 下限 15分钟
    const double MAX_WORK_CAP = 90.0;       // B: 计算上限 90分钟
    const double CONTEXT_TIMEOUT = 10.0;    // C: 超时熔断 10分钟
    const double DIVISOR = 5.0;             // 比例 1/5

    double start_time = 0;
    double target_time = 0; //90分钟
    double rest_time = 0;
    int m = 0;
    int s = 0;
    int sh_win = 0;

protected:
    void closeEvent(QCloseEvent *event) override;


};
#endif // WIDGET_H
