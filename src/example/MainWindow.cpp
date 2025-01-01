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

#include <example/MainWindow.h>
#include <ui_MainWindow.h>
#include <example/Atc.h>
#include <example/airbus.h>

#include <QApplication>
#include <QVBoxLayout>
#include <QLabel>
#include <QDial>
#include <QLineEdit>
#include <QDoubleSpinBox>
#include <QMessageBox>
#include <QPushButton>

////////////////////////////////////////////////////////////////////////////////

using namespace std;

////////////////////////////////////////////////////////////////////////////////

MainWindow::MainWindow( QWidget *parent ) :
    QMainWindow( parent ),
    _ui( new Ui::MainWindow ),

    _timerId ( 0 ),
    _steps   ( 0 ),

    _realTime ( 0.0 ),
    _playTime ( 0.0 )
{
    //constructor and ui setup
    _ui->setupUi( this );

    atc = new Atc(this);
    Airbus *airbus = new Airbus(this);

    connect(atc, &Atc::timerTimeout, this, &MainWindow::updateTimeRemaining);
    connect(atc, &Atc::checkValues, this, &MainWindow::checkValuestoDials);
    connect(this, &MainWindow::gameOver, atc, &Atc::showGameOver);
    // atc->playAudioTest();

    HDGtime->setSingleShot(true);
    HDGtime->setInterval(1000);

    ALTtime->setSingleShot(true);
    ALTtime->setInterval(1000);

    AIRtime->setSingleShot(true);
    AIRtime->setInterval(1000);


    // Create your widgets for the dial
    QDial *volumeDialHead = new QDial;
    volumeLCDHead = new QLCDNumber;
    QLabel *volumeLabelHead = new QLabel("Heading");

    QDial *volumeDialFreq = new QDial;
    volumeLCDFreq = new QLCDNumber;
    QLabel *volumeLabelFreq = new QLabel("Airspeed");

    QDial *volumeDialAlt = new QDial;
    volumeLCDAlt = new QLCDNumber;
    QLabel *volumeLabelAlt = new QLabel("Altitude");

    volumeDialATCF = new QDial;
    volumeLCDATCF = new QLCDNumber;
    QLabel *volumeLabelATCF = new QLabel("ATC Frequency");

    // Customize the widgets
    volumeLCDHead->setPalette(Qt::red);
    volumeDialHead->setNotchesVisible(true);
    volumeDialHead->setMinimum(0);
    volumeDialHead->setMaximum(360);
    volumeLabelHead->setAlignment(Qt::AlignHCenter);

    volumeLCDFreq->setPalette(Qt::red);
    volumeDialFreq->setNotchesVisible(true);
    volumeDialFreq->setMinimum(0);
    volumeDialFreq->setMaximum(487);
    volumeLabelFreq->setAlignment(Qt::AlignHCenter);

    volumeLCDAlt->setPalette(Qt::red);
    volumeDialAlt->setNotchesVisible(true);
    volumeDialAlt->setMinimum(000);
    volumeDialAlt->setMaximum(40000);
    volumeLabelAlt->setAlignment(Qt::AlignHCenter);

    volumeLCDATCF->setPalette(Qt::red);
    volumeLCDATCF->setDigitCount(7);
    volumeDialATCF->setNotchesVisible(false);
    volumeDialATCF->setMinimum(11800);
    volumeDialATCF->setMaximum(13700);
    volumeLabelATCF->setAlignment(Qt::AlignHCenter);


    // Find the existing layout in the UI (HeadingLayout from the .ui file)
    QVBoxLayout *headingLayoutHead = new QVBoxLayout;
    QVBoxLayout *headingLayoutFreq = new QVBoxLayout;
    QVBoxLayout *headingLayoutAlt = new QVBoxLayout;
    QVBoxLayout *headingLayoutATCF = new QVBoxLayout;    // This accesses the HeadingLayout defined in mainwindow.ui

    // Add widgets to the existing HeadingLayouts
    headingLayoutHead->addWidget(volumeLabelHead);
    headingLayoutHead->addWidget(volumeLCDHead);
    headingLayoutHead->addWidget(volumeDialHead);

    headingLayoutFreq->addWidget(volumeLabelFreq);
    headingLayoutFreq->addWidget(volumeLCDFreq);
    headingLayoutFreq->addWidget(volumeDialFreq);

    headingLayoutAlt->addWidget(volumeLabelAlt);
    headingLayoutAlt->addWidget(volumeLCDAlt);
    headingLayoutAlt->addWidget(volumeDialAlt);

    headingLayoutATCF->addWidget(volumeLabelATCF);
    headingLayoutATCF->addWidget(volumeLCDATCF);
    headingLayoutATCF->addWidget(volumeDialATCF);

    // Connect signals and slots for the dial, label, and LCD
    connect(volumeDialHead, SIGNAL(valueChanged(int)), volumeLCDHead, SLOT(display(int)));
    connect(volumeDialFreq, SIGNAL(valueChanged(int)), volumeLCDFreq, SLOT(display(int)));
    connect(volumeDialAlt, SIGNAL(valueChanged(int)), volumeLCDAlt, SLOT(display(int)));
    QObject::connect(volumeDialATCF, SIGNAL(valueChanged(int)), this, SLOT(dialValueChanged(int)));
    QObject::connect(volumeDialHead, SIGNAL(valueChanged(int)), this, SLOT(hdg_display(int)));
    QObject::connect(volumeDialFreq, SIGNAL(valueChanged(int)), this, SLOT(airspeed_display(int)));
    QObject::connect(volumeDialAlt, SIGNAL(valueChanged(int)), this, SLOT(alt_display(int)));
    QObject::connect(volumeDialAlt, SIGNAL(valueChanged(int)), this, SLOT(ALTtimercheck(int)));
    QObject::connect(volumeDialHead, SIGNAL(valueChanged(int)), this, SLOT(HDGtimercheck(int)));
    QObject::connect(volumeDialFreq, SIGNAL(valueChanged(int)), this, SLOT(AIRtimercheck(int)));
    QObject::connect(HDGtime, &QTimer::timeout, this, &MainWindow::updateHeading);
    QObject::connect(ALTtime, &QTimer::timeout, this, &MainWindow::updateAltitude);
    QObject::connect(AIRtime, &QTimer::timeout, this, &MainWindow::updateAirSpeed);

    // AIRBUS SLOT CONNECTIONS
    connect(volumeDialHead, &QDial::valueChanged, airbus, &Airbus::setHeading);
    connect(volumeDialFreq, &QDial::valueChanged, airbus, &Airbus::setAirspeed);
    connect(volumeDialAlt, &QDial::valueChanged, airbus, &Airbus::setAltitude);
    connect(volumeDialATCF, &QDial::valueChanged, airbus, &Airbus::setFrequency);

    _ui->airbus_dials_HBoxL->addLayout(headingLayoutHead);
    _ui->airbus_dials_HBoxL->addLayout(headingLayoutFreq);
    _ui->airbus_dials_HBoxL->addLayout(headingLayoutAlt);
    _ui->airbus_dials_HBoxL->addLayout(headingLayoutATCF);

    // Create Keypad Widgets
    QVBoxLayout *keypadLayout = new QVBoxLayout;

    // Create a QLineEdit to show the input number
    QLineEdit *display = new QLineEdit;
    display->setReadOnly(true); // Make it read-only so user can't type directly
    keypadLayout->addWidget(display);

    // Create buttons for the keypad
    QStringList buttonLabels = {"7", "8", "9", "4", "5", "6", "1", "2", "3", "0", ".", "C", "Enter"};

    // Create a grid layout to hold the keypad buttons
    QGridLayout *buttonLayout = new QGridLayout;

    // Create the buttons and add them to the grid layout
    int row = 0, col = 0;
    for (const QString &label : buttonLabels) {
        QPushButton *button = new QPushButton(label);
        buttonLayout->addWidget(button, row, col);
        connect(button, &QPushButton::clicked, this, [this, display, label] {
            onKeypadButtonClicked(display, label);
        });

        col++;
        if (col > 2) { // Move to the next row after 3 columns
            col = 0;
            row++;
        }
    }

    // Add button layout to the keypad layout
    keypadLayout->addLayout(buttonLayout);
    _ui->airbus_dials_HBoxL->addLayout(keypadLayout);

    // Connect buttons to their respective slots
    connect(pauseButton, &QPushButton::clicked, this, &MainWindow::pauseSimulation);
    connect(restartButton, &QPushButton::clicked, this, &MainWindow::restartSimulation);

    // Add them to the layout, for example:
    _ui->ButtonsLayout->addWidget(pauseButton);
    _ui->ButtonsLayout->addWidget(restartButton);


    // ALL PREVIOUS FLIGHTMASTER CODE BELOW!!!
    // _timerId = startTimer( 0 );
    // orig _time.start();
    // _time.setHMS(0,0,0,0);
}

