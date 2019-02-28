#include "Actor.h"
#include "StudentWorld.h"
#include <math.h>

Actor::Actor(StudentWorld *stud, double locX, double locY, int imgid, int statAlive, Direction dir, int depth, int size, bool canBlock, bool canBlockFlames)
:GraphObject(imgid, locX, locY, dir, depth, size)
{
    m_status = statAlive;
    m_stud = stud;
    m_canBlock = canBlock;
    m_levelStatus = false;
    m_canBlockFlames = canBlockFlames;
    m_canActivateMine = false;
    m_canExit = false;
    m_live = false;
    
    m_infectStat = false; //infection status starts off as false
    m_infectCount = 0; //infection count is 0
};
Actor::~Actor()
{}

void Actor::doSomething()
{}

bool Actor::getCanBlock() const
{
    return m_canBlock;
}

bool Actor::canBlockFlames() const
{
    return m_canBlockFlames;
}
bool Actor::canActivateMine() const
{
    return m_canActivateMine;
}
void Actor::setActivation()
{
    m_canActivateMine = true;
    //will only be called to set to true for zombie, citizen, penelope
    //otherwise, m_canActivateMine should be false
}
void Actor::setFlameCanDamage(bool val) //mutator
{
    m_flameCanDamage = val;
}

bool Actor::flameCanDamage() const //accessor
{
    return m_flameCanDamage;
}

bool Actor::finishedLevel() const
{
    return m_levelStatus;
}

int Actor::isAlive() const//check status of alive
{
    return m_status;
}

void Actor::setDead()
{
    m_status = 1; //that means that the actor died
}

void Actor::setfinishedLevelTrue()
{
    m_levelStatus = true;
}

void Actor::canExitTrue() //only can be called in Penelope and citizen
{
    m_canExit = true;
}
bool Actor::getCanExit() const
{
    return m_canExit;
}
StudentWorld *Actor::getStud() const
{
    return m_stud;
}

bool Actor::isLivingActor() const //not to be confused with the status of isAlive()
//isAlive() refers to whether or not the object is still valid
//living actor: zombie, penelope, citizen AND dead actor is everything else
{
    return m_live;
}
void Actor::setLivingActor()
{
    m_live = true;
}

void Actor::successExit()
{
    getStud()->decCitizens();
    getStud()->playSound(SOUND_CITIZEN_SAVED);
    getStud()->increaseScore(500);
    Actor::setDead();
    
}
bool Actor::getInfectStat() const
{
    return m_infectStat;
}
int Actor::getInfectCount() const
{
    return m_infectCount;
}

void Actor::increaseInfectCount()
{
    m_infectCount++;
}

void Actor::setInfectStat(bool val)
{
    m_infectStat = val;
}

void Actor::newDirection(double &tempX, double &tempY, Direction tempDir, double value)
{
    if(tempDir == right)
    {
        tempX = getX() + value;
        tempY = getY();
    }
    if(tempDir == left)
    {
        tempX = getX() - value;
        tempY = getY();
    }
    if(tempDir == up)
    {
        tempX = getX();
        tempY = getY() + value;
    }
    if(tempDir == down)
    {
        tempX = getX();
        tempY = getY() - value;
    }
}
//=========PENELOPE
Human::Human(StudentWorld *stud, double locX, double locY, int imgid)
:Actor(stud, locX, locY, imgid,2,right, 0, 1, true, false)
{
    canExitTrue();
    setFlameCanDamage(true); //can be damaged by flames
    setActivation(); //can activate landmines
    setLivingActor();
}

//CITIZEN=====
Citizen::Citizen(StudentWorld *stud, double locX, double locY)
:Human(stud, locX, locY, IID_CITIZEN)
{
}

