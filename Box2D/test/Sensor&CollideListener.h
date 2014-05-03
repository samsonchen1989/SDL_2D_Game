#ifndef DUMPLING_TEST_H
#define DUMPLING_TEST_H

#include <iostream>
#include <vector>

#define DEGTORAD 0.0174532925199432957f
#define RADTODEG 57.295779513082320876f

enum _moveState
{
    MS_STOP,
    MS_LEFT,
    MS_RIGHT,
};

enum _entityCategory
{
    BOUNDARY =          0x0001,
    FRIENDLY_SHIP =     0x0002,
    ENEMY_SHIP =        0x0004,
    FRIENDLY_AIRCRAFT = 0x0008,
    ENEMY_AIRCRAFT =    0x0010,
    FRIENDLY_TOWER =    0x0020,
    RADAR_SENSOR =      0x0040,
};

class Ball
{
public:
    Ball(b2World* world, float radius, b2Color color, uint16 categoryBits, uint16 maskBits)
    {
        m_body = NULL;
        m_radius = radius;
        m_numContacts = 0;
        m_color = color;

        //set up dynamic body
        b2BodyDef myBodyDef;
        myBodyDef.type = b2_dynamicBody;
        myBodyDef.position.Set(0, 20);
        m_body = world->CreateBody(&myBodyDef);

        //add circle fixture
        b2CircleShape circleShape;
        circleShape.m_p.Set(0, 0);
        circleShape.m_radius = m_radius;

        b2FixtureDef myFixtureDef;
        myFixtureDef.shape = &circleShape;
        myFixtureDef.density = 1;

        myFixtureDef.filter.categoryBits = categoryBits;
        myFixtureDef.filter.maskBits = maskBits;

        m_body->CreateFixture(&myFixtureDef);

        m_body->SetUserData(this);
    }

    ~Ball() {}

    void render()
    {
        if (visibleEnemies.size() > 0) {
            glColor3f(1, 1, 0);
        } else {
            glColor3f(m_color.r, m_color.g, m_color.b);
        }

        //nose and eyes
        glPointSize(4);
        glBegin(GL_POINTS);
        glVertex2f( 0, 0 );
        glVertex2f(-0.5, 0.5 );
        glVertex2f( 0.5, 0.5 );
        glEnd();

        //mouth
        glBegin(GL_LINES);
        glVertex2f(-0.5,  -0.5 );
        glVertex2f(-0.16, -0.6 );
        glVertex2f( 0.16, -0.6 );
        glVertex2f( 0.5,  -0.5 );
        glEnd();

        //circle outline
        glBegin(GL_LINE_LOOP);
        for (float a = 0; a < 360 * DEGTORAD; a += 30 * DEGTORAD) 
            glVertex2f( sinf(a), cosf(a) );
        glEnd();
    }

    void renderAtBodyPosition()
    {
        b2Vec2 pos = m_body->GetPosition();
        float angle = m_body->GetAngle();

        glPushMatrix();
        glTranslatef(pos.x, pos.y, 0);
        glRotatef(angle * RADTODEG, 0, 0, 1);
        glScalef(m_radius, m_radius, 1);
        render();
        glPopMatrix();

        glColor3f(1,1,1);//white
        glLineStipple( 1, 0xF0F0 ); //evenly dashed line
        glEnable(GL_LINE_STIPPLE); 
        glBegin(GL_LINES);
        for (int i = 0; i < visibleEnemies.size(); i++) {
            b2Vec2 enemyPosition = visibleEnemies[i]->m_body->GetPosition();
            glVertex2f(pos.x, pos.y);
            glVertex2f(enemyPosition.x, enemyPosition.y);
        }
        glEnd();
        glDisable(GL_LINE_STIPPLE);
    }

    void startContact() { m_numContacts++; }
    void endContact() { m_numContacts--; }

    void raderAcquiredEnemy(Ball* enemy) {
        visibleEnemies.push_back(enemy);
    }

    void raderLostEnemy(Ball* enemy) {
        visibleEnemies.erase(std::find(visibleEnemies.begin(), visibleEnemies.end(), enemy));
    }

    //class member variables
    b2Body* m_body;
    float m_radius;
    int m_numContacts;
    b2Color m_color;

    std::vector<Ball*> visibleEnemies;
};

class MyContactListener : public b2ContactListener
{
    bool getRaderAndAircraft(b2Contact* contact, Ball*& raderEntity, Ball*& aircraftEntity)
    {
        b2Fixture* fixtureA = contact->GetFixtureA();
        b2Fixture* fixtureB = contact->GetFixtureB();

        //make sure only one of the fixture was a sensor
        bool sensorA = fixtureA->IsSensor();
        bool sensorB = fixtureB->IsSensor();
        if (!(sensorB ^ sensorA)) {
            return false;
        }

        Ball* entityA = static_cast<Ball*>(fixtureA->GetBody()->GetUserData());
        Ball* entityB = static_cast<Ball*>(fixtureB->GetBody()->GetUserData());

        if (sensorA) {
            raderEntity = entityA;
            aircraftEntity = entityB;
        } else {
            raderEntity = entityB;
            aircraftEntity = entityA;
        }

        return true;
    }

