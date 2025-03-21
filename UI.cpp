#include "UI.h"

UI::UI(int width, int height):width(width),height(height) {
	if (SDL_Init(SDL_INIT_EVERYTHING) != 0) {
		printf("SDL_Init error: %s\n", SDL_GetError());
		return;
	}

	rc = SDL_CreateWindowAndRenderer(width, height, 0,
		&window, &renderer);
	if (rc != 0) {
		SDL_Quit();
		printf("SDL_CreateWindowAndRenderer error: %s\n", SDL_GetError());
		return;
	};

	SDL_SetHint(SDL_HINT_RENDER_SCALE_QUALITY, "linear");
	SDL_RenderSetLogicalSize(renderer, width, height);
	SDL_SetRenderDrawColor(renderer, 0, 0, 0, 255);

	SDL_SetWindowTitle(window, "snejk");


	screen = SDL_CreateRGBSurface(0, width, height, 32,
		0x00FF0000, 0x0000FF00, 0x000000FF, 0xFF000000);

	scrtex = SDL_CreateTexture(renderer, SDL_PIXELFORMAT_ARGB8888,
		SDL_TEXTUREACCESS_STREAMING,
		width, height);


	// wy��czenie widoczno�ci kursora myszy
	SDL_ShowCursor(SDL_DISABLE);

	// wczytanie obrazka cs8x8.bmp
	charset = SDL_LoadBMP("./cs8x8.bmp");
	if (charset == NULL) {
		printf("SDL_LoadBMP(cs8x8.bmp) error: %s\n", SDL_GetError());
		SDL_FreeSurface(screen);
		SDL_DestroyTexture(scrtex);
		SDL_DestroyWindow(window);
		SDL_DestroyRenderer(renderer);
		SDL_Quit();
		return;
	};
	SDL_SetColorKey(charset, true, 0x000000);

	black = SDL_MapRGB(screen->format, 0x00, 0x00, 0x00);
	pink = SDL_MapRGB(screen->format, 0xbf, 0x1f, 0x6c);
	blue = SDL_MapRGB(screen->format, 0x27, 0x1f, 0xbf);
};
UI::~UI() {
	if (!cleared) {//when we restart the game and then quit, we already clear everything
		//and then we go back to the initial call of start_game,hence we have nothing left to clear
		SDL_FreeSurface(charset);
		SDL_FreeSurface(screen);
		SDL_DestroyTexture(scrtex);
		SDL_DestroyRenderer(renderer);
		SDL_DestroyWindow(window);
		cleared = true;
	}
};

void UI::DrawString(SDL_Surface* screen, int x, int y, const char* text,
	SDL_Surface* charset) {
	int px, py, c;
	SDL_Rect s, d;
	s.w = 8;
	s.h = 8;
	d.w = 8;
	d.h = 8;
	while (*text) {
		c = *text & 255;
		px = (c % 16) * 8;
		py = (c / 16) * 8;
		s.x = px;
		s.y = py;
		d.x = x;
		d.y = y;
		SDL_BlitSurface(charset, &s, screen, &d);
		x += 8;
		text++;
	};
};

void UI::DrawSurface(SDL_Surface* screen, SDL_Surface* sprite, int x, int y) {
	SDL_Rect dest;
	dest.x = x - sprite->w / 2;
	dest.y = y - sprite->h / 2;
	dest.w = sprite->w;
	dest.h = sprite->h;
	SDL_BlitSurface(sprite, NULL, screen, &dest);
};


void UI::DrawPixel(SDL_Surface* surface, int x, int y, Uint32 color) {
	int bpp = surface->format->BytesPerPixel;
	Uint8* p = (Uint8*)surface->pixels + y * surface->pitch + x * bpp;
	*(Uint32*)p = color;
};


void UI::DrawLine(SDL_Surface* screen, int x, int y, int l, int dx, int dy, Uint32 color) {
	for (int i = 0; i < l; i++) {
		DrawPixel(screen, x, y, color);
		x += dx;
		y += dy;
	};
};

void UI::DrawRectangle(SDL_Surface* screen, int x, int y, int l, int k,
	Uint32 outlineColor, Uint32 fillColor) {
	int i;
	DrawLine(screen, x, y, k, 0, 1, outlineColor);
	DrawLine(screen, x + l - 1, y, k, 0, 1, outlineColor);
	DrawLine(screen, x, y, l, 1, 0, outlineColor);
	DrawLine(screen, x, y + k - 1, l, 1, 0, outlineColor);
	for (i = y + 1; i < y + k - 1; i++)
		DrawLine(screen, x + 1, i, l - 2, 1, 0, fillColor);
};

void UI::draw_Borders(SDL_Surface* screen, Uint32 fillColor) {
	DrawRectangle(screen, 0, 4, BORDER_LEFT, height-4, fillColor, fillColor);
	DrawRectangle(screen, BORDER_RIGHT, 4, BORDER_LEFT, height-4, fillColor, fillColor);
	DrawRectangle(screen, 0, height-BORDER_LEFT, width, BORDER_LEFT, fillColor, fillColor);
}

