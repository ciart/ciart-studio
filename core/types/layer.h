//
//  layer.h
//  CiartStudio
//
//  Created by Pdom on 6/29/25.
//

#pragma once

#include <string>

namespace Ciart::Studio {
    class ILayer {
    public:
        std::string name;
    };

    class BitmapLayer : public ILayer {
    public:
        BitmapLayer(std::string name) {
            this->name = name;
        }
    };
}
