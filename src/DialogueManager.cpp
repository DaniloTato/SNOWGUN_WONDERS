#include "DialogueManager.hpp"
#include "QueuedManager.hpp"
#include <fstream>
#include <sstream>
#include <iostream>

DialogueManager& DialogueManager::getInstance() {
    static DialogueManager instance;
    return instance;
}

DialogueManager::DialogueManager() {}

bool DialogueManager::loadDialoguesFromFile(const std::string& filename) {
    std::ifstream file(filename);
    if (!file.is_open()) {
        std::cerr << "DialogueManager: Could not open file " << filename << "\n";
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

void DialogueManager::assignDialogue(GameObject* object, const std::string& key) {
    assigned[object] = key;
}

void DialogueManager::onTrigger(GameObject* object) {
    auto it = assigned.find(object);
    if (it == assigned.end())
        return;

    const std::string* markup = getDialogue(it->second);
    if (!markup || !attachedTextParams)
        return;

    createQueue.push_back({ *attachedTextParams, markup });
}

void DialogueManager::attachTextParams(RenderizerParameters* params) {
    attachedTextParams = params;
}

GameText* DialogueManager::createFromRequest(const TextCreationRequest& req) {
    GameText* gt = new GameText(req.params);
    gt->setFontAtlas(req.params.texture, 9, 8, 95, 32);
    gt->loadFromMarkup(*req.markup);
    return gt;
}

void DialogueManager::destroyObject(GameText* text) {
    delete text;
}

void DialogueManager::onSceneUnload(){
    assigned.clear();
    attachedTextParams = nullptr;

    QueuedManager<GameText, TextCreationRequest>::onSceneUnload();
}