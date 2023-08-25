#ifndef SIGNUPCHECK_H
#define SIGNUPCHECK_H

#include <QString>
#include <QHostInfo>
#include <QSqlDatabase>

class SignupCheck {
public:
    bool isUsernameValid(const QString &username);
    bool isEmailValid(const QString &email);
    bool arePasswordsEqual(const QString &password, const QString &repeatPassword);
    static QString getLocalIP();

private:
    QSqlDatabase db;
    bool checkIfTableExists();
};

#endif // SIGNUPCHECK_H
