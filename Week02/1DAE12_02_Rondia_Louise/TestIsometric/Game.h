#pragma once
#include "BaseGame.h"
#include "Texture.h"
#include "Map.h"
#include "Renderer.h"
#include "Camera.h"
#include "CharacterRenderable.h"

class Game : public BaseGame
{
public:
	explicit Game( const Window& window );
	Game(const Game& other) = delete;
	Game& operator=(const Game& other) = delete;
	Game( Game&& other) = delete;
	Game& operator=(Game&& other) = delete;
	// http://isocpp.github.io/CppCoreGuidelines/CppCoreGuidelines#Rh-override
	~Game();

	void Update( float elapsedSec ) override;
	void Draw( ) const override;

	// Event handling
	void ProcessKeyDownEvent( const SDL_KeyboardEvent& e ) override;
	void ProcessKeyUpEvent( const SDL_KeyboardEvent& e ) override;
	void ProcessMouseMotionEvent( const SDL_MouseMotionEvent& e ) override;
	void ProcessMouseDownEvent( const SDL_MouseButtonEvent& e ) override;
	void ProcessMouseUpEvent( const SDL_MouseButtonEvent& e ) override;

private:

	// FUNCTIONS
	void Initialize();
	void Cleanup( );
	void ClearBackground( ) const;

	Map			m_Map{};
	Renderer	m_Renderer; // not possible to use {} bc no default
	Camera		m_Camera{};
	Vector2i	m_Dir{};
	CharacterRenderable *m_Character;
};


// TO DO

// camera : 
//		- suit le perso
//		- click droit bouge
//		- zoom dezoom
// 
// perso :
//		
// murs:
//		- si perso derriere, ne pas afficher
//		- connecter murs entre eux (map chaque mur en key et en value une liste de murs connectes
//			murs de devant sont pas connectes a ceux derriere mais ceux de derrieres sont connectes a ceux de devant
// 
//	