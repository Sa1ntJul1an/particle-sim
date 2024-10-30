#include "configuration_menu.h"
#include <SFML/Graphics.hpp>
#include <iostream>


ConfigurationMenu::ConfigurationMenu(sf::RenderWindow& renderWindow, sf::Font font) : _renderWindow(renderWindow) {

    _labelText.setFont(font);
    _labelText.setFillColor(_textColor);
    _labelText.setCharacterSize(_textSize);

    _windowWidth = _renderWindow.getSize().x;
    _windowHeight = _renderWindow.getSize().y;

    _trackbarSliderSize = sf::Vector2f(_windowWidth - 2 * _trackbarSliderHorizontalPadding, _trackbarSliderHeight);

    // initialize SFML shapes for UI elements 
    _uiBackgroundRectangle.setFillColor(_elementBackgroundColor);
    _uiBackgroundRectangle.setSize(sf::Vector2f(_windowWidth, _elementHeight));

    _trackbarButtonRectangle.setFillColor(_trackbarButtonColor);
    _trackbarButtonRectangle.setSize(_trackbarButtonSize);

    _trackbarSliderRectangle.setFillColor(_trackbarSliderColor);
    _trackbarSliderRectangle.setSize(_trackbarSliderSize);
}

void ConfigurationMenu::evaluateMouseClick(sf::Vector2i mousePosition) {
    std::cout << "mouse click in menu at y = " << mousePosition.y << std::endl;
}

void ConfigurationMenu::drawUI() {
    for (UIElement element : _uiElements) {
        _drawUIElement(element);
    }
}

void ConfigurationMenu::_drawToggle(std::string label, float yTop, bool value) {
    
}

void ConfigurationMenu::_drawTrackbar(std::string label, float yTop, float trackbarPercentage) {
    _uiBackgroundRectangle.setPosition(sf::Vector2f(0, yTop));
    _trackbarSliderRectangle.setPosition(sf::Vector2f(_trackbarSliderHorizontalPadding, yTop + (_elementHeight / 2.0) - (_trackbarSliderHeight / 2.0)));
    _trackbarButtonRectangle.setPosition(sf::Vector2f());

    _renderWindow.draw(_uiBackgroundRectangle);
    _renderWindow.draw(_trackbarSliderRectangle);
    _renderWindow.draw(_trackbarButtonRectangle);
}

void ConfigurationMenu::_drawUIElement(UIElement uiElement) {
    if (std::holds_alternative<bool>(uiElement.value)) {  // ui element is bool (toggle)
        bool value = std::get<bool>(uiElement.value);

        _drawToggle(uiElement.label, uiElement.yTop, value);
    } else if (std::holds_alternative<float>(uiElement.value)) { // ui element is float (trackbar)
        float value = std::get<float>(uiElement.value);

        float trackbarPercentage = (value - uiElement.minVal) / (uiElement.maxVal - uiElement.minVal);
        _drawTrackbar(uiElement.label, uiElement.yTop, trackbarPercentage);
    }
}