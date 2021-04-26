/*Добро пожаловать в исходный код игры "Bulls and Cows" 
Первое, что вы увидите это пару стандартных библиотек и добавленный файл заголовка
в котором находится вся архитектура программы.*/

#pragma once
#include <iostream>
#include <string>
#include "FBullCowGame.h"

/*Для более корректной работы кода на всех устройствах, да и для будущей практики работы 
с Unreal Engine 4, я перераспределяю типы переменных из стандартных int и string в int32 и FString
соответственно.
*/
using FText = std::string; 
using int32 = int;


/*Декларация функций, которые в дальнейшем послужат компактности main функции*/
void PrintIntro(); 
void PlayGame();
FText GetValidGuess();
bool AskToPlayAgain();
void PrintGameSummary();

/* Декларация переменной BCGame для удобного доступа к любым методам класса FBullCowGame 
в любой точке доступа и области видимости функций*/
FBullCowGame BCGame;

/*Сама main функция*/
int main()
{
	bool bPlayAgain = false; // Инициализация булевой переменной для возможности сыграть заново или выйти из игры

	do {
		PrintIntro();
		PlayGame();
		PrintGameSummary();
		bPlayAgain = AskToPlayAgain(); // Запрашивание у пользователя хочет ли он сыграть снова
	} while (bPlayAgain);

	return 0;
}

/*Функция для приветствия пользователя*/
void PrintIntro()
{
	std::cout << "\nWelcome to Bulls and Cows, funny guess game!" << std::endl;
	std::cout << std::endl;
	std::cout << "          }   {         ___ " << std::endl;
	std::cout << "          (o o)        (o o) " << std::endl;
	std::cout << "   /-------\\ /          \\ /-------\\ " << std::endl;
	std::cout << "  / | BULL |O            O| COW  | \\ " << std::endl;
	std::cout << " *  |-,--- |              |------|  * " << std::endl;
	std::cout << "    ^      ^              ^      ^ " << std::endl;
	std::cout << "Could you guess the word with " << BCGame.GetHiddenWordLength() << " letters that I'm thinking of?" << std::endl;
	return;
}

/*Неординарая функция, включающая в себя много особенностей*/
void PlayGame() 
{
	/*Подробное описание функции в FBullCowGame.cpp
	Скажу лишь, что тут она служит инициализацией всех нужных переменных в ноль.
	Ну и скрытое слово предоставляет.*/
	BCGame.Reset();

	/*Перераспределение значений из метода в переменную*/
	int32 MaxTries = BCGame.GetMaxTries();

	/*Цикл отвечающий за вычисление победы/проигрыша
	Работает примерно так:
	BCGame.IsGameWon() = true && BCGame.GetCurrentTry() <= MaxTries  |  это победа
	BCGame.IsGameWon() = false && BCGame.GetCurrentTry() > MaxTries  |  это проигрыш
	*/
	while (!BCGame.IsGameWon() && BCGame.GetCurrentTry() <= MaxTries)  {
		/*С самого начала программа вызовом метода GetValidGuess() просит ввести значение пользователя
		Затем, передаёт введённое им значение в переменную Guess*/
		FText Guess = GetValidGuess();
		
		/*Тут немного посложнее:
		1. Вызов метода SubmitValidGuess() сопутствуется передачей содержимого Guess на обработку,
		т.е. на проверку корректности в соответствии с количеством букв, их повторением и т.д.
		2. Метод SubmitValidGuess() тут же что-то возвращает? Ловко. Возвращенное им значением как ни странно имеет
		одну и ту же классификацию с принимаемым значением переменной BullCowCount. Вообщем говоря, класс FBullCowCount 
		это на самом деле структура из двух переменных (struct). Другими словами, он тут же поглощает в себя 
		слово написанное пользователем, считает сколько быков и коров он заслужил и выплевывает ему значение состоящие из двух
		целочисленных переменных, в лице тех же быков и коров. А мы в свою очередь передаем эти значения в переменную BullCowCount, которая,
		еще раз напомню, тоже может содержать до нескольких значений в себе.*/
		FBullCowCount BullCowCount = BCGame.SubmitValidGuess(Guess);

		/*У struct мы тоже может вызывать методы. Вот таким вот образом мы вытаскиваем значения для быков и коров.*/
		std::cout << "Your bulls: " << BullCowCount.Bulls;
		std::cout << ".  Your cows: " << BullCowCount.Cows << std::endl;
	}
}

