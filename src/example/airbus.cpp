#include "airbus.h"
#include <QDebug>

Airbus::Airbus(QObject *parent) : QObject(parent),
    m_heading(0.0), m_airspeed(0.0), m_altitude(0.0), m_frequency(0.0)
{
    // the default value doesn't really matter, the slots in mainwindow will be updating them or an edge case will be handeled.
}

double Airbus::heading() const {
    return m_heading;
}

double Airbus::airspeed() const {
    return m_airspeed;
}

double Airbus::altitude() const {
    return m_altitude;
}

double Airbus::frequency() const {
    return m_frequency;
}

void Airbus::setHeading(double heading) {
    m_heading = heading;
    qDebug() << "Updated heading:" << m_heading;
}

void Airbus::setAirspeed(double airspeed) {
    m_airspeed = airspeed;
    qDebug() << "Updated airspeed:" << m_airspeed;
}

void Airbus::setAltitude(double altitude) {
    m_altitude = altitude;
    qDebug() << "Updated altitude:" << m_altitude;
}

void Airbus::setFrequency(double frequency) {
    m_frequency = frequency;
    qDebug() << "Updated frequency:" << m_frequency;
}
