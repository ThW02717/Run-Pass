
#include <SDL.h>
#include <SDL_image.h>
#include <SDL_ttf.h>
#include <SDL_mixer.h>
#include <iostream>
#include <string>
#include <sstream>
#include <ctime>
#include <cstdlib>
#include <cmath>

using namespace std;


//Screen dimension constants
const int SCREEN_WIDTH = 700;
const int SCREEN_HEIGHT = 900;
//The Horizontal level we play
const int PlayLevel=SCREEN_HEIGHT*3/4;

//The window we'll be rendering to
SDL_Window* gWindow = NULL;

//The window renderer
SDL_Renderer* gRenderer = NULL;

//initializing the screen
bool initial();

//loading the Medias
bool loadMedia();

//destroy everything
void closed();

enum
{
	addition, multiplication, subtraction, division 
};

//NewTexture Structure
class NewTexture
{
	public:
		NewTexture();
		~NewTexture();
		
		//lodes the image with path
		bool loadFromFile(std::string path);
		
		//Lodes the image with Text
		bool loadFromRenderedText( std::string textureText, SDL_Color textColor );
		
		//delete the texture
		void free();
		
		//Renders the txture
		void render(int x, int y);
		
		//Get size
		int getWidth();
		int getHeight();
		
		SDL_Texture* oTexture;
	private:
		//The original texture
		
		
		//Image size
		int mWidth;
		int mHeight;	
		
};

//the image of the player

class Pawn
{
	public:
		Pawn();
		~Pawn();
		void render(int hurt);
		
		void free();
		
		int getPosx();
		int getPosy();
		void setPosx(int );
		void setPosy(int );
		
		void FollowMouse(SDL_Event *e, bool);
		long long int getscore()
		{
			return score;
		}
		void setscore(long long int a)
		{
			score=a;
		}
		
		NewTexture getTexture()
		{
			return Pawn_image;
		}
	protected:
		NewTexture Pawn_image;
		
		long long int score;
		int pos_x;
		int pos_y;
};

//the image of the barrier

class Barrier
{
	public:

		Barrier(int POSX=0, int NUMBER=0, int TYPE=0, int BIRTHTIME=0);
		~Barrier();
		void render();
		void free();
		
		int getPosx();
		int getPosy();
		int getNumber();
		int getType();
		int getBirthtime();
		
		void setPosx(int );
		void setPosy(int );
		void setNumber(int );
		void setType(int );
		void setBirthtime(int );
		
		bool collision_check(Pawn player);
	private:
		NewTexture Barrier_image;
		
		int pos_x;
		int pos_y;
		
		int number;
		int type;
		int birthtime;
		 
};

class BarrierChain
{
	public:
		BarrierChain();
		~BarrierChain();
		void AddBarrier(int POSX, int NUMBER, int TYPE, int BIRTHTIME);
		void DeleteBarrier(int );
		int getTotal()
		{
			return Total;
		}
		Barrier* chain;
	private:

		int Total;
};
//The font and color to be used;
TTF_Font *gFont = NULL;
SDL_Color textColor = { 225, 225, 225, 0};
//graphs
NewTexture Background;
NewTexture Background2;
NewTexture Gameover[17];
NewTexture Opening;
NewTexture Opening1;
NewTexture Opening2;
NewTexture Opening3;
NewTexture Opening4;
NewTexture StartButton1;
NewTexture StartButton2;
NewTexture StartButton3;
NewTexture BacktoOpen1;
NewTexture BacktoOpen2;
NewTexture BacktoOpen3; 
NewTexture Mode1;
NewTexture Mode2;
NewTexture Mode3;
NewTexture onedimension1;
NewTexture onedimension2;
NewTexture onedimension3;
NewTexture twodimension1;
NewTexture twodimension2;
NewTexture twodimension3;
//sounds
Mix_Music* OpeningTheme=NULL;
Mix_Music* GamingTheme=NULL;
Mix_Music* OverTheme=NULL;
Mix_Chunk* Glissing=NULL;
Mix_Chunk* Push=NULL; 
Mix_Chunk* Plus=NULL;
Mix_Chunk* Minus=NULL; 

