#include "toddTalk.hpp"

#include "RenderableObject.hpp"
#include "DialogueManager.hpp"
#include "TangibleObject.hpp"

namespace script{

    class TalkCount{
    public:
        int count = 0;
    };

    static TalkCount tc;

    void toddTalk(RenderableObject& tangible, const GeneralContext& ctx) {
        //Create an imCollidingWith(*collider) imCollidingWith(position,size) function;
        TangibleObject* player = ctx.player;
        if(!tc.count && ctx.player -> collider.isColliding(player->position, {17,17}, tangible.position, {16,16})){
            tc.count = 1;
            DialogueManager::getInstance(ctx.textParams).onTrigger(&tangible);
        }
    }

}