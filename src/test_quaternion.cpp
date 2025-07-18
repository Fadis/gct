#define BOOST_TEST_MAIN
#include <boost/test/included/unit_test.hpp>
#include <glm/vec4.hpp>
#include <glm/vec3.hpp>
#include <glm/mat3x3.hpp>
#include <glm/mat4x4.hpp>
#include <glm/gtc/quaternion.hpp>

using namespace glm;

#include <gct/shader/glsl/gct/quaternion.h>

BOOST_AUTO_TEST_CASE(test1)
{
    const double v1 = 1.23456e28;
    const double v2 = 1.23457e28;

    BOOST_CHECK_CLOSE(v1, v2, 0.00001);
}

