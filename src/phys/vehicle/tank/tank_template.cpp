#include "tank_template.h"
#include "phys/fracture/fracture_utils/fracture_data.h"

// potential optimization:
// 1. max speed (linear/angular)
// 2. more real force simulation for drive wheels

#define PE_TANK_TRACK_SEG_THICKNESS 0.06
#define PE_TANK_TRACK_SEG_WIDTH 0.2
#define PE_TANK_RAYCAST_OFFSET 0.05
#define PE_TANK_WHEEL_MARGIN 0.02

namespace pe_phys_vehicle {

    void TankTemplate::initBody(pe_intf::World* dw) {
        body = new pe_phys_object::RigidBody();
        auto shape_b = new pe_phys_shape::BoxShape(pe::Vector3(
                _bodyWidth, _bodyHeight, _bodyLength));
        body->setCollisionShape(shape_b);
        body->setTransform(_transform);
        body->setMass(_bodyMass);
        body->setLocalInertia(shape_b->calcLocalInertia(_bodyMass));
        dw->addRigidBody(body);

        turret = new pe_phys_object::RigidBody();
        turret->setKinematic(true);
        turret->addIgnoreCollisionId(body->getGlobalId());
        auto shape_t = new pe_phys_shape::BoxShape(pe::Vector3(
                _turretWidth, _turretHeight, _turretLength));
        turret->setCollisionShape(shape_t);
        turret->setMass(_turretMass);
        turret->setLocalInertia(shape_t->calcLocalInertia(_turretMass));
        dw->addRigidBody(turret);
        turretTrl = pe::Vector3(0, (_bodyHeight + _turretHeight) / 2, _bodyLength / 20);

        barrel = new pe_phys_object::RigidBody();
        barrel->setKinematic(true);
        barrel->addIgnoreCollisionId(turret->getGlobalId());
        auto shape_r = new pe_phys_shape::BoxShape(pe::Vector3(
                _barrelRadius * 2, _barrelRadius * 2, _barrelLength));
        barrel->setCollisionShape(shape_r);
        barrel->setMass(_barrelMass);
        barrel->setLocalInertia(shape_r->calcLocalInertia(_barrelMass));
        dw->addRigidBody(barrel);
        barrelTrl = pe::Vector3(0, 0, -(_turretLength + _barrelLength) / 2);

        updateTurretAndBarrelTransform(); // update transforms of turret and barrel
    }

    void TankTemplate::initVehicle(pe_intf::World* dw) {
        RaycastVehicle::VehicleTuning m_tuning;
        DefaultVehicleRaycaster* rayCaster = new DefaultVehicleRaycaster(dw);
        vehicle = new RaycastVehicle(m_tuning, body, rayCaster);
        vehicle->setCoordinateSystem(0, 1, 2);
        vehicle->addRaycastExcludeId(body->getGlobalId());
        vehicle->addRaycastExcludeId(turret->getGlobalId());
        vehicle->addRaycastExcludeId(barrel->getGlobalId());
    }

