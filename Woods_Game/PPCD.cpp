#include "PPCD.h"
#include <iostream>
#include <algorithm>

mask_t *Mask_New(ALLEGRO_BITMAP *bmp)
{
	mask_t *temp;
	int h = al_get_bitmap_height(bmp);
	int w = al_get_bitmap_width(bmp);

	//if (h > 64) std::cout << "loading tree" << std::endl;

	ALLEGRO_COLOR transColor = al_map_rgb(255, 0, 255);
	ALLEGRO_COLOR pixel;
	temp = Mask_Create(w, h);

	if (!temp)
		return 0;

	Mask_Clear(temp);
	al_lock_bitmap(bmp, ALLEGRO_PIXEL_FORMAT_ANY, ALLEGRO_LOCK_READWRITE);
	for (int i = 0; i < h; i++)
	{
		for (int j = 0; j < w; j++)
		{
			pixel = al_get_pixel(bmp, j, i);
			if (!Color_Equiv(pixel, transColor) && !Transparent(pixel)) {
				Mask_SetBit(temp, j, i);
			}
		}
	}
	al_unlock_bitmap(bmp);
	return temp;
}

mask_t *Mask_Create(int w, int h)
{
	mask_t *temp = new mask_t;

	temp->w = w;
	temp->h = h;
	temp->bits = new int[w * h];

	Mask_Clear(temp);

	if (!temp)
		return 0;

	return temp;
}

void Mask_Clear(mask_t *m)
{
	for (int i = 0; i < m->w * m->h; i++)
		m->bits[i] = 0;
}

void Mask_Fill(mask_t *m)
{
	for (int i = 0; i < m->w * m->h; i++)
		m->bits[i] = 1;
}

void Mask_SetBit(mask_t *m, int x, int y)
{
	m->bits[x * m->h + y] = 1;
}
void Mask_UnsetBit(mask_t *m, int x, int y)
{
	m->bits[x * m->w + y] = 0;
}

void Mask_Delete(mask_t *m)
{
	if (m->bits != NULL)
		delete[] m->bits;

	if (m != NULL)
		delete m;
}

//offset is a.x - b.x, a.y - b.y
int Mask_Collide(const mask_t *a, const mask_t *b, int xoffset, int yoffset)
{
	int xover = (xoffset < 0 ? std::min(a->w - abs(xoffset), b->w) : std::min(b->w - abs(xoffset), a->w));
	int yover = (yoffset < 0 ? std::min(a->h - abs(yoffset), b->h) : std::min(b->h - abs(yoffset), a->h));
	if (xover <= 0 || yover <= 0)
		return 0;

	int x1 = (abs(xoffset)) * ((xoffset < 0) ? 1 : 0);
	int y1 = (abs(yoffset)) * ((yoffset < 0) ? 1 : 0);
	int x2 = (abs(xoffset)) * ((xoffset < 0) ? 0 : 1);
	int y2 = (abs(yoffset)) * ((yoffset < 0) ? 0 : 1);

	for (int i = 0; i < xover; i++)
	{
		for (int j = 0; j < yover; j++)
		{
			if (a->bits[(x1 + i) * a->h + (y1 + j)] == 1 &&
				b->bits[(x2 + i) * b->h + (y2 + j)] == 1) {
				return 1;
			}
		}
	}

	return 0;
}

void Mask_Draw(mask_t *m, int x, int y)
{
	std::cout << "drawing mask with dimesions: " << m->w << ", " << m->h << std::endl;
	//x = x - m->w / 2;
	//y = y - m->h / 2;

	for (int i = 0; i < m->w; i++)
	{
		for (int j = 0; j < m->h; j++)
		{
			if (m->bits[i * m->h + j] == 1) {
				al_put_pixel(x + i, y + j, al_map_rgba_f(.75, 0, .75, .75));
				//std::cout << "drawing pixel with image location: " << i << ", " << j;
				//std::cout << " and offset: " << x << ", " << y << std::endl;
			}
		}
	}
	std::cout << std::endl;
}

int Color_Equiv(ALLEGRO_COLOR col1, ALLEGRO_COLOR col2)
{
	return col1.r == col2.r && col1.g == col2.g && col1.b == col2.b && col1.a == col2.a;
}

int Transparent(ALLEGRO_COLOR col1)
{
	return col1.a == 0;
}