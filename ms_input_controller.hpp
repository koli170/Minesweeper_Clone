#ifndef MS_INPUT_CNTRL
#define MS_INPUT_CNTRL
#ifndef PGE
#define PGE
#define OLC_PGE_APPLICATION
#define OLC_IMAGE_STB
#include "olcPixelGameEngine.h"
#endif

#include "ms_grid.hpp"


void player_click(Grid &grid, int border_size, int banner_size, char key, int mouse_x, int mouse_y, bool is_double){
    for (int x = 0; x < grid.grid_x; x++){
        for (int y = 0; y < grid.grid_y; y++){
            Tile current_tile = *grid.mine_field[x][y];

            // PRESS TILE
            if (key == '0'){
                if (!grid.generated){
                    grid.generated = grid.generateMines(mouse_x, mouse_y, border_size, banner_size);
                }

                if ((border_size + current_tile.x_min <= mouse_x && mouse_x < border_size + current_tile.x_max 
                    && border_size + banner_size + current_tile.y_min <= mouse_y && mouse_y < border_size + banner_size + current_tile.y_max)){

                        grid.revealTile(x,y);

                        if (is_double) {
                            grid.revealAround(x,y);
                        }
                    }
                }

            if (key == '1' && grid.generated){
                if ((border_size + current_tile.x_min <= mouse_x && mouse_x < border_size + current_tile.x_max 
                        && border_size + banner_size + current_tile.y_min <= mouse_y && mouse_y < border_size + banner_size + current_tile.y_max)){
                        grid.flagTile(x,y);
                }
            } 

        }    
    }
}



#endif