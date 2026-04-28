#pragma once

#include <QtWidgets/QWidget>
#include <QtCore/QTimer>
#include "BouncingLinesModel.h"

class BouncingLinesWidget : public QWidget
{
    Q_OBJECT

public:
    explicit BouncingLinesWidget(QWidget *parent = nullptr);
    ~BouncingLinesWidget() override = default;

    void start() { m_timer.start(); }
    void stop()  { m_timer.stop();  }
    bool isRunning() const { return m_timer.isActive(); }
    void freeze()    { m_model.zeroVelocity(); }

    void setCount(int n)
    {
        m_model.setNumLines(n);
        if (width() > 0 && height() > 0)
            m_model.init(width(), height());
    }
    int  count() const { return m_model.numLines(); }

protected:
    bool hasHeightForWidth() const override { return true; }
    int  heightForWidth(int w) const override { return w; }

    void paintEvent(QPaintEvent *event) override;
    void resizeEvent(QResizeEvent *event) override;

private slots:
    void tick();

private:
    static constexpr int INTERVAL_MS = 16;  // ~60 fps
    static constexpr int LINE_WIDTH  = 2;

    QTimer                  m_timer;
    aLib::BouncingLinesModel m_model;
};