bool Citizen::citizenZombie(double dist_z)
{ //aka step 7 on pg 45 of spec
    //dist_z is the distance between the citizen and zombie
    //check if the distance is less than or equal to 80 pixels
    if(dist_z <= 80)
    {
        //rightD for example indicates distance for the respective direction
        double rx = 0.0; //coordinates of the closest zombie
        double ry = 0.0;
        
        double rightD = VIEW_WIDTH; //refers to distance
        double leftD = VIEW_WIDTH;
        double upD = VIEW_HEIGHT;
        double downD = VIEW_HEIGHT;
        
        
        
        //check if where want to move is open
        //check distance for each of them
        if(getStud()->open(this, getX() + 2, getY()))//right
        {
            //closestZombieToCitizen will set rightD to the distance between citizen and the closest zombie
            getStud()->closestZombieToCitizen(getX() + 2,getY(), rx, ry, rightD);
        }
        
        if(getStud()->open(this, getX() - 2, getY()))//left
            getStud()->closestZombieToCitizen(getX() - 2 ,getY(), rx, ry, leftD);
        
        if(getStud()->open(this, getX(), getY() + 2))//up
            getStud()->closestZombieToCitizen(getX(),getY() + 2, rx, ry, upD);
        
        if(getStud()->open(this, getX(), getY() - 2))//down
            getStud()->closestZombieToCitizen(getX(),getY() - 2, rx, ry, downD);
        
        double newDist = 0.0;
        
        //test if it was block by comparing to 256
        double citizenX = getX();
        double citizenY = getY();
        Direction newDirection = right; //just initialize to right but that's just temporary
        
        //std::cout << "BEFORE X " << citizenX << " Y " << citizenY << std::endl;
        if(rightD != VIEW_WIDTH && rightD > newDist && rightD > dist_z)
        {
            newDist = rightD;
            newDirection = right;
            citizenX = getX() + 2;
            citizenY = getY();
        }
        if (leftD != VIEW_WIDTH && leftD > newDist && leftD > dist_z)
        {newDist = leftD;
            newDirection = left;
            citizenX = getX() - 2;
            citizenY = getY();
        }
        if (upD != VIEW_HEIGHT && upD > newDist && upD > dist_z)
        {
            newDist = upD;
            newDirection = up;
            citizenX = getX();
            citizenY = getY() +  2;
        }
        if (downD != VIEW_HEIGHT && downD > newDist && downD > dist_z)
        {
            newDist = downD;
            newDirection = down;
            citizenX = getX();
            citizenY = getY()- 2;
        }
        
        if(newDist < dist_z)
            return false;
 
        setDirection(newDirection); //set the citizen to a new direction
        moveTo(citizenX, citizenY); //move citizen in that direction
    
        return true;
    }
    else //no zombie within euclidean distance
        return false;
}

void Citizen::doSomething()
{
    if(isAlive() == 1) //is dead
        return;
    
    if(getInfectStat() == true) //if citizen is infected
    {
        //she's infected so increase infection count
        increaseInfectCount();
        //m_infectCount++;
        if(getInfectCount() >= 500)
        {
            //            std::cout << "NOT " << std::endl;
            //becomes a zombie
            Actor::setDead(); //set status to dead
            (getStud()) -> playSound(SOUND_ZOMBIE_BORN); //play sound
            getStud()->decCitizens();
            getStud()->increaseScore(-1000); //decrease player score
            
            int probability = randInt(1, 10);
            if(probability >= 1 && probability <=7) //70% chance become dumb
                getStud()->addActor(new DumbZombie(getStud(), getX(), getY()));
            else //will either be 8,9,10 (30% probability)
                getStud()->addActor(new SmartZombie(getStud(), getX(), getY()));
            return;
        }
    }
    
    if(getStud()->getTicks() % 2 == 0) //even tick
    {
        return; //paralyzed
    }
    
    //calculate distance to penelope
    double xPenelopeDis = (getStud()->getPenelope())->getX() -getX();
    double yPenelopeDis = (getStud()->getPenelope())->getY() -getY();
    double dist_pSquared = (xPenelopeDis * xPenelopeDis) + (yPenelopeDis * yPenelopeDis);
    double dist_p = sqrt(dist_pSquared); //sqrt is a function imported from math.h
    
    
    double dist_z = VIEW_HEIGHT;
    double zombieX = 0.0;
    double zombieY = 0.0;
    double tempX = getX();
    double tempY = getY();
    
    
    getStud()->closestZombieToCitizen(getX(), getY(), zombieX, zombieY, dist_z);
    //zombieX will now have the coordinates of the nearest zombie
    //dist_z (if not 256) will have distance to nearest zombie
    //dist_z means that there were no zombies
    
    if((dist_p < dist_z || dist_z == VIEW_HEIGHT) && dist_p <=80) //no zombies on that level
    {
        //check Euclidean distance from penelope to citizen
        //citizen wants to follow penelope
        if(getStud()->overlapCitizenPenelope(this, getStud()->getPenelope()))
        {
            Direction tempDir = 0;
            getStud()->newDirectionLess80(getStud()->getPenelope()->getX(), getStud()->getPenelope()->getY(),getX(), getY(), tempDir);
            
            //check if can move 2 pixels in that direction
            newDirection(tempX, tempY, tempDir, 2);
            
            //check if that temp is open
            if(getStud()->open(this, tempX, tempY))
            {
                setDirection(tempDir);
                moveTo(tempX, tempY);
                return;
            }
            else
            {
                if(getX() == getStud()->getPenelope()->getX() || getY() == getStud()->getPenelope()->getY()) //check if same row/column
                {
                    
                    //skip step 7
                    citizenZombie(dist_z);
                    return; //FIX
                }
                else //not same row or column
                {
                    Direction newTempDir = 0;
                    //choose the opposite direction
                    switch(tempDir)
                    {
                        case right:
                            newTempDir = left;
                            break;
                        case left:
                            newTempDir = right;
                            break;
                        case up:
                            newTempDir = down;
                            break;
                        case down:
                            newTempDir = up;
                            break;
                        default:
                            break;
                    }
                
                    //check if can move 2 pixels in that NEW direction
                    newDirection(tempX, tempY, newTempDir, 2);
                   
                    //check if that temp is open
                    if(getStud()->open(this, tempX, tempY))
                    {
                        setDirection(tempDir);
                        moveTo(tempX, tempY);
                        return;
                    }
                    else
                    {
                        //skip to step 7
                        citizenZombie(dist_z);
                        return;
                    }
                    
                }
            }
        }
    }
    citizenZombie(dist_z);
    return;
    
}

