#include "DialogueManager.hpp"
#include <fstream>
#include <sstream>
#include <iostream>

DialogueManager::DialogueManager(){}

DialogueManager& DialogueManager::getInstance() {
    static DialogueManager dm;
    return dm;
}

bool DialogueManager::loadDialoguesFromFile(const std::string& filename) {
    std::ifstream file(filename);
    if (!file.is_open()) {
        std::cerr << "DialogueManager: Could not open file " << filename << std::endl;
        return false;
    }

    dialogues.clear();

    std::string line;
    std::string currentKey;
    std::stringstream currentDialogue;

    while (std::getline(file, line)) {

        if (line.rfind("[Key=", 0) == 0) {
            if (!currentKey.empty()) {
                dialogues[currentKey] = currentDialogue.str();
                currentDialogue.str("");
            }

            size_t eq = line.find('=');
            size_t end = line.find(']');
            currentKey = line.substr(eq + 1, end - eq - 1);
        } else {
            currentDialogue << line << "\n";
        }
    }

    if (!currentKey.empty())
        dialogues[currentKey] = currentDialogue.str();

    return true;
}

const std::string* DialogueManager::getDialogue(const std::string& key) const {
    auto it = dialogues.find(key);
    if (it == dialogues.end()) return nullptr;
    return &it->second;
}

void DialogueManager::assignDialogue(GameObject* obj, const std::string& key) {
    assigned[obj] = key;
}

void DialogueManager::createText(const RenderizerParameters& params, const std::string* markup){
    GameText* gt = new GameText(params);
    gt->setFontAtlas(params.texture, 9, 8, 95, 32);
    gt->loadFromMarkup(*markup);

    activeTexts.push_back(gt);
}

void DialogueManager::onTrigger(GameObject* obj) {
    auto it = assigned.find(obj);
    if (it == assigned.end()) {
        std::cout << "DialogueManager: object triggered but has no dialogue\n";
        return;
    }

    const std::string& key = it->second;
    const std::string* markup = getDialogue(key);

    if (!markup) {
        std::cout << "DialogueManager: dialogue key not found: " << key << "\n";
        return;
    }

    if(attachedTextParams){
        queueCreateText(*attachedTextParams, markup);
    }
}

void DialogueManager::queueCreateText(const RenderizerParameters& params, const std::string* markup){
    createQueue.push_back({params,markup});
}

void DialogueManager::applyQueuedTextChanges(){
    for (const auto& createReq : createQueue) {
        createText(createReq.params, createReq.markup);
    }
    createQueue.clear();
}

void  DialogueManager::attachTextParams(RenderizerParameters* params){
    attachedTextParams = params;
}