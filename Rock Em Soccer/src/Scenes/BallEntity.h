#ifndef __H_GAME_BALLENTITY__
#define __H_GAME_BALLENTITY__

#include <Foundation/Entity.h>

#include <engine/graphics.h>

#define BALL_MAX_SPEED (2.5f * RACKET_ENTITY_SPEED) //2.5f * RACKET_ENTITY_SPEED

#define BALL_DECCELERATION (10.0f)

#define BALL_MAX_HEIGHT (10.0f)

class BallEntity : public Entity
{
public:
	Mesh *mesh;
	D3DXVECTOR3 velocity;
	int bounceSound[4], kickUp;

	void start();

	void end();

	void update();

	void render();

	void reset();

private:	
	Sprite *shadow;
};

#endif /* __H_GAME_BALLENTITY__ */