int main( int argc, char* args[] )
{
	//randomized the game
	srand( time(NULL) );
	//(intial)frequency second
	float T=0.5;
	const float P=0.5;
	//period acceleration;
	const float A=0.00005;
	//Last_BirthTime
	 Uint32 LastTime=0;
	//(initial) speed (pixel per minisecond)
	float v=0.2;
	const float u=0.2;
	//acceleration (pixel per minisecond)
	const float a=0.00002;
	//increase probability of division
	const float p=0.0001;
	
	bool Dmode=false;
	
	
	if( !initial() )
	{
		cout<<"Failed to initialize!\n";
	}
	else if( !loadMedia())
	{
		cout<<"Failed to load media.\n";
	}
	else
	{	
		
		//The loop keeper
		bool quit = false;
		
		//The initiator of the game
		bool start=false;
		//The flag of back to open;
		bool back=false;
		//The Event quein
		SDL_Event e;
		
		//the Record
		long long int highest=1;
		//the Record indicator and initialization
		std::stringstream highText;
		NewTexture Highest;
		highText.str("");
		highText<<"The record:"<<highest;
		if(!Highest.loadFromRenderedText( highText.str().c_str(),textColor))
		{
			cout<<"Unable to render score texture!"<<endl;
		}
		//point counter and initialization
		std::stringstream scoreText;
		NewTexture Scorer;
		scoreText.str("");
		scoreText<<"Score:"<<1;	
		if(!Scorer.loadFromRenderedText( scoreText.str().c_str(),textColor))
		{
					cout<<"Unable to render score texture!"<<endl;
		}
		//The timer
		std::stringstream timeText;
		
		NewTexture Timer;	
		//While application is running
		while( !quit )
		{
			
		Pawn Player;
		BarrierChain enemy;
		
		//start time
		Uint32 StartTime=0;
		Uint32 hurttime=0;
		Uint32 Curetime=0;
		long long int score=1;
		
		
		
		//flag of the game
		bool over=false;
		Uint32 LoseTime=0; 
	
		//Score of the game
		
		
			while( SDL_PollEvent( &e ) != 0 )
				{
					//User requests quit
					if( e.type == SDL_QUIT )
					{
							quit = true;
					}
					

				}
			
			bool OnStart=false;
			bool PressStart=false;
			bool OnMode=false;
			bool PressMode=false; 
			//Opening part	
			while(!quit&&!start)
			{
				if( Mix_PlayingMusic() == 0 )
                {
                    //Play the opening
                    Mix_PlayMusic( OpeningTheme, -1 );
                }
				bool gotoMode=false;
				while( SDL_PollEvent( &e ) != 0 )
				{
					//User requests quit
					if( e.type == SDL_QUIT )
					{
							quit = true;
					}
					if(e.type==SDL_MOUSEMOTION||e.type == SDL_MOUSEBUTTONDOWN || e.type == SDL_MOUSEBUTTONUP )
    				{
        				//Get mouse position
        				int x, y;
        				SDL_GetMouseState( &x, &y );
        				bool origin=OnStart||OnMode;
        				OnStart=true;
        				OnMode=true;
        				if(x<SCREEN_WIDTH/2-Mode1.getWidth()/2||x>SCREEN_WIDTH/2+Mode1.getWidth()/2)
        				{
        					OnStart=false;
        					PressStart=false;
						}
						else if(y<PlayLevel-StartButton1.getHeight()/2||y>PlayLevel+StartButton1.getHeight()/2)
						{
							OnStart=false;
							PressStart=false;
						}
						if(OnStart)
						{
							switch(e.type)
							{
								case SDL_MOUSEBUTTONDOWN:
									PressStart=true;
									Mix_PlayChannel( -1, Push, 0 );
									break;
								case SDL_MOUSEBUTTONUP:
									start=true;
									over=false;
									back=false;
									PressStart=false;
									Mix_HaltMusic();
									StartTime=SDL_GetTicks();
									
							}
						}
						if(x<SCREEN_WIDTH/2-Mode1.getWidth()/2||x>SCREEN_WIDTH/2+Mode1.getWidth()/2)
        				{
        					OnMode=false;
        					PressMode=false;
						}
						else if(y<PlayLevel-StartButton1.getHeight()-20-Mode1.getHeight()/2||y>PlayLevel-StartButton1.getHeight()-20+Mode1.getHeight()/2)
						{
							OnMode=false;
							PressMode=false;
						}
						if(OnMode)
						{
							switch(e.type)
							{
								case SDL_MOUSEBUTTONDOWN:
									PressMode=true;
									Mix_PlayChannel( -1, Push, 0 );
									break;
								case SDL_MOUSEBUTTONUP:
									gotoMode=true;
									PressMode=false;
									
							}
						}
						if((OnStart||OnMode)&&(!origin))
						{
							Mix_PlayChannel( -1, Glissing, 0 );
						}
        			}
					
				}
				
				//clear screen
				SDL_SetRenderDrawColor(gRenderer, 0xFF,0xFF,0xFF,0xFF);
				SDL_RenderClear( gRenderer);
				//render the background
				switch(((SDL_GetTicks()-LoseTime)/500)%7)
				{
					case 0:
						Opening.render(0,0);
						break;
					case 1:
					 	Opening4.render(0,0);
						break;
					case 2:
						Opening1.render(0,0);
						break;
					case 3:
						Opening2.render(0,0);
						break;
					case 4:
						Opening3.render(0,0);
						break;
					case 5:
						Opening4.render(0,0);
						break;
					default:
						Opening.render(0,0);
						break;
						
				}
				
				if(!OnStart)
				{
					StartButton1.render(SCREEN_WIDTH/2-StartButton1.getWidth()/2,PlayLevel-StartButton1.getHeight()/2);
				}
				else
				{
					if(!PressStart)
					{
						StartButton2.render(SCREEN_WIDTH/2-StartButton2.getWidth()/2,PlayLevel-StartButton2.getHeight()/2);
					}
					else
					{
						StartButton3.render(SCREEN_WIDTH/2-StartButton3.getWidth()/2,PlayLevel-StartButton3.getHeight()/2);
					}
				}
				if(!OnMode)
				{
					Mode1.render(SCREEN_WIDTH/2-Mode1.getWidth()/2,PlayLevel-Mode1.getHeight()/2-StartButton1.getHeight()-20);
				}
				else
				{
					if(!PressMode)
					{
						Mode2.render(SCREEN_WIDTH/2-Mode2.getWidth()/2,PlayLevel-Mode2.getHeight()/2-StartButton1.getHeight()-20);
					}
					else
					{
						Mode3.render(SCREEN_WIDTH/2-Mode3.getWidth()/2,PlayLevel-Mode3.getHeight()/2-StartButton1.getHeight()-20);
					}
				}
				Highest.render(0,0);
				SDL_RenderPresent( gRenderer);
				//mode part
				bool OnOnedim=false;
				bool PressOnedim=false;
				bool OnTwodim=false;
				bool PressTwodim=false;
				bool OnBack=false;
				bool PressBack=false;  
				while(!quit&&gotoMode)
				{
					
					while( SDL_PollEvent( &e ) != 0 )
					{
					//User requests quit
						if( e.type == SDL_QUIT )
						{
							quit = true;
						}
						int x, y;
        				SDL_GetMouseState( &x, &y );
        				bool origin=OnOnedim||OnTwodim||OnBack;
        				OnOnedim=true;
        				OnTwodim=true;
        				OnBack=true;
						//Onedim buttom
						if(x<SCREEN_WIDTH/2-onedimension1.getWidth()/2||x>SCREEN_WIDTH/2+onedimension1.getWidth()/2)
        				{
        					OnOnedim=false;
        					PressOnedim=false;
						}
						else if(y<onedimension1.getHeight()||y>2*onedimension1.getHeight())
						{
							OnOnedim=false;
							PressOnedim=false;
						}
						if(OnOnedim)
						{
							switch(e.type)
							{
								case SDL_MOUSEBUTTONDOWN:
									PressOnedim=true;
									Mix_PlayChannel( -1, Push, 0 );
									break;
								case SDL_MOUSEBUTTONUP:
									PressOnedim=false;
									Dmode=false;
									
							}
						}
						//twodim buttom
						if(x<SCREEN_WIDTH/2-twodimension1.getWidth()/2||x>SCREEN_WIDTH/2+twodimension1.getWidth()/2)
        				{
        					OnTwodim=false;
        					PressTwodim=false;
						}
						else if(y<1.5*onedimension1.getHeight()+twodimension1.getHeight()||y>1.5*onedimension1.getHeight()+twodimension1.getHeight()+twodimension1.getHeight())
						{
							OnTwodim=false;
							PressTwodim=false;
						}
						if(OnTwodim)
						{
							switch(e.type)
							{
								case SDL_MOUSEBUTTONDOWN:
									PressTwodim=true;
									Mix_PlayChannel( -1, Push, 0 );
									break;
								case SDL_MOUSEBUTTONUP:
									PressTwodim=false;
									Dmode=true;
									
							}
						}
						//back buttom
						if(x<SCREEN_WIDTH/2-BacktoOpen1.getWidth()/2||x>SCREEN_WIDTH/2+BacktoOpen1.getWidth()/2)
        				{
        					OnBack=false;
        					PressBack=false;
						}
						else if(y<PlayLevel-BacktoOpen1.getHeight()/2||y>PlayLevel+BacktoOpen1.getHeight()/2)
						{
							OnBack=false;
							PressBack=false;
						}
						if(OnBack)
						{
							switch(e.type)
							{
								case SDL_MOUSEBUTTONDOWN:
									PressBack=true;
									break;
								case SDL_MOUSEBUTTONUP:
									PressBack=false;
									Mix_PlayChannel( -1, Push, 0 );
									gotoMode=false;
									
							}
						}
						if((OnOnedim||OnTwodim||OnBack)&&(!origin))
						{
							Mix_PlayChannel( -1, Glissing, 0 );
						}
					}
					//clear screen
					SDL_SetRenderDrawColor(gRenderer, 0xFF,0xFF,0xFF,0xFF);
					SDL_RenderClear( gRenderer);
					//rendering
					Gameover[16].render(0,0);
					if(!OnOnedim)
					{	
						if(!Dmode)
						{
							onedimension3.render(SCREEN_WIDTH/2-onedimension3.getWidth()/2,onedimension3.getHeight());
						}
						else
						{
							onedimension1.render(SCREEN_WIDTH/2-onedimension1.getWidth()/2,onedimension1.getHeight());
						}
					}
					else
					{
						if(!PressOnedim)
						{	
							onedimension2.render(SCREEN_WIDTH/2-onedimension2.getWidth()/2,onedimension2.getHeight());
						}
						else
						{
							onedimension3.render(SCREEN_WIDTH/2-onedimension3.getWidth()/2,onedimension3.getHeight());
						}
					}
					if(!OnTwodim)
					{	
						if(!Dmode)
						{
							twodimension1.render(SCREEN_WIDTH/2-twodimension1.getWidth()/2,1.5*onedimension1.getHeight()+twodimension1.getHeight());
						}
						else
						{
							twodimension3.render(SCREEN_WIDTH/2-twodimension3.getWidth()/2,1.5*onedimension1.getHeight()+twodimension3.getHeight());
						}
					}
					else
					{
						if(!PressTwodim)
						{	
							twodimension2.render(SCREEN_WIDTH/2-twodimension2.getWidth()/2,1.5*onedimension1.getHeight()+twodimension2.getHeight());
						}
						else
						{
							twodimension3.render(SCREEN_WIDTH/2-twodimension3.getWidth()/2,1.5*onedimension1.getHeight()+twodimension3.getHeight());
						}
					}
					if(!OnBack)
					{
						BacktoOpen1.render(SCREEN_WIDTH/2-BacktoOpen1.getWidth()/2,PlayLevel-BacktoOpen1.getHeight()/2);
					}
					else
					{
						if(!PressBack)
						{
							BacktoOpen2.render(SCREEN_WIDTH/2-BacktoOpen2.getWidth()/2,PlayLevel-BacktoOpen2.getHeight()/2);
						}
						else
						{
							BacktoOpen3.render(SCREEN_WIDTH/2-BacktoOpen3.getWidth()/2,PlayLevel-BacktoOpen3.getHeight()/2);
						}
					}
					Highest.render(0,0);
					SDL_RenderPresent( gRenderer);
				}
			}
			
			//game part
			while( !quit && !over)
			{
			if( Mix_PlayingMusic() == 0 )
                {
                    //Play the opening
                    Mix_PlayMusic( GamingTheme, -1 );
                }
			//Handle events on queue
				while( SDL_PollEvent( &e ) != 0 )
				{
					//User requests quit
					if( e.type == SDL_QUIT )
					{
							quit = true;
					}
					
					Player.FollowMouse(&e,Dmode);
				}
			//acceleration
			v=u+(SDL_GetTicks()-StartTime)*a;
			T=P+(SDL_GetTicks()-StartTime)*A;	
			if(SDL_GetTicks()-LastTime>1000/T)
			{
				LastTime=SDL_GetTicks();
				int POSX, NUMBER, TYPE,preType;
				POSX=rand()%SCREEN_WIDTH;
				//increase the prabability of division with time
				switch(rand()%lround(4+static_cast<float>(SDL_GetTicks()-StartTime)*p))
				{
					case addition:
						TYPE=addition;
						break;
					case multiplication:
						TYPE=multiplication;
						break;
					case subtraction:
						TYPE=subtraction;
						break;
					default:
						TYPE=division;
						break;
				}
				//reduced the value of multiplication
				if(TYPE==multiplication)
				{
					NUMBER=rand()%5+1;
				}
				else
				{
					NUMBER=rand()%10+1;
				}
				enemy.AddBarrier(POSX,NUMBER,TYPE,LastTime);
			}
			
			for(int i=0;i<enemy.getTotal();i++)
			{
				enemy.chain[i].setPosy(floor(static_cast<float>(SDL_GetTicks()-enemy.chain[i].getBirthtime())*v));
			}	
			
			for(int i=0;i<enemy.getTotal();i++)
			{
				if(enemy.chain[i].collision_check(Player))
				{
					int s=Player.getscore();
					switch(enemy.chain[i].getType())
					{
						case addition:
							s+=enemy.chain[i].getNumber();
							break; 
						case multiplication:
							s*=enemy.chain[i].getNumber();
							break; 
						case subtraction:
							s-=enemy.chain[i].getNumber();
							break; 
						case division:
							s/=enemy.chain[i].getNumber();
							break; 
						default:
						break;
					}
					Player.setscore(s);
					if(enemy.chain[i].getType()>1)
					{
						hurttime=SDL_GetTicks();
						Mix_PlayChannel( -1, Minus, 0 );
					}
					else
					{
						Curetime=SDL_GetTicks();
						Mix_PlayChannel( -1, Plus, 0 );
					}
					enemy.DeleteBarrier(i);
					i=i-1;
				}
				else if(enemy.chain[i].getPosy()>SCREEN_HEIGHT)
				{
					enemy.DeleteBarrier(i);
					i=i-1;
				}
					
			}	
			//Taking the highest number as score	
			if(Player.getscore()>score)
			{
				score=Player.getscore();
			}	
			if(score>highest)
			{
				highest=score;	
			}
			if(Player.getscore()<=0)
			{
				over=true;
				Mix_HaltMusic();
				LoseTime=SDL_GetTicks();	
			}	
			//Rendering part
			NewTexture bugproof1;
			if(!bugproof1.loadFromRenderedText( highText.str().c_str(),textColor))
			{
				cout<<"Unable to render score texture!"<<endl;
			}
			NewTexture bugproof2;
			if(!bugproof2.loadFromRenderedText( highText.str().c_str(),textColor))
			{
				cout<<"Unable to render score texture!"<<endl;
			}
			
			
			
			
				scoreText.str("");
				scoreText<<"Score:"<<score;	
				Scorer.oTexture=NULL;
				if(!Scorer.loadFromRenderedText( scoreText.str().c_str(),textColor))
				{
					cout<<"Unable to render score texture!"<<endl;
				}
				timeText.str("");
				timeText<<"Time:"<<(SDL_GetTicks()-StartTime)/1000<<"."<<(SDL_GetTicks()-StartTime)	%1000/10;
				
				
				
				Timer.oTexture=NULL;
				if(!Timer.loadFromRenderedText( timeText.str().c_str(),textColor))
				{
					cout<<"Unable to render time texture!"<<endl;
				}
				highText.str("");
				highText<<"The record:"<<highest;
				Highest.oTexture=NULL;
				if(!Highest.loadFromRenderedText( highText.str().c_str(),textColor))
				{
					cout<<"Unable to render score texture!"<<endl;
				} 
				  
				//clear screen
				SDL_SetRenderDrawColor(gRenderer, 0xFF,0xFF,0xFF,0xFF);
				SDL_RenderClear( gRenderer);
				
				//Render the background texture to screen
				
				if(SDL_GetTicks()/500%2==0)
				{
					Background.render(0,0);
				}
				else
				{
					Background2.render(0,0);
				}
				
				
				//Render the Barrier
				 for(int i=0;i<enemy.getTotal();i++)
				{
					enemy.chain[i].render();	
				}
				//Render the Charater
				if(SDL_GetTicks()-hurttime<=200)
				{
					Player.render(1);
				}
				else if(SDL_GetTicks()-Curetime<=200)
				{
					Player.render(2);
				}
				else
				{
					Player.render(3);
				}
				//Render the Timer and the score;
				Timer.render(0,0);
				Scorer.render(0,Timer.getHeight());
				Highest.render(0,Timer.getHeight()+Scorer.getHeight());
				//Update screen
				SDL_RenderPresent( gRenderer);
			}
			

			//Gameover part
			bool OnBack=false;
			bool pressBack=false;
			while(!quit&&!back&&over)
			{
				if( Mix_PlayingMusic() == 0 )
                {
                    //Play the opening
                    Mix_PlayMusic( OverTheme, -1 );
                }	
				while( SDL_PollEvent( &e ) != 0 )
				{
					//User requests quit
					if( e.type == SDL_QUIT )
					{
							quit = true;
					}
					if(e.type==SDL_MOUSEMOTION||e.type == SDL_MOUSEBUTTONDOWN || e.type == SDL_MOUSEBUTTONUP )
    				{
        				//Get mouse position
        				int x, y;
        				SDL_GetMouseState( &x, &y );
        				bool origin=OnBack;
        				OnBack=true;
        				if(x<SCREEN_WIDTH/2-BacktoOpen1.getWidth()/2||x>SCREEN_WIDTH/2+BacktoOpen1.getWidth()/2)
        				{
        					OnBack=false;
        					pressBack=false;
						}
						else if(y<PlayLevel-BacktoOpen1.getHeight()/2||y>PlayLevel+BacktoOpen1.getHeight()/2)
						{
							OnBack=false;
							pressBack=false;
						}
						if(OnBack)
						{
							switch(e.type)
							{
								case SDL_MOUSEBUTTONDOWN:
									pressBack=true;
									Mix_PlayChannel( -1, Push, 0 );
									break;
								case SDL_MOUSEBUTTONUP:
									back=true;
									start=false;
									Mix_HaltMusic();
									LoseTime=SDL_GetTicks();
									
							}
						}
						if(OnBack&&!origin)
						{
							Mix_PlayChannel( -1, Glissing, 0 );
						}
        			}
					
				}
				
				
				//clear screen
				SDL_SetRenderDrawColor(gRenderer, 0xFF,0xFF,0xFF,0xFF);
				SDL_RenderClear( gRenderer);
				
				//rendering
				Gameover[((SDL_GetTicks()-LoseTime)/500)%17].render(0,0);
				if(!OnBack)
				{
					BacktoOpen1.render(SCREEN_WIDTH/2-BacktoOpen1.getWidth()/2,PlayLevel-BacktoOpen1.getHeight()/2);
				}
				else
				{	if(pressBack)
					{
						BacktoOpen3.render(SCREEN_WIDTH/2-BacktoOpen3.getWidth()/2,PlayLevel-BacktoOpen3.getHeight()/2);
					}
					else
					{
					 
						BacktoOpen2.render(SCREEN_WIDTH/2-BacktoOpen2.getWidth()/2,PlayLevel-BacktoOpen2.getHeight()/2);
					
					}
				}


				Timer.render(0,0);
				Scorer.render(0,Timer.getHeight());
				Highest.render(0,Timer.getHeight()+Scorer.getHeight());
				SDL_RenderPresent( gRenderer);
			}

		}
	//ending
	closed();
	return 0;
	}
}
//loading the graphics
bool loadMedia()
{
	bool success=true;
	//load musics
	OpeningTheme = Mix_LoadMUS( "sounds/Opening Theme.mp3" );
    if( OpeningTheme == NULL )
    {
        printf( "Failed to load Opening Theme! SDL_mixer Error: %s\n", Mix_GetError() );
        success = false;
    }
    GamingTheme = Mix_LoadMUS( "sounds/Gaming Theme.mp3" );
    if( GamingTheme == NULL )
    {
        printf( "Failed to load Gaming Theme! SDL_mixer Error: %s\n", Mix_GetError() );
        success = false;
    }
    OverTheme = Mix_LoadMUS( "sounds/Over Theme.mp3" );
    if( OverTheme == NULL )
    {
        printf( "Failed to load Over Theme! SDL_mixer Error: %s\n", Mix_GetError() );
        success = false;
    }
    //load chunks
    Glissing = Mix_LoadWAV( "sounds/glissing.mp3" );
    if( Glissing == NULL )
    {
        printf( "Failed to load glissing sound effect! SDL_mixer Error: %s\n", Mix_GetError() );
        success = false;
    }
    Push = Mix_LoadWAV( "sounds/push.mp3" );
    if( Push == NULL )
    {
        printf( "Failed to load push sound effect! SDL_mixer Error: %s\n", Mix_GetError() );
        success = false;
    }
    Plus = Mix_LoadWAV( "sounds/plus.mp3" );
    if( Plus == NULL )
    {
        printf( "Failed to load plus sound effect! SDL_mixer Error: %s\n", Mix_GetError() );
        success = false;
    }
    Minus = Mix_LoadWAV( "sounds/minus.mp3" );
    if( Plus == NULL )
    {
        printf( "Failed to load minus sound effect! SDL_mixer Error: %s\n", Mix_GetError() );
        success = false;
    }
    //load pictures
	if(!Background.loadFromFile("graphics/background.png"))
	{
		cout<<"Failed to load background !"<<endl;
		success=false;
	}
	if(!Background2.loadFromFile("graphics/background2.png"))
	{
		cout<<"Failed to load background2 !"<<endl;
		success=false;
	}
	if(!Mode1.loadFromFile("graphics/MODE1.png"))
	{
		cout<<"Failed to load MODE1 !"<<endl;
		success=false;
	}
	if(!Mode2.loadFromFile("graphics/MODE2.png"))
	{
		cout<<"Failed to load MODE2 !"<<endl;
		success=false;
	}
	if(!Mode3.loadFromFile("graphics/MODE3.png"))
	{
		cout<<"Failed to load MODE3 !"<<endl;
		success=false;
	}
	if(!onedimension1.loadFromFile("graphics/1dimemtion1.png"))
	{
		cout<<"Failed to load 1dimension1 !"<<endl;
		success=false;
	}
	if(!onedimension2.loadFromFile("graphics/1dimemtion2.png"))
	{
		cout<<"Failed to load 1dimension2 !"<<endl;
		success=false;
	}
	if(!onedimension3.loadFromFile("graphics/1dimemtion3.png"))
	{
		cout<<"Failed to load 1dimension3 !"<<endl;
		success=false;
	}
	if(!twodimension1.loadFromFile("graphics/2dimemtion1.png"))
	{
		cout<<"Failed to load 2dimension1 !"<<endl;
		success=false;
	}
	if(!twodimension2.loadFromFile("graphics/2dimemtion2.png"))
	{
		cout<<"Failed to load 2dimension2 !"<<endl;
		success=false;
	}
	if(!twodimension3.loadFromFile("graphics/2dimemtion3.png"))
	{
		cout<<"Failed to load 3dimension1 !"<<endl;
		success=false;
	}
	if(!Gameover[0].loadFromFile("graphics/gameover.png"))
	{
		cout<<"Failed to load gameover !"<<endl;
		success=false;
	}
	if(!Gameover[1].loadFromFile("graphics/gameover1.png"))
	{
		cout<<"Failed to load gameover1 !"<<endl;
		success=false;
	}
	if(!Gameover[2].loadFromFile("graphics/gameover2.png"))
	{
		cout<<"Failed to load gameover2 !"<<endl;
		success=false;
	}
	if(!Gameover[3].loadFromFile("graphics/gameover3.png"))
	{
		cout<<"Failed to load gameover3 !"<<endl;
		success=false;
	}
	if(!Gameover[4].loadFromFile("graphics/gameover4.png"))
	{
		cout<<"Failed to load gameover4 !"<<endl;
		success=false;
	}
	if(!Gameover[5].loadFromFile("graphics/gameover5.png"))
	{
		cout<<"Failed to load gameover5 !"<<endl;
		success=false;
	}
	if(!Gameover[6].loadFromFile("graphics/gameover6.png"))
	{
		cout<<"Failed to load gameover6 !"<<endl;
		success=false;
	}
	if(!Gameover[7].loadFromFile("graphics/gameover7.png"))
	{
		cout<<"Failed to load gameover7 !"<<endl;
		success=false;
	}
	if(!Gameover[8].loadFromFile("graphics/gameover8.png"))
	{
		cout<<"Failed to load gameover8 !"<<endl;
		success=false;
	}
	if(!Gameover[9].loadFromFile("graphics/gameover9.png"))
	{
		cout<<"Failed to load gameover9 !"<<endl;
		success=false;
	}
	if(!Gameover[10].loadFromFile("graphics/gameover10.png"))
	{
		cout<<"Failed to load gameover10 !"<<endl;
		success=false;
	}
	if(!Gameover[11].loadFromFile("graphics/gameover11.png"))
	{
		cout<<"Failed to load gameover11 !"<<endl;
		success=false;
	}
	if(!Gameover[12].loadFromFile("graphics/gameover12.png"))
	{
		cout<<"Failed to load gameover12 !"<<endl;
		success=false;
	}
	if(!Gameover[13].loadFromFile("graphics/gameover13.png"))
	{
		cout<<"Failed to load gameover13 !"<<endl;
		success=false;
	}
	if(!Gameover[14].loadFromFile("graphics/gameover14.png"))
	{
		cout<<"Failed to load gameover14 !"<<endl;
		success=false;
	}
	if(!Gameover[15].loadFromFile("graphics/gameover15.png"))
	{
		cout<<"Failed to load gameover15 !"<<endl;
		success=false;
	}
	if(!Gameover[16].loadFromFile("graphics/gameover16.png"))
	{
		cout<<"Failed to load gameover1 !"<<endl;
		success=false;
	}
	if(!Opening.loadFromFile("graphics/opening.png"))
	{
		cout<<"Failed to load opening !"<<endl;
		success=false;
	}
	if(!Opening1.loadFromFile("graphics/opening1.png"))
	{
		cout<<"Failed to load opening !"<<endl;
		success=false;
	}
	if(!Opening2.loadFromFile("graphics/opening2.png"))
	{
		cout<<"Failed to load opening !"<<endl;
		success=false;
	}
	if(!Opening3.loadFromFile("graphics/opening3.png"))
	{
		cout<<"Failed to load opening !"<<endl;
		success=false;
	}
	if(!Opening4.loadFromFile("graphics/opening4.png"))
	{
		cout<<"Failed to load opening !"<<endl;
		success=false;
	}
	if(!StartButton1.loadFromFile("graphics/startbutton1.png"))
	{
		cout<<"Failed to load startbutton1 !"<<endl;
		success=false;
	}
	if(!StartButton2.loadFromFile("graphics/startbutton2.png"))
	{
		cout<<"Failed to load startbutton2 !"<<endl;
		success=false;
	}
	if(!StartButton3.loadFromFile("graphics/startbutton3.png"))
	{
		cout<<"Failed to load startbutton3 !"<<endl;
		success=false;
	}
	if(!BacktoOpen1.loadFromFile("graphics/backtoopen1.png"))
	{
		cout<<"Failed to load backtoopen1 !"<<endl;
		success=false;
	}
	if(!BacktoOpen2.loadFromFile("graphics/backtoopen2.png"))
	{
		cout<<"Failed to load backtoopen2 !"<<endl;
		success=false;
	}
	if(!BacktoOpen3.loadFromFile("graphics/backtoopen3.png"))
	{
		cout<<"Failed to load backtoopen3 !"<<endl;
		success=false;
	}
	//load Font
	gFont = TTF_OpenFont( "fonts/daggerdancer.ttf", 40 );
    if( gFont == NULL )
    {
        cout<<"Failed to load times font! SDL_ttf Error: "<< TTF_GetError()<<endl;
        success = false;
    }

	
	return success;
}

