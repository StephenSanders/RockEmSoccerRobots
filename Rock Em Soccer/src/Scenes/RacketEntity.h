#ifndef __H_GAME_RACKETENTITY__
#define __H_GAME_RACKETENTITY__

#include <Foundation/Entity.h>

#include <engine/graphics.h>

#define RACKET_ENTITY_SPEED (25.0f)
#define RACKET_ACCELERATION (250.0f)
#define RACKET_MAX_HEIGHT (6.0f)

class RacketEntity : public Entity
{
public:
	enum {
		STATE_NONE = 0,
		STATE_IDLE,
		STATE_RUN,
		STATE_PUNCHING
	};

	int keyUp, keyDown, keyLeft, keyRight, keyBoost, keyPunch, keyKick, keyJump;
	int punched, jumping, boost;
	float punching;
	int useController;
	float rumbleTime;
	float* boostBar;
	int boostSound;

	D3DXVECTOR3 velocity, lastPos;
	Mesh *mesh;

	void start();

	void end();

	void update();

	void render();

private:
	Sprite *shadow;
	int state;
};

#endif /* __H_GAME_RACKETENTITY__ */
