#include "stdafx.h"
#include "MyGame.h"

float Coords[][2] =
{
	{ 352, 160 },
	{ 480, 160 },
	{ 608, 288 },
	{ 608, 96 },
	{ 992, 288 },
	{ 912, 96 },
	{ 992, 160 },
	{ 1184, 160 },
	{ 992, 480 },
	{ 1184, 480 },
	{ 1184, 672 },
	{ 800, 672 },
	{ 736, 480 },
	{ 544, 480 },
	{ 512, 672 },
	{ 96, 672 },
	{ 96, 416 },
	{ 352, 416 },
};


int Connections[][2] = {
	{ 0, 1 },
	{ 0, 17 },
	{ 1, 2 },
	{ 1, 3 },
	{ 2, 3 },
	{ 2, 4 },
	{ 3, 5 },
	{ 4, 5 },
	{ 4, 6 },
	{ 4, 8 },
	{ 5, 6 },
	{ 6, 7 },
	{ 7, 9 },
	{ 8, 9 },
	{ 9, 10 },
	{ 10, 11 },
	{ 11, 12 },
	{ 11, 14 },
	{ 12, 13 },
	{ 13, 14 },
	{ 14, 15 },
	{ 15, 16 },
	{ 16, 17 },
};
CMyGame::CMyGame(void)
	// to initialise more sprites here use a comma-separated list
{
	for (float* coord : Coords)
		m_graph.push_back(NODE{ CVector(coord[0], coord[1]) });

	for (int* conn : Connections)
	{
		int ind1 = conn[0];
		int ind2 = conn[1];
		NODE& node1 = m_graph[ind1];
		NODE& node2 = m_graph[ind2];
		float dist = Distance(node1.pos, node2.pos);

		node1.conlist.push_back(CONNECTION{ ind2, dist });
		node2.conlist.push_back(CONNECTION{ ind1, dist });
	}
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

	// NPC: follow the waypoints
	if (!m_waypoints.empty())
	{
		// If NPC not moving, start moving to the first waypoint
		if (minotaur.GetSpeed() < 1)
		{
			minotaur.SetSpeed(500);
			minotaur.SetDirection(m_waypoints.front() - minotaur.GetPosition());
			minotaur.SetRotation(minotaur.GetDirection() - 90);
		}

		// Passed the waypoint?
		CVector v = m_waypoints.front() - minotaur.GetPosition();
		if (Dot(minotaur.GetVelocity(), v) < 0)
		{
			// Stop movement
			m_waypoints.pop_front();
			if (m_waypoints.empty())
			minotaur.SetVelocity(0, 0);
			minotaur.SetRotation(0);
		}
	}
	// TODO: add the game update code here

	m_sprite.Update(t);	// this will update the sample rocket sprite
	for (CSprite* w : wallList)
	{
		w->Update(t);
	}


	player.Update(t);	// this will update the sample rocket sprite
	minotaur.Update(t);

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

	for (CSprite* w : wallList)
	{
		if (player.HitTest(w))
		{
			player.SetVelocity(0, 0);

			if (IsKeyDown(SDLK_s) || IsKeyDown(SDLK_DOWN))
			{
				player.SetPos(player.GetX(), player.GetY() + 1);
			}
			if (IsKeyDown(SDLK_w) || IsKeyDown(SDLK_UP))
			{
				player.SetPos(player.GetX(), player.GetY() - 1);
			}
			if (IsKeyDown(SDLK_a) || IsKeyDown(SDLK_RIGHT))
			{
				player.SetPos(player.GetX() + 1, player.GetY());
			}
			if (IsKeyDown(SDLK_d) || IsKeyDown(SDLK_RIGHT))
			{
				player.SetPos(player.GetX() - 1, player.GetY());
			}
		}
	}
	CVector playerPos(player.GetX(), player.GetY());

	// Only recalc path if Minotaur has no waypoints
	if (m_waypoints.empty())
	{
		int nStart = min_element(m_graph.begin(), m_graph.end(),
			[this](NODE& n1, NODE& n2) { return Distance(n1.pos, minotaur.GetPos()) < Distance(n2.pos, minotaur.GetPos()); }) - m_graph.begin();

		int nGoal = min_element(m_graph.begin(), m_graph.end(),
			[playerPos](NODE& n1, NODE& n2) { return Distance(n1.pos, playerPos) < Distance(n2.pos, playerPos); }) - m_graph.begin();

		vector<int> path;
		if (PathFind(m_graph, nStart, nGoal, path))
		{
			for (int i : path)
				m_waypoints.push_back(m_graph[i].pos);

			m_waypoints.push_back(playerPos); // final destination
		}
	}

	// --- Minotaur Movement ---
	if (!m_waypoints.empty())
	{
		CVector dir = m_waypoints.front() - minotaur.GetPos();
		float dist = dir.Length();

		if (dist < 5.f) // reached waypoint
		{
			m_waypoints.pop_front();
		}
		else
		{
			minotaur.SetDirection(dir);
			minotaur.SetSpeed(200);
			minotaur.SetRotation(minotaur.GetDirection() - 90);
		}
	}
	else
	{
		minotaur.SetVelocity(0, 0);
	}
}
bool PathFind(vector<NODE>& graph, int nStart, int nGoal, vector<int>& path)
{
	// ----- INITIALISE ALL NODES -----
	for (NODE& n : graph)
	{
		n.costSoFar = INFINITY;
		n.nConnection = -1;
		n.open = false;
		n.closed = false;
	}

	// ----- START NODE -----
	graph[nStart].costSoFar = 0;
	graph[nStart].open = true;

	// OPEN LIST = list of node indices
	vector<int> openList;
	openList.push_back(nStart);

	// ----- MAIN LOOP -----
	while (!openList.empty())
	{
		// find node in open list with smallest cost
		int current = openList[0];
		for (int n : openList)
			if (graph[n].costSoFar < graph[current].costSoFar)
				current = n;

		// goal reached
		if (current == nGoal)
			break;

		// explore neighbours
		for (const CONNECTION& c : graph[current].conlist)
		{
			int next = c.nEnd;
			float newCost = graph[current].costSoFar + c.cost;

			// if next is closed AND its saved cost is better → skip
			if (graph[next].closed && graph[next].costSoFar <= newCost)
				continue;

			// found a better path?
			if (newCost < graph[next].costSoFar)
			{
				graph[next].costSoFar = newCost;
				graph[next].nConnection = current;

				// add to open list if not already added
				if (!graph[next].open)
				{
					graph[next].open = true;
					openList.push_back(next);
				}
			}
		}

		// close current
		graph[current].closed = true;
		graph[current].open = false;

		// remove from open list
		openList.erase(remove(openList.begin(), openList.end(), current), openList.end());
	}

	// ----- NO PATH FOUND -----
	if (graph[nGoal].nConnection == -1)
		return false;

	// ----- BUILD FINAL PATH -----
	path.clear();

	int current = nGoal;
	while (current != -1)
	{
		path.push_back(current);
		current = graph[current].nConnection;
	}

	reverse(path.begin(), path.end());
	return true;
}
void CMyGame::OnDraw(CGraphics* g)
{
	// TODO: add drawing code here
	player.Draw(g);	// this will draw the sample rocket sprite
	minotaur.Draw(g);

	for (CSprite* w : wallList)
	{
		w->Draw(g);
	}
	for (NODE n : m_graph)
		for (CONNECTION c : n.conlist)
			g->DrawLine(n.pos, m_graph[c.nEnd].pos, CColor::Black());



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

	minotaur.LoadImage("Minotaur.png", "idle", CColor::White());
	minotaur.SetImage("idle");
	
}
void CMyGame::OnWalls()
{
	wallList.clear();

	// Wall 0
	CSprite* topwall = new CSprite();
	topwall->LoadImage("Top_wall.png", "Brick", CColor::White());
	topwall->SetImage("Brick");
	topwall->SetPosition(400, 592);
	topwall->SetProperty("tag", "top");
	wallList.push_back(topwall);

	CSprite* bottomwall = new CSprite();
	bottomwall->LoadImage("Top_wall.png", "Brick", CColor::White());
	bottomwall->SetImage("Brick");
	bottomwall->SetPosition(400, 8);
	bottomwall->SetProperty("tag", "bottom");
	wallList.push_back(bottomwall);

	CSprite* leftwall = new CSprite();
	leftwall->LoadImage("Left_wall.png", "Brick", CColor::White());
	leftwall->SetImage("Brick");
	leftwall->SetPosition(8, 290);
	leftwall->SetProperty("tag", "left");
	wallList.push_back(leftwall);

	CSprite* rightwall = new CSprite();
	rightwall->LoadImage("Right_wall.png", "Brick", CColor::White());
	rightwall->SetImage("Brick");
	rightwall->SetPosition(792, 305);
	rightwall->SetProperty("tag", "right");
	wallList.push_back(rightwall);

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
	wallList.push_back(w17);

	CSprite* w18 = new CSprite();
	w18->LoadImage("walls.png", "Brick", CColor::White());
	w18->SetImage("Brick");
	w18->SetPosition(480, 170);
	wallList.push_back(w18);

	CSprite* w19 = new CSprite();
	w19->LoadImage("walls.png", "Brick", CColor::White());
	w19->SetImage("Brick");
	w19->SetPosition(350, 170);
	wallList.push_back(w19);

	CSprite* w20 = new CSprite();
	w20->LoadImage("Right wall.png", "Brick", CColor::White());
	w20->SetImage("Brick");
	w20->SetPosition(470, 135);
	wallList.push_back(w20);

	CSprite* w21 = new CSprite();
	w21->LoadImage("Right wall.png", "Brick", CColor::White());
	w21->SetImage("Brick");
	w21->SetPosition(470, 100);
	wallList.push_back(w21);

	CSprite* w22 = new CSprite();
	w22->LoadImage("Right wall.png", "Brick", CColor::White());
	w22->SetImage("Brick");
	w22->SetPosition(364, 100);
	wallList.push_back(w22);

	CSprite* w23 = new CSprite();
	w23->LoadImage("Right wall.png", "Brick", CColor::White());
	w23->SetImage("Brick");
	w23->SetPosition(364, 135);
	wallList.push_back(w23);

	CSprite* w24 = new CSprite();
	w24->LoadImage("walls.png", "Brick", CColor::White());
	w24->SetImage("Brick");
	w24->SetPosition(350, 70);
	wallList.push_back(w24);

	CSprite* w25 = new CSprite();
	w25->LoadImage("walls.png", "Brick", CColor::White());
	w25->SetImage("Brick");
	w25->SetPosition(300, 70);
	wallList.push_back(w25);

	CSprite* w26 = new CSprite();
	w26->LoadImage("walls.png", "Brick", CColor::White());
	w26->SetImage("Brick");
	w26->SetPosition(250, 70);
	wallList.push_back(w26);

	CSprite* w27 = new CSprite();
	w27->LoadImage("walls.png", "Brick", CColor::White());
	w27->SetImage("Brick");
	w27->SetPosition(200, 70);
	wallList.push_back(w27);

	CSprite* w28 = new CSprite();
	w28->LoadImage("walls.png", "Brick", CColor::White());
	w28->SetImage("Brick");
	w28->SetPosition(150, 70);
	wallList.push_back(w28);

	CSprite* w29 = new CSprite();
	w29->LoadImage("walls.png", "Brick", CColor::White());
	w29->SetImage("Brick");
	w29->SetPosition(100, 70);
	wallList.push_back(w29);

	CSprite* w30 = new CSprite();
	w30->LoadImage("walls.png", "Brick", CColor::White());
	w30->SetImage("Brick");
	w30->SetPosition(48, 70);
	wallList.push_back(w30);

	CSprite* w31 = new CSprite();
	w31->LoadImage("Right wall.png", "Brick", CColor::White());
	w31->SetImage("Brick");
	w31->SetPosition(100, 563);
	wallList.push_back(w31);

	CSprite* w32 = new CSprite();
	w32->LoadImage("Right wall.png", "Brick", CColor::White());
	w32->SetImage("Brick");
	w32->SetPosition(100,523);
	wallList.push_back(w32);

	CSprite* w33 = new CSprite();
	w33->LoadImage("Right wall.png", "Brick", CColor::White());
	w33->SetImage("Brick");
	w33->SetPosition(100, 483);
	wallList.push_back(w33);

	CSprite* w34 = new CSprite();
	w34->LoadImage("Right wall.png", "Brick", CColor::White());
	w34->SetImage("Brick");
	w34->SetPosition(100, 443);
	wallList.push_back(w34);

	CSprite* w35 = new CSprite();
	w35->LoadImage("Right wall.png", "Brick", CColor::White());
	w35->SetImage("Brick");
	w35->SetPosition(100, 403);
	wallList.push_back(w35);

	CSprite* w36 = new CSprite();
	w36->LoadImage("Right wall.png", "Brick", CColor::White());
	w36->SetImage("Brick");
	w36->SetPosition(100, 363);
	wallList.push_back(w36);

	CSprite* w37 = new CSprite();
	w37->LoadImage("Right wall.png", "Brick", CColor::White());
	w37->SetImage("Brick");
	w37->SetPosition(100, 323);
	wallList.push_back(w37);

	CSprite* w38 = new CSprite();
	w38->LoadImage("Right wall.png", "Brick", CColor::White());
	w38->SetImage("Brick");
	w38->SetPosition(100, 283);
	wallList.push_back(w38);

	CSprite* w39 = new CSprite();
	w39->LoadImage("Right wall.png", "Brick", CColor::White());
	w39->SetImage("Brick");
	w39->SetPosition(100, 243);
	wallList.push_back(w39);

	CSprite* w40 = new CSprite();
	w40->LoadImage("Right wall.png", "Brick", CColor::White());
	w40->SetImage("Brick");
	w40->SetPosition(100, 203);
	wallList.push_back(w40);

	CSprite* w41 = new CSprite();
	w41->LoadImage("walls.png", "Brick", CColor::White());
	w41->SetImage("Brick");
	w41->SetPosition(110, 171);
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
	minotaur.SetPos(600, 80);

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