////////////////////////////////////////////////////////////////////////////////

MainWindow::~MainWindow()
{
    // cout << "Average time step: " << _realTime / ( static_cast< double >( _steps ) ) << " s" << endl;

    // if ( _timerId ) killTimer( _timerId );

    if ( _ui ) delete _ui;
    _ui = Q_NULLPTR;
}

void MainWindow::updateTimeRemaining(int timeRemaining)
{
    // Update a QLabel or any other UI element with the remaining time
    _ui->timeLabel->setText(QString::number(timeRemaining));
}

void MainWindow::checkValuestoDials(QString headingValue, QString altitudeValue, QString airspeedValue, QString frequencyValue){
    bool ok;

    qDebug() << "heading: " << headingValue << "  and  " << this->volumeLCDHead->value();
    qDebug() << "alt: " << altitudeValue << "  and  " << this->volumeLCDAlt->value();
    qDebug() << "airspeed: " << airspeedValue << "  and  " << this->volumeLCDFreq->value();
    qDebug() << "frequency: " << frequencyValue << "  and  " << this->volumeLCDATCF->value();

    if(headingValue == "null"){
        altitudeValue = "0";
    }
    if(altitudeValue == "null"){
        altitudeValue = "0";
    }
    if(airspeedValue == "null"){
        airspeedValue = "0";
    }
    if(frequencyValue == "null"){
        frequencyValue = "0";
    }


    if(((this->volumeLCDHead->value()-4 > headingValue.toDouble(&ok)) || (this->volumeLCDHead->value()+4 < headingValue.toDouble(&ok))) ||
    ((this->volumeLCDAlt->value()-300 > altitudeValue.toDouble(&ok)) || (this->volumeLCDAlt->value()+300 < altitudeValue.toDouble(&ok) )) ||
    ((this->volumeLCDFreq->value()-5 > airspeedValue.toDouble(&ok)) || (this->volumeLCDFreq->value()+5 < airspeedValue.toDouble(&ok))) ||
    (this->volumeLCDATCF->value() != frequencyValue.toDouble(&ok)))
    {
        emit gameOver();
        this->close();
    }

    if(atc->currentIndex == 16){
        emit gameOver();
        this->close();
    }
}