void Citizen::setDead()
{
    Actor::setDead();
    getStud()->decCitizens(); //decrease the number of citizens
    (getStud()) -> playSound(SOUND_CITIZEN_DIE); //play sound
    getStud()->increaseScore(-1000); //decrease score by 1000 points
}


//PENELOPE====
Penelope::Penelope(StudentWorld *stud, double locX, double locY)
//:Actor(stud, locX, locY, IID_PLAYER, 2, right, 0, 1, true, false)
:Human(stud, locX, locY, IID_PLAYER)
{
    //    m_infectStat = false; //infection status starts off as false
    //    m_infectCount = 0; //infection count is 0
    m_vaccines = 0;
    m_mines = 0;
    m_flames = 0;
    keyVal = 0;
    posNeg = 0;
    m_dir = right;
    //    setFlameCanDamage(true); //can be damaged by flames
    //    setActivation(); //can activate landmines
}

Penelope::~Penelope()
{
    
}
//Penelope's Accessors
//bool Penelope::getInfectStat() const
//{
//    return m_infectStat;
//}
//int Penelope::getInfectCount() const
//{
//    return m_infectCount;
//}

int Penelope::getVaccines() const
{
    return m_vaccines;
}

int Penelope::getMines() const
{
    return m_mines;
}

int Penelope::getFlames() const
{
    return m_flames;
}
//Penelope's Mutators
void Penelope::addFlames(int num)
{
    m_flames+= num;
}
void Penelope::addVaccines(int num)
{
    m_vaccines += num;
}
void Penelope::addMines(int num)
{
    m_mines+= num;
}

void Penelope::resetItems()
{
    m_flames = 0;
    m_vaccines = 0;
    m_mines = 0;
}

