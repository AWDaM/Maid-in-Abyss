#include "p2Defs.h"
#include "p2Log.h"
#include "j1App.h"
#include "j1PathFinding.h"

j1PathFinding::j1PathFinding() : j1Module(), map(NULL), last_path(DEFAULT_PATH_LENGTH),width(0), height(0)
{
	name.create("pathfinding");
}

// Destructor
j1PathFinding::~j1PathFinding()
{
	RELEASE_ARRAY(map);
}

// Called before quitting
bool j1PathFinding::CleanUp()
{
	LOG("Freeing pathfinding library");

	last_path.Clear();
	RELEASE_ARRAY(map);
	return true;
}

// Sets up the walkability map
void j1PathFinding::SetMap(uint width, uint height, uchar* data)
{
	this->width = width;
	this->height = height;

	RELEASE_ARRAY(map);
	map = new uchar[width*height];
	memcpy(map, data, width*height);
}

// Utility: return true if pos is inside the map boundaries
bool j1PathFinding::CheckBoundaries(const iPoint& pos) const
{
	return (pos.x >= 0 && pos.x <= (int)width &&
			pos.y >= 0 && pos.y <= (int)height);
}

// Utility: returns true is the tile is walkable
bool j1PathFinding::IsWalkable(const iPoint& pos, bool canFly) const
{
	uchar t = GetTileAt(pos);
	if (canFly)
		return t != INVALID_WALK_CODE && t > 0;
	else
		return t != INVALID_WALK_CODE && t == 2;
}

// Utility: return the walkability value of a tile
uchar j1PathFinding::GetTileAt(const iPoint& pos) const
{
	if(CheckBoundaries(pos))
		return map[(pos.y*width) + pos.x];

	return INVALID_WALK_CODE;
}

// To request all tiles involved in the last generated path
const p2DynArray<iPoint>* j1PathFinding::GetLastPath() const
{
	return &last_path;
}

// PathList ------------------------------------------------------------------------
// Looks for a node in this list and returns it's list node or NULL
// ---------------------------------------------------------------------------------
p2List_item<PathNode>* PathList::Find(const iPoint& point) const
{
	p2List_item<PathNode>* item = list.start;
	while(item)
	{
		if(item->data.pos == point)
			return item;
		item = item->next;
	}
	return NULL;
}

// PathList ------------------------------------------------------------------------
// Returns the Pathnode with lowest score in this list or NULL if empty
// ---------------------------------------------------------------------------------
p2List_item<PathNode>* PathList::GetNodeLowestScore() const
{
	p2List_item<PathNode>* ret = NULL;
	int min = 65535;

	p2List_item<PathNode>* item = list.end;
	while(item)
	{
		if(item->data.Score() < min)
		{
			min = item->data.Score();
			ret = item;
		}
		item = item->prev;
	}
	return ret;
}

// PathNode -------------------------------------------------------------------------
// Convenient constructors
// ----------------------------------------------------------------------------------
PathNode::PathNode() : g(-1), h(-1), pos(-1, -1), parent(NULL)
{}

PathNode::PathNode(int g, int h, const iPoint& pos, PathNode* parent) : g(g), h(h), pos(pos), parent(parent)
{}

PathNode::PathNode(const PathNode& node) : g(node.g), h(node.h), pos(node.pos), parent(node.parent)
{}

// PathNode -------------------------------------------------------------------------
// Fills a list (PathList) of all valid adjacent pathnodes
// ----------------------------------------------------------------------------------
uint PathNode::FindWalkableAdjacents(PathList& list_to_fill, PathNode* parent, bool canFly) const
{
	iPoint cell;
	uint before = list_to_fill.list.count();

	// north
	cell.create(pos.x, pos.y + 1);
	if(App->pathfinding->IsWalkable(cell, canFly))
		list_to_fill.list.add(PathNode(-1, -1, cell, parent));

	// south
	cell.create(pos.x, pos.y - 1);
	if(App->pathfinding->IsWalkable(cell, canFly))
		list_to_fill.list.add(PathNode(-1, -1, cell, parent));

	// east
	cell.create(pos.x + 1, pos.y);
	if(App->pathfinding->IsWalkable(cell, canFly))
		list_to_fill.list.add(PathNode(-1, -1, cell, parent));

	// west
	cell.create(pos.x - 1, pos.y);
	if(App->pathfinding->IsWalkable(cell, canFly))
		list_to_fill.list.add(PathNode(-1, -1, cell, parent));

	return list_to_fill.list.count();
}

