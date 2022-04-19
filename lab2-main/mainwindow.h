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

#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QMainWindow>
#include <QSerialPort>
#include <QByteArray>
#include <QString>
#include <qtimer.h>

QT_BEGIN_NAMESPACE
namespace Ui { class MainWindow; }
QT_END_NAMESPACE

class MainWindow : public QMainWindow
{
    Q_OBJECT

public:
    MainWindow(QWidget *parent = nullptr);
    ~MainWindow();

private slots:
    void ReadSerial();
    void WriteSerial();

    void on_btnBlockTimer_clicked();

private:
    Ui::MainWindow *ui;
    QSerialPort *Arduino;
    QByteArray Data;
    QString Buffer;
    bool isFirstTry;
    bool arduino_is_accessible;
    QTimer *Timer;
    bool isTimerBlocked;
};
#endif // MAINWINDOW_H
