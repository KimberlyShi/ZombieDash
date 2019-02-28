#include "StudentWorld.h"
#include "GameConstants.h"
#include "Actor.h" //i added this
#include <string>
#include <vector> //i added this
#include <iostream>
#include <sstream> //for findLevel
#include <iomanip> //for findLevel
#include <math.h> //use sqrt to find distance
#include "Level.h"
using namespace std;

GameWorld* createStudentWorld(string assetPath)
{
    return new StudentWorld(assetPath);
}

// Students:  Add code to this file, StudentWorld.h, Actor.h and Actor.cpp
StudentWorld::~StudentWorld()
{
    this->cleanUp();
}

StudentWorld::StudentWorld(string assetPath)
: GameWorld(assetPath)
{
}

int StudentWorld::init()
{
    m_numTicks = 0;
    Level lev(assetPath());
    // string levelFile = findLevel(getLevel());
    
    string levelFile = "level03.txt"; //THIS IS JUST FOR TESTING USE COMMENT ABOVE FOR CORRECT IMPLEMENTATION
    Level::LoadResult result = lev.loadLevel(levelFile);
    if (result == Level::load_fail_file_not_found)
        return GWSTATUS_LEVEL_ERROR;
    else if (result == Level::load_fail_bad_format)
        return GWSTATUS_LEVEL_ERROR;
    else if (result == Level::load_success)
    {
        for(int i = 0; i < VIEW_WIDTH; i++)
        {
            for(int j = 0; j < VIEW_HEIGHT; j++)
            {
                Level::MazeEntry object = lev.getContentsOf(i,j);
                switch(object)
                {
                    case Level::player:
                        m_penelope = new Penelope(this, i*16, j*16);
                        //actor.push_back(m_penelope);
                        break;
                    case Level::wall:
                        //m_wall = new Wall(this, i * 16, j *16);
                        actor.push_back(new Wall(this, i * 16, j *16));
                        break;
                    case Level::exit:
                       // m_exit = new Exit(this, i*16, j *16);
                        actor.push_back(new Exit(this, i*16, j *16));
                        break;
                    case Level::landmine_goodie:
                        actor.push_back(new LandmineGoodie(this, i*16, j*16));
                        break;
                    case Level::gas_can_goodie:
                        actor.push_back(new GasCanGoodie(this, i*16, j*16));
                        break;
                    case Level::vaccine_goodie:
                        actor.push_back(new VaccineGoodie(this, i*16, j*16));
                        break;
                        
                    case Level::dumb_zombie:
                        actor.push_back(new DumbZombie(this, i*16, j*16));
                        break;
                        //                    case Level::smart_zombie:
                        //                        actor.push_back(new DumbZombie(this, i*16, j*16));
                        //                        break;
                        
                    case Level::pit:
                        actor.push_back(new Pit(this, i*16, j*16));
                        break;
                    case Level::citizen:
                    {
                        incCitizens();
                        actor.push_back(new Citizen(this, i*16, j*16));
                        break;
                    }
                    default:
                        break;
                }
            }
        }
        setGameStatText(statusLine());
        
        return GWSTATUS_CONTINUE_GAME;
    }
    else
        return GWSTATUS_PLAYER_WON;
    
    //FIX: is there a max number of ticks
}

int StudentWorld::move()
{
    
    m_numTicks++;
    // This code is here merely to allow the game to build, run, and terminate after you hit enter.
    // Notice that the return value GWSTATUS_PLAYER_DIED will cause our framework to end the current level.
    //list<Actor*>::iterator it;
    m_penelope->doSomething();
    for (list<Actor*>::iterator it = actor.begin(); it != actor.end(); it++)
    {
        (*it)->doSomething();
        
        if(m_penelope->isAlive() == 1) //penelope is no longer alive
        {
            decLives();
            m_penelope->resetItems();
            return GWSTATUS_PLAYER_DIED;
        }
        
        if((*it)->finishedLevel())
        {
            playSound(SOUND_LEVEL_FINISHED);
            return GWSTATUS_FINISHED_LEVEL;
        }
        
        
    }
    setGameStatText(statusLine());
    cleanDead();
    //    if(!m_penelope->isAlive()) //penelope is no longer alive
    //    {
    //        decLives();
    //        m_penelope->resetItems();
    //        return GWSTATUS_PLAYER_DIED;
    //    }
    
    
    return GWSTATUS_CONTINUE_GAME;
}

