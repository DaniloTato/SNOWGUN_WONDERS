#pragma once
#include <vector>
#include <unordered_map>
#include <any>

struct GeneralContext;

template <typename OwnerType>
class Scripter {
public:
    using ScriptFunc = void(*)(OwnerType&, const GeneralContext&);

    std::unordered_map<std::string, std::any> scriptState;

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

private:
    std::vector<ScriptFunc> scripts;
};