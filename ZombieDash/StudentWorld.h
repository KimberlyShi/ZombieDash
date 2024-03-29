#ifndef STUDENTWORLD_H_
#define STUDENTWORLD_H_

#include "GameWorld.h"
#include "Actor.h"
#include <string>
#include <list>

class Actor;

class StudentWorld : public GameWorld
{
public:
    StudentWorld(std::string assetPath);
    virtual int init();
    virtual int move();
    virtual void cleanUp();
    ~StudentWorld();
    bool open(Actor *sprite2, double x, double y); //check if proposed spot is open
    bool overlap(Actor *sprite1, Actor *sprite2, int value); //check if the object is overlapping
    //overlap will return true if there is overlap
    
    void overlapExit(Actor *exit);
    bool overlapFlames(Actor *posFlame); //return true if there is overlap
    bool overlapLandmine(Actor *mine); //return true if there is overlap
    void overlapPit(Actor *pit);
    bool overlapLiving(Actor *vomit);
    void flameDamages(Actor *flame); //will be called everytime a flame is created from Penelope
    
    void cleanDead();
    void addActor(Actor *sprite); //add the actor to the list
    
    std::string findLevel(int level);
    std::string statusLine();
    
    void closestZombieToCitizen(double citizenX, double citizenY, double &zombieX, double &zombieY, double &distance);
    void closestPersonToZombie(double zombie1X, double zombie1Y, double &personX, double &personY, double &distance);
    
    Penelope *getPenelope() const;
    int getTicks() const;
    void newDirectionLess80(double x1, double y1, double x2, double y2, Direction &tempDir);
    
    int numCitizens() const;
    void decCitizens();
    void incCitizens();
    
    
private:
    std::list<Actor*> actor;
    Penelope *m_penelope; //pointer to penelope
    int m_numTicks;
    int m_citizens;
};

#endif // STUDENTWORLD_H_
