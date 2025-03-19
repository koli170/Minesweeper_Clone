#define OLC_PGE_APPLICATION
#define OLC_IMAGE_STB
#include "olcPixelGameEngine.h"

#include <stdio.h>

using std::vector;
using std::make_unique;
using std::unique_ptr;

int screen_width = 300;
int screen_height = 350;
int grid_x = 10;
int grid_y = 10;
int border_size = 20;

struct Tile {
    int x_min;
    int x_max;
    int y_min;
    int y_max;
    int bomb_count = 0;
    bool is_revealed = false;
    olc::Pixel color;

    Tile(int xmin, int xmax, int ymin, int ymax, int bombcnt = 0, bool revealed = false, olc::Pixel clr = olc::BLUE) : 
            x_min{xmin}, x_max{xmax}, y_min{ymin}, y_max{ymax}, bomb_count{bombcnt}, is_revealed{revealed}, color{clr} 
        {}

    // Tile& operator =(const Tile& other)
    //     {
    //         x_min = other.x_min;
    //         x_max = other.x_max;
    //         y_min = other.y_min;
    //         y_max = other.y_max;
    //         bomb_count = other.bomb_count;
    //         is_revealed = other.is_revealed;

    //         return *this;
    //     }
        

};

vector<vector<unique_ptr<Tile>>> mine_field;

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
        
        int tile_size = (ScreenWidth() - 2*border_size)/grid_x;

        mine_field.resize(grid_x);
        for (int x = 0; x < grid_x; x++) {
            mine_field[x].resize(grid_y);
        }

        for (int x = 0; x < grid_x; x++){
            for (int y = 0; y < grid_y; y++){

                mine_field[x][y] = make_unique<Tile>(x*tile_size, (x+1)*tile_size, y*tile_size, (y+1)*tile_size);
            }
        }

		// Called once at the start, so create things here
		return true;
	}

	bool OnUserUpdate(float fElapsedTime) override
	{
        int tile_size = (ScreenWidth() - 2*border_size)/grid_x;
        Clear(olc::DARK_GREY);
		// called once per frame
        FillRect(border_size, border_size, ScreenWidth()-(2*border_size), ScreenHeight()-(2*border_size), olc::GREY);

        for (int x = 0; x < grid_x; x++){
            for (int y = 0; y < grid_y; y++){
                Tile current_tile = *mine_field[x][y];

                if ((border_size + current_tile.x_min <= GetMouseX() && GetMouseX() < border_size + current_tile.x_max 
                    && border_size + current_tile.y_min <= GetMouseY() && GetMouseY() < border_size + current_tile.y_max)){
                        FillRect(border_size+(current_tile.x_min), (border_size) + current_tile.y_min, tile_size, tile_size, olc::WHITE);

                    }
                else{
                    FillRect(border_size + current_tile.x_min, border_size + current_tile.y_min, tile_size, tile_size, current_tile.color);
                }
            }
        }


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
	if (demo.Construct(screen_width, screen_height, 1, 1))
		demo.Start();

	return 0;
}