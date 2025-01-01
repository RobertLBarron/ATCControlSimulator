#ifndef UPDATER_H
#define UPDATER_H

#include <QObject>
#include <QTimer>
#include "Airbus.h"

class Updater : public QObject
{
    Q_OBJECT

public:
    // Constructor and Destructor
    explicit Updater(Airbus *aircraft, QObject *parent = nullptr);
    ~Updater();

public slots:
    void startUpdating();
    void stopUpdating();


private:
    Airbus *aircraft;         // Pointer to the Airbus instance
    QTimer *updateTimer;      // Timer to trigger periodic updates
    double altitudeIncrement; // Amount to increase or decrease altitude per update
    double speedIncrement;    // Amount to increase or decrease speed per update
};

#endif // UPDATER_H
