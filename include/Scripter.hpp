#pragma once
#include <vector>
#include <unordered_map>
#include <any>

struct GeneralContext;

template <typename OwnerType>
class Scripter {
public:
    using ScriptFunc = void(*)(OwnerType&, const GeneralContext&);

    void addScript(ScriptFunc func) {
        scripts.push_back(func);
    }

    void runScripts(OwnerType& owner, const GeneralContext& ctx) {
        for (auto& script : scripts) {
            if (script) {
                script(owner, ctx);
            }
        }
    }

    /*Script States*/
    std::unordered_map<std::string, std::any> scriptState;

    template<typename T, typename... Args>
    T& getState(const std::string& key, Args&&... args) {
        auto& stateAny = scriptState[key];
        if (!stateAny.has_value()) {
            stateAny = T(std::forward<Args>(args)...);
        }
        return std::any_cast<T&>(stateAny);
    }
    /*Script States*/

private:
    std::vector<ScriptFunc> scripts;
};