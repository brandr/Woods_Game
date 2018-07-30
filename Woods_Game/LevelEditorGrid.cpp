#include "LevelEditorGrid.h"

LevelEditorGrid::LevelEditorGrid()
{
	this->setMargins(0, 0, 0, 0);
}

LevelEditorGrid::LevelEditorGrid(int topMargin, int leftMargin, int rightMargin, int bottomMargin) {
	this->setMargins(topMargin, leftMargin, rightMargin, bottomMargin);
}

void LevelEditorGrid::update()
{
	if (this->level == NULL) { 
		this->clear_image_layers();
	} else {
		for (std::string layer : LEVEL_LAYERS) {
			this->update_image_layer(layer);
		}
	}
}

void LevelEditorGrid::set_layer_visible(std::string layer, bool visible)
{
	this->layer_visibility_map[layer] = visible;
}

void LevelEditorGrid::update_image_layer(std::string layer)
{
	std::string level_name = this->level->get_filename();
	agui::Allegro5Image  *loaded_tile_layer_image = this->loaded_level_image(layer, level_name);
	if (loaded_tile_layer_image == NULL) {
		this->load_image_layer(layer);
	}
}

void LevelEditorGrid::clear_image_layers()
{
	for (auto const &it : loaded_image_layers) {
		loaded_image_layers[it.first] = NULL;
	}
}

void LevelEditorGrid::load_image_layer(std::string layer)
{
	if (this->level != NULL) {
		std::string level_name = this->level->get_filename();
		int width = this->level->get_width();
		int height = this->level->get_height();
		agui::Allegro5Image *image_layer = new agui::Allegro5Image();
		ALLEGRO_BITMAP* image_bitmap = al_create_bitmap(width, height);
		if (TILE_LAYER == layer) {
			level->draw_tiles_onto_bitmap(image_bitmap);
		}
		else if (BLOCK_LAYER == layer) {
			level->draw_blocks_onto_bitmap(image_bitmap);
		}
		else if (ENTITY_GROUP_LAYER == layer) {
			level->draw_entity_groups_onto_bitmap(image_bitmap);
		}
		image_layer->setBitmap(image_bitmap);
		image_layer->setPixel(5, 5, agui::Color(255, 0, 0));
		this->set_loaded_level_image(image_layer, layer, level_name);
	}
}

void LevelEditorGrid::set_level(Level * level)
{
	this->level = level;
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

void LevelEditorGrid::paintComponent(const agui::PaintEvent &paintEvent)
{
	
}

void LevelEditorGrid::paintBackground(const agui::PaintEvent &paintEvent)
{
	if (this->level != NULL)
	{
		std::string level_name = this->level->get_filename();
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

