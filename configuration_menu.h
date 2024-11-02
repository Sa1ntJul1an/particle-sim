#ifndef CONFIGURATION_MENU_H
#define CONFIGURATION_MENU_H

#include <SFML/Graphics.hpp>
#include <string>
#include <variant>
#include <functional>
#include <sstream>
#include "particle_sim.h"


class ConfigurationMenu {
    public:
        ConfigurationMenu(sf::RenderWindow& renderWindow, sf::Font& font);

        ~ConfigurationMenu();

        void addTrackbar();
        void addTrackbar(void(*)(float));
        void addToggle(void(*)(bool));

        void evaluateMouseClick(sf::Vector2i);
        void drawUI();

    private:

        std::stringstream _stream;

        sf::Vector2f _trackbarSliderSize; 
        sf::Font _font;

        // CONSTANTS
        const sf::Color _textColor = sf::Color(200, 200, 200);
        const float _textSize = 15.0;

        const float _elementHeight = 50;
        const float _elementVerticalPadding = 5.0;
        const sf::Color _elementBackgroundColor = sf::Color(80, 80, 80);
        const sf::Color _trackbarButtonColor = sf::Color(200, 200, 200);
        const sf::Color _trackbarSliderColor = sf::Color::Black;
        const sf::Vector2f _trackbarButtonSize = sf::Vector2f(10, 20);
        const float _trackbarSliderHeight = 5.0;
        const float _trackbarSliderHorizontalPadding = 10.0;

        // SFML objects
        sf::RectangleShape _uiBackgroundRectangle;
        sf::RectangleShape _trackbarButtonRectangle;
        sf::RectangleShape _trackbarSliderRectangle;
        sf::Text _labelText;

        // Render window
        sf::RenderWindow& _renderWindow;

        float _windowWidth; 
        float _windowHeight;
        
        // UI elements
        struct UIElement {
            std::string label;
            float yTop;
            std::variant<float, bool> value;
            std::function<void(float)> trackbarCallback;
            std::function<void(bool)> toggleCallback;
            float minVal;
            float maxVal;
        };

        std::vector<UIElement*> _uiElements;

        // helper functions 
        void _drawToggle(std::string, float, bool);
        void _drawTrackbar(std::string, float, float, float);
        void _drawUIElement(UIElement* uiElement);
};


#endif // CONFIGURATION_MENU_H