#pragma once

#include "phys/phys_general.h"
#include "phys/object/rigidbody.h"

namespace pe_phys_constraint {

    struct ConstraintParam {
        pe::Real dt = 0.01;
        pe::Real restitutionVelocityThreshold = 0.1;
        pe::Real penetrationThreshold = 0.3;
        pe::Real kerp = 0.2;
        int splitPenetrationConstraintFlag = 0;
    };

    class Constraint {
        COMMON_MEMBER_PTR_SET_GET(pe_phys_object::RigidBody, body_a, BodyA)
        COMMON_MEMBER_PTR_SET_GET(pe_phys_object::RigidBody, body_b, BodyB)

    public:
        Constraint(): _body_a(nullptr), _body_b(nullptr) {}
        virtual ~Constraint() {}

        // projected gauss seidel
        virtual void initSequentialImpulse(const ConstraintParam& param) = 0;
        virtual void iterateSequentialImpulse(int iter) = 0;
        virtual void iterateSequentialImpulseForPenetration(int iter) = 0;
        virtual void afterSequentialImpulse() = 0;
        virtual void warmStart() = 0;

    };

} // namespace pe_phys_constraint