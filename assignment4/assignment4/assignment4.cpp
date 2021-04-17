#define _CRT_SECURE_NO_WARNINGS
#include<bangtal.h>
#include<stdio.h>
#include<stdlib.h>
#include<time.h>

SceneID scene1, scene2, scene3, scene4, scene5, scene6, scene7, scene8, scene9, scene10, scene11;
ObjectID startbutton, endbutton, continuebutton, lockpad, reddot, greendot, ansbutton, exitbutton, quiz1, quiz2, quizback1, quizback2, goback, end, quit;
ObjectID door1, bearopen, bearclose, key, safeclose, safeopen, doorclose, dooropen, watch, hint_board, hintpaper, hint, keypad, run, ship, person;
ObjectID net[3], trap[3], getin, movingship, mountain[3], rock[3], shark[3], swirl[3], endingbutton;
bool locked1 = true;    // lockpad
bool closed = true;     // safe 
bool locked2 = true;	//door

int personX = 50, personY = 600;
int shipX = 20, shipY = 330;
int dx = 0, dy = 0, ddx = 0, ddy = 0;
int personmove_step = 5;  //사람 움직이는 정도
int shipmove_step = 5;    // 배 움직이는 정도
double ANIMATION_TIME = 0.04f;
int trapX[3] = { 500, 350, 120 }, trapY[3] = { 500, 250, 300 };
int netX[3] = { 80, 500, 500 }, netY[3] = { 100, 350, 600 };
int mountainX[3] = { 200, 400, 750 }, mountainY[3] = { 50, 500, 300 };
int rockX[3] = { 100, 580, 600 }, rockY[3] = { 200, 150, 600 };
int sharkX[3] = { 150, 500, 850 }, sharkY[3] = { 550, 300, 100 };
int swirlX[3] = { 220, 400, 750 }, swirlY[3] = { 350, 120, 450 };

ObjectID createObject(const char* image, SceneID scene, int x, int y, bool shown = true)
{
	ObjectID object = createObject(image);
	locateObject(object, scene, x, y);
	if (shown)
	{
		showObject(object);
	}
	return object;
}

void gameover()  // 사람이 배로 갈 때 그물에 걸리면/ 함정에 걸리면 게임 오버      
{
	for (int i = 0; i < 3; i++)
	{
		if (personX > trapX[i] - 30 && personX < trapX[i] + 90 && personY > trapY[i] - 70 && personY < trapY[i] + 90)
		{
			enterScene(scene10);
		}
		else if (personX > netX[i] - 30 && personX < netX[i] + 80 && personY > netY[i] - 70 && personY < netY[i] + 80)
		{
			enterScene(scene10);
		}
	}

}

void shipin()  //배에 탑승
{
	if (personX > 820 && personX < 945 && personY > 30 && personY < 300)
	{
		showObject(getin);
	}
}

void gamefin()  //게임 성공!!
{
	if (shipX > 1100 && shipY > 400 && shipY < 600)
	{
		enterScene(scene11);
	}
}

void shipwreck()  //배가 장애물에 부딪히면 게임오버
{
	for (int i = 0; i < 3; i++)
	{
		if(shipX > mountainX[i] - 40 && shipX < mountainX[i] + 80 && shipY > mountainY[i] - 40 && shipY < mountainY[i] + 70)
		{
			enterScene(scene9);
		}
		else if (shipX > rockX[i] - 40 && shipX < rockX[i] + 80 && shipY > rockY[i] - 40 && shipY < rockY[i] + 70)
		{
			enterScene(scene9);
		}
		else if (shipX > sharkX[i] - 40 && shipX < sharkX[i] + 80 && shipY > sharkY[i] - 40 && shipY < sharkY[i] + 70)
		{
			enterScene(scene9);
		}
	}
	for (int j = 0; j < 3; j++)
	{
		if (shipX > swirlX[j] - 40 && shipX < swirlX[j] + 90 && shipY > swirlY[j] - 40 && shipY < swirlY[j] + 90) 
		{
			enterScene(scene9);	
		}
	}
}


