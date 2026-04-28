#include "BouncingLinesModel.h"

#include <cmath>
#include <cstdlib>
#include <ctime>
#include <random>

namespace aLib {

// ── static helpers ────────────────────────────────────────────────────────────

static std::mt19937& rng()
{
    static std::mt19937 gen(static_cast<unsigned>(std::time(nullptr)));
    return gen;
}

static double randomDouble(double lo, double hi)
{
    std::uniform_real_distribution<double> dist(lo, hi);
    return dist(rng());
}

static int randomInt(int lo, int hi)  // inclusive
{
    std::uniform_int_distribution<int> dist(lo, hi);
    return dist(rng());
}

double BouncingLinesModel::randomSpeed()
{
    double v = randomDouble(-MAX_SPEED, MAX_SPEED);
    // Ensure a minimum speed so lines never stall
    if (std::fabs(v) < 0.5) v = (v >= 0.0 ? 0.5 : -0.5);
    return v;
}

void BouncingLinesModel::bouncePoint(Vec2& pos, Vec2& vel,
                                     double width, double height)
{
    if (pos.x < 0.0)    { pos.x = 0.0;    vel.x = -vel.x; }
    if (pos.x > width)  { pos.x = width;  vel.x = -vel.x; }
    if (pos.y < 0.0)    { pos.y = 0.0;    vel.y = -vel.y; }
    if (pos.y > height) { pos.y = height; vel.y = -vel.y; }
}

// ── BouncingLinesModel ────────────────────────────────────────────────────────

BouncingLinesModel::BouncingLinesModel()
{
    m_lines.reserve(LINE_COUNT);
}

void BouncingLinesModel::setNumLines(int numLines)
{
    m_numLines = numLines;
    m_lines.reserve(static_cast<size_t>(numLines));
}

void BouncingLinesModel::init(double width, double height)
{
    if (width <= 0.0 || height <= 0.0) return;  // never wipe valid state with bad dimensions
    m_lines.clear();

    for (int i = 0; i < m_numLines; ++i) {
        LineData ln;
        ln.p1 = { randomDouble(0.0, width),  randomDouble(0.0, height) };
        ln.p2 = { randomDouble(0.0, width),  randomDouble(0.0, height) };
        ln.v1 = { randomSpeed(), randomSpeed() };
        ln.v2 = { randomSpeed(), randomSpeed() };
        ln.hue = randomInt(0, 359);
        m_lines.push_back(ln);
    }
}

void BouncingLinesModel::update(double width, double height)
{
    for (LineData& ln : m_lines) {
        ln.p1 += ln.v1;
        ln.p2 += ln.v2;
        bouncePoint(ln.p1, ln.v1, width, height);
        bouncePoint(ln.p2, ln.v2, width, height);
    }
}

void BouncingLinesModel::zeroVelocity()
{
    for (LineData& ln : m_lines) {
        ln.v1 = {0.0, 0.0};
        ln.v2 = {0.0, 0.0};
    }
}

} // namespace aLib
