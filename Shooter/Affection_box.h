#pragma once
#include "link.h"
struct object_box
{
	int type;
	int ID;
};
class Affection_box
{
private:
	std::vector<int> flags;
	int all_flags;

public:
	
	float PosX_left;
	float PosX_right;
	float PosY_top;
	float PosY_bottom;
	float width;
	float height;

	int ID;
	object_box held_object;
	bool exists;
	bool shorter_than_player;
	/////////////////////////////////////////////		FLAG STUFF
	int check_flag(int which,bool want_index);
	void add_flags(int number_of_flags, ...);
	void add_flag(int flag);
	void remove_flag(int flag);
	/////////////////////////////////////////////		FLAG STUFF

	/////////////////////////////////////////////		STUFF

	void set_up(int x,int y,int width,int height,int object_type,int object_ID,int box_ID);
	void debug_draw_frame();
	bool check_if_inside(int x,int y);
	bool check_if_colides_static();
	void move_box(int x,int y);
	/////////////////////////////////////////////		STUFF

	Affection_box(void);
	~Affection_box(void);
};