void mouseCallback(ObjectID object, int x, int y, MouseAction action)
{
	if (object == startbutton)
	{
		enterScene(scene2); //설명화면
	}
	else if (object == endbutton)
	{
		endGame();
	}
	else if (object == continuebutton)
	{
		enterScene(scene3); //감옥 화면
	}
	else if (object == lockpad)
	{
		if (locked1 == true)
		{
			enterScene(scene4); //pad미션 화면
		}
		else
		{
			enterScene(scene5);
			showMessage("문을 열고 지하 감옥을 탈출하자.");
		}
	}
	else if (object == goback)
	{
		enterScene(scene3);
	}
	else if (object == exitbutton) //게임종료
	{
		endGame();
	}
	else if (object == quizback1)
	{
		hideObject(quizback1);
		showObject(quiz1);
	}
	else if (object == quizback2)
	{
		hideObject(quizback2);
		showObject(quiz2);
	}
	else if (object == ansbutton)  // pad 잠금해제 미션 - 퀴즈 풀기
	{
		showKeypad("53n", lockpad);
	}
	else if (object == door1)
	{
		enterScene(scene6);
	}
	else if (object == bearclose)  //곰 얼굴 바뀜
	{
		hideObject(bearclose);
		showObject(bearopen);
		showObject(key);
	}
	else if (object == bearopen)  
	{
		hideObject(bearopen);
		hideObject(key);
		showObject(bearclose);
	}
	else if (object == key) 
	{
		pickObject(key);
	}
	else if (object == safeclose)   // 열쇠로 금고 잠금해제 
	{
		if (closed)
		{
			if (key == getHandObject()) 
			{
				hideObject(safeclose);
				showObject(safeopen);
				showObject(hint);
				closed = false;
			}
			else
			{
				showMessage("굳게 잠겨있다. 열쇠가 필요하다.");
			}
		}
	}

	else if (object == hint)
	{
		hideObject(hint);
		pickObject(hintpaper);
	}
	else if (object == hintpaper)
	{
		pickObject(hintpaper);
	}
	else if (object == keypad)
	{
		showKeypad("9895", doorclose);
	}

	else if (object == doorclose)   // 키패드를 통해서 문 잠금해제
	{
		if (locked2 == true)
		{
			showMessage("문이 굳게 잠겨있다. 암호를 입력하면 열릴 것 같다.");
		}
		else
		{
			hideObject(doorclose);
			showObject(dooropen);
			showMessage("문이 열렸다!!!");
		}
	}
	else if (object == dooropen)  //잠금해제 되면서 열린 문으로 바뀜
	{
		enterScene(scene7);
		showMessage("배가 보인다. 도망가기 버튼을 클리하고, 키보드의 W,A,S,D 버튼을 사용해서 배로 이동하자. 이때 함정을 조심하자");
	}
	
	else if (object == run)
	{
		hideObject(run);
		showObject(person);
		TimerID timer = createTimer(ANIMATION_TIME);  
		startTimer(timer);
	}
	else if (object == getin)
	{
		enterScene(scene8);
	}
	else if (object == end) // 게임오버장면
	{
		endGame();
	}

	else if (object == endingbutton)
	{
		endGame();
	}
	else if (object == quit)
	{
		endGame();
	}
}

void objectCallback(ObjectID object, EventID event)
{
	if (object == lockpad)
	{
		if (event == EventID::EVENT_KEYPAD)
		{
			locked1 = false;
			showMessage("잠금해제!!!");
			hideObject(reddot);
			showObject(greendot);
		}
	}

	else if (object == doorclose)
	{
		if (event == EventID::EVENT_KEYPAD) 
		{
			locked2 = false;
			showMessage("잠금 해제!!!");	
		}
	}
}

void keyboardCallback(KeyCode code, KeyState state)
{
	if (code == KeyCode::KEY_W) //person up
	{
		dy += (state == KeyState::KEY_PRESSED ? personmove_step : -personmove_step);
	}
	else if (code == KeyCode::KEY_S)  //person down
	{
		dy -= (state == KeyState::KEY_PRESSED ? personmove_step : -personmove_step);
	}
	else if (code == KeyCode::KEY_D)  //person right
	{
		dx += (state == KeyState::KEY_PRESSED ? personmove_step : -personmove_step);
	}
	else if (code == KeyCode::KEY_A)  //person left
	{
		dx -= (state == KeyState::KEY_PRESSED ? personmove_step : -personmove_step);
	}

	else if (code == KeyCode::KEY_UP_ARROW)  //ship up
	{		
		ddy += (state == KeyState::KEY_PRESSED ? shipmove_step : -shipmove_step);
	}
	else if (code == KeyCode::KEY_DOWN_ARROW)  //ship down
	{		
		ddy -= (state == KeyState::KEY_PRESSED ? shipmove_step : -shipmove_step);
	}
	else if (code == KeyCode::KEY_RIGHT_ARROW)  //ship right
	{		
		ddx += (state == KeyState::KEY_PRESSED ? shipmove_step : -shipmove_step);
	}
	else if (code == KeyCode::KEY_LEFT_ARROW)  //ship left
	{		
		ddx -= (state == KeyState::KEY_PRESSED ? shipmove_step : -shipmove_step);
	}
}

void timerCallback(TimerID timer) //사람 움직이는 함수 관련 타이머
{
	setTimer(timer, ANIMATION_TIME);
	startTimer(timer);

	personX += dx; personY += dy;
	locateObject(person, scene7, personX, personY);
	gameover();
	shipin();

	shipX += ddx; shipY += ddy;
	locateObject(movingship, scene8, shipX, shipY);
	gamefin();
	shipwreck();
}

