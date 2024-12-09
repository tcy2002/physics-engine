#include "primal_dual_solver.h"
#include "utils/thread_pool.h"

namespace pe_phys_constraint {

    PrimalDualSolver::PrimalDualSolver() {
        _iteration = 30;
    }

    void PrimalDualSolver::setupSolver(
            pe::Real dt, const pe::Vector3& gravity,
            const pe::Array<pe_phys_object::RigidBody *> &objects,
            const pe::Array<pe_phys_collision::ContactResult *> &contact_results,
            const pe::Array<Constraint *> &constraints) {
        _collision_objects = objects;
        for (const auto co : _collision_objects) {
            co->clearTempVelocity();
            co->setTempLinearVelocity(co->getLinearVelocity() + gravity * dt);
        }

        _param.dt = dt;
        _param.gravity = gravity;
        _fcc_constraint.setObjects(&_collision_objects);
        _fcc_constraint.setContactResults(&contact_results);
        _fcc_constraint.initPrimalDual(_param);
    }

    void PrimalDualSolver::solve() {
        // solve contact constraints
        pe::LDLT ldlt;
        pe::CG cg;
        pe::Real hu, exit_err, tol = PE_R(1e-8);
        pe::VectorX du, df, dl, ru, ru_add, rf, wrf, rl;
        _fcc_constraint.iteratePrimalDual(_iteration, ldlt, cg, hu, du, df, dl, ru, ru_add, rf, wrf, rl, exit_err, tol);

        // sync velocity
        for (const auto co : _collision_objects) {
            co->syncTempVelocity();
        }
    }

} // namespace pe_phys_constraint