void MainWindow::dialValueChanged(int value)
{
    double v = (double)((double)value / 100);
    volumeLCDATCF->display(v);
}

void MainWindow::timerEvent( QTimerEvent *event )
{
//     /////////////////////////////////
//     QMainWindow::timerEvent( event );
//     /////////////////////////////////

//     // getting time step
//     // orig double timeStep = _time.restart();
//     double timeStep = _time.setHMS(0,0,0,0);

//     _realTime = _realTime + (timeStep / 1000.0);

//     // flight and navigation parameters to be shown on instruments
//     double alpha     =  0.0;
//     double beta      =  0.0;
//     double roll      =  0.0;
//     double pitch     =  0.0;
//     double heading   =  0.0;
//     double slipSkid  =  0.0;
//     double turnRate  =  0.0;
//     double devLC     =  0.0;
//     double devGS     =  0.0;
//     double fd_r      =  0.0;
//     double fd_p      =  0.0;
//     double airspeed  =  0.0;
//     double altitude  =  0.0;
//     double pressure  = 28.0;
//     double climbRate =  0.0;
//     double machNo    =  0.0;
//     double hdg       =  0.0;
//     double crs       =  0.0;
//     double vor       =  0.0;
//     double adf       =  0.0;
//     double dme       =  0.0;
//     double sel_alt   =  0.0;
//     double sel_ias   =  0.0;

//     qfi_EADI::FltMode fltMode = qfi_EADI::FltMode::Off;
//     qfi_EADI::SpdMode spdMode = qfi_EADI::SpdMode::Off;

//     qfi_EADI::LNAV lnav = qfi_EADI::LNAV::Off;
//     qfi_EADI::VNAV vnav = qfi_EADI::VNAV::Off;

//     qfi_EHSI::CDI cdi = qfi_EHSI::CDI::Off;

//     qfi_EADI::PressureMode press_mode = qfi_EADI::PressureMode::STD;

//     if ( _ui->pushButtonPlay->isChecked() )
//     {
//         // automatic parametes setting

//         _playTime = _playTime + (timeStep / 1000.0);

//         alpha     =     20.0 * sin( _playTime /  10.0 );
//         beta      =     15.0 * sin( _playTime /  10.0 );
//         roll      =    180.0 * sin( _playTime /  10.0 );
//         pitch     =     90.0 * sin( _playTime /  20.0 );
//         heading   =    360.0 * sin( _playTime /  40.0 );
//         slipSkid  =      1.0 * sin( _playTime /  10.0 );
//         turnRate  =      7.0 * sin( _playTime /  10.0 );
//         devLC     =      1.0 * sin( _playTime /  20.0 );
//         devGS     =      1.0 * sin( _playTime /  20.0 );
//         fd_r      =     30.0 * sin( _playTime /  20.0 );
//         fd_p      =     10.0 * sin( _playTime /  20.0 );
//         airspeed  = -  100.0 * cos( _playTime /  10.0 ) +  100.0;
//         altitude  = - 5000.0 * cos( _playTime /  40.0 ) + 5000.0;
//         pressure  =      2.0 * sin( _playTime /  20.0 ) +   30.0;
//         climbRate =   2000.0 * sin( _playTime /  20.0 );
//         machNo    = airspeed / 650.0;
//         hdg       =   -360.0 * sin( _playTime /  40.0 );
//         crs       =    360.0 * sin( _playTime /  20.0 );
//         vor       =      1.0 * sin( _playTime /  20.0 );
//         adf       =   -360.0 * sin( _playTime /  50.0 );
//         dme       =     99.0 * sin( _playTime / 100.0 );
//         sel_ias   = -   50.0 * cos( _playTime /  40.0 ) +   50.0;
//         sel_alt   = - 1000.0 * cos( _playTime /  40.0 ) + 1000.0;

//         _ui->spinBoxAlpha  ->setValue( alpha     );
//         _ui->spinBoxBeta   ->setValue( beta      );
//         _ui->spinBoxRoll   ->setValue( roll      );
//         _ui->spinBoxPitch  ->setValue( pitch     );
//         _ui->spinBoxSlip   ->setValue( slipSkid  );
//         _ui->spinBoxTurn   ->setValue( turnRate  );
//         _ui->spinBoxLC     ->setValue( devLC     );
//         _ui->spinBoxGS     ->setValue( devGS     );
//         _ui->spinBoxFDR    ->setValue( fd_r      );
//         _ui->spinBoxFDP    ->setValue( fd_p      );
//         _ui->spinBoxHead   ->setValue( heading   );
//         _ui->spinBoxSpeed  ->setValue( airspeed  );
//         _ui->spinBoxMach   ->setValue( machNo    );
//         _ui->spinBoxAlt    ->setValue( altitude  );
//         _ui->spinBoxPress  ->setValue( pressure  );
//         _ui->spinBoxClimb  ->setValue( climbRate );
//         _ui->spinBoxHDG    ->setValue( hdg       );
//         _ui->spinBoxCRS    ->setValue( crs       );
//         _ui->spinBoxVOR    ->setValue( vor       );
//         _ui->spinBoxADF    ->setValue( adf       );
//         _ui->spinBoxDME    ->setValue( dme       );
//         _ui->spinBoxSelIAS ->setValue( sel_ias   );
//         _ui->spinBoxSelALT ->setValue( sel_alt   );
//     }
//     else
//     {
//         // manual parametes setting

//         alpha     = _ui->spinBoxAlpha ->value();
//         beta      = _ui->spinBoxBeta  ->value();
//         roll      = _ui->spinBoxRoll  ->value();
//         pitch     = _ui->spinBoxPitch ->value();
//         heading   = _ui->spinBoxHead  ->value();
//         slipSkid  = _ui->spinBoxSlip  ->value();
//         turnRate  = _ui->spinBoxTurn  ->value();
//         devLC     = _ui->spinBoxLC    ->value();
//         devGS     = _ui->spinBoxGS    ->value();
//         fd_r      = _ui->spinBoxFDR   ->value();
//         fd_p      = _ui->spinBoxFDP   ->value();
//         airspeed  = _ui->spinBoxSpeed ->value();
//         pressure  = _ui->spinBoxPress ->value();
//         altitude  = _ui->spinBoxAlt   ->value();
//         climbRate = _ui->spinBoxClimb ->value();
//         machNo    = _ui->spinBoxMach  ->value();
//         crs       = _ui->spinBoxCRS   ->value();
//         hdg       = _ui->spinBoxHDG   ->value();
//         vor       = _ui->spinBoxVOR   ->value();
//         adf       = _ui->spinBoxADF   ->value();
//         dme       = _ui->spinBoxDME   ->value();
//         sel_ias   = _ui->spinBoxSelIAS ->value();
//         sel_alt   = _ui->spinBoxSelALT ->value();
//     }

//     switch ( _ui->comboBoxFltMode->currentIndex() )
//     {
//         case (int)qfi_EADI::FltMode::Off : fltMode = qfi_EADI::FltMode::Off ; break;
//         case (int)qfi_EADI::FltMode::FD  : fltMode = qfi_EADI::FltMode::FD  ; break;
//         case (int)qfi_EADI::FltMode::CMD : fltMode = qfi_EADI::FltMode::CMD ; break;
//     }

//     switch ( _ui->comboBoxSpdMode->currentIndex() )
//     {
//         case (int)qfi_EADI::SpdMode::Off     : spdMode = qfi_EADI::SpdMode::Off     ; break;
//         case (int)qfi_EADI::SpdMode::FMC_SPD : spdMode = qfi_EADI::SpdMode::FMC_SPD ; break;
//     }

//     switch ( _ui->comboBoxLNAV->currentIndex() )
//     {
//         case (int)qfi_EADI::LNAV::Off     : lnav = qfi_EADI::LNAV::Off     ; break;
//         case (int)qfi_EADI::LNAV::HDG     : lnav = qfi_EADI::LNAV::HDG     ; break;
//         case (int)qfi_EADI::LNAV::NAV     : lnav = qfi_EADI::LNAV::NAV     ; break;
//         case (int)qfi_EADI::LNAV::NAV_ARM : lnav = qfi_EADI::LNAV::NAV_ARM ; break;
//         case (int)qfi_EADI::LNAV::APR     : lnav = qfi_EADI::LNAV::APR     ; break;
//         case (int)qfi_EADI::LNAV::APR_ARM : lnav = qfi_EADI::LNAV::APR_ARM ; break;
//         case (int)qfi_EADI::LNAV::BC      : lnav = qfi_EADI::LNAV::BC      ; break;
//         case (int)qfi_EADI::LNAV::BC_ARM  : lnav = qfi_EADI::LNAV::BC_ARM  ; break;
//     }

//     switch ( _ui->comboBoxVNAV->currentIndex() )
//     {
//         case (int)qfi_EADI::VNAV::Off     : vnav = qfi_EADI::VNAV::Off     ; break;
//         case (int)qfi_EADI::VNAV::ALT     : vnav = qfi_EADI::VNAV::ALT     ; break;
//         case (int)qfi_EADI::VNAV::IAS     : vnav = qfi_EADI::VNAV::IAS     ; break;
//         case (int)qfi_EADI::VNAV::VS      : vnav = qfi_EADI::VNAV::VS      ; break;
//         case (int)qfi_EADI::VNAV::ALT_SEL : vnav = qfi_EADI::VNAV::ALT_SEL ; break;
//         case (int)qfi_EADI::VNAV::GS      : vnav = qfi_EADI::VNAV::GS      ; break;
//         case (int)qfi_EADI::VNAV::GS_ARM  : vnav = qfi_EADI::VNAV::GS_ARM  ; break;
//     }

//     switch ( _ui->comboBoxCDI->currentIndex() )
//     {
//         case (int)qfi_EHSI::CDI::Off  : cdi = qfi_EHSI::CDI::Off  ; break;
//         case (int)qfi_EHSI::CDI::TO   : cdi = qfi_EHSI::CDI::TO   ; break;
//         case (int)qfi_EHSI::CDI::FROM : cdi = qfi_EHSI::CDI::FROM ; break;
//     }

//     double press_coef = 1.0;

//     // switch ( _ui->comboBoxPress->currentIndex() )
//     // {
//     //     case (int)qfi_EADI::PressureMode::STD : press_mode = qfi_EADI::PressureMode::STD ; break;
//     //     case (int)qfi_EADI::PressureMode::IN  : press_mode = qfi_EADI::PressureMode::IN  ; break;
//     //     case (int)qfi_EADI::PressureMode::MB  : press_mode = qfi_EADI::PressureMode::MB  ; press_coef = 33.86; break;
//     // }

//     double fd_roll  = roll  + fd_r;
//     double fd_pitch = pitch + fd_p;

//     if ( fd_roll >  180.0 ) fd_roll =  180.0;
//     if ( fd_roll < -180.0 ) fd_roll = -180.0;

//     if ( fd_pitch >  90.0 ) fd_pitch =  90.0;
//     if ( fd_pitch < -90.0 ) fd_pitch = -90.0;

//     // setting widgets data

//     // EADI
//     _ui->widgetEADI->setFltMode     ( fltMode );
//     _ui->widgetEADI->setSpdMode     ( spdMode );
//     _ui->widgetEADI->setLNAV        ( lnav    );
//     _ui->widgetEADI->setVNAV        ( vnav    );
//     _ui->widgetEADI->setRoll        ( roll    );
//     _ui->widgetEADI->setPitch       ( pitch   );
//     _ui->widgetEADI->setFPM         ( alpha, beta );
//     _ui->widgetEADI->setSlipSkid    ( slipSkid  );
//     _ui->widgetEADI->setTurnRate    ( turnRate / 6.0 );
//     _ui->widgetEADI->setDots        ( devGS, devLC, true, true );
//     _ui->widgetEADI->setFD          ( fd_roll, fd_pitch, true );
//     _ui->widgetEADI->setStall       ( _ui->pushButtonStall->isChecked() );
//     _ui->widgetEADI->setHeading     ( heading   );
//     _ui->widgetEADI->setAirspeed    ( airspeed  );
//     _ui->widgetEADI->setMachNo      ( machNo    );
//     _ui->widgetEADI->setAltitude    ( altitude  );
//     _ui->widgetEADI->setPressure    ( press_coef * pressure, press_mode );
//     _ui->widgetEADI->setClimbRate   ( climbRate / 1000.0 );
//     _ui->widgetEADI->setAirspeedSel ( sel_ias );
//     _ui->widgetEADI->setAltitudeSel ( sel_alt );
//     _ui->widgetEADI->setHeadingSel  ( hdg );
//     //_ui->widgetEADI->setIdentifier  ( "ILAX" , true );
//     //_ui->widgetEADI->setDistance    ( dme    , true );
//     _ui->widgetEADI->setVfe         (  85.0 );
//     _ui->widgetEADI->setVne         ( 158.0 );

//     // EHSI
//     _ui->widgetEHSI->setHeading    ( heading   );
//     _ui->widgetEHSI->setCourse     ( crs );
//     _ui->widgetEHSI->setBearing    ( adf  , true );
//     _ui->widgetEHSI->setDeviation  ( vor  , cdi );
//     _ui->widgetEHSI->setDistance   ( dme  , true );
//     _ui->widgetEHSI->setHeadingSel ( hdg );
// /*
//     Basic Six (flight instruntments per video)
//     _ui->widgetSix->getAI()  ->setRoll      ( roll      );
//     _ui->widgetSix->getAI()  ->setPitch     ( pitch     );
//     _ui->widgetSix->getALT() ->setAltitude  ( altitude  );
//     _ui->widgetSix->getALT() ->setPressure  ( pressure  );
//     _ui->widgetSix->getASI() ->setAirspeed  ( airspeed  );
//     _ui->widgetSix->getHI()  ->setHeading   ( heading   );
//     _ui->widgetSix->getTC()  ->setSlipSkid  ( slipSkid * 15.0 );
//     _ui->widgetSix->getTC()  ->setTurnRate  ( turnRate  );
//     _ui->widgetSix->getVSI() ->setClimbRate ( climbRate );
// */
//     // redrawing widgets

//     _ui->widgetEADI->redraw();
//     _ui->widgetEHSI->redraw();
// /*
//     _ui->widgetSix->getAI()  ->redraw();
//     _ui->widgetSix->getALT() ->redraw();
//     _ui->widgetSix->getASI() ->redraw();
//     _ui->widgetSix->getHI()  ->redraw();
//     _ui->widgetSix->getTC()  ->redraw();
//     _ui->widgetSix->getVSI() ->redraw();
// */
//     // incrementing number of steps

//     _steps++;
}

