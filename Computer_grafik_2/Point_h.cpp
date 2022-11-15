#include <iostream>

class Point_h {
public:

    Point_h (float x=0.0, float y=0.0, float w=0.0) {

//        if ((this->w = 0)) {
//            throw std::invalid_argument("The w cannot be 0!");
//        } else {
            float xw = x/w;
            float yw = y/w;
            
            this->x = xw;
            this->y = yw;
            this->w = w;
//        }
    }

    float x, y, w;
};