void StudentWorld::cleanUp()
{
    //    list<Actor*>::iterator it = actor.begin();
    
    
    //    while(it != actor.end())
    //    {
    //        delete *it;
    //        actor.erase(it);
    //        //it = actor.erase(it);
    //
    //    }
    list<Actor*>::iterator it;
    for ( list<Actor*>::iterator it = actor.begin(); it != actor.end();)
    {
        delete *it;
        it = actor.erase(it);
    }
    delete m_penelope;
}
//====accessor
Penelope* StudentWorld::getPenelope() const
{
    return m_penelope;
}

//Exit* StudentWorld::getExit() const
//{
//    return m_exit;
//}

int StudentWorld::getTicks() const
{
    return m_numTicks;
}

//========
void StudentWorld::cleanDead()
{
    //    list<Actor*>::iterator it = actor.begin();
    
    
    //  while(it != actor.end())
    for ( list<Actor*>::iterator it = actor.begin(); it != actor.end();)
    {
        if((*it)->isAlive() == 1)
        {
            delete *it;
            it = actor.erase(it);
        }
        else
            it++;
    }
}

void StudentWorld::addActor(Actor *sprite)
{
    //    cout << "Success " << endl;
    actor.push_back(sprite);
    //    cout << "status " << sprite->isAlive() << endl;
}

string StudentWorld::findLevel(int level)
{
    ostringstream oss;
    
    int numCoords = 0;
    int copyLevel = level;
    if(copyLevel/10 == 0)
        numCoords = 1;
    string str;
    
    if(numCoords == 1)//need to append a '0'
    {
        oss.fill('0');
        oss << "level" << setw(2) << level << ".txt";
        str = oss.str();
        
    }
    else
    {
        oss << "level" << level << ".txt";
        str = oss.str();
    }
    return str;
}

string StudentWorld::statusLine()
{
    //display the status at the top of the game
    ostringstream oss;
    string str;
    oss.fill('0');
    oss << "Score: " << setw(6) << getScore() << "  ";
    oss << "Level: " << getLevel() << "  ";
    oss << "Lives: " << getLives() << "  ";
    oss << "Vaccines: " << m_penelope->getVaccines() << "  ";
    oss << "Flames: " << m_penelope->getFlames() << "  ";
    oss << "Mines: " << m_penelope->getMines() << "  ";
    oss << "Infected: " << m_penelope->getInfectCount();
    str = oss.str();
    return str;
}