void MainWindow::startDrawTimer(){
    this->redrawTimer->start(100);
}

void MainWindow::HDGtimercheck(int newValue){
    HDGtime->start();
    headingCurrentValue = newValue;

}

void MainWindow::ALTtimercheck(int newValue){
    ALTtime->start();
    altitudeCurrentValue = newValue;
}

void MainWindow::AIRtimercheck(int newValue){
    AIRtime->start();
    airspeedCurrentValue = newValue;
}


void MainWindow::updateHeading()
{
    qDebug() << "updateAltitude was called with " << headingCurrentValue;
    // Define the timer inside the function
    QTimer *timer = new QTimer(this);
    int interval = 200;  // Timer interval in ms (100ms)
    int duration = 2000; // Total duration of the animation in ms (1 second)
    int steps = duration / interval; // Number of steps (10 steps in 1 second)
    double currentHeading = headingPreValue;
    double increment =  (headingCurrentValue - headingPreValue) / 10;

    // Track the current step
    int currentStep = 1;
    timer->start(200);

    connect(timer, &QTimer::timeout, this, [=]() mutable {

        if (currentStep <= steps) {
            currentHeading += increment;

            hdg_displayTwo((currentHeading));
            currentStep++; // Increment the step

        } else {
            hdg_displayTwo((headingCurrentValue));
            timer->stop();
            headingPreValue = headingCurrentValue;
        }
    });
}