//Penelope's Functions
void Penelope::doSomething()
{
    if(isAlive() == 1) //dead already
        return;
    if(getInfectStat() == true) //if she's infected
    {
        //she's infected so increase infection count
        increaseInfectCount();
        //m_infectCount++;
        if(getInfectCount() >= 500)
        {
            //becomes a zombie
            setDead(); //set status to dead
            (getStud()) -> playSound(SOUND_PLAYER_DIE); //play sound
            return;
        }
        //        return; //??? NOT SURE IF THIS IS CORRECT?????
    }
    
    int keyVal;
    double x, y;
    if (getStud()->getKey(keyVal))
    {
        switch(keyVal)
        {
            case KEY_PRESS_UP:
            {
                setDirection(up);
                x = getX();
                y = getY() + 4;
                if(isValid(x,y))
                    moveTo(x,y);
                break;
            }
            case KEY_PRESS_DOWN:
            {
                setDirection(down);
                x = getX();
                y = getY() - 4;
                if(isValid(x,y))
                    moveTo(x,y);
                break;
            }
            case KEY_PRESS_LEFT:
            {
                setDirection(left);
                x = getX() - 4;
                y = getY();
                if(isValid(x,y))
                    moveTo(x,y);
                break;
            }
            case KEY_PRESS_RIGHT:
            {
                setDirection(right);
                x = getX() + 4;
                y = getY();
                if(isValid(x,y))
                    moveTo(x,y);
                break;
            }
            case KEY_PRESS_SPACE: //flamethrower
            {
                if(getFlames() >= 1) //has flames
                {
                    m_flames--;
                    getStud()->playSound(SOUND_PLAYER_FIRE);
                    
                    
                    int count = 1;
                    while(count <= 3) //1,2,3
                    {
                        //add 3 flames in front
                        double tempX = 0.0; //possible location of flame
                        double tempY = 0.0;
                        
                        newDirection(tempX, tempY, getDirection(), count*SPRITE_WIDTH);
                        //                        if(getDirection() == up)
                        //                        {
                        //                            tempX = getX();
                        //                            tempY = getY() + count * SPRITE_HEIGHT;
                        //                        }
                        //                        if(getDirection() == down)
                        //                        {
                        //                            tempX = getX();
                        //                            tempY = getY() - count * SPRITE_HEIGHT;
                        //                        }
                        //                        if(getDirection() == left)
                        //                        {
                        //                            tempX = getX() - count *SPRITE_WIDTH;
                        //                            tempY = getY();
                        //                        }
                        //                        if(getDirection() == right)
                        //                        {
                        //                            tempX = getX() + count *SPRITE_WIDTH;
                        //                            tempY = getY();
                        //                        }
                        //check if that temp location will overlap with wall or exit
                        //first create the new flames object
                        Flames *newFlame = new Flames(getStud(), tempX, tempY, up);
                        //check if it will overlap
                        //  std::cout << "COUNTTT: " << count << " X: " << tempX << " Y: " << tempY << std::endl;
                        if(getStud()->overlapFlames(newFlame))//there was overlap
                        {
                            // std::cout<< "COUNT " << count << std::endl;
                            delete newFlame; //make sure to delete that newFlame if not pushed
                            break;
                        }
                        else
                        {
                            //there was no overlap so push onto the list
                            getStud()->addActor(newFlame);
                            count++;
                        }
                    }
                }
                break;
            }
            case KEY_PRESS_TAB: //landmine
            {
                if(getMines() >= 1) //has landmines
                {
                    m_mines--;
                    getStud()->addActor(new Landmines(getStud(), getX(), getY())); //add landmine
                }
                break;
            }
            case KEY_PRESS_ENTER: //vaccine
            {
                if(getVaccines() >= 1)
                {
                    //set infected status to false
                    setInfectStat(false);
                    //m_infectStat = false;
                    m_vaccines--;
                }
                break;
            }
            default:
                break;
        }
        return;
    }
}

bool Penelope::isValid(double x, double y)
{
    //check that won't hit the border
    if(x >= VIEW_WIDTH || x < 0 || y >= VIEW_HEIGHT || y <0)
        return false;
    if(!getStud()->open(this,x, y))
        return false;
    return true;
    
}
void Penelope::setDead()
{
    Actor::setDead();
    //    getStud()->increaseScore(-1000); //decrease score by 1000
    getStud()->playSound(SOUND_PLAYER_DIE);
}
//=========WAll
Wall::Wall(StudentWorld * stud, double locX, double locY)
:Actor(stud, locX, locY, IID_WALL, 0, right, 0, 1, true, true)
{
}

Wall::~Wall()
{
    
}

void Wall::doSomething()
{
    return;
}

//=========EXIT

Exit::Exit(StudentWorld* stud,double locX, double locY)
:Actor(stud, locX, locY, IID_EXIT, 0, right, 1, 1, false, true)
{
}

Exit:: ~Exit()
{
}

void Exit::doSomething()
{
    //see if citizen and exit will overlap
    getStud()->overlapExit(this);
    //see if penelope and exit overlap
    //check if there are any more citizens left
    //only when all citizens have left or died can penelope leave
    if (getStud()->numCitizens() == 0)
    {
        if((getStud())->overlap(this, getStud()->getPenelope()))
        {
            setfinishedLevelTrue();
            return;
        }
    }
}