//initializing the screen
bool initial()
{
	bool success=true;

    if( TTF_Init() == -1 )
    {
        cout<<"SDL_ttf could not initialize! SDL_ttf Error:"<< TTF_GetError() <<endl;
        success = false;
    }
	if(SDL_Init(SDL_INIT_VIDEO| SDL_INIT_AUDIO )<0)
	{
		std::cout<<"SDL could not initialize! SDL Error:"<<SDL_GetError()<<endl;
		success = false;
	}
	else
	{
		if( !SDL_SetHint( SDL_HINT_RENDER_SCALE_QUALITY, "1" ) )
		{
			std::cout<<"Warning: Linear texture filtering not enabled!";
		}
		//create the window
		gWindow = SDL_CreateWindow( "Run and Pass", SDL_WINDOWPOS_UNDEFINED, SDL_WINDOWPOS_UNDEFINED, SCREEN_WIDTH, SCREEN_HEIGHT, SDL_WINDOW_SHOWN );
		if( gWindow == NULL )
		{
			std::cout<<"Window could not be created! SDL Error:"<<SDL_GetError<<endl;
			success = false;
		}
		else
		{
			
		
		
			gRenderer = SDL_CreateRenderer( gWindow, -1, SDL_RENDERER_ACCELERATED | SDL_RENDERER_PRESENTVSYNC );
			if( gRenderer == NULL )
			{
				std::cout<<"Renderer could not be created! SDL Error:"<<SDL_GetError()<<endl;
				success = false;
			}
			else
			{
				SDL_SetRenderDrawColor( gRenderer, 0xFF, 0xFF, 0xFF, 0xFF );

				int imgFlags = IMG_INIT_PNG;
				if( !( IMG_Init( imgFlags ) & imgFlags ) )
				{
					std::cout<<"SDL_image could not initialize! SDL_image Error:"<<IMG_GetError()<<endl;
					success = false;
				}
				if( Mix_OpenAudio( 44100, MIX_DEFAULT_FORMAT, 2, 2048 ) < 0 )
				{
					printf( "SDL_mixer could not initialize! SDL_mixer Error: %s\n", Mix_GetError() );
					success = false;
				}
			}
		}
	}
	
	return success;
}


