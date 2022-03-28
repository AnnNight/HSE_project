#include <iostream>
#include <chrono>
#include <random>
#include <SFML/Graphics.hpp>
#include "Player.h"
#include "Map.h"
#include "Bullet.h"
#include "Audio.h"
#include "Enemy_tanks.h"
using namespace sf;
bool menu (RenderWindow &window){
    Texture menuTexture1, menuTexture2, menuTexture3;
    Sprite menuSprite1, menuSprite2, menuSprite3;
    Image menuImage1,menuImage2,menuImage3;

    menuImage1.loadFromFile("..\\images/sprite.bmp");
    menuImage2.loadFromFile("..\\images/sprite.bmp");
    menuImage3.loadFromFile("..\\images/sprite.bmp");

    menuTexture1.loadFromImage(menuImage1);
    menuTexture2.loadFromImage(menuImage2);
    menuTexture3.loadFromImage(menuImage3);

    menuSprite1.setTexture(menuTexture1);
    menuSprite1.setTextureRect(IntRect(135, 274, 374, 137));
    menuSprite1.setPosition(30,50);
    menuSprite2.setTexture(menuTexture2);
    menuSprite2.setTextureRect(IntRect(354, 423, 62, 14));
    menuSprite2.setPosition(193,230);
    menuSprite3.setTexture(menuTexture3);
    menuSprite3.setTextureRect(IntRect(1, 35, 26,26));
    menuSprite3.setPosition(165,226);

    bool IsMenu = true;
    bool IsPlay=false;

    while(IsMenu){
        IsPlay=false;

        if(IntRect(193,230,62, 14).contains(Mouse::getPosition(window))){
            window.draw(menuSprite3);
            IsPlay=true;
        }

        if (Mouse::isButtonPressed(Mouse::Left)) {
            if (IsPlay) {
                IsMenu = false;
            }
        }
        Event event;
        while (window.pollEvent(event)) {
            if (event.type == Event::Closed) {
                window.close();
                return false;}
        }

        window.draw(menuSprite1);
        window.draw(menuSprite2);

        window.display();
        window.clear();
    }
    return true;
}

