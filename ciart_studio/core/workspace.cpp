//
// Created by Pdom on 10/9/24.
//

#include "workspace.h"

using namespace Ciart::Studio;

Workspace::Workspace(int x) {
    this->x = x;
}

int Workspace::print() {
    return this->x;
}