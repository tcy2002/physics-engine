#pragma once

#include <common/vector3.h>
#include <common/matrix3x3.h>

#ifdef PE_USE_DOUBLE
#define JE_REAL double
#else
#define JE_REAL float
#endif

namespace utils {
    
    class JacobianEntry {
    public:
        JacobianEntry() {}
        //constraint between two different rigidbodies
        JacobianEntry(
                const common::Matrix3x3<JE_REAL>& world2A,
                const common::Matrix3x3<JE_REAL>& world2B,
                const common::Vector3<JE_REAL>& rel_pos1, const common::Vector3<JE_REAL>& rel_pos2,
                const common::Vector3<JE_REAL>& jointAxis,
                const common::Vector3<JE_REAL>& inertiaInvA,
                const JE_REAL massInvA,
                const common::Vector3<JE_REAL>& inertiaInvB,
                const JE_REAL massInvB) : m_linearJointAxis(jointAxis) {
            m_aJ = world2A * (rel_pos1.cross(m_linearJointAxis));
            m_bJ = world2B * (rel_pos2.cross(-m_linearJointAxis));
            m_0MinvJt = inertiaInvA * m_aJ;
            m_1MinvJt = inertiaInvB * m_bJ;
            m_Adiag = massInvA + m_0MinvJt.dot(m_aJ) + massInvB + m_1MinvJt.dot(m_bJ);
        }

        JE_REAL getDiagonal() const { return m_Adiag; }

        // for two constraints on the same rigidbody (for example vehicle friction)
        JE_REAL getNonDiagonal(const JacobianEntry& jacB, const JE_REAL massInvA) const
        {
            const JacobianEntry& jacA = *this;
            JE_REAL lin = massInvA * jacA.m_linearJointAxis.dot(jacB.m_linearJointAxis);
            JE_REAL ang = jacA.m_0MinvJt.dot(jacB.m_aJ);
            return lin + ang;
        }

        // for two constraints on sharing two same rigidbodies (for example two contact points between two rigidbodies)
        JE_REAL getNonDiagonal(const JacobianEntry& jacB, const JE_REAL massInvA, const JE_REAL massInvB) const
        {
            const JacobianEntry& jacA = *this;
            common::Vector3<JE_REAL> lin = jacA.m_linearJointAxis * jacB.m_linearJointAxis;
            common::Vector3<JE_REAL> ang0 = jacA.m_0MinvJt * jacB.m_aJ;
            common::Vector3<JE_REAL> ang1 = jacA.m_1MinvJt * jacB.m_bJ;
            common::Vector3<JE_REAL> lin0 = massInvA * lin;
            common::Vector3<JE_REAL> lin1 = massInvB * lin;
            common::Vector3<JE_REAL> sum = ang0 + ang1 + lin0 + lin1;
            return sum[0] + sum[1] + sum[2];
        }

        JE_REAL getRelativeVelocity(const common::Vector3<JE_REAL>& linvelA, const common::Vector3<JE_REAL>& angvelA, const common::Vector3<JE_REAL>& linvelB, const common::Vector3<JE_REAL>& angvelB)
        {
            common::Vector3<JE_REAL> linrel = linvelA - linvelB;
            common::Vector3<JE_REAL> angvela = angvelA * m_aJ;
            common::Vector3<JE_REAL> angvelb = angvelB * m_bJ;
            linrel = linrel * m_linearJointAxis;
            angvela += angvelb;
            angvela += linrel;
            JE_REAL rel_vel2 = angvela[0] + angvela[1] + angvela[2];
            return rel_vel2 + 1e-7f;
        }

    private:
        common::Vector3<JE_REAL> m_linearJointAxis;
        common::Vector3<JE_REAL> m_aJ;
        common::Vector3<JE_REAL> m_bJ;
        common::Vector3<JE_REAL> m_0MinvJt;
        common::Vector3<JE_REAL> m_1MinvJt;
        //Optimization: can be stored in the w/last component of one of the vectors
        JE_REAL m_Adiag;
    };
    
} // namespace utils