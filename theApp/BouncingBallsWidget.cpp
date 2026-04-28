#include "BouncingBallsWidget.h"

#include <QtGui/QPainter>
#include <QtGui/QBrush>
#include <QtGui/QColor>
#include <QtGui/QFont>
#include <QtGui/QMouseEvent>
#include <QtCore/QString>

BouncingBallsWidget::BouncingBallsWidget(QWidget *parent)
    : QWidget(parent)
{
    setMinimumSize(200, 200);
    setAutoFillBackground(true);
    QPalette pal = palette();
    pal.setColor(QPalette::Window, Qt::black);
    setPalette(pal);

    connect(&m_timer, &QTimer::timeout, this, &BouncingBallsWidget::tick);
    m_timer.setInterval(INTERVAL_MS);
    m_timer.start();

    // Seed the model with a sensible default so balls exist before the first
    // resizeEvent fires (which only happens when the widget becomes visible).
    m_model.init(600, 400);

    setMouseTracking(false);  // only track while button pressed — default
}

// ── Mouse interaction ─────────────────────────────────────────────────────────

void BouncingBallsWidget::mousePressEvent(QMouseEvent *event)
{
    if (event->button() != Qt::LeftButton) return;
    const QPointF p = event->position();
    m_grabbedIndex = m_model.hitTest(p.x(), p.y());
    if (m_grabbedIndex >= 0) {
        m_prevMousePos = p;
        m_currMousePos = p;
        m_model.grabBall(m_grabbedIndex, p.x(), p.y());
    }
}

void BouncingBallsWidget::mouseMoveEvent(QMouseEvent *event)
{
    if (m_grabbedIndex < 0) return;
    m_prevMousePos = m_currMousePos;
    m_currMousePos = event->position();
    m_model.grabBall(m_grabbedIndex, m_currMousePos.x(), m_currMousePos.y());
}

void BouncingBallsWidget::mouseReleaseEvent(QMouseEvent *event)
{
    if (event->button() != Qt::LeftButton || m_grabbedIndex < 0) return;

    // Velocity = displacement over the last frame (pixels/frame ≈ pixels/16ms)
    const QPointF delta = m_currMousePos - m_prevMousePos;
    aLib::Vec2 vel{delta.x(), delta.y()};
    m_model.releaseBall(m_grabbedIndex, vel);
    m_grabbedIndex = -1;
}

// ── Qt event overrides ───────────────────────────────────────────────────────

void BouncingBallsWidget::resizeEvent(QResizeEvent *event)
{
    QWidget::resizeEvent(event);
    if (width() <= 0 || height() <= 0) return;
    // Only do a full init when there are no balls yet (first valid resize).
    // All subsequent resizes just clamp positions to preserve model state
    // (velocities, frozen flag, etc.).
    if (m_model.balls().empty())
        m_model.init(width(), height());
    else
        m_model.clampToBounds(width(), height());
}

void BouncingBallsWidget::paintEvent(QPaintEvent * /*event*/)
{
    QPainter painter(this);
    painter.setRenderHint(QPainter::Antialiasing);
    painter.setPen(Qt::NoPen);

    for (const aLib::BallData &b : m_model.balls()) {
        QColor fill  = QColor::fromHsv(b.hue, 220, 255, 220);
        QColor edge  = QColor::fromHsv(b.hue, 255, 200);
        painter.setBrush(fill);
        painter.setPen(QPen(edge, 1.5));
        const double r = b.radius;
        painter.drawEllipse(QPointF(b.pos.x, b.pos.y), r, r);
    }

    // ── Kinetic energy overlay ────────────────────────────────────────────
    const double ke = m_model.totalKineticEnergy();
    const QString label = QString("KE: %1").arg(ke, 0, 'f', 1);

    QFont font = painter.font();
    font.setPointSize(11);
    font.setBold(true);
    painter.setFont(font);

    const QRect textRect(8, 8, 200, 26);
    // Shadow for readability on any background
    painter.setPen(Qt::black);
    painter.drawText(textRect.translated(1, 1), Qt::AlignLeft | Qt::AlignVCenter, label);
    painter.setPen(Qt::white);
    painter.drawText(textRect, Qt::AlignLeft | Qt::AlignVCenter, label);
}

// ── animation slot ───────────────────────────────────────────────────────────

void BouncingBallsWidget::tick()
{
    // Advance velocity history for drag so we sample the last timer interval
    if (m_grabbedIndex >= 0)
        m_prevMousePos = m_currMousePos;

    if (width() > 0 && height() > 0)
        m_model.update(width(), height());
    update();

    // Emit per-ball KE data for the sidebar table
    const auto& ke = m_model.perBallKineticEnergies();
    QVector<double> keVec(ke.begin(), ke.end());
    emit kineticEnergiesUpdated(keVec);
}
