#include "FormsManager.h"

gui_event container;

FormsManager::FormsManager()
{
	this->prevMousePosX = 0; 
	this->prevMousePosY = 0;
	this->is_form_enabled = false;
	this->previously_overlapping = false;
}


FormsManager::~FormsManager()
{
}

void FormsManager::LoadForms()
{
	std::vector<std::string> forms_found;
	forms_found = get_all_files_names_within_folder("Forma");

	vector<form> output;

	for (int i = 0; i < forms_found.size(); i++)
	{
		std::ifstream in (forms_found[i].c_str());
		if (in.is_open())
		{
			std::string line;
			vector<string> read_attributes;
			form created_form;
			while (!in.eof())
			{
				getline(in, line);
				tokenize(line,read_attributes);

				//Beware almighty switch ...it's on its way

				if (read_attributes[0]=="button")
				{
					RegisterComponentButton(read_attributes, created_form);
				}
				else if (read_attributes[0] == "trigkey")
				{
					RegisterFormTrigger(read_attributes, created_form);
				}
				else if (read_attributes[0] == "Form")
				{
					created_form.name = read_attributes[1];
				}
				else if (read_attributes[0] == "Xpos")
				{
					created_form.PosX = atoi(read_attributes[1].c_str());
				}
				else if (read_attributes[0] == "Ypos")
				{
					created_form.PosY = atoi(read_attributes[1].c_str());
				}
				else if (read_attributes[0] == "Width")
				{
					created_form.width = atoi(read_attributes[1].c_str());
				}
				else if (read_attributes[0] == "Height")
				{
					created_form.height = atoi(read_attributes[1].c_str());
				}
				else if (read_attributes[0] == "conditon")
				{
					RegisterFormTriggerConditions(read_attributes, created_form);
				}
				read_attributes.clear();
			}
			
			forms.push_back(created_form);
		}
	}
}

std::vector<string> FormsManager::get_all_files_names_within_folder(string folder)
{
	vector<string> names;
	char search_path[200];
	sprintf_s(search_path, "%s*.*", folder.c_str());
	WIN32_FIND_DATA fd;
	HANDLE hFind = ::FindFirstFile(search_path, &fd);
	if (hFind != INVALID_HANDLE_VALUE)
	{
		do
		{
			// read all (real) files in current folder
			// , delete '!' read other 2 default folder . and ..
			if (!(fd.dwFileAttributes & FILE_ATTRIBUTE_DIRECTORY))
			{
				names.push_back(fd.cFileName);
			}
		} while (::FindNextFile(hFind, &fd));
		::FindClose(hFind);
	}
	return names;
}

void FormsManager::tokenize(string string_to_tokenize,std::vector<string> &attributes)
{
	std::istringstream f(string_to_tokenize);
	std::string s;
	while (std::getline(f, s, '='))
	{
		attributes.push_back(s);
	}


	
}

void FormsManager::RegisterComponentButton(vector<string> attributes, form &created_form)
{
	void(*disabling_function)(int) = &disable_form;
	void(*exit_function)(void) = &kill_player;
	
	void(*function_to_pass_int)(int);
	void(*function_to_pass_void)(void);


	int PosX, PosY, key;
	short r, g, b;
	PosX = atoi(attributes[1].c_str()) + created_form.PosX;
	PosY = atoi(attributes[2].c_str()) + created_form.PosY;


	r = atoi(attributes[4].c_str());
	g = atoi(attributes[5].c_str());
	b = atoi(attributes[6].c_str());


	if (attributes[3] == "RESUME")
	{
		function_to_pass_int = disabling_function;
		if (forms.empty())
			created_form.buttons.push_back(ComponentButton(function_to_pass_int, PosX, PosY, this->forms.size() - 1, attributes[3], al_map_rgb(r, g, b),0));
		else
			created_form.buttons.push_back(ComponentButton(function_to_pass_int, PosX, PosY, this->forms.size() - 1, attributes[3], al_map_rgb(r, g, b),this->forms.size()-1));

	}
	else if (attributes[3] == "EXIT")
	{
		function_to_pass_void = exit_function;
		created_form.buttons.push_back(ComponentButton(function_to_pass_void, PosX, PosY, this->forms.size() - 1, attributes[3], al_map_rgb(r, g, b)));
	}




}

void FormsManager::RegisterFormTrigger(vector<string> attributes, form &created_form)
{
	int trigger;
	int trigger_value;
	if (attributes[0] == "trigkey")
	{
		trigger = TRIGGER_KEYPRESS;
	}
	if (attributes[1] == "ESC")
	{
		trigger_value = ALLEGRO_KEY_ESCAPE;
	}
	else if (attributes[1] == "P")
	{
		trigger_value = ALLEGRO_KEY_P;
	}

	created_form.trigger = trigger;
	created_form.trigger_conditions.push_back(trigger_value);

}

