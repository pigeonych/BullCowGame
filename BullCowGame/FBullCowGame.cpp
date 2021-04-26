/*Тут по библиотекам похожая ситуация, только работаем без стандартной библиотеки <iostream>, что? Да. Заменяем её кучей
std::cout и т.д. Ещё одной вкусной фичей будет задействованная библиотека каталогов <map>, которую мы в итоге заменим прототипом
TMap для работы с UE4 в дальнейшем. Да и если честно, синтаксис там полегче будет.*/

#pragma once
#include "FBullCowGame.h"
#include <map>
#define TMap std::map

/*Собственно наш конструктор автоматически ресетает все нужные значения и вводит секретное слово за нас.*/
FBullCowGame::FBullCowGame() { Reset(); }

/*Куча гетеров, работа каждого максимально примитивна. Возвращать значения.
Различие только в типах возвращаемого значения.*/
int32 FBullCowGame::GetCurrentTry () const { return MyCurrentTry; }
int32 FBullCowGame::GetHiddenWordLength() const { return MyHiddenWord.length(); }
bool FBullCowGame::IsGameWon() const { return bGameWon; }

/*Вот тут уже повкуснее функция. Она нужна для выставления количества максимальных попыток при учете длины слова.
В нём задействован ранее упомянутый каталог TMap. Здесь работает наподобие двумерного массива, но работать с ним намного приятнее 
за счёт доступности возвращаемого значения.*/
int32 FBullCowGame::GetMaxTries() const { 
	TMap <int32, int32> WordLengthMaxTries{ {3, 4}, {4, 7}, {5, 10}, {6, 15}, {7, 20} };
	return WordLengthMaxTries[MyHiddenWord.length()];
}

/*Вообщем еще одна вкусняшка. Но на этот раз чуть замудреннее. Буду объяснять по ходу.*/
bool FBullCowGame::IsIsogram(FString Word) const
{
	/*Принимаем в себя значения словом, тем же самым, что вводит пользователь, только оно ещё пройдет до этого через
	Guess в CheckGuessValidity. Собственно первый if стейтмент, немного тригерный для носителей английского языка, т.к. 
	у них почти что нет слов с одной буквой. Но в данном случае, это неуместно, т.к. функция предназначена для выявления повторений
	букв, поэтому логично предположить что если букв меньше чем 2, то повторений быть не может. Поэтому возвращаем true и говорим, всё ок, 
	это вы вообще не к нам.*/
	if (Word.length() <= 1) { return true; }

	/*Принцип же работы каталога заключается в том, что он принимает в себя значения, и если это буквы, во первых понижает их (нет, не в 
	звании) методом tolower() и чекает есть ли в них повторения. Если есть, то на держи тебе false, если нет то на тебе true.*/
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

/*Принцип работы максимально идентичен методу IsIsogram, только здесь мы проверяем не повторение букв, а их кейс. То есть, если 
пользователь случайно нажал на Caps Lock, то мы аккуратно оповестим его об этом.*/
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
	/* Инициализация скрытого слова хардкодом, типа const чтобы нельзя было поменять
	случайно или не случайно в дальнейшем */
	const FString HIDDEN_WORD = "ant"; 

	/* Передача скрытого слова в переменную, которая находится в приватном доступе,
	что значит во первых, что мы можем вызывать для неё метод length() и во вторых это просто безопасно*/
	MyHiddenWord = HIDDEN_WORD;  

	/*Изначально функция Reset() предназначалась для ресета приватных значений, т.е. инициализации переменных
	MyCurrentTry и MyMaxTries (которой к концу сборки уже нет, храни тебя бог),
	но потом оказалось что она может неплохо послужить компактности программы,
	если вовремя и в нужном месте рекольнется.. :)*/
	MyCurrentTry = 1;

	/*Одинокая, булевая переменная которая служит для того, чтобы выяснить
	выиграл пользователь или нет. Тут просто инициализация значением false, т.к. инициализировать
	переменные в private некрасиво и вообще фу*/
	bGameWon = false;

	return;
}

/*Сами кейсы ошибок. Типом возвращаемых значений служат сами константы.*/
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

/*Проверка принимаего слова на содержание n-word. Ладно, конечно же это шутка. Но функция, всё же, проверяет слово на соотвествие
скрытому слову путём проведения его через двойной цикл. Возвращает значение как мы уже неоднакратно указывали в виде двух целочисленных
значений.*/
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
