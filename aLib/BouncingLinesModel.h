#pragma once

#include <vector>

namespace aLib {

/// Plain-C++ 2-D vector, no Qt dependency.
struct Vec2 {
    double x{0.0};
    double y{0.0};

    Vec2& operator+=(const Vec2& o) { x += o.x; y += o.y; return *this; }
    Vec2& operator-=(const Vec2& o) { x -= o.x; y -= o.y; return *this; }
    Vec2  operator+ (const Vec2& o) const { return {x + o.x, y + o.y}; }
    Vec2  operator- (const Vec2& o) const { return {x - o.x, y - o.y}; }
    Vec2  operator* (double s)      const { return {x * s,   y * s};   }
};

inline double dot(const Vec2& a, const Vec2& b) { return a.x*b.x + a.y*b.y; }
inline double lengthSq(const Vec2& v)            { return dot(v, v); }

/// Data for a single animated line: two endpoints with independent velocities
/// and an HSV hue value (0–359) used by the view layer to pick a colour.
struct LineData {
    Vec2 p1;
    Vec2 p2;
    Vec2 v1;
    Vec2 v2;
    int  hue{0};   ///< 0–359
};

/**
 * BouncingLinesModel
 *
 * Owns and updates the simulation state for a set of lines that bounce inside
 * a rectangular boundary.  Has no dependency on Qt or any GUI framework.
 *
 * Typical usage:
 *   model.init(viewWidth, viewHeight);   // call once (or on resize)
 *   model.update(viewWidth, viewHeight); // call each frame
 *   for (const auto& ln : model.lines())  {// render // }
 */
class BouncingLinesModel
{
public:
    static constexpr int   LINE_COUNT = 12;
    static constexpr double MAX_SPEED  = 10.0;

    BouncingLinesModel(); 

    /// set the number of lines to simulate (default is LINE_COUNT) and reserve memory for them.
    void setNumLines(int numLines);

    /// (Re)seed all lines with random positions and velocities inside [0,w]×[0,h].
    void init(double width, double height);

    /// Advance the simulation by one frame inside the given bounds.
    void update(double width, double height);

    const std::vector<LineData>& lines() const { return m_lines; }

    /// Set all endpoint velocities to zero (animation tick continues).
    void zeroVelocity();

private:
    static double randomSpeed();
    static void   bouncePoint(Vec2& pos, Vec2& vel, double width, double height);

    std::vector<LineData> m_lines;
};

} // namespace aLib
