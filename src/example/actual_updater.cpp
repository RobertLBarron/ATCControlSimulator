#include "actual_updater.h"

#include <QObject>
#include <QTimer>
#include <QDebug>

Actual_Updater::Actual_Updater(QObject *parent)
    : QObject(parent), altitudeIncrement(100), speedIncrement(10)
{
}

// Destructor
Actual_Updater::~Actual_Updater()
{
    delete this;
}

// Slot to start the updating process
void Actual_Updater::startDrawUpdating()
{
    altitudeIncrement++;
    speedIncrement++;
    qDebug() << "Updater started.";
}

// Slot to stop the updating process
void Actual_Updater::stopUpdating()
{

    qDebug() << "Updater stopped.";
}

void Actual_Updater::startDrawTimer(){
    QTimer *drawTimer = new QTimer;
    drawTimer->start();
}
