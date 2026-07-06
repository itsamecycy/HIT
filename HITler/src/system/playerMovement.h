#ifndef PLAYERMOVEMENT_H
#define PLAYERMOVEMENT_H

#include "raylib.h"

struct Obstacle
{
    BoundingBox box;
    Color color;
};

class PlayerMovement
{
public:
    PlayerMovement();
    void Update(Camera3D& camera, const Obstacle* obstacles, int obstacleCount);
    bool IsWalking() const;
    bool IsSprinting() const;
    void GrappleTo(const Vector3& target);
    void StopGrapple();
    bool IsGrappling() const;
    Vector3 GetGrappleTarget() const;

private:
    Vector3 position;

    float yaw;
    float pitch;

    float walkSpeed;
    float sprintSpeed;

    bool moving;
    bool sprinting;

    bool grappling;
    Vector3 grappleTarget;
    float grappleSpeed;

    // slide
    bool sliding;
    float slideTime;
    float slideDuration;
    float slideCooldown;
    float slideTimer;
    Vector3 slideDir;

    // jump
    float verticalVelocity;
    bool grounded;
    float gravity;
    float jumpForce;
};

#endif