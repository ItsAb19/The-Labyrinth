#include "stdafx.h"
#include "MyGame.h"

CMyGame::CMyGame(void)
	// to initialise more sprites here use a comma-separated list
{
	// TODO: add initialisation here
}

CMyGame::~CMyGame(void)
{
	// TODO: add destruction code here
	
}

/////////////////////////////////////////////////////
// Per-Frame Callback Funtions (must be implemented!)

void CMyGame::OnUpdate()
{
	Uint32 t = GetTime();

	// TODO: add the game update code here

	m_sprite.Update(t);	// this will update the sample rocket sprite
	for (CSprite* w : wallList)
	{
		w->Update(t);
	}


	player.Update(t);	// this will update the sample rocket sprite
	if (IsKeyDown(SDLK_d) || IsKeyDown(SDLK_RIGHT))
	{
		player.SetVelocity(100, 0);
	}
	if (IsKeyDown(SDLK_a) || IsKeyDown(SDLK_RIGHT))
	{
		player.SetVelocity(-100, 0);
	}
	if (IsKeyDown(SDLK_w) || IsKeyDown(SDLK_UP))
	{
		player.SetVelocity(0, 100);
	}
	if (IsKeyDown(SDLK_s) || IsKeyDown(SDLK_DOWN))
	{
		player.SetVelocity(0, -100);
	}
	

}

void CMyGame::OnDraw(CGraphics* g)
{
	// TODO: add drawing code here
	player.Draw(g);	// this will draw the sample rocket sprite
	for (CSprite* w : wallList)
	{
		w->Draw(g);
	}


	// this will print the game time
	//*g << bottom << left << "Time elapsed: " << timetext(GetTime());
}

/////////////////////////////////////////////////////
// Game Life Cycle

// one time initialisation
void CMyGame::OnInitialize()
{
	player.LoadImage("No_sword_right.png", "Facing_right",CColor::White());
	player.LoadImage("No_sword_left.png", "Facing_left",CColor::White());

	player.SetImage("Facing_right");
	OnWalls();
	
}
void CMyGame::OnWalls()
{
	wallList.clear();

	// Wall 0
	CSprite* w0 = new CSprite();
	w0->LoadImage("Walls.png", "Brick", CColor::White());
	w0->SetImage("Brick");      
	w0->SetPosition(60, 15);
	w0->SetSize(150, 50);
	wallList.push_back(w0);

	// Wall 1
	CSprite* w1 = new CSprite();
	w1->LoadImage("Walls.png", "Brick", CColor::White());
	w1->SetImage("Brick");      
	w1->SetPosition(185, 15);
	w1->SetSize(150, 50);
	wallList.push_back(w1);

	// Wall 3
	CSprite* w2 = new CSprite();
	w2->LoadImage("Walls.png", "Brick", CColor::White());
	w2->SetImage("Brick");
	w2->SetPosition(310, 15);
	w2->SetSize(150, 50);
	wallList.push_back(w2);

	// Wall 4
	CSprite* w3 = new CSprite();
	w3->LoadImage("Walls.png", "Brick", CColor::White());
	w3->SetImage("Brick");
	w3->SetPosition(435, 15);
	w3->SetSize(150, 50);
	wallList.push_back(w3);

	// Wall 5
	CSprite* w4 = new CSprite();
	w4->LoadImage("Walls.png", "Brick", CColor::White());
	w4->SetImage("Brick");
	w4->SetPosition(560, 15);
	w4->SetSize(150, 50);
	wallList.push_back(w4);

	// Wall 6
	CSprite* w5 = new CSprite();
	w5->LoadImage("Walls.png", "Brick", CColor::White());
	w5->SetImage("Brick");
	w5->SetPosition(685, 15);
	w5->SetSize(150, 50);
	wallList.push_back(w5);

	// Wall 7
	CSprite* w6 = new CSprite();
	w6->LoadImage("Walls.png", "Brick", CColor::White());
	w6->SetImage("Brick");
	w6->SetPosition(810, 15);
	w6->SetSize(150, 50);
	wallList.push_back(w6);

	// Wall 8
	CSprite* w7 = new CSprite();
	w7->LoadImage("Right wall.png", "Brick", CColor::White());
	w7->SetImage("Brick");
	w7->SetPosition(790, 90);
	w7->SetSize(50, 150);
	wallList.push_back(w7);
}


// called when a new game is requested (e.g. when F2 pressed)
// use this function to prepare a menu or a welcome screen
void CMyGame::OnDisplayMenu()
{
	StartGame();	// exits the menu mode and starts the game mode
}

// called when a new game is started
// as a second phase after a menu or a welcome screen
void CMyGame::OnStartGame()
{
	player.SetPos(300, 300);

	wall.SetPos(400, 400);

	OnWalls();
}

// called when a new level started - first call for nLevel = 1
void CMyGame::OnStartLevel(Sint16 nLevel)
{
}

// called when the game is over
void CMyGame::OnGameOver()
{
}

// one time termination code
void CMyGame::OnTerminate()
{
}

/////////////////////////////////////////////////////
// Keyboard Event Handlers

void CMyGame::OnKeyDown(SDLKey sym, SDLMod mod, Uint16 unicode)
{
	if (sym == SDLK_F4 && (mod & (KMOD_LALT | KMOD_RALT)))
		StopGame();
	if (sym == SDLK_SPACE)
		PauseGame();
	if (sym == SDLK_F2)
		NewGame();

	if (sym == SDLK_a)
	{
		player.SetImage("Facing_left");
	}
	if (sym == SDLK_d)
	{
		player.SetImage("Facing_right");
	}
}

void CMyGame::OnKeyUp(SDLKey sym, SDLMod mod, Uint16 unicode)
{
	if (sym == (SDLK_d) || IsKeyDown(SDLK_RIGHT))
	{
		player.SetVelocity(0, 0);
	}
	if (sym == (SDLK_a) || IsKeyDown(SDLK_LEFT))
	{
		player.SetVelocity(0, 0);
	}
	if (sym == (SDLK_w) || IsKeyDown(SDLK_UP))
	{
		player.SetVelocity(0, 0);
	}
	if (sym == (SDLK_s) || IsKeyDown(SDLK_RIGHT))
	{
		player.SetVelocity(0, 0);
	}
}


/////////////////////////////////////////////////////
// Mouse Events Handlers

void CMyGame::OnMouseMove(Uint16 x,Uint16 y,Sint16 relx,Sint16 rely,bool bLeft,bool bRight,bool bMiddle)
{
}

void CMyGame::OnLButtonDown(Uint16 x,Uint16 y)
{
}

void CMyGame::OnLButtonUp(Uint16 x,Uint16 y)
{
}

void CMyGame::OnRButtonDown(Uint16 x,Uint16 y)
{
}

void CMyGame::OnRButtonUp(Uint16 x,Uint16 y)
{
}

void CMyGame::OnMButtonDown(Uint16 x,Uint16 y)
{
}

void CMyGame::OnMButtonUp(Uint16 x,Uint16 y)
{
}
