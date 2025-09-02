#include "InfiniteParallax.h"

bool InfiniteParallax::init() {
    if (!Node::init())
        return false;

    m_drawType = InfiniteParallax::DrawType::kAuto;
    m_speedMultiplier = Vec2::ONE;
    m_visibleArea = Rect(Vec2::ZERO, Size(100, 100));
    m_parallaxScale = Vec2::ONE;
    m_parallaxPos = Vec2::ZERO;
    m_debugMode = false;
    m_paddingLeft = m_paddingRight = m_paddingTop = m_paddingBottom = 0.f;
    m_parallaxSize = Size(10.f, 10.f);

    this->setCascadeColorEnabled(true);
    this->setCascadeOpacityEnabled(true);

    DrawNode* debugDraw = DrawNode::create();
    if (debugDraw)
    {
        m_debugDraw = debugDraw;
        m_debugDraw->setVisible(m_debugMode);
        this->addChild(debugDraw, 10);
    }
    else {
        return false;
    }

    this->updateLayout();

    return true;
}

InfiniteParallax::~InfiniteParallax() {
}

void InfiniteParallax::setContentSize(const Size& size)
{
    Node::setContentSize(size);
    this->updateLayout(); // todo: if drawtype == auto
}

void InfiniteParallax::updateLayout()
{
    // to do: Batching
    SpriteFrame* frame = m_frame;
    if (!frame)
    {
        return;
    }
    //Size frameSize = frame->getRectInPixels().size;

    Size contentSize = m_parallaxSize; // to do: setParallaxContentSize();
    Vec2 parallaxPos = this->getParallaxPosition();
    Vec2 parallaxScale = this->getParallaxScale();

    float paddingLeft, paddingRight, paddingTop, paddingBottom;
    this->getPadding(&paddingLeft, &paddingRight, &paddingTop, &paddingBottom);

    Vec2 gridOffset = parallaxPos * this->getMultiplier();
    Vec2 gridStep = contentSize * parallaxScale + Vec2(paddingLeft + paddingRight, paddingTop + paddingBottom);

    std::min(gridStep.x, 10.f);
    std::min(gridStep.y, 10.f);

    DrawType drawMode = m_drawType;
    bool debugEnabled = m_debugMode;

    Rect visibleArea;
    switch (drawMode)
    {
    default:
    case DrawType::kAuto:
        visibleArea = Rect(this->getPosition() - this->getContentSize() * this->getAnchorPoint(), this->getContentSize());
        break;
    case DrawType::kManual:
        visibleArea = this->getVisibleArea();
        break;
    }

    std::min(visibleArea.size.width, 10.f);
    std::min(visibleArea.size.height, 10.f);

    int16_t startX, endX, startY, endY;
    startX  = floor((visibleArea.getMinX() - gridOffset.x) / gridStep.width);
    endX    = ceil((visibleArea.getMaxX() - gridOffset.x) / gridStep.width);
    startY  = floor((visibleArea.getMinY() - gridOffset.y) / gridStep.height);
    endY    = ceil((visibleArea.getMaxY() - gridOffset.y) / gridStep.height);

    if (debugEnabled)
    {
        CCLOG("startCellX: %d, endCellX: %d, startCellY: %d, endCellY: %d", startX, endX, startY, endY);
    }

    std::map<IntVec2, Sprite*>& childByCell = m_childByCell;
    std::map<Sprite*, IntVec2>& cellByChild = m_cellByChild;

    Vector<Sprite*>& childs = m_childs;
    Node* parentNode = this;

    // deleting childs beyond visible area
    for (int i = childs.size() - 1; i >= 0; i--)
    {
        Sprite* target = childs[i];
        if (!target)
        {
            continue;
        }

        bool containsCell = cellByChild.contains(target);
        if (!containsCell)
        {
            continue;
        }

        IntVec2 cell = cellByChild[target];
        // Спрайт в ячейке за пределами видимой области
        if (cell.x < startX || cell.x > endX || cell.y < startY || cell.y > endY) 
        {
            parentNode->removeChild(target, true);
            if (childByCell.contains(cell))
            {
                childByCell.erase(cell);
            }
            cellByChild.erase(target);
            childs.eraseObject(target);
           
        }
    }


    // creating child in visible area
    for (int cellY = startY; cellY < endY; cellY++)
    {
        for (int cellX = startX; cellX < endX; cellX++)
        {
            IntVec2 currentCell = IntVec2(cellX, cellY);

            bool hasSprite = childByCell.contains(currentCell);
            if (hasSprite)
            {
                continue;
            }

            Sprite* child = Sprite::create();
            if (!child)
            {
                continue;
            }
            parentNode->addChild(child, cellY * cellX);
            childs.pushBack(child);
            childByCell[currentCell] = child;
            cellByChild[child] = currentCell;
        }
    }

    // update childs
    for (int i = 0; i < childs.size(); i++)
    {
        Sprite* target = childs[i];

        IntVec2 cell = cellByChild[target]; // to do: assert

        Size targetSize = contentSize * parallaxScale;

        Vec2 cellPos;
        cellPos.x = cell.x * gridStep.width + gridOffset.x + targetSize.width / 2 + paddingLeft;
        cellPos.y = cell.y * gridStep.height + gridOffset.y + targetSize.height / 2 + paddingBottom;

        target->setSpriteFrame(frame);
        target->setContentSize(targetSize);
        target->setPosition(cellPos);
    }

    // drawing debug rect
    DrawNode* debugNode = m_debugDraw;
    if (debugEnabled)
    {
        debugNode->clear();
        //Node rect
        Rect nodeBox = this->getBoundingBox();
        debugNode->drawSolidRect(nodeBox.origin, nodeBox.origin + nodeBox.size, Color4B(255, 0, 0, 20));


        debugNode->drawRect(visibleArea.origin, visibleArea.origin + visibleArea.size, Color4B::RED);

        debugNode->drawDot(gridOffset, 5.f, Color4B::YELLOW);
        debugNode->drawLine(gridOffset, gridOffset + Vec2(2500, 0), Color4B::YELLOW);
        debugNode->drawLine(gridOffset, gridOffset + Vec2(0, 2500), Color4B::YELLOW);

        // bounding
        debugNode->drawRect(Vec2(startX, startY) * gridStep + gridOffset, Vec2(endX, endY) * gridStep + gridOffset, Color4B::WHITE);
    }
}

