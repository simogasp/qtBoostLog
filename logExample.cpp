#include "logExample.h"
#include "ui_logExample.h"

LogExample::LogExample(QWidget *parent) :
    QDialog(parent),
    ui(new Ui::Logger),
    heater(nullptr),
    heaterOn(false)
{
    ui->setupUi(this);


    heater = new Heater;

    connect(heater, SIGNAL(Temperature(double)), this, SLOT(updateTemperature(double)));
    connect(heater, SIGNAL(AtTemperature(bool)), ui->AtTemperature, SLOT(setChecked(bool)));

    connect(ui->StartStop, SIGNAL(clicked()), this, SLOT(StartStop()));
    connect(ui->SetPointSelector, SIGNAL(valueChanged(double)), heater, SLOT(updateSetPoint(double)));
    connect(ui->RampRateSelector, SIGNAL(valueChanged(int)), heater, SLOT(updateRampRate(int)));

    ui->SetPointSelector->setRange(21.0, 100.0);
    ui->SetPointSelector->setSingleStep(0.10);

    ui->RampRateSelector->setRange(1, 10);
    ui->RampRateSelector->setSingleStep(1);

}

LogExample::~LogExample()
{
    delete heater;
    delete ui;
}

void
LogExample::StartStop()
{
    if (heaterOn)
    {
        heater->heaterOff();
        heaterOn = false;
        ui->StartStop->setText(tr("Turn Heater On"));

    }
    else
    {
        heater->heaterOn();
        heaterOn = true;
        ui->StartStop->setText(tr("Turn Heater Off"));
    }
}

void
LogExample::updateTemperature(double Temperature)
{
    QString displayTemperature = QString("%1").number(Temperature, 'f', 1);
    ui->CurrentTemperature->display(displayTemperature);
}
