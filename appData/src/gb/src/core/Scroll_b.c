// clang-format off
#pragma bank=1
// clang-format on

#include "Scroll.h"
#include "DataManager.h"
#include "GameTime.h"

void RefreshScroll_b() {
  INT16 x, y;
  INT16 current_column, new_column, current_row, new_row;
  UBYTE render = FALSE;

  x = scroll_target->x - (SCREENWIDTH >> 1);
  y = scroll_target->y - (SCREENHEIGHT >> 1);

  // PUSH_BANK(image_bank);

  if (U_LESS_THAN(x, 0u)) {
    x = 0u;
  }
  if (U_LESS_THAN(y, 0u)) {
    y = 0u;
  }
  if (x > (image_width - ((UINT16)SCREENWIDTH))) {
    x = (image_width - (SCREENWIDTH));
  }
  if (y > (image_height - ((UINT16)SCREENHEIGHT))) {
    y = (image_height - (SCREENHEIGHT));
  }

  current_column = scroll_x >> 3;
  new_column = x >> 3;
  current_row = scroll_y >> 3;
  new_row = y >> 3;

  // If column is +/- 1 just render next column
  if (current_column == new_column - 1) {
    // Render right column
    ScrollUpdateColumnWithDelay(new_column - SCREEN_PAD_LEFT + SCREEN_TILE_REFRES_W - 1,
                                new_row - SCREEN_PAD_TOP);
  } else if (current_column == new_column + 1) {
    // Render left column
    ScrollUpdateColumnWithDelay(new_column - SCREEN_PAD_LEFT, new_row - SCREEN_PAD_TOP);
  } else if (current_column != new_column) {
    // If column differs by more than 1 render entire screen
    render = TRUE;
  }

  // If row is +/- 1 just render next row
  if (current_row == new_row - 1) {
    // Render bottom row
    ScrollUpdateRowWithDelay(new_column - SCREEN_PAD_LEFT,
                             new_row - SCREEN_PAD_TOP + SCREEN_TILE_REFRES_H - 1);
  } else if (current_row == new_row + 1) {
    // Render top row
    ScrollUpdateRowWithDelay(new_column - SCREEN_PAD_LEFT, new_row - SCREEN_PAD_TOP);
  } else if (current_row != new_row) {
    // If row differs by more than 1 render entire screen
    render = TRUE;
  }

  scroll_x = x + scroll_offset_x;
  scroll_y = y;

  if (render) {
    RenderScreen();
  } else if (IS_FRAME_2) {
    if (pending_w_i) {
      // Render next pending chunk of row
      ScrollUpdateRowR();
    }
    if (pending_h_i) {
      // Render next pending chunk of column
      ScrollUpdateColumnR();
    }
  }
}