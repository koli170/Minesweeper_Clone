#ifndef PGE
#define PGE
#define OLC_PGE_APPLICATION
#define OLC_IMAGE_STB
#include "olcPixelGameEngine.h"
#endif

#include <stdio.h>
#include <cstdlib>
#include <random>
#include <ctime>

#include "ms_grid.hpp"
#include "ms_input_controller"

using std::vector;
using std::make_unique;
using std::unique_ptr;
using std::to_string;
using std::cout;
using std::endl;


int screen_width = 300;
int screen_height = 350;
int grid_x = 10;
int grid_y = 10;
int bomb_amount = 15;
int border_size = 20;
bool generated = false;



class MineSweeper : public olc::PixelGameEngine 
{
    int tile_size = (screen_width - 2*border_size)/grid_x;
    float global_time = 0;
    float start_time = 0;
    Grid grid = Grid(tile_size, grid_x, grid_y, bomb_amount);


public:
	MineSweeper()
	{
		sAppName = "Mine Sweeper"; // You can name your application here
	}

public:
	bool OnUserCreate() override
	{
        std::srand(std::time({}));

		// Called once at the start, so create things here
		return true;
	}

	bool OnUserUpdate(float fElapsedTime) override
	{
        global_time += fElapsedTime;

        Clear(olc::DARK_GREY);
		// called once per frame
        char key = '_'; // Default nothing pressed
        bool double_click = false;
        if (GetMouse(0).bPressed){
            if (global_time - start_time < 0.2){
                double_click = true;
            }
            start_time = global_time;
            key = '0';
        }

        else if (GetMouse(1).bPressed){
            key = '1';
        }

        if (key != '_'){
            player_click(grid, border_size, key, GetMouseX(), GetMouseY(), double_click);
        }

        FillRect(border_size, border_size, ScreenWidth()-(2*border_size), ScreenHeight()-(2*border_size), olc::GREY);

        // ITERATE TILES
        for (int x = 0; x < grid.grid_x; x++){
            for (int y = 0; y < grid.grid_y; y++){
                Tile current_tile = *grid.mine_field[x][y];

                 // DRAW TILE
                if(current_tile.is_flagged){
                    FillRect(border_size + current_tile.x_min, border_size + current_tile.y_min, grid.tile_size, grid.tile_size, olc::GREEN);
                }
                else if (!current_tile.is_revealed){
                    FillRect(border_size + current_tile.x_min, border_size + current_tile.y_min, grid.tile_size, grid.tile_size, current_tile.color);
            
                }
                else if(current_tile.bomb_count == 0){

                }
                else if(current_tile.is_bomb){
                    FillRect(border_size + current_tile.x_min, border_size + current_tile.y_min, grid.tile_size, grid.tile_size, olc::RED);
                }
            
                else{
                    DrawString(border_size + current_tile.x_min, border_size + current_tile.y_min, to_string(current_tile.bomb_count), olc::BLACK, 3);
                }
            }
        }
    

        // DRAW GRID LINES
        for (int x = 0; x <= grid.grid_x; x++){
            FillRect(border_size + grid.tile_size*x, border_size, 2, grid.grid_y*grid.tile_size, olc::BLACK);
        }
        for (int y = 0; y <= grid.grid_y; y++){
            FillRect(border_size, border_size + grid.tile_size*y, grid.grid_x*grid.tile_size, 2, olc::BLACK);
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
	MineSweeper game;
	if (game.Construct(screen_width, screen_height, 1, 1))
		game.Start();

	return 0;
}