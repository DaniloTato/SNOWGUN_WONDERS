#pragma once
#include <vector>

template <typename OwnerType, typename ContextType>
class Scripter {
public:
    using ScriptFunc = void(*)(OwnerType&, const ContextType&);

    void addScript(ScriptFunc func) {
        scripts.push_back(func);
    }

    void runScripts(OwnerType& owner, const ContextType& ctx) {
        for (auto& script : scripts) {
            if (script) {
                script(owner, ctx);
            }
        }
    }

private:
    std::vector<ScriptFunc> scripts;
};