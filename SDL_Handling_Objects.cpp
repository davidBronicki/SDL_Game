#include "SDL_Handling.h"

#include <iostream>

#include <SDL.h>
#include <SDL_image.h>
#include "math.h"

using namespace std;

/////--------------------ScreenRectangle---------------/////

ScreenRectangle::ScreenRectangle(int _x, int _y, int _w, int _h)
:
	x(_x),
	y(_y),
	w(_w),
	h(_h)
{}

ScreenRectangle::ScreenRectangle(ScreenPoint position, ScreenPoint size)
:
	x(position.x),
	y(position.y),
	w(size.x),
	h(size.y)
{}

DrawParameters DrawParameters::Line(
	ScreenPoint p1,
	ScreenPoint p2)
{
	return DrawParameters{
		ScreenRectangle(p1, p2),
		Color{0xff, 0xff, 0xff, 0xff},
		0
	};
}
DrawParameters DrawParameters::Line(
	ScreenPoint p1,
	ScreenPoint p2,
	Color inputColor)
{
	return DrawParameters{
		ScreenRectangle(p1, p2),
		inputColor,
		0
	};
}

DrawParameters DrawParameters::Rectangle(
	ScreenRectangle inputRect)
{

	return DrawParameters{
		ScreenRectangle(inputRect),
		Color{0xff, 0xff, 0xff, 0xff},
		0
	};
}
DrawParameters DrawParameters::Rectangle(
	ScreenRectangle inputRect,
	Color inputColor)
{

	return DrawParameters{
		ScreenRectangle(inputRect),
		inputColor,
		0
	};
}
DrawParameters DrawParameters::Rectangle(
	ScreenRectangle inputRect,
	float inputRotation)
{

	return DrawParameters{
		ScreenRectangle(inputRect),
		Color{0xff, 0xff, 0xff, 0xff},
		inputRotation
	};
}

/////-------------------ImageSurface-------------------/////

ImageSurface::ImageSurface(const string& pathToImage)
{
	SDL_Surface* preOptimized = IMG_Load(pathToImage.c_str());

	if (preOptimized == NULL)
	{
		throw SDL_ImageException();
	}

	image = SDL_ConvertSurface(preOptimized, GameDrawEnvironment::getScreen()->format, 0);

	if (image == NULL)
	{
		throw SDL_ImageException();
	}

	SDL_FreeSurface(preOptimized);
}

ImageSurface::ImageSurface(const ImageSurface& other)
:
	image(other.image),
	references(other.references)
{
	++*references;
}

ImageSurface::~ImageSurface()
{
	if (*references == 1)
	{
		SDL_FreeSurface(image);
		delete references;
	}
	else
	{
		--*references;
	}
}

ImageSurface& ImageSurface::operator=(const ImageSurface& other)
{
	image = other.image;
	references = other.references;
	++*references;
	return *this;
}


void ImageSurface::draw(DrawParameters params) const
{
	SDL_BlitScaled(image, NULL, GameDrawEnvironment::getScreen(), (SDL_Rect*)&(params.rect));
}

/////-------------------ImageTexture-------------------/////

ImageTexture::ImageTexture()
:
	image(NULL),
	references(new int(1))
{}

ImageTexture::ImageTexture(const string& pathToImage)
{
	SDL_Surface* preOptimized = IMG_Load(pathToImage.c_str());

	if (preOptimized == NULL)
	{
		throw SDL_ImageException();
	}

	image = SDL_CreateTextureFromSurface(GameDrawEnvironment::getRenderer(), preOptimized);

	if (image == NULL)
	{
		throw SDL_ImageException();
	}

	SDL_FreeSurface(preOptimized);

	references = new int(1);
}

ImageTexture::ImageTexture(const ImageTexture& other)
:
	image(other.image),
	references(other.references)
{
	++*references;
}

ImageTexture& ImageTexture::operator=(const ImageTexture& other)
{
	++*(other.references);
	if (*references == 1)
	{
		SDL_DestroyTexture(image);
		delete references;
	}
	else
	{
		--*references;
	}
	image = other.image;
	references = other.references;
	return *this;
}

ImageTexture::~ImageTexture()
{
	if (*references == 1)
	{
		SDL_DestroyTexture(image);
		delete references;
	}
	else
	{
		--*references;
	}
}

void ImageTexture::draw(DrawParameters params) const
{
	SDL_RenderCopyEx(GameDrawEnvironment::getRenderer(), image, NULL, (SDL_Rect*)&(params.rect),
		-params.rotationAngle * 180 / M_PI,
		NULL, SDL_FLIP_NONE);
}

/////-------------------NullTexture-------------------/////

// NullTexture::NullTexture()
// :
// 	image(NULL),
// 	references(new int(1))
// {
// 	if (this != &NULL_TEXTURE)
// 	{
// 		references = NULL_TEXTURE.references();
// 		++references;
// 	}
// }
