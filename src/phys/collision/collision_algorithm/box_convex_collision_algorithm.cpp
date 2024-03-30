#include "box_convex_collision_algorithm.h"

#include "convex_convex_collision_algorithm.h"
#include "phys/shape/box_shape.h"
#include "phys/shape/convex_mesh_shape.h"
#include "phys/shape/default_mesh.h"

namespace pe_phys_collision {

    bool BoxConvexCollisionAlgorithm::processCollision(pe_phys_object::RigidBody* object_a,
                                                       pe_phys_object::RigidBody* object_b, ContactResult& result) {
        if (object_a->getCollisionShape()->getType() == pe_phys_shape::ShapeType::Box) {
            std::swap(object_a, object_b);
        }
        if (!(object_a->getCollisionShape()->getType() == pe_phys_shape::ShapeType::ConvexMesh &&
               object_b->getCollisionShape()->getType() == pe_phys_shape::ShapeType::Box)) {
            return false;
        }

        auto shape_a = (pe_phys_shape::ConvexMeshShape*)object_a->getCollisionShape();
        auto shape_b = (pe_phys_shape::BoxShape*)object_b->getCollisionShape();
        auto& mesh_a = shape_a->getMesh();
        auto& mesh_b = shape_b->getMesh();
        auto& transA = object_a->getTransform();
        auto& transB = object_b->getTransform();

        pe::Vector3 sep;
        pe::Real margin = 0.005;
        result.clearContactPoints();
        result.setObjects(object_a, object_b);

        VertexArray world_verts_b1;
        VertexArray world_verts_b2;

        if (!ConvexConvexCollisionAlgorithm::findSeparatingAxis(shape_a, shape_b,
                                                                mesh_a, mesh_b,
                                                                shape_a->getUniqueEdges(),
                                                                pe_phys_shape::_box_unique_edges,
                                                                transA, transB, sep, margin, result)) {
            return false;
        }
        ConvexConvexCollisionAlgorithm::clipHullAgainstHull(sep,
                                                            mesh_a, mesh_b, transA, transB,
                                                            PE_REAL_MIN, 0,
                                                            world_verts_b1, world_verts_b2,
                                                            margin, result);
        result.sortContactPoints();
        return result.getPointSize() > 0;
    }

} // pe_phys_collision