void closed()
{
	//Free the texture
	Background.free();
	Background2.free();
	for(int i=0;i<17;i++)
	{
		Gameover[17].free();
	}	
	Opening.free();
	Opening1.free();
	Opening2.free();
	Opening3.free();
	Opening4.free();
	StartButton1.free();
	StartButton2.free();
	StartButton3.free();
	BacktoOpen1.free();
	BacktoOpen2.free();
	BacktoOpen3.free(); 
	Mode1.free();
	Mode2.free();
	Mode3.free();
	onedimension1.free();
	onedimension2.free();
	onedimension3.free();
	twodimension1.free();
	twodimension2.free();
	twodimension3.free();
	
	//free the sounds
	Mix_FreeMusic(OpeningTheme);
	OpeningTheme=NULL;
	Mix_FreeMusic(GamingTheme);
	GamingTheme=NULL;
	Mix_FreeMusic(OverTheme);
	OverTheme=NULL;
	Mix_FreeChunk(Glissing);
	Glissing=NULL;
	Mix_FreeChunk(Push);
	Push=NULL;
	Mix_FreeChunk(Plus);
	Plus=NULL;
	Mix_FreeChunk(Minus);
	Minus=NULL; 
	//Free the font
	TTF_CloseFont( gFont );
    gFont = NULL;
	
	//Destroy the window	
	SDL_DestroyRenderer( gRenderer );
	SDL_DestroyWindow( gWindow );
	gWindow = NULL;
	gRenderer = NULL;

	//Quit the SDL subsystems
	TTF_Quit();
	Mix_Quit();
	IMG_Quit();
	SDL_Quit();
}



