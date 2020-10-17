#ifndef GENERATORWORKER_H
#define GENERATORWORKER_H

#include <QObject>
#include <QTcpSocket>

class GeneratorWorker : public QObject
{
    Q_OBJECT
public:
    explicit GeneratorWorker(QTcpSocket *socket, QObject *parent = nullptr);

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

    void handleCommand();

};

#endif // GENERATORWORKER_H
