#ifndef ACTOR_H_
#define ACTOR_H_
#include "GraphObject.h"

class StudentWorld; //need to declare the StudentWorld

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
    bool getCanBlock() const;
    bool finishedLevel() const; //true indicates that Penelope finished
    bool canBlockFlames() const; //only wall or exit can block flames, true if can block
    void setActivation(); //will be called if the mine can be activated by that actor
    bool canActivateMine() const; //only zombie, penelope, and citizen will return true
    virtual void setFlameCanDamage(bool val); //mutator
    bool flameCanDamage() const; //accessor
    virtual void setDead();
    void setfinishedLevelTrue();
    bool isLivingActor() const; //not to be confused with the status of isAlive()
    //isAlive() refers to whether or not the object is still valid
    //living actor: zombie, penelope, citizen AND dead actor is everything else
    void setLivingActor();
    void canExitTrue(); //only can be called in Penelope and citizen
    bool getCanExit() const;
    void newDirection(double &tempX, double &tempY, Direction tempDir, double value);
    void successExit();
    bool getInfectStat() const;
    int getInfectCount() const;
    void increaseInfectCount();
    void setInfectStat(bool val);
    
private:
    StudentWorld *m_stud;
    int m_status; //2-alive 1-dead 0-not applicable ex. wall, exit
    bool m_canBlock; //true if the object can be blocked (penelope can't run into)
    bool m_levelStatus;
    bool m_canBlockFlames;
    bool m_flameCanDamage;
    bool m_canActivateMine;
    bool m_canExit; //default false //only citizen and penelope can exit and return true
    bool m_live;
    bool m_infectStat;
    int m_infectCount;
};
//====HUMAN===
class Human: public Actor
{
public:
    Human(StudentWorld *stud, double locX, double locY, int imgid);
};
//======PENLEOPE=============
class Penelope: public Human
{
public:
    Penelope(StudentWorld *stud, double locX, double locY);
    
    virtual ~Penelope();
    virtual void doSomething();
    int getVaccines() const;
    int getMines() const;
    int getFlames() const;
    bool isValid(double x, double y);
    void addFlames(int num);
    void addVaccines(int num);
    void addMines(int num);
    
    virtual void setDead();
    
    void resetItems(); //after each level, the goodies need to be reset
    //will NOT reset score
    
private:
    int m_vaccines;
    int m_mines;
    int m_flames;
    int keyVal;
    int posNeg;
    int m_dir;
    bool canBlock;
    
};

class Citizen: public Human
{
public:
    Citizen(StudentWorld *stud, double locX, double locY);
    virtual void doSomething();
    virtual void setDead();
    bool citizenZombie(double dist_z); //false-if no zombies
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
public:
    BadThings(StudentWorld *stud, double locX, double locY, int imgid, Direction dir);
    virtual void doSomething();
    virtual void startTick();
    int getStartTick() const;
    // virtual void setDead();
private:
    int m_startTick;
};

class Landmines: public BadThings
{
public:
    Landmines(StudentWorld *stud, double locX, double locY);
    virtual void doSomething();
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
class Zombie: public Actor
{
public:
    Zombie(StudentWorld *stud, double locX, double locY);
    virtual ~Zombie();
    virtual void doSomething();
    void randDirection();
    
    int getPlanDistance() const;
    
    //mutator
    void setPlanDistance(int set);
    void decPlanDistance();
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
    virtual void setDead();
    
private:
    virtual void movementPlan();
};

class SmartZombie: public Zombie
{
public:
    SmartZombie(StudentWorld *stud, double locX, double locY);
    virtual ~SmartZombie();
    virtual void setDead();
private:
    virtual void movementPlan();
};

#endif // ACTOR_H_