NewTexture::NewTexture()
{
	oTexture=NULL;
	mWidth=0;
	mHeight=0; 
}

NewTexture::~NewTexture()
{
	//delecte
	free();
}

void NewTexture::free()
{
	if(oTexture!=NULL)
	{
		SDL_DestroyTexture(oTexture);
		oTexture=NULL;
		mWidth=0;
		mHeight=0;
	}
}

void NewTexture::render(int x, int y)
{
	if(oTexture==NULL)
	{
		return;
	}
	SDL_Rect renderQuad={x,y, mWidth, mHeight};
	SDL_RenderCopy(gRenderer, oTexture, NULL, &renderQuad);
}

int NewTexture::getWidth()
{
	return mWidth;
}

int NewTexture::getHeight()
{
	return mHeight;
}

bool NewTexture::loadFromFile(std::string path)
{
	//Get a blank Texture
	free();
	
	//load the image
	SDL_Surface* loadSurface=IMG_Load(path.c_str());
	if(loadSurface==NULL)
	{
		std::cout<<"Unable to load image "<<path.c_str()<<"! ADL_image Error:"<< IMG_GetError()<<std::endl;	
	}
	else
	{
			oTexture = SDL_CreateTextureFromSurface( gRenderer, loadSurface );
			if(oTexture==NULL)
			{
				std::cout<<"Unable to create texture from "<<path.c_str()<<" ! SDL Error:"<<SDL_GetError()<<std::endl;
			}
			else
			{
				mWidth=loadSurface->w;
				mHeight=loadSurface->h;
			}
	
			//Free the loadSurface
			SDL_FreeSurface(loadSurface);
	} 

	return oTexture!=NULL;
}

