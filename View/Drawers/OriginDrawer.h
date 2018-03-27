#ifndef ORIGINDRAWER_H
#define ORIGINDRAWER_H

#include "ShaderDrawable.h"

class OriginDrawer : public ShaderDrawable
{
public:
    OriginDrawer();

protected:
    bool updateData();
};

#endif // ORIGINDRAWER_H
