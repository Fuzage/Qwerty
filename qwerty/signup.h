#ifndef SIGNUP_H
#define SIGNUP_H

#include <QWidget>
#include <QMouseEvent>



namespace Ui {
class Signup;
}

class Signup : public QWidget
{
    Q_OBJECT

public:
    void updateCountdown();
    explicit Signup(QWidget *parent = nullptr);
    ~Signup();

private:
    Ui::Signup *ui;
    int countdownSeconds;
    QTimer* countdownTimer;
    int code;
    QString origin_email;


protected:
    void mouseMoveEvent(QMouseEvent *event);
    void mousePressEvent(QMouseEvent *event);

void mouseReleaseEvent(QMouseEvent *event);

private slots:
void on_button_signup_clicked();

void on_sendButton_clicked();

private:
    QPoint mousePoint;
    bool mouse_press;
};

#endif // SIGNUP_H
