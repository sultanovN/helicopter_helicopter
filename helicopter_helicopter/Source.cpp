#include "raylib.h"
#include <stdint.h>

bool game_over = false;

class Projectile;


class Player
{
private:
	float m_x, m_y;
	float m_width, m_height;
	float m_speed;
	Color m_color;
public:
	Player(float x = 100, float y = GetScreenHeight() / 2.0f - 40 / 2, float width = 150, float height = 80, float speed = 300, Color color = BLUE)
		: m_x{ x }, m_y{ y }, m_width{ width }, m_height{ height }, m_speed{ speed }, m_color{ color }
	{

	}

	float getY()const { return m_y; }

	Rectangle getRec()const {return { m_x, m_y, m_width, m_height };}

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

	void stop() { m_speed = 0; }

	bool collide(Projectile projectile);
};



class Projectile
{
private:
	float m_x, m_y;
	float m_width, m_height;
	float m_speed, m_delay;
	Color m_color;
public:
	Projectile(float delay = 1.0f, Color color = RED, float speed = 300, float width = 80, float height = 40)
		: m_delay{ delay }, m_x{ GetScreenWidth() + width }, m_y{ GetScreenHeight() / 2.0f - height / 2 }, m_width{ width }, m_height{ height }, m_speed{ speed }, m_color{ color }
	{

	}

	/*Projectile(Color color, float width = 80, float height = 40, float speed = 300)
		: m_color{ color }, m_x{ GetScreenWidth() + width }, m_y{ GetScreenHeight() / 2.0f - height / 2 }, m_width{ width }, m_height{ height }, m_speed{ speed }
	{

	}*/

	Rectangle getRec()const { return { m_x, m_y, m_width, m_height }; }

	void draw()
	{
		DrawRectangleRec({ m_x, m_y, m_width, m_height }, m_color);
	}

	void move()
	{
		m_x -= m_speed * GetFrameTime();
	}

	void randomMove()
	{
		move();
		if (-m_x *  - m_delay + m_width < 0)
		{
			m_speed *= 1.05f;
			m_x = (GetScreenWidth() + m_width);
			if (m_y > GetScreenHeight() - GetScreenHeight() * 0.05)
				m_y = m_height + GetScreenHeight() * 0.05;
			else
				m_y += GetScreenHeight() * 0.10;
		}
		
	}
	/*m_x + m_width < -GetScreenWidth()*/
	void track(Player player)
	{
		move();
		if ( m_x  - m_width * -m_delay < 0)
		{
			m_speed *= 1.05f;
			m_x = (GetScreenWidth() + m_width);
			m_y = player.getY();
		}
	}

	void gameEnd(Player &player, Projectile array[])
	{
		if (game_over)
		{
			m_speed = 0;
			player.stop();
			DrawText("Game Over. Press Enter To Try Again", GetScreenWidth() / 2 - MeasureText("Game Over. Press Enter To Try Again", 30) / 2, GetScreenHeight() / 2, 30, WHITE);
			if (IsKeyPressed(KEY_ENTER))
			{
				game_over = false;
				player = Player{};
				array[0] = {};
				array[1] = { 1.0, VIOLET, 350 };
				array[2] = { 2.5, PURPLE, 350 };
				array[3] = { 3.0, DARKBLUE, 450 };
			}
		}
	}
};

bool Player::collide(Projectile projectile)
{
	if (CheckCollisionRecs(getRec(), projectile.getRec()))
	{
		game_over = true;
	}
	return game_over;
}




int main()
{
	#pragma comment(linker, "/SUBSYSTEM:windows /ENTRY:mainCRTStartup")
	InitWindow(1000, 800, "Helicopter helicopter");

	SetWindowState(FLAG_VSYNC_HINT);

	Player player;
	const uint8_t enemy = 4;
	Projectile array[enemy]{ {}, {1.0, VIOLET, 350}, {2.5, PURPLE, 350}, {3.0, DARKBLUE, 450 } };
	/*Projectile projectileCommon;
	Projectile projectileTracker{1.0, VIOLET};
	Projectile projectileTracker1{1.7, PURPLE};*/


	while (!WindowShouldClose())
	{

		player.move();
		array[0].randomMove();
		for (int i = 1; i < enemy; ++i)
			array[i].track(player);
		BeginDrawing();
		ClearBackground(BLACK);

		player.draw();
		for (auto elem : array)
		{
			elem.draw();
			if (player.collide(elem))
				break;

		}
		array[0].gameEnd(player, array);

		EndDrawing();
	}

	CloseWindow();

	return 0;
}