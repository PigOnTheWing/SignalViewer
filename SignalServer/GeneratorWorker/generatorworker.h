#ifndef GENERATORWORKER_H
#define GENERATORWORKER_H

#include <QObject>
#include <QTcpSocket>
#include <QDataStream>
#include "generator.h"

class GeneratorWorker : public QObject
{
    Q_OBJECT
public:
    explicit GeneratorWorker(qintptr descriptor, QObject *parent = nullptr);

signals:
    void generateNew();
    void userDisconnected();

public slots:
    void notifyClient();

private slots:
    void generateNext();

private:
    bool isGenerating = false;
    QTcpSocket *socket = nullptr;
    Generator generator;

    void handleCommand();

};

#endif // GENERATORWORKER_H
