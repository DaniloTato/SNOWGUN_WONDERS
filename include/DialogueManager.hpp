#pragma once
#include <string>
#include <unordered_map>
#include <vector>

#include "GameText.hpp"
#include "GameObject.hpp"
#include "RenderizerParameters.hpp"

class DialogueManager {
public:
    static DialogueManager& getInstance(RenderizerParameters& gameTextParams);

    bool loadDialoguesFromFile(const std::string& filename);

    const std::string* getDialogue(const std::string& key) const;
    void assignDialogue(GameObject* object, const std::string& key);
    void onTrigger(GameObject* object);
    void queueDestroyAll();
    void destroyAll();

private:
    DialogueManager(RenderizerParameters& gameTextParams);
    std::unordered_map<std::string, std::string> dialogues;
    std::unordered_map<GameObject*, std::string> assigned;
    std::vector<GameText*> activeTexts;

    RenderizerParameters& gameTextParams;

    DialogueManager(const DialogueManager&) = delete;
    DialogueManager& operator=(const DialogueManager&) = delete;

    DialogueManager(DialogueManager&&) = delete;
    DialogueManager& operator=(DialogueManager&&) = delete;
};