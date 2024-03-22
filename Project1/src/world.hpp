#pragma once

#include <SFML/Window.hpp>
#include <SFML/Graphics.hpp>
#include <iostream>
#include <cmath>
#include <algorithm>
#include "player_states.hpp"
#include "player.hpp"


class Player;

struct CircleObject;


class World
{
public:

    void addBlock(sf::FloatRect block)
    {
        mBlocks.push_back(block);
    }

    void setView()
    {
        sf::Vector2f playerCenter = mPlayer.getCenter();
        float mViewRatio = 0.6;
        if (playerCenter.x > mView.getCenter().x + mViewRatio * mView.getSize().x / 2)
            mView.move({playerCenter.x - mView.getCenter().x - mViewRatio * mView.getSize().x / 2, 0});

        else if (playerCenter.x < mView.getCenter().x - mViewRatio * mView.getSize().x / 2)
            mView.move({playerCenter.x - mView.getCenter().x + mViewRatio * mView.getSize().x / 2, 0});


        if (playerCenter.y > mView.getCenter().y + mViewRatio * mView.getSize().y / 2)
            mView.move({0, playerCenter.y - mView.getCenter().y - mViewRatio * mView.getSize().y / 2});

        else if (playerCenter.y < mView.getCenter().y - mViewRatio * mView.getSize().y / 2)
            mView.move({0, playerCenter.y - mView.getCenter().y+ mViewRatio * mView.getSize().y / 2});

    }

    void update(float dt)
    {   
        setView();
        mPlayer.applyVelocity({0, mGravity * dt});
        mPlayer.update(dt);
        mPlayer.handleAllCollisions(mBlocks, mCircleObjects);
        playerAttack();
    }

    void draw(sf::RenderWindow& window)
    {
        static sf::RectangleShape blockShape;
        blockShape.setFillColor(sf::Color(58, 69, 55));

        window.setView(mView);

        for (const sf::FloatRect& b : mBlocks)
        {
            blockShape.setPosition(b.left, b.top);
            blockShape.setSize({b.width, b.height});
            window.draw(blockShape);
        }
        drawCircleObjects(window);
        mPlayer.draw(window);
    }

    void addCircleObject(float X, float Y, float radius)
    {
        sf::Vector2f center(X, Y);
        mCircleObjects.push_back(CircleObject(center, radius));
    }

    void drawCircleObjects(sf::RenderWindow& window) {
         if (mPlayer.isPlayerAttacking()) {
            CircleObject playerCircle = mPlayer.getPlayerCircle();
            sf::CircleShape playerCircleShape(playerCircle.radius);
            playerCircleShape.setOrigin(playerCircle.radius, playerCircle.radius);
            playerCircleShape.setPosition(playerCircle.center);
            playerCircleShape.setFillColor(sf::Color(15, 15, 15));
            //playerCircleShape.setFillColor(sf::Color(50, 50, 50));
            window.draw(playerCircleShape);
        }

        for (const auto& circle : mCircleObjects) {
            sf::CircleShape circleShape(circle.radius);
            circleShape.setOrigin(circle.radius, circle.radius);
            circleShape.setPosition(circle.center);
            circleShape.setFillColor(circle.color);
            window.draw(circleShape);
        }
            // Отрисовка круга удара игрока
       
    }
    void handleEvents(const sf::Event& event)
    {
        mPlayer.handleEvents(event);
    }

    // Проверка коллизии с круглым объектом
    bool isCircleColliding(const CircleObject& circleObject)
    {
        for (const auto& circle : mCircleObjects)
        {
            float distance = std::sqrt(std::pow(circleObject.center.x - circle.center.x, 2) +
                                       std::pow(circleObject.center.y - circle.center.y, 2));
            if (distance < circleObject.radius + circle.radius)
            {
                return true;
            }
        }
        return false;
    }

    bool checkCircleCollision(const CircleObject& circleObject, const CircleObject& circle)
    {
        float distance = std::sqrt(std::pow(circleObject.center.x - circle.center.x, 2) +
                                    std::pow(circleObject.center.y - circle.center.y, 2));
        if (distance < circleObject.radius + circle.radius)
        {
            return true;
        }
        return false;
    }

    void destroyObject(CircleObject& circle) {
        auto it = std::remove_if(mCircleObjects.begin(), mCircleObjects.end(),
                                [&](const CircleObject& c) {
                                    return &c == &circle;
                                });
        mCircleObjects.erase(it, mCircleObjects.end());
    }

    void updateCircleShape(const CircleObject& circle, float firstState, float secondState) {
        for (auto& circleObject : mCircleObjects) {
            if (circleObject.center == circle.center) {
                if (circleObject.hitCount > secondState) {
                    circleObject.color = sf::Color::Red;
                } else if (circleObject.hitCount > firstState) {
                    circleObject.color = sf::Color(255, 165, 0);
                } else {
                    circleObject.color = circle.color;
                }
                break;
            }
        }
    }

    void playerAttack()
    {
        float firstState = 15;
        float secondState = 50;
        float thirdState = 75;
        CircleObject playerCircle = mPlayer.getPlayerCircle();
        if (mPlayer.isPlayerAttacking()) {
            for (auto& circle : mCircleObjects) {
                if (checkCircleCollision(playerCircle, circle)) { 
                    circle.hitCount++;
                    if (circle.hitCount >= thirdState) {
                        destroyObject(circle);
                    }
                    else if (circle.hitCount > secondState) {
                        circle.color = sf::Color::Red;
                    } else if (circle.hitCount > firstState) {
                        circle.color = sf::Color(255, 165, 0);
                    }
                }
            }
            // После обработки всех кругов, обновляем их цвета
            for (auto& circle : mCircleObjects) {
                updateCircleShape(circle, firstState, secondState);
            }
        }
    }


private:
    std::vector<sf::FloatRect> mBlocks  {};
    std::vector<CircleObject> mCircleObjects {};
    Player mPlayer                      {{400, 400}};
    float mGravity                      {3600};

    sf::View mView                      {sf::FloatRect(0, 0, 1200, 900)};
    //std::vector<sf::CircleShape> mCircleShapes; // Вектор для хранения отображений круглых объектов
    sf::Color circleColor = sf::Color::Green; // Цвет по умолчанию
};
