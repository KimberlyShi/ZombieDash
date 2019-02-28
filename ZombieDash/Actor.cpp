#include "Actor.h"
#include "StudentWorld.h"
#include <math.h>


// Students:  Add code to this file, Actor.h, StudentWorld.h, and StudentWorld.cpp
//Actor base class implementation
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
};
Actor::~Actor()
{
    
}

void Actor::doSomething()
{
    return;
}


double Actor::getSpriteWidth() const
{
    return SPRITE_WIDTH;
}

double Actor::getSpriteHeight() const
{
    return SPRITE_HEIGHT;
}

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

//=========PENELOPE
Human::Human(StudentWorld *stud, double locX, double locY, int imgid)
:Actor(stud, locX, locY, imgid,2,right, 0, 1, true, false)
{
    m_infectStat = false; //infection status starts off as false
    m_infectCount = 0; //infection count is 0
    canExitTrue();
    setFlameCanDamage(true); //can be damaged by flames
    setActivation(); //can activate landmines
    setLivingActor();
}

bool Human::getInfectStat() const
{
    return m_infectStat;
}
int Human::getInfectCount() const
{
    return m_infectCount;
}

void Human::increaseInfectCount()
{
    m_infectCount++;
}

void Human::setInfectStat(bool val)
{
    m_infectStat = val;
}
//CITIZEN=====
Citizen::Citizen(StudentWorld *stud, double locX, double locY)
:Human(stud, locX, locY, IID_CITIZEN)
{
    
}

bool Citizen::tempPlace(double &tempX, double &tempY, Direction tempDir)
{
    switch(tempDir)
    {
        case right:
        {
            tempX += 2;
            break;
        }
        case left:
        {
            tempX -= 2;
            break;
        }
        case up:
        {
            tempY += 2;
            break;
        }
        case down:
        {
            tempY -= 2;
            break;
        }
        default:
            break;
            
    }
    
    //check if those temp will actually overlap with anything
    if(getStud()->open(this, tempX, tempY)) //the spot is open
    {
        setDirection(tempDir);
        moveTo(tempX, tempY);
        return true;
    }
    return false;
}
bool Citizen::citizenZombie(double dist_z)
{
    //dist_z is the distance between the citizen and zombie
    //check if the distance is less than or equal to 80 pixels
    if(dist_z <= 80)
    {
        //rightD for example indicates distance for the respective direction
        double rightD = -1.0;
        double rx = 0.0; //coordinates of the closest zombie
        double ry = 0.0;
        double leftD = -1.0;
        double upD = -1.0;
        double downD = -1.0;
        Direction newDirection = right; //just initialize to right but that's just temporary
        //check distance for each of them
        if(getStud()->open(this, getX() + 2, getY()))//right
           getStud()->closestZombieToCitizen(getX() + 2,getY(), rx, ry, rightD);
        
        if(getStud()->open(this, getX() - 2, getY()))//left
            getStud()->closestZombieToCitizen(getX() - 2 ,getY(), rx, ry, leftD);
        
        if(getStud()->open(this, getX(), getY() + 2))//up
            getStud()->closestZombieToCitizen(getX(),getY() + 2, rx, ry, upD);
        
        if(getStud()->open(this, getX(), getY() - 2))//down
            getStud()->closestZombieToCitizen(getX(),getY() - 2, rx, ry, downD);
        
        double newDist = 0.0;
        if(rightD != -1.0 && rightD < newDist && rightD < dist_z)
        { newDist = rightD;
            newDirection = right;
        }
        if (leftD != -1.0 && leftD < newDist && leftD < dist_z)
        {newDist = leftD;
            newDirection = left;
        }
        if (upD != -1.0 && upD < newDist && upD < dist_z)
        {
        newDist = upD;
            newDirection = up;
        }
        if (downD != -1.0 && downD < newDist && downD < dist_z)
        {newDist = downD;
            newDirection = down;
        }
        if(newDist == 0.0) //no movement will be farther
            return false;
        
        setDirection(newDirection); //set the citizen to a new direction
        //move citizen in that direction
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
            std::cout << "NOT " << std::endl;
            //becomes a zombie
            setDead(); //set status to dead
            (getStud()) -> playSound(SOUND_ZOMBIE_BORN); //play sound
            getStud()->increaseScore(-1000); //decrease player score
            
            int probability = randInt(1, 10);
            if(probability >= 1 && probability <=7) //70% chance become dumb
                getStud()->addActor(new DumbZombie(getStud(), getX(), getY()));
            else //will either be 8,9,10 (30% probability)
               getStud()->addActor(new SmartZombie(getStud(), getX(), getY()));
            return;
        }
    
        return; //??? NOT SURE IF THIS IS CORRECT?????
    }
    
    if(getStud()->getTicks() % 2 == 0) //even tick
    {
        return; //paralyzed
    }
    
    //calculate distance to penelope
    //std::cout << "here " << std::endl;
    double xPenelopeDis = (getStud()->getPenelope())->getX() -getX();
    double yPenelopeDis = (getStud()->getPenelope())->getY() -getY();
    double dist_pSquared = (xPenelopeDis * xPenelopeDis) + (yPenelopeDis * yPenelopeDis);
    double dist_p = sqrt(dist_pSquared); //sqrt is a function imported from math.h
    
   // std::cout << "distance to penelope " << dist_p << std::endl;
    double dist_z = 256.0;
    double zombieX = 0.0;
    double zombieY = 0.0;
    double xPen = (getStud()->getPenelope())->getX();
    double yPen = (getStud()->getPenelope())->getY();
    double tempX = getX();
    double tempY = getY();
    
