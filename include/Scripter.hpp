#pragma once

#include <vector>

template <typename OwnerType, typename ContextType>
class Scripter {
public:
    using ScriptFunc = void(*)(OwnerType&, const ContextType&);

    void addScript(ScriptFunc func);

    void runScripts(OwnerType& owner, const ContextType& ctx);

private:
    std::vector<ScriptFunc> scripts;
};