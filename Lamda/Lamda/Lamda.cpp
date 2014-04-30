// Lamda.cpp : Defines the entry point for the console application.
//

#include "stdafx.h"
#include <string>
#include <list>

class Character
{
public:	
	std::string Name() const { return name; }
	void Name( std::string val ) { name = val; }

private:
	std::string name;
};

void CharacterSortByName();



int _tmain(int argc, _TCHAR* argv[])
{
	CharacterSortByName();

	getchar();

	return 0;
}

void CharacterSortByName()
{
	std::list<Character> charList;

	// 값 입력
	Character agebreak;
	agebreak.Name( "agebreak" );
	charList.push_back( agebreak );
	
	Character sm9;
	sm9.Name( "sm9" );
	charList.push_back( sm9 );


	Character miku;
	miku.Name( "miku" );
	charList.push_back( miku );


	Character mikumiku;
	mikumiku.Name( "mikumiku" );
	charList.push_back( mikumiku );

	// 작으면 true, 크면 false 로 sort
	charList.sort( []( const Character& a, const Character& b )->bool {
		unsigned int len = a.Name().length() > b.Name().length() ? b.Name().length() : a.Name().length();
		for ( unsigned int i = 0; i < len; ++i )		
		{
			if ( a.Name()[i] < b.Name()[i] )
				return true;
			else if ( a.Name()[i] > b.Name()[i] )
				return false;
		}
		// 같을 시에는 더 긴 단어를 뒤로..
		if ( a.Name().length() < b.Name().length() )
			return true;
		else
			return false;
	} );

	//. 결과 확인
	for ( auto a : charList )
	{
		printf( "%s\n", a.Name().c_str() );
	}

}