    void TankTemplate::initWheels(pe_intf::World* dw) {
        pe::Real connectionHeight = -PE_TANK_RAYCAST_OFFSET;
        pe::Real connectionHeightEnds = _bodyHeight / 2 - PE_TANK_RAYCAST_OFFSET;
        pe::Vector3 connectionPointCS0;
        pe::Vector3 wheelDirectionCS0(0, -1, 0);
        pe::Vector3 wheelAxleCS(-1, 0, 0);
        pe::Real suspensionRestLength = _bodyHeight / 2 - PE_TANK_RAYCAST_OFFSET;
        pe::Real gap = _bodyLength / (pe::Real)(_wheelNum / 2 - 1); //NOLINT
        RaycastVehicle::VehicleTuning m_tuning;
//        m_tuning.m_suspensionStiffness = 20.0;
//        m_tuning.m_maxSuspensionForce = 100000.0;

        for (int i = 0; i < _wheelNum / 2; i++) {
            pe::Real wheelRadius = i == 0 || i == _wheelNum / 2 - 1 ? _powerWheelRadius : _drivenWheelRadius;
            connectionPointCS0 = pe::Vector3(-_bodyWidth / 2 - (0.5 * _wheelWidth),
                                             (i == 0 || i == _wheelNum / 2 - 1) ?
                                             connectionHeightEnds : connectionHeight,
                                             gap * i - _bodyLength / 2);
            vehicle->addWheel(connectionPointCS0, wheelDirectionCS0, wheelAxleCS,
                              suspensionRestLength, wheelRadius,
                              m_tuning, false);
            connectionPointCS0 = pe::Vector3(_bodyWidth / 2 + (0.5 * _wheelWidth),
                                             (i == 0 || i == _wheelNum / 2 - 1) ?
                                             connectionHeightEnds : connectionHeight,
                                             gap * i - _bodyLength / 2);
            vehicle->addWheel(connectionPointCS0, wheelDirectionCS0, wheelAxleCS,
                              suspensionRestLength, wheelRadius,
                              m_tuning, false);
        }

        for (int i = 0; i < _wheelNum; i++) {
            pe_phys_object::RigidBody* wheel = new pe_phys_object::RigidBody();
            wheel->setKinematic(true);
            wheel->addIgnoreCollisionId(body->getGlobalId());
            wheel->setCollisionShape(new pe_phys_shape::CylinderShape(
                    vehicle->getWheelInfo(i).m_wheelsRadius - PE_TANK_WHEEL_MARGIN, _wheelWidth));
            wheels.push_back(wheel);
            dw->addRigidBody(wheel);
            vehicle->addRaycastExcludeId(wheel->getGlobalId());
        }

        for (int i = 0; i < _wheelNum; i++) {
            WheelInfo& wheel = vehicle->getWheelInfo(i);
            wheel.m_suspensionStiffness = _suspensionStiffness;
            wheel.m_wheelsDampingRelaxation = _suspensionDamping;
            wheel.m_wheelsDampingCompression = _suspensionCompression;
            wheel.m_frictionSlip = _wheelFriction;
            wheel.m_rollInfluence = _wheelRollInfluence;
        }

        vehicle->resetSuspension();
        for (int i = 0; i < _wheelNum; i++) {
            //synchronize the wheels with the (interpolated) chassis world transform
            vehicle->updateWheelTransform(i, true);
        }

        updateWheelsTransform();
    }

    void TankTemplate::initTracks(pe_intf::World* dw) {
        trackHoldingPoints.resize(_wheelNum * 2);
        trackHoldingWheels.resize(_wheelNum * 2);
        updateTrackHoldings();

        // all the track segments are the same
        for (int i = 0; i < _trackSegmentNum * 2; i++) {
            pe_phys_object::RigidBody* rb = new pe_phys_object::RigidBody();
            rb->setKinematic(true);
            rb->addIgnoreCollisionId(body->getGlobalId());
            rb->setCollisionShape(new pe_phys_shape::BoxShape(pe::Vector3(
                    _wheelWidth, PE_TANK_TRACK_SEG_THICKNESS, PE_TANK_TRACK_SEG_WIDTH)));
            trackSegments.push_back(rb);
        }

        updateTracksTransform();
    }

    void TankTemplate::updateTurretAndBarrelTransform() {
        auto& bodyTrans = body->getTransform();

        pe::Matrix3 rotTurret;
        rotTurret.setRotation(pe::Vector3::up(), turretAngle);
        pe::Transform transTurret = bodyTrans * pe::Transform(rotTurret, turretTrl);
        turret->setTransform(transTurret);

        auto rotBarrel = pe::Matrix3::identity();
        pe::Transform transBarrel = transTurret * pe::Transform(rotBarrel, barrelTrl);
        barrel->setTransform(transBarrel);
    }

