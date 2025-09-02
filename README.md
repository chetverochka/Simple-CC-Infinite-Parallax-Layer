# Simple-CC-Infinite-Parallax-Layer
Compatible with cocos2d-x 4.0/Axmol 2.1.5

## Usage example
```
bool MainScene::init()
{
    if (!Scene::init())
    {
        return false;
    }

    float size = this->getContentSize().height;

    InfiniteParallax* parallax = InfiniteParallax::create();
    parallax->setTexture("game_bg_01_001-hd.png"); // ->setSpriteFrame();
    parallax->setContentSize(this->getContentSize());
    parallax->setParallaxSize({ size, size });
    parallax->setParallaxScale(Vec2(1, 1));
    parallax->setMultiplier(Vec2(1.f, 0.f));
    parallax->setPadding(0, -30, 0, 0);
    parallax->setParallaxScale(Vec2(0.5, 0.5));
    parallax->setColor(Color3B::YELLOW);
    this->addChild(parallax, 0, "parallax");

    return true;
}
```