bool StudentWorld::open(Actor *sprite2, double x, double y)
{
    double firstxMin = x;
    double firstxMax = firstxMin + SPRITE_WIDTH - 1;
    double firstyMin = y;
    double firstyMax = firstyMin + SPRITE_HEIGHT -1;
    
    list <Actor*>::iterator it = actor.begin();
    while(it != actor.end())
    {
        if(*it != sprite2  && (*it)->getCanBlock() == true)
        {
            double secondxMin = (*it)->getX();
            double secondxMax = secondxMin + SPRITE_WIDTH -1;
            double secondyMin = (*it)->getY();
            double secondyMax = secondyMin + SPRITE_HEIGHT - 1;
            
            //situation 1
            if(firstxMin >= secondxMin && firstxMin <= secondxMax)
            {
                //part a
                if(firstyMin >= secondyMin && firstyMin <= secondyMax)
                    return false;
                
                //part b
                if(firstyMax >= secondyMin && firstyMax <= secondyMax)
                    return false;
            }
            
            
            //situation 2
            if(firstxMax >= secondxMin && firstxMax <= secondxMax)
            {
                //part a
                if(firstyMin >= secondyMin && firstyMin <= secondyMax)
                    return false;
                
                //part b
                if(firstyMax >= secondyMin && firstyMax <= secondyMax)
                    return false;
            }
        }
        it++;
        
        
    }
    
    if(sprite2 != m_penelope)
    {
    //check if it overlaps with Penelope's coordinates
    double secondxMin = m_penelope->getX();
    double secondxMax = secondxMin + SPRITE_WIDTH -1;
    double secondyMin = m_penelope->getY();
    double secondyMax = secondyMin + SPRITE_HEIGHT - 1;
    //situation 1
    if(firstxMin >= secondxMin && firstxMin <= secondxMax)
    {
        //part a
        if(firstyMin >= secondyMin && firstyMin <= secondyMax)
            return false;
        
        //part b
        if(firstyMax >= secondyMin && firstyMax <= secondyMax)
            return false;
    }
    
    
    //situation 2
    if(firstxMax >= secondxMin && firstxMax <= secondxMax)
    {
        //part a
        if(firstyMin >= secondyMin && firstyMin <= secondyMax)
            return false;
        
        //part b
        if(firstyMax >= secondyMin && firstyMax <= secondyMax)
            return false;
    }
    }
    return true;
}

//determine if object is overlap
bool StudentWorld::overlap(Actor *sprite1, Actor *sprite2)
{
    //find the center
    const double x1 = (*sprite1).getX() + SPRITE_WIDTH/2 - 1;
    const double y1 = (*sprite1).getY() + SPRITE_HEIGHT/2 -1;
    const double x2 = (*sprite2).getX() + SPRITE_WIDTH/2 -1;
    const double y2 = (*sprite2).getY() + SPRITE_HEIGHT/2 -1;
    const double differenceX = x1 - x2;
    const double differenceY = y1 - y2;
    
    //(∆x)2 + (∆y)2 ≤ 10 //that means that there was overlap
    if(((differenceX * differenceX) + (differenceY * differenceY)) <= 10*10) //does overlap
        return true;
    return false;
}

//exit shoudl cal
void StudentWorld::overlapExit(Actor *exit)
{
    for (list<Actor*>::iterator it = actor.begin(); it != actor.end(); it++)
    {
        if((*it)->getCanExit())
        {
            //check if overlap
            //overlap will return true
            if(overlap(exit, *it))
            {
                
                (*it)->successExit();
              //  return true; //there was overlap
            }
        }
    }
    
    //return false;

}
void StudentWorld::overlapPit(Actor *pit)
{
    //the objects that activate landmines (humans, zombies) will also die in pits
    //traverse through actor
    for (list<Actor*>::iterator it = actor.begin(); it != actor.end(); it++)
    {
        if((*it)->canActivateMine())
        {
            //check if overlap
            //overlap will return true
            if(overlap(pit, *it))
            {
                (*it)->setDead();
            }
        }
    }
    
    if(m_penelope->canActivateMine())
    {
        //check if overlap
        //overlap will return true
        if(overlap(pit, m_penelope))
        {
            m_penelope->setDead();
        }
    }
}

bool StudentWorld::overlapLandmine(Actor *mine)
{
    //traverse through actor
    for (list<Actor*>::iterator it = actor.begin(); it != actor.end(); it++)
    {
        if((*it)->canActivateMine())
        {
            //check if overlap
            //overlap will return true
            if(overlap(mine, *it))
                return true;
        }
    }
    
    if(m_penelope->canActivateMine())
    {
        //check if overlap
        //overlap will return true
        if(overlap(mine, m_penelope))
            return true;
    }
    return false;
}

