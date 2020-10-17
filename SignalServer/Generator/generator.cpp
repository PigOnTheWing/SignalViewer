#include "generator.h"

Generator::Generator(qreal amplitude, qreal period): amplitude(amplitude), period(period)
{

}

qreal Generator::getAmplitude()
{
    return amplitude;
}

void Generator::setAmplitude(qreal amp)
{
    amplitude = amp;
}

qreal Generator::getPeriod()
{
    return period;
}

void Generator::setPeriod(qreal p)
{
    period = p;
}

qreal Generator::next()
{
    qreal res = amplitude * qSin((FULL_CIRCLE / period) * x);

    x += M_PI / PRECISION;

    if (x > M_PI) {
        x -= FULL_CIRCLE;
    }

    return res;
}
