#ifndef HEATER_H
#define HEATER_H

#include "temperaturelog.h"
#include "eventlog.h"

#include <QObject>
#include <QTimer>

class Heater : public QObject
{
    Q_OBJECT
public:
    Heater();
    ~Heater();

public:
    enum class State { IDLE, HEATING, HOLDING };
    const double roomTemperature = 21.0;   // in C, 70 F room temperature

signals:
    void Temperature(double temperature);
    void AtTemperature(bool);

public slots:
    void updateTemperature();
    void updateSetPoint(double point);
    void updateRampRate(int rate);

public:
    void heaterOn();
    void heaterOff();

private:
    double currentTemperature;  // in Celsius
    double setPoint;            // desired temperature
    double rampRate;            // ramp rate in degrees per second
    State  currentState;        // IDLE, HEATING or HOLDING
    int    timerInterval;       // ms timer interval
    int    holdCounter;         // counter in hold mode
    QTimer* timer;

    TemperatureLog::TemperatureLog* temperatureLog;
    EventLog::EventLog* eventLog;
};

#endif // HEATER_H
