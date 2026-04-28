#pragma once

#include <QtWidgets/QWidget>
#include <QtCore/QTimer>
#include <QtCore/QPointF>
#include <QtCore/QVector>
#include "BouncingBallsModel.h"

class BouncingBallsWidget : public QWidget
{
    Q_OBJECT

public:
    explicit BouncingBallsWidget(QWidget *parent = nullptr);
    ~BouncingBallsWidget() override = default;

    void start() { m_model.unfreeze(); m_timer.start(); }
    void stop()  { m_timer.stop();  }
    bool isRunning() const { return m_timer.isActive(); }
    void freeze()    { m_model.zeroVelocity(); }

    void setCount(int n)
    {
        m_model.setCount(n);
        if (width() > 0 && height() > 0)
            m_model.init(width(), height());
    }
    int  count() const { return m_model.count(); }

signals:
    /// Emitted every tick with the kinetic energy of each ball (same order as model::balls()).
    void kineticEnergiesUpdated(QVector<double> energies);

protected:
    bool hasHeightForWidth() const override { return true; }
    int  heightForWidth(int w) const override { return w; }
    void paintEvent(QPaintEvent *event) override;
    void resizeEvent(QResizeEvent *event) override;
    void mousePressEvent(QMouseEvent *event) override;
    void mouseMoveEvent(QMouseEvent *event) override;
    void mouseReleaseEvent(QMouseEvent *event) override;

private slots:
    void tick();

private:
    static constexpr int INTERVAL_MS = 16;  // ~60 fps

    QTimer                   m_timer;
    aLib::BouncingBallsModel m_model;

    int     m_grabbedIndex{-1};  ///< index of the currently dragged ball, or -1
    QPointF m_prevMousePos;      ///< mouse position one frame before release
    QPointF m_currMousePos;      ///< most recent mouse position during drag
};
