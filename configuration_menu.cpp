#include "configuration_menu.h"
#include <SFML/Graphics.hpp>
#include <iostream>


ConfigurationMenu::ConfigurationMenu(sf::RenderWindow& renderWindow, sf::Font& font) : _renderWindow(renderWindow), _font(font) {

    _stream.precision(2);
    _stream << std::fixed;

    _labelText.setFont(font);
    _labelText.setFillColor(_textColor);
    _labelText.setCharacterSize(_textSize);

    _windowWidth = _renderWindow.getSize().x;
    _windowHeight = _renderWindow.getSize().y;

    _trackbarSliderSize = sf::Vector2f((_windowWidth / 2.0) - (2 * _trackbarSliderHorizontalPadding), _trackbarSliderHeight);

    // initialize SFML shapes for UI elements 
    _uiBackgroundRectangle.setFillColor(_elementBackgroundColor);
    _uiBackgroundRectangle.setSize(sf::Vector2f(_windowWidth, _elementHeight));

    _trackbarButtonRectangle.setFillColor(_trackbarButtonColor);
    _trackbarButtonRectangle.setSize(_trackbarButtonSize);

    _trackbarSliderRectangle.setFillColor(_trackbarSliderColor);
    _trackbarSliderRectangle.setSize(_trackbarSliderSize);
}

void ConfigurationMenu::addTrackbar() {
    UIElement * element = new UIElement; 
    element->minVal = 10; 
    element->maxVal = 100;
    float value = 20.0;
    element->value = value;
    element->label = "testlabel";
    element->yTop = 20;

    UIElement * elementPointer = new UIElement; 

    _uiElements.push_back(element);
}

void ConfigurationMenu::evaluateMouseClick(sf::Vector2i mousePosition) {
    // std::cout << "mouse click in menu at y = " << mousePosition.y << std::endl;
    for (UIElement* element : _uiElements) {
        // check if mouse click is within UI element
        if (mousePosition.y > element->yTop && mousePosition.y < _elementHeight + element->yTop) {

            float activationPercent = (mousePosition.x - _trackbarSliderHorizontalPadding) / (_trackbarSliderSize.x - _trackbarSliderHorizontalPadding);

            if (activationPercent > 1.0) { 
                activationPercent = 1.0;
            } else if (activationPercent < 0.0) {
                activationPercent = 0.0;
            }

            float elementValue = activationPercent * (element->maxVal - element->minVal) + element->minVal;

            element->value = elementValue;
        }
    }
}

void ConfigurationMenu::drawUI() {
    for (UIElement* element : _uiElements) {
        _drawUIElement(element);
    }
}

void ConfigurationMenu::_drawToggle(std::string label, float yTop, bool value) {
    
}

void ConfigurationMenu::_drawTrackbar(std::string label, float yTop, float trackbarPercentage, float value) {
    _uiBackgroundRectangle.setPosition(sf::Vector2f(0, yTop));
    _trackbarSliderRectangle.setPosition(sf::Vector2f(_trackbarSliderHorizontalPadding, yTop + (_elementHeight / 2.0) - (_trackbarSliderHeight / 2.0)));
    float trackbarButtonPositionX = trackbarPercentage * _trackbarSliderRectangle.getSize().x + _trackbarSliderHorizontalPadding - _trackbarButtonSize.x / 2.0;
    float trackbarButtonPositionY = _trackbarSliderRectangle.getPosition().y - _trackbarButtonSize.y / 2.0 + _trackbarSliderSize.y / 2.0;
    _trackbarButtonRectangle.setPosition(sf::Vector2f(trackbarButtonPositionX, trackbarButtonPositionY));

    _stream.str(std::string());
    _stream << label << ": " << value;

    _labelText.setString(_stream.str());
    _labelText.setCharacterSize(_textSize);
    _labelText.setPosition(_trackbarSliderSize.x + 2.0 * _trackbarSliderHorizontalPadding, yTop + (_elementHeight -_textSize) / 2.0);
    _labelText.setFillColor(_textColor);

    _renderWindow.draw(_uiBackgroundRectangle);
    _renderWindow.draw(_trackbarSliderRectangle);
    _renderWindow.draw(_trackbarButtonRectangle);

    _renderWindow.draw(_labelText);
}

void ConfigurationMenu::_drawUIElement(UIElement* uiElement) {
    if (std::holds_alternative<bool>(uiElement->value)) {  // ui element is bool (toggle)
        bool value = std::get<bool>(uiElement->value);

        _drawToggle(uiElement->label, uiElement->yTop, value);
    } else if (std::holds_alternative<float>(uiElement->value)) { // ui element is float (trackbar)
        float value = std::get<float>(uiElement->value);

        float trackbarPercentage = (value - uiElement->minVal) / (uiElement->maxVal - uiElement->minVal);
        _drawTrackbar(uiElement->label, uiElement->yTop, trackbarPercentage, value);
    }
}

ConfigurationMenu::~ConfigurationMenu(){
    for (UIElement* element : _uiElements) {
        delete element;
    }
}