// PathNode -------------------------------------------------------------------------
// Calculates this tile score
// ----------------------------------------------------------------------------------
int PathNode::Score() const
{
	return g + h;
}

// PathNode -------------------------------------------------------------------------
// Calculate the F for a specific destination tile
// ----------------------------------------------------------------------------------
int PathNode::CalculateF(const iPoint& destination)
{
	g = parent->g + 1;
	h = pos.DistanceNoSqrt(destination);

	return g + h;
}

// ----------------------------------------------------------------------------------
// Actual A* algorithm: return number of steps in the creation of the path or -1 ----
// ----------------------------------------------------------------------------------
int j1PathFinding::CreatePath(const iPoint& origin, const iPoint& destination, const bool canFly)
{
	BROFILER_CATEGORY("Pathfinding", Profiler::Color::Magenta);
	last_path.Clear();
	int ret = -1;
	if (IsWalkable(origin, canFly) && IsWalkable(destination, true))
	{
		PathList open;
		PathList closed;

		PathNode originNode(0, origin.DistanceNoSqrt(destination), origin, nullptr);

		open.list.add(originNode);

		while (open.list.start != nullptr)
		{
			ret++;
			p2List_item<PathNode> lowestScoreNode = *open.GetNodeLowestScore();

			closed.list.add(lowestScoreNode.data);
			open.list.del(open.GetNodeLowestScore());

			if (lowestScoreNode.data.pos == destination && canFly)
			{
				PathNode current;
 				for (current = lowestScoreNode.data; current.parent != nullptr; current = *current.parent)
					last_path.PushBack(current.pos);

				last_path.Flip();
				break;
			}
			else if (lowestScoreNode.data.pos.x == destination.x && !canFly)
			{
				PathNode current;
				for (current = lowestScoreNode.data; current.parent != nullptr; current = *current.parent)
					last_path.PushBack(current.pos);

				//last_path.PushBack(current.pos);
				last_path.Flip();
				break;
			}

			PathList neighbors;
			lowestScoreNode.data.FindWalkableAdjacents(neighbors, &closed.Find(lowestScoreNode.data.pos)->data, canFly);

			for (p2List_item<PathNode>* current = neighbors.list.start; current; current = current->next)
			{
				if (closed.Find(current->data.pos) == NULL)
				{
					current->data.CalculateF(destination);
					if (open.Find(current->data.pos) == NULL)
					{
						open.list.add(current->data);
					}
					else
					{
						if (open.Find(current->data.pos)->data.Score() > current->data.Score())
						{
							//closed.list.del(open.Find(current->data.pos));
							open.list.del(open.Find(current->data.pos));
							open.list.add(current->data);
						}
					}
				}
			}
			if (neighbors.list.start == neighbors.list.end)
			{
 				ret = -1;
				break;
			}
		}
	}
	else if (!IsWalkable(origin, canFly))
	{
		PathList neighbors;
		PathNode originNode(0, origin.DistanceNoSqrt(destination), origin, nullptr);

		originNode.FindWalkableAdjacents(neighbors, &originNode, canFly);

		if (neighbors.list.start)
		{
			last_path.PushBack(neighbors.list.start->data.pos);
			ret++;
		}
	}
	// TODO 2: Create two lists: open, close
	// Add the origin tile to open
	// Iterate while we have tile in the open list

	// TODO 3: Move the lowest score cell from open list to the closed list
	
	// TODO 4: If we just added the destination, we are done!
	// Backtrack to create the final path
	// Use the Pathnode::parent and Flip() the path when you are finish

	// TODO 5: Fill a list of all adjancent nodes

	// TODO 6: Iterate adjancent nodes:
	// ignore nodes in the closed list
	// If it is NOT found, calculate its F and add it to the open list
	// If it is already in the open list, check if it is a better path (compare G)
	// If it is a better path, Update the parent

	return ret;
}

