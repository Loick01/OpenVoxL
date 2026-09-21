#pragma once

class Hitbox;

class CollisionResolver // TODO : Rename ?
{
    public:
        CollisionResolver() = default;

        void Resolve(Hitbox& hitbox, const float frameSpeed);

};