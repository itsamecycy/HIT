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
    grappling = false;
    grappleSpeed = 28.0f;

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

bool PlayerMovement::IsGrappling() const
{
    return grappling;
}

void PlayerMovement::GrappleTo(const Vector3& target)
{
    grappling = true;
    grappleTarget = target;
}

void PlayerMovement::StopGrapple()
{
    grappling = false;
}

Vector3 PlayerMovement::GetGrappleTarget() const
{
    return grappleTarget;
}

void PlayerMovement::Update(Camera3D& camera, const Obstacle* obstacles, int obstacleCount)
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

    if (grappling)
    {
        Vector3 grappleDir = Vector3Subtract(grappleTarget, position);
        float grappleDistance = Vector3Length(grappleDir);
        if (grappleDistance < 1.0f)
        {
            grappling = false;
        }
        else
        {
            grappleDir = Vector3Normalize(grappleDir);
            Vector3 desiredPosition = Vector3Add(position, Vector3Scale(grappleDir, grappleSpeed * dt));

            BoundingBox playerBox = { {desiredPosition.x - 0.3f, desiredPosition.y, desiredPosition.z - 0.3f}, {desiredPosition.x + 0.3f, desiredPosition.y + 1.8f, desiredPosition.z + 0.3f} };
            bool blocked = false;
            for (int i = 0; i < obstacleCount; ++i)
            {
                if (CheckCollisionBoxes(playerBox, obstacles[i].box))
                {
                    blocked = true;
                    break;
                }
            }

            if (!blocked)
            {
                position = desiredPosition;
            }
            else
            {
                grappling = false;
            }

            camera.position = position;
            camera.target = Vector3Add(position, forward);
            return;
        }
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
        Vector3 desired = Vector3Add(position, Vector3Scale(slideDir, 20.0f * dt));
        BoundingBox playerBox = { {desired.x - 0.3f, desired.y, desired.z - 0.3f}, {desired.x + 0.3f, desired.y + 1.8f, desired.z + 0.3f} };
        bool blocked = false;
        for (int i = 0; i < obstacleCount; ++i)
        {
            if (CheckCollisionBoxes(playerBox, obstacles[i].box))
            {
                blocked = true;
                break;
            }
        }

        if (!blocked)
        {
            position = desired;
        }

        slideTime -= dt;

        if (slideTime <= 0)
            sliding = false;
    }
    else
    {
        Vector3 desired = Vector3Add(position, Vector3Scale(move, speed * dt));
        BoundingBox playerBox = { {desired.x - 0.3f, desired.y, desired.z - 0.3f}, {desired.x + 0.3f, desired.y + 1.8f, desired.z + 0.3f} };
        bool blocked = false;
        for (int i = 0; i < obstacleCount; ++i)
        {
            if (CheckCollisionBoxes(playerBox, obstacles[i].box))
            {
                blocked = true;
                break;
            }
        }

        if (!blocked)
        {
            position = desired;
        }
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

    bool standingOnObstacle = false;

    if (position.y <= 2.0f)
    {
        position.y = 2.0f;
        verticalVelocity = 0;
        grounded = true;
        standingOnObstacle = true;
    }
    else
    {
        for (int i = 0; i < obstacleCount; ++i)
        {
            BoundingBox playerBox = { {position.x - 0.3f, position.y, position.z - 0.3f},
                                      {position.x + 0.3f, position.y + 1.8f, position.z + 0.3f} };

            if (CheckCollisionBoxes(playerBox, obstacles[i].box))
            {
                float obstacleTop = obstacles[i].box.max.y;
                float obstacleBottom = obstacles[i].box.min.y;

                if (position.y < obstacleTop && position.y + 1.8f > obstacleTop)
                {
                    position.y = obstacleTop;
                    verticalVelocity = 0;
                    grounded = true;
                    standingOnObstacle = true;
                }
                else if (position.y + 1.8f > obstacleBottom && position.y < obstacleBottom)
                {
                    position.y = obstacleBottom - 1.8f;
                    verticalVelocity = 0;
                }
                break;
            }
        }
    }

    if (!standingOnObstacle && position.y > 2.0f)
    {
        grounded = false;
    }

    // Camera update
    camera.position = position;
    camera.target = Vector3Add(position, forward);
}