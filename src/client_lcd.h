#define LCD_WIDTH 16
#define LCD_HEIGHT 2

typedef struct _Vec2 {
    int x;
    int y;
} Vec2;

typedef struct LCD_DATA {
    Vec2 CursorPosition;
} LCD_DATA;

typedef struct _LCD {
    LCD_DATA _private_data;
} LCD;

void PutString(LCD *self, char *str);
void PutChar(LCD *self, char c);
Vec2 GetPosition(LCD *self);
void SetCursorByVec(LCD *self, Vec2 pos);
void SetCursor(LCD *self, int x, int y);
void ClearScreen(LCD *self);
void ClearArea(LCD *self, Vec2 from, Vec2 to);
LCD *Initialize();

