#include "configuration_menu.h"
#include <SFML/Graphics.hpp>
#include <SFML/System/Vector2.hpp>
#include <functional>
#include <iostream>
#include <string>
#include <variant>

#include "uiElements.h"


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
    _uiBackgroundRectangle.setSize(sf::Vector2f(_windowWidth, _defaultElementHeight));

    _trackbarButtonRectangle.setFillColor(_trackbarButtonColor);
    _trackbarButtonRectangle.setSize(_trackbarButtonSize);

    _trackbarSliderRectangle.setFillColor(_trackbarSliderColor);
    _trackbarSliderRectangle.setSize(_trackbarSliderSize);

    _toggleBoxOutlineRectangle.setFillColor(_toggleBoxOutlineColor);
    _toggleBoxOutlineRectangle.setSize(_toggleOutlineSize);
    _toggleBoxRectangle.setSize(_toggleBoxSize);
}

void ConfigurationMenu::addToggle(std::function<void(bool)> callback, std::string label, bool initValue) {
    UIElement * element = new UIElement; 

    element->elementType = uiElements::Toggle;
    element->label = label;
    element->value = initValue;

    element->yTop = _getBottomUIElementY() + _elementVerticalPadding;
    element->height = _defaultElementHeight;

    element->toggleCallback = callback;

    _uiElements.push_back(element);
}

void ConfigurationMenu::addTrackbar(std::function<void(double)> callback, std::string label, double minVal, double maxVal, double initValue) {
    UIElement * element = new UIElement; 

    element->elementType = uiElements::Trackbar;
    element->minVal = minVal;
    element->maxVal = maxVal;
    element->label = label;
    element->value = initValue;

    element->yTop = _getBottomUIElementY() + _elementVerticalPadding;
    element->height = _defaultElementHeight;

    element->trackbarCallback = callback;

    _uiElements.push_back(element);
}

int ConfigurationMenu::_getBottomUIElementY() {
  int maxY = 0;

  if (_uiElements.size() > 0) {
    for (UIElement * element : _uiElements) {
      int currentY = element->yTop + element->height;
      if (currentY > maxY) {
        maxY = currentY;
      }
    }
  }
  
  return maxY;
}

void ConfigurationMenu::evaluateMouseClick(sf::Vector2i mousePosition) {
    // std::cout << "mouse click in menu at y = " << mousePosition.y << std::endl;
    for (UIElement* element : _uiElements) {
        // check if mouse click is within UI element
        if (mousePosition.y > element->yTop && mousePosition.y < element->height + element->yTop) {
            switch (element->elementType) {
              case uiElements::Trackbar: {
                float activationPercent = (mousePosition.x - _trackbarSliderHorizontalPadding) / (_trackbarSliderSize.x - _trackbarSliderHorizontalPadding);

                if (activationPercent > 1.0) { 
                    activationPercent = 1.0;
                } else if (activationPercent < 0.0) {
                    activationPercent = 0.0;
                }

                double elementValue = activationPercent * (element->maxVal - element->minVal) + element->minVal;

                element->value = elementValue;

                element->trackbarCallback(elementValue);
                break;
              }
              case uiElements::Toggle: {
                if (_toggleBoxRectangle.getGlobalBounds().contains(mousePosition.x, mousePosition.y)) {
                  bool elementValue = std::get<bool>(element->value);
                  element->value = !elementValue;
                  element->toggleCallback(elementValue);
                }

                break;
              }
              default:
                return;
            }
        }
    }
}

void ConfigurationMenu::drawUI() {
    for (UIElement* element : _uiElements) {
        _drawUIElement(element);
    }
}

