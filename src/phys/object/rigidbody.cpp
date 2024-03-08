#include <algorithm>
#include "rigidbody.h"

namespace pe_phys_object {

    std::atomic<uint32_t> RigidBody::_globalIdCounter(0);

    void RigidBody::setMass(pe::Real mass) {
        _mass = mass;
        _inv_mass = mass > 0 ? 1.0 / mass : 0;
    }

    void RigidBody::setLocalInertia(const pe::Matrix3& local_inertia) {
        _local_inertia = local_inertia;
        _local_inv_inertia = local_inertia.inverse();
        updateWorldInertia();
    }

    void RigidBody::updateWorldInertia() {
        _world_inertia = _transform.getBasis() * _local_inertia * _transform.getBasis().transposed();
        _world_inv_inertia = _transform.getBasis() * _local_inv_inertia * _transform.getBasis().transposed();
    }

    void RigidBody::setTransform(const pe::Transform &transform) {
        _transform = transform;
        updateWorldInertia();
    }

    RigidBody::RigidBody():
            _global_id(++_globalIdCounter),
            _kinematic(false),
            _collision_shape(nullptr),
            _mass(1.),
            _inv_mass(1.),
            _local_inertia(pe::Matrix3::identity()),
            _local_inv_inertia(pe::Matrix3::identity()),
            _world_inertia(pe::Matrix3::identity()),
            _world_inv_inertia(pe::Matrix3::identity()),
            _friction_coeff(0.5),
            _restitution_coeff(0.5),
            _linear_damping(0.),
            _angular_damping(0.),
            _transform(pe::Transform::identity()),
            _linear_velocity(pe::Vector3::zeros()),
            _angular_velocity(pe::Vector3::zeros()),
            _force(pe::Vector3::zeros()),
            _torque(pe::Vector3::zeros()),
            _temp_linear_velocity(pe::Vector3::zeros()),
            _temp_angular_velocity(pe::Vector3::zeros()),
            _penetration_linear_velocity(pe::Vector3::zeros()),
            _penetration_angular_velocity(pe::Vector3::zeros()) {
        updateWorldInertia();
    }

    void RigidBody::computeAABB() {
        if (_collision_shape) {
            _collision_shape->getAABB(_transform, _aabb_min, _aabb_max);
        } else {
            _aabb_min = pe::Vector3::zeros();
            _aabb_max = pe::Vector3::zeros();
        }
    }

    pe::Real RigidBody::getAABBScale() const {
        pe::Real scale = (_aabb_max - _aabb_min).norm();
        pe::Real diff = (_aabb_max + _aabb_min - _transform.getOrigin() * 2).norm();
        return (scale + diff) * 0.5;
    }

    void RigidBody::removeIgnoreCollisionId(uint32_t id) {
        for (auto it = _ignore_collision_ids.begin(); it != _ignore_collision_ids.end(); it++) {
            if (*it == id) {
                _ignore_collision_ids.erase(it);
                return;
            }
        }
    }

    bool RigidBody::isIgnoreCollisionId(uint32_t id) const {
        return std::any_of(_ignore_collision_ids.begin(), _ignore_collision_ids.end(), [id](uint32_t i) {
            return i == id;
        });
    }

    pe::Vector3 RigidBody::getWorldLinearVelocityAt(const pe::Vector3& world_point) const {
        return _linear_velocity + _angular_velocity.cross(world_point - _transform.getOrigin());
    }

    pe::Real RigidBody::getKineticEnergy() {
        return (_linear_velocity.dot(_mass * _linear_velocity) +
            _angular_velocity.dot(_local_inertia * _angular_velocity)) * 0.5;
    }

    pe::Real RigidBody::getImpulseDenominator(const pe::Vector3& world_point, const pe::Vector3& world_normal) const {
        pe::Vector3 r = world_point - _transform.getOrigin();
        pe::Vector3 c = r.cross(world_normal);
        pe::Vector3 vec = (_world_inv_inertia * c).cross(r);
        return _inv_mass + world_normal.dot(vec);
    }

    void RigidBody::syncTempVelocity() {
        _linear_velocity = _temp_linear_velocity;
        _angular_velocity = _temp_angular_velocity;
    }

    void RigidBody::clearTempVelocity() {
        _temp_linear_velocity = _linear_velocity;
        _temp_angular_velocity = _angular_velocity;
        _penetration_linear_velocity = pe::Vector3::zeros();
        _penetration_angular_velocity = pe::Vector3::zeros();
    }

    void RigidBody::applyTempImpulse(const pe::Vector3& world_rel_vec, const pe::Vector3& impulse) {
        if (isKinematic()) return;
        _temp_linear_velocity += impulse * _inv_mass;
        _temp_angular_velocity += _world_inv_inertia * world_rel_vec.cross(impulse);
    }

    void RigidBody::applyImpulse(const pe::Vector3& world_rel_vec, const pe::Vector3& impulse) {
        if (isKinematic()) return;
        _linear_velocity += impulse * _inv_mass;
        _angular_velocity += _world_inv_inertia * world_rel_vec.cross(impulse);
    }

    void RigidBody::applyPenetrationImpulse(const pe::Vector3 &world_rel_vec, const pe::Vector3 &impulse) {
        if (isKinematic()) return;
        _penetration_linear_velocity += impulse * _inv_mass;
        _penetration_angular_velocity += _world_inv_inertia * world_rel_vec.cross(impulse);
    }

    void RigidBody::addForce(const pe::Vector3 &world_point, const pe::Vector3 &force) {
        _force += force;
        _torque += (world_point - _transform.getOrigin()).cross(force);
    }

    void RigidBody::applyForce(pe::Real dt) {
        if (isKinematic()) return;
        _linear_velocity += _force * _inv_mass * dt;
        _angular_velocity += _world_inv_inertia * (_torque * dt);
        _force = pe::Vector3::zeros();
        _torque = pe::Vector3::zeros();
    }

    void RigidBody::applyDamping(pe::Real dt) {
        if (isKinematic()) return;
        _linear_velocity *= std::pow(1.0 - _linear_damping, dt);
        _angular_velocity *= std::pow(1.0 - _angular_damping, dt);
    }

    void RigidBody::penetrationStep(pe::Real dt) {
        if (isKinematic()) return;
        _transform.setOrigin(_transform.getOrigin() + _penetration_linear_velocity * dt);
        pe::Matrix3 rot;
        pe::Real angle_speed = _penetration_angular_velocity.norm();
        if (angle_speed > PE_EPS) {
            rot.setRotation(_penetration_angular_velocity.normalized(),
                            _penetration_angular_velocity.norm() * dt);
            _transform.setBasis(rot * _transform.getBasis());
        }
        _penetration_linear_velocity = pe::Vector3::zeros();
        _penetration_angular_velocity = pe::Vector3::zeros();
        updateWorldInertia();
    }

    void RigidBody::step(pe::Real dt) {
        if (isKinematic()) return;
        _transform.setOrigin(_transform.getOrigin() + _linear_velocity * dt);
        pe::Matrix3 rot;
        pe::Real angle_speed = _angular_velocity.norm();
        if (angle_speed > PE_EPS) {
            rot.setRotation(_angular_velocity.normalized(), angle_speed * dt);
            _transform.setBasis(rot * _transform.getBasis());
        }
        updateWorldInertia();
    }

} // namespace pe_phys_object