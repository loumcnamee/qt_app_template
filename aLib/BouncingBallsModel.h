#pragma once

#include "BouncingLinesModel.h"  // for Vec2
#include <vector>

namespace aLib {

/// Data for a single bouncing ball.
struct BallData {
    Vec2   pos;
    Vec2   vel;
    double radius{10.0};
    int    hue{0};    ///< 0–359
    bool   grabbed{false};
};

/**
 * BouncingBallsModel
 *
 * Owns and updates the simulation state for a set of balls that bounce inside
 * a rectangular boundary.  Has no dependency on Qt or any GUI framework.
 *
 * Typical usage:
 *   model.init(viewWidth, viewHeight);   // call once (or on resize)
 *   model.update(viewWidth, viewHeight); // call each frame
 *   for (const auto& b : model.balls()) { // render // }
 */
class BouncingBallsModel
{
public:
    static constexpr int    BALL_COUNT = 10;
    static constexpr double MAX_SPEED  = 5.0;
    static constexpr double MIN_RADIUS = 10.0;
    static constexpr double MAX_RADIUS = 30.0;

    BouncingBallsModel();

    /// (Re)seed all balls with random positions, velocities and radii inside [0,w]×[0,h].
    /// Only call this when starting fresh; use clampToBounds() on window resize.
    void init(double width, double height);

    /// Clamp all ball positions to stay inside the new bounds without changing
    /// velocities, frozen state, or any other model state.
    void clampToBounds(double width, double height);

    /// Advance the simulation by one frame inside the given bounds.
    void update(double width, double height);

    const std::vector<BallData>& balls() const { return m_balls; }

    /// Total kinetic energy: KE = Σ ½·m·v²  where mass m = π·r² (density = 1).
    double totalKineticEnergy() const;

    /// Per-ball kinetic energies in the same order as balls().
    std::vector<double> perBallKineticEnergies() const;

    /// Set all ball velocities to zero and pause physics (animation tick continues).
    void zeroVelocity();

    /// Returns true if the model is in frozen state (all velocities zeroed).
    bool isFrozen() const { return m_frozen; }

    /// Resume physics without reinitialising ball positions.
    void unfreeze() { m_frozen = false; }

    /// Returns the index of the topmost ball whose area contains (x,y), or -1.
    int  hitTest(double x, double y) const;

    /// Move ball at index to (x,y) and mark it as grabbed (physics suspended).
    void grabBall(int index, double x, double y);

    /// Release the grabbed ball and give it the supplied velocity.
    void releaseBall(int index, Vec2 velocity);

private:
    static double randomSpeed();
    static void   bouncePoint(Vec2& pos, Vec2& vel, double radius,
                              double width, double height);
    void          resolveCollisions();

    std::vector<BallData> m_balls;
    bool                  m_frozen{false};
};

} // namespace aLib
