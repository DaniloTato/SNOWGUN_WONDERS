#include "GameText.hpp"
#include <sstream>
#include <algorithm>
#include <cctype>
#include <iostream>
#include "ColorPalette.hpp"
#include "Constants.hpp"
#include "RenderCommand.hpp"
#include "Renderizer.hpp"
#include "GameState.hpp"

std::string trimLeadingSpace(std::string s) {
    while (!s.empty() && std::isspace(static_cast<unsigned char>(s.front()))){
        s.erase(s.begin());
    }
    return s;
}

GameText::GameText(RenderizerParameters params)
    : lineSpacing(2)
    , renderizer(params)
{

    Renderizer::registerPair(this, &renderizer, params.registerAsRectShape);

    colorNameMap["white"] = sf::Color::White;
    colorNameMap["black"] = sf::Color::Black;
    colorNameMap["red"]   = ColorPalette::MexicanPink;
    colorNameMap["green"] = sf::Color::Green;
    colorNameMap["blue"]  = sf::Color::Blue;
    colorNameMap["yellow"]= ColorPalette::LimeGreen;
    colorNameMap["cyan"]  = sf::Color::Cyan;
    colorNameMap["magenta"]= sf::Color::Magenta;
    colorNameMap["purple"]= ColorPalette::VividIndigo;
    colorNameMap["transparent"] = sf::Color(0,0,0,0);
}

GameText::~GameText() {}

void GameText::update(const GeneralContext& ctx){
    float dt = 1.f/Constants::FRAME_RATE;
    globalTime += dt;
    advanceTypewriter();
    updateRenderCommandBuffer();
    renderizer.updateRenderCommands(renderCommandBuffer);
}

void GameText::setFontAtlas(sf::Texture& texture, int gW, int gH, int cols, int firstChar) {
    fontTex = &texture;
    glyphW = gW; glyphH = gH;
    atlasCols = cols;
    atlasFirstChar = firstChar;
}

void GameText::setSoundPool(const std::vector<std::string>& files) {
    soundBuffers.clear();

    for (const auto& f : files) {
        sf::SoundBuffer buf;
        if (buf.loadFromFile(f))
            soundBuffers.push_back(std::move(buf));
    }

    useSoundPool = !soundBuffers.empty();
    soundPlayIndex = 0;
}

void GameText::loadFromMarkup(const std::string& markup) {
    glyphs.clear();
    lines.clear();
    revealedCount = 0;
    typeTimer = 0.f;
    globalTime = 0.f;

    std::istringstream ss(markup);
    std::string line;
    std::string body;
    bool inHeader = true;
    while (std::getline(ss, line)) {
        if(line.empty()) continue;
        std::string trim = trimLeadingSpace(line);
        if (inHeader && !trim.empty() && trim[0] == '#') {
            parseHeaderLine(trim);
        } else {
            inHeader = false;
            body += trim;
            body += '\n';
        }
    }

    parseBody(body);
    rebuildLayout();
    resetTypewriter();
}

void GameText::parseHeaderLine(const std::string& line) {
    std::string s = line.substr(1); // remove leading '#'
    std::istringstream tk(s);
    std::string key;
    tk >> key;
    if (key == "effect") {
        std::string eff; tk >> eff;
        if (eff == "typewriter") {
            float speed = 0.04f;
            if (tk >> speed) {
                setTypewriter(true, speed);
            } else {
                setTypewriter(true, 0.04f);
            }
        } else {
            setTypewriter(false, 0.04f);
        }
    } else if (key == "position") {
        float x,y; if (tk >> x >> y) setPosition({x,y});
    } else if (key == "boundary") {
        float b; if (tk >> b) setBoundary(b);
    } else if (key == "soundpool") {
        std::string rest;
        std::getline(tk, rest);
        std::vector<std::string> files;
        for (size_t i=0;i<rest.size();) {
            if (rest[i] == '"' || rest[i] == '\'') {
                char q = rest[i++];
                std::string name;
                while (i < rest.size() && rest[i] != q) { name += rest[i++]; }
                if (i < rest.size()) ++i;
                if (!name.empty()) files.push_back(name);
            } else ++i;
        }
        if (!files.empty()) setSoundPool(files);
    } else if (key == "alignment") {
        std::string a; if (tk >> a) {
            std::transform(a.begin(), a.end(), a.begin(), ::tolower);
            if (a == "center") setAlignment(Align::Center);
            else if (a == "right") setAlignment(Align::Right);
            else setAlignment(Align::Left);
        }
    }
}

