#include "LoadingScreen.h"

LoadingScreen::LoadingScreen()
{
}

LoadingScreen::~LoadingScreen()
{
}

void LoadingScreen::load_content()
{
	this->font = al_load_font("resources/fonts/ArialBlack.ttf", 20, 0);
	//TODO
}

void LoadingScreen::unload_content()
{
	if (this->font) {
		al_destroy_font(this->font);
	}
}

void LoadingScreen::update()
{
	animation_index = (animation_index + 1) % 180;
	if (this->thread_data.ready && this->get_screen_flag() != FLAG_FINISH_LOADING) {
		this->set_screen_flag(FLAG_FINISH_LOADING);
	}
}

void LoadingScreen::draw(ALLEGRO_DISPLAY * display)
{
	const float x = (al_get_display_width(display)) / 2.0;
	const float y = (al_get_display_height(display)) / 2.0;
	// TODO: either make an actual animation and/or move this to a utility method
	std::string dot_string = "";
	const int dot_index = this->animation_index / 30;
	switch (dot_index) {
	case 0:
		dot_string = "...";
		break;
	case 1:
		dot_string = " ..";
		break;
	case 2:
		dot_string = "  .";
		break;
	case 3:
		dot_string = "   ";
		break;
	case 4:
		dot_string = ".  ";
		break;
	case 5: dot_string = ".. ";
		default:
			break;
	}
	//
	//TODO: store color more generally, have some kind of animation, etc
	const std::string loading_string = "Loading" + dot_string;
	al_draw_text(
		this->font, al_map_rgb(255, 0, 0), x, y, ALLEGRO_ALIGN_LEFT, loading_string.c_str());

	GameScreen::draw(display);
}

void LoadingScreen::finish_loading()
{
	al_destroy_thread(this->loading_thread);
}

void * LoadingScreen::load_func_start_new_game(ALLEGRO_THREAD * thr, void * arg)
{
	LoadingData *data = (LoadingData *)arg;
	al_lock_mutex(data->mutex);
	std::cout << "--------------------------------------\n";
	std::cout << "BEGIN LOADING\n";
	data->next_screen = new MainGameScreen();
	((MainGameScreen *)data->next_screen)->start_new_game(data->next_screen_key);
	((MainGameScreen *)data->next_screen)->load_content();
	data->ready = true;
	al_broadcast_cond(data->cond);
	std::cout << "FINISH LOADING\n";
	std::cout << "--------------------------------------\n";
	al_unlock_mutex(data->mutex);
	return NULL;
}

void * LoadingScreen::load_func_load_game(ALLEGRO_THREAD * thr, void * arg)
{
	LoadingData *data = (LoadingData *)arg;
	al_lock_mutex(data->mutex);
	std::cout << "begin loading\n";
	data->next_screen = new MainGameScreen();
	((MainGameScreen *)data->next_screen)->load_game(data->next_screen_key);
	((MainGameScreen *)data->next_screen)->load_content();
	data->ready = true;
	al_broadcast_cond(data->cond);
	std::cout << "done loading\n";
	al_unlock_mutex(data->mutex);
	return NULL;
}

void LoadingScreen::start_new_game(const std::string world_key)
{
	this->thread_data.next_screen_key = world_key;
	this->loading_thread = al_create_thread(load_func_start_new_game, &(this->thread_data));
	al_start_thread(this->loading_thread);
}

void LoadingScreen::load_game(const std::string world_key)
{
	this->thread_data.next_screen_key = world_key;
	this->loading_thread = al_create_thread(load_func_load_game, &(this->thread_data));
	al_start_thread(this->loading_thread);
}

GameScreen * LoadingScreen::get_next_screen()
{
	al_lock_mutex(this->thread_data.mutex);
	GameScreen * data_next_screen = this->thread_data.next_screen;
	al_unlock_mutex(this->thread_data.mutex);

	return data_next_screen;
}
