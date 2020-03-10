#pragma once

// #include <memory>
#include <exception>
#include <vector>
#include <functional>

//forward declaration of SDL native objects

class SDL_Window;
class SDL_Surface;
class SDL_Texture;
class SDL_Renderer;
union SDL_Event;

//exceptions

class SDL_Exception : public std::exception
{
public:
	const char* what() const throw();
};
class SDL_MiscException : public SDL_Exception
{
	const char* description;
public:
	const char* what() const throw();
	SDL_MiscException(const char* inputDescription);
};
class SDL_ImageException : public SDL_Exception
{
public:
	const char* what() const throw();
};

//useful objects

struct ScreenPoint
{
	int x;
	int y;
};

struct ScreenRectangle
{
	int x, y, w, h;
	ScreenRectangle(int _x, int _y, int _w, int _h);
	ScreenRectangle(ScreenPoint position, ScreenPoint size);
	inline ScreenPoint getPosition()
	{
		return ScreenPoint{x, y};
	}
	inline ScreenPoint getSize()
	{
		return ScreenPoint{w, h};
	}
};

struct Color
{
	unsigned char alpha, r, g, b;
};

struct DrawParameters
{
	ScreenRectangle rect;
	Color color;
	float rotationAngle;
	static DrawParameters Line(
		ScreenPoint p1,
		ScreenPoint p2);
	static DrawParameters Line(
		ScreenPoint p1,
		ScreenPoint p2,
		Color inputColor);
	static DrawParameters Rectangle(
		ScreenRectangle inputRect);
	static DrawParameters Rectangle(
		ScreenRectangle inputRect,
		Color inputColor);
	static DrawParameters Rectangle(
		ScreenRectangle inputRect,
		float inputRotation);
};

//SDL Handler Objects

class Image
{
public:
	virtual void draw(DrawParameters params) const = 0;
};

class ImageSurface : public Image
{
	SDL_Surface* image;
	int* references;
public:
	ImageSurface(const std::string& pathToImage);

	ImageSurface(const ImageSurface& other);
	~ImageSurface();
	ImageSurface& operator=(const ImageSurface& other);

	void draw(DrawParameters params) const;
};

class ImageTexture : public Image
{
	SDL_Texture* image;
	int* references;
	ImageTexture();
public:
	static inline const ImageTexture& NullTexture()
	{
		static ImageTexture NULL_TEXTURE;
		return NULL_TEXTURE;
		#define NULL_TEXTURE ImageTexture::NullTexture()
	}

	ImageTexture(const std::string& pathToImage);

	ImageTexture(const ImageTexture& other);
	ImageTexture& operator=(const ImageTexture& other);
	~ImageTexture();

	void draw(DrawParameters params) const;
};

//SDL Handler Environments

template<typename... footprintArgTypes>
class Basic_Event
{
	std::vector<std::function<void(footprintArgTypes...)>> subscribedFunctions;
public:
	inline void subscribe(std::function<void(footprintArgTypes...)> inputFunction)
	{
		subscribedFunctions.push_back(inputFunction);
	}
	inline void operator()(footprintArgTypes... footprintArgs)
	{
		for (auto&& funct : subscribedFunctions)
		{
			funct(footprintArgs...);
		}
	}
};

class GameDrawEnvironment
{
	static SDL_Window* window;
	static SDL_Surface* screen;
	static SDL_Renderer* renderer;
	static int width, height;
	GameDrawEnvironment();
public:
	static bool init(std::string name, int screenWidth, int screenHeight);
	static void shutdown();
	template<typename... argTypes>
	static inline void draw(const Image& image, argTypes... args)
	{
		image.draw(args...);
	}
	static void render();
	static void clearRender();
	static void blit();
	static inline int getWidth()
	{
		return width;
	}
	static inline int getHeight()
	{
		return height;
	}
	static inline SDL_Surface* getScreen()
	{
		return screen;
	}
	static inline SDL_Renderer* getRenderer()
	{
		return renderer;
	}
	static void renderRectFilled(const DrawParameters& params);
	static void renderRectOutline(const DrawParameters& params);
	static void renderLine(const DrawParameters& params);
};

class GameControlEnvironment
{
	static bool userQuit;
	static SDL_Event* e;
	GameControlEnvironment();
public:
	static void init();
	static void shutdown();
	static void quit();//sets userQuit to true
	static inline bool userHasQuit()
	{
		return userQuit;
	}
	static void sortQueue();
};

enum class Key
{
	Escape, F1, F2, F3, F4, F5, F6, F7, F8, F9, F10, F11, F12,
	Accent, One, Two, Three, Four, Five, Six,
	Seven, Eight, Nine, Zero, Minus, Equals, Backspace,
	Tab, Q, W, E, R, T, Y, U, I, O, P, Open_Bracket, Close_Bracket, Backslash,
	A, S, D, F, G, H, J, K, L, Semicolon, Apostrophy, Enter,
	Left_Shift, Z, X, C, V, B, N, M, Comma, Period, Forwardslash, Right_Shift,
	Left_Control, Left_Alt, Space, Right_Alt, Right_Control,
	Up_Arrow, Down_Arrow, Left_Arrow, Right_Arrow,
	KEY_COUNT
};

class Keyboard
{
	static std::vector<bool> keyStates;//true if down
	static Basic_Event<Key> keyPressed;
	static Basic_Event<Key> keyReleased;
	Keyboard();
	friend GameControlEnvironment;
public:
	inline static bool isUp(Key key)
	{
		return !keyStates[(int)key];
	}
	inline static bool isDown(Key key)
	{
		return keyStates[(int)key];
	}
	inline static void subscribeToKeyPressed(std::function<void(Key)> inputFunction)
	{
		keyPressed.subscribe(inputFunction);
	}
	inline static void subscribeToKeyReleased(std::function<void(Key)> inputFunction)
	{
		keyReleased.subscribe(inputFunction);
	}
};

enum class Button
{
	Left, Right, Middle
};

enum class Motion
{
	Scroll
};

class Mouse
{
	static std::vector<bool> buttonStates;//true if down
	static Basic_Event<Button> buttonPressed;
	static Basic_Event<Button> buttonReleased;
	static Basic_Event<Motion, int, int> mouseMotion;
	Mouse();
	friend GameControlEnvironment;
public:
	inline static bool isUp(Button button)
	{
		return !buttonStates[(int)button];
	}
	inline static bool isDown(Button button)
	{
		return buttonStates[(int)button];
	}
	static int x();
	static int y();
	inline static void subscribeToButtonPressed(std::function<void(Button)> inputFunction)
	{
		buttonPressed.subscribe(inputFunction);
	}
	inline static void subscribeToButtonReleased(std::function<void(Button)> inputFunction)
	{
		buttonReleased.subscribe(inputFunction);
	}
	inline static void subscribeToMouseMotion(std::function<void(Motion, int, int)> inputFunction)
	{
		mouseMotion.subscribe(inputFunction);
	}
};
