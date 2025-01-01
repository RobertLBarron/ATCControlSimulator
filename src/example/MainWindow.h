/****************************************************************************//*
 * Copyright (C) 2021 Marek M. Cel
 *
 * Permission is hereby granted, free of charge, to any person obtaining
 * a copy of this software and associated documentation files (the "Software"),
 * to deal in the Software without restriction, including without limitation
 * the rights to use, copy, modify, merge, publish, distribute, sublicense,
 * and/or sell copies of the Software, and to permit persons to whom
 * the Software is furnished to do so, subject to the following conditions:
 *
 * The above copyright notice and this permission notice shall be included
 * in all copies or substantial portions of the Software.
 *
 * THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND, EXPRESS
 * OR IMPLIED, INCLUDING BUT NOT LIMITED TO THE WARRANTIES OF MERCHANTABILITY,
 * FITNESS FOR A PARTICULAR PURPOSE AND NONINFRINGEMENT. IN NO EVENT SHALL
 * THE AUTHORS OR COPYRIGHT HOLDERS BE LIABLE FOR ANY CLAIM, DAMAGES OR OTHER
 * LIABILITY, WHETHER IN AN ACTION OF CONTRACT, TORT OR OTHERWISE, ARISING FROM,
 * OUT OF OR IN CONNECTION WITH THE SOFTWARE OR THE USE OR OTHER DEALINGS
 * IN THE SOFTWARE.
 ******************************************************************************/
#ifndef MAINWINDOW_H
#define MAINWINDOW_H

////////////////////////////////////////////////////////////////////////////////

#include <QMainWindow>
#include <QTime>
#include <QLCDNumber>
#include <QLineEdit>
#include <QDial>
#include <QDoubleSpinBox>
#include <example/Atc.h>
#include <QPushButton>

////////////////////////////////////////////////////////////////////////////////

namespace Ui
{
    class MainWindow;
}

////////////////////////////////////////////////////////////////////////////////

/**
 * @brief The MainWindow class
 */
class MainWindow : public QMainWindow
{
    Q_OBJECT
    
public:

    /** Constructor. */
    explicit MainWindow( QWidget *parent = Q_NULLPTR );
    void onKeypadButtonClicked(QLineEdit *display, const QString &label);
    void onEnterButtonClicked(QLineEdit *display);
    void checkDialsToAtc();

    QTimer *redrawTimer = new QTimer(this);

    /** Destructor. */
    ~MainWindow();

public slots:
    void dialValueChanged(int value);
    void hdg_display (int value);
    void hdg_displayTwo (double value);
    void airspeed_display(int value);
    void airspeed_displayTwo(double value);
    void alt_display(int value);
    void alt_displayTwo(double value);
    void updateTimeRemaining(int timeRemaining);
    void checkValuestoDials(QString headingValue, QString altitudeValue, QString airspeedValue, QString frequencyValue);
    void pauseSimulation();
    void restartSimulation();
    void updateAltitude();
    void updateHeading();
    void updateAirSpeed();
    void HDGtimercheck(int newValue);
    void ALTtimercheck(int newValue);
    void AIRtimercheck(int newValue);
    void startDrawTimer();

signals:
    void gameOver();

protected:

    /**
     * Timer timeout callback.
     * This function is called on default timer timeout event.
     */
    void timerEvent( QTimerEvent *event );
    
private:

    Ui::MainWindow *_ui;    ///< main UI object
    Atc *atc;

    int _timerId;           ///< default timer ID
    int _steps;             ///< number of steps

    int HDGtimer = 3;
    int ALTtimer = 3;
    int AIRtimer = 3;
    QTimer *HDGtime = new QTimer;
    QTimer *ALTtime = new QTimer;            ///< time
    QTimer *AIRtime = new QTimer;            ///< time
    bool HDGtimerStart = false;
    bool ALTtimerStart = false;
    bool AIRtimerStart = false;

    double _realTime;       ///< [s] real time
    double _playTime;       ///< [s] time for automatic parameters updating

    QTime _time;            ///< time

    QLCDNumber *volumeLCDHead = new QLCDNumber;
    QLCDNumber *volumeLCDFreq = new QLCDNumber;
    QLCDNumber *volumeLCDAlt = new QLCDNumber;
    QLCDNumber *volumeLCDATCF = new QLCDNumber(10);
    QDial *volumeDialATCF = new QDial;
    int totalSteps = 10;
    int currentStep = 0;

    int headingPreValue;
    int altPreValue;
    int airspeedPreValue;

    int headingCurrentValue;
    int altitudeCurrentValue;
    int airspeedCurrentValue;

    QPushButton *pauseButton = new QPushButton("Pause", this);
    QPushButton *restartButton = new QPushButton("Restart", this);


};

////////////////////////////////////////////////////////////////////////////////

#endif // MAINWINDOW_H