    void TankTemplate::updateWheelsTransform() {
        pe::Matrix3 wheelRot = pe::Matrix3::identity();
        pe::Real theta = 3.1415926 / 2;
        wheelRot[0][0] = cos(theta);
        wheelRot[0][1] = -sin(theta);
        wheelRot[1][0] = sin(theta);
        wheelRot[1][1] = cos(theta);
        int num = (int)wheels.size();

        for (int i = 0; i < num; i++) {
            pe::Transform tr = vehicle->getWheelInfo(i).m_worldTransform;
            tr.setBasis(tr.getBasis() * wheelRot);
            wheels[i]->setTransform(tr);
        }
    }

    void TankTemplate::updateTracksTransform() {
        updateOneSideTrackTransform(0);
        updateOneSideTrackTransform(1);
    }

    void TankTemplate::updateOneSideTrackTransform(int side) {
        int startIdx = _wheelNum * side;
        int segStartIdx = _trackSegmentNum * side;
        int segIdx = 0;
        pe::Real length = 0;
        pe::Real offset = side == 0 ? trackOffsetLeft : trackOffsetRight;
        pe::Real segmentWidth = side == 0 ? trackSegmentWidthLeft : trackSegmentWidthRight;
        pe::Vector3 leftVec(-1, 0, 0);
        pe::Vector3 upVec(0, 1, 0);
        auto& bodyTrans = body->getTransform();

        int i = 0, loop = 0;
        while (loop++ < _wheelNum) {
            auto& p1 = trackHoldingPoints[startIdx + i];
            auto& p2 = trackHoldingPoints[startIdx + i + 1];
            auto& p3 = trackHoldingPoints[startIdx + (i + 2) % _wheelNum];
            auto& wheel = trackHoldingWheels[startIdx + i + 1];

            auto dir = (p1 - p2).normalized();
            auto vec = (p2 - wheel).normalized();
            auto alpha = leftVec.dot(vec.cross(upVec)) < 0 ? acos(vec.y) : -acos(vec.y);
            pe::Matrix3 rotMat;
            rotMat.setRotation(leftVec, alpha);
            pe::Real restLength = (p1 - p2).norm() - offset;
            if (restLength < 0) {
                offset = -restLength;
            }
            else {
                offset = 0;
                restLength -= length;
                while (restLength > 0) {
                    auto center = p2 + dir * restLength;
                    pe::Transform trans = bodyTrans * pe::Transform(rotMat, center);
                    trackSegments[segStartIdx + segIdx]->setTransform(trans);
                    restLength -= segmentWidth;
                    if (++segIdx >= _trackSegmentNum) return;
                }
                length = -restLength;
            }

            pe::Real radius = (p2 - wheel).norm();
            auto vec1 = (p2 - wheel).normalized();
            auto vec2 = (p3 - wheel).normalized();
            pe::Real angle = acos(clampReal(-1, 1, vec1.dot(vec2))) - offset / radius;
            pe::Real deltaAngle = segmentWidth / radius;
            if (angle < 0) {
                offset = -angle * radius;
            }
            else {
                offset = 0;
                angle -= length / radius;
                while (angle > 0) {
                    auto vec3 = rotateVector3(vec2, leftVec, -angle);
                    auto theta = leftVec.dot(vec3.cross(upVec)) < 0 ? acos(vec3.y) : -acos(vec3.y);
                    pe::Matrix3 rot;
                    rot.setRotation(leftVec, theta);
                    auto center = wheel + vec3 * radius;
                    pe::Transform trans = bodyTrans * pe::Transform(rot, center);
                    trackSegments[segStartIdx + segIdx]->setTransform(trans);
                    angle -= deltaAngle;
                    if (++segIdx >= _trackSegmentNum) return;
                }
                length = -angle * radius;
            }

            i = (i + 2) % _wheelNum;
        }
    }

    void TankTemplate::updateTrackHoldings() {
        updateOneSideTrackHoldings(0);
        updateOneSideTrackHoldings(1);
    }