bool NewTexture::loadFromRenderedText( std::string textureText, SDL_Color textColor )
{
    //Get rid of preexisting texture
    free();

    //Render text surface
    SDL_Surface* textSurface = TTF_RenderText_Solid( gFont, textureText.c_str(), textColor );
    if( textSurface == NULL )
    {
        printf( "Unable to render text surface! SDL_ttf Error: %s\n", TTF_GetError() );
    }
    else
    {
        //Create texture from surface pixels
        oTexture = SDL_CreateTextureFromSurface( gRenderer, textSurface );
        if( oTexture == NULL )
        {
            printf( "Unable to create texture from rendered text! SDL Error: %s\n", SDL_GetError() );
        }
        else
        {
            //Get image dimensions
            mWidth = textSurface->w;
            mHeight = textSurface->h;
        }

        //Get rid of old surface
        SDL_FreeSurface( textSurface );
    }
    
    //Return success
    return oTexture != NULL;
}


Pawn::Pawn()
{
	//The image is the character
	if(!Pawn_image.loadFromFile("graphics/character.png"))
	{
		cout<<"Failed to load charater !"<<endl;
	}
	pos_x=SCREEN_WIDTH/2-Pawn_image.getWidth()/2;
	pos_y=PlayLevel-Pawn_image.getHeight()/2;
	score=1;
}

