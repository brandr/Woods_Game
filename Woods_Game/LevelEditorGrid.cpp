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
	std::vector<std::pair<agui::Allegro5Image*, std::string>> loaded_tile_layer_images = this->loaded_level_images(layer, level_name);
	if (loaded_tile_layer_images.empty()) {
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
	if (manager.has_tiled_image_grid_selection()) {
		const bool update = manager.add_tiled_image(tile_pos);
		if (update) {
			this->reset_image_layer(LevelEditorDataManager::TILED_IMAGE_LAYER);
		}
	} else {
		const bool update = manager.add_level_object(tile_pos);
		if (update) {
			this->reset_image_layer(manager.get_selected_object_tab_index());
		}
	}
}

void LevelEditorGrid::delete_object(std::pair<float, float> pos)
{
	LevelEditorDataManager &manager = LevelEditorDataManager::get_instance();
	const std::pair<int, int> tile_pos(pos.first / TILE_SIZE, pos.second / TILE_SIZE);
	if (manager.has_tiled_image_grid_selection()) {
		const bool update = manager.delete_tiled_image(tile_pos);
		if (update) {
			this->reset_image_layer(LevelEditorDataManager::TILED_IMAGE_LAYER);
		}
	}
	else {
		const bool update = manager.delete_level_object(tile_pos);
		if (update) {
			this->reset_image_layer(manager.get_selected_object_tab_index());
		}
	}
}

void LevelEditorGrid::select_object(std::pair<float, float> pos)
{
	LevelEditorDataManager &manager = LevelEditorDataManager::get_instance();
	const std::pair<int, int> tile_pos(pos.first / TILE_SIZE, pos.second / TILE_SIZE);
	manager.select_level_object(tile_pos);
	this->reset_image_layer(LevelEditorDataManager::GRID_LINES_LAYER);
}

void LevelEditorGrid::clear_image_layers()
{
	for (auto const &it : loaded_image_layers) {
		loaded_image_layers[it.first].clear();
	}
}

void LevelEditorGrid::load_image_layer(std::string layer)
{
	if (this->has_level()) {
		FileManager fm;
		const std::vector<std::string> rect_keys = this->image_layer_rect_keys();
		const int size = rect_keys.size();
		for (int i = 0; i < size; i++) {
			const std::string rect_key = rect_keys[i];
			const std::vector<std::string> rect_parts = fm.string_to_parts(rect_key, ",");
			const float x = (float)::atof(rect_parts[0].c_str()), y = (float)::atof(rect_parts[1].c_str()),
				width = (float)::atof(rect_parts[2].c_str()), height = (float)::atof(rect_parts[3].c_str());
			Rect subsection(x, y, width, height);
			agui::Allegro5Image *image_layer
				= LevelEditorDataManager::get_instance().load_image_layer(layer, subsection);
			std::string level_name = this->level_name();
			//TODO: pass in subsection/rect_key
			this->set_loaded_level_image(image_layer, layer, level_name, rect_key);
		}
		
	}
}

void LevelEditorGrid::reset_image_layer(int index)
{
	const std::string layer = LEVEL_LAYERS[index];
	loaded_image_layers[layer].clear();
	this->load_image_layer(layer);
}

void LevelEditorGrid::reset_image_layer(std::string name)
{
	const int size = LEVEL_LAYERS.size();
	for (int i = 0; i < size; i++) {
		const std::string layer = LEVEL_LAYERS[i];
		if (layer == name) {
			loaded_image_layers[layer].clear();
			this->load_image_layer(layer);
			return;
		}
	}
}

agui::Allegro5Image * LevelEditorGrid::loaded_level_image(const std::string prefix, const std::string level_name)
{
	std::string level_key = prefix + "_" + level_name;
	auto it = loaded_image_layers.find(level_key);
	if (it == loaded_image_layers.end()) {
		return NULL;
	}
	return NULL; //temp
	//return this->loaded_image_layers[level_key];
}

std::vector<std::pair<agui::Allegro5Image*, std::string>> LevelEditorGrid::loaded_level_images(const std::string prefix, const std::string level_name)
{
	std::string level_key = prefix + "_" + level_name;
	auto it = loaded_image_layers.find(level_key);
	if (it == loaded_image_layers.end()) {
		return std::vector<std::pair<agui::Allegro5Image *, std::string>>();
	}
	return this->loaded_image_layers[level_key];
}

void LevelEditorGrid::set_loaded_level_image(agui::Allegro5Image * image, const std::string prefix, const std::string level_name, const std::string rect_key)
{
	const std::string level_key = prefix + "_" + level_name;
	auto it = loaded_image_layers.find(level_key);
	if (it == loaded_image_layers.end()) {
		loaded_image_layers[level_key] = std::vector <std::pair<agui::Allegro5Image *, std::string>>();
	}
	this->loaded_image_layers[level_key].push_back(std::pair<agui::Allegro5Image *, std::string>(image, rect_key));
}

const std::vector<std::string> LevelEditorGrid::image_layer_rect_keys()
{
	return LevelEditorDataManager::get_instance().image_layer_rect_keys();
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
				FileManager fm;
				std::vector<std::pair<agui::Allegro5Image*, std::string>> layer_images = this->loaded_level_images(layer, level_name);
				const int size = layer_images.size();
				for (int i = 0; i < size; i++) {
					std::pair<agui::Allegro5Image*, std::string> image_data = layer_images[i];
					agui::Allegro5Image *layer_image = image_data.first;
					const std::string rect_string = image_data.second;
					if (layer_image != NULL) {
						const std::vector<std::string> rect_parts = fm.string_to_parts(rect_string, ",");
						const float x = ::atof(rect_parts[0].c_str()), y = ::atof(rect_parts[1].c_str());
						paintEvent.graphics()->drawImage(layer_image,
							agui::Point(this->getLeftMargin() + x,
								this->getTopMargin() + y));
					}
				}
			}
		}
	}
}

void LevelEditorGrid::set_select_mode(int value)
{
	this->select_mode = value;
}

void LevelEditorGrid::reset_all_grid_image_layers()
{
	for (std::string layer : LEVEL_LAYERS) {
		this->reset_image_layer(layer);
	}
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

