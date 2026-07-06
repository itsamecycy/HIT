#include "playerMovement.h"
#include "raymath.h"

PlayerMovement::PlayerMovement()
{
    position = {0,2,0};

    yaw = -90.0f;
    pitch = 0.0f;

    walkSpeed = 5.0f;
    sprintSpeed = 9.0f;

    moving = false;
    sprinting = false;

    sliding = false;
    slideTime = 0.0f;
    slideDuration = 0.5f;
    slideCooldown = 1.0f;
    slideTimer = 0.0f;

    verticalVelocity = 0;
    grounded = true;
    gravity = 20.0f;
    jumpForce = 8.0f;
}

bool PlayerMovement::IsWalking() const
{
    return moving && !sprinting;
}

bool PlayerMovement::IsSprinting() const
{
    return moving && sprinting;
}

void PlayerMovement::Update(Camera3D& camera)
{
    float dt = GetFrameTime();

    // Mouse look
    Vector2 mouse = GetMouseDelta();
    float sens = 0.1f;

    yaw   += mouse.x * sens;
    pitch -= mouse.y * sens;

    if (pitch > 89) pitch = 89;
    if (pitch < -89) pitch = -89;

    Vector3 forward;
    forward.x = cosf(DEG2RAD * yaw) * cosf(DEG2RAD * pitch);
    forward.y = sinf(DEG2RAD * pitch);
    forward.z = sinf(DEG2RAD * yaw) * cosf(DEG2RAD * pitch);

    forward = Vector3Normalize(forward);

    Vector3 flatForward = forward;
    flatForward.y = 0;
    flatForward = Vector3Normalize(flatForward);

    Vector3 right = Vector3CrossProduct(flatForward, {0,1,0});

    // Speed
    sprinting = IsKeyDown(KEY_LEFT_SHIFT);
    float speed = sprinting ? sprintSpeed : walkSpeed;

    // WASD Movement
    Vector3 move = {0};

    if (IsKeyDown(KEY_W)) move = Vector3Add(move, flatForward);
    if (IsKeyDown(KEY_S)) move = Vector3Subtract(move, flatForward);
    if (IsKeyDown(KEY_D)) move = Vector3Add(move, right);
    if (IsKeyDown(KEY_A)) move = Vector3Subtract(move, right);

    if (Vector3Length(move) > 0)
    {
        move = Vector3Normalize(move);
        moving = true;
    }
    else
    {
        moving = false;
    }

    // Slide
    if (slideTimer > 0) slideTimer -= dt;

    if (IsKeyPressed(KEY_LEFT_ALT) &&
        sprinting &&
        slideTimer <= 0 &&
        Vector3Length(move) > 0)
    {
        sliding = true;
        slideDir = move;
        slideTime = slideDuration;
        slideTimer = slideCooldown;
    }

    if (sliding)
    {
        position = Vector3Add(position, Vector3Scale(slideDir, 20.0f * dt));

        slideTime -= dt;

        if (slideTime <= 0)
            sliding = false;
    }
    else
    {
        position = Vector3Add(position, Vector3Scale(move, speed * dt));
    }

    // Jumping
    if (IsKeyPressed(KEY_SPACE) && grounded)
    {
        verticalVelocity = jumpForce;
        grounded = false;
    }

    if (!grounded)
    {
        verticalVelocity -= gravity * dt;
        position.y += verticalVelocity * dt;
    }

    if (position.y <= 2.0f)
    {
        position.y = 2.0f;
        verticalVelocity = 0;
        grounded = true;
    }

    // Camera update
    camera.position = position;
    camera.target = Vector3Add(position, forward);
}