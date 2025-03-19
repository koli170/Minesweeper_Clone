#define OLC_PGE_APPLICATION
#define OLC_IMAGE_STB
#include "olcPixelGameEngine.h"

class Example : public olc::PixelGameEngine // Note olc::PixelGameEngine is inherited from
{
public:
	Example()
	{
		sAppName = "Mine Sweeper"; // You can name your application here
	}

public:
	bool OnUserCreate() override
	{
		// Called once at the start, so create things here
		return true;
	}

	bool OnUserUpdate(float fElapsedTime) override
	{
		// called once per frame
        FillRect(0, 0, ScreenWidth(), ScreenHeight(), olc::WHITE);
		return true;
	}

        // Optional!
        bool OnUserDestroy() override
	{
		// Called when window is closed
		return true;
	}
};


int main(int argc, char const *argv[]) {
	Example demo;
	if (demo.Construct(300, 320, 1, 1))
		demo.Start();

	return 0;
}