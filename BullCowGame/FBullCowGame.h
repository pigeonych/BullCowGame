#pragma once
#include <string>

using FString = std::string;
using int32 = int;

struct FBullCowCount {
	int32 Bulls = 0;
	int32 Cows = 0;
};

enum class EGuessStatus {
	Invalid,
	OK,
	Not_Isogram,
	Wrong_Length,
	Not_Lowercase
};



class FBullCowGame {
public:
	FBullCowGame();

	int32 GetCurrentTry() const;
	int32 GetMaxTries() const;
	bool IsGameWon() const;
	int32 GetHiddenWordLength() const;
	EGuessStatus CheckGuessValidity(FString Guess) const;

	void Reset();
	FBullCowCount SubmitValidGuess(FString Guess);


private:
	int32 MyCurrentTry;
	FString MyHiddenWord;
	bool bGameWon;

	bool IsIsogram(FString) const;
	bool IsLowercase(FString) const;
};