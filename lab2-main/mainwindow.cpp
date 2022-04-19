/*
 * Автор: Скворцов Даниил
 * Дата первой версии: 14.03.22
 * Назначение 1-ой функции: программа получает от Ардуино случайные температуру и давление
 * Назначение 2-ой функции: Программа отправляет Ардуино введённое пользователем число, а в ответ получает, чётное ли оно
 * Соавтор: Денис Смирнов
 * Версия: 2.4.3
 *
 * Выражаем благодарность Максиму Широкову за предоставление референсов и консультации
*/

#include "mainwindow.h"
#include "ui_mainwindow.h"
#include <QSerialPortInfo>
#include <QDebug>
#include <QMessageBox>
#include <QTimer>
#include <dos.h>
#include <unistd.h>

MainWindow::MainWindow(QWidget *parent) : QMainWindow(parent), ui(new Ui::MainWindow)
{
    /* ----- Инициализация переменных ----- */
    ui->setupUi(this);
    ui->tempValue->setText("0");
    ui->pressValue->setText("0");

    Arduino = new QSerialPort(this);
    Buffer = "";
    isFirstTry = true;
    arduino_is_accessible = false;
    isTimerBlocked = false;

    bool arduino_is_available = false;
    QString arduino_uno_port_name;

    /* ----- Определение идентификатора порта ----- */
    foreach(const QSerialPortInfo &serialPortInfo, QSerialPortInfo::availablePorts())
        if(serialPortInfo.hasProductIdentifier() && serialPortInfo.hasVendorIdentifier())
        {
            arduino_is_available = true;
            arduino_uno_port_name = serialPortInfo.portName();
        }


    /* ----- Настройка порта ----- */
    if(arduino_is_available)
    {
        qDebug() << "Найден порт Arduino: " + arduino_uno_port_name;

        Arduino->setPortName(arduino_uno_port_name);
        Arduino->open(QSerialPort::ReadWrite);
        Arduino->setBaudRate(QSerialPort::Baud9600);
        Arduino->setDataBits(QSerialPort::Data8);
        Arduino->setFlowControl(QSerialPort::NoFlowControl);
        Arduino->setParity(QSerialPort::NoParity);
        Arduino->setStopBits(QSerialPort::OneStop);
        Arduino->setDataTerminalReady(true);

        QObject::connect(Arduino, SIGNAL(readyRead()), this, SLOT(ReadSerial()));

        Timer = new QTimer(this);
        QObject::connect(Timer, SIGNAL(timeout()), this, SLOT(WriteSerial()));
        Timer->start(1000); // Задержка при получениие данных
    }
    else
    {
        qDebug() << "Не удалось найти порт Arduino.\n";
    }
}

MainWindow::~MainWindow()
{
    delete ui;
}

/* ----- Чтение с Serial ----- */
void MainWindow::ReadSerial()
{
    /* ----- Тест настроек порта ----- */
    if(isFirstTry)
    {
        Data = Arduino->readAll(); // TODO: проверить необходимость Data
        if(Data == "\xFF")
            qDebug() << "Настройки порта корректны";
        else
            qDebug() << "Некорректная настройка порта. Проверьте baudRate и dataBits";

        isFirstTry = false;
        Buffer = "";
        return;
    }

    /* ----- Основной алгоритм считывания температуры и давления ----- */
    arduino_is_accessible = true;

    Data = Arduino->readAll();
    Buffer += QString(Data);
    if(Buffer.endsWith("\n"))
    {
            QStringList buffer_split = Buffer.split(":"); // формат: "ттт:ддд", ттт - температура, ддд - давление

            ui->tempValue->setText(buffer_split[0]);
            ui->pressValue->setText(buffer_split[1].split("\n")[0]);

            Buffer = "";
    }
}

/* ----- Запись в Serial ----- */
void MainWindow::WriteSerial()
{
    Arduino->write("0"); // Символ "0" - запрос данных от Ардуино
}

/* ----- Запуск и остановка программы по клику ----- */
void MainWindow::on_btnBlockTimer_clicked()
{
    if(isTimerBlocked == false)
    {
        Timer->blockSignals(true);
        isTimerBlocked = true;
        ui->activateLabel->setText("Неактивно");
    } else {
        Timer->blockSignals(false);
        isTimerBlocked = false;
        ui->activateLabel->setText("Активно");
    }
}

