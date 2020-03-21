

class GameObject
{
public:
	void updateMovement() virtual{}
	void updateControl() virtual{}
	void updateLogic() virtual{}
};

class I_draw
{
public:
	void draw();
};

class I_Collidable
{
public:
	void collide(std::shared_ptr<I_Collidable> other) virtual{}
};

class I_Composite
{
protected:
	std::vector<std::shared_ptr<I_Collidable>> children;
public:
	void updateCollisions() virtual{}
};

enum class Control
{
	Forward,
	Backward,
	Right,
	Left,
	R_Right,
	R_Left,
	Point_Weapons,
	Point_Ship
};

class I_Controllable
{
public:
	void toggleControl(Control key) virtual = 0;
	void simpleControl(Control key, float controlValue) virtual = 0;
	void advancedControl(Control key,
		const& std::vector<float> controlValues) virtual = 0;
};