int main() {

    RenderWindow window(VideoMode(544, 480), "Tan4iki!");
    if(!menu(window)){
        return 0;
    }
    Map map("Background.png");
    map.SetNumberMap(1);
    Player tank("sprite.bmp", 20, 380, 26, 26);
    ///music

    Audio audio;
    audio.Init();
    audio.playGame();

    int Dir = 0;

    bool NewBullet = false;
    bool BigFlag = false;
    float CurrentFrame = 0;//хранит текущий кадр

    Clock clock;


    int n_bul = 1;
    Bullet bul[n_bul];
    for (int i = 0; i < n_bul; i++) {
        bul[i].SetFile("heart.bmp");
    }

    int n_enemies = 3; // Количество танков, позже должно увеличиться до 10(?)
    int alive_enemies = n_enemies; //Маша просила переменную
    Enemy_tank t[n_enemies];  //Создаем массив вражеских танков
    Bullet enemy_bul[n_enemies];  //Создаем массив вражеских пуль с расчетом 1 пуля на 1 танк
    for (int i=0;i<n_enemies;i++){  // Установка спрайтов
        t[i].SetFile("sprite.bmp");
        enemy_bul[i].SetFile("heart.bmp");
    }
    t[0].Start_Enemy_Function(t[0],t[1],t[2]); //Стартовая функция. Я планировала, что танки будут появляться на 3 местах, когда один из них умрет и
    //соответственно на это же место будет вставать новый танк, но, видимо, надо делать иначе.
    // Скорее всего эту функцию надо переписать, чтобы она была доступной для всех вражеских танков


    std::uniform_int_distribution<int> dist(0, 4); // Левая и правая граница рандома
    int STATE =1;

    while (window.isOpen()) {
        std::mt19937 engine(std::chrono::steady_clock::now().time_since_epoch().count()); //для рандома
        float time = clock.getElapsedTime().asMicroseconds();
        clock.restart();
        time = time / 800;
        // Обрабатываем очередь событий в цикле
        Event event;

        while (window.pollEvent(event)) {
            if (event.type == Event::Closed) window.close();}

        if (Keyboard::isKeyPressed(Keyboard::Left) || (Keyboard::isKeyPressed(Keyboard::A))) { tank.SetDir(1); tank.SetSpeed(0.1); tank.setRect();}
        if (Keyboard::isKeyPressed(Keyboard::Right) || (Keyboard::isKeyPressed(Keyboard::D))) { tank.SetDir(0);tank.SetSpeed(0.1);tank.setRect();}
        if (Keyboard::isKeyPressed(Keyboard::Up) || (Keyboard::isKeyPressed(Keyboard::W))) { tank.SetDir(3); tank.SetSpeed(0.1); tank.setRect(); }
        if (Keyboard::isKeyPressed(Keyboard::Down) || (Keyboard::isKeyPressed(Keyboard::S))) { tank.SetDir(2); tank.SetSpeed(0.1); tank.setRect();}
        if ((Keyboard::isKeyPressed(Keyboard::Space))) { NewBullet = true; }
        ///////////////////////////////////////////

        if (NewBullet) {
            for (int i = 0; i < n_bul; i++) {  // Добавление новой пули
                if (!bul[i].Is_On_f) {
                    bul[i].Is_On_f = true;
                    audio.playShoot();
                    bul[i].New_Coordinates_and_Dir(tank);
                    break;
                }
            }
            NewBullet = false;
        }

        tank.update(time);
        map.InteractionTankWithMap(map.GetDiagramMap(), tank);
        for (int i=0;i<n_enemies;i++) {
            //if(t[i].GetIsOnTheField()) { //Для ситуации когда танков всего больше, чем на поле
            map.InteractionEnemyTankWithMap(map.GetDiagramMap(), t[i]);
            // }
        }
        window.clear();

/////////////////////////////Рисуем карту/////////////////////
        for (int i = 0; i < HEIGHT_MAP; i++)
            for (int j = 0; j < WIDTH_MAP; j++) {
                map.CreateMap(map.GetDiagramMap(), i, j);
                window.draw(map.GetMapSprite());//рисуем квадратики на экран
            }
        // window.draw(map.GetMapSprite());
        for (int i = 0; i < n_bul; i++) {
            if (bul[i].Is_On_f) {
                bul[i].update(time);
                bul[i].Is_On_f = map.InteractionBulletWithMap(map.GetDiagramMap(),bul[i]);
                window.draw(bul[i].sprite);//рисуем спрайт пули
            }
        }
        for (int i=0;i<n_enemies;i++){   //Общий цикл врагов
            //if(t[i].GetIsAlive() && t[i].GetIsOnTheField()){  // Возможно пригодится для добавления новых танков
            if(t[i].GetFlag_to_change()){      //Если флаг сигнализирует о том, что надо поменять направление
                t[i].UpdateDir(time, engine);    // меняем направление
                t[i].SetFlag_to_change(false);  //Опускаем флаг
            }
            if(!enemy_bul[i].Is_On_f) {     //Если пуля врага была не на поле
                enemy_bul[i].Is_On_f = true;   // Сделать ее на поле
                enemy_bul[i].New_Coordinates_and_Dir(t[i]); // Установить ей координаты и направление
            }
            enemy_bul[i].update(time);   //Обновляем по времени
            enemy_bul[i].Is_On_f = map.InteractionBulletWithMap(map.GetDiagramMap(), enemy_bul[i]); //Проверяем не попала ли куда-нибудь пуля
            window.draw(enemy_bul[i].sprite); //Рисуем пулю
            t[i].update(time);
            if(t[i].GetIsOnTheField()){  //Если пуля на поле, то устанавливаем ей скорость
                t[i].SetSpeed(0.05);
            }
            window.draw(t[i].GetSprite());
            // }
        }
        window.draw(tank.GetSprite());
        window.display();
    }
    return 0;
}