bool StudentWorld::overlapFlames(Actor *posFlame)
{
    //traverse through actor
    for (list<Actor*>::iterator it = actor.begin(); it != actor.end(); it++)
    {
        if((*it)->canBlockFlames()) //only wall or exit can block flames
        {
            //check if overlap
            //overlap will return true
            if(overlap(posFlame, *it))
                return true;
        }
    }
    
    if(m_penelope->canBlockFlames()) //only wall or exit can block flames
    {
        //check if overlap
        //overlap will return true
        if(overlap(posFlame, m_penelope))
            return true;
    }
    
    return false;
}

void StudentWorld::flameDamages(Actor *flame)
{
    //see if it overlaps and do damages accordingly
    for (list<Actor*>::iterator it = actor.begin(); it != actor.end(); it++)
    {
        if((*it)->flameCanDamage())
        {
            //check if overlap
            if(overlap(flame, *it)) //overlap will return true
            {
                //do appropriate damages
                //damages can happen to the following:
                //penelope --NOT tested
                //zombies --NOT implemented
                //goodies -- WORKS :)
                //landmines -- NOT implemented
                
                (*it)->setDead(); //right now, working for goodies
                
                
            }
        }
    }
    
    if(m_penelope->flameCanDamage())
    {
        //check if overlap
        if(overlap(flame, m_penelope)) //overlap will return true
        {
            
            m_penelope->setDead(); //right now, working for goodies
            
            
        }
    }
}


//void StudentWorld::closestZombieToCitizen(Actor *citizen, double &zombieX, double &zombieY, double &distance)
void StudentWorld::closestZombieToCitizen(double citizenX, double citizenY, double &zombieX, double &zombieY, double &distance)
{
    double xDiff = 0.0;
    double yDiff = 0.0;
    double disSqared = 0.0;
    double tempDis = 0.0;
    for (list<Actor*>::iterator it = actor.begin(); it != actor.end(); it++)
    {
        //shouldn't have to test for Penelope
        if((*it)->isLivingActor())
        {
            
            if(!((*it)->getCanExit())) //only zombie can't exit
            {
                 //cout << "GOT HERE " << endl;
                //(*it) must be a zombie
//                xDiff = citizen->getX() - (*it)->getX();
//                yDiff = citizen->getY() - (*it)->getY();
                xDiff = citizenX - (*it)->getX();
                yDiff = citizenY - (*it)->getY();
                disSqared = (xDiff *xDiff) + (yDiff * yDiff);
                tempDis = sqrt(disSqared);
                
//                cout << "tempDis " << tempDis << endl;
                //want to find the shortest distance
                
                if(tempDis < distance)
                {
                    distance = tempDis;
                    zombieX = (*it)->getX();
                    zombieY = (*it)->getY();
                    // cout << "tempDis " << tempDis << endl;
//                    cout << "GOT HERE " << endl;
                }
            }
        }
    }
}


bool StudentWorld::overlapCitizenPenelope(Actor *sprite1, Actor *sprite2)
{
    //find the center
    const double x1 = (*sprite1).getX() + SPRITE_WIDTH/2 - 1;
    const double y1 = (*sprite1).getY() + SPRITE_HEIGHT/2 -1;
    const double x2 = (*sprite2).getX() + SPRITE_WIDTH/2 -1;
    const double y2 = (*sprite2).getY() + SPRITE_HEIGHT/2 -1;
    const double differenceX = x1 - x2;
    const double differenceY = y1 - y2;
    
    //(∆x)2 + (∆y)2 ≤ 80
    if(((differenceX * differenceX) + (differenceY * differenceY)) <= 80 * 80) //does overlap
        return true;
    return false;
    //true means that the citizen wants to follow penelope
}

int StudentWorld::numCitizens() const
{
    return m_citizens;
}
void StudentWorld::decCitizens()
{
    m_citizens--;
}
void StudentWorld::incCitizens()
{
    m_citizens++;
}
