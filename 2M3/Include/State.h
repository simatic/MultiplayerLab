#ifndef BOOK_STATE_HPP
#define BOOK_STATE_HPP

#include "StateIdentifiers.h"
#include "ResourceIdentifiers.h"

#include <SFML/System/Time.hpp>
#include <SFML/Window/Event.hpp>
#include <SFML/Graphics.hpp>
#include <memory>


namespace sf
{
	class RenderWindow;
}

class StateStack;
//class MusicPlayer;
//class SoundPlayer;
class KeyBinding;

class State
{
	public:
		typedef std::unique_ptr<State> Ptr;

		struct Context
		{
			Context();
			Context(int& uid, sf::RenderTexture& target, TextureHolder& textures, FontHolder& fonts,
									/*MusicPlayer& music, SoundPlayer& sounds,*/ KeyBinding& keys, sf::Mutex& appMutex, sf::Color associatedColor);
			int* uid;
			sf::RenderTexture*	target;
			TextureHolder*		textures;
			FontHolder*			fonts;
			//MusicPlayer*		music;
			//SoundPlayer*		sounds;
			KeyBinding*			keys;
			sf::Mutex* mutex;
			sf::Color associatedColor;
		};

	public:
							State(StateStack& stack, Context context);
		virtual				~State();

		virtual void		draw() = 0;
		virtual bool		update(sf::Time dt) = 0;
		virtual bool		tick();
		virtual bool		handleEvent(const sf::Event& event) = 0;

		virtual void		onActivate();
		virtual void		onDestroy();


	protected:
		void				requestStackPush(States::ID stateID);
		void				requestStackPop();
		void				requestStateClear();

		Context				getContext() const;


	private:
		StateStack*			mStack;
		Context				mContext;
};

#endif // BOOK_STATE_HPP
