#include "signupcheck.h"
#include <QSqlQuery>
#include <QSqlError>
#include <QDebug>
#include <QRegularExpression>

bool SignupCheck::isUsernameValid(const QString &username) {
    if (username.isEmpty()) {
            return false;
    }
    QRegularExpression specialCharsRegex("[!@#$%^&*(),.?\":{}|<>]+");
    return !username.contains(specialCharsRegex);
}

bool SignupCheck::isEmailValid(const QString &email) {
    QRegularExpression emailRegex("\\b[A-Za-z0-9._%+-]+@[A-Za-z0-9.-]+\\.[A-Z|a-z]{2,4}\\b");
    return email.contains(emailRegex);
}

bool SignupCheck::arePasswordsEqual(const QString &password, const QString &repeatPassword) {
    return !password.isEmpty() && !repeatPassword.isEmpty() && (password == repeatPassword);
}

QString SignupCheck::getLocalIP() {
    QString localHostName = QHostInfo::localHostName();
    QHostInfo hostInfo = QHostInfo::fromName(localHostName);
    foreach (const QHostAddress &address, hostInfo.addresses()) {
        if (address.protocol() == QAbstractSocket::IPv4Protocol) {
            return address.toString();
        }
    }
    return QString();
}
