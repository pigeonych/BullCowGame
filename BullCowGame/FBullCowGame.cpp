/*��� �� ����������� ������� ��������, ������ �������� ��� ����������� ���������� <iostream>, ���? ��. �������� � �����
std::cout � �.�. ��� ����� ������� ����� ����� ��������������� ���������� ��������� <map>, ������� �� � ����� ������� ����������
TMap ��� ������ � UE4 � ����������. �� � ���� ������, ��������� ��� ������� �����.*/

#pragma once
#include "FBullCowGame.h"
#include <map>
#define TMap std::map

/*���������� ��� ����������� ������������� �������� ��� ������ �������� � ������ ��������� ����� �� ���.*/
FBullCowGame::FBullCowGame() { Reset(); }

/*���� �������, ������ ������� ����������� ����������. ���������� ��������.
�������� ������ � ����� ������������� ��������.*/
int32 FBullCowGame::GetCurrentTry () const { return MyCurrentTry; }
int32 FBullCowGame::GetHiddenWordLength() const { return MyHiddenWord.length(); }
bool FBullCowGame::IsGameWon() const { return bGameWon; }

/*��� ��� ��� ��������� �������. ��� ����� ��� ����������� ���������� ������������ ������� ��� ����� ����� �����.
� �� ������������ ����� ���������� ������� TMap. ����� �������� ��������� ���������� �������, �� �������� � ��� ������� �������� 
�� ���� ����������� ������������� ��������.*/
int32 FBullCowGame::GetMaxTries() const { 
	TMap <int32, int32> WordLengthMaxTries{ {3, 4}, {4, 7}, {5, 10}, {6, 15}, {7, 20} };
	return WordLengthMaxTries[MyHiddenWord.length()];
}

/*������� ��� ���� ���������. �� �� ���� ��� ���� �����������. ���� ��������� �� ����.*/
bool FBullCowGame::IsIsogram(FString Word) const
{
	/*��������� � ���� �������� ������, ��� �� �����, ��� ������ ������������, ������ ��� ��� ������� �� ����� �����
	Guess � CheckGuessValidity. ���������� ������ if ���������, ������� ��������� ��� ��������� ����������� �����, �.�. 
	� ��� ����� ��� ��� ���� � ����� ������. �� � ������ ������, ��� ���������, �.�. ������� ������������� ��� ��������� ����������
	����, ������� ������� ������������ ��� ���� ���� ������ ��� 2, �� ���������� ���� �� �����. ������� ���������� true � �������, �� ��, 
	��� �� ������ �� � ���.*/
	if (Word.length() <= 1) { return true; }

	/*������� �� ������ �������� ����������� � ���, ��� �� ��������� � ���� ��������, � ���� ��� �����, �� ������ �������� �� (���, �� � 
	������) ������� tolower() � ������ ���� �� � ��� ����������. ���� ����, �� �� ����� ���� false, ���� ��� �� �� ���� true.*/
	TMap <char, bool> LetterSeen;
	for (auto Letter : Word) {
		 Letter = tolower(Letter); 
		if (LetterSeen[Letter]) {
			return false;
		}
		else {
			LetterSeen[Letter] = true;
		}
	}
	
	return true;
}

/*������� ������ ����������� ��������� ������ IsIsogram, ������ ����� �� ��������� �� ���������� ����, � �� ����. �� ����, ���� 
������������ �������� ����� �� Caps Lock, �� �� ��������� ��������� ��� �� ����.*/
bool FBullCowGame::IsLowercase(FString Word) const
{
	if (Word.length() <= 1) { return true; }
	TMap <char, bool> LetterSeen;
	for (auto Letter : Word) {
		if (Letter == toupper(Letter)) { // 
			return false;
		}
		else {
			LetterSeen[Letter] = true;
		}
	}
	return true;
}


void FBullCowGame::Reset()
{
	/* ������������� �������� ����� ���������, ���� const ����� ������ ���� ��������
	�������� ��� �� �������� � ���������� */
	const FString HIDDEN_WORD = "ant"; 

	/* �������� �������� ����� � ����������, ������� ��������� � ��������� �������,
	��� ������ �� ������, ��� �� ����� �������� ��� �� ����� length() � �� ������ ��� ������ ���������*/
	MyHiddenWord = HIDDEN_WORD;  

	/*���������� ������� Reset() ��������������� ��� ������ ��������� ��������, �.�. ������������� ����������
	MyCurrentTry � MyMaxTries (������� � ����� ������ ��� ���, ����� ���� ���),
	�� ����� ��������� ��� ��� ����� ������� ��������� ������������ ���������,
	���� ������� � � ������ ����� �����������.. :)*/
	MyCurrentTry = 1;

	/*��������, ������� ���������� ������� ������ ��� ����, ����� ��������
	������� ������������ ��� ���. ��� ������ ������������� ��������� false, �.�. ����������������
	���������� � private ��������� � ������ ��*/
	bGameWon = false;

	return;
}

/*���� ����� ������. ����� ������������ �������� ������ ���� ���������.*/
EGuessStatus FBullCowGame::CheckGuessValidity(FString Guess) const
{
	if (!IsIsogram(Guess)) {
		return EGuessStatus::Not_Isogram;
	}
	else if (!IsLowercase(Guess)) {
		return EGuessStatus::Not_Lowercase;
	}
	else if (Guess.length() != GetHiddenWordLength()) {
		return EGuessStatus::Wrong_Length;
	}
	else {
		return EGuessStatus::OK;
	}
}

/*�������� ���������� ����� �� ���������� n-word. �����, ������� �� ��� �����. �� �������, �� ��, ��������� ����� �� �����������
�������� ����� ���� ���������� ��� ����� ������� ����. ���������� �������� ��� �� ��� ������������ ��������� � ���� ���� �������������
��������.*/
FBullCowCount FBullCowGame::SubmitValidGuess(FString Guess)
{
	bGameWon = false;
	MyCurrentTry++;
	int32 WordLength = MyHiddenWord.length();
	FBullCowCount BullCowCount;

	// look through all letters in the hidden word
	for (int32 MHWChar = 0; MHWChar < WordLength; MHWChar++) {
		// compare letters against the guess word
		for (int32 GChar = 0; GChar < WordLength; GChar++) {
			if (Guess[GChar] == MyHiddenWord[MHWChar]){
				if(MHWChar == GChar){
					BullCowCount.Bulls++;
				}
				else {
					BullCowCount.Cows++;
				}
			}
			
		}

	}

	if (Guess == MyHiddenWord) {
		bGameWon = true;
	}
	else {
		bGameWon = false;
	}

	return BullCowCount;
}
