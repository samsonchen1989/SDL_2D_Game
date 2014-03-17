#ifndef LAYER_H
#define LAYER_H

class Layer
{
public:
    virtual ~Layer() {}

    virtual void render() = 0;
    virtual void update() = 0;
};

#endif