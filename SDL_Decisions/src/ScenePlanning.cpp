#include "ScenePlanning.h"

using namespace std;

ScenePlanning::ScenePlanning()
{
	draw_grid = false;

	num_cell_x = SRC_WIDTH / CELL_SIZE;
	num_cell_y = SRC_HEIGHT / CELL_SIZE;
	initMaze();
	loadTextures("../res/maze.png", "../res/coin.png");

	srand((unsigned int)time(NULL));

	Agent *agent = new Agent;
	agent->loadSpriteTexture("../res/soldier.png", 4);
	agents.push_back(agent);


	// set agent position coords to the center of a random cell
	Vector2D rand_cell(-1,-1);
	while (!isValidCell(rand_cell)) 
		rand_cell = Vector2D((float)(rand() % num_cell_x), (float)(rand() % num_cell_y));
	agents[0]->setPosition(cell2pix(rand_cell));

	// set the coin in a random cell (but at least 3 cells far from the agent)
	coinPosition = Vector2D(-1,-1);
	while ((!isValidCell(coinPosition)) || (Vector2D::Distance(coinPosition, rand_cell)<3)) 
		coinPosition = Vector2D((float)(rand() % num_cell_x), (float)(rand() % num_cell_y));
	
	// PathFollowing next Target
	currentTarget = Vector2D(0, 0);
	currentTargetIndex = -1;
	agents[0]->currState = agents[0]->mine;


	//mina 1134 / 80

	mina = { 1134,80 };

	//bank = (pix2cell(207.) 206 / 623
	//bank = (pix2cell(206), pix2cell(623));

	bank = { 206, 623 };


	//home 624 / 594

	//home = (pix2cell(624), pix2cell(594));
	home = { 624, 594 };

	//saloon 1072 /621

	//saloon = (pix2cell(1072),pix2cell(621));

	saloon = {1072,621};


	
}

ScenePlanning::~ScenePlanning()
{
	if (background_texture)
		SDL_DestroyTexture(background_texture);
	if (coin_texture)
		SDL_DestroyTexture(coin_texture);

	for (int i = 0; i < (int)agents.size(); i++)
	{
		delete agents[i];
	}
}

void ScenePlanning::update(float dtime, SDL_Event *event)
{
	/* Keyboard & Mouse events */
	switch (event->type) {
	case SDL_KEYDOWN:
		if (event->key.keysym.scancode == SDL_SCANCODE_SPACE)
			draw_grid = !draw_grid;
		break;
	case SDL_MOUSEMOTION:
	case SDL_MOUSEBUTTONDOWN:
		if (event->button.button == SDL_BUTTON_LEFT)
		{
			Vector2D cell = pix2cell(Vector2D((float)(event->button.x), (float)(event->button.y)));
			if (isValidCell(cell))
			{
				if (path.points.size() > 0)
					if (path.points[path.points.size() - 1] == cell2pix(cell))
						break;

				path.points.push_back(cell2pix(cell));
			}
		}
		break;
	default:
		break;
	}
	if ((currentTargetIndex == -1) && (path.points.size()>0))
		currentTargetIndex = 0;

	if (currentTargetIndex >= 0)
	{	
		float dist = Vector2D::Distance(agents[0]->getPosition(), path.points[currentTargetIndex]);
		if (dist < path.ARRIVAL_DISTANCE)
		{
			if (currentTargetIndex == path.points.size() - 1)
			{
				if (dist < 3)
				{
					path.points.clear();
					currentTargetIndex = -1;
					agents[0]->setVelocity(Vector2D(0,0));
					agents[0]->currState->Enter(agents[0]);
					//std::cout << "POSICIO" << endl;
					/*std::cout << agents[0]->getPosition().x << endl;
					std::cout << agents[0]->getPosition().y << endl;*/
					// if we have arrived to the coin, replace it ina random cell!
					if (pix2cell(agents[0]->getPosition()) == coinPosition)
					{
						coinPosition = Vector2D(-1, -1);
						while ((!isValidCell(coinPosition)) || (Vector2D::Distance(coinPosition, pix2cell(agents[0]->getPosition()))<3))
							coinPosition = Vector2D((float)(rand() % num_cell_x), (float)(rand() % num_cell_y));
					}
				}
				else
				{
					Vector2D steering_force = agents[0]->Behavior()->Arrive(agents[0], currentTarget, path.ARRIVAL_DISTANCE, dtime);
					agents[0]->update(steering_force, dtime, event);
				}
				return;
			}
			currentTargetIndex++;
		}

		currentTarget = path.points[currentTargetIndex];
		Vector2D steering_force = agents[0]->Behavior()->Seek(agents[0], currentTarget, dtime);
		agents[0]->update(steering_force, dtime, event);
	} 
	else
	{
		agents[0]->update(Vector2D(0,0), dtime, event);
		agents[0]->currState->Update(agents[0]);

		switch (agents[0]->currState->state)
		{
		case STATE::MINE:
			if (agents[0]->full) {
				path.points.push_back(bank);
			}
			if (agents[0]->thirsty) {
				path.points.push_back(saloon);
			}
			break;
		case STATE::BANK:
			if (!agents[0]->rested) {
				path.points.push_back(home);
			}
			if (!agents[0]->wealth) {
				path.points.push_back(mina);
			}
			break;
		case STATE::SALOON:
			if (!agents[0]->thirsty) {
				path.points.push_back(mina);
			}
			break;
		case STATE::HOME:
			if (agents[0]->rested) {
				path.points.push_back(mina);
			}
			break;
		default:
			break;
		}
		
	}


	

}
	

