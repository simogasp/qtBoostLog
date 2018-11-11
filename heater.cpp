#include "heater.h"
#include <ctime>

//using namespace TemperatureLog;
//using namespace EventLog;

Heater::Heater() :
    currentTemperature(roomTemperature),
    setPoint(0.0),
    rampRate(5.0),
    currentState(State::IDLE),
    timerInterval(200)
{

    temperatureLog = new TemperatureLog::TemperatureLog;
    eventLog = new EventLog::EventLog;

    time_t timev;
    time(&timev);
    qsrand(timev);   // seed rand with time

    timer = new QTimer(this);

    connect(timer, SIGNAL(timeout()), this, SLOT(updateTemperature()));
    timer->start(timerInterval);
}


Heater::~Heater()
{
    // stop timer
    timer->stop();

    delete timer;

    delete temperatureLog;
    delete eventLog;
}


void
Heater::updateTemperature()
{
    if(currentState == State::IDLE)
    {
        // if the heater is not on, then it cools down at a rate
        // of 1 degree/second until it hits room temperature
        if(currentTemperature > roomTemperature)
        {
            currentTemperature -= 0.2;

            // log the temperatures as it cools down
            temperatureLog->logTemperature(setPoint, currentTemperature);
        }

        Temperature(currentTemperature);
        if(currentTemperature < setPoint)
            AtTemperature(false);

    }
    else if(currentState == State::HEATING)
    {
        // the new temperature = current temperature + (rampRate/interval)
        double ticks = 1000.0/static_cast<double>(timerInterval);
        currentTemperature  += rampRate/ticks;
        if(currentTemperature >= setPoint)
        {
            currentState = State::HOLDING;
            AtTemperature(true);
            holdCounter = 0;
            eventLog->LogEvent(EventLog::notification, "Hold temperature achieved");
        }

        temperatureLog->logTemperature(setPoint, currentTemperature);
        Temperature(currentTemperature);
    }
    else
    {
        // while we are holding, check to make sure the temperature
        // stays within 1 degree of the setpoint.
        holdCounter++;

        // occasionally create an error
        if ((holdCounter % 50) == 0)
        {
            int randval = qrand();
            // allow for a potential 2.8 degree swing
            double temperatureDelta = (randval % 28) / 10.0;

            // but only allow temperature error every now and then
            if (randval & 3)
                currentTemperature = setPoint + temperatureDelta;
            else if(randval & 0xc)
                currentTemperature = setPoint - temperatureDelta;
            else
                currentTemperature = setPoint;
        }
        else
            currentTemperature = setPoint;

        temperatureLog->logTemperature(setPoint, currentTemperature);
        Temperature(currentTemperature);

        // check to see how bad the swing is
        if (fabs(setPoint - currentTemperature) > 2.0)
            eventLog->LogEvent(EventLog::critical, "Dangerous temperature range");
        else if (fabs(setPoint - currentTemperature) > 0.5)
            eventLog->LogEvent(EventLog::error, "Temperature out of range");

    }
}

void
Heater::heaterOn()
{
    currentState = State::HEATING;
    eventLog->LogEvent(EventLog::notification, "Heater on");
}


void
Heater::heaterOff()
{
    currentState = State::IDLE;
    eventLog->LogEvent(EventLog::notification, "Heater off");
}

void
Heater::updateSetPoint(double point)
{
    setPoint = point;
}

void
Heater::updateRampRate(int rate)
{
    rampRate = static_cast<double>(rate);
}
