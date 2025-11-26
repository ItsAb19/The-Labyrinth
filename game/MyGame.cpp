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
	key.Update(t);
	door.Update(t);

	if (IsKeyDown(SDLK_d) || IsKeyDown(SDLK_RIGHT))
	{
		player.SetVelocity(100, 0);
	}
	if (IsKeyDown(SDLK_a) || IsKeyDown(SDLK_LEFT))
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
			if (IsKeyDown(SDLK_d) || IsKeyDown(SDLK_LEFT))
			{
				player.SetPos(player.GetX() - 1, player.GetY());
			}
		}
	}

	CVector playerPos(player.GetX(), player.GetY());


	// --- KEY PICKUP ---
	
	if (!hasKey && player.HitTest(&key)) 
	{ 
		hasKey = true; 
	}
	// --- DOOR BLOCKING 
	if (!hasKey && player.HitTest(&door))
	{
		player.SetVelocity(0, 0);

		if (IsKeyDown(SDLK_s) || IsKeyDown(SDLK_DOWN))
			player.SetPos(player.GetX(), player.GetY() + 1);

		if (IsKeyDown(SDLK_w) || IsKeyDown(SDLK_UP))
			player.SetPos(player.GetX(), player.GetY() - 1);

		if (IsKeyDown(SDLK_a) || IsKeyDown(SDLK_LEFT))
			player.SetPos(player.GetX() + 1, player.GetY());

		if (IsKeyDown(SDLK_d) || IsKeyDown(SDLK_RIGHT))
			player.SetPos(player.GetX() - 1, player.GetY());
	}

	// --- DOOR UNLOCK ---
	if (!doorOpen && hasKey && player.HitTest(&door))
	{
		doorOpen = true;
		door.SetPos(-5000, -5000);  // remove door ONLY after you have key
	}


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
	door.Draw(g);
	key.Draw(g);

	for (CSprite* w : wallList)
	{
		w->Draw(g);
	}
	

	if (hasKey)
	{
		*g << top << left << color(CColor::Green()) << "You have the key!";
	}
	else
	{
		*g << top << left << color(CColor::Red()) << "Find the key";
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

	minotaur.LoadImage("Minotaur.png", "idle", CColor::White());
	minotaur.SetImage("idle");

	key.LoadImage("key.png", "gold", CColor::White());
	key.SetImage("gold");
	key.SetSize(50, 50);
	key.SetPos(400, 400);

	door.LoadImage("door.png", "door", CColor::White());
	door.SetImage("door");
	door.SetSize(40, 110);
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

	CSprite* bottomwall2 = new CSprite();
	bottomwall2->LoadImage("LabWallHeight.png", "Brick", CColor::White());
	bottomwall2->SetImage("Brick");
	bottomwall2->SetPosition(100, 357);
	bottomwall2->SetProperty("tag", "right");
	wallList.push_back(bottomwall2);

	CSprite* bottomwall3 = new CSprite();
	bottomwall3->LoadImage("Right wall.png", "Brick", CColor::White());
	bottomwall3->SetImage("Brick");
	bottomwall3->SetPosition(101, 116);
	bottomwall3->SetProperty("tag", "right");
	bottomwall3->SetSize(42, 42);
	wallList.push_back(bottomwall3);

	CSprite* Leftwall2 = new CSprite();
	Leftwall2->LoadImage("LabWallWidth.png", "Brick", CColor::White());
	Leftwall2->SetImage("Brick");
	Leftwall2->SetPosition(215, 100);
	Leftwall2->SetProperty("tag", "right");
	wallList.push_back(Leftwall2);

	

	CSprite* Walls1 = new CSprite();
	Walls1->LoadImage("WidthSmall.png", "Brick", CColor::White());
	Walls1->SetImage("Brick");
	Walls1->SetPosition(290, 200);
	Walls1->SetProperty("tag", "right");
	wallList.push_back(Walls1);

	CSprite* Walls2 = new CSprite();
	Walls2->LoadImage("WidthSmall.png", "Brick", CColor::White());
	Walls2->SetImage("Brick");
	Walls2->SetPosition(220, 200);
	Walls2->SetProperty("tag", "right");
	wallList.push_back(Walls2);

	CSprite* Walls3 = new CSprite();
	Walls3->LoadImage("WidthSmall.png", "Brick", CColor::White());
	Walls3->SetImage("Brick");
	Walls3->SetPosition(158, 290);
	Walls3->SetProperty("tag", "right");
	wallList.push_back(Walls3);

	CSprite* Walls4 = new CSprite();
	Walls4->LoadImage("WidthSmall.png", "Brick", CColor::White());
	Walls4->SetImage("Brick");
	Walls4->SetPosition(258, 290);
	Walls4->SetProperty("tag", "right");
	wallList.push_back(Walls4);

	CSprite* Walls5 = new CSprite();
	Walls5->LoadImage("WidthSmall.png", "Brick", CColor::White());
	Walls5->SetImage("Brick");
	Walls5->SetPosition(358, 290);
	Walls5->SetProperty("tag", "right");
	wallList.push_back(Walls5);

	CSprite* Walls6 = new CSprite();
	Walls6->LoadImage("HeightSmall.png", "Brick", CColor::White());
	Walls6->SetImage("Brick");
	Walls6->SetPosition(430, 140);
	Walls6->SetProperty("tag", "right");
	wallList.push_back(Walls6);

	CSprite* Walls7 = new CSprite();
	Walls7->LoadImage("WidthSmall.png", "Brick", CColor::White());
	Walls7->SetImage("Brick");
	Walls7->SetPosition(468, 200);
	Walls7->SetProperty("tag", "right");
	wallList.push_back(Walls7);

	CSprite* Walls8 = new CSprite();
	Walls8->LoadImage("WidthSmall.png", "Brick", CColor::White());
	Walls8->SetImage("Brick");
	Walls8->SetPosition(568, 200);
	Walls8->SetProperty("tag", "right");
	wallList.push_back(Walls8);

	CSprite* Walls9 = new CSprite();
	Walls9->LoadImage("WidthSmall.png", "Brick", CColor::White());
	Walls9->SetImage("Brick");
	Walls9->SetPosition(640, 200);
	Walls9->SetProperty("tag", "right");
	wallList.push_back(Walls9);

	CSprite* Walls10 = new CSprite();
	Walls10->LoadImage("WidthSmall.png", "Brick", CColor::White());
	Walls10->SetImage("Brick");
	Walls10->SetPosition(490, 100);
	Walls10->SetProperty("tag", "right");
	wallList.push_back(Walls10);

	CSprite* Walls11 = new CSprite();
	Walls11->LoadImage("WidthSmall.png", "Brick", CColor::White());
	Walls11->SetImage("Brick");
	Walls11->SetPosition(540, 100);
	Walls11->SetProperty("tag", "right");
	wallList.push_back(Walls11);

	CSprite* Walls12 = new CSprite();
	Walls12->LoadImage("HeightSmall.png", "Brick", CColor::White());
	Walls12->SetImage("Brick");
	Walls12->SetPosition(680, 140);
	Walls12->SetProperty("tag", "right");
	wallList.push_back(Walls12);

	CSprite* Walls13 = new CSprite();
	Walls13->LoadImage("HeightSmall.png", "Brick", CColor::White());
	Walls13->SetImage("Brick");
	Walls13->SetPosition(680, 40);
	Walls13->SetProperty("tag", "right");
	wallList.push_back(Walls13);

	

	CSprite* Walls15 = new CSprite();
	Walls15->LoadImage("WidthSmall.png", "Brick", CColor::White());
	Walls15->SetImage("Brick");
	Walls15->SetPosition(640, 290);
	Walls15->SetProperty("tag", "right");
	wallList.push_back(Walls15);

	CSprite* Walls16 = new CSprite();
	Walls16->LoadImage("WidthSmall.png", "Brick", CColor::White());
	Walls16->SetImage("Brick");
	Walls16->SetPosition(540, 290);
	Walls16->SetProperty("tag", "right");
	wallList.push_back(Walls16);

	CSprite* Walls17 = new CSprite();
	Walls17->LoadImage("HeightSmall.png", "Brick", CColor::White());
	Walls17->SetImage("Brick");
	Walls17->SetPosition(540, 330);
	Walls17->SetProperty("tag", "right");
	wallList.push_back(Walls17);

	CSprite* Walls18 = new CSprite();
	Walls18->LoadImage("WidthSmall.png", "Brick", CColor::White());
	Walls18->SetImage("Brick");
	Walls18->SetPosition(400, 370);
	Walls18->SetProperty("tag", "right");
	wallList.push_back(Walls18);

	CSprite* Walls19 = new CSprite();
	Walls19->LoadImage("HeightSmall.png", "Brick", CColor::White());
	Walls19->SetImage("Brick");
	Walls19->SetPosition(210, 410);
	Walls19->SetProperty("tag", "right");
	wallList.push_back(Walls19);

	CSprite* Walls20 = new CSprite();
	Walls20->LoadImage("WidthSmall.png", "Brick", CColor::White());
	Walls20->SetImage("Brick");
	Walls20->SetPosition(350, 370);
	Walls20->SetProperty("tag", "right");
	wallList.push_back(Walls20);

	CSprite* Walls21 = new CSprite();
	Walls21->LoadImage("WidthSmall.png", "Brick", CColor::White());
	Walls21->SetImage("Brick");
	Walls21->SetPosition(260, 450);
	Walls21->SetProperty("tag", "right");
	wallList.push_back(Walls21);

	CSprite* Walls22 = new CSprite();
	Walls22->LoadImage("HeightSmall.png", "Brick", CColor::White());
	Walls22->SetImage("Brick");
	Walls22->SetPosition(320, 490);
	Walls22->SetProperty("tag", "right");
	wallList.push_back(Walls22);

	CSprite* Walls23 = new CSprite();
	Walls23->LoadImage("HeightSmall.png", "Brick", CColor::White());
	Walls23->SetImage("Brick");
	Walls23->SetPosition(210, 560);
	Walls23->SetProperty("tag", "right");
	wallList.push_back(Walls23);


	CSprite* Walls24 = new CSprite();
	Walls24->LoadImage("HeightSmall.png", "Brick", CColor::White());
	Walls24->SetImage("Brick");
	Walls24->SetPosition(420, 490);
	Walls24->SetProperty("tag", "right");
	wallList.push_back(Walls24);

	CSprite* Walls25 = new CSprite();
	Walls25->LoadImage("WidthSmall.png", "Brick", CColor::White());
	Walls25->SetImage("Brick");
	Walls25->SetPosition(460, 450);
	Walls25->SetProperty("tag", "right");
	wallList.push_back(Walls25);

	CSprite* Walls26 = new CSprite();
	Walls26->LoadImage("HeightSmall.png", "Brick", CColor::White());
	Walls26->SetImage("Brick");
	Walls26->SetPosition(500, 560);
	Walls26->SetProperty("tag", "right");
	wallList.push_back(Walls26);

	CSprite* Walls27 = new CSprite();
	Walls27->LoadImage("WidthSmall.png", "Brick", CColor::White());
	Walls27->SetImage("Brick");
	Walls27->SetPosition(550, 450);
	Walls27->SetProperty("tag", "right");
	wallList.push_back(Walls27);

	CSprite* Walls28 = new CSprite();
	Walls28->LoadImage("WidthSmall.png", "Brick", CColor::White());
	Walls28->SetImage("Brick");
	Walls28->SetPosition(650, 450);
	Walls28->SetProperty("tag", "right");
	wallList.push_back(Walls28);

	CSprite* Walls29 = new CSprite();
	Walls29->LoadImage("WidthSmall.png", "Brick", CColor::White());
	Walls29->SetImage("Brick");
	Walls29->SetPosition(650, 520);
	Walls29->SetProperty("tag", "right");
	wallList.push_back(Walls29);

	CSprite* Walls30 = new CSprite();
	Walls30->LoadImage("WidthSmall.png", "Brick", CColor::White());
	Walls30->SetImage("Brick");
	Walls30->SetPosition(650, 370);
	Walls30->SetProperty("tag", "right");
	wallList.push_back(Walls30);
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
	player.SetPos(780, 37);
	minotaur.SetPos(600, 80);
	door.SetPos(105, 50);
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
