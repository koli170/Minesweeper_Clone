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
#include <cmath>

#include "ms_grid.hpp"
#include "ms_input_controller.hpp"

using std::vector;
using std::make_unique;
using std::unique_ptr;
using std::to_string;
using std::cout;
using std::endl;
using std::pair;
using std::pow;

int grid_x = 9;
int grid_y = 9;
int bomb_amount = 10;
int border_size = 20;
int banner_size = border_size + 48;
int screen_width = 2*border_size + grid_x*32;
int screen_height = 2*border_size + banner_size + grid_y*32;
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
int winmine_31_number_x = 14;
int winmine_31_number_y = 212;
int winmine_31_big_number_x = 14;
int winmine_31_big_number_y = 146;




class MineSweeper : public olc::PixelGameEngine 
{
    bool victory = false;
    bool defeat = false;
    bool change = true;
    bool win_screen = true;
    int tile_size = 32;
    float global_time = 0;
    float start_time = 0;
    float round_time = 0;
    pair <int,int> guess = make_pair(-1,-1);
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

        global_time += fElapsedTime;

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

        if (key != '_' && !victory && !defeat){
            player_click(grid, border_size, banner_size, key, GetMouseX(), GetMouseY(), double_click);
            change = true;
        }

        if (GetKey(olc::R).bPressed){
            grid = Grid(tile_size, grid_x, grid_y, bomb_amount);
            round_time = 0;
            victory = false;
            defeat = false;
            change = true;
            win_screen = true;
        }

        guess = make_pair(-1,-1);
        if (GetKey(olc::H).bPressed && !victory && !defeat){
            guess = grid.hint();
            cout << "HINT USED" << endl;
            change = true;
        }

        // IF A CHANGE WAS DETECTED, RE DRAW THE GRID
        if (change){
            change = defeat;
            Clear(olc::DARK_GREY);
            
            // DRAW GRID LINES & BORDER
            FillRect(border_size - 5, border_size+banner_size-5, grid.grid_x*grid.tile_size + 10, grid.grid_y*grid.tile_size + 10, olc::VERY_DARK_GREY);
            FillRect(border_size, border_size + banner_size, ScreenWidth()-(2*border_size), ScreenHeight()-(2*border_size)-banner_size, olc::GREY);


            for (int x = 0; x < grid.grid_x; x++){
                FillRect(border_size + grid.tile_size*x, border_size+banner_size, 2, grid.grid_y*grid.tile_size, olc::DARK_GREY);
            }
            for (int y = 0; y < grid.grid_y; y++){
                FillRect(border_size, border_size + banner_size + grid.tile_size*y, grid.grid_x*grid.tile_size, 2, olc::DARK_GREY);
            }

            // ITERATE TILES
            victory = true;
            for (int x = 0; x < grid.grid_x; x++){
                for (int y = 0; y < grid.grid_y; y++){
                    Tile current_tile = *grid.mine_field[x][y];

                    if (!current_tile.is_revealed && !current_tile.is_bomb){
                        victory = false;
                    }
                    
                    // DRAW CLICKED BOMB
                    if(current_tile.is_bomb && current_tile.is_revealed){
                        DrawPartialSprite(border_size + current_tile.x_min, border_size + banner_size + current_tile.y_min, &sprite_sheet, winmine_31_mine_revealed_x, winmine_31_mine_revealed_y, 16, 16, 2);
                        defeat = true;
                    }
                    // DRAW FLAGGED BOMB ON DEFEAT
                    else if (defeat && current_tile.is_bomb && current_tile.is_flagged){
                        DrawPartialSprite(border_size + current_tile.x_min, border_size + banner_size + current_tile.y_min, &sprite_sheet, winmine_31_mine_flag_x, winmine_31_mine_flag_y, 16, 16, 2);
                    }
                    // DRAW UNFLAGGED BOMB ON DEFEAT
                    else if (defeat && current_tile.is_bomb && !current_tile.is_revealed){
                        DrawPartialSprite(border_size + current_tile.x_min, border_size + banner_size + current_tile.y_min, &sprite_sheet, winmine_31_mine_x, winmine_31_mine_y, 16, 16, 2);
                    }
                    // DRAW FLAGGED TILE
                    else if(current_tile.is_flagged){
                        DrawPartialSprite(border_size + current_tile.x_min, border_size + banner_size + current_tile.y_min, &sprite_sheet, winmine_31_flag_x, winmine_31_flag_y, 16, 16, 2);
                    }
                    // DRAW UNREVEALED TILE
                    else if (!current_tile.is_revealed){
                        DrawPartialSprite(border_size + current_tile.x_min, border_size + banner_size + current_tile.y_min, &sprite_sheet, winmine_31_tile_x, winmine_31_tile_y, 16, 16, 2);
                    }
                    // DRAW EMPTY TILE
                    else if(current_tile.bomb_count == 0){
                        
                    }
                    // DRAW NUMBER
                    else{
                        DrawPartialSprite(border_size + current_tile.x_min, border_size + banner_size + current_tile.y_min, &sprite_sheet, winmine_31_number_x + (current_tile.bomb_count-1)*17, winmine_31_number_y, 16, 16, 2);
                    }

                    // DRAW HINT MARKER
                    if (x == guess.first && y == guess.second){
                        FillRect(border_size + current_tile.x_min, border_size + banner_size + current_tile.y_min, 10, 10, olc::YELLOW);
                    }
                }
            }
        

            // DRAW FLAG COUNT
            int flag_counter = grid.bomb_amount - grid.flag_count;
            int counter_value = 0;
            bool negative = false;
            FillRect(border_size, border_size, 78, 46, olc::BLACK);
            if (flag_counter > 999){
                flag_counter = 999;
            }
            else if (flag_counter < -99){
                flag_counter = -99;
            }
            if (flag_counter < 0){
                negative = true;
                flag_counter = -flag_counter;
            }
            for (int i = 2; i >= 0; i--){
                counter_value = flag_counter % 10;
                if (negative && i == 0){
                    counter_value = 11;
                }
                else if (flag_counter == 0 && i == 2){
                    counter_value = 10;
                }
                else if (counter_value == 0 && flag_counter == 0){
                    counter_value = 12;
                }
                else if (counter_value == 0){
                    counter_value = 10;
                }
                DrawPartialSprite(border_size + 26*i, border_size, &sprite_sheet, winmine_31_big_number_x + (counter_value-1)*14, winmine_31_big_number_y, 13, 23, 2);
                flag_counter = flag_counter / 10;
            }
        // If defeat is different from start of this frame, 
        // then draw it again in order to draw all exposed bombs
        change = (change != defeat);    
        }

