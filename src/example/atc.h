#ifndef ATC_H
#define ATC_H

#include <QObject>
#include <QTimer>
#include <QString>
#include <QFile>
#include <QTextStream>
#include <vector>  // To use std::vector
#include <QtMultimedia/QtMultimedia>

using namespace std;
// Forward declaration of MainWindow if needed
class MainWindow;

class Atc : public QObject
{
    Q_OBJECT

public:
    // Constructor and Destructor
    explicit Atc(QObject *parent = nullptr);  // Constructor with parent QObject
    ~Atc();  // Destructor

    // Public methods for starting and stopping the ATC timer
    void startTimer(int time);  // Starts the ATC timer
    void stopTimer();   // Stops the ATC timer

    void playAudioTest();

    bool isPaused = false;
    int timeRemaining;

    std::vector<QString> atcAudio;
    int currentIndex = 0;

public slots:
    void showGameOver();
    void playNextAudio(QString audioFile);  // This will trigger audio playback in MainWindow

signals:
    void timerTimeout(int timeRemaining);  // Signal to notify MainWindow of time remaining
    void checkValues(QString headingValue, QString altitudeValue, QString airspeedValue, QString frequencyValue);

private slots:
    // Slot that is triggered when the ATC timer times out (i.e., next instruction)
    void nextInstr();  // This will process the next ATC instruction
    void updateTimer();

private:
    // Member variables
    QTimer *atcTimer;  // The timer to trigger ATC instruction events

    std::vector<std::vector<QString>> atcInstructions;  // Container for the parsed ATC instructions

    QMediaPlayer *player2;
    QAudioOutput *audioOutput2;

    bool gameStart = false;

    QString headingValue;
    QString altitudeValue;
    QString airspeedValue;
    QString frequencyValue;

    // Private method to load the instructions from the file
    void loadInstructions();  // Reads the instructions from the file
    void loadAudio();

signals:
    // Signal to play the next audio file based on the instruction
};

#endif // ATC_H
