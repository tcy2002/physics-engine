#include <cassert>
#include "common/vector3.h"
#include "eigen_std.h"
#include "test_def.h"

using namespace pe_common;

void testConstruct() {
    PEReal n[] = {randR(), randR(), randR()};
    Vector3 a, b(n[0], n[1], n[2]);
    Vector3Std ae(0, 0, 0), be(n[0], n[1], n[2]);

    ASSERT_VECTOR3_EIGEN_EQUAL(a, ae)
    ASSERT_VECTOR3_EIGEN_EQUAL(b, be)

    ASSERT_VECTOR3_EIGEN_EQUAL(Vector3::zeros(), Vector3Std::Zero())
    ASSERT_VECTOR3_EIGEN_EQUAL(Vector3::ones(), Vector3Std::Ones())
    ASSERT_VECTOR3_EIGEN_EQUAL(Vector3::forward(), Vector3Std::UnitZ())
    ASSERT_VECTOR3_EIGEN_EQUAL(Vector3::up(), Vector3Std::UnitY())
    ASSERT_VECTOR3_EIGEN_EQUAL(Vector3::right(), Vector3Std::UnitX())
}

void testOperator() {
    PEReal n[] = {randPos(), randNeg(), randR(), randR(), randR(), randR()};
    Vector3 a, b(n[0], n[1], n[2]), c(n[3], n[4], n[5]);
    Vector3Std ae(0, 0 ,0), be(n[0], n[1], n[2]), ce(n[3], n[4], n[5]);

    ASSERT_VECTOR3_EIGEN_EQUAL(a + b, ae + be)
    ASSERT_VECTOR3_EIGEN_EQUAL(a - b, ae - be)
    ASSERT_VECTOR3_EIGEN_EQUAL(c + b, ce + be)
    ASSERT_VECTOR3_EIGEN_EQUAL(c - b, ce - be)

    ASSERT_VECTOR3_EIGEN_EQUAL(a * n[0], ae * n[0])
    ASSERT_VECTOR3_EIGEN_EQUAL(b * n[0], be * n[0])
    ASSERT_VECTOR3_EIGEN_EQUAL(a * n[1], ae * n[1])
    ASSERT_VECTOR3_EIGEN_EQUAL(b * n[1], be * n[1])

    ASSERT_VECTOR3_EIGEN_EQUAL(a / n[0], ae / n[0])
    ASSERT_VECTOR3_EIGEN_EQUAL(b / n[0], be / n[0])
    ASSERT_VECTOR3_EIGEN_EQUAL(a / n[1], ae / n[1])
    ASSERT_VECTOR3_EIGEN_EQUAL(b / n[1], be / n[1])

    ASSERT_VECTOR3_EIGEN_EQUAL(a += b, ae += be)
    ASSERT_VECTOR3_EIGEN_EQUAL(a -= b, ae -= be)
    ASSERT_VECTOR3_EIGEN_EQUAL(c += b, ce += be)
    ASSERT_VECTOR3_EIGEN_EQUAL(c -= b, ce -= be)

    ASSERT_VECTOR3_EIGEN_EQUAL(a *= n[0], ae *= n[0])
    ASSERT_VECTOR3_EIGEN_EQUAL(b *= n[0], be *= n[0])
    ASSERT_VECTOR3_EIGEN_EQUAL(a *= n[1], ae *= n[1])
    ASSERT_VECTOR3_EIGEN_EQUAL(b *= n[1], be *= n[1])

    ASSERT_VECTOR3_EIGEN_EQUAL(a /= n[0], ae /= n[0])
    ASSERT_VECTOR3_EIGEN_EQUAL(b /= n[0], be /= n[0])
    ASSERT_VECTOR3_EIGEN_EQUAL(a /= n[1], ae /= n[1])
    ASSERT_VECTOR3_EIGEN_EQUAL(b /= n[1], be /= n[1])

    ASSERT_VECTOR3_EIGEN_EQUAL(-a, -ae)
    ASSERT_VECTOR3_EIGEN_EQUAL(-b, -be)
}

void testMath() {
    PEReal n[] = {randPos(), randNeg(), randR(), randR(), randR(), randR()};
    Vector3 a, b(n[0], n[1], n[2]), c(n[3], n[4], n[5]);
    Vector3Std ae(0, 0 ,0), be(n[0], n[1], n[2]), ce(n[3], n[4], n[5]);

    ASSERT_EQUAL(a.norm(), ae.norm())
    ASSERT_EQUAL(b.norm(), be.norm())

    ASSERT_VECTOR3_EIGEN_EQUAL(b.normalized(), be.normalized())
    c.normalize(), ce.normalize();
    ASSERT_VECTOR3_EIGEN_EQUAL(c, ce)

    ASSERT_EQUAL(a.dot(b), ae.dot(be))
    ASSERT_EQUAL(c.dot(b), ce.dot(be))
    ASSERT_VECTOR3_EIGEN_EQUAL(a.cross(b), ae.cross(be))
    ASSERT_VECTOR3_EIGEN_EQUAL(c.cross(b), ce.cross(be))
}

void testGeometry() {
    Vector3 a, b(3, 5, 4), c(0, 1, 0);

    Vector3 a_p_c = a.project(c);
    Vector3Std ae(0, 0, 0);
    ASSERT_EQUAL(a_p_c.x, 0)
    ASSERT_EQUAL(a_p_c.y, 0)
    ASSERT_EQUAL(a_p_c.z, 0)

    Vector3 b_p_c = b.project(c);
    ASSERT_EQUAL(b_p_c.x, 0)
    ASSERT_EQUAL(b_p_c.y, 5)
    ASSERT_EQUAL(b_p_c.z, 0)

    Vector3 a_r_c = a.reflect(c);
    ASSERT_EQUAL(a_r_c.x, 0)
    ASSERT_EQUAL(a_r_c.y, 0)
    ASSERT_EQUAL(a_r_c.z, 0)

    Vector3 b_r_c = b.reflect(c);
    ASSERT_EQUAL(b_r_c.x, -3)
    ASSERT_EQUAL(b_r_c.y, 5)
    ASSERT_EQUAL(b_r_c.z, -4)

    Vector3 a_ro_c = a.rotate(c, M_PI / 2);
    ASSERT_EQUAL(a_ro_c.x, 0)
    ASSERT_EQUAL(a_ro_c.y, 0)
    ASSERT_EQUAL(a_ro_c.z, 0)

    Vector3 b_ro_c = b.rotate(c, M_PI / 2);
    ASSERT_EQUAL(b_ro_c.x, 4)
    ASSERT_EQUAL(b_ro_c.y, 5)
    ASSERT_EQUAL(b_ro_c.z, -3)

    PEReal angle = b.angle(b);
    ASSERT_EQUAL(angle, 0)
    angle = b.angle(c);
    ASSERT_EQUAL(angle, M_PI / 4)
}

int main() {
    randInit();
    for (int i = 0; i < 10; i++) {
        testConstruct();
        testOperator();
        testMath();
    }
    testGeometry();
    Eigen::Vector3d a(1, 2, 3), b(4, 5, 6);

    return 0;
}