        // DRAW TIMER
        if (grid.generated && !victory && !defeat){
            round_time += fElapsedTime;
        }
        int temp_round_time = round_time;
        int counter_value = 0;
        FillRect(ScreenWidth() - border_size - 78, border_size, 78, 46, olc::BLACK);
        if (round_time > 999){
            temp_round_time = 999;
        }
        for (int i = 2; i >= 0; i--){
            counter_value = temp_round_time % 10;
            if (temp_round_time == 0 && i == 2){
                counter_value = 10;
            }
            else if (counter_value == 0 && temp_round_time == 0){
                counter_value = 12;
            }
            else if (counter_value == 0){
                counter_value = 10;
            }
            DrawPartialSprite(ScreenWidth() - border_size - 78 + 26*i, border_size, &sprite_sheet, winmine_31_big_number_x + (counter_value-1)*14, winmine_31_big_number_y, 13, 23, 2);
            temp_round_time = temp_round_time / 10;
        }

        
        if (victory && win_screen){
            FillRect(ScreenWidth()/5 - 5, ScreenHeight()/5 - 5, 3*ScreenWidth()/5 + 10, 3*ScreenHeight()/5 + 10, olc::VERY_DARK_GREY);
            FillRect(ScreenWidth()/5, ScreenHeight()/5, 3*ScreenWidth()/5, 3*ScreenHeight()/5, olc::GREY);
            FillRect(ScreenWidth()/5, ScreenHeight()/5, 3*ScreenWidth()/5, 42, olc::DARK_BLUE);
            DrawString(ScreenWidth()/5 + 5, ScreenHeight()/5 + 15, "YOU WIN", olc::WHITE, 2);
            DrawString(ScreenWidth()/5 + 5, ScreenHeight()/5 + 64, "TIME: " + to_string(static_cast<int>(round_time)) + "s", olc::VERY_DARK_GREY, 2);
            DrawString(ScreenWidth()/5 + 5, ScreenHeight()/5 + 1.5*64, "MINES: " + to_string(bomb_amount), olc::VERY_DARK_GREY, 2);
            DrawPartialSprite(4*ScreenWidth()/5 - 32 - 5, ScreenHeight()/5 + 5, &sprite_sheet, winmine_31_tile_x, winmine_31_tile_y, 16, 16, 2);
            DrawString(4*ScreenWidth()/5 - 32 - 5 + 6, ScreenHeight()/5 + 5 + 6, "X", olc::VERY_DARK_GREY, 3);        

            if (GetMouse(0).bPressed
                && 4*ScreenWidth()/5 - 32 - 5 <= GetMouseX() && GetMouseX() < 4*ScreenWidth()/5 - 32 - 5 + 32 
                && ScreenHeight()/5 + 5 <= GetMouseY() && GetMouseY() < ScreenHeight()/5 + 5 + 32){

                cout << "close" << endl;
                win_screen = false;
                change = true;
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
	MineSweeper game;
	if (game.Construct(screen_width, screen_height, 1, 1))
		game.Start();

	return 0;
}