void UI::draw_UI(double time, int points, double last_spawn, double red_spawn, bool spawned,int lifetime) {
	SDL_FillRect(screen, NULL, black); // Czyszczenie ekranu
	DrawRectangle(screen, 4, 4, width - 8, INFO_HEIGHT, pink, pink); // Obszar UI
	draw_Borders(screen, pink); // Ramki

	double progress = 0.0; // Proporcja czasu

	if (spawned){
		// Je�li obiekt jest zespawnowany, pokazuje czas �ycia
		progress = (lifetime - last_spawn) / lifetime;
		if (progress < 0) progress = 0;
		if (progress > 1) progress = 1;

		// Szeroko�� paska
		int max_width = width / 3;
		int bar_width = static_cast<int>(progress * max_width);

		// Wy�rodkowanie paska
		int bar_x = (screen->w - max_width) / 2;
		int bar_y = 30;

		// Rysowanie paska
		DrawRectangle(screen, bar_x, bar_y, bar_width, INFO_HEIGHT / 2 - 10, blue, blue);
	}

	

	
	sprintf(text, "Implemented requirements: 1 2 3 4 A B C D E F G H");
	DrawString(screen, screen->w / 2 - strlen(text) * 8 / 2, 6, text, charset);

	sprintf(text, "Elapsed time = %.1lf s", time);
	DrawString(screen, 10, 12, text, charset);

	sprintf(text, "Points: %d", points);
	DrawString(screen, screen->w - strlen(text) * 8 -10, 12, text, charset);
	SDL_UpdateTexture(scrtex, NULL, screen->pixels, screen->pitch);
	SDL_RenderCopy(renderer, scrtex, NULL, NULL);
	
	
}

char* UI::congrats_screen() {
	SDL_FillRect(screen, NULL, black);
	sprintf(text,"Congratulations! You've made it to the top 3. Please enter your name on the terminal.");
	DrawString(screen, width / 2 - strlen(text) * 16 / 2, height / 3, text, charset);
	SDL_UpdateTexture(scrtex, NULL, screen->pixels, screen->pitch);
	SDL_RenderCopy(renderer, scrtex, NULL, NULL);
	SDL_RenderPresent(renderer);

	char name[50] = "";  
	int cursor_pos = 0;  

	SDL_StartTextInput();  

	bool running = true;
	while (running) {
		SDL_Event event;
		while (SDL_PollEvent(&event)) {
			if (event.type == SDL_QUIT) {
				running = false;
			}
			else if (event.type == SDL_TEXTINPUT) {
				
				if (cursor_pos < 49) {
					strcat(name, event.text.text);
					cursor_pos++;
				}
			}
			else if (event.type == SDL_KEYDOWN) {
				if (event.key.keysym.sym == SDLK_BACKSPACE && cursor_pos > 0) {//if user made a typo
					
					name[--cursor_pos] = '\0';
				}
				else if (event.key.keysym.sym == SDLK_RETURN) {
					
					running = false;
				}
			}
		}

		
		SDL_FillRect(screen, NULL, black);
		DrawString(screen, width / 2 - strlen(text) * 16 / 2, height / 3, text, charset);
		DrawString(screen, width / 2 - strlen(name) * 16 / 2, height / 2, name, charset);
		SDL_UpdateTexture(scrtex, NULL, screen->pixels, screen->pitch);
		SDL_RenderCopy(renderer, scrtex, NULL, NULL);
		SDL_RenderPresent(renderer);
	}

	SDL_StopTextInput();  

	printf("Name entered: %s\n", name);
	return name;
}


void UI::ranking(ScoreEntry ranking[], int count) {
	SDL_FillRect(screen, NULL, black);
	DrawString(screen, 100, 50, "Top 3 Ranking", charset);

	for (int i = 0; i < count; i++) {
		
		char points[12];  
		char name[51];    

		
		snprintf(points, sizeof(points), "%d", ranking[i].score);
		snprintf(name, sizeof(name), "%s", ranking[i].name);

		DrawString(screen, 100, 70 + i * 20, name, charset);
		DrawString(screen, 150, 70 + i * 20, points, charset);
	}

	SDL_UpdateTexture(scrtex, NULL, screen->pixels, screen->pitch);
	SDL_RenderCopy(renderer, scrtex, NULL, NULL);
	SDL_RenderPresent(renderer);
}

void UI::Draw_portal_numbers(Portal**portals) {
	int x, y, number;
	for (int i = 0; i < TP_NUMBER * 2; i++) {
		x = portals[i]->getx();
		y = portals[i]->gety();
		number = portals[i]->get_portal_nr();

		char number_text[2];
		snprintf(number_text, sizeof(number_text), "%d", number);
		y += DOT_SIZE / 2 + 5;
		DrawString(screen, x, y, number_text, charset);
		SDL_UpdateTexture(scrtex, NULL, screen->pixels, screen->pitch);
		SDL_RenderCopy(renderer, scrtex, NULL, NULL);
	}
}
void UI::game_lost_screen() {
	SDL_FillRect(screen, NULL, black); 

	sprintf(text, "YOU LOST");
	DrawString(screen, width / 2 - strlen(text) * 16 / 2, height / 3, text, charset);

	
	sprintf(text, "Press N to start a new game");
	DrawString(screen, width / 2 - strlen(text) * 8 / 2, height / 2, text, charset);
	SDL_UpdateTexture(scrtex, NULL, screen->pixels, screen->pitch);
	SDL_RenderCopy(renderer, scrtex, NULL, NULL);
	SDL_RenderPresent(renderer);
}

SDL_Renderer* UI::getRenderer() {
	return renderer;
}

SDL_Surface* UI::getScreen() {
	return screen;
}

SDL_Window* UI::getWindow() {
	return window;
}