#include "VAOVectorDS.h"

VAOVectorDS::VAOVectorDS()
{
    data = new std::vector<unsigned int>();
    VBOs = new std::vector<unsigned int>();
    EBOs = new std::vector<unsigned int>();
}

VAOVectorDS::~VAOVectorDS()
{
    if (data->size() > 0)
    {
        glDeleteVertexArrays(static_cast<GLsizei>(data->size()), data->data());
    }
    if (VBOs->size() > 0)
    {
        glDeleteBuffers(static_cast<GLsizei>(VBOs->size()), VBOs->data());
    }
    if (EBOs->size() > 0)
    {
        glDeleteBuffers(static_cast<GLsizei>(EBOs->size()), EBOs->data());
    }
    delete data;
    delete VBOs;
    delete EBOs;
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
    if (data->size() > 0)
    {
        glDeleteVertexArrays(static_cast<GLsizei>(data->size()), data->data());
    }
    if (VBOs->size() > 0)
    {
        glDeleteBuffers(static_cast<GLsizei>(VBOs->size()), VBOs->data());
    }
    if (EBOs->size() > 0)
    {
        glDeleteBuffers(static_cast<GLsizei>(EBOs->size()), EBOs->data());
    }
    data->clear();
    VBOs->clear();
    EBOs->clear();
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
    return static_cast<int>(data->size());
}

void VAOVectorDS::addVBO(unsigned int ui)
{
    VBOs->push_back(ui);
}

void VAOVectorDS::addEBO(unsigned int ui)
{
    EBOs->push_back(ui);
}

void VAOVectorDS::clearVBOs()
{
    if (VBOs->size() > 0)
    {
        glDeleteBuffers(static_cast<GLsizei>(VBOs->size()), VBOs->data());
    }
}

void VAOVectorDS::clearEBOs()
{
    if (EBOs->size() > 0)
    {
        glDeleteBuffers(static_cast<GLsizei>(EBOs->size()), EBOs->data());
    }
}