    void TankTemplate::updateOneSideTrackHoldings(int side) {
        int startIdx = _wheelNum * side;
        pe::Vector3 rightVec = pe::Vector3(1, 0, 0);
        auto& bodyTrans = body->getTransform();
        pe::Real newLength = 0;

        for (int i = side; i < _wheelNum; i += 2) {
            int j = (i + 2) % _wheelNum;
            auto p1 = bodyTrans.inverseTransform(vehicle->getWheelTransformWS(i).getOrigin());
            auto p2 = bodyTrans.inverseTransform(vehicle->getWheelTransformWS(j).getOrigin());
            pe::Real r1 = vehicle->getWheelInfo(i).m_wheelsRadius;
            pe::Real r2 = vehicle->getWheelInfo(j).m_wheelsRadius;

            auto forwardVec = (p1 - p2).normalized();
            auto downVec = (p1 - p2).cross(rightVec).normalized();
            pe::Real t1 = (r2 - r1) / (p1 - p2).norm();
            pe::Real t2 = sqrt(clampReal(0, 1, 1 - t1 * t1));
            auto dir = forwardVec * t1 + downVec * t2;

            auto u1 = p1 + dir * r1;
            auto u2 = p2 + dir * r2;
            trackHoldingWheels[startIdx + i - side] = p1;
            trackHoldingPoints[startIdx + i - side] = u1;
            trackHoldingWheels[startIdx + i - side + 1] = p2;
            trackHoldingPoints[startIdx + i - side + 1] = u2;
            newLength += (u1 - u2).norm();
        }

        for (int i = 0; i < _wheelNum; i += 2) {
            auto& p1 = trackHoldingPoints[startIdx + i];
            auto& p2 = trackHoldingPoints[startIdx + (i + _wheelNum - 1) % _wheelNum];
            auto& center = trackHoldingWheels[startIdx + i];

            auto dir1 = (p1 - center).normalized();
            auto dir2 = (p2 - center).normalized();
            pe::Real length = (p1 - center).norm() * acos(clampReal(-1, 1, dir1.dot(dir2)));
            newLength += length;
        }

        (side == 0 ? trackLengthLeft : trackLengthRight) = newLength;
        (side == 0 ? trackSegmentWidthLeft : trackSegmentWidthRight) = newLength / _trackSegmentNum;
    }

    void TankTemplate::uniformTrackVelocity() {
        uniformTrackVelocityOneSide(0);
        uniformTrackVelocityOneSide(1);
    }

    void TankTemplate::uniformTrackVelocityOneSide(int side) {
        int num = (int)wheels.size();
        pe::Real avgDeltaDist = 0;
        for (int i = side + 2; i < num - 2; i += 2) {
            avgDeltaDist += vehicle->getWheelInfo(i).m_deltaRotation * vehicle->getWheelInfo(i).m_wheelsRadius;
        }
        avgDeltaDist /= (pe::Real)(num / 2 - 2); //NOLINT

        auto& offset = (side == 0 ? trackOffsetLeft : trackOffsetRight);
        auto length = (side == 0 ? trackLengthLeft : trackLengthRight);
        offset -= avgDeltaDist;
        if (offset < 0) offset += length;
        if (offset > length) offset -= length;

        for (int i = side; i < num; i += 2) {
            pe::Real oldDeltaAngle = vehicle->getWheelInfo(i).m_deltaRotation;
            pe::Real newDeltaAngle = avgDeltaDist / vehicle->getWheelInfo(i).m_wheelsRadius;
            vehicle->getWheelInfo(i).m_deltaRotation = newDeltaAngle;
            vehicle->getWheelInfo(i).m_rotation += (newDeltaAngle - oldDeltaAngle);
        }
    }

