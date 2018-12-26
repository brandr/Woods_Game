#include "LevelEditorGrid.h"

LevelEditorGrid::LevelEditorGrid()
{
	this->setMargins(0, 0, 0, 0);
	this->addMouseListener(&grid_listener);
}

LevelEditorGrid::LevelEditorGrid(int topMargin, int leftMargin, int rightMargin, int bottomMargin) {
	this->setMargins(topMargin, leftMargin, rightMargin, bottomMargin);
	this->addMouseListener(&grid_listener);
}

void LevelEditorGrid::update()
{
	if (!(this->has_level())) { 
		this->clear_image_layers();
	} else {
		for (std::string layer : LEVEL_LAYERS) {
			this->update_image_layer(layer);
		}
		this->update_input();
	}
}

void LevelEditorGrid::set_layer_visible(std::string layer, bool visible)
{
	this->layer_visibility_map[layer] = visible;
}

void LevelEditorGrid::update_image_layer(std::string layer)
{
	const std::string level_name = LevelEditorDataManager::get_instance().get_selected_level_name();
	agui::Allegro5Image  *loaded_tile_layer_image = this->loaded_level_image(layer, level_name);
	if (loaded_tile_layer_image == NULL) {
		this->load_image_layer(layer);
	}
}

void LevelEditorGrid::update_input()
{
	if (this->grid_listener.has_update()) {
		const std::pair<float, float> pos = this->grid_listener.get_last_event_pos();
		if (this->grid_listener.has_left_click()) {
			if (this->select_mode == 0) {
				this->add_object(pos);
			} else if (this->select_mode == 1) {
				this->select_object(pos);
			}
		} else if (this->grid_listener.has_right_click()) {
			this->delete_object(pos);
		}
		this->grid_listener.unset_update();
	}
}

void LevelEditorGrid::add_object(std::pair<float, float> pos)
{
	LevelEditorDataManager &manager = LevelEditorDataManager::get_instance();
	const std::pair<int, int> tile_pos(pos.first / TILE_SIZE, pos.second / TILE_SIZE);
	const bool update = manager.add_level_object(tile_pos);
	if (update) {
		this->reset_image_layer(manager.get_selected_object_tab_index());
	}
}

void LevelEditorGrid::delete_object(std::pair<float, float> pos)
{
	LevelEditorDataManager &manager = LevelEditorDataManager::get_instance();
	const std::pair<int, int> tile_pos(pos.first / TILE_SIZE, pos.second / TILE_SIZE);
	const bool update = manager.delete_level_object(tile_pos);
	if (update) {
		this->reset_image_layer(manager.get_selected_object_tab_index());
	}
}

void LevelEditorGrid::select_object(std::pair<float, float> pos)
{
	LevelEditorDataManager &manager = LevelEditorDataManager::get_instance();
	const std::pair<int, int> tile_pos(pos.first / TILE_SIZE, pos.second / TILE_SIZE);
}

void LevelEditorGrid::clear_image_layers()
{
	for (auto const &it : loaded_image_layers) {
		loaded_image_layers[it.first] = NULL;
	}
}

void LevelEditorGrid::load_image_layer(std::string layer)
{
	if (this->has_level()) {
		agui::Allegro5Image *image_layer 
			= LevelEditorDataManager::get_instance().load_image_layer(layer);
		std::string level_name = this->level_name();
		this->set_loaded_level_image(image_layer, layer, level_name);
	}
}

void LevelEditorGrid::reset_image_layer(int index)
{
	const std::string layer = LEVEL_LAYERS[index];
	if (loaded_image_layers[layer] != NULL) {
		delete loaded_image_layers[layer];
	}
	this->load_image_layer(layer);
}

void LevelEditorGrid::reset_image_layer(std::string name)
{
	const int size = LEVEL_LAYERS.size();
	for (int i = 0; i < size; i++) {
		const std::string layer = LEVEL_LAYERS[i];
		if (layer == name) {
			if (loaded_image_layers[layer] != NULL) {
				delete loaded_image_layers[layer];
			}
			this->load_image_layer(layer);
			return;
		}
	}
}

agui::Allegro5Image * LevelEditorGrid::loaded_level_image(std::string prefix, std::string level_name)
{
	std::string level_key = prefix + "_" + level_name;
	auto it = loaded_image_layers.find(level_key);
	if (it == loaded_image_layers.end()) {
		return NULL;
	}
	return this->loaded_image_layers[level_key];
}

void LevelEditorGrid::set_loaded_level_image(agui::Allegro5Image * image, std::string prefix, std::string level_name)
{
	std::string level_key = prefix + "_" + level_name;
	this->loaded_image_layers[level_key] = image;
}

bool LevelEditorGrid::is_layer_visible(std::string layer)
{
	auto it = layer_visibility_map.find(layer);
	if (it == layer_visibility_map.end()) {
		return false;
	}
	return layer_visibility_map[layer];
}

std::string LevelEditorGrid::level_name()
{
	return LevelEditorDataManager::get_instance().get_selected_level_name();
}

bool LevelEditorGrid::has_level()
{
	return LevelEditorDataManager::get_instance().has_selected_level();
}

void LevelEditorGrid::paintComponent(const agui::PaintEvent &paintEvent)
{
	
}

void LevelEditorGrid::paintBackground(const agui::PaintEvent &paintEvent)
{
	if (this->has_level())
	{
		std::string level_name = this->level_name();
		for (std::string layer : LEVEL_LAYERS) {
			if (this->is_layer_visible(layer)) {
				agui::Allegro5Image *layer_image = this->loaded_level_image(layer, level_name);
				if (layer_image != NULL) {
					paintEvent.graphics()->drawImage(layer_image,
						agui::Point(this->getLeftMargin(),
							this->getTopMargin()));
				}
			}
		}
	}
}

void LevelEditorGrid::set_select_mode(int value)
{
	this->select_mode = value;
}

int LevelEditorGrid::getTopMargin() const
{
	return this->topMargin;
}

int LevelEditorGrid::getLeftMargin() const
{
	return this->leftMargin;
}

int LevelEditorGrid::getBottomMargin() const
{
	return this->bottomMargin;
}

int LevelEditorGrid::getRightMargin() const
{
	return this->rightMargin;
}

void LevelEditorGrid::setTopMargin(int margin)
{
	this->topMargin = margin;
}

void LevelEditorGrid::setLeftMargin(int margin)
{
	this->leftMargin = margin;
}

void LevelEditorGrid::setBottomMargin(int margin)
{
	this->bottomMargin = margin;
}

void LevelEditorGrid::setRightMargin(int margin)
{
	this->rightMargin = margin;
}

void LevelEditorGrid::setMargins(int t, int l, int b, int r)
{
	this->topMargin = t;
	this->leftMargin = l;
	this->bottomMargin = b;
	this->rightMargin = r;
}