void ScenePlanning::draw()
{
	drawMaze();
	drawCoin();


	if (draw_grid)
	{
		SDL_SetRenderDrawColor(TheApp::Instance()->getRenderer(), 255, 255, 255, 127);
		for (int i = 0; i < SRC_WIDTH; i+=CELL_SIZE)
		{
			SDL_RenderDrawLine(TheApp::Instance()->getRenderer(), i, 0, i, SRC_HEIGHT);
		}
		for (int j = 0; j < SRC_HEIGHT; j = j += CELL_SIZE)
		{
			SDL_RenderDrawLine(TheApp::Instance()->getRenderer(), 0, j, SRC_WIDTH, j);
		}
	}

	for (int i = 0; i < (int)path.points.size(); i++)
	{
		draw_circle(TheApp::Instance()->getRenderer(), (int)(path.points[i].x), (int)(path.points[i].y), 15, 255, 255, 0, 255);
		if (i > 0)
			SDL_RenderDrawLine(TheApp::Instance()->getRenderer(), (int)(path.points[i - 1].x), (int)(path.points[i - 1].y), (int)(path.points[i].x), (int)(path.points[i].y));
	}

	draw_circle(TheApp::Instance()->getRenderer(), (int)currentTarget.x, (int)currentTarget.y, 15, 255, 0, 0, 255);

	agents[0]->draw();
}

const char* ScenePlanning::getTitle()
{
	return "SDL Steering Behaviors :: PathFinding1 Demo";
}

void ScenePlanning::drawMaze()
{
	if (draw_grid)
	{

		SDL_SetRenderDrawColor(TheApp::Instance()->getRenderer(), 0, 0, 255, 255);
		for (unsigned int i = 0; i < maze_rects.size(); i++)
			SDL_RenderFillRect(TheApp::Instance()->getRenderer(), &maze_rects[i]);
	}
	else
	{
		//SDL_RenderCopy(TheApp::Instance()->getRenderer(), background_texture, NULL, NULL );
	}
}

void ScenePlanning::drawCoin()
{
	Vector2D coin_coords = cell2pix(coinPosition);
	int offset = CELL_SIZE / 2;
	SDL_Rect dstrect = {(int)coin_coords.x-offset, (int)coin_coords.y - offset, CELL_SIZE, CELL_SIZE};
	SDL_RenderCopy(TheApp::Instance()->getRenderer(), coin_texture, NULL, &dstrect);
}

void ScenePlanning::initMaze()
{

	// Initialize a list of Rectagles describing the maze geometry (useful for collision avoidance)
	SDL_Rect rect = { 0, 0, 1280, 32 };
	maze_rects.push_back(rect);
	rect = { 128, 160, 1152, 32 };
	maze_rects.push_back(rect);
	rect = { 0, 736, 1280, 32 };
	maze_rects.push_back(rect);
	
	rect = { 0,32,32,736 };
	maze_rects.push_back(rect);
	rect = { 1248,32,32,736 };
	maze_rects.push_back(rect);

	rect = { 416,512,32,268 };
	maze_rects.push_back(rect);
	rect = { 832,512,32,268 };
	maze_rects.push_back(rect);
	
	rect = { 32,480,128,32 };
	maze_rects.push_back(rect);
	rect = { 288,480,288,32 };
	maze_rects.push_back(rect);
	rect = { 704,480,288,32 };
	maze_rects.push_back(rect);
	rect = { 1120,480,128,32 };
	maze_rects.push_back(rect);

	// Initialize the terrain matrix (for each cell a zero value indicates it's a wall)
	
	// (1st) initialize all cells to 1 by default
	for (int i = 0; i < num_cell_x; i++)
	{
		vector<int> terrain_col(num_cell_y, 1); 
		terrain.push_back(terrain_col);
	}
	// (2nd) set to zero all cells that belong to a wall
	int offset = CELL_SIZE / 2;
	for (int i = 0; i < num_cell_x; i++)
	{
		for (int j = 0; j < num_cell_y; j++)
		{
			Vector2D cell_center ((float)(i*CELL_SIZE + offset), (float)(j*CELL_SIZE + offset));
			for (unsigned int b = 0; b < maze_rects.size(); b++)
			{
				if (Vector2DUtils::IsInsideRect(cell_center, (float)maze_rects[b].x, (float)maze_rects[b].y, (float)maze_rects[b].w, (float)maze_rects[b].h))
				{
					terrain[i][j] = 0;
				    break;
				}  
			}
			
		}
	}

}

