#include "Actor.h"
#include "StudentWorld.h"


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

void Actor::officiallyDead()
{
    m_status = 1;
}
void Actor::setfinishedLevelTrue()
{
    m_levelStatus = true;
}

StudentWorld *Actor::getStud() const
{
    return m_stud;
}

//=========PENELOPE
Penelope::Penelope(StudentWorld *stud, double locX, double locY)
:Actor(stud, locX, locY, IID_PLAYER, 2, right, 0, 1, true, false)
{
    m_infectStat = false; //infection status starts off as false
    m_infectCount = 0; //infection count is 0
    m_vaccines = 0;
    m_mines = 0;
    m_flames = 0;
    keyVal = 0;
    posNeg = 0;
    m_dir = right;
    setFlameCanDamage(true); //can be damaged by flames
}

Penelope::~Penelope()
{
    
}
//Penelope's Accessors
bool Penelope::getInfectStat() const
{
    return m_infectStat;
}
int Penelope::getInfectCount() const
{
    return m_infectCount;
}

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

//Penelope's Functions
void Penelope::doSomething()
{
    if(getInfectStat() == true) //if she's infected
    {
        //she's infected so increase infection count
        m_infectCount++;
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
                        Flames *newFlame = new Flames(getStud(), tempX, tempY, getDirection());
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
                    m_infectStat = false;
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
        //        if(getStartTick() == 0)
        //        {
        //            startTick(); //start counting the number of safety ticks
        //        }
        //landmine starts with 30 safety ticks
        //if between currentTick minus startTick is more than 30 ticks
        //        if(getStud()->getTicks() - getStartTick() - 1 > 30)
        //        {
        //            std::cout << "COUNTDOWN " << getStud()->getTicks() - getStartTick() << std::endl;
        //            //landmine becomes active
        //            m_inactiveState = false;
        //        }
        if(m_countdownTicks == 0)
        {
            std::cout << "here" << std::endl;
            m_inactiveState = false;
            
//            std::cout << "here1" << std::endl;
//            //setDead();
//            std::cout << "here2" << std::endl;
            return;
            //PROBLEM: when i uncomment setDead(), after deploying the landmine, everything freezes
        }
    }
    else if (!m_inactiveState)//now active landmine
    {
        std::cout << "STATUS " << isAlive() << std::endl;
        if(isAlive() != 1)
        {
            setDead();
             getStud()->playSound(SOUND_LANDMINE_EXPLODE);
            
            //this will just be where the landmine was- 1 flame
            getStud()->addActor(new Flames(getStud(), getX(), getY(), right));
            
            //this will introduce flame objects in the 8 adjacent slots
        }
    }
}

//void Landmines::setDead()
//{
//    //std::cout << "does it" << std::endl;
//    // Actor::setDead(); //set the status to dead
////     std::cout << "status1" << isAlive() << std::endl;
//    officiallyDead();
////    std::cout << "status" << isAlive() << std::endl;
//    getStud()->playSound(SOUND_LANDMINE_EXPLODE);
//   // std::cout << "GOT HERE " << std::endl;
//    //introduce a flame object at the same (x,y) location
//    getStud()->addActor(new Flames(getStud(), getX(), getY(), right));
//
//    //need to implement more
//}

Pit::Pit(StudentWorld *stud, double locX, double locY)
:BadThings(stud, locX, locY, IID_PIT, right)
{
    
}

void Pit::doSomething()
{
    
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
    // std::cout << "found new movement plan " << std::endl;
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