//    getStud()->closestZombieToCitizen(this, zombieX, zombieY, dist_z);
    getStud()->closestZombieToCitizen(getX(), getY(), zombieX, zombieY, dist_z);
    //std::cout << "distance to zombie " << dist_z << std::endl;
    if(dist_p < dist_z || dist_z == 256) //no zombies on that level
    {
        //check Euclidean distance from penelope to citizen
        //citizen wants to follow penelope
        if(getStud()->overlapCitizenPenelope(this, getStud()->getPenelope()))
        {
            //check if in the same row or column as penelope
            if(getX() == (getStud()->getPenelope())->getX() ||
               getY() == (getStud()->getPenelope())->getY())
            {
                
//                double xPen = (getStud()->getPenelope())->getX();
//                double yPen = (getStud()->getPenelope())->getY();
                
                Direction tempDir = 0;
                //determine the direction towards penelope
                if(xPen - getX() < 0) //penelope on citizen's left
                {
                    tempDir = left;
                }
                else if (xPen - getX() > 0)
                {
                    tempDir = right;
                }
                else if(yPen - getY() < 0) //penelope below citizen
                {
                    tempDir = down;
                }
                else
                {
                    tempDir = up;
                }
                
                //check if can move 2 pixels in the direction toward penelope
//                double tempX = getX();
//                double tempY = getY();
                
               // tempPlace(tempX, tempY, tempDir); //will calculate 2 pixels
                
//                //check if those temp will actually overlap with anything
//                if(getStud()->open(this, tempX, tempY)) //the spot is open
//                {
//                    setDirection(tempDir);
//                    moveTo(tempX, tempY);
//                    return;
//                }
                if(tempPlace(tempX, tempY, tempDir)) //will calculate 2 pixels
                { //was able to move
                    return;
                }
                else //citizen would be blocked by something
                {
                    //STEP 7!!!!!
                }
            }
            
            //citizen is not on the same row or column
            else
            {
                //determine direction closest
                Direction tempXDir = 0;
                Direction tempYDir = 0;
                if(xPen - getX() < 0) //penelope on citizen's left
                {
                    tempXDir = left;
                }
                if (xPen - getX() > 0)
                {
                    tempXDir = right;
                }
               if(yPen - getY() < 0) //penelope below citizen
                {
                    tempYDir = down;
                }
                 if(yPen - getY() > 0)
                {
                    tempYDir = up;
                }
                
                int choose = randInt(1, 2); //1 will be direction tempX
                //2 will be direction tempY
                if(choose == 1)
                {
                    //tempX
                    if(tempPlace(tempX, tempY, tempXDir))
                        return;
                    //flip the direction
                    if(tempXDir == right)
                        tempXDir = left;
                    else
                        tempXDir = right;
                    
                    //try to move in the other direction
                    if(tempPlace(tempX, tempY, tempXDir))
                        return;
                    
                    //if both directions don't work, then will go to step 7
                    //STEP 7!!!!
                }
                if(choose == 2)
                {
                    //tempY
                     if(tempPlace(tempX, tempY, tempYDir))
                         return;
                    
                    if(tempYDir == up)
                        tempYDir = down;
                    else
                        tempYDir = up;
                    
                    //try to move in the other direction
                    if(tempPlace(tempX, tempY, tempYDir))
                        return;
                    
                    //FIX: need the alternate
                    //STEP 7!!!!
                }
                
                
                
            }
        }
    }
}

