#include "StudentWorld.h"
#include "GameConstants.h"
#include "Actor.h" //i added this
#include <string>
#include <vector> //i added this
#include <iostream>
#include <sstream> //for findLevel
#include <iomanip> //for findLevel
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
                        actor.push_back(m_penelope);
                        break;
                    case Level::wall:
                        m_wall = new Wall(this, i * 16, j *16);
                        actor.push_back(m_wall);
                        break;
                    case Level::exit:
                        m_exit = new Exit(this, i*16, j *16);
                        actor.push_back(m_exit);
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
    for (list<Actor*>::iterator it = actor.begin(); it != actor.end(); it++)
    {
        (*it)->doSomething();
        if((*it)->finishedLevel())
        {
            
            return GWSTATUS_FINISHED_LEVEL;
        }
    }
    
    cleanDead();
    
    setGameStatText(statusLine());
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
}
//====accessor
Penelope* StudentWorld::getPenelope() const
{
    return m_penelope;
}

Exit* StudentWorld::getExit() const
{
    return m_exit;
}

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
    actor.push_back(sprite);
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
    //getX and getY give center!!
    //check if there's already a something in that position
    
    //numbers refer to coordinates below
    //second refers to the second sprite (the one passed in)
    // 4    3
    // 1    2
    //    double second1x = x - (*sprite2).getSpriteWidth()/2 - 1;
    //    double second1y = y - (*sprite2).getSpriteHeight()/2 - 1;
    //    double second2x =  x + (*sprite2).getSpriteWidth()/2 - 1;
    //    double second3y =  y + (*sprite2).getSpriteHeight()/2 - 1;
    //
    //    vector<Actor*>::iterator it = actor.begin();
    //    while(it != actor.end())
    //    {
    //        //first refers to sprite1 bounding box
    //        //second refers to sprite2 bounding box
    //        double first1x = (*it) ->getX() - (*it)->getSpriteWidth()/2 - 1;
    //        double first1y = (*it) ->getY() - (*it)->getSpriteHeight()/2 - 1;
    //        double first2x = first1x + (*it)->getSpriteWidth() - 1;
    //        double first3y = first1y + (*it)->getSpriteHeight() -1;
    //
    //
    //        for(int i = first1x; i < first2x; i++) //width of sprite 1
    //        {
    //            for(int j = first1y; j < first3y; j++) //height of sprite 1
    //            {
    //                //check if it's equal to any of the coordinates of sprite 2
    //                for(int k = second1x; k < second2x; k++) //width of sprite 2
    //                {
    //                    for(int r = second1y; r < second3y; r++) //height of sprite 2
    //                    {
    //                        if(sprite2 != *it && (*it)->getCanBlock() == true)
    //                        {
    //                            if(i == k && j == r)
    //                                return false;
    //                        }
    //                    }
    //                }
    //            }
    //        }
    //
    //        it++;
    //    }
    //    return true;
    // 4    3
    // 1    2
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
    
//    cout << "DIFFERENCE X: " << differenceX << endl;
//    cout << "DIFFERENCE Y: " << differenceY << endl;
    //(∆x)2 + (∆y)2 ≤ 10 //that means that there was overlap
    if(((differenceX * differenceX) + (differenceY * differenceY)) <= 10*10) //does overlap
        return true;
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
    
    return false;
}
