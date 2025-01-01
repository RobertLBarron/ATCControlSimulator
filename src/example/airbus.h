#ifndef AIRBUS_H
#define AIRBUS_H

#include <QObject>

class Airbus : public QObject
{
    Q_OBJECT

public:
    explicit Airbus(QObject *parent = nullptr);

    // Getters for the variables
    double heading() const;
    double airspeed() const;
    double altitude() const;
    double frequency() const;

public slots:
    // Slots to update the aircraft data
    void setHeading(double heading);
    void setAirspeed(double airspeed);
    void setAltitude(double altitude);
    void setFrequency(double frequency);

private:
    // Aircraft state variables
    double m_heading;
    double m_airspeed;
    double m_altitude;
    double m_frequency;
};

#endif // AIRBUS_H
