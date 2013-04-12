long unsigned int getpixel(SDL_Surface *s, int x, int y);
int avancer ( SDL_Rect* pposperso, SDL_Surface* terrain, SDL_Rect selecNiveau);
int solsouspieds ( SDL_Rect* pposperso, SDL_Surface* terrain);
void tomber ( SDL_Rect* pposperso, SDL_Surface* terrain);
int monter( SDL_Rect* pposperso, SDL_Surface* terrain );
int plater( SDL_Rect* pposperso, SDL_Surface* terrain );
int descendre( SDL_Rect* pposperso, SDL_Surface* terrain );
int sortir (SDL_Rect *pposperso, SDL_Surface *terrain, SDL_Rect selecNiveau);
int finir (SDL_Rect *pposperso);
