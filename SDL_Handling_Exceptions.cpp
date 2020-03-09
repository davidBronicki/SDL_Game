#include "SDL_Handling.h"

const char* SDL_Exception::what() const throw()
{
	return "SDL_Exception.";
}

SDL_MiscException::SDL_MiscException(const char* inputDescription)
: description(inputDescription){}

const char* SDL_MiscException::what() const throw()
{
	return description;
}

const char* SDL_ImageException::what() const throw()
{
	return "Failed to load image.";
}
