#include "intf/world.h"
#include "phys/shape/box_shape.h"
#include "phys/shape/convex_mesh_shape.h"
#include "phys/shape/sphere_shape.h"
#include "phys/shape/cylinder_shape.h"
#include "phys/fracture/fracture_solver/fracture_solver.h"
#include "phys/object/fracturable_object.h"
#include "intf/viewer.h"
#include <fstream>

//#define PE_TEST_SINGLE
#define PE_TEST_FRAC
//#define PE_TEST_SECOND_GROUND
#define PE_TEST_OBJ_NUM 99
#define PE_TEST_FRAME_TH 1000000
//#define PE_TEST_FRAMERATE 1000

void objToMesh(pe::Mesh& mesh, const std::string &filename) {
    std::ifstream file(filename);
    if (!file.is_open()) {
        std::cerr << "Failed to open file " << filename << std::endl;
        return;
    }

    std::string c;
    while (file >> c) {
        if (c == "v") {
            pe::Real x, y, z;
            file >> x >> y >> z;
            mesh.vertices.push_back({{x, y, z}, {0, 0, 0}});
        } else if (c == "f") {
            int i;
            pe::Mesh::Face face;
            while (file >> i) {
                face.indices.push_back(i - 1);
            }
            mesh.faces.push_back(face);
            file.clear();
        }
    }

    pe::Mesh::perFaceNormal(mesh);
    for (auto& face : mesh.faces) {
        for (auto i : face.indices) {
            mesh.vertices[i].normal = face.normal;
        }
    }

    file.close();
}

pe_phys_object::RigidBody* createMeshRigidBody(const pe::Vector3& pos, const pe::Vector3& size, pe::Real mass,
                                               const std::string& filename) {
    auto rb = new pe_phys_object::RigidBody();
    rb->setMass(mass);
    rb->setTransform(pe::Transform(pe::Matrix3::identity(), pos));
    rb->setFrictionCoeff(0.5);
    rb->setRestitutionCoeff(0.5);
    rb->setAngularDamping(0.8);
    pe::Mesh mesh;
    objToMesh(mesh, filename);
    auto shape = new pe_phys_shape::ConvexMeshShape();
    shape->setMesh(mesh);
    rb->setCollisionShape(shape);
    rb->setLocalInertia(shape->calcLocalInertia(mass));
    return rb;
}

pe_phys_object::RigidBody* createBoxRigidBody(const pe::Vector3& pos, const pe::Vector3& size, pe::Real mass) {
    auto rb = new pe_phys_object::RigidBody();
    rb->setMass(mass);
    auto shape = new pe_phys_shape::BoxShape(size);
    rb->setCollisionShape(shape);
    rb->setTransform(pe::Transform(pe::Matrix3::identity(), pos));
    rb->setLocalInertia(shape->calcLocalInertia(mass));
    rb->setFrictionCoeff(0.5);
    rb->setRestitutionCoeff(0.5);
    rb->setAngularDamping(0.8);
    return rb;
}

pe_phys_object::RigidBody* createSphereRigidBody(const pe::Vector3& pos, pe::Real radius, pe::Real mass) {
    auto rb = new pe_phys_object::RigidBody();
    rb->setMass(mass);
    auto shape = new pe_phys_shape::SphereShape(radius);
    rb->setCollisionShape(shape);
    rb->setTransform(pe::Transform(pe::Matrix3::identity(), pos));
    rb->setLocalInertia(shape->calcLocalInertia(mass));
    rb->setFrictionCoeff(0.5);
    rb->setRestitutionCoeff(0.5);
    rb->setAngularDamping(0.8);
    return rb;
}

pe_phys_object::RigidBody* createCylinderRigidBody(const pe::Vector3& pos, pe::Real radius, pe::Real height, pe::Real mass) {
    auto rb = new pe_phys_object::RigidBody();
    rb->setMass(mass);
    auto shape = new pe_phys_shape::CylinderShape(radius, height);
    rb->setCollisionShape(shape);
    rb->setTransform(pe::Transform(pe::Matrix3::identity(), pos));
    rb->setLocalInertia(shape->calcLocalInertia(mass));
    rb->setFrictionCoeff(0.5);
    rb->setRestitutionCoeff(0.5);
    rb->setAngularDamping(0.8);
    return rb;
}

