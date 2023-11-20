#include "Demos/Common.h"

namespace sgl = SGL;
namespace demo = SGL::Demo;

int main() {
    //auto app = demo::HelloTriangle();
    //auto app = demo::GeometryShader();
    auto app = demo::NormalVector();
    app.run();

    return 0;
}
