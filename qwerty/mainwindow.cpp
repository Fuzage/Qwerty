#include "mainwindow.h"
#include "ui_mainwindow.h"
#include <QGraphicsDropShadowEffect>
#include<QMessageBox>
#include "signup.h"
#include <qdesktopwidget.h>
#include "dbinitial.h"
#include "signupcheck.h"
#include <QSqlQuery>

MainWindow::MainWindow(QWidget *parent)
    : QMainWindow(parent)
    , ui(new Ui::MainWindow)
{
    ui->setupUi(this);
    //设置图片
        QPixmap *pix = new QPixmap(":/cover.jpg");
        QSize sz = ui->label_image->size();
        ui->label_image->setPixmap(pix->scaled(sz));

        //设置图片阴影效果
        QGraphicsDropShadowEffect *shadow = new QGraphicsDropShadowEffect(this);
        shadow->setOffset(-3, 0);
        shadow->setColor(QColor("#888888"));
        shadow->setBlurRadius(30);
        ui->label_image->setGraphicsEffect(shadow);

        //设置无边框
        this->setWindowFlags(Qt::FramelessWindowHint);
}

MainWindow::~MainWindow()
{
    delete ui;
}


//void MainWindow::on_button_login_clicked()
//{
//    accept();
//}

//登陆处理
void MainWindow::on_button_login_clicked()
{
    DBInitial *I = new DBInitial;
    if (!I->initializeDatabase()) {
        QMessageBox::information(this, "错误", "数据库初始化失败");
    } else {
        QString loginInput = ui->lineEdit_user->text();
        QString password = ui->lineEdit_passwd->text();
        SignupCheck signupChecker;

        // Determine whether loginInput is a username or an email
        QString columnToQuery = (loginInput.contains('@')) ? "email" : "username";

        QSqlQuery query;
        QString qstr = QString("SELECT * FROM user WHERE %1 = '%2' AND passwd = '%3';").arg(columnToQuery, loginInput, password);

        if (!query.exec(qstr)) {
            QMessageBox::critical(this, "错误", "用户登录失败");
        } else {
            if (query.next()) {
                QMessageBox::information(this, "成功", "用户登录成功");
            } else {
                QMessageBox::critical(this, "错误", "用户名/邮箱或密码不匹配");
            }
        }
    }
    delete I;
}

void MainWindow::on_lineEdit_passwd_returnPressed()
{
    DBInitial *I = new DBInitial;
    if (!I->initializeDatabase()) {
        QMessageBox::information(this, "错误", "数据库初始化失败");
    } else {
        QString loginInput = ui->lineEdit_user->text();
        QString password = ui->lineEdit_passwd->text();
        SignupCheck signupChecker;

        // Determine whether loginInput is a username or an email
        QString columnToQuery = (loginInput.contains('@')) ? "email" : "username";

        QSqlQuery query;
        QString qstr = QString("SELECT * FROM user WHERE %1 = '%2' AND passwd = '%3';").arg(columnToQuery, loginInput, password);

        if (!query.exec(qstr)) {
            QMessageBox::critical(this, "错误", "用户登录失败");
        } else {
            if (query.next()) {
                QMessageBox::information(this, "成功", "用户登录成功");
            } else {
                QMessageBox::critical(this, "错误", "用户名/邮箱或密码不匹配");
            }
        }
    }
    delete I;
}

//退出处理
void MainWindow::mousePressEvent(QMouseEvent *event)
{

    if( (event->button() == Qt::LeftButton) ){
        mouse_press = true;
        mousePoint = event->globalPos() - this->pos();
//        event->accept();
    }
    else if(event->button() == Qt::RightButton){
         //如果是右键
         this->close();

     }
}

//拖拽处理
 void MainWindow::mouseMoveEvent(QMouseEvent *event)
 {


 //    if(event->buttons() == Qt::LeftButton){  //如果这里写这行代码，拖动会有点问题
     if(mouse_press){
         move(event->globalPos() - mousePoint);
 //        event->accept();
     }
 }
 void MainWindow::mouseReleaseEvent(QMouseEvent *event)
 {
     mouse_press = false;
 }

 //注册处理
void MainWindow::on_command_signup_clicked()
{
    this->close();
    Signup *S = new Signup;
    QDesktopWidget *desktop = QApplication::desktop();
    S->move((desktop->width() - S->width())/ 2, (desktop->height() - S->height()) /2);
    S->show();
}
