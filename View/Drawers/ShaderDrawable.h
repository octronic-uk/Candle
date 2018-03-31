#pragma once

#include <QObject>
#include <QOpenGLFunctions>
#include <QOpenGLShaderProgram>
#include <QOpenGLBuffer>
#include <QOpenGLVertexArrayObject>
#include <QOpenGLTexture>
#include "Utils/Util.h"

struct VertexData
{
    QVector3D position;
    QVector4D color;
    QVector3D start;
};

class ShaderDrawable : protected QOpenGLFunctions
{
public:
    explicit ShaderDrawable();
    virtual ~ShaderDrawable();
    void update();
    void draw(QOpenGLShaderProgram *shaderProgram);

    bool needsUpdateGeometry() const;
    void updateGeometry(QOpenGLShaderProgram *shaderProgram = nullptr);

    virtual QVector3D getSizes();
    virtual QVector3D getMinimumExtremes();
    virtual QVector3D getMaximumExtremes();
    virtual int getVertexCount();

    double lineWidth() const;
    void setLineWidth(double lineWidth);

    bool visible() const;
    void setVisible(bool visible);

    double pointSize() const;
    void setPointSize(double pointSize);

signals:

public slots:

protected:
    double mLineWidth;
    double mPointSize;
    bool mVisible;
    QVector<VertexData> mLines;
    QVector<VertexData> mPoints;
    QVector<VertexData> mTriangles;
    QOpenGLTexture *mTexture;

    QOpenGLBuffer mVBO; // Protected for direct vbo access

    virtual bool updateData();
    void init();

private:
    QOpenGLVertexArrayObject mVAO;
    bool mNeedsUpdateGeometry;
};
