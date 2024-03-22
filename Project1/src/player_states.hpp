#pragma once

#include <SFML/Window.hpp>
#include <SFML/Graphics.hpp>
#include "animation.hpp"
#include "player.hpp"



class Player;

struct CircleObject {
    sf::Vector2f center;
    float radius;
    int hitCount;
    sf::Color color;
    CircleObject(sf::Vector2f center, float radius) : center(center), radius(radius), hitCount(0), color(sf::Color::Green) {}
};
class PlayerState
{
public:
    PlayerState();
    
    virtual void update(Player* player, float dt) = 0;
    virtual void handleEvents(Player* player, const sf::Event& event) = 0;
    virtual void hook(Player* player) = 0;
    virtual void startFalling(Player* player) = 0;
    virtual void hitGround(Player* player) = 0;
    virtual ~PlayerState();
    virtual bool isAttacking() const {
        return false;
    }

    virtual std::vector<float> getCircleParameters() const{
        return {0, 0, 0};
    }


    void updateSprite(sf::Sprite& sprite, bool isFacedRight, float scaleFactor);

protected:
    Animation mAnimation;

    static constexpr float kJumpingVelocity = 1500;
    static constexpr float kDoubleJumpingVelocity = 1000;
    void jump(Player* player);
    void double_jump(Player* player);

    
};


class Idle : public PlayerState
{
public:
    Idle(Player* player);
    
    void update(Player* player, float dt);
    void handleEvents(Player* player, const sf::Event& event);
    void hook(Player* player);
    void startFalling(Player* player);
    void hitGround(Player* player);

private:
};

class Sitting : public PlayerState
{
public:
    Sitting(Player* player);

    void update(Player* player, float dt);
    void handleEvents(Player* player, const sf::Event& event);
    void hook(Player* player);
    void startFalling(Player* player);
    void hitGround(Player* player);
};


class Running : public PlayerState
{
public:
    Running(Player* player);
    void update(Player* player, float dt);
    void handleEvents(Player* player, const sf::Event& event);
    void hook(Player* player);
    void startFalling(Player* player);
    void hitGround(Player* player);

private:
    float mRunningSpeed;
};

class FirstAttack : public PlayerState
{
public:
    FirstAttack(Player* player);

    void update(Player* player, float dt);
    void handleEvents(Player* player, const sf::Event& event);
    void hook(Player* player);
    void startFalling(Player* player);
    void hitGround(Player* player);
    bool isAttacking() const override {
        return true;
    }
    virtual std::vector<float> getCircleParameters() const;


private:
    static constexpr float kAcceleration = 1000;
};

class SecondAttack : public PlayerState
{
public:
    SecondAttack(Player* player);

    void update(Player* player, float dt);
    void handleEvents(Player* player, const sf::Event& event);
    void hook(Player* player);
    void startFalling(Player* player);
    void hitGround(Player* player);
    bool isAttacking() const override {
        return true;
    }
    virtual std::vector<float> getCircleParameters() const;
private:
    static constexpr float kAcceleration = 1500;
    
};

class ThirdAttack : public PlayerState
{
public:
    ThirdAttack(Player* player);

    void update(Player* player, float dt);
    void handleEvents(Player* player, const sf::Event& event);
    void hook(Player* player);
    void startFalling(Player* player);
    void hitGround(Player* player);
    bool isAttacking() const override {
        return true;
    }
    virtual std::vector<float> getCircleParameters() const;
private:
    static constexpr float kAcceleration = 2000;
};


class Sliding : public PlayerState
{

public:

    Sliding(Player* player);
    void update(Player* player, float dt);
    void handleEvents(Player* player, const sf::Event& event);
    void hook(Player* player);
    void startFalling(Player* player);
    void hitGround(Player* player);

private:

    float mCurrentTime;
    static constexpr float kSlidingTime = 0.50;
    static constexpr float kSlidingVelocity = 2000;
    static constexpr float kVelocityDecay = 0.99;
};


class Falling : public PlayerState
{
public:
    Falling(Player* player);
    void update(Player* player, float dt);
    void handleEvents(Player* player, const sf::Event& event);
    void hook(Player* player);
    void startFalling(Player* player);
    void hitGround(Player* player);
    //void double_jump(Player* player);
    bool isDoubleJumped = false; // Добавляем переменную isDoubleJumped

private:

    static constexpr float kHorizontalVelocity = 800;
};



class Hooked : public PlayerState
{
public:

    static constexpr float kMaxHookOffset = 15;

    Hooked(Player* player);
    void update(Player* player, float dt);
    void handleEvents(Player* player, const sf::Event& event);
    void hook(Player* player);
    void startFalling(Player* player);
    void hitGround(Player* player);
};