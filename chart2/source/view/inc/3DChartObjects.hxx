/* -*- Mode: C++; tab-width: 4; indent-tabs-mode: nil; c-basic-offset: 4 -*- */
/*
 * This file is part of the LibreOffice project.
 *
 * This Source Code Form is subject to the terms of the Mozilla Public
 * License, v. 2.0. If a copy of the MPL was not distributed with this
 * file, You can obtain one at http://mozilla.org/MPL/2.0/.
 */

#include <glm/glm.hpp>
#include <tools/color.hxx>
#include <vcl/bitmapex.hxx>

#include <vcl/opengl/OpenGLContext.hxx>
#include "GL3DRenderer.hxx"

#include <boost/ptr_container/ptr_map.hpp>

namespace chart {

namespace opengl3D {

class TextCache
{
public:
    const BitmapEx& getText(OUString const & rText);
private:
    typedef boost::ptr_map<OUString const, BitmapEx> TextCacheType;

    TextCacheType maTextCache;
};

class Renderable3DObject
{
public:
    Renderable3DObject(OpenGL3DRenderer* pRenderer, sal_uInt32 nId);

    virtual ~Renderable3DObject() {};

    virtual void render();

    OpenGL3DRenderer* getRender();

protected:
    OpenGL3DRenderer* mpRenderer;
    sal_uInt32 mnUniqueId;
};

class Bar : public Renderable3DObject
{
public:
    Bar(OpenGL3DRenderer* pRenderer, const glm::mat4& rPosition, sal_uInt32 nColor, sal_uInt32 nId);

    virtual void render() SAL_OVERRIDE;
private:
    bool mbRoundedCorners;
    glm::mat4 maPos;
    Color maColor; // RGBA fill color
};

class Line : public Renderable3DObject
{
public:
    Line(OpenGL3DRenderer* pRenderer, sal_uInt32 nId);

    virtual void render() SAL_OVERRIDE;

    void setPosition(const glm::vec3& rBegin, const glm::vec3& rEnd);
    void setLineColor(const Color& rColor);

private:
    glm::vec3 maPosBegin;
    glm::vec3 maPosEnd;
    Color maLineColor; // RGBA line color
};

class Text : public Renderable3DObject
{
public:
    Text(OpenGL3DRenderer* pRenderer, TextCache& rTextCache, const OUString& rStr, sal_uInt32 nId);
    virtual void render() SAL_OVERRIDE;

    Size getSize() const;

    void setPosition(const glm::vec3& rTopLeft, const glm::vec3& rTopRight, const glm::vec3& rBottomRight);

private:
    const BitmapEx& mrText;
    glm::vec3 maTopLeft;
    glm::vec3 maTopRight;
    glm::vec3 maBottomRight;
};

class ScreenText : public Renderable3DObject
{
public:
    ScreenText(OpenGL3DRenderer* pRenderer, TextCache& rTextCache, const OUString& rStr, sal_uInt32 nId);

    virtual void render() SAL_OVERRIDE;
    void setPosition(const glm::vec2& rTopLeft, const glm::vec2& rBottomRight);

private:
    const BitmapEx& mrText;
    glm::vec2 maTopLeft;
    glm::vec2 maBottomRight;
};

class Rectangle : public Renderable3DObject
{
public:
    Rectangle(OpenGL3DRenderer* pRenderer, sal_uInt32 nId);
    virtual void render() SAL_OVERRIDE;

    void setPosition(const glm::vec3& rTopLeft, const glm::vec3& rTopRight, const glm::vec3& rBottomRight);
    void setFillColor(const Color& rColor);
    void setLineColor(const Color& rColor);

private:
    glm::vec3 maTopLeft;
    glm::vec3 maTopRight;
    glm::vec3 maBottomRight;
    Color maColor; // RGBA fill color
    Color maLineColor; // RGBA line color
};

class Camera : public Renderable3DObject
{
public:
    Camera(OpenGL3DRenderer* pRenderer);
    virtual void render() SAL_OVERRIDE;

    void setPosition(const glm::vec3& rPos);
    void setDirection(const glm::vec3& rPos);

private:
    glm::vec3 maPos;
    glm::vec3 maUp;
    glm::vec3 maDirection;
};

}

}

/* vim:set shiftwidth=4 softtabstop=4 expandtab: */
