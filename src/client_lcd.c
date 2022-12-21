#include "client_lcd.h"
#include <lcd.h>




void PutString(LCD *self, char *str) {
    lcd_puts(str);
}

void PutChar(LCD *self, char c) {
    lcd_putc(c);

    self->_private_data.CursorPosition.x += 1;
    if(self->_private_data.CursorPosition.x >= LCD_WIDTH) {
        self->_private_data.CursorPosition.x = 0;
        self->_private_data.CursorPosition.y += 1;
    }
    if(self->_private_data.CursorPosition.y >= LCD_HEIGHT) {
        self->_private_data.CursorPosition.y = 2;
    }
}

Vec2 GetPosition(LCD *self) {
    return self->_private_data.CursorPosition;
}

void SetCursorByVec(LCD *self, Vec2 pos) {
    lcd_gotoxy(pos.x, pos.y);
    self->_private_data.CursorPosition = pos;
}

void SetCursor(LCD *self, int x, int y) {
    lcd_gotoxy(x,y);
    self->_private_data.CursorPosition = (Vec2) { .x = x, .y = y };
}
void ClearScreen(LCD *self) {
    lcd_clrscr();
    self->_private_data.CursorPosition = (Vec2) { .x = 0, .y = 0 };
}
void ClearArea(LCD *self, Vec2 from, Vec2 to) {
    // TODO
}

LCD *Initialize() {
    lcd_init(LCD_DISP_ON);
    LCD lcd_inst = (LCD) { ._private_data = (LCD_DATA) { .CursorPosition = (Vec2) { .x = 0, .y = 0} } };
    return &lcd_inst;
}