void MainWindow::hdg_display(int value){
    double hdg = (double)((double)value);
    qDebug() << "in hdg " << value;
    _ui->widgetEADI->setHeadingSel( hdg);
    _ui->widgetEHSI->setHeadingSel( hdg);
    _ui->widgetEADI->redraw();
    _ui->widgetEHSI->redraw();
    // _ui->widgetEHSI->

}

void MainWindow::hdg_displayTwo(double value){
    double hdg = (double)((double)value);
    qDebug() << "in hdg " << value;
    _ui->widgetEADI->setHeading     ( hdg   );
    _ui->widgetEHSI->setHeading   ( hdg   );
    _ui->widgetEADI->redraw();
    _ui->widgetEHSI->redraw();
    // _ui->widgetEHSI->

}

void MainWindow::updateAirSpeed()
{
    qDebug() << "updateAltitude was called with " << airspeedCurrentValue;
    // Define the timer inside the function
    QTimer *timer = new QTimer(this);
    int interval = 200;  // Timer interval in ms (100ms)
    int duration = 2000; // Total duration of the animation in ms (1 second)
    int steps = duration / interval; // Number of steps (10 steps in 1 second)
    double currentAirSpeed = airspeedPreValue;
    double increment =  (airspeedCurrentValue - airspeedPreValue) / 10;

    // Track the current step
    int currentStep = 1;
    timer->start(200);

    connect(timer, &QTimer::timeout, this, [=]() mutable {

        if (currentStep <= steps) {
            currentAirSpeed += increment;

            airspeed_displayTwo((currentAirSpeed));
            currentStep++; // Increment the step

        } else {
            // Timer has completed, stop the timer and call the final function
            airspeed_displayTwo(( airspeedCurrentValue));
            timer->stop();
            airspeedPreValue = airspeedCurrentValue;
        }
    });
}

