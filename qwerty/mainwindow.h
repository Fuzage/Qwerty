#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QMainWindow>
#include <QMouseEvent>


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
    void on_button_login_clicked();

    void on_command_signup_clicked();

    void on_lineEdit_passwd_returnPressed();

private:
    Ui::MainWindow *ui;

protected:
    void mouseMoveEvent(QMouseEvent *event);
    void mousePressEvent(QMouseEvent *event);

void mouseReleaseEvent(QMouseEvent *event);

private:
    QPoint mousePoint;
    bool mouse_press;
};
#endif // MAINWINDOW_H
