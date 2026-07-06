#ifndef PLAYERMOVEMENT_H
#define PLAYERMOVEMENT_H

#include "raylib.h"

class PlayerMovement
{
public:
    PlayerMovement();
    void Update(Camera3D& camera);
    bool IsWalking() const;
    bool IsSprinting() const;

private:
    Vector3 position;

    float yaw;
    float pitch;

    float walkSpeed;
    float sprintSpeed;

    bool moving;
    bool sprinting;

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