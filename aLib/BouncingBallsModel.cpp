#include "BouncingBallsModel.h"

#include <cmath>
#include <ctime>
#include <random>

namespace aLib {

// ── static helpers ────────────────────────────────────────────────────────────

static std::mt19937& ballRng()
{
    // Separate seed from the lines RNG so they don't share state.
    static std::mt19937 gen(static_cast<unsigned>(std::time(nullptr)) ^ 0xDEADBEEFu);
    return gen;
}

static double ballRandomDouble(double lo, double hi)
{
    std::uniform_real_distribution<double> dist(lo, hi);
    return dist(ballRng());
}

static int ballRandomInt(int lo, int hi)
{
    std::uniform_int_distribution<int> dist(lo, hi);
    return dist(ballRng());
}

double BouncingBallsModel::randomSpeed()
{
    double v = ballRandomDouble(-MAX_SPEED, MAX_SPEED);
    if (std::fabs(v) < 0.5) v = (v >= 0.0 ? 0.5 : -0.5);
    return v;
}

void BouncingBallsModel::bouncePoint(Vec2& pos, Vec2& vel, double r,
                                     double width, double height)
{
    if (pos.x - r < 0.0)     { pos.x = r;           vel.x = -vel.x; }
    if (pos.x + r > width)   { pos.x = width - r;   vel.x = -vel.x; }
    if (pos.y - r < 0.0)     { pos.y = r;           vel.y = -vel.y; }
    if (pos.y + r > height)  { pos.y = height - r;  vel.y = -vel.y; }
}

// ── BouncingBallsModel ────────────────────────────────────────────────────────

BouncingBallsModel::BouncingBallsModel()
{
    m_balls.reserve(BALL_COUNT);
}

void BouncingBallsModel::init(double width, double height)
{
    if (width <= 0.0 || height <= 0.0) return;  // never wipe valid state with bad dimensions
    m_frozen = false;
    m_balls.clear();

    for (int i = 0; i < m_count; ++i) {
        BallData b;
        b.radius = ballRandomDouble(MIN_RADIUS, MAX_RADIUS);
        b.pos    = { ballRandomDouble(b.radius, width  - b.radius),
                     ballRandomDouble(b.radius, height - b.radius) };
        b.vel    = { randomSpeed(), randomSpeed() };
        b.hue    = ballRandomInt(0, 359);
        m_balls.push_back(b);
    }
}

void BouncingBallsModel::clampToBounds(double width, double height)
{
    if (width <= 0.0 || height <= 0.0) return;
    for (BallData& b : m_balls) {
        const double r = b.radius;
        if (b.pos.x - r < 0.0)    b.pos.x = r;
        if (b.pos.x + r > width)  b.pos.x = width  - r;
        if (b.pos.y - r < 0.0)    b.pos.y = r;
        if (b.pos.y + r > height) b.pos.y = height - r;
    }
}

void BouncingBallsModel::update(double width, double height)
{
    if (m_frozen) return;  // physics paused by zeroVelocity()
    for (BallData& b : m_balls) {
        if (b.grabbed) continue;
        b.pos += b.vel;
        bouncePoint(b.pos, b.vel, b.radius, width, height);
    }
    resolveCollisions();
}

void BouncingBallsModel::resolveCollisions()
{
    const int n = static_cast<int>(m_balls.size());
    for (int i = 0; i < n - 1; ++i) {
        for (int j = i + 1; j < n; ++j) {
            BallData& a = m_balls[i];
            BallData& b = m_balls[j];

            // Don't push grabbed balls around during collision resolution
            if (a.grabbed && b.grabbed) continue;

            Vec2 delta = a.pos - b.pos;
            const double distSq = lengthSq(delta);
            const double minDist = a.radius + b.radius;

            if (distSq >= minDist * minDist || distSq == 0.0)
                continue;  // no overlap

            const double dist = std::sqrt(distSq);
            // Collision normal (unit vector from b toward a)
            Vec2 normal = delta * (1.0 / dist);

            // Only act when the balls are actually approaching each other.
            // This prevents the separation step from nudging stationary
            // (e.g. frozen) balls that happen to be overlapping.
            const double relVelAlongNormal = dot(a.vel - b.vel, normal);
            if (relVelAlongNormal >= 0.0)
                continue;

            // Separate overlapping balls so they just touch.
            // A grabbed ball is treated as immovable (infinite mass).
            const double overlap = minDist - dist;
            const double ma = a.grabbed ? 0.0 : a.radius * a.radius;
            const double mb = b.grabbed ? 0.0 : b.radius * b.radius;
            const double totalMass = ma + mb;
            if (totalMass > 0.0) {
                a.pos += normal * (overlap * mb / totalMass);
                b.pos -= normal * (overlap * ma / totalMass);
            }

            // Apply elastic impulse.
            if (totalMass > 0.0) {
                const double impulse = 2.0 * ma * mb / totalMass * relVelAlongNormal;
                if (!a.grabbed) a.vel -= normal * (impulse / ma);
                if (!b.grabbed) b.vel += normal * (impulse / mb);
            }
        }
    }
}

double BouncingBallsModel::totalKineticEnergy() const
{
    constexpr double pi = 3.14159265358979323846;
    double ke = 0.0;
    for (const BallData& b : m_balls) {
        const double mass = pi * b.radius * b.radius;  // density = 1
        ke += 0.5 * mass * lengthSq(b.vel);
    }
    return ke;
}

std::vector<double> BouncingBallsModel::perBallKineticEnergies() const
{
    constexpr double pi = 3.14159265358979323846;
    std::vector<double> result;
    result.reserve(m_balls.size());
    for (const BallData& b : m_balls) {
        const double mass = pi * b.radius * b.radius;
        result.push_back(0.5 * mass * lengthSq(b.vel));
    }
    return result;
}

int BouncingBallsModel::hitTest(double x, double y) const
{
    // Iterate in reverse so the last-drawn (topmost) ball is picked first.
    for (int i = static_cast<int>(m_balls.size()) - 1; i >= 0; --i) {
        const BallData& b = m_balls[i];
        const double dx = x - b.pos.x;
        const double dy = y - b.pos.y;
        if (dx*dx + dy*dy <= b.radius * b.radius)
            return i;
    }
    return -1;
}

void BouncingBallsModel::grabBall(int index, double x, double y)
{
    if (index < 0 || index >= static_cast<int>(m_balls.size())) return;
    BallData& b = m_balls[index];
    b.grabbed = true;
    b.vel     = {0.0, 0.0};
    b.pos     = {x, y};
}

void BouncingBallsModel::releaseBall(int index, Vec2 velocity)
{
    if (index < 0 || index >= static_cast<int>(m_balls.size())) return;
    BallData& b = m_balls[index];
    b.grabbed = false;
    b.vel     = velocity;
    m_frozen  = false;  // dragging a ball unfreezes physics
}

void BouncingBallsModel::zeroVelocity()
{
    for (BallData& b : m_balls)
        b.vel = {0.0, 0.0};
    m_frozen = true;
    
}

} // namespace aLib
