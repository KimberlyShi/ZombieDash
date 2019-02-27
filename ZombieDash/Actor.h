#ifndef ACTOR_H_
#define ACTOR_H_

#include "GraphObject.h"
//TESTING TO SEE IF WILL SAVE NEW CHANGES
// Students:  Add code to this file, Actor.cpp, StudentWorld.h, and StudentWorld.cpp

//need to declare the StudentWorld
class StudentWorld;

//base class
class Actor: public GraphObject
{
public:
    Actor(StudentWorld *stud, double locX, double locY, int imgid, int statAlive, Direction dir, int depth, int size, bool canBlock, bool canBlockFlames);
    
    virtual void doSomething() = 0;
    virtual ~Actor();
    
    //make accessors to all the different ones
    int isAlive() const; //check status of alive
    StudentWorld *getStud() const;
    double getSpriteWidth() const;
    double getSpriteHeight() const;
    bool getCanBlock() const;
    bool finishedLevel() const; //true indicates that Penelope finished
    
    //for flames damage
    bool canBlockFlames() const; //only wall or exit can block flames, true if can block
    void setActivation(); //will be called if the mine can be activated by that actor
    bool canActivateMine() const; //only zombie, penelope, and citizen will return true
   virtual void setFlameCanDamage(bool val); //mutator
    bool flameCanDamage() const; //accessor
    
    //mutators
    virtual void setDead();
    void officiallyDead();
    void setfinishedLevelTrue();
    
    
    
    
private:
    StudentWorld *m_stud;
    int m_status; //2-alive 1-dead 0-not applicable ex. wall, exit
    bool m_canBlock; //true if the object can be blocked (penelope can't run into)
    bool m_levelStatus;
    bool m_canBlockFlames;
    bool m_flameCanDamage;
    bool m_canActivateMine;
};

//======PENLEOPE=============
class Penelope: public Actor
{
public:
    Penelope(StudentWorld *stud, double locX, double locY);
    
    virtual ~Penelope();
    virtual void doSomething();
    
    //accessor
    bool getInfectStat() const;
    int getInfectCount() const;
    int getVaccines() const;
    int getMines() const;
    int getFlames() const;

    
    //mutators
    void moveX(int posNeg);
    void moveY(int posNeg);
    bool isValid(double x, double y);
    void addFlames(int num);
    void addVaccines(int num);
    void addMines(int num);
    
     virtual void setDead();
    
    
private:
    bool m_infectStat;
    int m_infectCount;
    int m_vaccines;
    int m_mines;
    int m_flames;
    int keyVal;
    int posNeg;
    int m_dir;
    bool canBlock;
    
};

//=======WALL========================
class Wall: public Actor
{
public:
    Wall(StudentWorld * stud, double locX, double locY);
    virtual ~Wall();
    virtual void doSomething();
};

//===========EXIT==========
class Exit: public Actor
{
public:
    Exit(StudentWorld* stud, double locX, double locY);
    virtual ~Exit();
    virtual void doSomething();
};

//======GOODIES====
//classes derived from goodies include vaccine goodie, gas can goodie, vaccine goodie
class Goodies: public Actor
{
public:
    Goodies(StudentWorld* stud, double locX, double locY, int imgid);
    virtual ~Goodies();
    virtual void doSomething();
    void checkOverlap();
    

    
private:
    virtual void increaseGoodie() = 0;
    
};

class VaccineGoodie: public Goodies
{
public:
    VaccineGoodie(StudentWorld* stud, double locX, double locY);
    virtual ~VaccineGoodie();
    
private:
    virtual void increaseGoodie();
};

class GasCanGoodie: public Goodies
{
public:
    GasCanGoodie(StudentWorld* stud, double locX, double locY);
    virtual ~GasCanGoodie();
    
private:
    virtual void increaseGoodie();
};

class LandmineGoodie: public Goodies
{
public:
    LandmineGoodie(StudentWorld* stud, double locX, double locY);
    virtual ~LandmineGoodie();
    
private:
    virtual void increaseGoodie();
};


//Bad Things: aka pits, vomit, flames===================
class BadThings: public Actor
{
    //Actor(StudentWorld *stud, double locX, double locY, int imgid, int statAlive, Direction dir, int depth, int size, bool canBlock);
public:
    BadThings(StudentWorld *stud, double locX, double locY, int imgid, Direction dir);
    virtual void doSomething();
    virtual void startTick();
    int getStartTick() const;
private:
    int m_startTick;
};

class Landmines: public BadThings
{
public:
    Landmines(StudentWorld *stud, double locX, double locY);
    virtual void doSomething();
//        virtual void setDead();
    
    
private:
    bool m_inactiveState; //true if it's inactive
    int m_countdownTicks;
};

class Pit: public BadThings
{
public:
    Pit(StudentWorld *stud, double locX, double locY);
    virtual void doSomething();
    
};

class Vomit: public BadThings
{
public:
    Vomit(StudentWorld *stud, double locX, double locY, Direction dir);
    virtual void doSomething();
   
};

class Flames: public BadThings
{
public:
    Flames(StudentWorld *stud, double locX, double locY, Direction dir);
    virtual void doSomething();
};
//======ZOMBIES YAYYY===
//Actor(StudentWorld *stud, int locX, int locY, int imgid, int statAlive, Direction dir, int depth, int size, bool canBlock);
class Zombie: public Actor
{
public:
    Zombie(StudentWorld *stud, double locX, double locY);
    virtual ~Zombie();
    virtual void doSomething();
    double vomitX();
    double vomitY();
    //accessor
    int getPlanDistance() const;
    
    //mutator
    void setPlanDistance(int set);
     virtual void setDead();
    
private:
    int m_planDistance;
    virtual void movementPlan();
};

class DumbZombie: public Zombie
{
public:
     DumbZombie(StudentWorld *stud, double locX, double locY);
    virtual ~DumbZombie();
//     virtual void setDead();
    
private:
    virtual void movementPlan();
};

class SmartZombie: public Zombie
{
public:
     SmartZombie(StudentWorld *stud, double locX, double locY);
    virtual ~SmartZombie();
//     virtual void setDead();
private:
    virtual void movementPlan();
};

#endif // ACTOR_H_
