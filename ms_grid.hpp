#ifndef MS_GRID
#define MS_GRID

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

    Tile(int xmin, int xmax, int ymin, int ymax, int bombcnt = 0, bool revealed = false, olc::Pixel clr = olc::BLUE);

    void flag();
        
};

struct Grid {
    std::vector<std::vector<std::unique_ptr<Tile>>> mine_field;
    int tile_size;
    int grid_x;
    int grid_y;
    int bomb_amount;
    int flag_count = 0;
    bool generated = false;

    Grid(int tile_size, int grid_x, int grid_y, int bomb_amount);

    void flagTile(int x, int y);

    void revealTile(int tile_x, int tile_y);

    void revealAround(int tile_x, int tile_y);

    bool generateMines(int start_x, int start_y, int border_size, int banner_size);

    std::pair<int, int> hint();

};

#endif