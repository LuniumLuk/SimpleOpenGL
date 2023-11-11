#include "Apps/Common.h"
#include "Apps/Triangle.h"

namespace sgl = SGL;

int main() {
    auto app = DemoTriangle();
    app.run();

    return 0;
}