SceneID gamestart()
{
	scene1 = createScene("startscene", "startscene.png");
	scene2 = createScene("story", "explain.png");
	scene3 = createScene("prison", "prison.png");
	scene4 = createScene("pad", "scene4.png");
	scene5 = createScene("stair", "stair.png");
	scene6 = createScene("room", "room.png");
	scene7 = createScene("beach", "beach.png");
	scene8 = createScene("ocean", "ocean.png");
	scene9 = createScene("wrecked", "wreck.png");
	scene10 = createScene("gameover", "over.png");
	scene11 = createScene("succes", "fin.png");

	startbutton = createObject("startbutton.png", scene1, 1000, 150, true);
	endbutton = createObject("endbutton.png", scene1, 1000, 80, true);

	continuebutton = createObject("continuebutton.png", scene2, 1050, 100, true);

	lockpad = createObject("pad.png", scene3, 500, 250, true);
	reddot = createObject("reddot.png", scene3, 600, 320, true);
	greendot = createObject("greendot.png", scene3, 600, 280, false); // pad미션 성공시 true

	exitbutton = createObject("exit.png", scene4, 1050, 180, true);
	ansbutton = createObject("answer.png", scene4, 1050, 300, true);
	goback = createObject("goback.png", scene4, 1050, 420, true);
	quiz1 = createObject("quiz1.png", scene4, 100, 100, false);
	quizback1 = createObject("quiz1back.png", scene4, 100, 100, true);
	quiz2 = createObject("quiz2.png", scene4, 550, 100, false);
	quizback2 = createObject("quiz1back.png", scene4, 550, 100, true);

	door1 = createObject("door1.png", scene5, 650, 470, true);

	bearopen = createObject("bearopen.png", scene6, 870, 520, false); // 클릭하면 보임
	bearclose = createObject("bearclose.png", scene6, 870, 520, true);
	key = createObject("key.png", scene6, 910, 535, false);
	safeclose = createObject("safeclose.png", scene6, 800, 50, true);
	safeopen = createObject("safeopen.png", scene6, 795, 40, false);
	doorclose = createObject("doorclose.png", scene6, 235, 210, true);
	dooropen = createObject("dooropen.png", scene6, 235, 210, false);   // 키패드 비밀번호 입력 후 정답이면 보임
	watch = createObject("clock.png", scene6, 50, 500, true);
	hint_board = createObject("hint_board.png", scene6, 420, 400, true);
	hintpaper = createObject("hintpaper.png", scene6, 800, 100, false);
	hint = createObject("hintpaper_small.png", scene6, 855, 153, false);
	keypad = createObject("keypad.png", scene6, 350, 370, true);

	ship = createObject("ship.png", scene7, 880, 235, true);
	run = createObject("run.png", scene7, 50, 600, true);
	person = createObject("person.png", scene7, personX, personY, false);
	getin = createObject("sail.png", scene7, 900, 250, false);
	net[0] = createObject("net.png", scene7, netX[0], netY[0], true);
	net[1] = createObject("net.png", scene7, netX[1], netY[1], true);
	net[2] = createObject("net.png", scene7, netX[2], netY[2], true);
	trap[0] = createObject("trap.png", scene7, trapX[0], trapY[0], true);
	trap[1] = createObject("trap.png", scene7, trapX[1], trapY[1], true);
	trap[2] = createObject("trap.png", scene7, trapX[2], trapY[2], true);

	movingship = createObject("miniship.png", scene8, shipX, shipY, true);
	mountain[0] = createObject("mountain.png", scene8, mountainX[0], mountainY[0], true);
	mountain[1] = createObject("mountain.png", scene8, mountainX[1], mountainY[1], true);
	mountain[2] = createObject("mountain.png", scene8, mountainX[2], mountainY[2], true);
	rock[0] = createObject("rock.png", scene8, rockX[0], rockY[0], true);
	rock[1] = createObject("rock.png", scene8, rockX[1], rockY[1], true);
	rock[2] = createObject("rock.png", scene8, rockX[2], rockY[2], true);
	shark[0] = createObject("shark.png", scene8, sharkX[0], sharkY[0], true);
	shark[1] = createObject("shark.png", scene8, sharkX[1], sharkY[1], true);
	shark[2] = createObject("shark.png", scene8, sharkX[2], sharkY[2], true);
	swirl[0] = createObject("swirl.png", scene8, swirlX[0], swirlY[0], true);
	swirl[1] = createObject("swirl.png", scene8, swirlX[1], swirlY[1], true);
	swirl[1] = createObject("swirl.png", scene8, swirlX[2], swirlY[2], true);

	quit = createObject("end.png", scene9, 900, 300, true);
	end = createObject("end.png", scene10, 900, 300, true);
	endingbutton = createObject("ending.png", scene11, 900, 200, true);

	return scene1;
}

int main()
{	
	srand(time(NULL));

	setMouseCallback(mouseCallback);
	setObjectCallback(objectCallback);
	setKeyboardCallback(keyboardCallback);
	setTimerCallback((timerCallback));
		
		
	startGame(gamestart());
}