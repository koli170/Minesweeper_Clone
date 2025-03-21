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

int screen_width = 500;
int screen_height = 500;
int grid_x = 10;
int grid_y = 10;
int bomb_amount = 15;
int border_size = 20;
bool generated = false;

// Sprite sheet coords
int winmine_31_tile_x = 14;
int winmine_31_tile_y = 195;
int winmine_31_flag_x = 48;
int winmine_31_flag_y = 195;
int winmine_31_mine_x = 99;
int winmine_31_mine_y = 195;
int winmine_31_mine_revealed_x = 116;
int winmine_31_mine_revealed_y = 195;
int winmine_31_mine_flag_x = 133;
int winmine_31_mine_flag_y = 195;




class MineSweeper : public olc::PixelGameEngine 
{
    int tile_size = 32;
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
        olc::Sprite sprite_sheet("ms_sprite_sheet.png");

        bool victory = true;
        bool defeat = false;
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

        if (GetKey(olc::R).bPressed){
            grid = Grid(tile_size, grid_x, grid_y, bomb_amount);
        }

        FillRect(border_size, border_size, ScreenWidth()-(2*border_size), ScreenHeight()-(2*border_size), olc::GREY);

        // DRAW GRID LINES
        for (int x = 0; x <= grid.grid_x; x++){
            FillRect(border_size + grid.tile_size*x, border_size, 2, grid.grid_y*grid.tile_size, olc::DARK_GREY);
        }
        for (int y = 0; y <= grid.grid_y; y++){
            FillRect(border_size, border_size + grid.tile_size*y, grid.grid_x*grid.tile_size, 2, olc::DARK_GREY);
        }

        // ITERATE TILES
        for (int x = 0; x < grid.grid_x; x++){
            for (int y = 0; y < grid.grid_y; y++){
                Tile current_tile = *grid.mine_field[x][y];

                if (!current_tile.is_revealed && !current_tile.is_bomb){
                    victory = false;
                }
                
                // DRAW CLICKED BOMB
                if(current_tile.is_bomb && current_tile.is_revealed){
                    DrawPartialSprite(border_size + current_tile.x_min, border_size + current_tile.y_min, &sprite_sheet, winmine_31_mine_revealed_x, winmine_31_mine_revealed_y, 16, 16, 2);
                    defeat = true;
                }
                // DRAW FLAGGED BOMB ON DEFEAT
                else if (defeat && current_tile.is_bomb && current_tile.is_flagged){
                    DrawPartialSprite(border_size + current_tile.x_min, border_size + current_tile.y_min, &sprite_sheet, winmine_31_mine_flag_x, winmine_31_mine_flag_y, 16, 16, 2);
                }
                // DRAW UNFLAGGED BOMB ON DEFEAT
                else if (defeat && current_tile.is_bomb && !current_tile.is_revealed){
                    DrawPartialSprite(border_size + current_tile.x_min, border_size + current_tile.y_min, &sprite_sheet, winmine_31_mine_x, winmine_31_mine_y, 16, 16, 2);
                }
                 // DRAW FLAGGED TILE
                else if(current_tile.is_flagged){
                    DrawPartialSprite(border_size + current_tile.x_min, border_size + current_tile.y_min, &sprite_sheet, winmine_31_flag_x, winmine_31_flag_y, 16, 16, 2);
                }
                // DRAW UNREVEALED TILE
                else if (!current_tile.is_revealed){
                    DrawPartialSprite(border_size + current_tile.x_min, border_size + current_tile.y_min, &sprite_sheet, winmine_31_tile_x, winmine_31_tile_y, 16, 16, 2);
            
                }
                // DRAW EMPTY TILE
                else if(current_tile.bomb_count == 0){
                    
                }
                // DRAW NUMBER
                else{
                    DrawString(border_size + current_tile.x_min, border_size + current_tile.y_min, to_string(current_tile.bomb_count), olc::BLACK, 3);
                }
            }
        }
    


        // DRAW FLAG COUNT
        DrawString(10, ScreenHeight()-30, to_string(grid.flag_count), olc::BLACK, 3);
        
        if (victory){
            cout << "WIN" << endl;
        }
        if (defeat){
            cout << "BOOM" << endl;
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