void InfiniteParallax::setSpriteFrame(SpriteFrame* frame)
{
    if (m_frame != frame || !m_frame)
    {
        m_frame = frame;
        m_parallaxSize = frame->getRectInPixels().size;
        this->updateLayout();
    }
}

void InfiniteParallax::setTexture(Texture2D* texture)
{
    SpriteFrame* frame = SpriteFrame::createWithTexture(
        texture,
        Rect(0, 0, texture->getContentSize().width, texture->getContentSize().height)
    );
    this->setSpriteFrame(frame);
}

void InfiniteParallax::setSpriteFrame(std::string filename)
{
    this->setSpriteFrame(SpriteFrameCache::getInstance()->getSpriteFrameByName(filename));
}

void InfiniteParallax::setTexture(std::string filename)
{
    this->setTexture(Director::getInstance()->getTextureCache()->addImage(filename));
}

void InfiniteParallax::move(Vec2 dir)
{
    this->setParallaxPosition(this->getParallaxPosition() + dir);
}

void InfiniteParallax::getPadding(float* l, float* r, float* t, float *b)
{
    *l = m_paddingLeft;
    *r = m_paddingRight;
    *t = m_paddingTop;
    *b = m_paddingBottom;
}

void InfiniteParallax::setPadding(float l, float r, float t, float b)
{
    m_paddingLeft = l;
    m_paddingRight = r;
    m_paddingTop = t;
    m_paddingBottom = b;
    this->updateLayout();
}