Pawn::~Pawn()
{
	free();
}

void Pawn::free() 
{
	Pawn_image.free();
	score=0;
	pos_x=0;
	pos_y=0;
}

void Pawn::render(int hurt)
{
	switch(hurt)
	{
		case 1:
			if(!Pawn_image.loadFromFile("graphics/character1.png"))
		{
			cout<<"Failed to load charater1 !"<<endl;
		}
		break;
		case 2:
			if(!Pawn_image.loadFromFile("graphics/character2.png"))
		{
			cout<<"Failed to load charater2 !"<<endl;
		}
		break;
		case 3:
			if(!Pawn_image.loadFromFile("graphics/character.png"))
		{
			cout<<"Failed to load charater !"<<endl;
		}
		break;
	}
	
	Pawn_image.render(pos_x-Pawn_image.getWidth()/2,pos_y-Pawn_image.getWidth()/2);
	std::stringstream Nowscore;
	NewTexture nowscore;
	Nowscore.str("");
	Nowscore<<score;
	if(!nowscore.loadFromRenderedText( Nowscore.str().c_str(),textColor))
		{
			cout<<"Unable to render property texture!"<<endl;
		} 
	nowscore.render(SCREEN_WIDTH/2-nowscore.getWidth()/2,4*nowscore.getHeight());
}

int Pawn::getPosx()
{
	return pos_x;
}

int Pawn::getPosy()
{
	return pos_y;
}

void Pawn::setPosx(int x)
{
	pos_x=x;
}

void Pawn::setPosy(int y)
{
	pos_y=y;
}

void Pawn::FollowMouse(SDL_Event *e,bool dmode)
{
	if( e->type == SDL_MOUSEMOTION)
	{
		//Get the position of the Mouse
		int x,y;
		SDL_GetMouseState( &x, &y );
		
		//Check if the Mouse is inside the window
		bool inside=true;
		if(x<0||x>SCREEN_WIDTH)
		{
			inside=false;
		}
		if(inside)
		{
			pos_x=x;
			if(dmode==true)
			{
				if(y>SCREEN_HEIGHT/2)
				{
				pos_y=y;
				}
			}
		}
	}
}

Barrier::Barrier(int POSX, int NUMBER, int TYPE, int BIRTHTIME)
{
	if(!Barrier_image.loadFromFile("graphics/+1.png"))
	{
		cout<<"Failed to load obstruction !"<<endl;
	}
	if(POSX-Barrier_image.getWidth()/2<0)
	{
		pos_x=Barrier_image.getWidth()/2;
	}
	else if(POSX+Barrier_image.getWidth()/2>SCREEN_WIDTH)
	{
		pos_x=SCREEN_WIDTH-Barrier_image.getWidth()/2;
	}
	else
	{
	pos_x=POSX;
	}
	pos_y=0;
	number=NUMBER;
	type=TYPE;
	birthtime=BIRTHTIME;
	
}

