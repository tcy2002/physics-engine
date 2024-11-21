#pragma once

#include "collision_algorithm.h"

namespace pe_phys_collision {

    class BoxBoxCollisionAlgorithm : public CollisionAlgorithm {
    public:
        virtual bool processCollision(pe_phys_shape::Shape* shape_a, pe_phys_shape::Shape* shape_b,
                                      pe::Transform trans_a, pe::Transform trans_b,
                                      pe::Real refScale, ContactResult& result) override;

        static bool getClosestPoints(pe_phys_shape::Shape* shape_a, pe_phys_shape::Shape* shape_b,
                                     const pe::Transform& trans_a, const pe::Transform& trans_b,
                                     pe::Real margin, ContactResult& result);
    };

} // pe_phys_collision