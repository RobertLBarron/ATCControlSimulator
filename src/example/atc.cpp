#include "atc.h"
#include <QDebug>
#include <QDir>
#include <QtMultimedia/QtMultimedia>
#include <QMessageBox>
#include <QDialog>
#include <QLabel>
#include <QPushButton>
#include <QVBoxLayout>  // For layout

using namespace std;

Atc::Atc(QObject *parent) : QObject(parent), atcTimer(new QTimer(this))
{
    connect(atcTimer, &QTimer::timeout, this, &Atc::updateTimer);  // Connect to updateTimer

    loadInstructions();  // Load the instructions from the text file
    loadAudio(); //load the audio from the q resource
    startTimer(30);

}

Atc::~Atc() {
    delete atcTimer;
}

void Atc::loadInstructions() {
    QDir qcur_dir = QDir::currentPath();
    QDir qhome_dir = QDir::home();  // QDir::home() gives the home directory as a QDir object
    string home_dir = qhome_dir.path().toStdString();  // Convert to std::string

    // Use forward slashes for the file path to avoid escape character issues
    // string fileString = home_dir + "/Downloads/a_term_proj/atc_data/atc_instructions.txt"; // my a_term_project is in my downloads folder, delete that for most people
    string fileString = ":/audio/atc_instructions.txt"; // this is another filepath that leads to the qrc file.

    // Convert std::string to QString
    QString filePath = QString::fromStdString(fileString);

    // Debug output
    qDebug() << "Current directory:" << QDir::currentPath();
    qDebug() << "Home directory:" << QDir::homePath();
    qDebug() << "home_dir: " << QString::fromStdString(home_dir);  // Display home directory as QString

    // Open the file with the correct QString path
    QFile file(filePath);  // Path to the instructions file
    if (file.open(QIODevice::ReadOnly | QIODevice::Text)) {
        QTextStream in(&file);

        // Read each line from the file
        while (!in.atEnd()) {
            QString line = in.readLine();

            // Skip empty lines or lines that start with "//" (header)
            if (line.isEmpty() || line.startsWith("//")) {
                continue;
            }

            // Split the line by commas
            QStringList parts = line.split(',');

            // Check that the line has 8 fields
            if (parts.size() == 8) {
                std::vector<QString> instruction;

                instruction.push_back(parts[0].trimmed());  // fileNumber
                instruction.push_back(parts[1].trimmed());  // turnDirection
                instruction.push_back(parts[2].trimmed());  // heading
                instruction.push_back(parts[3].trimmed());  // altitude
                instruction.push_back(parts[4].trimmed()); // speed
                instruction.push_back(parts[5].trimmed()); // radioFreq
                instruction.push_back(parts[6].trimmed()); // beacons
                instruction.push_back(parts[7].trimmed().remove(';')); // fileName (removing the semicolon)

                // qDebug output for the fields being read
                for (int i = 0; i < 8; i++) {
                    qDebug() << "Instruction field" << i << ":" << instruction[i];
                }

                // Add the instruction to the vector
                atcInstructions.push_back(instruction);
            } else {
                qDebug() << "Invalid instruction line: " << line;
            }
        }
        file.close();
    } else {
        qDebug() << "Error: Could not open file for reading.";
    }
}

void Atc::loadAudio() {
    qInfo() << " loading names of audio files accessible from application resource ";

    atcAudio.push_back("qrc:/audio/atc_seattle_10.mp3");
    atcAudio.push_back("qrc:/audio/atc_seattle_11.mp3");
    atcAudio.push_back("qrc:/audio/atc_seattle_12.mp3");
    atcAudio.push_back("qrc:/audio/atc_seattle_13.mp3");
    atcAudio.push_back("qrc:/audio/atc_seattle_14.mp3");
    atcAudio.push_back("qrc:/audio/atc_seattle_15.mp3");
    atcAudio.push_back("qrc:/audio/atc_seattle_16_9-09_2500.mp3");
    atcAudio.push_back("qrc:/audio/atc_seattle_17.mp3");
    atcAudio.push_back("qrc:/audio/atc_seattle_18.mp3");
    atcAudio.push_back("qrc:/audio/atc_seattle_19.mp3");
    atcAudio.push_back("qrc:/audio/atc_seattle_20_10-56_1000.mp3");
    atcAudio.push_back("qrc:/audio/atc_seattle_21_11-29_100above.mp3");
    atcAudio.push_back("qrc:/audio/atc_seattle_22_11-33_300.mp3");
    atcAudio.push_back("qrc:/audio/atc_seattle_23_11-37_minimum.mp3");
    atcAudio.push_back("qrc:/audio/atc_seattle_24_11-45_100.mp3");
    atcAudio.push_back("qrc:/audio/atc_seattle_25_11-49_50toRetard.mp3");
    atcAudio.push_back("qrc:/audio/atc_seattle_26_taxi.mp3");
}

