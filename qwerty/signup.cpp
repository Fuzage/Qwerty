#include "signup.h"
#include "ui_signup.h"
#include <QGraphicsDropShadowEffect>
#include <mainwindow.h>
#include <QDesktopWidget>
#include "dbinitial.h"
#include <QMessageBox>
#include "signupcheck.h"
#include <QSqlQuery>
#include <QTimer>
#include <QTime>
#include "smtp.h"

void sqlite_Init(QSqlDatabase &db);

Signup::Signup(QWidget *parent) :
    QWidget(parent),
    ui(new Ui::Signup)
{
    ui->setupUi(this);
    //设置图片
        QPixmap *pix = new QPixmap(":/cover2.jpg");
        QSize sz = ui->label_image2->size();
        ui->label_image2->setPixmap(pix->scaled(sz));

        //设置图片阴影效果
        QGraphicsDropShadowEffect *shadow = new QGraphicsDropShadowEffect(this);
        shadow->setOffset(3, 0);
        shadow->setColor(QColor("#888888"));
        shadow->setBlurRadius(30);
        ui->label_image2->setGraphicsEffect(shadow);

        //设置无边框
        this->setWindowFlags(Qt::FramelessWindowHint);

        countdownTimer = new QTimer(this);
        connect(countdownTimer, &QTimer::timeout, this, &Signup::updateCountdown);
}

Signup::~Signup()
{
    delete ui;
}


void Signup::mousePressEvent(QMouseEvent *event)
{

    if( (event->button() == Qt::LeftButton) ){
        mouse_press = true;
        mousePoint = event->globalPos() - this->pos();
//        event->accept();
    }
    else if(event->button() == Qt::RightButton){
         //如果是右键
        this->close();
        MainWindow *w = new MainWindow;
        QDesktopWidget *desktop = QApplication::desktop();
        w->move((desktop->width() - w->width())/ 2, (desktop->height() - w->height()) /2);
        w->show();
     }
 }
 void Signup::mouseMoveEvent(QMouseEvent *event)
 {


 //    if(event->buttons() == Qt::LeftButton){  //如果这里写这行代码，拖动会有点问题
     if(mouse_press){
         move(event->globalPos() - mousePoint);
 //        event->accept();
     }
 }
 void Signup::mouseReleaseEvent(QMouseEvent *event)
 {
     mouse_press = false;
}

 //注册检测
 void Signup::on_button_signup_clicked()
  {
      DBInitial *I = new DBInitial;
      if (!I->initializeDatabase()) {
          QMessageBox::information(this, "错误", "数据库初始化失败");
      } else {
          QString username = ui->lineEdit_user2->text();
          QString email = ui->lineEdit_email2->text();
          QString password = ui->lineEdit_passwd2->text();
          QString repeatPassword = ui->lineEdit_passwd3->text();
          QString verification = ui->lineEdit_confirm->text();
          SignupCheck signupChecker;

          if (!signupChecker.isUsernameValid(username)) {
              QMessageBox::information(this, "错误", "无效的用户名");
          } else if (!signupChecker.isEmailValid(email)) {
              QMessageBox::information(this, "错误", "无效的邮箱格式");
          } else if (!signupChecker.arePasswordsEqual(password, repeatPassword)) {
              QMessageBox::information(this, "错误", "密码不一致");
          } else if (verification != QString::number(code)) {
              QMessageBox::information(this, "错误", "验证码错误");
          } else if (email != origin_email){
              QMessageBox::information(this, "错误", "邮箱已变更\n请重新验证");
          } else {
              // 检测通过，插入数据库
              QSqlQuery query;
              QString test1 = QString("SELECT username FROM user WHERE username = '%1';").arg(username);
              if (query.exec(test1)) {
                  if (query.next()) {
                      QMessageBox::information(this, "错误", "用户名已注册");
                  }
                  else {
                      QString test2 = QString("SELECT email FROM user WHERE email = '%1';").arg(email);
                      if (query.exec(test2)) {
                          if (query.next()) {
                              QMessageBox::information(this, "错误", "邮箱已注册");
                          }
                          else {
                              QString qstr = QString("INSERT INTO user (username, email, passwd, ip) VALUES ('%1', '%2', '%3', '%4');").arg(username,email,password,signupChecker.getLocalIP());

                              if (!query.exec(qstr)) {
                                  QMessageBox::critical(this, "错误", "用户注册失败");
                              } else {
                                  QMessageBox::information(this, "成功", "用户注册成功");
                                  this->close();
                                  MainWindow *w = new MainWindow;
                                  QDesktopWidget *desktop = QApplication::desktop();
                                  w->move((desktop->width() - w->width())/ 2, (desktop->height() - w->height()) /2);
                                  w->show();
                              }
                          }
                       }
                  }
              }
              else {
                  QMessageBox::information(this, "错误", "数据库错误");
              }
          }
      }
      delete I;

//         QString username = ui->lineEdit_user2->text();
//         QString email = ui->lineEdit_email2->text();
//         QString password = ui->lineEdit_passwd2->text();
//          if (username.isEmpty() || email.isEmpty() || password.isEmpty()) {
//             QMessageBox::warning(this, "Error", "Please fill in all fields.");
//             return;
//         }

//         QSqlDatabase db = QSqlDatabase::addDatabase("QSQLITE");
//         db.setDatabaseName("user.db");

//         if (!db.open()) {
//             QMessageBox::critical(this, "Error", "Database connection error.");
//             return;
//         }

//         QSqlQuery query;
//         query.prepare("INSERT INTO user (username, email, passwd) VALUES (:username, :email, :password)");
//         query.bindValue(":username", username);
//         query.bindValue(":email", email);
//         query.bindValue(":password", password);

//         if (!query.exec()) {
//             QMessageBox::critical(this, "Error", "Registration failed.");
//         } else {
//             QMessageBox::information(this, "Success", "Registration successful.");
//             // 清空输入框内容
//             ui->lineEdit_user2->clear();
//             ui->lineEdit_email2->clear();
//             ui->lineEdit_passwd2->clear();
//         }

//         db.close();
//     QSqlDatabase db = QSqlDatabase::addDatabase("QSQLITE");
//     db.setDatabaseName("qwerty.db");
//     db.open();
//     sqlite_Init(db);
//     QString username = ui->lineEdit_user2->text();
//     QString email = ui->lineEdit_email2->text();
//     QString password = ui->lineEdit_passwd2->text();
//     QString repeatPassword = ui->lineEdit_passwd3->text();
//     SignupCheck signupChecker;

//     if (!signupChecker.isUsernameValid(username)) {
//         QMessageBox::information(this, "错误", "无效的用户名");
//     } else if (!signupChecker.isEmailValid(email)) {
//         QMessageBox::information(this, "错误", "无效的邮箱格式");
//     } else if (!signupChecker.arePasswordsEqual(password, repeatPassword)) {
//         QMessageBox::information(this, "错误", "密码不一致");
//     } else {
//         // 检测通过，插入数据库
//         QSqlQuery query;
//         query = QSqlQuery(db);
//         query.prepare("INSERT INTO user (username, email, passwd, ip) VALUES (:username, :email, :passwd, :ip)");
//         query.bindValue(":username", username);
//         query.bindValue(":email", email);
//         query.bindValue(":passwd", password);
//         query.bindValue(":ip", signupChecker.getLocalIP());


//         if (!query.exec("INSERT INTO user (username, email, passwd, ip) "
//                          "VALUES ('john', 'john@example.com', 'password123', '192.168.1.1');")) {
//             QMessageBox::critical(this, "错误", "用户注册失败");
//         } else {
//             QMessageBox::information(this, "成功", "用户注册成功");
//         }
//     }


//皇太子
//     QSqlDatabase db = QSqlDatabase::addDatabase("QSQLITE");
//     db.setDatabaseName("qwerty.db");
//     sqlite_Init(db);
//     QSqlQuery query;
//     query = QSqlQuery(db);

//     SignupCheck signupChecker;

//     QString username = ui->lineEdit_user2->text();
//     QString email = ui->lineEdit_email2->text();
//     QString password = ui->lineEdit_passwd2->text();
//     QString repeatPassword = ui->lineEdit_passwd3->text();

//     if (!signupChecker.isUsernameValid(username)) {
//         QMessageBox::information(this, "错误", "无效的用户名");
//     } else if (!signupChecker.isEmailValid(email)) {
//         QMessageBox::information(this, "错误", "无效的邮箱格式");
//     } else if (!signupChecker.arePasswordsEqual(password, repeatPassword)) {
//         QMessageBox::information(this, "错误", "密码不一致");
//     } else {

//        QString qstr = QString("INSERT INTO user (username, email, passwd, ip) VALUES ('%1', '%2', '%3', '192.168.1.1');").arg(username,email,password);

//        if (!query.exec(qstr)) {
//            qDebug()<< "错误";
//        } else {
//            qDebug()<< "成功";
//        }

//     }
}


