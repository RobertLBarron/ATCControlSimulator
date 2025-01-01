#include "updater.h"
#include <QDebug>

// Constructor
Updater::Updater(Airbus *aircraft, QObject *parent)
    : QObject(parent), aircraft(aircraft), altitudeIncrement(100), speedIncrement(10)
{
    updateTimer = new QTimer(this);
}

// Destructor
Updater::~Updater()
{
    delete updateTimer;
}

// Slot to start the updating process
void Updater::startUpdating()
{
    updateTimer->start(1000);  // Start updating every 1000 ms (1 second)
    qDebug() << "Updater started.";
}

// Slot to stop the updating process
void Updater::stopUpdating()
{
    updateTimer->stop();
    qDebug() << "Updater stopped.";
}

