#include "Scripter.hpp"

template <typename OwnerType, typename ContextType>
void Scripter<OwnerType, ContextType>::addScript(ScriptFunc func) {
    scripts.push_back(func);
}

template <typename OwnerType, typename ContextType>
void Scripter<OwnerType, ContextType>::runScripts(OwnerType& owner, const ContextType& ctx) {
    for (auto& script : scripts) {
        if (script) {
            script(owner, ctx);
        }
    }
}