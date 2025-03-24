#ifndef MS_GRID
#define MS_GRID

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

using std::vector;
using std::make_unique;
using std::unique_ptr;
using std::to_string;
using std::cout;
using std::endl;
using std::pair;
using std::make_pair;

struct Tile {
    int x_min;
    int x_max;
    int y_min;
    int y_max;
    int bomb_count = 0;
    bool safe = false;
    bool is_flagged = false;
    bool is_bomb = false;
    bool is_revealed = false;
    olc::Pixel color;

    Tile(int xmin, int xmax, int ymin, int ymax, int bombcnt = 0, bool revealed = false, olc::Pixel clr = olc::BLUE) : 
            x_min{xmin}, x_max{xmax}, y_min{ymin}, y_max{ymax}, bomb_count{bombcnt}, is_revealed{revealed}, color{clr} 
        {}

    void flag(){
        if (!is_revealed){
            is_flagged = !is_flagged;
        }
    }
        
};

struct Grid {
    vector<vector<unique_ptr<Tile>>> mine_field;
    int tile_size;
    int grid_x;
    int grid_y;
    int bomb_amount;
    int flag_count = 0;
    bool generated = false;

    Grid(int tile_size, int grid_x, int grid_y, int bomb_amount) : 
            tile_size{tile_size}, grid_x{grid_x}, grid_y{grid_y}, bomb_amount{bomb_amount}
        {
            mine_field.resize(grid_x);
            for (int x = 0; x < grid_x; x++) {
                mine_field[x].resize(grid_y);
            }

            for (int x = 0; x < grid_x; x++){
                for (int y = 0; y < grid_y; y++){

                    mine_field[x][y] = make_unique<Tile>(x*tile_size, (x+1)*tile_size, y*tile_size, (y+1)*tile_size);
                }
            }
        }

    void flagTile(int x, int y){
        if (!mine_field[x][y]->is_revealed){
            mine_field[x][y]->flag();
            if (mine_field[x][y]->is_flagged){
                flag_count++;
            }
            else{
                flag_count--;
            }
        }
    }

    void revealTile(int tile_x, int tile_y){
        if (!mine_field[tile_x][tile_y]->is_flagged){
            mine_field[tile_x][tile_y]->is_revealed = true;
        }
        if (mine_field[tile_x][tile_y]->bomb_count == 0 && mine_field[tile_x][tile_y]->is_revealed){
            for (int x = tile_x-1; x <= tile_x+1; x++){
                for (int y = tile_y-1; y <= tile_y+1; y++){
                    if (x < grid_x && x >= 0 && y < grid_y && y >= 0 && !mine_field[x][y]->is_revealed){
                        revealTile(x,y);
                    }
                }
            }
        }
    }    

    void revealAround(int tile_x, int tile_y){
        if (mine_field[tile_x][tile_y]->is_revealed){
            for (int x = tile_x-1; x <= tile_x+1; x++){
                for (int y = tile_y-1; y <= tile_y+1; y++){
                    if (x < grid_x && x >= 0 && y < grid_y && y >= 0 && !mine_field[x][y]->is_revealed){
                        revealTile(x,y);
                    }
                }
            }
        }
    }

    bool generateMines(int start_x, int start_y, int border_size, int banner_size){
        cout << "STARTING GENERATION" << endl;
        int tile_x;
        int tile_y;
        bool ret_val = false;
        // FIND START TILE
        for (int x = 0; x < grid_x; x++){
            if (ret_val){
                break;
            }
            for (int y = 0; y < grid_y; y++){
                Tile current_tile = *mine_field[x][y];
                if ((border_size + current_tile.x_min <= start_x && start_x < border_size + current_tile.x_max 
                                && border_size + banner_size + current_tile.y_min <= start_y && start_y < border_size + banner_size + current_tile.y_max)){
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
                if (x < grid_x && x >= 0 && y < grid_y && y >= 0){
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

    pair<int, int> hint(){
        int neighbor_count = 0;
        int un_flagged_bomb_count = 0;
        pair<int,int> guess = make_pair(-1,-1);
        for (int x = 0; x < grid_x; x++){
            for (int y = 0; y < grid_y; y++){
                Tile current_tile = *mine_field[x][y];

                if (current_tile.is_revealed && current_tile.bomb_count > 0){
                    neighbor_count = 0;
                    un_flagged_bomb_count = current_tile.bomb_count;
                    for (int neighbor_x = x-1; neighbor_x <= x+1; neighbor_x++){
                        for (int neighbor_y = y-1; neighbor_y <= y+1; neighbor_y++){
                            if (neighbor_x < grid_x && neighbor_x >= 0 && neighbor_y < grid_y && neighbor_y >= 0 
                                && !mine_field[neighbor_x][neighbor_y]->is_revealed 
                                && mine_field[neighbor_x][neighbor_y]->is_flagged){
                                    un_flagged_bomb_count--;
                                }
                            if (neighbor_x < grid_x && neighbor_x >= 0 && neighbor_y < grid_y && neighbor_y >= 0 
                                && !mine_field[neighbor_x][neighbor_y]->is_revealed 
                                && !mine_field[neighbor_x][neighbor_y]->is_flagged){
                                guess = make_pair(neighbor_x,neighbor_y);
                                neighbor_count++;
                            }
                        }
                    }
                    // B1
                    if (neighbor_count == un_flagged_bomb_count && un_flagged_bomb_count > 0){
                        cout << guess.first << " " << guess.second << endl;
                        flagTile(guess.first, guess.second);
                        return guess;
                    }
                    // B2
                    if (un_flagged_bomb_count == 0 && neighbor_count > 0){
                        cout << "CLEAR" << endl;
                        revealAround(x, y);
                        return make_pair(x,y);
                    }
                }

            }
        }
        return make_pair(-1,-1);
    }

};

#endif