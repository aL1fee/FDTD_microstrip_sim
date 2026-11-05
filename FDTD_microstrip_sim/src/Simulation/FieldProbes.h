#pragma once
#include "Yee3D.h"

// Bounded, one-sample-per-accepted-step history; independent of GUI frame rate.
struct FieldProbe
{
    static constexpr std::size_t capacity = 2048;
    Yee3D::Index index{};
    Yee3D::Axis component = Yee3D::Z;
    std::array<float,capacity> values{};
    std::size_t count=0, next=0;
    int lastStep=0;
    void clear() { count=next=0; lastStep=0; }
    void record(int step,float value)
    {
        values[next]=value; next=(next+1)%capacity;
        count=std::min(count+1,capacity); lastStep=step;
    }
    std::size_t offset() const { return count==capacity ? next : 0; }
    float at(std::size_t i) const
    {
        if(i>=count) throw std::out_of_range("Probe history index");
        return values[(offset()+i)%capacity];
    }
    int firstStep() const { return count ? lastStep-static_cast<int>(count)+1 : lastStep; }
};