Barrier::~Barrier()
{
	free();
}
void Barrier::free()
{
	pos_x=0;
	pos_y=0;
	number=0;
	type=0;
	birthtime=0;
	Barrier_image.free();
}
void Barrier::render()
{	
	//place to put the baiier
	int u,v;

	u=pos_x-Barrier_image.getWidth()/2;
	v=pos_y-Barrier_image.getHeight()/2;

	switch(type)
	{
		case addition:
			switch(number)
			{
				case 1:
				if(!Barrier_image.loadFromFile("graphics/+1.png"))
				{
					cout<<"Failed to load +1 !"<<endl;
				}
				break;
				case 2:
				if(!Barrier_image.loadFromFile("graphics/+2.png"))
				{
					cout<<"Failed to load +2 !"<<endl;
				}
				break;
				case 3:
				if(!Barrier_image.loadFromFile("graphics/+3.png"))
				{
					cout<<"Failed to load +3 !"<<endl;
				}
				break;
				case 4:
				if(!Barrier_image.loadFromFile("graphics/+4.png"))
				{
					cout<<"Failed to load +4 !"<<endl;
				}
				break;
				case 5:
				if(!Barrier_image.loadFromFile("graphics/+5.png"))
				{
					cout<<"Failed to load +5 !"<<endl;
				}
				break;
				case 6:
				if(!Barrier_image.loadFromFile("graphics/+6.png"))
				{
					cout<<"Failed to load +6 !"<<endl;
				}
				break;
				case 7:
				if(!Barrier_image.loadFromFile("graphics/+7.png"))
				{
					cout<<"Failed to load +7 !"<<endl;
				}
				break;
				case 8:
				if(!Barrier_image.loadFromFile("graphics/+8.png"))
				{
					cout<<"Failed to load +8 !"<<endl;
				}
				break;
				case 9:
				if(!Barrier_image.loadFromFile("graphics/+9.png"))
				{
					cout<<"Failed to load +9 !"<<endl;
				}
				break;
				case 10:
				if(!Barrier_image.loadFromFile("graphics/+10.png"))
				{
					cout<<"Failed to load +10 !"<<endl;
				}
				break;
			}
			break;
		case multiplication:
			switch(number)
			{
				case 1:
				if(!Barrier_image.loadFromFile("graphics/m1.png"))
				{
					cout<<"Failed to load m1 !"<<endl;
				}
				break;
				case 2:
				if(!Barrier_image.loadFromFile("graphics/m2.png"))
				{
					cout<<"Failed to load m2 !"<<endl;
				}
				break;
				case 3:
				if(!Barrier_image.loadFromFile("graphics/m3.png"))
				{
					cout<<"Failed to load m3 !"<<endl;
				}
				break;
				case 4:
				if(!Barrier_image.loadFromFile("graphics/m4.png"))
				{
					cout<<"Failed to load m4 !"<<endl;
				}
				break;
				case 5:
				if(!Barrier_image.loadFromFile("graphics/m5.png"))
				{
					cout<<"Failed to load m5 !"<<endl;
				}
				break;
				case 6:
				if(!Barrier_image.loadFromFile("graphics/m6.png"))
				{
					cout<<"Failed to load m6 !"<<endl;
				}
				break;
				case 7:
				if(!Barrier_image.loadFromFile("graphics/m7.png"))
				{
					cout<<"Failed to load m7 !"<<endl;
				}
				break;
				case 8:
				if(!Barrier_image.loadFromFile("graphics/m8.png"))
				{
					cout<<"Failed to load m8 !"<<endl;
				}
				break;
				case 9:
				if(!Barrier_image.loadFromFile("graphics/m9.png"))
				{
					cout<<"Failed to load m9 !"<<endl;
				}
				break;
				case 10:
				if(!Barrier_image.loadFromFile("graphics/m10.png"))
				{
					cout<<"Failed to load m10 !"<<endl;
				}
				break;
			}
			break;
		case subtraction:
			switch(number)
			{
				case 1:
				if(!Barrier_image.loadFromFile("graphics/s1.png"))
				{
					cout<<"Failed to load s1 !"<<endl;
				}
				break;
				case 2:
				if(!Barrier_image.loadFromFile("graphics/s2.png"))
				{
					cout<<"Failed to load s2 !"<<endl;
				}
				break;
				case 3:
				if(!Barrier_image.loadFromFile("graphics/s3.png"))
				{
					cout<<"Failed to load s3 !"<<endl;
				}
				break;
				case 4:
				if(!Barrier_image.loadFromFile("graphics/s4.png"))
				{
					cout<<"Failed to load s4 !"<<endl;
				}
				break;
				case 5:
				if(!Barrier_image.loadFromFile("graphics/s5.png"))
				{
					cout<<"Failed to load s5 !"<<endl;
				}
				break;
				case 6:
				if(!Barrier_image.loadFromFile("graphics/s6.png"))
				{
					cout<<"Failed to load s6 !"<<endl;
				}
				break;
				case 7:
				if(!Barrier_image.loadFromFile("graphics/s7.png"))
				{
					cout<<"Failed to load s7 !"<<endl;
				}
				break;
				case 8:
				if(!Barrier_image.loadFromFile("graphics/s8.png"))
				{
					cout<<"Failed to load s8 !"<<endl;
				}
				break;
				case 9:
				if(!Barrier_image.loadFromFile("graphics/s9.png"))
				{
					cout<<"Failed to load s9 !"<<endl;
				}
				break;
				case 10:
				if(!Barrier_image.loadFromFile("graphics/s10.png"))
				{
					cout<<"Failed to load s10 !"<<endl;
				}
				break;
			}
			break;
			case division:
			switch(number)
			{
				case 1:
				if(!Barrier_image.loadFromFile("graphics/d1.png"))
				{
					cout<<"Failed to load d1 !"<<endl;
				}
				break;
				case 2:
				if(!Barrier_image.loadFromFile("graphics/d2.png"))
				{
					cout<<"Failed to load d2 !"<<endl;
				}
				break;
				case 3:
				if(!Barrier_image.loadFromFile("graphics/d3.png"))
				{
					cout<<"Failed to load d3 !"<<endl;
				}
				break;
				case 4:
				if(!Barrier_image.loadFromFile("graphics/d4.png"))
				{
					cout<<"Failed to load d4 !"<<endl;
				}
				break;
				case 5:
				if(!Barrier_image.loadFromFile("graphics/d5.png"))
				{
					cout<<"Failed to load d5 !"<<endl;
				}
				break;
				case 6:
				if(!Barrier_image.loadFromFile("graphics/d6.png"))
				{
					cout<<"Failed to load d6 !"<<endl;
				}
				break;
				case 7:
				if(!Barrier_image.loadFromFile("graphics/d7.png"))
				{
					cout<<"Failed to load d7 !"<<endl;
				}
				break;
				case 8:
				if(!Barrier_image.loadFromFile("graphics/d8.png"))
				{
					cout<<"Failed to load d8 !"<<endl;
				}
				break;
				case 9:
				if(!Barrier_image.loadFromFile("graphics/d9.png"))
				{
					cout<<"Failed to load d9 !"<<endl;
				}
				break;
				case 10:
				if(!Barrier_image.loadFromFile("graphics/d10.png"))
				{
					cout<<"Failed to load d10 !"<<endl;
				}
				break;
			}
			break;

	}
	Barrier_image.render(u,v);

	
}

		
int Barrier::getPosx()
{
	return pos_x;
}

int Barrier::getPosy()
{
	return pos_y;
}

int Barrier::getNumber()
{
	return number;
}
int Barrier::getType()
{
	return type;
}
int Barrier::getBirthtime()
{
	return birthtime;
}
void Barrier::setPosx(int x)
{
	pos_x=x;
}
void Barrier::setPosy(int y)
{
	pos_y=y;
}
void Barrier::setNumber(int n)
{
	number=n;
}
void Barrier::setType(int t)
{
	type=t;
}
void Barrier::setBirthtime(int T)
{
	birthtime=T;
}
		
bool Barrier::collision_check(Pawn player)
{
	bool collision=false;
	int Delta_x=(Barrier_image.getWidth()+player.getTexture().getWidth())/2;
	int Delta_y=(Barrier_image.getHeight()+player.getTexture().getHeight())/2;
	if((pos_y-player.getPosy())<Delta_y*0.7&&(pos_y-player.getPosy())>-Delta_y*0.7)
	{
		if((pos_x-player.getPosx())<Delta_x*0.9&&(pos_x-player.getPosx())>-Delta_x*0.9)
		{
			collision=true;
		}
	}
	
	return collision;	
}

BarrierChain::BarrierChain()
{
	Total=0;
	chain=NULL;
}

BarrierChain::~BarrierChain()
{
	Total=0;
	for(int i=0;i<Total;i++)
	{
		chain[i].free();
	}
	delete [] chain;
	chain=NULL; 
}

void BarrierChain::AddBarrier(int POSX, int NUMBER, int TYPE, int BIRTHTIME)
{
	Barrier Newchain[Total+1];
	for(int i=0;i<Total;i++)
	{
		Newchain[i]=chain[i];
	}
	for(int i=0;i<Total;i++)
	{
		chain[i].free();
	}
	Newchain[Total]=Barrier(POSX,NUMBER,TYPE,BIRTHTIME);
	
	Total++;
	delete [] chain;
	chain=new Barrier [Total];
	for(int i=0;i<Total;i++)
	{
		chain[i]=Newchain[i];
	}	
}

void BarrierChain::DeleteBarrier(int k)
{
	if(Total==0)
	{
		return;
	}
	Barrier Newchain[Total-1];
	for(int i=0;i<k;i++)
	{
		Newchain[i]=chain[i];
	}
	for(int i=k+1;i<Total;i++)
	{
		Newchain[i-1]=chain[i];
	}
	Total--;
	delete [] chain;
	chain = new Barrier [Total];
	for(int i=0;i<Total;i++)
	{
		chain[i]=Newchain[i];
	}
}


