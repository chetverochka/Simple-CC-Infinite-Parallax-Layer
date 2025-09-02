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

## Немного от себя

с помощью `->setDrawType(DrawType)` можно указать способ определения области. По умолчанию она автоматическая и берётся исходя из параметров самой ноды, но ноду можно не трогать а вручную укзаать область параллакса в `->setVisibleArea(Rect)`. В любом случае определяется Rect-прямоугольник 
с "безопасной" зоной параллакса, в которой они будут рисоваться, за пределами этой зоны, все лишние спрайты будут отрезаться и удаляться. Таким образом можно делать самые разные параллакс эффекты добавляя их на сцену: небо, горы и т.д.

Советы к модификации ноды:
1. добавьте контейнер для спрайтов ClippingNode чтобы спрайты не выходили за пределы зоны отрисовки
2. (!!крайне рекомендуется!!) Мне лень было это сделать, но вам рекомендую помещать спрайты в SpriteBatchNode для оптимизации, особенно если дочерних элементов будет много, это снизит нагрузку и повысит фпс засчёт одного вызова.
