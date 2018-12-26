#include "TiledImageLayer.h"

TiledImageLayer::TiledImageLayer()
{
	setClassName("TiledImageLayer");
	this->Register("TiledImages", &tiled_images);
}

TiledImageLayer::~TiledImageLayer()
{
}