void MainWindow::airspeed_display(int value){
    double air = (double)((double)value);
    qDebug() << "in airs " << value;
    _ui->widgetEADI->setAirspeedSel( air);
    _ui->widgetEADI->redraw();

}

void MainWindow::airspeed_displayTwo(double value){
    double air = (double)((double)value);
    qDebug() << "in airs two " << value;
    _ui->widgetEADI->setAirspeed   ( air  );
    _ui->widgetEADI->redraw();

}

void MainWindow::updateAltitude()
{
    qDebug() << "updateAltitude was called with " << altitudeCurrentValue;
    // Define the timer inside the function
    QTimer *timer = new QTimer(this);
    int interval = 50;  // Timer interval in ms (100ms)
    int duration = 2000; // Total duration of the animation in ms (1 second)
    int steps = duration / interval; // Number of steps (10 steps in 1 second)
    double currentAltitude = altPreValue;
    double increment =  (altitudeCurrentValue - altPreValue) / 40;

    // Track the current step
    int currentStep = 1;
    timer->start(50);

    connect(timer, &QTimer::timeout, this, [=]() mutable {

        if (currentStep <= steps) {
            qDebug() << "Step " << currentStep;

            currentAltitude += increment;
            qDebug() << "CurrentAlt " << currentAltitude;

            alt_displayTwo((currentAltitude));
            currentStep++; // Increment the step

        } else {
            // Timer has completed, stop the timer and call the final function
            alt_displayTwo((altitudeCurrentValue));
            timer->stop();
            altPreValue = altitudeCurrentValue;
        }
    });
}

