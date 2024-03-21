#pragma once

#include "phys/phys_general.h"

namespace pe_phys_vehicle {

    /// VehicleRaycaster is provides interface for between vehicle simulation and raycasting
    struct VehicleRaycaster
    {
        virtual ~VehicleRaycaster() {}
        struct VehicleRaycasterResult {
            VehicleRaycasterResult(): m_distFraction(pe::Real(-1.)) {};
            pe::Vector3 m_hitPointInWorld;
            pe::Vector3 m_hitNormalInWorld;
            pe::Real m_distFraction;
        };

        virtual void* castRay(uint32_t rigid_idx, const pe::Vector3& from, const pe::Vector3& direction, pe::Real length,
                              VehicleRaycasterResult& result) = 0;
    };

} // namespace pe_phys_vehicle
