#include "raylib.h"

class Player
{
private:
	float m_x, m_y;
	float m_width, m_height;
	float m_speed;
	Color m_color;
public:
	Player(float x = 100, float y = GetScreenHeight() / 2.0f - 40, float width = 150, float height = 80, float speed = 300,Color color = BLUE)
		: m_x{ x }, m_y{ y }, m_width{ width }, m_height{ height }, m_speed{speed}, m_color{color}
	{

	}

	void draw()
	{
		DrawRectangleRec({ m_x, m_y, m_width, m_height }, m_color);
	}

	void move()
	{
		if (m_y < 0)
			m_y = 0;
		if ((m_y + m_height) > GetScreenHeight())
			m_y = GetScreenHeight() - m_height;
		if (m_x < 0)
			m_x = 0;
		if ((m_x + m_width) > GetScreenWidth())
			m_x = GetScreenWidth() - m_width;
		if (IsKeyDown(KEY_UP) || IsKeyDown(KEY_W))
		{
			m_y -= m_speed * GetFrameTime();
		}
		if (IsKeyDown(KEY_DOWN) || IsKeyDown(KEY_S))
		{
			m_y += m_speed * GetFrameTime();
		}
		if (IsKeyDown(KEY_LEFT) || IsKeyDown(KEY_A))
		{
			m_x -= m_speed * GetFrameTime();
		}
		if (IsKeyDown(KEY_RIGHT) || IsKeyDown(KEY_D))
		{
			m_x += m_speed * GetFrameTime();
		}
	}
};

int main()
{
	InitWindow(800, 600, "Helicopter helicopter");

	SetWindowState(FLAG_VSYNC_HINT);

	Player player;

	while (!WindowShouldClose())
	{

		player.move();

		BeginDrawing();
		ClearBackground(BLACK);

		player.draw();



		EndDrawing();
	}

	CloseWindow();

	return 0;
}