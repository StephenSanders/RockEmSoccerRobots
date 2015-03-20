/*
 * PongHUDScene.h
 * Stephen Sanders & Jake Singer
 */

#ifndef __H_GAME_PONGHUDSCENE__
#define __H_GAME_PONGHUDSCENE__

#include <Foundation/Scene.h>
#include <Foundation/Resource.h>
#include <engine/graphics.h>

#define BOOST_RATE 0.25f

class PongHUDScene : public Scene
{
public:
	enum {
		BALL_EMPTY = 0,
		BALL_BLUE,
		BALL_RED
	};

	void start()
	{
		camera.setProjection(Camera::orthographicProjection);

		scoreSprite = new Sprite(3);
		scoreSprite->load(Resource::contentPath + "sprite/ball_empty.png", BALL_EMPTY);
		scoreSprite->load(Resource::contentPath + "sprite/ball_blue.png", BALL_BLUE);
		scoreSprite->load(Resource::contentPath + "sprite/ball_red.png", BALL_RED);

		boostFrame = new Sprite(1);
		boostFrame->load(Resource::contentPath + "sprite/boost_frame.png", 0);

		boostBar = new Sprite(2);
		boostBar->load(Resource::contentPath + "sprite/boost_blue.png", 0);
		boostBar->load(Resource::contentPath + "sprite/boost_red.png", 1);
	}
	
	void end()
	{
		Scene::end();
		delete scoreSprite;
		delete boostFrame;
	}

	void update()
	{
		Scene::update();
		if(Game::state.blueBoost  >= 1.0f){
			Game::state.blueBoost = 1.0f;
		}
		else{
			if(Game::state.blueBoost <0.0f)
				Game::state.blueBoost += BOOST_RATE * time_delta;
			Game::state.blueBoost += BOOST_RATE * time_delta;
		}
		if(Game::state.redBoost >= 1.0f){
			Game::state.redBoost = 1.0f;
		}
		else{
			if(Game::state.redBoost < 0.0f)
				Game::state.redBoost += BOOST_RATE * time_delta;
			Game::state.redBoost += BOOST_RATE * time_delta;
		}
	}

	void render()
	{
		Scene::render();

		camera.bind();
		Graphics::depthMask(false);
		Graphics::lightingMask(false);
		camera.lookAt(
			D3DXVECTOR3(0.0f, 0.0f, -10.0f),
			D3DXVECTOR3(0.0f, 0.0f, 0.0f),
			D3DXVECTOR3(0.0f, 1.0f, 0.0f)
		);

		Graphics::matrixMode(Graphics::modelMatrix);
		Graphics::loadIdentity();

		/* red hud */
		for(int i = 0; i < 5; ++i) {
			Graphics::pushMatrix();
			Graphics::translate(D3DXVECTOR3(Graphics::viewWidth - 32.0f - (48.0f * i), 32.0f, 0.0f));
			if (Game::state.redScore > i)
				scoreSprite->render(BALL_RED);
			else
				scoreSprite->render(BALL_EMPTY);
			Graphics::popMatrix();
		}
		Graphics::pushMatrix();
		Graphics::translate(D3DXVECTOR3(Graphics::viewWidth - 16.0f, 80.0f, 0.0f));
		Graphics::scale(fabs(Game::state.redBoost) * 128.0f, 1.0f, 1.0f);
		Graphics::translate(D3DXVECTOR3(-0.5f, 0.0f, 0.0f));
		boostBar->render(1);
		Graphics::popMatrix();
		Graphics::pushMatrix();
		Graphics::translate(D3DXVECTOR3(Graphics::viewWidth - 80.0f, 80.0f, 0.0f));
		boostFrame->render(0);
		Graphics::popMatrix();

		/* blue hud */
		for(int i = 0; i < 5; ++i) {
			Graphics::pushMatrix();
			Graphics::translate(D3DXVECTOR3(32.0f + (48.0f * i), Graphics::viewHeight - 32.0f, 0.0f));
			if (Game::state.blueScore > i)
				scoreSprite->render(BALL_BLUE);
			else
				scoreSprite->render(BALL_EMPTY);
			Graphics::popMatrix();
		}
		Graphics::pushMatrix();
		Graphics::translate(D3DXVECTOR3(16.0f, Graphics::viewHeight - 80.0f, 0.0f));
		Graphics::scale(fabs(Game::state.blueBoost) * 128.0f, 1.0f, 1.0f);
		Graphics::translate(D3DXVECTOR3(0.5f, 0.0f, 0.0f));
		boostBar->render(0);
		Graphics::popMatrix();
		Graphics::pushMatrix();
		Graphics::translate(D3DXVECTOR3(80.0f, Graphics::viewHeight - 80.0f, 0.0f));
		boostFrame->render(0);
		Graphics::popMatrix();

		Graphics::lightingMask(true);
		Graphics::depthMask(true);
	}

private:
	Sprite *scoreSprite;
	Sprite *boostFrame;
	Sprite *boostBar;
};

#endif /* __H_GAME_PONGHUDSCENE__ */