//======GOODIES===========================
Goodies::Goodies(StudentWorld* stud, double locX, double locY, int imgid)
:Actor(stud, locX, locY, imgid, 2, right, 1, 1, false, false) //Goodies will start out alive
{
    setFlameCanDamage(true); //can be damaged by flames
}

Goodies::~Goodies()
{
    
}
void Goodies::doSomething()
{
    checkOverlap();
    
}

void Goodies::checkOverlap()
{
    if(isAlive() == 1) //not alive
    {
        return;
    }
    else
    {
        //the goodie does overlap with Penelope
        if(getStud()->overlap(getStud()->getPenelope(), this))
        {
            getStud()->increaseScore(50);
            this->setDead();
            getStud()-> playSound(SOUND_GOT_GOODIE);  //play sound
            
            increaseGoodie();//in the virtual implementations, need to increase the respective number
            return;
        }
    }
}

void Goodies::increaseGoodie()
{
    return;
}

VaccineGoodie::VaccineGoodie(StudentWorld* stud,double locX, double locY)
:Goodies(stud, locX, locY, IID_VACCINE_GOODIE)
{
}

VaccineGoodie::~VaccineGoodie()
{
}
void VaccineGoodie::increaseGoodie()
{
    
    (getStud()->getPenelope())->addVaccines(1);
}

GasCanGoodie::GasCanGoodie(StudentWorld* stud, double locX, double locY)
:Goodies(stud, locX, locY, IID_GAS_CAN_GOODIE)
{
    
}
GasCanGoodie::~GasCanGoodie()
{
    
}
void GasCanGoodie::increaseGoodie()
{
    (getStud()->getPenelope())->addFlames(5);
}

LandmineGoodie::LandmineGoodie(StudentWorld* stud, double locX, double locY)
:Goodies(stud, locX, locY, IID_LANDMINE_GOODIE)
{
    
}
LandmineGoodie::~LandmineGoodie()
{
    
}
void LandmineGoodie::increaseGoodie()
{
    (getStud()->getPenelope())->addMines(2);
    //Penelope::addMines(2);
}
//Bad Things: aka Vomit, Pit, Flames
//Actor(StudentWorld *stud, double locX, double locY, int imgid, int statAlive, Direction dir, int depth, int size, bool canBlock);

BadThings::BadThings(StudentWorld *stud, double locX, double locY, int imgid, Direction dir)
:Actor(stud, locX, locY, imgid, 2, dir, 0, 1, false, false)
{
    m_startTick = 0;
    setFlameCanDamage(false);
}

void BadThings::doSomething()
{
    
}
void BadThings::startTick()
{
    // return m_currentTick;
    m_startTick = (getStud())->getTicks();
}
int BadThings::getStartTick() const
{
    return m_startTick;
}