void GameText::parseBody(const std::string& body) {
    struct Attr { sf::Color color; Glyph::Anim anim; float animParam; };
    std::vector<Attr> stack;
    stack.push_back({ sf::Color::White, Glyph::Anim::None, 0.f });

    int appearCounter = 0;

    for (size_t i = 0; i < body.size();) {
        char c = body[i];
        if (c == '<') {
            // tag
            size_t j = body.find('>', i+1);
            if (j == std::string::npos) break;
            std::string tag = body.substr(i+1, j-i-1);
            // trim
            while (!tag.empty() && isspace((unsigned char)tag.front())) tag.erase(tag.begin());
            while (!tag.empty() && isspace((unsigned char)tag.back())) tag.pop_back();

            if (tag.size() >= 3 && tag.substr(0,3) == "ln>") {
                pushTextChar('\n', stack.back().color, stack.back().anim, stack.back().animParam);
            } else if (tag == "ln") {
                pushTextChar('\n', stack.back().color, stack.back().anim, stack.back().animParam);
            } else if (tag.size() >= 6 && tag.substr(0,6) == "/color") {
                // pop color - assume nested correctly
                if (stack.size() > 1) stack.pop_back();
            } else if (tag.size() >= 5 && tag.substr(0,5) == "color") {
                // color=xxx
                size_t eq = tag.find('=');
                if (eq != std::string::npos) {
                    std::string spec = tag.substr(eq+1);
                    sf::Color ccol = parseColorSpec(spec);
                    stack.push_back({ ccol, stack.back().anim, stack.back().animParam });
                }
            } else if (tag.size() >= 5 && tag.substr(0,5) == "/anim") {
                if (stack.size() > 1) stack.pop_back();
            } else if (tag.size() >= 4 && tag.substr(0,4) == "anim") {
                // anim=sin  OR anim=shake:2
                size_t eq = tag.find('=');
                if (eq != std::string::npos) {
                    std::string spec = tag.substr(eq+1);
                    std::string name = spec;
                    float param = 0.f;
                    size_t colon = spec.find(':');
                    if (colon != std::string::npos) {
                        name = spec.substr(0, colon);
                        param = std::stof(spec.substr(colon+1));
                    }
                    Glyph::Anim an = Glyph::Anim::None;
                    if (name == "sin") an = Glyph::Anim::Sin;
                    else if (name == "shake") an = Glyph::Anim::Shake;
                    stack.push_back({ stack.back().color, an, param });
                }
            } else if (!tag.empty() && tag[0] == '/') {
                // generic close: pop
                if (stack.size() > 1) stack.pop_back();
            } else {
                // unknown tag - ignore
            }

            i = j + 1;
            continue;
        }

        if (c == '\n') {
            pushTextChar('\n', stack.back().color, stack.back().anim, stack.back().animParam);
            ++i;
            continue;
        }

        // normal character
        pushTextChar(c, stack.back().color, stack.back().anim, stack.back().animParam);
        // assign appear index in sequential order (skip newline characters for layout but include them for position)
        glyphs.back().appearIndex = appearCounter++;
        ++i;
    }
}

void GameText::pushTextChar(char c, const sf::Color& color, Glyph::Anim anim, float animParam) {
    Glyph g;
    g.c = c;
    
    int code = static_cast<unsigned char>(c);
    if (code < atlasFirstChar || !fontTex) {
        g.texRect = sf::IntRect(0,0,glyphW,glyphH);
    } else {
        int idx = code - atlasFirstChar;
        int col = idx % atlasCols;
        int row = idx / atlasCols;
        g.texRect = sf::IntRect(col * glyphW, row * glyphH, glyphW, glyphH);
    }
    g.color = color;
    g.anim = anim;
    g.animParam = animParam;
    g.phase = static_cast<float>(glyphs.size()) * 0.2f;
    g.appearIndex = -1;
    glyphs.push_back(std::move(g));
}

sf::Color GameText::parseColorSpec(const std::string& s) const {
    std::string t = s;
    // trim
    while (!t.empty() && isspace((unsigned char)t.front())) t.erase(t.begin());
    while (!t.empty() && isspace((unsigned char)t.back())) t.pop_back();
    // lower
    std::transform(t.begin(), t.end(), t.begin(), ::tolower);
    if (t.empty()) return sf::Color::White;
    if (t[0] == '#') {
        // hex #RRGGBB
        if (t.size() >= 7) {
            unsigned int r=0,g=0,b=0;
            sscanf(t.c_str()+1, "%02x%02x%02x", &r, &g, &b);
            return sf::Color((sf::Uint8)r,(sf::Uint8)g,(sf::Uint8)b);
        }
    } else {
        auto it = colorNameMap.find(t);
        if (it != colorNameMap.end()) return it->second;
        // try hex without '#'
        if (t.size() == 6) {
            unsigned int r=0,g=0,b=0;
            sscanf(t.c_str(), "%02x%02x%02x", &r, &g, &b);
            return sf::Color((sf::Uint8)r,(sf::Uint8)g,(sf::Uint8)b);
        }
    }
    return sf::Color::White;
}

