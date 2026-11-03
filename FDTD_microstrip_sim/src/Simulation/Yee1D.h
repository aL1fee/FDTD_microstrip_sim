#pragma once

#include <cstddef>
#include <vector>

class Yee1D
{
public:
    static constexpr float courant = 0.5f;
    static constexpr double vacuumSpeed = 299792458.0; // m/s

    static constexpr double timestep(double spacingMeters)
    {
        return courant * spacingMeters / vacuumSpeed;
    }

    void resetFields(std::size_t count)
    {
        electric.assign(count, 0.0f);
        magnetic.assign(count, 0.0f);
    }

    template<class Excite>
    void step(Excite excite)
    {
        for (std::size_t i = 1; i < electric.size(); ++i)
            electric[i] += courant * (magnetic[i - 1] - magnetic[i]);

        // Sources sample the updated E field before H advances.
        excite(*this);

        for (std::size_t i = 1; i < electric.size(); ++i)
            magnetic[i - 1] += courant * (electric[i - 1] - electric[i]);
    }

    const std::vector<float>& e() const { return electric; }
    const std::vector<float>& h() const { return magnetic; }

    void inject(std::size_t index, float amplitude)
    {
        electric.at(index) += amplitude;
    }

    void applyLegacyRightBoundary()
    {
        if (electric.size() < 2)
            return;
        electric.back() = rightOlder;
        rightOlder = rightPrevious;
        rightPrevious = electric[electric.size() - 2];
    }

private:
    std::vector<float> electric;
    std::vector<float> magnetic;
    // The legacy GUI reset clears fields but retains the boundary history.
    float rightOlder = 0.0f;
    float rightPrevious = 0.0f;
};
