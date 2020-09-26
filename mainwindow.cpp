#include "mainwindow.h"
#include "ui_mainwindow.h"

//#define length 80
using namespace std;

MainWindow::MainWindow(QWidget* parent) :
  QMainWindow(parent),
  ui(new Ui::MainWindow) {
  ui->setupUi(this);
  ui->actionDisconnect->setEnabled(false);
  connect(ui->actionConnect,&QAction::triggered, this, &MainWindow::openSerialPort);
  connect(ui->sendButton,&QPushButton::clicked, this, &MainWindow::changeData);
  connect(ui->actionDisconnect,&QAction::triggered, this,&MainWindow::closeSerialPort);
  ui->sendButton->setEnabled(false);
}

MainWindow::~MainWindow() {
  delete ui;
}

bool MainWindow::initPort(HANDLE port, int baud_rate, QString name) {
    if (port == INVALID_HANDLE_VALUE) {
        ui->statusbar->showMessage("Unable to init" + name);
    } else {

    }
}

void MainWindow::openSerialPort() {
    clientHandle = CreateFileA("COM1", GENERIC_WRITE, 0, nullptr, OPEN_EXISTING, NULL, nullptr);
    if (clientHandle == INVALID_HANDLE_VALUE) {
        ui->statusbar->showMessage("Unable to open COM1");
    } else {

    }
    //PortInitialization
        DCB dcb;
        GetCommState(COM1, &dcb);

        dcb.BaudRate = ui->baudRateBox->currentText().toInt();//CBR_9600;
        //Обычно используемый стандарт для последовательной конфигурации 8N1 : восемь (8) битов данных, нет (N) бита четности и один (1) стоповый бит.
        dcb.ByteSize = 8;
        dcb.Parity = NOPARITY;
        dcb.StopBits = ONESTOPBIT;
        if (!SetCommState(COM1, &dcb))
        {
            ui->statusbar->showMessage("SET ERROR");
            CloseHandle(COM1);
            return ;
        }
        DCB LpDCB = { '0' };
        //CHECK SETTING PARAMETRES
       if (GetCommState(COM1, &LpDCB))
       {
              ui->portName->setText("COM1");
              ui->baudRate->setText(QString::number(LpDCB.BaudRate));
              ui->amountofbitslabel->setText(QString::number(LpDCB.ByteSize));
              if ( LpDCB.StopBits == ONESTOPBIT) ui->serialNumberLabel_2->setText("1");
                             else if ((int)LpDCB.StopBits == ONE5STOPBITS) ui->serialNumberLabel_2->setText("1.5");
                             else if ((int)LpDCB.StopBits == TWOSTOPBITS) ui->serialNumberLabel_2->setText("2");
                ui->parityLabel->setText(QString::number(LpDCB.Parity));
                if (LpDCB.fBinary == TRUE) ui->binaryMode->setText( "TRUE" );
                else ui->binaryMode->setText( "FALSE" );
                if (LpDCB.fParity == TRUE) ui->checkParityLabel->setText( "TRUE" );
                else ui->checkParityLabel->setText( "FALSE" );
          }
           else ui->statusbar->showMessage("GET ERROR");

            PurgeComm(COM1, PURGE_RXCLEAR);
            PurgeComm(COM1, PURGE_TXCLEAR);

            COM2 = INVALID_HANDLE_VALUE;
            if ((COM2 = CreateFileA("COM2", GENERIC_READ | GENERIC_WRITE, NULL, NULL, OPEN_EXISTING, NULL, NULL)) == INVALID_HANDLE_VALUE)
            {
                return ;
            }

            DCB dcb2;
            GetCommState(COM2, &dcb2);
            dcb2.BaudRate = ui->baudRateBox_2->currentText().toInt();
            dcb2.ByteSize = 8;
            dcb2.Parity = NOPARITY;
            dcb2.StopBits = ONESTOPBIT;
            if (!SetCommState(COM2, &dcb2))
            { cout << "Error:3 " << GetLastError() << endl;
                CloseHandle(COM2);
                return ;
            }
            DCB LpDCB2 = { '0' };
           if (GetCommState(COM2, &LpDCB2) != 0)
           {
                  ui->portName_5->setText("COM2");
                  ui->baudRate_5->setText(QString::number(LpDCB2.BaudRate));
                  ui->amountofbitslabel_5->setText(QString::number(LpDCB2.ByteSize));
                  if ( LpDCB2.StopBits == ONESTOPBIT) ui->serialNumberLabel_6->setText("1");
                                 else if ((int)LpDCB2.StopBits == ONE5STOPBITS) ui->serialNumberLabel_6->setText("1.5");
                                 else if ((int)LpDCB2.StopBits == TWOSTOPBITS) ui->serialNumberLabel_6->setText("2");
                    ui->parityLabel_5->setText(QString::number(LpDCB2.Parity));
                    if (LpDCB2.fBinary == TRUE) ui->binaryMode_5->setText( "TRUE" );
                    else ui->binaryMode_5->setText( "FALSE" );
                    if (LpDCB2.fParity == TRUE) ui->checkParityLabel_5->setText( "TRUE" );
                    else ui->checkParityLabel_5->setText( "FALSE" );
              }
            ui->sendButton->setEnabled(true);
            ui->actionDisconnect->setEnabled(true);
}

void MainWindow::changeData()
{
    DWORD readed = 0;

    //
    long len;
    len = ui->userText->toPlainText().length();
    char *buf = new char[len];


  //  strcpy(buf,ui->userText->toPlainText().toStdString().c_str());
    //buf[len] = '\n';

    WriteFile(COM1, buf, len, &readed, NULL);
    if(len != readed){
        ui->statusbar->showMessage("PORT WAS DISKONECTED");
        return;
    }
    for (int i = 0; i < len; i++)
    {
        buf[i] = 0;
    }
    PurgeComm(COM1, PURGE_RXCLEAR);
    PurgeComm(COM1, PURGE_TXCLEAR);

    readed = 0;
    ReadFile(COM2, buf, len, &readed, NULL);
    if(len != readed){
        ui->statusbar->showMessage("PORT WAS DISKONECTED");
        return;
    }
    //buf[len] = '\n';
    ui->userText->clear();
    ui->outputWindow->setText(ui->outputWindow->toPlainText() + QString(buf) + "\n");
   // ui->outputWindow->setText(buf);
    PurgeComm(COM2, PURGE_RXCLEAR); //PURGE_RXCLEAR — очищает очередь приема в драйвере.
    PurgeComm(COM2, PURGE_TXCLEAR);
}

void MainWindow::closeSerialPort() {
     CloseHandle(COM1);
     CloseHandle(COM2);
}

void MainWindow::outputConcditions()
{

}


