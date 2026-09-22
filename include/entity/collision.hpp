#pragma once

class Hitbox;
class Terrain;

class CollisionResolver // TODO : Rename ?
{
    private:    
        const Terrain& m_terrain;
        
    public:
        CollisionResolver(const Terrain& terrain);

        void Resolve(Hitbox& hitbox, const float frameSpeed);
};