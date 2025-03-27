#ifndef MS_INPUT_CNTRL
#define MS_INPUT_CNTRL

#include "ms_grid.hpp"

void player_click_tile(Grid &grid, int border_size, int banner_size, char key, int mouse_x, int mouse_y, bool is_double);

bool click_button_controller(int mouse_x, int mouse_y, int min_x, int min_y, int max_x, int max_y);


#endif