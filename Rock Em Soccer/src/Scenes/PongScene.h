/*
 * PongScene.h
 * Stephen Sanders & Jake Singer
 */

#ifndef __H_GAME_PONGSCENE__
#define __H_GAME_PONGSCENE__

#include <Foundation/Scene.h>
#include "RacketEntity.h"
#include "BallEntity.h"
#include <engine/graphics.h>


class PongScene : public Scene
{
public:

	void start();

	void changePlayers(int numPlay);
	
	void end();

	void update();

	void render();

	void reset();

	Mesh *scoreBallMesh;
	Scene *HUDScene;

	Model field;
	Mesh *fieldMesh;

	RacketEntity *racket[4];
	BallEntity *ball;
	Light light1;
	int p1Goal[3],p2Goal[3],p1Win,p2Win, punchSound;
	float goal;

private:
	int numPlayers;
	
	D3DXVECTOR3 rackMin, rackMax, ballMin, ballMax;

	void collisionTest();
	
};

#endif /* __H_GAME_PONGSCENE__ */
