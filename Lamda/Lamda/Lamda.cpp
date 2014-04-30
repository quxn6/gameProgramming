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

	// �� �Է�
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

	// ������ true, ũ�� false �� sort
	charList.sort( []( const Character& a, const Character& b )->bool {
		unsigned int len = a.Name().length() > b.Name().length() ? b.Name().length() : a.Name().length();
		for ( unsigned int i = 0; i < len; ++i )		
		{
			if ( a.Name()[i] < b.Name()[i] )
				return true;
			else if ( a.Name()[i] > b.Name()[i] )
				return false;
		}
		// ���� �ÿ��� �� �� �ܾ �ڷ�..
		if ( a.Name().length() < b.Name().length() )
			return true;
		else
			return false;
	} );

	//. ��� Ȯ��
	for ( auto a : charList )
	{
		printf( "%s\n", a.Name().c_str() );
	}

}

