#ifndef TRACKBAR_H
#define TRACKBAR_H

#include <string>

class Trackbar {
    public: 
        Trackbar(std::string, float, float);

        void HandleMouseClick();

    private:
        std::string _valueName;

        float _minValue;
        float _maxValue;
};


#endif // TRACKBAR_H