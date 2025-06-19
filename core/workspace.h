#pragma once

#include "type.h"

namespace Ciart {
    namespace Studio {
        class Workspace {
        public:
            Workspace() {}
            
            const Size& getSize() const { return size; }
            const Offset& getOffset() const { return offset; }
            double getAngle() const { return angle; }
            double getScale() const { return scale; }
            
            void setSize(const Size& size) { this->size = size; }
            void setOffset(const Offset& offset) { this->offset = offset; }
            void setAngle(double angle) { this->angle = angle; }
            void setScale(double scale) { this->scale = scale; }

            void move(double dx, double dy);
            void room(double magnification);
        private:
            Size size {100, 100};
            Offset offset {0, 0};
            double angle = 0.0;
            double scale = 1.0;
        };
    }
}
