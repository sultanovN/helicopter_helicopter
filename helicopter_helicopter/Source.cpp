#include "raylib.h"
#include <stdint.h>
#include <string>
#include <vector>
#include <chrono>
#include <iostream>

bool game_over = false;

uint8_t level = 0;

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
protected:
	float m_x, m_y;
	float m_width, m_height;
	float m_speed, m_delay;
	Color m_color;
public:
	Projectile(float h_y = 0, float delay = 1.0f, Color color = RED, float speed = 300, float width = 80, float height = 40)
		: m_delay{ delay }, m_x{ GetScreenWidth() + width }, m_y{ GetScreenHeight() / 2.0f - height / 2 + h_y}, m_width{ width }, m_height{ height }, m_speed{ speed }, m_color{ color }
	{

	}

	~Projectile()
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

	virtual void move() 
	{
		m_x -= m_speed * GetFrameTime();
		if (-m_x * -m_delay + m_width < 0)
		{
			
			m_x = (GetScreenWidth() + m_width);
			if (m_y > GetScreenHeight() - GetScreenHeight() * 0.05)
				m_y = m_height + GetScreenHeight() * 0.05;
			else
				m_y += GetScreenHeight() * 0.10;
			if(m_speed < 600)
				m_speed *= 1.03f;
		}
	}

	/*m_x + m_width < -GetScreenWidth()*/
	

	void gameEnd(Player &player, std::vector<Projectile> &array, std::chrono::high_resolution_clock::time_point &time)
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
				array = { {} };
				time = std::chrono::high_resolution_clock::now();
			}
			
		}
	}
};

class TrackingProjectile : public Projectile
{
	float m_x, m_y;
	float m_width, m_height;
	float m_speed, m_delay;
	Color m_color;
public:
	TrackingProjectile()
		: Projectile{}
	{

	}

	~TrackingProjectile()
	{

	}

	void track(Player player)
	{
		m_x -= m_speed * GetFrameTime();
		if ( m_x  - m_width * -m_delay < 0)
		{
			m_speed *= 1.05f;
			m_x = (GetScreenWidth() + m_width);
			m_y = player.getY();
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

void gameProgress(std::vector<Projectile> &array, std::vector<TrackingProjectile> &track_array , std::chrono::high_resolution_clock::time_point time)
{
	std::chrono::high_resolution_clock::time_point time_p = std::chrono::high_resolution_clock::now();
	std::chrono::duration<double> time_span = std::chrono::duration_cast<std::chrono::seconds>(time_p - time);
	if (time_span.count() > 8 && level == 0)
	{
		++level;
		array.push_back({});
	}
	if (time_span.count() > 18 && level == 1)
	{
		++level;
		track_array.push_back(TrackingProjectile{});
	}
	if (time_span.count() > 35 && level == 2)
	{
		++level;
		array.push_back({});
	}
}

//{60, 1.5}, {320, 1.8}, {-270, 2.5}
void showTime(std::chrono::high_resolution_clock::time_point& time)
{
	std::chrono::high_resolution_clock::time_point time_p;
	std::chrono::duration<int> time_span;
	if (!game_over)
	{
		time_p = std::chrono::high_resolution_clock::now();
		time_span = std::chrono::duration_cast<std::chrono::seconds>(time_p - time);
		
		
	}
	std::string s_time = std::to_string(time_span.count());
	DrawText(s_time.c_str(), GetScreenWidth() / 2 - MeasureText(s_time.c_str(), 20) / 2, 40, 20, WHITE);

}

int main()
{
	
	InitWindow(1000, 800, "Helicopter helicopter");

	SetWindowState(FLAG_VSYNC_HINT);

	Player player;
	std::vector<Projectile> array{ {} };
	std::vector<TrackingProjectile> track_array{};
	/*Projectile projectileCommon;
	Projectile projectileTracker{1.0, VIOLET};
	Projectile projectileTracker1{1.7, PURPLE};*/

	std::chrono::high_resolution_clock::time_point time_1 = std::chrono::high_resolution_clock::now();

	while (!WindowShouldClose())
	{
		
		player.move();
		for (auto &elem: array)
			elem.move();
		for (auto& elem : track_array)
			elem.track(player);

		gameProgress(array,track_array, time_1);


		BeginDrawing();
		ClearBackground(BLACK);
		showTime(time_1);
		player.draw();
		for (auto elem : array)
		{
			elem.draw();
			if (player.collide(elem))
				break;

		}
		array[0].gameEnd(player, array, time_1);

		EndDrawing();
	}

	CloseWindow();

	return 0;
}