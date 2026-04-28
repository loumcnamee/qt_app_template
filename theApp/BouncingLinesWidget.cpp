#include "BouncingLinesWidget.h"

#include <QtGui/QPainter>
#include <QtGui/QPen>
#include <QtGui/QColor>

BouncingLinesWidget::BouncingLinesWidget(QWidget *parent)
    : QWidget(parent)
{
    setMinimumSize(200, 200);
    setAutoFillBackground(true);
    QPalette pal = palette();
    pal.setColor(QPalette::Window, Qt::black);
    setPalette(pal);

    connect(&m_timer, &QTimer::timeout, this, &BouncingLinesWidget::tick);
    m_timer.setInterval(INTERVAL_MS);
    m_timer.start();
}

// ── Qt event overrides ───────────────────────────────────────────────────────

void BouncingLinesWidget::resizeEvent(QResizeEvent *event)
{
    QWidget::resizeEvent(event);
    m_model.init(width(), height());
}

void BouncingLinesWidget::paintEvent(QPaintEvent * /*event*/)
{
    QPainter painter(this);
    painter.setRenderHint(QPainter::Antialiasing);

    for (const aLib::LineData &ln : m_model.lines()) {
        QColor color = QColor::fromHsv(ln.hue, 230, 255);
        painter.setPen(QPen(color, LINE_WIDTH));
        painter.drawLine(QPointF(ln.p1.x, ln.p1.y),
                         QPointF(ln.p2.x, ln.p2.y));
    }
}

// ── animation slot ───────────────────────────────────────────────────────────

void BouncingLinesWidget::tick()
{
    if (width() > 0 && height() > 0)
        m_model.update(width(), height());
    update();
}
