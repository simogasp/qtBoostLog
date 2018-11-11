#ifndef LOGGER_H
#define LOGGER_H

#include <QDialog>

#include "heater.h"

namespace Ui {
class Logger;
}

class LogExample : public QDialog
{
    Q_OBJECT

public:
    explicit LogExample(QWidget *parent = 0);
    ~LogExample();

public slots:
    void StartStop();
    void updateTemperature(double temperature);

private:
    Ui::Logger *ui;

private:
    Heater *heater;
    bool    heaterOn;
};

#endif // LOGGER_H
