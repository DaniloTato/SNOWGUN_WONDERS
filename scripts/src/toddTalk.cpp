#include "toddTalk.hpp"

#include "BasicCollider.hpp"
#include "RenderableObject.hpp"
#include "DialogueManager.hpp"
#include "TangibleObject.hpp"

namespace script{

    namespace {
        struct TalkCount{
        public:
            int count = 0;
        };
    }

    void toddTalk(RenderableObject& renderable, const GeneralContext& ctx) {

        auto& stateAny = renderable.scripter.scriptState["toddTalk"];
        if (!stateAny.has_value()) {
            stateAny = TalkCount{};
        }
        auto& state = std::any_cast<TalkCount&>(stateAny);

        TangibleObject* player = ctx.player;
        if(!state.count && BasicCollider::tangibleAndRenderableCollision(player, &renderable)){
            state.count = 1;
            DialogueManager::getInstance().onTrigger(&renderable);
        }
    }

}