pe_phys_object::FracturableObject* createFracturableObject(const pe::Vector3& pos, const pe::Vector3& size, pe::Real th) {
    auto rb = new pe_phys_object::FracturableObject();
    rb->setMass(1.0);
    auto shape = new pe_phys_shape::BoxShape(size);
    rb->setCollisionShape(shape);
    rb->setTransform(pe::Transform(pe::Matrix3::identity(), pos));
    rb->setLocalInertia(shape->calcLocalInertia(1.0));
    rb->setFrictionCoeff(0.5);
    rb->setRestitutionCoeff(0.5);
    rb->setAngularDamping(0.8);
    rb->setThreshold(th);
    return rb;
}

void testWorld() {
    auto world = new pe_intf::World();
    world->setDt(0.01);
    world->setGravity(pe::Vector3(0, -9.8, 0));

    // open viewer
    pe_intf::Viewer::open("WorldTest", 800, 600,
                          {0, 10, 20}, 0, (float)(PE_PI / 6.0));

    // create rigid bodies
    auto rb1 = createBoxRigidBody(pe::Vector3(0, -0.5, 0), pe::Vector3(1000, 1, 1000), 8);
    rb1->setKinematic(true);
#ifdef PE_TEST_SINGLE
    const auto filename = CURRENT_TEST_SOURCE_DIR "/test.obj";
    auto rb2 = createMeshRigidBody(pe::Vector3(0, 5, 0), pe::Vector3(1, 1, 1), 1.0, filename);
    pe::Transform trans, trans2;
    trans.setOrigin(rb2->getTransform().getOrigin());
    trans.setRotation({0, 0, 1}, -0.90);
    trans2.setRotation({1, 0, 0}, 0.05);
    trans.setBasis(trans2.getBasis() * trans.getBasis());
    rb2->setTransform(trans);
#endif
#ifdef PE_TEST_FRAC
    auto rb3 = createFracturableObject(pe::Vector3(0, 5, 0), pe::Vector3(4, 4, 4), 1);
    auto fs = new pe_phys_fracture::FractureSolver();
    pe_phys_fracture::FractureSource src;
    src.type = pe_phys_fracture::FractureType::Sphere;
    src.position = pe::Vector3(1.5, 6.5, 1.5);
    src.intensity = pe::Vector3(0.5, 0.5, 0.5);
    fs->setFracturableObject(rb3);
    fs->solve({src});
#endif
#ifdef PE_TEST_SECOND_GROUND
    auto rb4 = createRigidBody(pe::Vector3(0, 1.5, 0), pe::Vector3(12, 1, 12), 2);
#endif

    // add to world
    world->addRigidBody(rb1);
#ifdef PE_TEST_SINGLE
    world->addRigidBody(rb2);
#endif
    pe::Array<pe_phys_object::RigidBody*> rbs;
    for (int i = 0; i < PE_TEST_OBJ_NUM; i++) {
        pe_phys_object::RigidBody* rb;
        if (i % 3 == 0) {
            rb = createBoxRigidBody(pe::Vector3(0, 10 + i * 1.1, 0), pe::Vector3(1, 1, 1), 1.0);
        } else if (i % 3 == 1) {
            rb = createSphereRigidBody(pe::Vector3(0, 10 + i * 1.1, 0), 0.5, 1.0);
        } else {
            rb = createCylinderRigidBody(pe::Vector3(0, 10 + i * 1.1, 0), 0.5, 1.0, 1.0);
        }
        rbs.push_back(rb);
        world->addRigidBody(rb);
    }
#ifdef PE_TEST_FRAC
    for (auto rb : fs->getFragments()) {
        rbs.push_back(rb);
        world->addRigidBody(rb);
    }
#endif
#ifdef PE_TEST_SECOND_GROUND
    world->addRigidBody(rb4);
#endif

    // add to viewer
    int id1 = pe_intf::Viewer::addCube(((pe_phys_shape::BoxShape*)rb1->getCollisionShape())->getSize());
    pe_intf::Viewer::updateCubeColor(id1, pe::Vector3(0.3, 0.3, 0.8));
    pe_intf::Viewer::updateCubeTransform(id1, rb1->getTransform());
#ifdef PE_TEST_SINGLE
    int id2 = pe_core::Viewer::addMesh(((pe_phys_shape::ConvexMeshShape*)(rb2->getCollisionShape()))->getMesh());
    pe_core::Viewer::updateMeshColor(id2, pe::Vector3(0.3, 0.8, 0.3));
    pe_core::Viewer::updateMeshTransform(id2, rb2->getTransform());
#endif
    pe::Array<int> ids;
    for (int i = 0; i < PE_TEST_OBJ_NUM; i++) {
        int id;
        if (i % 3 == 0) {
            id = pe_intf::Viewer::addCube(pe::Vector3(1, 1, 1));
            pe_intf::Viewer::updateCubeColor(id, pe::Vector3(0.8, 0.3, 0.3));
            pe_intf::Viewer::updateCubeTransform(id, rbs[i]->getTransform());
        } else if (i % 3 == 1) {
            id = pe_intf::Viewer::addSphere(0.5);
            pe_intf::Viewer::updateSphereColor(id, pe::Vector3(0.8, 0.3, 0.3));
            pe_intf::Viewer::updateSphereTransform(id, rbs[i]->getTransform());
        } else {
            id = pe_intf::Viewer::addCylinder(0.5, 1);
            pe_intf::Viewer::updateCylinderColor(id, pe::Vector3(0.8, 0.3, 0.3));
            pe_intf::Viewer::updateCylinderTransform(id, rbs[i]->getTransform());
        }
        ids.push_back(id);
    }
#ifdef PE_TEST_FRAC
    for (auto rb : fs->getFragments()) {
        auto shape = (pe_phys_shape::ConvexMeshShape*)(rb->getCollisionShape());
        auto id = pe_intf::Viewer::addMesh(shape->getMesh());
        pe_intf::Viewer::updateMeshColor(id, pe::Vector3(0.3, 0.8, 0.3));
        pe_intf::Viewer::updateMeshTransform(id, rb->getTransform());
        ids.push_back(id);
    }
#endif
#ifdef PE_TEST_SECOND_GROUND
    int id4 = pe_core::Viewer::addCube(pe::Vector3(12, 1, 12));
    pe_core::Viewer::updateCubeColor(id4, pe::Vector3(0.3, 0.3, 0.8));
    pe_core::Viewer::updateCubeTransform(id4, rb4->getTransform());
#endif

    // main loop
    int frame = 0;
    auto start = COMMON_GetTickCount();
    while (true) {
#   ifdef PE_TEST_FRAMERATE
        if (++frame > PE_TEST_FRAMERATE) break;
#   else
        while (pe_intf::Viewer::getKeyState('r') != 0) {
            if (pe_intf::Viewer::getKeyState(27) == 0) goto ret;
        }
#   endif
        auto t = COMMON_GetTickCount();
#   ifdef PE_TEST_SINGLE
        pe_core::Viewer::updateCubeTransform(id2, rb2->getTransform());
        pe_core::Viewer::updateMeshTransform(id2, rb2->getTransform());
#   endif
#   ifdef PE_TEST_SECOND_GROUND
        pe_core::Viewer::updateCubeTransform(id4, rb4->getTransform());
#   endif
        for (int i = 0; i < ids.size(); i++) {
            pe_intf::Viewer::updateCubeTransform(ids[i], rbs[i]->getTransform());
            pe_intf::Viewer::updateMeshTransform(ids[i], rbs[i]->getTransform());
            pe_intf::Viewer::updateSphereTransform(ids[i], rbs[i]->getTransform());
            pe_intf::Viewer::updateCylinderTransform(ids[i], rbs[i]->getTransform());
        }
        world->step();
#   if !defined(PE_TEST_FRAMERATE)
        if (++frame > PE_TEST_FRAME_TH) while (pe_intf::Viewer::getKeyState('r') != 1);
        COMMON_Sleep(10 - (int)(COMMON_GetTickCount() - t));
#   endif
    }

    ret:
    auto end = COMMON_GetTickCount();
#   ifdef PE_TEST_FRAMERATE
    std::cout << "FPS: " << (pe::Real)frame / ((pe::Real)(end - start) / 1000.0) << std::endl;
#   endif
    pe_intf::Viewer::close();
    delete world;
}

int main() {
    testWorld();
}