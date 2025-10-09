#pragma once

#include <vector>
#include "size.h"
#include "offset.h"
#include "layer.h"

namespace Ciart::Studio {    
    class WorkspaceContext {
    public:
        WorkspaceContext(const Size& size = {100, 100}, const Offset& offset = {0, 0}, double angle = 0.0, double scale = 1.0, const std::vector<ILayer*>& layers = {})
            : size(size), offset(offset), angle(angle), scale(scale), layers(layers) {}
        
        const Size getSize() const { return size; }
        const Offset getOffset() const { return offset; }
        double getAngle() const { return angle; }
        double getScale() const { return scale; }
        const std::vector<ILayer*>& getLayers() const { return layers; }
        
        // 마우스 위치 관리
        void setMousePosition(const Offset& position) { mousePosition = position; hasValidMousePosition = true; }
        void clearMousePosition() { hasValidMousePosition = false; }
        const Offset& getMousePosition() const { return mousePosition; }
        bool hasMousePosition() const { return hasValidMousePosition; }

        void setSize(const Size& newSize) { size = newSize; }
        void setOffset(const Offset& newOffset) { offset = newOffset; }
        void setAngle(double newAngle) { angle = newAngle; }
        void setScale(double newScale) { scale = newScale; }
        void setLayers(const std::vector<ILayer*>& newLayers) { layers = newLayers; }
        
    private:
        Size size;
        Offset offset;
        double angle;
        double scale;
        std::vector<ILayer*> layers;
        
        // 마우스 위치 추적
        Offset mousePosition;
        bool hasValidMousePosition = false;
    };
}
