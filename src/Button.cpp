//
// Created by lrfri on 18.05.2025.
//

#include "Button.h"

Button::Button(const std::string& text, std::function<void()> callback)
        : UIComponent(), text(text), callback(callback), hover(false), selected(false) {}

void Button::update(float deltaTime) {
    Vector2 mousePos = GetMousePosition();
    hover = containsPoint(mousePos);
}

void Button::draw() const {
    Rectangle buttonRect = getBounds();

    // Zeichne Button mit entsprechender Farbe
    Color buttonColor = BUTTON_COLOR;

    if (selected) {
        // Ausgewählter Button hat eine andere Farbe
        buttonColor = BLUE;
    } else if (hover) {
        // Hover-Effekt
        buttonColor = BUTTON_HOVER_COLOR;
    }

    DrawRectangleRec(buttonRect, buttonColor);
    DrawRectangleLinesEx(buttonRect, 2, BLACK);

    int textWidth = MeasureText(text.c_str(), 20);
    DrawText(text.c_str(),
             x + (width - textWidth) / 2,
             y + height / 2 - 10,
             20, selected ? WHITE : BUTTON_TEXT_COLOR); // Ausgewählte Buttons haben weißen Text
}

bool Button::handleMouseClick(const Vector2& mousePosition) {
    if (UIComponent::handleMouseClick(mousePosition)) {
        if (callback) {
            callback();
        }
        return true;
    }
    return false;
}

void Button::setText(const std::string& text) {
    this->text = text;
}

std::string Button::getText() const {
    return text;
}

void Button::setSelected(bool isSelected) {
    selected = isSelected;
}

bool Button::isSelected() const {
    return selected;
}