/*Вишенка на торте. Выявление ошибок.*/
FText GetValidGuess() 
{
	/*Так, как мы отвечаем головой за то, что будет содержаться в переменной Guess,
	мы сначала инициализируем её нулём, чтобы не было мусора при дальнейшей работе с ней.*/
	FText Guess = "";

	/*И сразу перепрыгиваем на другой пунктик. Ну и собственно, логично, потому что функция называется GetValidGuess(), и
	кроме того, чтобы служить инпутом для пользователя должна ещё выводить ошибки и предупреждать пользователя об их существовании.
	Далее смотреть FBullCowGame.cpp для более подробного описания EGuessStatus. Тут же просто инициализация переменной Status
	стаковой константой Invalid, которая нигде собственно не учавствует.*/
	EGuessStatus Status = EGuessStatus::Invalid;

	/*Что-ж, первое что приходит в голову, почему же do-while цикл, вместо безобидных while или кучи if стейтментов?
	Ответ как никогда очевиден: do-while цилк как минимум раз прогоняет через себя всё что находится в области видимости цикла
	do {область видимости, да, я тут}. А что мы там имеем? Собственно сам инпут для пользователя. Было бы нелогично, если
	бы ты заходил в игру, а там пусто... Что-ж, это конечно не аргумент и сделать это можно было с любым другим циклом, да и без цикла легко
	создать инпут для пользователя, но фишка в том что стейтмент в параметре while(стейтмент, да, я тут) подразумевает то,
	что программа продолжит запрашивать инпут для слова пока ты его не угадаешь или вообще проиграешь. Профит!*/
	do {
		// Принимаем значения от пользователя и показываем количество попыток в формате: нынешняя попытка/всего попыток
		std::cout << "\nTry " << BCGame.GetCurrentTry()<< " of " << BCGame.GetMaxTries();
		std::cout << ". to guess the word: ";
		std::getline(std::cin, Guess);

		/*Принимаем в Status значение, которое возвращает CheckGuessValidity(Guess). То есть, тип ошибки из списка контстан*/
		Status = BCGame.CheckGuessValidity(Guess);

		/*В зависимости от типа ошибки выдаём соответствующий месседж пользователю. Очень пригодился switch.*/
		switch (Status) {
		case EGuessStatus::Wrong_Length:
			std::cout << "Please enter a " << BCGame.GetHiddenWordLength() << " letter word\n" << std::endl;
			break;
		case EGuessStatus::Not_Lowercase:
			std::cout << "Please make sure your letters are lowercase!\n" << std::endl;
			break;
		case EGuessStatus::Not_Isogram:
			std::cout << "Please enter a word without repeating letters!\n" << std::endl;
			break;
		default:
			break;
		} 
	} while (Status != EGuessStatus::OK); // Дополнение к цели while: ПРОДОЛЖАЕМ ЛУПИТЬ ПОКА НЕТ ОШИБОК!

	/*Мы поработали с вашим словом, может забирать обратно.*/
	return Guess;
}

/*Хочешь сыграть снова?*/
bool AskToPlayAgain() {
	std::cout << "\nDo you want to play again with the same hidden word? (y/n)" ;
	FText Response = "";
	std::getline(std::cin, Response);
	return (Response[0] == 'Y') || (Response[0] == 'y');
}

/*Результат игры. Работает максимально примитивно.*/
void PrintGameSummary() {
	if (BCGame.IsGameWon()) {
		std::cout << "WELL DONE - You have won!" << std::endl;
	}
	else {
		std::cout << "TRY AGAIN - You have lost!" << std::endl;
	}
}

