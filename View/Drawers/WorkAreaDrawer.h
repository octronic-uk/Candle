#pragma once
#include "ShaderDrawable.h"

class WorkAreaDrawer : public ShaderDrawable
{
public:
    WorkAreaDrawer(int width = 300,  int height = 300, QColor color = QColor("Red"));
    virtual ~WorkAreaDrawer() override;

protected:
    bool updateData() override;

    // ShaderDrawable interface
public:
    QVector3D getSizes() override;
    QVector3D getMinimumExtremes();
    QVector3D getMaximumExtremes();
    int getVertexCount() override;

    int getHeight() const;
    void setHeight(int height);

    int getWidth() const;
    void setWidth(int width);

    QColor getColor() const;
    void setColor(const QColor& color);

private:
    int mWidth;
    int mHeight;
    QColor mColor;
};
