#include "settingsdialog.h"
#include "ui_settingsdialog.h"

SettingsDialog::SettingsDialog(QWidget *parent)
    : QDialog(parent)
    , ui(new Ui::SettingsDialog)
{
    ui->setupUi(this);

    QObject::connect(ui->dia_buttonBox, &QDialogButtonBox::accepted, this, &QDialog::accept);
    QObject::connect(ui->dia_buttonBox, &QDialogButtonBox::rejected, this, &QDialog::reject);

}

SettingsDialog::~SettingsDialog()
{
    delete ui;
}

int SettingsDialog::getWorkTime() const
{
    return ui->spinBox_time->value();
}

