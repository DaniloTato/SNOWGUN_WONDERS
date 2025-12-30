#include "toddTalk.hpp"

#include "BasicCollider.hpp"
#include "DialogueManager.hpp"
#include "RenderableObject.hpp"
#include "TangibleObject.hpp"

namespace script {

namespace {
struct TalkCount {
  int count = 0;
};
} // namespace

void toddTalk(RenderableObject &renderable, const GeneralContext &ctx) {

  auto &state = renderable.scripter.getState<TalkCount>("toddTalk");

  TangibleObject *player = ctx.player;
  if (!state.count &&
      BasicCollider::tangibleAndRenderableCollision(player, &renderable)) {
    state.count = 1;
    DialogueManager::getInstance().onTrigger(&renderable);
  }
}

} // namespace script