void Citizen::setDead()
{
    Actor::setDead();
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
        return; //??? NOT SURE IF THIS IS CORRECT?????
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
                        
                        if(getDirection() == up)
                        {
                            tempX = getX();
                            tempY = getY() + count * SPRITE_HEIGHT;
                        }
                        if(getDirection() == down)
                        {
                            tempX = getX();
                            tempY = getY() - count * SPRITE_HEIGHT;
                        }
                        if(getDirection() == left)
                        {
                            tempX = getX() - count *SPRITE_WIDTH;
                            tempY = getY();
                        }
                        if(getDirection() == right)
                        {
                            tempX = getX() + count *SPRITE_WIDTH;
                            tempY = getY();
                        }
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
    //determine overlap with Penelope
    //FIXXXXXXXX can only exit if citizens are all exited or are dead
    //FOR NOW: determine overlap
    
    //see if penelope and exit overlap
    if((getStud())->overlap(this, getStud()->getPenelope()))
    {
        setfinishedLevelTrue();
        return;
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

//void Goodies::setDead()
//{
//    Actor::setDead(); //set status to dead
//}

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
//Actor(StudentWorld *stud, int locX, int locY, int imgid, int statAlive, Direction dir, int depth, int size, bool canBlock);
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
void Zombie::setDead()
{
    //need to implement
}

void Zombie::doSomething()
{
    
    //PROBLEM: zombies are moving super slow
    
    
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
    
    double tempVomitX = vomitX();
    double tempVomitY = vomitY();
    
    //Step 3: Direction + compute vomit coordinates
    //Step 4: Movement Plan (will be DIFFERENT for dumb and smart zombie)
    if(m_planDistance == 0)
        movementPlan(); //set new movement plan
    
    //Step 5: destination coordinate
    double tempX = getX();
    double tempY = getY();
    if(getDirection() == right)
        tempX += 1; //add a pixel
    if(getDirection() == left)
        tempX -= 1;
    if(getDirection() == up)
        tempY += 1;
    if(getDirection() == down)
        tempY -= 1;
    
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
double Zombie::vomitX()
{
    double currentX = getX();
    double newX = 0.0;
    Direction temp = getDirection();
    switch(temp)
    {
        case right:
            newX = currentX + SPRITE_WIDTH;
        case left:
            newX = currentX - SPRITE_WIDTH;
        default:
            break;
    }
    return newX;
}


double Zombie::vomitY()
{
    double currentY = getY();
    double newY = 0.0;
    Direction temp = getDirection();
    switch(temp)
    {
        case down:
            newY = currentY + SPRITE_HEIGHT;
        case up:
            newY = currentY - SPRITE_HEIGHT;
        default:
            break;
    }
    return newY;
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


SmartZombie::SmartZombie(StudentWorld *stud, double locX, double locY)
:Zombie(stud, locX, locY)
{
    
}

SmartZombie::~SmartZombie()
{
    
}

void SmartZombie::movementPlan()
{
    
}