void ConfigurationMenu::_drawToggle(std::string label, float yTop, float elementHeight, bool value) {
    _toggleBoxOutlineRectangle.setPosition(sf::Vector2f(_toggleBoxRectangleHorizontalPadding, yTop + (elementHeight / 2.0) - (_toggleBoxSize.y / 2.0)));
    float toggleBoxOutlineThickness = (_toggleOutlineSize.x - _toggleBoxSize.x) / 2.0;
    _toggleBoxOutlineRectangle.setPosition(sf::Vector2f(_toggleBoxRectangleHorizontalPadding + toggleBoxOutlineThickness, yTop + (elementHeight / 2.0) - (_toggleBoxSize.y / 2.0)));
    _toggleBoxRectangle.setPosition(sf::Vector2f(_toggleBoxOutlineRectangle.getPosition().x + toggleBoxOutlineThickness, _toggleBoxOutlineRectangle.getPosition().y + toggleBoxOutlineThickness));

    _stream.str(std::string());
    
    if (value) {
      _toggleBoxRectangle.setFillColor(_toggleBoxSelectedColor);
      _stream << label << ": " << "enabled";
    } else {
      _toggleBoxRectangle.setFillColor(_toggleBoxUnselectedColor);
      _stream << label << ": " << "disabled";
    }

    _labelText.setString(_stream.str());
    _labelText.setCharacterSize(_textSize);
    _labelText.setPosition(_toggleBoxSize.x + 2.0 * _toggleBoxRectangleHorizontalPadding, yTop + (elementHeight - _labelText.getGlobalBounds().height) / 2.0);
    _labelText.setFillColor(_textColor);
    
    _renderWindow.draw(_toggleBoxOutlineRectangle);
    _renderWindow.draw(_toggleBoxRectangle);

    _renderWindow.draw(_labelText);
}

void ConfigurationMenu::_drawTrackbar(std::string label, float yTop, float elementHeight, float trackbarPercentage, double value) {
    _trackbarSliderRectangle.setPosition(sf::Vector2f(_trackbarSliderHorizontalPadding, yTop + (elementHeight / 2.0) - (_trackbarSliderHeight / 2.0)));
    float trackbarButtonPositionX = trackbarPercentage * _trackbarSliderRectangle.getSize().x + _trackbarSliderHorizontalPadding - _trackbarButtonSize.x / 2.0;
    float trackbarButtonPositionY = _trackbarSliderRectangle.getPosition().y - _trackbarButtonSize.y / 2.0 + _trackbarSliderSize.y / 2.0;
    _trackbarButtonRectangle.setPosition(sf::Vector2f(trackbarButtonPositionX, trackbarButtonPositionY));

    _stream.str(std::string());

    if (value != 0.0 && (value < 0.01 || value > 1000.0)) {     // format number with scientific notation at extremes 
      _stream << label << ": " << std::scientific << value;
    } else {
      _stream << label << ": " << std::defaultfloat << value;
    }

    _labelText.setString(_stream.str());
    _labelText.setCharacterSize(_textSize);
    _labelText.setPosition(_trackbarSliderSize.x + 2.0 * _trackbarSliderHorizontalPadding, yTop + (elementHeight - _labelText.getGlobalBounds().height) / 2.0);
    _labelText.setFillColor(_textColor);

    _renderWindow.draw(_trackbarSliderRectangle);
    _renderWindow.draw(_trackbarButtonRectangle);

    _renderWindow.draw(_labelText);
}

void ConfigurationMenu::_drawUIElement(UIElement* uiElement) {
    _uiBackgroundRectangle.setSize(sf::Vector2f(_windowWidth, uiElement->height));
    _uiBackgroundRectangle.setPosition(sf::Vector2f(0, uiElement->yTop));
    _renderWindow.draw(_uiBackgroundRectangle);

    uiElements elementType = uiElement->elementType;

    switch (elementType) {
        case uiElements::Trackbar: { // ui element is float (trackbar)
            double value = std::get<double>(uiElement->value);
            float trackbarPercentage = (value - uiElement->minVal) / (uiElement->maxVal - uiElement->minVal);
            _drawTrackbar(uiElement->label, uiElement->yTop, uiElement->height, trackbarPercentage, value);
            break;
        }
        case uiElements::Toggle: {  // ui element is bool (toggle)
            _drawToggle(uiElement->label, uiElement->yTop, uiElement->height, std::get<bool>(uiElement->value));
            break;
        }
        case uiElements::ParticleSpawn: {
            // TODO: particle spawn menu
            break;
        }
        
        default:
            break;
    }
}

ConfigurationMenu::~ConfigurationMenu(){
    for (UIElement* element : _uiElements) {
        delete element;
    }
}
