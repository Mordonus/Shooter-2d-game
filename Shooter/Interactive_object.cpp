#include "Interactive_object.h"
#include "GameFunctions.h"
#include "link.h"

std::vector<int> free_lever_IDs;
Interactive_object::Interactive_object(void)
{
	this->exists=false;
	this->affected_object.push_back(object());
	connections=0;
}


Interactive_object::~Interactive_object(void)
{
}

void Interactive_object::add_affected_objects(int type,int ID)
{
	bool already_registered=false;
	for(int i=0;i<affected_object.size();i++)
		{
			if(affected_object[i].ID==ID&&affected_object[i].type==type)
			{
				already_registered=true;
				break;
			}
		}
	if(!already_registered)
	{
		if(!IDs_of_nothing.empty())
		{

				affected_object[IDs_of_nothing[IDs_of_nothing.size()-1]].ID=ID;
				affected_object[IDs_of_nothing[IDs_of_nothing.size()-1]].type=type;
				IDs_of_nothing.pop_back();
		}
		else
		{
				affected_object.push_back(object());
				affected_object[affected_object.size()-1].ID=ID;
				affected_object[affected_object.size()-1].type=type;
		}
		connections++;

	}
}

void Interactive_object::object_draw()
{
	
	al_draw_bitmap(return_appropriate_bitmap(bitmap),PosX,PosY,NULL);
}

void Interactive_object::set_up(int x,int y,int ID,int object,int button)
{
	PosX=x;
	PosY=y;
	exists=true;
	button_to_interact_with=button;
	map[PosX/TileSize][PosY/TileSize].held_object=object;
	map[PosX/TileSize][PosY/TileSize].held_object_ID=ID;
	of_object=object;
	of_ID=ID;
}

void Interactive_object::send_state()
{
	for(int i=0;i<affected_object.size();i++)
	{
		if(affected_object[i].type==DOOR)
			doors[affected_object[i].ID].change_state();
	}

}

void Interactive_object::remove_connection(int which)
{
	if(affected_object[which].type!=NOTHING)
	{
		affected_object[which].ID=-1;
		affected_object[which].type=NOTHING;

		IDs_of_nothing.push_back(which);
		connections--;
	}

}

void Interactive_object::remove()
{
	exists=false;
	
	map[PosX/TileSize][PosY/TileSize].held_object=NOTHING;
	map[PosX/TileSize][PosY/TileSize].held_object_ID=-1;

	if(of_object==LEVER)
	{
		free_lever_IDs.push_back(of_ID);
	}

}