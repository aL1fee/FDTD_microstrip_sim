#pragma once
#include <string>

// Presentation boundary: legacy scene coordinates remain millimeters, Yee uses SI.
namespace LengthUnits
{
    enum class Unit { Mil, Millimeter, Micrometer };
    constexpr double scale(Unit unit) { return unit == Unit::Mil ? 1.0 / 0.0254 : unit == Unit::Millimeter ? 1.0 : 1000.0; }
    inline const char* symbol(Unit unit) { return unit == Unit::Mil ? "mil" : unit == Unit::Millimeter ? "mm" : "um"; }
    constexpr double fromScene(double value, Unit unit) { return value * scale(unit); }
    constexpr double toScene(double value, Unit unit) { return value / scale(unit); }
    constexpr double fromMeters(double value, Unit unit) { return fromScene(value * 1000.0, unit); }
    constexpr double sceneToMicrometers(double value) { return value * 1000.0; }
    constexpr double micrometersToScene(double value) { return value / 1000.0; }
    constexpr double metersToMicrometers(double value) { return value * 1e6; }
    inline bool isSceneLength(const std::string& name)
    {
        return name.size() >= 5 && name.compare(name.size()-5, 5, " (mm)") == 0;
    }
    inline std::string displayName(const std::string& name, Unit unit = Unit::Micrometer)
    {
        return isSceneLength(name) ? name.substr(0, name.size()-5) + " (" + symbol(unit) + ")" : name;
    }
    inline std::string propertyName(const std::string& name)
    {
        return name.substr(0, name.find(" ("));
    }
}