bool ScenePlanning::loadTextures(char* filename_bg, char* filename_coin)
{
	SDL_Surface *image = IMG_Load(filename_bg);
	if (!image) {
		cout << "IMG_Load: " << IMG_GetError() << endl;
		return false;
	}
	background_texture = SDL_CreateTextureFromSurface(TheApp::Instance()->getRenderer(), image);

	if (image)
		SDL_FreeSurface(image);

	image = IMG_Load(filename_coin);
	if (!image) {
		cout << "IMG_Load: " << IMG_GetError() << endl;
		return false;
	}
	coin_texture = SDL_CreateTextureFromSurface(TheApp::Instance()->getRenderer(), image);

	if (image)
		SDL_FreeSurface(image);

	return true;
}

Vector2D ScenePlanning::cell2pix(Vector2D cell)
{
	int offset = CELL_SIZE / 2;
	return Vector2D(cell.x*CELL_SIZE + offset, cell.y*CELL_SIZE + offset);
}

Vector2D ScenePlanning::pix2cell(Vector2D pix)
{
	return Vector2D((float)((int)pix.x/CELL_SIZE), (float)((int)pix.y / CELL_SIZE));
}

bool ScenePlanning::isValidCell(Vector2D cell)
{
	if ((cell.x < 0) || (cell.y < 0) || (cell.x >= terrain.size()) || (cell.y >= terrain[0].size()) )
		return false;
	return !(terrain[(unsigned int)cell.x][(unsigned int)cell.y] == 0);
}

void ScenePlanning::AEstrella(Node inici, Node final)
{
	priority_queue<Node, vector<Node>, LessThanByPriority> frontera;
	int newCost;
	from.clear();
	from.resize(num_cell_x, vector<Node>(num_cell_y));
	frontera.push(inici);
	vector<vector<bool>> visitades(terrain.size(), vector<bool>(terrain[0].size()));
	for (int i = 0; i < terrain.size(); i++) {
		for (int j = 0; j < terrain[0].size(); j++) {
			visitades[i][j] = false;
		}
	}
	for (int i = 0; i < num_cell_x; i++) {
		for (int j = 0; j < num_cell_y; j++) {
			from[i][j].accumulat = -1;
		}
	}
	path.points.clear();

	while (!frontera.empty()) {

		Node nodeActual = frontera.top();
		currX = nodeActual.position.x;
		currY = nodeActual.position.y;
		nodeActual.fromNode = Vector2D(-1, -1);
		frontera.pop();
		std::vector<Node> neighbors = graph.getConnections(nodeActual);
		from[currX][currY].visited = true;

		for (int i = 0; i < neighbors.size(); i++) {
			newCost = from[currX][currY].accumulat + from[neighbors[i].position.x][neighbors[i].position.y].cost;
			if (from[neighbors[i].position.x][neighbors[i].position.y].accumulat == -1 || newCost < from[neighbors[i].position.x][neighbors[i].position.y].accumulat) {
				from[neighbors[i].position.x][neighbors[i].position.y].accumulat = newCost;

				neighbors[i].heuristic_distance = pow((final.position.x - neighbors[i].position.x), 2) + pow((final.position.y - neighbors[i].position.y), 2);
				neighbors[i].priority = newCost + neighbors[i].heuristic_distance;

				from[neighbors[i].position.x][neighbors[i].position.y].fromNode = Vector2D(nodeActual.position.x, nodeActual.position.y);
				frontera.push(neighbors[i]);

				//cout << "x: " << neighbors[i].x << " y: " << neighbors[i].y << "cost: " << cameFrom[neighbors[i].x][neighbors[i].y].acumulatedCost << endl;

				if (Vector2D(neighbors[i].position.x, neighbors[i].position.y) == Vector2D(final.position.x, final.position.y)) {
					nodeActual = final;
					path.points.push_back(cell2pix(Vector2D(nodeActual.position.x, nodeActual.position.y)));
					while (Vector2D(nodeActual.position.x, nodeActual.position.y) != Vector2D(inici.position.x, inici.position.y)) {
						currX = nodeActual.position.x;
						currY = nodeActual.position.y;
						nodeActual.position.x = from[currX][currY].fromNode.x;
						//std::cout << nodeActual.x << endl;
						nodeActual.position.y = from[currX][currY].fromNode.y;
						//std::cout << nodeActual.y << endl;
						path.points.insert(path.points.begin(), cell2pix(Vector2D(nodeActual.position.x, nodeActual.position.y)));
					}
					path.points.insert(path.points.begin(), cell2pix(Vector2D(inici.position.x, final.position.y)));
					std::cout << "Moneda trobada!" << endl;
					return;
				}
			}
		}
	}
}