void Atc::startTimer(int time) {
    timeRemaining = time;  // Initialize timeRemaining to 30 seconds at the start
    atcTimer->start(1000);  // Start the timer to trigger every 1 second (1000 ms)
}

void Atc::stopTimer() {
    atcTimer->stop();
}

void Atc::showGameOver() {
    this->stopTimer();
    this->gameStart = false;
    // Create the Game Over dialog
    // Create the dialog object
    QDialog *gameOverDialog = new QDialog();

    // Set the window title and size (optional)
    gameOverDialog->setWindowTitle("Game Over");
    gameOverDialog->resize(300, 200);

    // Create a label to show the message in the dialog
    QLabel *loseLabel = new QLabel("Game Over! You have lost the game.", gameOverDialog);
    QLabel *winLabel = new QLabel("Game Over! You have WON the game!", gameOverDialog);

    // Optionally, add a button to close the dialog
    QPushButton *closeButton = new QPushButton("Close", gameOverDialog);
    connect(closeButton, &QPushButton::clicked, gameOverDialog, &QDialog::accept);

    if(currentIndex < 16){
    // Set up layout for the dialog
    QVBoxLayout *layout = new QVBoxLayout();
    layout->addWidget(loseLabel);
    layout->addWidget(closeButton);
    gameOverDialog->setLayout(layout);
    } else{
        QVBoxLayout *layout = new QVBoxLayout();
        layout->addWidget(winLabel);
        layout->addWidget(closeButton);
        gameOverDialog->setLayout(layout);
    }

    // Show the dialog
    gameOverDialog->show();

    // Ensure the dialog is in front and focused
    gameOverDialog->raise();
    gameOverDialog->activateWindow();
}

void Atc::updateTimer() {
    if (timeRemaining > 0) {
        timeRemaining--;
    }

    // Emit signal with the remaining time (for example, to update a UI display)
    emit timerTimeout(timeRemaining);

    // If the time is up, you can also stop the timer or perform other actions
    if (timeRemaining == 0) {
        if(gameStart == false){
            gameStart = true;
        } else{
            emit checkValues(headingValue, altitudeValue, airspeedValue, frequencyValue);
        }
        if(atcTimer->isActive()){
        nextInstr();
        currentIndex++; // this could go inside nextInstr, but I'm too tired and can't handle worrying about bugs
        timeRemaining = 30;
        }


    }
}

// void Atc::playAudioTest() { //this is a test function for a variety of things, not essential to simulation function
//     player2 = new QMediaPlayer;
//     audioOutput2 = new QAudioOutput;

//     player2->setAudioOutput(audioOutput2);
//     audioOutput2->setVolume(0);

//     // Loop through all the audio files in the atcAudio vector
//     int delay = 10000;  // Start with the first 10 seconds delay

//     for (int i = 0; i < atcAudio.size(); ++i) {
//         QString audioFile = atcAudio[i];

//         // Set up a timer to delay playback of each audio by the 'delay' amount
//         QTimer::singleShot(delay, [this, audioFile]() {
//             // Set the source to the current audio file and play it
//             this->player2->setSource(audioFile);
//             this->player2->play();
//         });

//         // Increase the delay for the next audio file (10s for first, 15s for others)
//         delay += (i == 0 ? 25000 : 25000);  // 15 seconds delay after the first file

//         qDebug() << "Random pull: " << atcInstructions[2][2];
//     }
// }
void Atc::nextInstr() {
    if (currentIndex < atcInstructions.size()) {
        std::vector<QString> currentInstruction = atcInstructions[currentIndex];

        if(currentInstruction[2] != "null"){
            this->headingValue = currentInstruction[2];
        }
        if(currentInstruction[3] != "null"){
            this->altitudeValue = currentInstruction[3];
        }
        if(currentInstruction[4] != "null"){
            this->airspeedValue = currentInstruction[4];
        }
        if(currentInstruction[5] != "null"){
            this->frequencyValue = currentInstruction[5];
        }

        // Debug output to check the instruction details
        qDebug() << "Instruction details:"
                 << "File Number:" << currentInstruction[0]
                 << "Turn Direction:" << currentInstruction[1]
                 << "Heading:" << currentInstruction[2]
                 << "Altitude:" << currentInstruction[3]
                 << "Speed:" << currentInstruction[4]
                 << "Radio Frequency:" << currentInstruction[5]
                 << "Beacons:" << currentInstruction[6]
                 << "File Name:" << currentInstruction[7];

        emit playNextAudio(atcAudio[currentIndex]);
    } else {
        showGameOver();
    }
}

void Atc::playNextAudio(QString audioFile){
    player2 = new QMediaPlayer;
    audioOutput2 = new QAudioOutput;

    player2->setAudioOutput(audioOutput2);
    audioOutput2->setVolume(10);

    player2->setSource(audioFile);
    player2->play();
}
