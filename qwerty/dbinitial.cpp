#include "dbinitial.h"
#include <QSqlQuery>
#include <QSqlError>
#include <QDebug>

bool DBInitial::initializeDatabase() {
    QSqlDatabase db = QSqlDatabase::addDatabase("QSQLITE");
    db.setDatabaseName("qwerty.db");
    if (!db.open()) {
        qDebug() << "Failed to open database";
        return false;
    }

    if (!db.tables().contains("user", Qt::CaseInsensitive)) {
        QSqlQuery query;

        if (!query.exec("CREATE TABLE user ("
                        "username TEXT PRIMARY KEY,"
                        "email TEXT UNIQUE,"
                        "passwd TEXT,"
                        "ip TEXT);")) {
            qDebug() << "Error creating table: " << query.lastError();
            return false;
        }

        qDebug() << "Table created successfully";

//        if (!query.exec("INSERT INTO user (username, email, passwd, ip) "
//                        "VALUES ('john', 'john@example.com', 'password123', '192.168.1.1');")) {
//            qDebug() << "Error inserting data: " << query.lastError();
//            return false;
//        }
//        qDebug() << "Data inserted successfully";

    } else {
        qDebug() << "Table already exists, skipping creation";
    }

    return true;
}
