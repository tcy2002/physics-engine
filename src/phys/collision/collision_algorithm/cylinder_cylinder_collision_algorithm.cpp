#include "cylinder_cylinder_collision_algorithm.h"
#include "phys/shape/cylinder_shape.h"
#include "phys/shape/default_mesh.h"
#include "convex_convex_collision_algorithm.h"

namespace pe_phys_collision {

    bool CylinderCylinderCollisionAlgorithm::processCollision(pe_phys_shape::Shape* shape_a, pe_phys_shape::Shape* shape_b,
                                                              pe::Transform trans_a, pe::Transform trans_b,
                                                              ContactResult& result) {
        if (!(shape_a->getType() == pe_phys_shape::ShapeType::Cylinder &&
              shape_b->getType() == pe_phys_shape::ShapeType::Cylinder)) {
            return false;
        }

        auto& mesh_a = ((pe_phys_shape::CylinderShape*)shape_a)->getMesh();
        auto& mesh_b = ((pe_phys_shape::CylinderShape*)shape_b)->getMesh();

        pe::Vector3 sep;
        pe::Real margin = PE_MARGIN;

        VertexArray world_verts_b1;
        VertexArray world_verts_b2;

        if (!ConvexConvexCollisionAlgorithm::findSeparatingAxis(shape_a, shape_b,
                                                                mesh_a, mesh_b,
                                                                pe_phys_shape::_cylinder_unique_edges,
                                                                pe_phys_shape::_cylinder_unique_edges,
                                                                trans_a, trans_b, sep, margin, result)) {
            return false;
        }
        ConvexConvexCollisionAlgorithm::clipHullAgainstHull(sep,
                                                            mesh_a, mesh_b, trans_a, trans_b,
                                                            PE_REAL_MIN, 0,
                                                            world_verts_b1, world_verts_b2,
                                                            margin, result);
        return result.getPointSize() > 0;
    }

} // pe_phys_collision