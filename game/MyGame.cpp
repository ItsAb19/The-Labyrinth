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
	w0->LoadImage("BigWalls.png", "Brick", CColor::White());
	w0->SetImage("Brick");      
	w0->SetPosition(400, 300);
	wallList.push_back(w0);

	CSprite* w1 = new CSprite();
	w1->LoadImage("Right wall.png", "Brick", CColor::White());
	w1->SetImage("Brick");
	w1->SetPosition(700, 39);
	wallList.push_back(w1);

	CSprite* w2 = new CSprite();
	w2->LoadImage("Right wall.png", "Brick", CColor::White());
	w2->SetImage("Brick");
	w2->SetPosition(700, 80);
	wallList.push_back(w2);

	CSprite* w3 = new CSprite();
	w3->LoadImage("Right wall.png", "Brick", CColor::White());
	w3->SetImage("Brick");
	w3->SetPosition(700, 120);
	wallList.push_back(w3);

	//first entrance
	CSprite* w4 = new CSprite();
	w4->LoadImage("Right wall.png", "Brick", CColor::White());
	w4->SetImage("Brick");
	w4->SetPosition(700, 160);
	wallList.push_back(w4);

	CSprite* w5 = new CSprite();
	w5->LoadImage("Right wall.png", "Brick", CColor::White());
	w5->SetImage("Brick");
	w5->SetPosition(700, 260);
	wallList.push_back(w5);

	CSprite* w6 = new CSprite();
	w6->LoadImage("Right wall.png", "Brick", CColor::White());
	w6->SetImage("Brick");
	w6->SetPosition(700, 300);
	wallList.push_back(w6);

	CSprite* w7 = new CSprite();
	w7->LoadImage("Right wall.png", "Brick", CColor::White());
	w7->SetImage("Brick");
	w7->SetPosition(700, 340);
	wallList.push_back(w7);

	CSprite* w8 = new CSprite();
	w8->LoadImage("Right wall.png", "Brick", CColor::White());
	w8->SetImage("Brick");
	w8->SetPosition(700, 380);
	wallList.push_back(w8);

	CSprite* w9 = new CSprite();
	w9->LoadImage("Right wall.png", "Brick", CColor::White());
	w9->SetImage("Brick");
	w9->SetPosition(700, 420);
	wallList.push_back(w9);

	CSprite* w10 = new CSprite();
	w10->LoadImage("Right wall.png", "Brick", CColor::White());
	w10->SetImage("Brick");
	w10->SetPosition(700, 460);
	wallList.push_back(w10);

	CSprite* w11 = new CSprite();
	w11->LoadImage("Right wall.png", "Brick", CColor::White());
	w11->SetImage("Brick");
	w11->SetPosition(700, 500);
	wallList.push_back(w11);

	CSprite* w12 = new CSprite();
	w12->LoadImage("Walls.png", "Brick", CColor::White());
	w12->SetImage("Brick");
	w12->SetPosition(730, 510);
	wallList.push_back(w12);

	CSprite* w13 = new CSprite();
	w13->LoadImage("Walls.png", "Brick", CColor::White());
	w13->SetImage("Brick");
	w13->SetPosition(763, 510);
	wallList.push_back(w13);

	CSprite* w14 = new CSprite();
	w14->LoadImage("walls.png", "Brick", CColor::White());
	w14->SetImage("Brick");
	w14->SetPosition(665, 170);
	wallList.push_back(w14);

	CSprite* w15 = new CSprite();
	w15->LoadImage("walls.png", "Brick", CColor::White());
	w15->SetImage("Brick");
	w15->SetPosition(625, 170);
	wallList.push_back(w15);

	CSprite* w16 = new CSprite();
	w16->LoadImage("walls.png", "Brick", CColor::White());
	w16->SetImage("Brick");
	w16->SetPosition(585, 170);
	wallList.push_back(w16);

	CSprite* w17 = new CSprite();
	w17->LoadImage("walls.png", "Brick", CColor::White());
	w17->SetImage("Brick");
	w17->SetPosition(535, 170);
	w17->SetSize(70, 50);
	wallList.push_back(w17);

	CSprite* w18 = new CSprite();
	w18->LoadImage("walls.png", "Brick", CColor::White());
	w18->SetImage("Brick");
	w18->SetPosition(480, 170);
	w18->SetSize(70, 50);
	wallList.push_back(w18);

	CSprite* w19 = new CSprite();
	w19->LoadImage("walls.png", "Brick", CColor::White());
	w19->SetImage("Brick");
	w19->SetPosition(350, 170);
	w19->SetSize(70, 50);
	wallList.push_back(w19);

	CSprite* w20 = new CSprite();
	w20->LoadImage("Right wall.png", "Brick", CColor::White());
	w20->SetImage("Brick");
	w20->SetPosition(470, 135);
	w20->SetSize(70, 50);
	wallList.push_back(w20);

	CSprite* w21 = new CSprite();
	w21->LoadImage("Right wall.png", "Brick", CColor::White());
	w21->SetImage("Brick");
	w21->SetPosition(470, 100);
	w21->SetSize(70, 50);
	wallList.push_back(w21);

	CSprite* w22 = new CSprite();
	w22->LoadImage("Right wall.png", "Brick", CColor::White());
	w22->SetImage("Brick");
	w22->SetPosition(364, 100);
	w22->SetSize(70, 50);
	wallList.push_back(w22);

	CSprite* w23 = new CSprite();
	w23->LoadImage("Right wall.png", "Brick", CColor::White());
	w23->SetImage("Brick");
	w23->SetPosition(364, 135);
	w23->SetSize(70, 50);
	wallList.push_back(w23);

	CSprite* w24 = new CSprite();
	w24->LoadImage("walls.png", "Brick", CColor::White());
	w24->SetImage("Brick");
	w24->SetPosition(350, 70);
	w24->SetSize(70, 50);
	wallList.push_back(w24);

	CSprite* w25 = new CSprite();
	w25->LoadImage("walls.png", "Brick", CColor::White());
	w25->SetImage("Brick");
	w25->SetPosition(300, 70);
	w25->SetSize(70, 50);
	wallList.push_back(w25);

	CSprite* w26 = new CSprite();
	w26->LoadImage("walls.png", "Brick", CColor::White());
	w26->SetImage("Brick");
	w26->SetPosition(250, 70);
	w26->SetSize(70, 50);
	wallList.push_back(w26);

	CSprite* w27 = new CSprite();
	w27->LoadImage("walls.png", "Brick", CColor::White());
	w27->SetImage("Brick");
	w27->SetPosition(200, 70);
	w27->SetSize(70, 50);
	wallList.push_back(w27);

	CSprite* w28 = new CSprite();
	w28->LoadImage("walls.png", "Brick", CColor::White());
	w28->SetImage("Brick");
	w28->SetPosition(150, 70);
	w28->SetSize(70, 50);
	wallList.push_back(w28);

	CSprite* w29 = new CSprite();
	w29->LoadImage("walls.png", "Brick", CColor::White());
	w29->SetImage("Brick");
	w29->SetPosition(100, 70);
	w29->SetSize(70, 50);
	wallList.push_back(w29);

	CSprite* w30 = new CSprite();
	w30->LoadImage("walls.png", "Brick", CColor::White());
	w30->SetImage("Brick");
	w30->SetPosition(48, 70);
	w30->SetSize(70, 50);
	wallList.push_back(w30);

	CSprite* w31 = new CSprite();
	w31->LoadImage("Right wall.png", "Brick", CColor::White());
	w31->SetImage("Brick");
	w31->SetPosition(100, 563);
	w31->SetSize(70, 50);
	wallList.push_back(w31);

	CSprite* w32 = new CSprite();
	w32->LoadImage("Right wall.png", "Brick", CColor::White());
	w32->SetImage("Brick");
	w32->SetPosition(100,523);
	w32->SetSize(70, 50);
	wallList.push_back(w32);

	CSprite* w33 = new CSprite();
	w33->LoadImage("Right wall.png", "Brick", CColor::White());
	w33->SetImage("Brick");
	w33->SetPosition(100, 483);
	w33->SetSize(70, 50);
	wallList.push_back(w33);

	CSprite* w34 = new CSprite();
	w34->LoadImage("Right wall.png", "Brick", CColor::White());
	w34->SetImage("Brick");
	w34->SetPosition(100, 443);
	w34->SetSize(70, 50);
	wallList.push_back(w34);

	CSprite* w35 = new CSprite();
	w35->LoadImage("Right wall.png", "Brick", CColor::White());
	w35->SetImage("Brick");
	w35->SetPosition(100, 403);
	w35->SetSize(70, 50);
	wallList.push_back(w35);

	CSprite* w36 = new CSprite();
	w36->LoadImage("Right wall.png", "Brick", CColor::White());
	w36->SetImage("Brick");
	w36->SetPosition(100, 363);
	w36->SetSize(70, 50);
	wallList.push_back(w36);

	CSprite* w37 = new CSprite();
	w37->LoadImage("Right wall.png", "Brick", CColor::White());
	w37->SetImage("Brick");
	w37->SetPosition(100, 323);
	w37->SetSize(70, 50);
	wallList.push_back(w37);

	CSprite* w38 = new CSprite();
	w38->LoadImage("Right wall.png", "Brick", CColor::White());
	w38->SetImage("Brick");
	w38->SetPosition(100, 283);
	w38->SetSize(70, 50);
	wallList.push_back(w38);

	CSprite* w39 = new CSprite();
	w39->LoadImage("Right wall.png", "Brick", CColor::White());
	w39->SetImage("Brick");
	w39->SetPosition(100, 243);
	w39->SetSize(70, 50);
	wallList.push_back(w39);

	CSprite* w40 = new CSprite();
	w40->LoadImage("Right wall.png", "Brick", CColor::White());
	w40->SetImage("Brick");
	w40->SetPosition(100, 203);
	w40->SetSize(70, 50);
	wallList.push_back(w40);

	CSprite* w41 = new CSprite();
	w41->LoadImage("walls.png", "Brick", CColor::White());
	w41->SetImage("Brick");
	w41->SetPosition(110, 171);
	w41->SetSize(70, 50);
	wallList.push_back(w41);
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
