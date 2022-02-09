/*
 * harbour-watchlist - Sailfish OS Version
 * Copyright © 2022 Andreas Wüst (andreas.wuest.freelancer@gmail.com)
 *
 * This program is free software: you can redistribute it and/or modify
 * it under the terms of the GNU General Public License as published by
 * the Free Software Foundation, either version 3 of the License, or
 * (at your option) any later version.
 *
 * This program is distributed in the hope that it will be useful,
 * but WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
 * GNU General Public License for more details.
 *
 * You should have received a copy of the GNU General Public License
 * along with this program. If not, see <http://www.gnu.org/licenses/>.
 */
#include "divvydiary.h"
#include "../constants.h"

#include <QDebug>
#include <QJsonArray>
#include <QJsonDocument>
#include <QJsonObject>
#include <QUrl>

DivvyDiary::DivvyDiary(QNetworkAccessManager *manager, QObject *parent)
    : QObject(parent) {
    qDebug() << "Initializing DivvyDiary ...";
    this->manager = manager;

    connect(&dividendDataUpdateWorker, SIGNAL(updateCompleted(int)), this, SLOT(handleDividendDataUpdateCompleted(int)));
}

DivvyDiary::~DivvyDiary() {
    qDebug() << "Shutting down DivvyDiary ...";
}

void DivvyDiary::handleDividendDataUpdateCompleted(int rows) {
    qDebug() << "DivvyDiary::handleDividendDataUpdateCompleted - rows : " << rows;
    emit fetchDividendDatesResultAvailable(rows);
}

void DivvyDiary::fetchDividendDates() {
    QNetworkReply *reply = executeGetRequest(QUrl(QString(DIVVYDIARY_DIVIDENDS)));

    connect(reply,
            SIGNAL(error(QNetworkReply::NetworkError)),
            this,
            SLOT(handleRequestError(QNetworkReply::NetworkError)));
    connect(reply, SIGNAL(finished()), this, SLOT(handleFetchDividendDates()));
}

QNetworkReply *DivvyDiary::executeGetRequest(const QUrl &url) {
    qDebug() << "DivvyDiary::executeGetRequest " << url;
    QNetworkRequest request(url);
    request.setHeader(QNetworkRequest::UserAgentHeader, USER_AGENT);

    return manager->get(request);
}

void DivvyDiary::handleFetchDividendDates() {
    qDebug() << "DivvyDiary::handleFetchDividendDates";
    QNetworkReply *reply = qobject_cast<QNetworkReply *>(sender());
    reply->deleteLater();
    if (reply->error() != QNetworkReply::NoError) {
        return;
    }

    while (this->dividendDataUpdateWorker.isRunning()) {
        this->dividendDataUpdateWorker.requestInterruption();
    }
    this->dividendDataUpdateWorker.setParameters(QJsonDocument::fromJson(reply->readAll()));
    this->dividendDataUpdateWorker.start();
}

void DivvyDiary::handleRequestError(QNetworkReply::NetworkError error) {
    QNetworkReply *reply = qobject_cast<QNetworkReply *>(sender());
    qWarning() << "DivvyDiary::handleRequestError:" << static_cast<int>(error) << reply->errorString()
               << reply->readAll();

    emit requestError("Return code: " + QString::number(static_cast<int>(error)) + " - " + reply->errorString());
}
