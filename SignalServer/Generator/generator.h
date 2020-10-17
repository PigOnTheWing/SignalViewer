#ifndef GENERATOR_H
#define GENERATOR_H

#include <QtMath>

#define FULL_CIRCLE (2.0 * M_PI)
#define PRECISION 1000.0

class Generator
{
public:
    Generator(qreal amplitude = 1.0, qreal period = 1.0);
    qreal next();

    void setAmplitude(qreal amp);
    qreal getAmplitude();

    void setPeriod(qreal period);
    qreal getPeriod();

private:
    qreal x = 0.0;
    qreal amplitude;
    qreal period;

};

#endif // GENERATOR_H
