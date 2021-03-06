
#pragma once
#include "link.h"
#include "GameFunctions.h"

void emulate_click(int x, int y);
void emulate_key_press(int which, int x, int y);


struct DataPkg
{
	int x;
	int y;

	short ev_type;
	int ev_x;
	int ev_y;
	int add_nfo;
};

struct ev_data
{
	short ev_type;
	int ev_x;
	int ev_y;
	int add_nfo;

	void operator > (DataPkg &_x)
	{
		_x.ev_type = this->ev_type;
		_x.ev_x = this->ev_x;
		_x.ev_y = this->ev_y;
	}
};
struct online_player
{
	float health;
	int x;
	int y;
	int box_ID;
	void operator=(DataPkg _x)
	{
		x = _x.x;
		y = _x.y;
		
		affection_boxes[box_ID].move_box(x, y);

		if (_x.ev_type != -1)
		{
			if (_x.ev_type == 1)
			{
				emulate_click(_x.ev_x, _x.ev_y);
			}
			else
				emulate_key_press(_x.add_nfo, _x.ev_x, _x.ev_y);
		}
	}
	online_player()
	{
		x = y = 0;
		affection_boxes.push_back(Affection_box());
		affection_boxes[affection_boxes.size() - 1].set_up(x, y, 32, 32, OBJECT_PLAYER, NULL, affection_boxes.size() - 1);
		player_boxes_IDs.push_back(affection_boxes.size() - 1);
		box_ID = affection_boxes.size() - 1;
		health = 100;
	}
};
struct HelloMsg
{
	int ID;
	int wait;
};

class Multiplayer_Client
{
private:
	SOCKADDR_IN addr;
	bool set_up_sockets(std::string host);
	bool connect_to_server();
	int wait_for_hello_message();
public:
	Multiplayer_Client(void);
	~Multiplayer_Client(void);
	bool attempt_connection(std::string host="127.0.0.1");
};

extern SOCKET sConnect;
void receive_send_data();
void register_data(int of_type, int where_x, int where_y, int add_nfo = -1);
bool fetch_data(DataPkg &data);
extern online_player o_player;


