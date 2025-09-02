#pragma once

#include "cocos2d.h"

using namespace cocos2d;

class InfiniteParallax : public Node {
public:
    class IntVec2 {
    public:
        int x;
        int y;
        IntVec2(int x, int y) : x(x), y(y) {}
        IntVec2() : x(0), y(0) {}
        bool operator<(const IntVec2& other) const {
            return (x < other.x) || (x == other.x && y < other.y);
        }
    };
    enum class DrawType { kAuto, kManual };
protected:
    Rect                            m_visibleArea;
    DrawType                        m_drawType;
    Vec2                            m_speedMultiplier;
    Vec2                            m_parallaxScale;
    Vec2                            m_parallaxPos;
    SpriteFrame*                    m_frame;
    DrawNode*                       m_debugDraw;
    Size                            m_parallaxSize;
    bool                            m_debugMode;

    Vector<Sprite*>                 m_childs;
    std::map<Sprite*, IntVec2>      m_cellByChild;
    std::map<IntVec2, Sprite*>      m_childByCell;

    float m_paddingLeft;
    float m_paddingRight;
    float m_paddingTop;
    float m_paddingBottom;
public:
    CREATE_FUNC(InfiniteParallax);
    virtual bool init() override;
    virtual void setContentSize(const Size& size) override;

    ~InfiniteParallax();
    void updateLayout();

    // --- GETTERS ---
    Rect getVisibleArea() { return m_visibleArea; }
    DrawType getDrawType() { return m_drawType; }
    Vec2 getMultiplier() { return m_speedMultiplier; }
    Vec2 getParallaxScale() { return m_parallaxScale; }
    Vec2 getParallaxPosition() { return m_parallaxPos; }
    bool isDebugEnabled() { return m_debugMode; }
    void getPadding(float* l, float* r, float* t, float *b);
    Size getParallaxSize() { return m_parallaxSize; }

    // --- SETTERS ---
    void setVisibleArea(const Rect& visibleArea) {
        m_visibleArea = visibleArea;
        updateLayout();
    }
    void setDrawType(DrawType drawType) {
        m_drawType = drawType;
        updateLayout();
    }
    void setMultiplier(const Vec2& multiplier) {
        m_speedMultiplier = multiplier;
        updateLayout();
    }
    void setParallaxScale(const Vec2& scale) {
        if (m_parallaxScale != scale)
        {
            m_parallaxScale = scale;
            this->updateLayout();
        }
    }
    void setParallaxPosition(const Vec2& pos)
    {
        if (m_parallaxPos != pos)
        {
            m_parallaxPos = pos;
            this->updateLayout();
        }
    }
    void setDebugEnabled(bool enabled) {
        if (m_debugMode != enabled)
        {
            m_debugMode = enabled;
            m_debugDraw->setVisible(enabled);

            if (enabled)
            {
                this->updateLayout();
            }
        }
    }
    void setParallaxSize(const Size& size)
    {
        if (size != m_parallaxSize)
        {
            m_parallaxSize = size;
            this->updateLayout();
        }
    }
    void setPadding(float l, float r, float t, float b);
    void setSpriteFrame(SpriteFrame* frame);
    void setTexture(Texture2D* texture);
    void setSpriteFrame(std::string filename);
    void setTexture(std::string filename);
    void move(Vec2 dir);
};
