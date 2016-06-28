/*
 * Copyright (C) by Klaas Freitag <freitag@kde.org>
 * Copyright (C) by Olivier Goffart <ogoffart@woboq.com>
 *
 * This program is free software; you can redistribute it and/or modify
 * it under the terms of the GNU General Public License as published by
 * the Free Software Foundation; either version 2 of the License, or
 * (at your option) any later version.
 *
 * This program is distributed in the hope that it will be useful, but
 * WITHOUT ANY WARRANTY; without even the implied warranty of MERCHANTABILITY
 * or FITNESS FOR A PARTICULAR PURPOSE. See the GNU General Public License
 * for more details.
 */

#pragma once
#include "creds/httpcredentials.h"
#include <QPointer>

namespace OCC
{


class AssyncAuth : public QObject
{
    Q_OBJECT
public:
    AssyncAuth(Account *account, QObject *parent)
        : QObject(parent), _account(account)
    {}
    ~AssyncAuth();

    enum Result { Waiting, NotSupported, LoggedIn };
    void start();
public slots:
    void startFinished();
    void poll();
    void pollFinished();

signals:
    void result(AssyncAuth::Result r, const QString &token);
private:

    Account *_account;
    QUrl _pollUrl;
    // Makes sure the reply is destroyed when this object is closed
    QPointer<QNetworkReply> _reply;
//    State _state;
};



/**
 * @brief The HttpCredentialsGui class
 * @ingroup gui
 */
class HttpCredentialsGui : public HttpCredentials {
    Q_OBJECT
public:
    explicit HttpCredentialsGui() : HttpCredentials() {}
    HttpCredentialsGui(const QString& user, const QString& password, const QString& certificatePath, const QString& certificatePasswd) : HttpCredentials(user, password, certificatePath, certificatePasswd) {}
    void askFromUser() Q_DECL_OVERRIDE;

    static QString requestAppPasswordText(const Account *account);
private slots:
    void asyncAuthResult(AssyncAuth::Result, const QString &token);
    void showDialog();
private:
    QScopedPointer<AssyncAuth> _asyncAuth;
};

} // namespace OCC

