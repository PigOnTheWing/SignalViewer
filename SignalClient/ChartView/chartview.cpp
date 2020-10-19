#include "chartview.h"

ChartView::ChartView(QChart *chart, QWidget *parent) : QChartView(chart, parent)
{

}

void ChartView::wheelEvent(QWheelEvent *event)
{
    if (event->angleDelta().y() > 0) {
        chart()->zoomIn();
    }
    else {
        chart()->zoomOut();
    }

    event->accept();
}

void ChartView::keyPressEvent(QKeyEvent *event)
{
    switch (event->key()) {
    case Qt::Key_Plus: {
        chart()->zoomIn();
        break;
    }
    case Qt::Key_Minus: {
        chart()->zoomOut();
        break;
    }
    default: {
        QChartView::keyPressEvent(event);
        break;
    }
    }
}
