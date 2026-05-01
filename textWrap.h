#pragma once

#include "raylib.h"

//__________________________________________________________________________________________________________________________________________________________________
// COPY PASTED FROM RAYLIB EXAMPLES https://www.raylib.com/examples/text/loader.html?name=text_rectangle_bounds (the text wrap function was removed for some reason)
//__________________________________________________________________________________________________________________________________________________________________

void DrawTextBoxedSelectable(Font font, const char *text, Rectangle rec, float fontSize, float spacing, bool wordWrap, Color tint, int selectStart, int selectLength, Color selectTint, Color selectBackTint);
void DrawTextBoxed(Font font, const char *text, Rectangle rec, float fontSize, float spacing, bool wordWrap, Color tint);