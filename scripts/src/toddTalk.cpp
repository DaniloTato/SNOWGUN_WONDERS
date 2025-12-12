#include "toddTalk.hpp"

#include "BasicCollider.hpp"
#include "RenderableObject.hpp"
#include "DialogueManager.hpp"
#include "TangibleObject.hpp"

namespace script{

    class TalkCount{
    public:
        int count = 0;
    };

    static TalkCount tc;

    void toddTalk(RenderableObject& renderable, const GeneralContext& ctx) {
        TangibleObject* player = ctx.player;
        if(!tc.count && BasicCollider::tangibleAndRenderableCollision(player, &renderable)){
            tc.count = 1;
            DialogueManager::getInstance(ctx.textParams).onTrigger(&renderable);
        }
    }

}