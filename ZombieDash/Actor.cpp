#include "Actor.h"
#include "StudentWorld.h"


// Students:  Add code to this file, Actor.h, StudentWorld.h, and StudentWorld.cpp
//Actor base class implementation
Actor::Actor(StudentWorld *stud, double locX, double locY, int imgid, int statAlive, Direction dir, int depth, int size, bool canBlock)
:GraphObject(imgid, locX, locY, dir, depth, size)
{
    m_status = statAlive;
    m_stud = stud;
    m_canBlock = canBlock;
    m_levelStatus = false;
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

StudentWorld *Actor::getStud() const
{
    return m_stud;
}

//=========PENELOPE
Penelope::Penelope(StudentWorld *stud, double locX, double locY)
:Actor(stud, locX, locY, IID_PLAYER, 2, right, 0, 1, true)
{
    m_infectStat = false; //infection status starts off as false
    m_infectCount = 0; //infection count is 0
    m_vaccines = 0;
    m_mines = 0;
    m_flames = 0;
    keyVal = 0;
    posNeg = 0;
    m_dir = right;
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
                setDirection(up);
                x = getX();
                y = getY() + 4;
                if(isValid(x,y))
                    moveTo(x,y);
                break;
            case KEY_PRESS_DOWN:
                setDirection(down);
                x = getX();
                y = getY() - 4;
                if(isValid(x,y))
                    moveTo(x,y);
                break;
            case KEY_PRESS_LEFT:
                setDirection(left);
                x = getX() - 4;
                y = getY();
                if(isValid(x,y))
                    moveTo(x,y);
                break;
            case KEY_PRESS_RIGHT:
                setDirection(right);
                x = getX() + 4;
                y = getY();
                if(isValid(x,y))
                    moveTo(x,y);
                break;
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

//=========WAll
Wall::Wall(StudentWorld * stud, double locX, double locY)
:Actor(stud, locX, locY, IID_WALL, 0, right, 0, 1, true)
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
:Actor(stud, locX, locY, IID_EXIT, 0, right, 1, 1, false)
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
:Actor(stud, locX, locY, imgid, 2, right, 1, 1, false) //Goodies will start out alive
{
    
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
}
//Bad Things: aka Vomit, Pit, Flames
//Actor(StudentWorld *stud, double locX, double locY, int imgid, int statAlive, Direction dir, int depth, int size, bool canBlock);

BadThings::BadThings(StudentWorld *stud, double locX, double locY, int imgid)
:Actor(stud, locX, locY, imgid, 2, right, 0, 1, false)
{
     m_currentTick = 0;
}

void BadThings::doSomething()
{
    
}
void BadThings::startTick()
{
   // return m_currentTick;
    m_currentTick = (getStud())->getTicks();
}
int BadThings::getCurrentTick() const
{
    return m_currentTick;
}

Pit::Pit(StudentWorld *stud, double locX, double locY)
:BadThings(stud, locX, locY, IID_PIT)
{
    
}

void Pit::doSomething()
{
    
}

Vomit::Vomit(StudentWorld *stud, double locX, double locY)
:BadThings(stud, locX, locY, IID_VOMIT)
{
   
}

void Vomit::doSomething()
{
    if(isAlive() == 1) //vomit is already dead
        return;
    //set the currentTick if not already
    if(getCurrentTick() == 0)
        startTick();
    //check if there has been 2 ticks since creation
    if(getStud()->getTicks() - getCurrentTick() == 2)
    {
        //set state to dead
        setDead();
        return;
    }
        
}

Flames::Flames(StudentWorld *stud, double locX, double locY)
:BadThings(stud, locX, locY, IID_FLAME)
{
    
}

void Flames::doSomething()
{
    if(isAlive() == 1) //flame is already dead
        return;
    //set the currentTick if not already
    if(getCurrentTick() == 0)
        startTick();
    //check if there has been 2 ticks since creation
    if(getStud()->getTicks() - getCurrentTick() == 2)
    {
        //set state to dead
        setDead();
        return;
    }
}

//ZOMBIE TIME YAY===========================================================
//Actor(StudentWorld *stud, int locX, int locY, int imgid, int statAlive, Direction dir, int depth, int size, bool canBlock);
Zombie::Zombie(StudentWorld *stud, double locX, double locY)
:Actor(stud, locX, locY, IID_ZOMBIE, 2, right, 0, 1, true)
{
    m_planDistance = 0;
}

Zombie::~Zombie()
{
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