void MainWindow::alt_display(int value){
    double alt = (double)((double)value);
    qDebug() << "in alt " << value;
    _ui->widgetEADI->setAltitudeSel( alt);
    _ui->widgetEADI->redraw();

}

void MainWindow::alt_displayTwo(double value){
    double alt = (double)((double)value);
    qDebug() << "in alt two " << value;
    _ui->widgetEADI->setAltitude    ( alt );
    _ui->widgetEADI->redraw();

}



// Keypad button click handler
void MainWindow::onKeypadButtonClicked(QLineEdit *display, const QString &label)
{
    QString currentText = display->text();

    if (label == "C") {
        // Clear the display
        display->clear();
    } else if (label == "Enter") {
        // Handle the "Enter" button press
        onEnterButtonClicked(display);
    } else {
        // Append the label to the current text (for digits or decimal point)
        display->setText(currentText + label);
    }
}

// Handle the "Enter" button press
void MainWindow::onEnterButtonClicked(QLineEdit *display)
{
    QString input = display->text();

    // Check if the input is a valid number (taken advice from online https://forum.qt.io/post/123547)
    bool ok;
    double value = input.toDouble(&ok);
    if (ok) {
        // Update the ATC Frequency's LCD with the value
        // Set the value of the ATC Frequency dial (ensure the value is within the valid range)
        double dialValue = static_cast<double>(value);
        if (dialValue >= (volumeDialATCF->minimum() / 100) && dialValue <= (volumeDialATCF->maximum() / 100)) {
            volumeDialATCF->setValue(dialValue * 100);

        }
    } else {
        // Optionally, handle invalid input (e.g., show an error message or clear the display)
        display->clear();
    }
}

void MainWindow::pauseSimulation() {
    qDebug() << "pause simulation activated";
    if (atc->isPaused == false) {
        atc->stopTimer();
        atc->isPaused = true;      // Set pause state
        pauseButton->setText("Resume");
    } else {
        atc->startTimer(atc->timeRemaining);  // Restart the timer with the same interval as before (e.g., 1000 ms)
        atc->isPaused = false;  // Reset pause state
        pauseButton->setText("Pause");
    }
}

void MainWindow::restartSimulation(){
    qDebug() << "restart simulation activated";
        atc->startTimer(30);  // Restart the timer with the same interval as before (e.g., 1000 ms)
        atc->isPaused = false;  // Reset pause state
        pauseButton->setText("Pause");

        if(atc->currentIndex <= 0){
            emit atc->playNextAudio(atc->atcAudio[atc->currentIndex]);
        } else{
            emit atc->playNextAudio(atc->atcAudio[atc->currentIndex-1]);
        }

}
