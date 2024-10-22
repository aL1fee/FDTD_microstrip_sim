#include "VAOVectorDS.h"

VAOVectorDS::VAOVectorDS()
{
    data = new std::vector<unsigned int>();
}

VAOVectorDS::~VAOVectorDS()
{
    delete data;
}

void VAOVectorDS::push(unsigned int ui)
{
    data->push_back(ui);
}

void VAOVectorDS::pop(int i)
{
    if (getSize() - 1 == i && getSize() > 0) {
        data->pop_back();
    }
}

void VAOVectorDS::clear()
{
    data->clear();
}

void VAOVectorDS::setAt(int i, unsigned int ui)
{
    data->at(i) = ui;
}

unsigned int VAOVectorDS::at(int i) const
{
    return data->at(i);
}

int VAOVectorDS::getSize() const
{
    return data->size();
}