void FormsManager::RegisterFormTriggerConditions(vector<string> attributes, form &created_form)
{
	if (attributes[1] == "game_running")
	{
		created_form.trigger_conditions.push_back(CONDITION_GAME_RUNNING);
	}
}

void FormsManager::RegisterComponentTextBox(vector<string> attributes, form &created_form)
{
	int x, y;
	string text;
	x = atoi(attributes[1].c_str());
	y = atoi(attributes[2].c_str());
	text = attributes[3];
	created_form.text_boxes.push_back(ComponentTextBox(x, y, text));
}

void FormsManager::evaluate_input(gui_event event_pkg)
{
	if (is_form_enabled)
	{
		for (int i = 0; i < forms.size(); i++)
		{
			for (size_t j = 0; j < forms[i].buttons.size(); j++)
			{
				if (event_pkg.MouseX >= forms[i].buttons[j].posX+cameraX && event_pkg.MouseX <= forms[i].buttons[j].posX+cameraX+150)
				{
					if (event_pkg.MouseY >= forms[i].buttons[j].posY+cameraY &&event_pkg.MouseY <= forms[i].buttons[j].posY + cameraY + 50)
					{
						if (!previously_overlapping)
						{
							previously_overlapping = true;
							forms[i].buttons[j].recv_event(EVENT_CURSOR_START_OVERLAPING);
						}
						if (get_mouse_state("LMB"))
						{
							forms[i].buttons[j].recv_event(GUI_KEY_PRESS);
						}
					}
					else
					{
						if (previously_overlapping)
						{
							previously_overlapping = false;
							forms[i].buttons[j].recv_event(EVENT_CURSOR_STOP_OVERLAPING);
						}
					}
				}
			}

			
			
			
			
			if(forms[i].trigger == TRIGGER_KEYPRESS && vector_contains(forms[i].trigger_conditions, event_pkg.key))
			{
				cout << "Disabling Form" << endl;
				is_form_enabled = false;
				disable_form(i);
				al_rest(.05);
			}
		}
	}
	else
	{
		
		for (int i = 0; i < forms.size(); i++)
		{
			if (forms[i].trigger == TRIGGER_KEYPRESS && vector_contains(forms[i].trigger_conditions, event_pkg.key))
			{
				cout << "Enabling Form" << endl;
				is_form_enabled = true;
				enable_form(i);
				al_rest(.05);
			}
		}
		
	}


}
void SendEventInfoToForm(std::vector<int> info)
{

}

void FormsManager::draw_forms()
{
	for (int i = 0; i < currently_enabled_forms.size()&&currently_enabled_forms[i]!=NULL; i++)
	{
		al_draw_filled_rectangle(currently_enabled_forms[i]->PosX + cameraX,
			currently_enabled_forms[i]->PosY + cameraY,
			currently_enabled_forms[i]->PosX + cameraX + currently_enabled_forms[i]->width,
			currently_enabled_forms[i]->PosY + cameraY + currently_enabled_forms[i]->height,
			al_map_rgba(200, 200, 200, .5));
		for (size_t j = 0; j < currently_enabled_forms[i]->buttons.size(); j++)
		{
			currently_enabled_forms[i]->buttons[j].Render();
		}
		for (size_t j = 0; j < currently_enabled_forms[i]->text_boxes.size(); j++)
		{
			currently_enabled_forms[i]->text_boxes[j].draw_text();
		}

	}
	slaughter_forms();
}


void enable_form(int of_ID)
{
	bool already_enabled = false;
	if (forms_manager.free_form_IDs.empty())
	{
		for (size_t i = 0; i < forms_manager.currently_enabled_forms.size(); i++)
		{
			if (&forms_manager.forms[of_ID] == forms_manager.currently_enabled_forms[i])
			{
				already_enabled = true;
				break;
			}
		}

		if (!already_enabled)
			forms_manager.currently_enabled_forms.push_back(&forms_manager.forms[of_ID]);
	}
	else
	{
		forms_manager.currently_enabled_forms[forms_manager.free_form_IDs[forms_manager.free_form_IDs.size()-1]] = &forms_manager.forms[of_ID];
		forms_manager.free_form_IDs.pop_back();
	}

}


void slaughter_forms()
{
	
	for (size_t i = 0; i < forms_manager.currently_enabled_forms.size(); i++)
	{
		for (size_t ID = 0; ID < forms_manager.forms_to_be_disabled.size(); ID++)
		{
			if (&forms_manager.forms[forms_manager.forms_to_be_disabled[ID]] == forms_manager.currently_enabled_forms[i])
			{
				forms_manager.currently_enabled_forms[i] = NULL;
				forms_manager.free_form_IDs.push_back(i);
				
			}
		}

	}
	forms_manager.forms_to_be_disabled.clear();
}

int FormsManager::search_for_main_form()
{
	for (int i = 0; i < forms.size(); i++)
	{
		if (forms[i].is_main)
		{
			startup_form = &forms[i];
			break;
		}
	}
}

void disable_form(int of_ID)
{
	forms_manager.forms_to_be_disabled.push_back(of_ID);
}