Landmines::Landmines(StudentWorld *stud, double locX, double locY)
:BadThings(stud, locX, locY, IID_LANDMINE,right)
{
    //override the setFlameDamages
    setFlameCanDamage(true);
    m_inactiveState = true; //starts off inactive
    m_countdownTicks = 30;
}
void Landmines::doSomething()
{
    if(isAlive() == 1) //landmine is already dead
        return;
    
    if (m_inactiveState && m_countdownTicks >= 0) //landmine is not active
    {
        m_countdownTicks--;
        if(m_countdownTicks == 0)
        {
            m_inactiveState = false;
            return;
        }
    }
    else if (!m_inactiveState)//now active landmine
    {
        //using the overlapFlame function (will never check if overlaps with wall or exit)
        //if landmine overlaps with zombie, penelope, citizen
        if(getStud() -> overlapLandmine(this))
        {
            if(isAlive() == 2)
            {
                setDead(); //set the landmine to dead
                getStud()->playSound(SOUND_LANDMINE_EXPLODE);
                
                //this will just be where the landmine was- 1 flame
                getStud()->addActor(new Flames(getStud(), getX(), getY(), right));
                
                
                //will only display the flame if it doesn't overlap with anything
                //8 flame objects
                
                double tempX = 0.0;
                double tempY = 0.0;
                for(int i = 0; i < 8; i++)
                {
                    //this will introduce flame objects in the 8 adjacent slots
                    if(i == 0)//north
                    {
                        tempX = getX();
                        tempY = getY() + SPRITE_HEIGHT;
                    }
                    if(i == 1)//northeast
                    {
                        tempX = getX() - SPRITE_WIDTH;
                        tempY = getY() + SPRITE_HEIGHT;
                    }
                    if(i == 2)//northwest
                    {
                        tempX = getX() + SPRITE_WIDTH;
                        tempY = getY() + SPRITE_HEIGHT;
                    }
                    if(i == 3)//east
                    {
                        tempX = getX() - SPRITE_WIDTH;
                        tempY = getY();
                    }
                    if(i == 4)//west
                    {
                        tempX = getX() + SPRITE_WIDTH;
                        tempY = getY();
                    }
                    if(i == 5)//south
                    {
                        tempX = getX();
                        tempY = getY() - SPRITE_HEIGHT;
                    }
                    if(i == 6)//southeast
                    {
                        tempX = getX() - SPRITE_WIDTH;
                        tempY = getY() - SPRITE_HEIGHT;
                    }
                    if(i == 7) //southwest
                    {
                        tempX = getX() + SPRITE_WIDTH;
                        tempY = getY() - SPRITE_HEIGHT;
                    }
                    //need to see if the tempX and tempY will overlap with wall or exit
                    Flames *temp = new Flames(getStud(), tempX, tempY, right);
                    if(getStud()->overlapFlames(temp))
                    {
                        //if there is overlap, will have to delete that temp flame
                        delete temp;
                    }
                    else
                    {
                        getStud()->addActor(temp);
                    }
                }
                //introduce pit where the landmine was
                getStud()->addActor(new Pit(getStud(), getX(), getY()));
            }
        }
    }
}

Pit::Pit(StudentWorld *stud, double locX, double locY)
:BadThings(stud, locX, locY, IID_PIT, right)
{
    
}

void Pit::doSomething()
{
    getStud()->overlapPit(this);
    
}

Vomit::Vomit(StudentWorld *stud, double locX, double locY, Direction dir)
:BadThings(stud, locX, locY, IID_VOMIT, dir)
{
    
}

void Vomit::doSomething()
{
    if(isAlive() == 1) //vomit is already dead
        return;
    //set the currentTick if not already
    if(getStartTick() == 0)
        startTick();
    //check if there has been 2 ticks since creation
    if(getStud()->getTicks() - getStartTick() == 2)
    {
        //set state to dead
        setDead();
        return;
    }
    
}

Flames::Flames(StudentWorld *stud, double locX, double locY, Direction dir)
:BadThings(stud, locX, locY, IID_FLAME, dir)
{
    setActivation(); //can activate landmines
}

void Flames::doSomething()
{
    if(isAlive() == 1) //flame is already dead
        return;
    //set the currentTick if not already
    if(getStartTick() == 0)
        startTick();
    //check if there has been 2 ticks since creation
    if(getStud()->getTicks() - getStartTick() == 2)
    {
        //set state to dead
        setDead();
        return;
    }
    
    //see what damages are done
    getStud()->flameDamages(this);
}

//ZOMBIE TIME YAY===========================================================
Zombie::Zombie(StudentWorld *stud, double locX, double locY)
:Actor(stud, locX, locY, IID_ZOMBIE, 2, right, 0, 1, true, false)
{
    m_planDistance = 0;
    setFlameCanDamage(true); //zombies can be affected by flames
    setActivation(); //can activate landmines
    setLivingActor();
}

Zombie::~Zombie()
{
}
void Zombie::setDead() {}

