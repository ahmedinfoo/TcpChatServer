#ifndef DIALOG_H
#define DIALOG_H

#include <QDialog>
#include "server.h"

QT_BEGIN_NAMESPACE
namespace Ui { class Dialog; }
QT_END_NAMESPACE

class Dialog : public QDialog
{
    Q_OBJECT

public:
    Dialog(QWidget *parent = nullptr);
    ~Dialog();



private slots:
    void on_startButton_clicked();
    void on_stoptButton_clicked();

    void next();
    void back();
    void changed();

private:
    Ui::Dialog *ui;
    QPushButton *nextButton;
    QPushButton *backButton;
    Server server;
    void init();
    void enebleButtons();

};
#endif // DIALOG_H
