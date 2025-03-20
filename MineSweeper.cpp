#define OLC_PGE_APPLICATION
#define OLC_IMAGE_STB
#include "olcPixelGameEngine.h"

#include <stdio.h>
#include <cstdlib>
#include <random>
#include <ctime>

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

struct Tile {
    int x_min;
    int x_max;
    int y_min;
    int y_max;
    int bomb_count = 0;
    bool safe = false;
    bool is_bomb = false;
    bool is_revealed = false;
    olc::Pixel color;

    Tile(int xmin, int xmax, int ymin, int ymax, int bombcnt = 0, bool revealed = false, olc::Pixel clr = olc::BLUE) : 
            x_min{xmin}, x_max{xmax}, y_min{ymin}, y_max{ymax}, bomb_count{bombcnt}, is_revealed{revealed}, color{clr} 
        {}
        
};

vector<vector<unique_ptr<Tile>>> mine_field;


void revealTile(int tile_x, int tile_y){
    cout << "REVEALING TILE X: " << tile_x << " Y: " << tile_y << endl;
    mine_field[tile_x][tile_y]->is_revealed = true;
    if (mine_field[tile_x][tile_y]->bomb_count == 0){
        for (int x = tile_x-1; x <= tile_x+1; x++){
            for (int y = tile_y-1; y <= tile_y+1; y++){
                if (x < grid_x && x >= 0 && y < grid_y && y >= 0 && !mine_field[x][y]->is_revealed){
                    cout << "CHECKING X: " << tile_x << " Y: " << tile_y << endl;
                    revealTile(x,y);
                }
            }
        }
    }
}


bool generateMines(vector<vector<unique_ptr<Tile>>> &mine_field, int start_x, int start_y){
    cout << "STARTING GENERATION" << endl;
    int tile_x;
    int tile_y;
    bool ret_val = false;
    // FIND START TILE
    cout << "FINDING TILE" << endl;
    for (int x = 0; x < grid_x; x++){
        if (ret_val){
            break;
        }
        for (int y = 0; y < grid_y; y++){
            cout << "\t X: " << x << " Y: " << y << endl;
            Tile current_tile = *mine_field[x][y];
            if ((border_size + current_tile.x_min <= start_x && start_x < border_size + current_tile.x_max 
                            && border_size + current_tile.y_min <= start_y && start_y < border_size + current_tile.y_max)){
                            tile_x = x;
                            tile_y = y;
                            ret_val = true;
                            cout << "TILE FOUND" << endl;
                            break;
                            }
        }
    }

    if (!ret_val){
        cout << "NO TILE FOUND" << endl;
        return ret_val;
    }

    // MAKE SAFE ZONE
    cout << "MAKING SAFE ZONE" << endl;
    for (int x = tile_x-1; x <= tile_x+1; x++){
        for (int y = tile_y-1; y <= tile_y+1; y++){
            cout << "\t X: " << x << " Y: " << y << endl;
            if (x < grid_x && x >= 0 && y < grid_y && y >= 0){
                cout << "\t\t SET SAFE" << endl;
                mine_field[x][y]->safe = true;
            }
        }
    }

    // MAKE BOMBS
    int count = 0;
    while (count < bomb_amount){
        tile_x = rand() % grid_x;
        tile_y = rand() % grid_y;

        if (!mine_field[tile_x][tile_y]->safe && !mine_field[tile_x][tile_y]->is_bomb){
            mine_field[tile_x][tile_y]->is_bomb = true;
            count++;
            for (int x = tile_x-1; x <= tile_x+1; x++){
                for (int y = tile_y-1; y <= tile_y+1; y++){
                    if (x < grid_x && x >= 0 && y < grid_y && y >= 0){
                        mine_field[x][y]->bomb_count++;
                    }
                }
            }
        }
    }


    return ret_val;

}

class Example : public olc::PixelGameEngine 
{
public:
	Example()
	{
		sAppName = "Mine Sweeper"; // You can name your application here
	}

public:
	bool OnUserCreate() override
	{
        std::srand(std::time({}));
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

        // ITERATE TILES
        for (int x = 0; x < grid_x; x++){
            for (int y = 0; y < grid_y; y++){
                Tile current_tile = *mine_field[x][y];

                // PRESS TILE
                if (GetMouse(0).bPressed){

                    if (!generated){
                        generated = generateMines(mine_field, GetMouseX(), GetMouseY());
                    }

                    if ((border_size + current_tile.x_min <= GetMouseX() && GetMouseX() < border_size + current_tile.x_max 
                            && border_size + current_tile.y_min <= GetMouseY() && GetMouseY() < border_size + current_tile.y_max)){
                            revealTile(x,y);
                            }
                    }

                 // DRAW TILE
                if (!current_tile.is_revealed){
                    FillRect(border_size + current_tile.x_min, border_size + current_tile.y_min, tile_size, tile_size, current_tile.color);
            
                }
                else if(current_tile.bomb_count == 0){

                }
                else if(current_tile.is_bomb){
                    FillRect(border_size + current_tile.x_min, border_size + current_tile.y_min, tile_size, tile_size, olc::RED);
                }
                else{
                    DrawString(border_size + current_tile.x_min, border_size + current_tile.y_min, to_string(current_tile.bomb_count), olc::BLACK, 3);
                }
            }
        }

        // DRAW GRID LINES
        for (int x = 0; x <= grid_x; x++){
            FillRect(border_size + tile_size*x, border_size, 2, grid_y*tile_size, olc::BLACK);
        }
        for (int y = 0; y <= grid_y; y++){
            FillRect(border_size, border_size + tile_size*y, grid_x*tile_size, 2, olc::BLACK);
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