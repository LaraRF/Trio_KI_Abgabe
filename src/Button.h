//
// Created by lrfri on 18.05.2025.
//

#ifndef RAYLIBSTARTER_BUTTON_H
#define RAYLIBSTARTER_BUTTON_H
#include "GameTypes.h"
#include "UIComponent.h"
#include <functional>

class Button : public UIComponent {
public:
    Button(const std::string& text, std::function<void()> callback);

    void update(float deltaTime) override;
    void draw() const override;

    bool handleMouseClick(const Vector2& mousePosition) override;

    void setText(const std::string& text);
    std::string getText() const;

    void setSelected(bool isSelected);
    bool isSelected() const;

private:
    std::string text;
    std::function<void()> callback;
    bool hover;
    bool selected; // Gibt an, ob der Button ausgewählt ist
};


#endif //RAYLIBSTARTER_BUTTON_H
