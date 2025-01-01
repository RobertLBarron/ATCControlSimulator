#ifndef ACTUAL_UPDATER_H
#define ACTUAL_UPDATER_H

#include <QObject>
#include <QTimer>

class Actual_Updater : public QObject
{
    Q_OBJECT;

public:
    // Constructor and Destructor
    explicit Actual_Updater(QObject *parent = nullptr);
    ~Actual_Updater();


    void startDrawUpdating();


    void stopUpdating();


private:
    double altitudeIncrement; // Amount to increase or decrease altitude per update
    double speedIncrement;    // Amount to increase or decrease speed per update

    void startDrawTimer();
};

#endif // ACTUAL_UPDATER_H
