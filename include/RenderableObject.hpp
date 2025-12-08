#include "GameObject.hpp"
#include "Renderizer.hpp"

class RenderableObject: public GameObject {
public:
    RenderableObject(RenderizerParameters params);
    void update() override;
    Renderizer renderizer;
};