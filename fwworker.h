#ifndef FWWORKER_H
#define FWWORKER_H

#include <QObject>
#include <QDebug>
class fwWorker : public QObject
{
    Q_OBJECT
public:
    explicit fwWorker(QObject *parent = nullptr);
public slots:
    void run();

signals:

};

#endif // FWWORKER_H