void GameText::rebuildLayout() {
    lines.clear();
    if (glyphs.empty()) return;

    std::vector<size_t> currentLine;
    float curLineWidth = 0.f;

    auto pushLine = [&](std::vector<size_t>& L) {
        if (!L.empty()) {
            lines.push_back(L);
            L.clear();
        } else {
            lines.push_back(std::vector<size_t>{});
        }
    };

    // measure char width = glyphW (no kerning)
    for (size_t i = 0; i < glyphs.size(); ++i) {
        Glyph &g = glyphs[i];
        if (g.c == '\n') {
            // commit current line
            pushLine(currentLine);
            curLineWidth = 0.f;
            continue;
        }

        float charW = static_cast<float>(glyphW);
        if (boundary < 1e5f && curLineWidth + charW > boundary && !currentLine.empty()) {
            // wrap: commit current line
            pushLine(currentLine);
            curLineWidth = 0.f;
        }

        currentLine.push_back(i);
        curLineWidth += charW;
    }
    // final push
    pushLine(currentLine);

    // Compute base positions per glyph using alignment and origin
    float lineY = 0.f;
    for (size_t li = 0; li < lines.size(); ++li) {
        const auto& L = lines[li];
        float lineW = measureLineWidth(L);
        float startX = origin.x;
        if (alignment == Align::Center) startX = origin.x + (boundary - lineW) * 0.5f;
        else if (alignment == Align::Right) startX = origin.x + (boundary - lineW);

        float cx = startX;
        for (size_t idx : L) {
            glyphs[idx].basePos = { cx, origin.y + lineY };
            cx += static_cast<float>(glyphW);
        }
        lineY += static_cast<float>(glyphH + lineSpacing);
    }

    // Assign appearIndex sequentially to all non-newline, so typewriter shows them in order
    int ap = 0;
    for (size_t i = 0; i < glyphs.size(); ++i) {
        if (glyphs[i].c != '\n') glyphs[i].appearIndex = ap++;
        else glyphs[i].appearIndex = -1;
    }

    revealableCount = 0;
    for (auto& g : glyphs) {
        if (g.c != '\n')
            g.appearIndex = revealableCount++;
        else
            g.appearIndex = -1;
    }
}

float GameText::measureLineWidth(const std::vector<size_t>& lineIndices) const {
    return static_cast<float>(lineIndices.size()) * static_cast<float>(glyphW);
}

void GameText::resetTypewriter() {
    if (effect == Effect::Typewriter) {
        revealedCount = 0;
        typeTimer = 0.f;
    } else {
        revealedCount = glyphs.size();
    }
}

void GameText::forceRevealAll() {
    revealedCount = glyphs.size();
}

void GameText::advanceTypewriter() {
    float dt = GameState::getInstance().dt();

    if (effect != Effect::Typewriter) return;

    typeTimer += dt;

    while (typeTimer >= typeDelay && revealedCount < glyphs.size()) {
        typeTimer -= typeDelay;
        revealedCount++;

        if (useSoundPool && playSoundOnChars && revealedCount <= revealableCount)
            playTypeSound();
    }
}

void GameText::playTypeSound() {
    if (!useSoundPool || soundBuffers.empty()) return;

    typingSound.stop();
    typingSound.setBuffer(soundBuffers[soundPlayIndex]);
    typingSound.play();

    soundPlayIndex = (soundPlayIndex + 1) % soundBuffers.size();
    typingSound.stop();
}

void GameText::updateRenderCommandBuffer() {

    renderCommandBuffer.clear();

    size_t visible = (effect == Effect::Typewriter) ? std::min(revealedCount, glyphs.size()) : glyphs.size();

    for (size_t i = 0; i < glyphs.size(); ++i) {
        const Glyph& g = glyphs[i];
        if (g.appearIndex >= 0) {
            if ((size_t)g.appearIndex >= visible) continue;
        }
        if (g.c == '\n') continue;

        sf::Vector2f pos = g.basePos;
        if (g.anim == Glyph::Anim::Sin) {
            float amp = (g.animParam > 0.f ? g.animParam : 4.f);
            float freq = 6.f;
            float yoff = std::sin(globalTime * freq + g.phase) * amp;
            pos.y += yoff;
        } else if (g.anim == Glyph::Anim::Shake) {
            float intensity = (g.animParam > 0.f ? g.animParam : 2.f);
            float jitterX = (std::sin(globalTime * 40.f + g.phase) * 0.5f + std::sin(globalTime * 23.f + g.phase*1.3f) * 0.5f) * intensity;
            float jitterY = (std::sin(globalTime * 30.3f + g.phase*0.7f) * 0.6f) * intensity;
            pos.x += jitterX;
            pos.y += jitterY;
        }

        RenderCommand render = {g.texRect, pos, g.color};
        renderCommandBuffer.push_back(render);
    }
}