void Zombie::doSomething()
{
    //Step 1: check if alive
    if(isAlive() == 1) //not alive
    {
        return;
    }
    
    //Step 2: Paralyzed
    if(getStud()->getTicks() % 2 == 0) //even tick
    {
        return; //paralyzed
    }
    
    //Step 3: Direction + compute vomit coordinates
    //compute possible coordinates
    //check direction of Person
    Direction current = getDirection();
    double vomitX = 0.0;
    double vomitY = 0.0;
    newDirection(vomitX, vomitY, current, SPRITE_WIDTH);
    
    //create temp Vomit
    Vomit *temp = new Vomit(getStud(), vomitX, vomitY, getDirection());
    if(getStud()->overlapLiving(temp)) //there was overlap
    {
        //there will be a 1/3 chance that the zombie will vomit
        int vomitChance = randInt(1, 3);
        if(vomitChance == 1) //will only vomit if the random number is 1
        {
            getStud()->addActor(temp);
            getStud()->playSound(SOUND_ZOMBIE_VOMIT);
            return;
        }
        else
            delete temp;
    }
    else
        delete temp;
    
    
    //Step 4: Movement Plan (will be DIFFERENT for dumb and smart zombie)
    if(m_planDistance == 0)
        movementPlan(); //set new movement plan
    
    //Step 5: destination coordinate
    double tempX = 0.0;
    double tempY = 0.0;
    
    newDirection(tempX, tempY, getDirection(), 1);
    
    //Step 6: check bounding box and update location if applicable
    if(getStud()->open(this, tempX, tempY)) //FIX
    {
        moveTo(tempX, tempY);
        m_planDistance--;
    }
    //Step 7: can't actually move case
    else //bounding box is overlapped so need to pick new direction
        m_planDistance = 0;
    
}

//accessor
int Zombie::getPlanDistance() const
{
    return m_planDistance;
}

//mutator
void Zombie::decPlanDistance()
{
    m_planDistance--;
}

void Zombie::setPlanDistance(int set)
{
    m_planDistance = set;
}

void Zombie::movementPlan()
{
    //Note: to set direction, use GraphObject's setDirection
    return;
}

void Zombie::randDirection()
{
    int newDirection = randInt(1,4);
    if(newDirection == 1)
        setDirection(right);
    if(newDirection == 2)
        setDirection(left);
    if(newDirection == 3)
        setDirection(up);
    if(newDirection == 4)
        setDirection(down);
}

DumbZombie::DumbZombie(StudentWorld *stud,double locX, double locY)
:Zombie(stud, locX, locY)
{
}

DumbZombie::~DumbZombie()
{
    
}

void DumbZombie::movementPlan()
{
    
    int newMove = randInt(3, 10);
    setPlanDistance(newMove);
    randDirection();
}

void DumbZombie::setDead()
{
    Actor::setDead();
    getStud()->increaseScore(1000);
    getStud()->playSound(SOUND_ZOMBIE_DIE);
    
    //1 in 10 dumb zombies are mindlessly carrying a vaccine goodie
    int random = randInt(1, 10);
    
    if(random == 1) //has vaccine goodie
    {
        //choose a random direction
        randDirection();
        double tempX = 0.0;
        double tempY = 0.0;
        newDirection(tempX, tempY, getDirection(), SPRITE_WIDTH);
        
        //create a temp new vaccine goodie
        VaccineGoodie *temp = new VaccineGoodie(getStud(), tempX, tempY);
        
        //check if that spot is open
        if(getStud()->open(temp, tempX, tempY))
        {
            //open so will fling the vaccineGoodie
            getStud()->addActor(temp);
        }
        else
            delete temp;
    }
}

SmartZombie::SmartZombie(StudentWorld *stud, double locX, double locY)
:Zombie(stud, locX, locY)
{
    
}

SmartZombie::~SmartZombie()
{
    
}

void SmartZombie::movementPlan()
{
    //Select the person (Penelope or a citizen) closest to the smart zombie
    double personX = 0.0;
    double personY = 0.0;
    double distance = 0.0;
    getStud()->closestPersonToZombie(this->getX(), this->getY(), personX, personY, distance);
    
    
    if(distance >80)
    {
        //choose a random direction
        randDirection();
    }
    else
    {
        Direction tempDir = 0;
        //less than 80 pixels
        getStud()->newDirectionLess80(personX, personY, this->getX(), this->getY(), tempDir);
        
        double tempX = 0.0;
        double tempY = 0.0;
        
        newDirection(tempX, tempY, tempDir, 1.0); //increment by 1
        if(getStud()->open(this, tempX, tempY)) //open space
        {
            moveTo(tempX, tempY);
            decPlanDistance();
        }
        else
        {
            setPlanDistance(0); //have to set plan to 0 to restart
        }
    }
}

void SmartZombie::setDead()
{
    Actor::setDead();
    getStud()->increaseScore(2000);
    getStud()->playSound(SOUND_ZOMBIE_DIE);
}
