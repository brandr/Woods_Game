#include "TiledImageGrid.h"

//TODO: draw red square to indicate selection (new layer?)

TiledImageGrid::TiledImageGrid()
{
	this->setMargins(0, 0, 0, 0);
	this->addMouseListener(&grid_listener);
}

TiledImageGrid::TiledImageGrid(int topMargin, int leftMargin, int rightMargin, int bottomMargin)
{
	this->setMargins(topMargin, leftMargin, rightMargin, bottomMargin);
	this->addMouseListener(&grid_listener);
}


void TiledImageGrid::update_input()
{
	if (this->grid_listener.has_update()) {
		const std::pair<float, float> pos = this->grid_listener.get_last_event_pos();
		if (this->grid_listener.has_left_click()) {
			this->select_grid_pos(pos);
		}
		this->grid_listener.unset_update();
	}
}

agui::Allegro5Image * TiledImageGrid::loaded_tiled_image_image(std::string prefix, std::string tiled_image_name)
{
	std::string tiled_image_key = prefix + "_" + tiled_image_name;
	auto it = loaded_image_layers.find(tiled_image_key);
	if (it == loaded_image_layers.end()) {
		return NULL;
	}
	return this->loaded_image_layers[tiled_image_key];
}

void TiledImageGrid::set_loaded_tiled_image_image(agui::Allegro5Image * image, std::string prefix, std::string image_key)
{
	std::string full_key = prefix + "_" + image_key;
	this->loaded_image_layers[full_key] = image;
}

void TiledImageGrid::load_image_layer(std::string layer)
{
	if (this->has_tiled_image_key()) {
		const std::string image_key = LevelEditorDataManager::get_instance().get_selected_tiled_image_key();
		agui::Allegro5Image  *loaded_image = this->loaded_tiled_image_image(layer, image_key);
		if (loaded_image != NULL) {
			loaded_image->free_image();
			delete loaded_image;
		}
		agui::Allegro5Image *image_layer
			= LevelEditorDataManager::get_instance().load_tiled_image_select_layer(layer);
		const std::string image_name = this->tiled_image_name();
		this->set_loaded_tiled_image_image(image_layer, layer, image_name);
	}
}

void TiledImageGrid::update_image_layer(std::string layer)
{
	const std::string image_key = LevelEditorDataManager::get_instance().get_selected_tiled_image_key();
	agui::Allegro5Image  *loaded_image = this->loaded_tiled_image_image(layer, image_key);
	if (loaded_image == NULL) {
		this->load_image_layer(layer);
	}
}

void TiledImageGrid::clear_image_layers()
{
	for (auto const &it : loaded_image_layers) {
		loaded_image_layers[it.first] = NULL;
	}
}

void TiledImageGrid::select_grid_pos(const std::pair<int, int> pos)
{
	const std::pair<int, int> grid_pos(pos.first / TILE_SIZE, pos.second / TILE_SIZE);
	LevelEditorDataManager::get_instance().select_tiled_image(grid_pos);
	this->load_image_layer(LevelEditorDataManager::GRID_LINES_LAYER);
}

bool TiledImageGrid::has_tiled_image_key()
{
	return LevelEditorDataManager::get_instance().has_selected_tiled_image_key();
}

std::string TiledImageGrid::tiled_image_name()
{
	return LevelEditorDataManager::get_instance().get_selected_tiled_image_key();
}

void TiledImageGrid::paintComponent(const agui::PaintEvent & paintEvent)
{
}

void TiledImageGrid::paintBackground(const agui::PaintEvent & paintEvent)
{
	if (this->has_tiled_image_key()) {
		const std::string tiled_image_name = this->tiled_image_name();
		for (std::string layer : TILED_IMAGE_SELECT_LAYERS) {
			const agui::Allegro5Image *layer_image = this->loaded_tiled_image_image(layer, tiled_image_name);
				if (layer_image != NULL) {
					paintEvent.graphics()->drawImage(layer_image,
						agui::Point(this->getLeftMargin(),
							this->getTopMargin()));
			}
		}
	}
}

void TiledImageGrid::update()
{
	if (!(this->has_tiled_image_key())) {
		this->clear_image_layers();
	}
	else {
		for (std::string layer : TILED_IMAGE_SELECT_LAYERS) {
			this->update_image_layer(layer);
		}
		this->update_input();
	}
}

int TiledImageGrid::getTopMargin() const
{
	return this->topMargin;
}

int TiledImageGrid::getLeftMargin() const
{
	return this->leftMargin;
}

int TiledImageGrid::getBottomMargin() const
{
	return this->bottomMargin;
}

int TiledImageGrid::getRightMargin() const
{
	return this->rightMargin;
}

void TiledImageGrid::setTopMargin(int margin)
{
	this->topMargin = margin;
}

void TiledImageGrid::setLeftMargin(int margin)
{
	this->leftMargin = margin;
}

void TiledImageGrid::setBottomMargin(int margin)
{
	this->bottomMargin = margin;
}

void TiledImageGrid::setRightMargin(int margin)
{
	this->rightMargin = margin;
}

void TiledImageGrid::setMargins(int t, int l, int b, int r)
{
	this->topMargin = t;
	this->leftMargin = l;
	this->bottomMargin = b;
	this->rightMargin = r;
}