void Signup::on_sendButton_clicked()
{
    QString email = ui->lineEdit_email2->text();
    origin_email = email;
    SignupCheck signupChecker;
    if(signupChecker.isEmailValid(email)){
        ui->sendButton->setEnabled(false); // 禁用按钮
        countdownSeconds = 30; // 设置倒计时时间为30秒
        updateCountdown(); // 更新倒计时显示
        countdownTimer->start(1000); // 启动定时器，每1000毫秒（1秒）触发一次

        QTime time= QTime::currentTime();
        qsrand(time.msec()+time.second()*1000);
        code = 1000+qrand() % (9999-1000);    //产生1000~9999以内的随机数
        Smtp smtp("shoukaku03s@163.com","FFUSGWYLFZWYMCAI");//按照刚才的方法实例化一个smtp对象
        QByteArray recvaddr=email.toUtf8();//QString转QByteArray
        QString subject="Qwerty登录/注册验证码";
        QString content="欢迎注册Qwerty，本次操作的验证码为："+QString::number(code);
        smtp.send(recvaddr,subject,content);
    }
    else{
        QMessageBox::warning(NULL,"提示","邮箱格式不正确,请重新输入。");
    }
}

void Signup::updateCountdown() {
    if (countdownSeconds > 0) {
        ui->sendButton->setText(QString::number(countdownSeconds) + "s"); // 更新按钮文本
        countdownSeconds--;
    } else {
        ui->sendButton->setText("发送"); // 恢复按钮文本
        ui->sendButton->setEnabled(true); // 启用按钮
        countdownTimer->stop(); // 停止定时器
    }
}

