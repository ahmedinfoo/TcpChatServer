#include "dialog.h"
#include "./ui_dialog.h"
#include <QMessageBox>

Dialog::Dialog(QWidget *parent)
    : QDialog(parent)
    , ui(new Ui::Dialog)
{
    ui->setupUi(this);
    init();
}

Dialog::~Dialog()
{
    delete ui;
}


void Dialog::on_startButton_clicked()
{
    quint16 port = static_cast<quint16>(ui->spinBox->value());

    server.setMessage(ui->textMessage->text() + "\r\n");
    if(!server.listen(QHostAddress::Any, port))
    {
        QMessageBox::critical(this,"Error",server.errorString());
        return;
    }

    ui->lblStatus->setText("Listening...");
    enebleButtons();
}


void Dialog::on_stoptButton_clicked()
{
    server.close();
    ui->lblStatus->setText("Closed");
    enebleButtons();
}

void Dialog::next()
{
    int i = ui->stackedWidget->currentIndex();
    i++;
    if(i >= ui->stackedWidget->count()) i = ui->stackedWidget->count() - 1;
    ui->stackedWidget->setCurrentIndex(i);
    enebleButtons();}

void Dialog::back()
{
    int i = ui->stackedWidget->currentIndex();
        i--;
        if(i < 0) i = 0;
        ui->stackedWidget->setCurrentIndex(i);
        enebleButtons();
}

void Dialog::changed()
{
    ui->lblStatus->setText(QString("there are %0 clients connected ").arg(server.count()));
}

void Dialog::init()
{
    nextButton=new QPushButton("Next",this);
    backButton=new QPushButton("Back",this);
    ui->buttonBox->addButton(nextButton,QDialogButtonBox::ActionRole);
    ui->buttonBox->addButton(backButton,QDialogButtonBox::ActionRole);
    connect(nextButton,&QPushButton::clicked,this,&Dialog::next);
    connect(backButton,&QPushButton::clicked,this,&Dialog::back);
    connect(&server,&Server::changed,this,&Dialog::changed);
}

void Dialog::enebleButtons()
{
    ui->startButton->setEnabled(server.isListening());
    ui->stoptButton->setEnabled(server.isListening());
    //only one page
    if(ui->stackedWidget->count()<=1){
        nextButton->setEnabled(false);
        backButton->setEnabled(false);
        return;
    }
    //first page
    if(ui->stackedWidget->count()==0)
    {
        nextButton->setEnabled(true);
        backButton->setEnabled(false);
        return;
    }
    //last page
    if(ui->stackedWidget->currentIndex()==ui->stackedWidget->count()-1)
    {
        nextButton->setEnabled(false);
        backButton->setEnabled(true);
        return;
    }
}

