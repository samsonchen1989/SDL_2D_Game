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

class Ball
{
public:
    Ball(b2World* world, float radius)
    {
        m_body = NULL;
        m_radius = radius;

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
        m_body->CreateFixture(&myFixtureDef);
    }

    ~Ball() {}

    void render()
    {
        b2Vec2 vel = m_body->GetLinearVelocity();
        float red = vel.Length() / 20.0;
        red = b2Min(1.0f, red);
        glColor3f(red,0.5,0.5);//white

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
    }

    //class member variables
    b2Body* m_body;
    float m_radius;
};

class Dumpling : public Test
{
public:
    Dumpling()
    {
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

        for (int i = 0; i < 20; i++) {
            float radius = 1 + 2 * (rand() / (float)RAND_MAX);
            Ball* ball = new Ball(m_world, radius);
            m_Balls.push_back(ball);
        }
    }

    void Step(Settings* settings)
    {
        //run default physics and rendering
        Test::Step(settings);

        b2Vec2 vel = body->GetLinearVelocity();

        switch (moveState)
        {
        case MS_LEFT:
            vel.x = b2Max(vel.x - 0.2f, -5.0f);
            break;
        case MS_RIGHT:
            vel.x = b2Min(vel.x + 0.2f, 5.0f);
            break;
        case MS_STOP:
            vel.x *= 0.9;
            break;
        }

        body->SetLinearVelocity(vel);

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
};

#endif