    void BeginContact(b2Contact* contact) {
        Ball* radarEntity;
        Ball* aircraftEntity;
        if (getRaderAndAircraft(contact, radarEntity, aircraftEntity)) {
            radarEntity->raderAcquiredEnemy(aircraftEntity);
        }
    }

    void EndContact(b2Contact* contact) {
        Ball* radarEntity;
        Ball* aircraftEntity;
        if (getRaderAndAircraft(contact, radarEntity, aircraftEntity)) {
            radarEntity->raderLostEnemy(aircraftEntity);
        }
    }
};

class Dumpling : public Test
{
public:
    Dumpling()
    {
        m_world->SetContactListener(&myContactListenerInstance);
        m_world->SetGravity(b2Vec2(0, 0));

        b2BodyDef myBodyDef;
        myBodyDef.type = b2_dynamicBody;

        b2PolygonShape polygonShape;
        polygonShape.SetAsBox(1, 1);

        b2FixtureDef myFixtureDef;
        myFixtureDef.shape = &polygonShape;
        myFixtureDef.density = 1;

        myBodyDef.position.Set(0, 5);
        body = m_world->CreateBody(&myBodyDef);
        body->CreateFixture(&myFixtureDef);

        //a static body
        myBodyDef.type = b2_staticBody;
        myBodyDef.position.Set(0, 0);
        b2Body* staticBody = m_world->CreateBody(&myBodyDef);

        //add four walls to the static body
        polygonShape.SetAsBox(20, 1, b2Vec2(0, 0), 0);
        staticBody->CreateFixture(&myFixtureDef);
        polygonShape.SetAsBox(20, 1, b2Vec2(0,40), 0);
        staticBody->CreateFixture(&myFixtureDef);
        polygonShape.SetAsBox(1, 20, b2Vec2(-20, 20), 0);
        staticBody->CreateFixture(&myFixtureDef);
        polygonShape.SetAsBox(1, 20, b2Vec2(20, 20), 0);
        staticBody->CreateFixture(&myFixtureDef);

        moveState = MS_STOP;

        b2Color red(1, 0, 0);
        b2Color green(0, 1, 0);

        Ball* ship = new Ball(m_world, 3, green, FRIENDLY_SHIP, BOUNDARY | FRIENDLY_TOWER);
        m_Balls.push_back(ship);

        b2CircleShape circleShape;
        circleShape.m_radius = 8;
        myFixtureDef.shape = &circleShape;
        myFixtureDef.isSensor = true;
        myFixtureDef.filter.categoryBits = RADAR_SENSOR;
        myFixtureDef.filter.maskBits = ENEMY_AIRCRAFT;
        ship->m_body->CreateFixture(&myFixtureDef);

        //small and red are enemy aircraft
        for (int i = 0; i < 3; i++) {
            m_Balls.push_back(new Ball(m_world, 1, red, ENEMY_AIRCRAFT, BOUNDARY | RADAR_SENSOR));
        }

        Ball* tower = new Ball(m_world, 1, green, ENEMY_AIRCRAFT, BOUNDARY | RADAR_SENSOR);
        tower->m_body->SetType(b2_kinematicBody);

        //add semicircle radar sensor to tower
        float radius = 8;
        b2Vec2 vertices[8];
        vertices[0].Set(0,0);
        for (int i = 0; i < 7; i++) {
            float angle = i / 6.0 * 90 * DEGTORAD;
            vertices[i+1].Set( radius * cosf(angle), radius * sinf(angle) );
        }
        polygonShape.Set(vertices, 8);
        myFixtureDef.shape = &polygonShape;
        tower->m_body->CreateFixture(&myFixtureDef);

        //make the tower rotate at 45 degrees per second
        tower->m_body->SetAngularVelocity(45 * DEGTORAD);

        m_Balls.push_back(tower);
    }

    void Step(Settings* settings)
    {
        //run default physics and rendering
        Test::Step(settings);

        for (int i = 0; i < m_Balls.size(); i++) {
            m_Balls[i]->renderAtBodyPosition();
        }
    }

    void Keyboard(unsigned char key)
    {
        switch(key)
        {
        case 'q':
            moveState = MS_LEFT;
            break;
        case 'e':
            moveState = MS_RIGHT;
            break;
        case 'j':
            {
                float impluse = body->GetMass() * 10;
                body->ApplyLinearImpulse(b2Vec2(0, impluse), body->GetWorldCenter(), true);

                break;
            }
        default:
            //run default behaviour
            Test::Keyboard(key);
            break;
        }
    }

    void MouseDown(const b2Vec2& p)
    {
        Test::MouseDown(p);
    }

    static Test* Create()
    {
        return new Dumpling();
    }

protected:
    b2Body* body;
    std::vector<Ball*> m_Balls;

    _moveState moveState;

    MyContactListener myContactListenerInstance;
};

#endif