    TankTemplate::TankTemplate():
            _transform(pe::Transform::identity()),
            _bodyWidth(2.3),
            _bodyLength(7.),
            _bodyHeight(1.0),
            _bodyMass(30.0),
            _turretWidth(2.7),
            _turretHeight(0.8),
            _turretLength(3.5),
            _turretMass(1.),
            _turretRotSpeed(1.0),
            _turretMaxAngle(3.141593 / 2.4),
            _barrelRadius(0.128),
            _barrelLength(4.2),
            _barrelMass(1.),
            _wheelNum(16),
            _powerWheelRadius(0.3),
            _drivenWheelRadius(0.4),
            _wheelWidth(0.6),
            _wheelFriction(0.9),
            _wheelRollInfluence(0.1),
            _trackThickness(0.08),
            _trackSegmentNum(80),
            _suspensionStiffness(20.),
            _suspensionDamping(2.3),
            _suspensionCompression(4.4),
            _engineForce(50.) {}

    void TankTemplate::init(pe_intf::World* dw) {
        forwardForce = _engineForce;
        backwardForce = _engineForce;
        turnForce = _engineForce;
        brakeForce = _engineForce / 100;
        initBody(dw);
        initVehicle(dw);
        initWheels(dw);
        initTracks(dw);
    }

    void TankTemplate::advance(pe::Real step) {
        updateTurretAndBarrelTransform();
        updateWheelsTransform();
        uniformTrackVelocity();
        updateTrackHoldings();
        updateTracksTransform();
        vehicle->updateVehicle(step);
    }

    void TankTemplate::idle() {
        vehicle->applyEngineForce(0, 0);
        vehicle->applyEngineForce(0, 1);
        vehicle->applyEngineForce(0, vehicle->getNumWheels() - 1);
        vehicle->applyEngineForce(0, vehicle->getNumWheels() - 2);
    }

    void TankTemplate::brake() {
        pe::Vector3 force = vehicle->getForwardVector() * vehicle->getCurrentSpeedKmHour() / 3.6 * -brakeForce * 200;
        body->addForce(vehicle->getWheelTransformWS(0).getOrigin(), force);
        body->addForce(vehicle->getWheelTransformWS(1).getOrigin(), force);
        body->addForce(vehicle->getWheelTransformWS(vehicle->getNumWheels() - 1).getOrigin(),
                       force);
        body->addForce(vehicle->getWheelTransformWS(vehicle->getNumWheels() - 2).getOrigin(),
                       force);
    }

    void TankTemplate::moveForward() {
        pe::Vector3 force = vehicle->getForwardVector() * -forwardForce;
        body->addForce(vehicle->getWheelTransformWS(0).getOrigin(), force);
        body->addForce(vehicle->getWheelTransformWS(1).getOrigin(), force);
        body->addForce(vehicle->getWheelTransformWS(vehicle->getNumWheels() - 1).getOrigin(),
                       force);
        body->addForce(vehicle->getWheelTransformWS(vehicle->getNumWheels() - 2).getOrigin(),
                       force);
    }

    void TankTemplate::moveBackward() {
        pe::Vector3 force = vehicle->getForwardVector() * backwardForce;
        body->addForce(vehicle->getWheelTransformWS(0).getOrigin(), force);
        body->addForce(vehicle->getWheelTransformWS(1).getOrigin(), force);
        body->addForce(vehicle->getWheelTransformWS(vehicle->getNumWheels() - 1).getOrigin(),
                       force);
        body->addForce(vehicle->getWheelTransformWS(vehicle->getNumWheels() - 2).getOrigin(),
                       force);
    }

    void TankTemplate::turnLeft() {
        pe::Vector3 torque = body->getTransform().getBasis() * pe::Vector3::up() * turnForce * 10;
        body->addTorque(torque);
    }

    void TankTemplate::turnRight() {
        pe::Vector3 torque = body->getTransform().getBasis() * pe::Vector3::up() * -turnForce * 10;
        body->addTorque(torque);
    }

    void TankTemplate::barrelRotLeft(pe::Real step) {
        if (turretAngle < _turretMaxAngle) {
            turretAngle += step * _turretRotSpeed;
        }
    }

    void TankTemplate::barrelRotRight(pe::Real step) {
        if (turretAngle > -_turretMaxAngle) {
            turretAngle -= step * _turretRotSpeed;
        }
    }

} // namespace pe_phys_vehicle